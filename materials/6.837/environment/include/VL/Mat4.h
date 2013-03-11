/*
	File:			Mat4.h

	Function:		Defines a 4 x 4 matrix.
					
	Author(s):		Andrew Willmott

	Copyright:		(c) 1995-2000, Andrew Willmott
 */

#ifndef __Mat4__
#define __Mat4__

#include "vl/VL.h"
// Defines the actual type for TMat4 etc.
#include "vl/Vec4.h"


// --- Mat4 Class -------------------------------------------------------------

class TVec3;

class TMat4 
{
public:
	
	// Constructors
	
	inline		TMat4();
				TMat4(TMReal a, TMReal b, TMReal c, TMReal d,
					TMReal e, TMReal f, TMReal g, TMReal h,
					TMReal i, TMReal j, TMReal k, TMReal l,
					TMReal m, TMReal n, TMReal o, TMReal p);
				TMat4(const TMat4 &m);					// Copy constructor
				TMat4(ZeroOrOne k);
				TMat4(Block k);
					
	// Accessor functions	
	inline Int			Rows() const { return(4); };
	inline Int			Cols() const { return(4); };
	
	inline TMVec4    	&operator [] (Int i);		   
	inline const TMVec4	&operator [] (Int i) const;	

	inline TMReal		*Ref() const;			// Return pointer to data

	// Assignment operators	
	TMat4	 			&operator =  (const TMat4 &m);	   
	inline TMat4	 	&operator =  (ZeroOrOne k);	   
	inline TMat4	 	&operator =  (Block k);	   
	TMat4 				&operator += (const TMat4 &m);	
	TMat4 				&operator -= (const TMat4 &m);	    
	TMat4 				&operator *= (const TMat4 &m);	    
	TMat4 				&operator *= (TMReal s);	    			
	TMat4 				&operator /= (TMReal s);	    		

	// Comparison operators	
	Bool				operator == (const TMat4 &m) const;	// M == N?
	Bool				operator != (const TMat4 &m) const;	// M != N?
	
	// Arithmetic operators	
	TMat4				operator + (const TMat4 &m) const;	// M + N
	TMat4				operator - (const TMat4 &m) const;	// M - N
	TMat4				operator - () const;				// -M
	TMat4				operator * (const TMat4 &m) const;	// M * N
	TMat4				operator * (TMReal s) const;		// M * s
	TMat4				operator / (TMReal s) const;		// M / s

	// Initialisers	
	Void		MakeZero();									// Zero matrix
	Void		MakeDiag(TMReal k = vl_one);				// I
	Void		MakeBlock(TMReal k = vl_one);				// all elts = k

	// Homogeneous Transforms	
	TMat4& 		MakeHRot(const TMVec3 &axis, Real theta);		
									// Rotate by theta radians about axis   
	TMat4& 		MakeHRot(const TQuaternion &q);	// Rotate by quaternion   
	TMat4&		MakeHScale(const TMVec3 &s);	// Scale by components of s

	TMat4& 		MakeHTrans(const TMVec3 &t);	// Translation by t

	TMat4&		Transpose();					// transpose in place
	TMat4&		AddShift(const TMVec3 &t);		// concat shift
	
	// Private...	
protected:	

	TMVec4	row[4];	
};


// --- Matrix operators -------------------------------------------------------

TMVec4			operator * (const TMat4 &m, const TMVec4 &v);	// m * v
TMVec4			operator * (const TMVec4 &v, const TMat4 &m);	// v * m
TMVec4	       	&operator *= (TMVec4 &a, const TMat4 &m);		// v *= m
inline TMat4	operator * (TMReal s, const TMat4 &m);			// s * m

TMat4			trans(const TMat4 &m);				// Transpose			
TMReal			trace(const TMat4 &m);				// Trace
TMat4			adj(const TMat4 &m);				// Adjoint
TMReal			det(const TMat4 &m);				// Determinant
TMat4			inv(const TMat4 &m);				// Inverse
TMat4			oprod(const TMVec4 &a, const TMVec4 &b);
													// Outer product

// The xform functions help avoid dependence on whether row or column
// vectors are used to represent points and vectors.
inline TVec4	xform(const TMat4 &m, const TVec4 &v); // Transform of v by m
inline TVec3	xform(const TMat4 &m, const TVec3 &v); // Hom. xform of v by m
inline TMat4	xform(const TMat4 &m, const TMat4 &n); // Xform v -> m(n(v))

std::ostream			&operator << (std::ostream &s, const TMat4 &m);
std::istream			&operator >> (std::istream &s, TMat4 &m);
	

// --- Inlines ----------------------------------------------------------------

inline TMat4::TMat4()
{
}

inline TMVec4 &TMat4::operator [] (Int i)
{
	CheckRange(i, 0, 4, "(Mat4::[i]) index out of range");
    return(row[i]);
}

inline const TMVec4 &TMat4::operator [] (Int i) const
{
	CheckRange(i, 0, 4, "(Mat4::[i]) index out of range");
    return(row[i]);
}

inline TMReal *TMat4::Ref() const
{
	return((TMReal *) row);
}

inline TMat4::TMat4(ZeroOrOne k)
{
	MakeDiag(k);
}

inline TMat4::TMat4(Block k)
{
	MakeBlock((ZeroOrOne) k);
}

inline TMat4 &TMat4::operator = (ZeroOrOne k)
{
	MakeDiag(k);

	return(SELF);
}

inline TMat4 &TMat4::operator = (Block k)
{
	MakeBlock((ZeroOrOne) k);

	return(SELF);
}

inline TMat4 operator * (TMReal s, const TMat4 &m)
{
	return(m * s);
}

#ifdef VL_ROW_ORIENT
inline TVec3 xform(const TMat4 &m, const TVec3 &v)
{ return(proj(TVec4(v, 1.0) * m)); }
inline TVec4 xform(const TMat4 &m, const TVec4 &v)
{ return(v * m); }
inline TMat4 xform(const TMat4 &m, const TMat4 &n)
{ return(n * m); }
#else
inline TVec3 xform(const TMat4 &m, const TVec3 &v)
{ return(proj(m * TVec4(v, 1.0))); }
inline TVec4 xform(const TMat4 &m, const TVec4 &v)
{ return(m * v); }
inline TMat4 xform(const TMat4 &m, const TMat4 &n)
{ return(m * n); }
#endif

#endif
