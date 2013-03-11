/*
	File:			Vec2.h

	Function:		Defines a length-2 vector.
					
	Author(s):		Andrew Willmott

	Copyright:		(c) 1995-2000, Andrew Willmott
 */

#ifndef __Vec2__
#define __Vec2__

#include "vl/VL.h"
// Defines the actual type for TVec2 etc.


// --- Vec2 Class -------------------------------------------------------------


class TVec2
{
public:

	// Constructors
	
	inline				TVec2();
	inline				TVec2(TVReal x, TVReal y);		// (x, y)
	inline				TVec2(const TVec2 &v);			// Copy constructor
	inline				TVec2(ZeroOrOne k);				// v[i] = vl_zero
	inline				TVec2(Axis k);					// v[k] = 1
		
	// Accessor functions
	
	inline TVReal 	   &operator [] (Int i);		   	// v[1] - Has no Index
	inline const TVReal &operator [] (Int i) const;		//        check

	inline Int			Elts() const { return(2); };
	inline TVReal		*Ref() const;					// Return ptr to data

	// Assignment operators
	
	inline TVec2	&operator =  (const TVec2 &a);	    				
	inline TVec2	&operator =  (ZeroOrOne k);	    				
	inline TVec2	&operator =  (Axis k);	    				
	
	inline TVec2	&operator += (const TVec2 &a);
	inline TVec2	&operator -= (const TVec2 &a);
	inline TVec2	&operator *= (const TVec2 &a);
	inline TVec2	&operator *= (TVReal s);
	inline TVec2	&operator /= (const TVec2 &a);
	inline TVec2	&operator /= (TVReal s);

	// Comparison operators

	inline Bool		operator == (const TVec2 &a) const;	// v == a?
	inline Bool		operator != (const TVec2 &a) const;	// v != a?

	// Arithmetic operators
		
	inline TVec2	operator + (const TVec2 &a) const;	// v + a
	inline TVec2	operator - (const TVec2 &a) const;	// v - a
	inline TVec2	operator - () const;				// -v
	inline TVec2	operator * (const TVec2 &a) const;	// v * a (vx * ax, ...)
	inline TVec2	operator * (TVReal s) const;		// v * s
	inline TVec2	operator / (const TVec2 &a) const;	// v / a (vx / ax, ...)
	inline TVec2	operator / (TVReal s) const;		// v / s

	// Initialisers
	
	inline TVec2	&MakeZero();						// Zero vector
	inline TVec2	&MakeUnit(Int i, TVReal k = vl_one);// I[i]
	inline TVec2	&MakeBlock(TVReal k = vl_one);		// All-k vector
	
	inline TVec2	&Normalise();						// normalise vector

	// Private...
	
protected:

	TVReal			elt[2]; 
};


// --- Vec operators ----------------------------------------------------------

inline TVec2	operator * (TVReal s, const TVec2 &v);// s * v
inline TVReal	dot(const TVec2 &a, const TVec2 &b);// v . a
inline TVReal	len(const TVec2 &v);				// || v ||
inline TVReal	sqrlen(const TVec2 &v);				// v . v
inline TVec2	norm(const TVec2 &v);				// v / || v ||
inline Void  	normalise(TVec2 &v);				// v = norm(v)
inline TVec2	cross(const TVec2 &v);				// cross prod.
	
std::ostream	&operator << (std::ostream &s, const TVec2 &v);
std::istream	&operator >> (std::istream &s, TVec2 &v);

	
// --- Inlines ----------------------------------------------------------------


inline TVReal &TVec2::operator [] (Int i)
{
	CheckRange(i, 0, 2, "(Vec2::[i]) index out of range");
    return(elt[i]);
}

inline const TVReal &TVec2::operator [] (Int i) const
{
	CheckRange(i, 0, 2, "(Vec2::[i]) index out of range");
    return(elt[i]);
}

inline TVec2::TVec2()
{
}

inline TVec2::TVec2(TVReal x, TVReal y)
{
	elt[0] = x;
	elt[1] = y;
}

inline TVec2::TVec2(const TVec2 &v) 
{
	elt[0] = v[0];
	elt[1] = v[1];
}

inline TVReal *TVec2::Ref() const
{
	return((TVReal *) elt);
}

inline TVec2 &TVec2::operator = (const TVec2 &v)
{
	elt[0] = v[0];
	elt[1] = v[1];
	
	return(SELF);
}

inline TVec2 &TVec2::operator += (const TVec2 &v)
{
	elt[0] += v[0];
	elt[1] += v[1];
	
	return(SELF);
}

