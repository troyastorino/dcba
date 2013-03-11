#include "surf.h"
#include "extra.h"
using namespace std;

namespace
{

    // We're only implenting swept surfaces where the profile curve is
    // flat on the xy-plane.  This is a check function.
    static bool checkFlat(const Curve &profile)
    {
        for (unsigned i=0; i<profile.size(); i++)
            if (profile[i].V[2] != 0.0 ||
                profile[i].T[2] != 0.0 ||
                profile[i].N[2] != 0.0)
                return false;

        return true;
    }
}

Surface makeSurfRev(const Curve &profile, unsigned steps)
{
    if (!checkFlat(profile))
    {
        cerr << "surfRev profile curve must be flat on xy plane." << endl;
        exit(0);
    }

    unsigned i;
    unsigned j;
    Surface surface;

    // rotate a copy repeatedly to create all the points on the surface
    for (i = 0; i < steps; i++) {
        float angle = 2.0f * M_PI * float(i) / steps;
        Matrix4f rot = Matrix4f::rotation(Vector3f(0.0, 1.0, 0.0), angle);
        Matrix3f normRot = Matrix3f(rot.getCol(0).xyz(), rot.getCol(1).xyz(), rot.getCol(2).xyz(), true);
        normRot = normRot.inverse();
        normRot.transpose();
        for (j = 0; j < profile.size(); j++) {
            // rotate and copy each point
            Vector4f fourPoint = Vector4f(profile[j].V, 1.0);
            Vector3f threePoint = (rot * fourPoint).xyz();
            surface.VV.push_back(threePoint);

            // rotate and copy each normals
            Vector3f normal = -1.0 * (normRot * profile[j].N);
            normal.normalize();
            surface.VN.push_back(normal);
        }
    }

    surface.VF = makeTriangularMesh(profile.size(), surface.VV.size(), steps, false);

    cerr << "\t>>> makeSurfRev called." << endl;

    return surface;
}

std::vector<Tup3u> makeTriangularMesh(unsigned pSize, unsigned sSize, unsigned steps, bool connect) {
    std::vector<Tup3u> result;
    unsigned k;
    unsigned l;
    unsigned end;
    if (connect) {
        end = pSize;
    }
    else {
        end = pSize-1;
    }
    // figure out which sets of points are surfaces and record them
    for (k = 0; k < steps; k++) {
        for (l = 0; l < end; l++) {
            unsigned start = l + pSize * k;
            Tup3u triangleOne(start, (start+pSize+1)%sSize, (start+pSize)%sSize);
            Tup3u triangleTwo(start, start+1, (start+pSize+1)%sSize);
            result.push_back(triangleOne);
            result.push_back(triangleTwo);
        }
    }
    return result;
}

Surface makeGenCyl(const Curve &profile, const Curve &sweep )
{
    Surface surface;
    unsigned i;
    unsigned j;

    if (!checkFlat(profile))
    {
        cerr << "genCyl profile curve must be flat on xy plane." << endl;
        exit(0);
    }

    // rotate a copy repeatedly to create all the points on the surface
    for (i = 0; i < sweep.size(); i++) {
        // our basis transform has z as T, x as N, and y as B. The origin is V
        Matrix4f basis = Matrix4f(Vector4f(sweep[i].N, 0.0)
                , Vector4f(sweep[i].B, 0.0)
                , Vector4f(sweep[i].T, 0.0)
                , Vector4f(sweep[i].V, 1.0), true);
        Matrix3f normRot = Matrix3f(basis.getCol(0).xyz(), basis.getCol(1).xyz(), basis.getCol(2).xyz(), true);
        normRot = normRot.inverse();
        normRot.transpose();
        for (j = 0; j < profile.size(); j++) {
            // rotate and copy each point
            Vector4f fourPoint = Vector4f(profile[j].V, 1.0);
            Vector3f threePoint = (basis * fourPoint).xyz();
            surface.VV.push_back(threePoint);

            // transform and copy each normals
            Vector3f normal = -1.0 * (normRot * profile[j].N);
            normal.normalize();
            surface.VN.push_back(normal);
        }
    }

    surface.VF = makeTriangularMesh(profile.size(), surface.VV.size(), sweep.size(), true);

    cerr << "\t>>> makeGenCyl called." << endl;

    return surface;
}

void drawSurface(const Surface &surface, bool shaded)
{
    // Save current state of OpenGL
    glPushAttrib(GL_ALL_ATTRIB_BITS);

    if (shaded)
    {
        // This will use the current material color and light
        // positions.  Just set these in drawScene();
        glEnable(GL_LIGHTING);
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

        // This tells openGL to *not* draw backwards-facing triangles.
        // This is more efficient, and in addition it will help you
        // make sure that your triangles are drawn in the right order.
        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);
    }
    else
    {
        glDisable(GL_LIGHTING);
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

        glColor4f(0.4f,0.4f,0.4f,1.f);
        glLineWidth(1);
    }

    glBegin(GL_TRIANGLES);
    for (unsigned i=0; i<surface.VF.size(); i++)
    {
        glNormal(surface.VN[surface.VF[i][0]]);
        glVertex(surface.VV[surface.VF[i][0]]);
        glNormal(surface.VN[surface.VF[i][1]]);
        glVertex(surface.VV[surface.VF[i][1]]);
        glNormal(surface.VN[surface.VF[i][2]]);
        glVertex(surface.VV[surface.VF[i][2]]);
    }
    glEnd();

    glPopAttrib();
}

void drawNormals(const Surface &surface, float len)
{
    // Save current state of OpenGL
    glPushAttrib(GL_ALL_ATTRIB_BITS);

    glDisable(GL_LIGHTING);
    glColor4f(0,1,1,1);
    glLineWidth(1);

    glBegin(GL_LINES);
    for (unsigned i=0; i<surface.VV.size(); i++)
    {
        glVertex(surface.VV[i]);
        glVertex(surface.VV[i] + surface.VN[i] * len);
    }
    glEnd();

    glPopAttrib();
}

void outputObjFile(ostream &out, const Surface &surface)
{
    
    for (unsigned i=0; i<surface.VV.size(); i++)
        out << "v  "
            << surface.VV[i][0] << " "
            << surface.VV[i][1] << " "
            << surface.VV[i][2] << endl;

    for (unsigned i=0; i<surface.VN.size(); i++)
        out << "vn "
            << surface.VN[i][0] << " "
            << surface.VN[i][1] << " "
            << surface.VN[i][2] << endl;

    out << "vt  0 0 0" << endl;
    
    for (unsigned i=0; i<surface.VF.size(); i++)
    {
        out << "f  ";
        for (unsigned j=0; j<3; j++)
        {
            unsigned a = surface.VF[i][j]+1;
            out << a << "/" << "1" << "/" << a << " ";
        }
        out << endl;
    }
}
