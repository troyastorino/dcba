/*
	File:			Mixed.h
	
	Function:		Provides 'mixed' operations that allow matrices of one
					type to be used to transform vectors of another. This
					can be useful if you want to keep your data in a lower
					precision format (e.g., float) for space reasons, but
					use a high-precision format for transformations (e.g.,
					double) for accuracy.

					The operations defined here are the various flavours of
					matrix/vector multiplication, and the necessary dot
					products to implement those.
	
	Author(s):		Andrew Willmott

	Copyright:		(c) 1997-2000, Andrew Willmott
*/

#include "vl/VL.h"

// dot prods

TMReal			dot(const TMVec &a, const TVec &b);	 // v . a
TMReal			dot(const TMSparseVec &a, const TSparseVec &b);
TMReal			dot(const TMSparseVec &a, const TVec &b);

// xform

inline TVec2	xform(const TMat2 &m, const TVec2 &v);
inline TVec2	xform(const TMat3 &m, const TVec2 &v);
inline TVec3	xform(const TMat3 &m, const TVec3 &v);
inline TVec3	xform(const TMat4 &m, const TVec3 &v);
inline TVec4	xform(const TMat4 &m, const TVec4 &v);

// matrix-vector multiplications

inline TVec2	&operator *= (TVec2 &v, const TMat2 &m);
inline TVec2	operator * (const TMat2 &m, const TVec2 &v);
inline TVec2	operator * (const TVec2 &v, const TMat2 &m);

inline TVec3	&operator *= (TVec3 &v, const TMat3 &m);	
inline TVec3	operator * (const TMat3 &m, const TVec3 &v);
inline TVec3	operator * (const TVec3 &v, const TMat3 &m);

TVec4			operator * (const TMat4 &m, const TVec4 &v);
TVec4			operator * (const TVec4 &v, const TMat4 &m);
TVec4	       	&operator *= (TVec4 &a, const TMat4 &m);	

TVec			operator * (const TMat &m, const TVec &v);
TVec			operator * (const TVec &v, const TMat &m);
TVec			&operator *= (TVec &v, const TMat &m);

#ifdef __SparseVec__
TSparseVec		&operator *= (TSparseVec &v, const TSparseMat &m);
TSparseVec		operator * (const TSparseVec &v, const TSparseMat &m);
TSparseVec		operator * (const TSparseMat &m, const TSparseVec &v);
TVec 			&operator *= (TVec &v, const TSparseMat &m);
TVec			operator * (const TVec &v, const TSparseMat &m);
TVec			operator * (const TSparseMat &m, const TVec &v);
#endif

// inlines

inline TVec2 operator * (const TMat2 &m, const TVec2 &v)
{
	TVec2 result;

	result[0] = m[0][0] * v[0] + m[0][1] * v[1];
	result[1] = m[1][0] * v[0] + m[1][1] * v[1];

	return(result);
}

inline TVec2 operator * (const TVec2 &v, const TMat2 &m)			
{
	TVec2 result;

	result[0] = v[0] * m[0][0] + v[1] * m[1][0];
	result[1] = v[0] * m[0][1] + v[1] * m[1][1];

	return(result);
}

inline TVec2 &operator *= (TVec2 &v, const TMat2 &m)		
{
	TVReal t;
	
	t    = v[0] * m[0][0] + v[1] * m[1][0];
	v[1] = v[0] * m[0][1] + v[1] * m[1][1];
	v[0] = t;

	return(v);
}

inline TVec3 operator * (const TMat3 &m, const TVec3 &v)
{
	TVec3 result;
	
	result[0] = v[0] * m[0][0] + v[1] * m[0][1] + v[2] * m[0][2];
	result[1] = v[0] * m[1][0] + v[1] * m[1][1] + v[2] * m[1][2];
	result[2] = v[0] * m[2][0] + v[1] * m[2][1] + v[2] * m[2][2];
	
	return(result);
}

inline TVec3 operator * (const TVec3 &v, const TMat3 &m)	
{
	TVec3 result;
	
	result[0] = v[0] * m[0][0] + v[1] * m[1][0] + v[2] * m[2][0];
	result[1] = v[0] * m[0][1] + v[1] * m[1][1] + v[2] * m[2][1];
	result[2] = v[0] * m[0][2] + v[1] * m[1][2] + v[2] * m[2][2];
	
	return(result);
}

inline TVec3 &operator *= (TVec3 &v, const TMat3 &m)			
{	
	TVReal t0, t1;
	
	t0   = v[0] * m[0][0] + v[1] * m[1][0] + v[2] * m[2][0];
	t1   = v[0] * m[0][1] + v[1] * m[1][1] + v[2] * m[2][1];
	v[2] = v[0] * m[0][2] + v[1] * m[1][2] + v[2] * m[2][2];
	v[0] = t0;
	v[1] = t1;

	return(v);
}

#ifdef VL_ROW_ORIENT
inline TVec2 xform(const TMat2 &m, const TVec2 &v)
{ return(v * m); }
inline TVec2 xform(const TMat3 &m, const TVec2 &v)
{ return(proj(TVec3(v, 1.0) * m)); }
inline TVec3 xform(const TMat3 &m, const TVec3 &v)
{ return(v * m); }
inline TVec3 xform(const TMat4 &m, const TVec3 &v)
{ return(proj(TVec4(v, 1.0) * m)); }
inline TVec4 xform(const TMat4 &m, const TVec4 &v)
{ return(v * m); }
#else
inline TVec2 xform(const TMat2 &m, const TVec2 &v)
{ return(m * v); }
inline TVec2 xform(const TMat3 &m, const TVec2 &v)
{ return(proj(m * TVec3(v, 1.0))); }
inline TVec3 xform(const TMat3 &m, const TVec3 &v)
{ return(m * v); }
inline TVec3 xform(const TMat4 &m, const TVec3 &v)
{ return(proj(m * TVec4(v, 1.0))); }
inline TVec4 xform(const TMat4 &m, const TVec4 &v)
{ return(m * v); }
#endif
