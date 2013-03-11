#include "curve.h"
#include "extra.h"
#ifdef WIN32
#include <windows.h>
#endif
#include <GL/gl.h>
using namespace std;

namespace
{
    // Approximately equal to.  We don't want to use == because of
    // precision issues with floating point.
    inline bool approx( const Vector3f& lhs, const Vector3f& rhs )
    {
        const float eps = 1e-8f;
        return ( lhs - rhs ).absSquared() < eps;
    }

    Matrix4f bezier = Matrix4f(Vector4f(1.0, -3.0, 3.0, -1.0)
            , Vector4f(0.0, 3.0, -6.0, 3.0)
            , Vector4f(0.0, 0.0, 3.0, -3.0)
            , Vector4f(0.0, 0.0, 0.0, 1.0), false);

    Matrix4f bspline = (1.0/6.0) * Matrix4f(Vector4f(1.0, 4.0, 1.0, 0.0)
            , Vector4f(-3.0, 0.0, 3.0, 0.0)
            , Vector4f(3.0, -6.0, 3.0, 0.0)
            , Vector4f(-1.0, 3.0, -3.0, 1.0), true);
    Vector3f noVector = Vector3f(-10000, 0, 0);
}

Curve evalBezier( const vector< Vector3f >& P, unsigned steps ) {
    return evalBezier(P, steps, noVector);
}

Curve evalBezier( const vector< Vector3f >& P, unsigned steps, Vector3f binormal)
{
    // Check
    if( P.size() < 4 || P.size() % 3 != 1 )
    {
        cerr << "evalBezier must be called with 3n+1 control points." << endl;
        exit( 0 );
    }

    unsigned i;
    unsigned j;
    unsigned k;
    unsigned sections = (P.size() - 1) / 3;
    Curve curve;

    // TODO:
    // You should implement this function so that it returns a Curve
    // (e.g., a vector< CurvePoint >).  The variable "steps" tells you
    // the number of points to generate on each piece of the spline.
    // At least, that's how the sample solution is implemented and how
    // the SWP files are written.  But you are free to interpret this
    // variable however you want, so long as you can control the
    // "resolution" of the discretized spline curve with it.

    // Make sure that this function computes all the appropriate
    // Vector3fs for each CurvePoint: V,T,N,B.
    // [NBT] should be unit and orthogonal.

    // Also note that you may assume that all Bezier curves that you
    // receive have G1 continuity.  Otherwise, the TNB will not be
    // be defined at points where this does not hold.

    cerr << "\t>>> evalBezier has been called with the following input:" << endl;

    cerr << "\t>>> Control points (type vector< Vector3f >): "<< endl;

    for(i = 0; i < P.size(); ++i ) {
        cerr << "\t>>> " << P[i] << endl;
    }

    // loop over this method once for each set of four points
    for (j = 0; j < sections; j++) {
        // find the points matrix for this one
        Vector3f point1 = P[j*3];
        Vector3f point2 = P[j*3 + 1];
        Vector3f point3 = P[j*3 + 2];
        Vector3f point4 = P[j*3 + 3];

        Matrix4f points;

        points.setCol(0, Vector4f(point1, 1.0));
        points.setCol(1, Vector4f(point2, 1.0));
        points.setCol(2, Vector4f(point3, 1.0));
        points.setCol(3, Vector4f(point4, 1.0));

        for(k = 0; k < steps; k++) {
            // calculate time at this step
            float time = float(k) / steps;
            // create a vector for the t matrix
            Vector4f basis = Vector4f(1.0, time, time*time, time*time*time);
            // multiply the bezier conversion matrix by this
            Vector4f convertedBasis = bezier * basis;
            // multiply this by the points to find the coordinates.
            Vector3f point = (points * convertedBasis).xyz();

            CurvePoint p;
            p.V = point;

            // next, we need to tangent

            // create a vector for the derivative
            Vector4f basisPrime = Vector4f(0.0, 1.0, 2.0*time, 3.0*time*time);
            // multiply by the bezier conversion
            Vector4f convertedBasisPrime = bezier * basisPrime;
            // multiply this by the points to find tangent vector
            Vector3f tangent = (points * convertedBasisPrime).xyz();

            p.T = tangent.normalized();

            // next, the normals

            if (k == 0 && j == 0) {
                // if this is the first set of normals being calculated, make sure we have a seed binormal and it is not parallel to the tangent
                if (binormal == noVector) {
                    binormal = Vector3f(0.0, 0.0, 1.0);
                }
                if (tangent.dot(tangent, binormal) == 1.0) {
                    cerr << "BINORMAL PARALLEL" << endl;
                    binormal = Vector3f(0.0, 1.0, 0.0);
                }
            }

            Vector3f normal = binormal.cross(binormal, tangent).normalized();
            p.N = normal;
            binormal = tangent.cross(tangent, normal).normalized();
            p.B = binormal;
            curve.push_back(p);
        }
    }

    cerr << "\t>>> Steps (type steps): " << steps << endl;

    // Right now this will just return this empty curve.
    return curve;
}

