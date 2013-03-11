/*
    File:       VLgl.h

    Function:	Provides some handy wrappers for using vl with
				OpenGL.

    Author:     Andrew Willmott

    Copyright:  (c) 2000, Andrew Willmott
*/

#ifndef __VLgl__
#define __VLgl__

// float versions

inline Void glVertex(const Vec2f &a) 
{ glVertex2fv(a.Ref()); }

inline Void glVertex(const Vec3f &a) 
{ glVertex3fv(a.Ref()); }

inline Void glVertex(const Vec4f &a) 
{ glVertex4fv(a.Ref()); }

inline Void glColor(const Vec3f &a) 
{ glColor3fv(a.Ref()); }

inline Void glColor(const Vec4f &a) 
{ glColor4fv(a.Ref()); }

inline Void glNormal(const Vec3f &a) 
{ glNormal3fv(a.Ref()); }

inline Void glTexCoord(const Vec2f &a) 
{ glTexCoord2fv(a.Ref()); }

inline Void glLoadMatrix(const Mat4f &m)
{
#ifdef VL_ROW_ORIENT
	glLoadMatrixf(m.Ref());
#else
	glLoadMatrixf(trans(m).Ref());
#endif
}

inline Void glMultMatrix(const Mat4f &m)
{
#ifdef VL_ROW_ORIENT
	glMultMatrixf(m.Ref());
#else
	glMultMatrixf(trans(m).Ref());
#endif
}

// double versions

inline Void glVertex(const Vec2d &a) 
{ glVertex2dv(a.Ref()); }

inline Void glVertex(const Vec3d &a) 
{ glVertex3dv(a.Ref()); }

inline Void glVertex(const Vec4d &a) 
{ glVertex4dv(a.Ref()); }

inline Void glColor(const Vec3d &a) 
{ glColor3dv(a.Ref()); }

inline Void glColor(const Vec4d &a) 
{ glColor4dv(a.Ref()); }

inline Void glNormal(const Vec3d &a) 
{ glNormal3dv(a.Ref()); }

inline Void glTexCoord(const Vec2d &a) 
{ glTexCoord2dv(a.Ref()); }

inline Void glLoadMatrix(const Mat4d &m)
{
#ifdef VL_ROW_ORIENT
	glLoadMatrixd(m.Ref());
#else
	glLoadMatrixd(trans(m).Ref());
#endif
}

inline Void glMultMatrix(const Mat4d &m)
{
#ifdef VL_ROW_ORIENT
	glMultMatrixd(m.Ref());
#else
	glMultMatrixd(trans(m).Ref());
#endif
}

// Note: glLoadMatrix[fd] expects matrices in column-major
// order, not row-order, hence the VL_ROW_ORIENT dependencies.
// OpenGL internally operates with row vectors (just like
// the original GL), and transformation matrices stored in
// row-major order. However, externally they pretend that
// they use column vectors; everything still works, but
// because trans(Av) = trans(v) trans(A), it appears that
// matrices are stored in column major order.

#endif