inline TVec2 &TVec2::operator -= (const TVec2 &v)
{
	elt[0] -= v[0];
	elt[1] -= v[1];
	
	return(SELF);
}

inline TVec2 &TVec2::operator *= (const TVec2 &v)
{
	elt[0] *= v[0];
	elt[1] *= v[1];
	
	return(SELF);
}

inline TVec2 &TVec2::operator *= (TVReal s)
{
	elt[0] *= s;
	elt[1] *= s;
	
	return(SELF);
}

inline TVec2 &TVec2::operator /= (const TVec2 &v)
{
	elt[0] /= v[0];
	elt[1] /= v[1];
	
	return(SELF);
}

inline TVec2 &TVec2::operator /= (TVReal s)
{
	elt[0] /= s;
	elt[1] /= s;
	
	return(SELF);
}

inline TVec2 TVec2::operator + (const TVec2 &a) const
{
	TVec2 result;
	
	result[0] = elt[0] + a[0];
	result[1] = elt[1] + a[1];
	
	return(result);
}

inline TVec2 TVec2::operator - (const TVec2 &a) const
{
	TVec2 result;
	
	result[0] = elt[0] - a[0];
	result[1] = elt[1] - a[1];
	
	return(result);
}

inline TVec2 TVec2::operator - () const
{
	TVec2 result;
	
	result[0] = -elt[0];
	result[1] = -elt[1];
	
	return(result);
}

inline TVec2 TVec2::operator * (const TVec2 &a) const			
{
	TVec2 result;
	
	result[0] = elt[0] * a[0];
	result[1] = elt[1] * a[1];
	
	return(result);
}

inline TVec2 TVec2::operator * (TVReal s) const
{
	TVec2 result;
	
	result[0] = elt[0] * s;
	result[1] = elt[1] * s;
	
	return(result);
}

inline TVec2 operator * (TVReal s, const TVec2 &v)
{
	return(v * s);
}

inline TVec2 TVec2::operator / (const TVec2 &a) const
{
	TVec2 result;
	
	result[0] = elt[0] / a[0];
	result[1] = elt[1] / a[1];
	
	return(result);
}

inline TVec2 TVec2::operator / (TVReal s) const
{
	TVec2 result;
	
	result[0] = elt[0] / s;
	result[1] = elt[1] / s;
	
	return(result);
}

inline TVReal dot(const TVec2 &a, const TVec2 &b)
{
	return(a[0] * b[0] + a[1] * b[1]);
}

inline TVec2 cross(const TVec2 &a)
{
	TVec2 result;
	
	result[0] =  a[1];
	result[1] = -a[0];
	
	return(result);
}

inline TVReal len(const TVec2 &v)
{
	return(sqrt(dot(v, v)));
}

inline TVReal sqrlen(const TVec2 &v)
{
	return(dot(v, v));
}

inline TVec2 norm(const TVec2 &v)	
{
	Assert(sqrlen(v) > 0.0, "normalising length-zero vector");
	return(v / len(v));
}

inline Void normalise(TVec2 &v)	
{
	v /= len(v);
}

inline TVec2 &TVec2::MakeUnit(Int i, TVReal k)
{
	if (i == 0)
	{ elt[0] = k; elt[1] = vl_zero; }
	else if (i == 1)
	{ elt[0] = vl_zero; elt[1] = k; }
	else 
		_Error("(Vec2::Unit) illegal unit vector");
	return(SELF);
}

inline TVec2 &TVec2::MakeZero()
{
	elt[0] = vl_zero; elt[1] = vl_zero;
	return(SELF);
}

inline TVec2 &TVec2::MakeBlock(TVReal k)
{
	elt[0] = k; elt[1] = k;
	return(SELF);
}

inline TVec2 &TVec2::Normalise()
{
	Assert(sqrlen(SELF) > 0.0, "normalising length-zero vector");
	SELF /= len(SELF);
	return(SELF);
}


inline TVec2::TVec2(ZeroOrOne k) 
{
	elt[0] = k;
	elt[1] = k;
}

inline TVec2::TVec2(Axis k) 
{
	MakeUnit(k, vl_one);
}

inline TVec2 &TVec2::operator = (ZeroOrOne k)
{
	elt[0] = k; elt[1] = k;
	
	return(SELF);
}

inline TVec2 &TVec2::operator = (Axis k)
{
	MakeUnit(k, vl_1);
	
	return(SELF);
}

inline Bool TVec2::operator == (const TVec2 &a) const
{
	return(elt[0] == a[0] && elt[1] == a[1]);
}

inline Bool TVec2::operator != (const TVec2 &a) const
{
	return(elt[0] != a[0] || elt[1] != a[1]);
}

#endif