Curve evalBspline( const vector< Vector3f >& P, unsigned steps )
{
    unsigned i;
    unsigned j;
    unsigned k;
    unsigned sections = P.size() - 3;
    Matrix4f conversion = bspline * bezier.inverse(NULL, 0.00001);
    if( P.size() < 4)
    {
        cerr << "evalBspline must be called with 4 or more control points." << endl;
        exit( 0 );
    }

    // TODO:
    // It is suggested that you implement this function by changing
    // basis from B-spline to Bezier.  That way, you can just call
    // your evalBezier function.

    cerr << "\t>>> evalBSpline has been called with the following input:" << endl;

    cerr << "\t>>> Control points (type vector< Vector3f >): "<< endl;

    for(i = 0; i < P.size(); ++i ) {
        cerr << "\t>>> " << P[i] << endl;
    }

    Curve curve;

    for (j = 0; j < sections; j++) {
        // convert this set of points to bernstein space, then call bezier
        // find the points matrix for this one
        Vector3f point1 = P[j];
        Vector3f point2 = P[j + 1];
        Vector3f point3 = P[j + 2];
        Vector3f point4 = P[j + 3];

        Matrix4f points;

        points.setCol(0, Vector4f(point1, 1.0));
        points.setCol(1, Vector4f(point2, 1.0));
        points.setCol(2, Vector4f(point3, 1.0));
        points.setCol(3, Vector4f(point4, 1.0));

        points = points * conversion;

        vector<Vector3f> controlPoints(4);

        controlPoints[0] = points.getCol(0).xyz();
        controlPoints[1] = points.getCol(1).xyz();
        controlPoints[2] = points.getCol(2).xyz();
        controlPoints[3] = points.getCol(3).xyz();

        // I had issues with binormals between sections of the curve not lining up in my corkscrew
        // This resulted in weird sweeps
        // To fix this I pass a seed binormal to the function if an appropriate binormal exists
        Vector3f seedBinormal = noVector;
        if (j != 0) {
            seedBinormal = curve[curve.size()-1].B;
        }

        Curve result = evalBezier(controlPoints, steps, seedBinormal);

        // combine the result with whatever already exists of the curve
        unsigned size = result.size();
        for (k = 0; k < size; k++) {
            if (j != sections - 1) {
                // if not the last section, then don't include the last point
                if (k == size - 1) {
                    continue;
                }
            }
            curve.push_back(result[k]);
        }
    }


    // for each point, transform it to bernstein space, then render it with the other function

    cerr << "\t>>> Steps (type steps): " << steps << endl;

    // Return an empty curve right now.
    return curve;
}

Curve evalCircle( float radius, unsigned steps )
{
    // This is a sample function on how to properly initialize a Curve
    // (which is a vector< CurvePoint >).

    // Preallocate a curve with steps+1 CurvePoints
    Curve R( steps+1 );

    // Fill it in counterclockwise
    for( unsigned i = 0; i <= steps; ++i )
    {
        // step from 0 to 2pi
        float t = 2.0f * M_PI * float( i ) / steps;

        // Initialize position
        // We're pivoting counterclockwise around the y-axis
        R[i].V = radius * Vector3f( cos(t), sin(t), 0 );

        // Tangent vector is first derivative
        R[i].T = Vector3f( -sin(t), cos(t), 0 );

        // Normal vector is second derivative
        R[i].N = Vector3f( -cos(t), -sin(t), 0 );

        // Finally, binormal is facing up.
        R[i].B = Vector3f( 0, 0, 1 );
    }

    return R;
}

void drawCurve( const Curve& curve, float framesize )
{
    // Save current state of OpenGL
    glPushAttrib( GL_ALL_ATTRIB_BITS );

    // Setup for line drawing
    glDisable( GL_LIGHTING );
    glColor4f( 1, 1, 1, 1 );
    glLineWidth( 1 );

    // Draw curve
    glBegin( GL_LINE_STRIP );
    for( unsigned i = 0; i < curve.size(); ++i )
    {
        glVertex( curve[ i ].V );
    }
    glEnd();

    glLineWidth( 1 );

    // Draw coordinate frames if framesize nonzero
    if( framesize != 0.0f )
    {
        Matrix4f M;

        for( unsigned i = 0; i < curve.size(); ++i )
        {
            M.setCol( 0, Vector4f( curve[i].N, 0 ) );
            M.setCol( 1, Vector4f( curve[i].B, 0 ) );
            M.setCol( 2, Vector4f( curve[i].T, 0 ) );
            M.setCol( 3, Vector4f( curve[i].V, 1 ) );

            glPushMatrix();
            glMultMatrixf( M );
            glScaled( framesize, framesize, framesize );
            glBegin( GL_LINES );
            glColor3f( 1, 0, 0 ); glVertex3d( 0, 0, 0 ); glVertex3d( 1, 0, 0 );
            glColor3f( 0, 1, 0 ); glVertex3d( 0, 0, 0 ); glVertex3d( 0, 1, 0 );
            glColor3f( 0, 0, 1 ); glVertex3d( 0, 0, 0 ); glVertex3d( 0, 0, 1 );
            glEnd();
            glPopMatrix();
        }
    }

    // Pop state
    glPopAttrib();
}

