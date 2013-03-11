/*
	File:			Mat3.h

	Function:		Defines a 3 x 3 matrix.
					
	Author(s):		Andrew Willmott

	Copyright:		(c) 1995-2000, Andrew Willmott
*/

#ifndef __Mat3__
#define __Mat3__

#include "vl/VL.h"
// Defines the actual type for TMat3 etc.
#include "vl/Vec3.h"


// --- Mat3 Class -------------------------------------------------------------


class TVec4;

class TMat3 
{
public:
	
	// Constructors
	
	inline 			TMat3();
		 			TMat3(TMReal a, TMReal b, TMReal c,
					     TMReal d, TMReal e, TMReal f,
					     TMReal g, TMReal h, TMReal i);
					TMat3(const TMat3 &m);			// Copy constructor
					TMat3(ZeroOrOne k);
					TMat3(Block k);
						
	// Accessor functions
	
	inline Int			Rows() const { return(3); };
	inline Int			Cols() const { return(3); };
	
	inline TMVec3    	&operator [] (Int i);	
	inline const TMVec3	&operator [] (Int i) const;

	inline TMReal 		*Ref() const;				// Return pointer to data

	// Assignment operators
	
	TMat3 				&operator =  (const TMat3 &m);	    
	inline TMat3 		&operator =  (ZeroOrOne k);	    
	inline TMat3 		&operator =  (Block k);	    
	TMat3 				&operator += (const TMat3 &m);	    
	TMat3 				&operator -= (const TMat3 &m);	   
	TMat3 				&operator *= (const TMat3 &m);	   
	TMat3 				&operator *= (TMReal s);	    		
	TMat3 				&operator /= (TMReal s);	   
	
	// Comparison operators
	
	Bool				operator == (const TMat3 &m) const;	// M == N?
	Bool				operator != (const TMat3 &m) const;	// M != N?
	
	// Arithmetic operators
	
	TMat3				operator + (const TMat3 &m) const;	// M + N
	TMat3				operator - (const TMat3 &m) const;	// M - N
	TMat3				operator - () const;				// -M
	TMat3				operator * (const TMat3 &m) const;	// M * N
	TMat3				operator * (TMReal s) const;		// M * s
	TMat3				operator / (TMReal s) const;		// M / s

	// Initialisers
	
	Void				MakeZero();							// Zero matrix
	Void				MakeDiag(TMReal k = vl_one);		// I
	Void				MakeBlock(TMReal k = vl_one);		// all elts = k

	// Vector Transforms
	
	TMat3& 				MakeRot(const TMVec3 &axis, Real theta);		    
	TMat3&				MakeRot(const TMVec4 &q);		// Rotate by quaternion 	
	TMat3& 				MakeScale(const TMVec3 &s);

	// Homogeneous Transforms
	
	TMat3&				MakeHRot(Real theta);			// Rotate by theta rads    
	TMat3&				MakeHScale(const TMVec2 &s);	// Scale by s
	TMat3& 				MakeHTrans(const TMVec2 &t);	// Translation by t
	
	// Private...

protected:

	TMVec3	row[3];	
};


// --- Matrix operators -------------------------------------------------------

inline TMVec3	&operator *= (TMVec3 &v, const TMat3 &m);		// v *= m
inline TMVec3	operator * (const TMat3 &m, const TMVec3 &v);	// m * v
inline TMVec3	operator * (const TMVec3 &v, const TMat3 &m);	// v * m
inline TMat3	operator * (const TMReal s, const TMat3 &m);	// s * m

TMat3			trans(const TMat3 &m);				// Transpose			
TMReal			trace(const TMat3 &m);				// Trace
TMat3			adj(const TMat3 &m);				// Adjoint
TMReal			det(const TMat3 &m);				// Determinant
TMat3			inv(const TMat3 &m);				// Inverse
TMat3			oprod(const TMVec3 &a, const TMVec3 &b);
													// Outer product

// The xform functions help avoid dependence on whether row or column
// vectors are used to represent points and vectors.
inline TVec3	xform(const TMat3 &m, const TVec3 &v); // Transform of v by m
inline TVec2	xform(const TMat3 &m, const TVec2 &v); // Hom. xform of v by m
inline TMat3	xform(const TMat3 &m, const TMat3 &n); // Xform v -> m(n(v))

std::ostream			&operator << (std::ostream &s, const TMat3 &m);
std::istream			&operator >> (std::istream &s, TMat3 &m);


// --- Inlines ----------------------------------------------------------------

inline TMat3::TMat3()
{
}

inline TMVec3 &TMat3::operator [] (Int i)
{
	CheckRange(i, 0, 3, "(Mat3::[i]) index out of range");
    return(row[i]);
}

inline const TMVec3 &TMat3::operator [] (Int i) const
{
	CheckRange(i, 0, 3, "(Mat3::[i]) index out of range");
    return(row[i]);
}

inline TMReal *TMat3::Ref() const
{
	return((TMReal *) row);
}

inline TMat3::TMat3(ZeroOrOne k)
{
	MakeDiag(k);
}

inline TMat3::TMat3(Block k)
{
	MakeBlock((ZeroOrOne) k);
}

inline TMat3 &TMat3::operator = (ZeroOrOne k)
{
	MakeDiag(k);

	return(SELF);
}
	
inline TMat3 &TMat3::operator = (Block k)
{
	MakeBlock((ZeroOrOne) k);

	return(SELF);
}
	
inline TMat3 operator *  (const TMReal s, const TMat3 &m)
{
	return(m * s);
}

inline TMVec3 operator * (const TMat3 &m, const TMVec3 &v)
{
	TMVec3 result;
	
	result[0] = v[0] * m[0][0] + v[1] * m[0][1] + v[2] * m[0][2];
	result[1] = v[0] * m[1][0] + v[1] * m[1][1] + v[2] * m[1][2];
	result[2] = v[0] * m[2][0] + v[1] * m[2][1] + v[2] * m[2][2];
	
	return(result);
}

inline TMVec3 operator * (const TMVec3 &v, const TMat3 &m)	
{
	TMVec3 result;
	
	result[0] = v[0] * m[0][0] + v[1] * m[1][0] + v[2] * m[2][0];
	result[1] = v[0] * m[0][1] + v[1] * m[1][1] + v[2] * m[2][1];
	result[2] = v[0] * m[0][2] + v[1] * m[1][2] + v[2] * m[2][2];
	
	return(result);
}

inline TMVec3 &operator *= (TMVec3 &v, const TMat3 &m)			
{	
	TMReal t0, t1;
	
	t0   = v[0] * m[0][0] + v[1] * m[1][0] + v[2] * m[2][0];
	t1   = v[0] * m[0][1] + v[1] * m[1][1] + v[2] * m[2][1];
	v[2] = v[0] * m[0][2] + v[1] * m[1][2] + v[2] * m[2][2];
	v[0] = t0;
	v[1] = t1;

	return(v);
}

#ifdef VL_ROW_ORIENT
inline TVec2 xform(const TMat3 &m, const TVec2 &v)
{ return(proj(TVec3(v, 1.0) * m)); }
inline TVec3 xform(const TMat3 &m, const TVec3 &v)
{ return(v * m); }
inline TMat3 xform(const TMat3 &m, const TMat3 &n)
{ return(n * m); }
#else
inline TVec2 xform(const TMat3 &m, const TVec2 &v)
{ return(proj(m * TVec3(v, 1.0))); }
inline TVec3 xform(const TMat3 &m, const TVec3 &v)
{ return(m * v); }
inline TMat3 xform(const TMat3 &m, const TMat3 &n)
{ return(m * n); }
#endif

#endif

