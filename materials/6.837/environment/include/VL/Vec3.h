/*
	File:			Vec3.h

	Function:		Defines a length-3 vector.
					
	Author(s):		Andrew Willmott

	Copyright:		(c) 1995-2000, Andrew Willmott
 */

#ifndef __Vec3__
#define __Vec3__

#include "vl/VL.h"
// Defines the actual type for TVec3 etc.
 
 
// --- Vec3 Class -------------------------------------------------------------


class TVec2;

class TVec3
{
public:

	// Constructors
	
	inline				TVec3();
	inline				TVec3(TVReal x, TVReal y, TVReal z);// [x, y, z]
	inline				TVec3(const TVec3 &v);			    // Copy constructor
	inline				TVec3(const TVec2 &v, TVReal w);	// Hom. 2D vector
	inline				TVec3(ZeroOrOne k);
	inline				TVec3(Axis a);
	
	// Accessor functions
	
	inline Int			Elts() const { return(3); };
	
	inline TVReal		&operator [] (Int i);		  
	inline const TVReal	&operator [] (Int i) const;

	inline TVReal		*Ref() const;					
						// Return pointer to data

	// Assignment operators
	
	inline TVec3	&operator =  (const TVec3 &a);			
	inline TVec3	&operator =  (ZeroOrOne k);	    				
	inline TVec3	&operator += (const TVec3 &a);
	inline TVec3	&operator -= (const TVec3 &a);
	inline TVec3	&operator *= (const TVec3 &a);
	inline TVec3	&operator *= (TVReal s);
	inline TVec3	&operator /= (const TVec3 &a);
	inline TVec3	&operator /= (TVReal s);
	
	// Comparison operators

	inline Bool		operator == (const TVec3 &a) const;	// v == a?
	inline Bool		operator != (const TVec3 &a) const;	// v != a?
	inline Bool		operator <  (const TVec3 &a) const; // v <  a?
	inline Bool		operator >= (const TVec3 &a) const; // v >= a?

	// Arithmetic operators
		
	inline TVec3	operator + (const TVec3 &a) const;	// v + a
	inline TVec3	operator - (const TVec3 &a) const;	// v - a
	inline TVec3	operator - () const;				// -v
	inline TVec3	operator * (const TVec3 &a) const;	// v * a (vx * ax, ...)
	inline TVec3	operator * (TVReal s) const;		// v * s
	inline TVec3	operator / (const TVec3 &a) const;	// v / a (vx / ax, ...)
	inline TVec3	operator / (TVReal s) const;		// v / s	

	// Initialisers
	
	inline TVec3	&MakeZero();						// Zero vector
	inline TVec3	&MakeUnit(Int i, TVReal k = vl_one);// I[i]
	inline TVec3	&MakeBlock(TVReal k = vl_one);		// All-k vector

	inline TVec3	&Normalise();						// normalise vector

	// Private...
	
protected:
	
	TVReal elt[3]; 
};


// --- Vec operators ----------------------------------------------------------

inline TVec3	operator * (TVReal s, const TVec3 &v);// s * v
inline TVReal	dot(const TVec3 &a, const TVec3 &b);// v . a
inline TVReal	len(const TVec3 &v);				// || v ||
inline TVReal	sqrlen(const TVec3 &v);				// v . v
inline TVec3	norm(const TVec3 &v);				// v / || v ||
inline Void		normalise(TVec3 &v);				// v = norm(v)
inline TVec3	cross(const TVec3 &a, const TVec3 &b);// a x b
inline TVec2	proj(const TVec3 &v);				// hom. projection

std::ostream	&operator << (std::ostream &s, const TVec3 &v);
std::istream	&operator >> (std::istream &s, TVec3 &v);


// --- Inlines ----------------------------------------------------------------

#include "vl/Vec2.h"

inline TVReal &TVec3::operator [] (Int i)
{
	CheckRange(i, 0, 3, "(Vec3::[i]) index out of range");
    return(elt[i]);
}

inline const TVReal &TVec3::operator [] (Int i) const
{
	CheckRange(i, 0, 3, "(Vec3::[i]) index out of range");
    return(elt[i]);
}

inline TVec3::TVec3()
{
}

inline TVec3::TVec3(TVReal x, TVReal y, TVReal z)
{
	elt[0] = x;
	elt[1] = y;
	elt[2] = z;
}

inline TVec3::TVec3(const TVec3 &v) 
{
	elt[0] = v[0];
	elt[1] = v[1];
	elt[2] = v[2];
}

inline TVec3::TVec3(const TVec2 &v, TVReal w) 
{
	elt[0] = v[0];
	elt[1] = v[1];
	elt[2] = w;
}

inline TVReal *TVec3::Ref() const
{
	return((TVReal *) elt);
}

inline TVec3 &TVec3::operator = (const TVec3 &v)
{
	elt[0] = v[0];
	elt[1] = v[1];
	elt[2] = v[2];
	
	return(SELF);
}

inline TVec3 &TVec3::operator += (const TVec3 &v)
{
	elt[0] += v[0];
	elt[1] += v[1];
	elt[2] += v[2];
	
	return(SELF);
}

inline TVec3 &TVec3::operator -= (const TVec3 &v)
{
	elt[0] -= v[0];
	elt[1] -= v[1];
	elt[2] -= v[2];
	
	return(SELF);
}

inline TVec3 &TVec3::operator *= (const TVec3 &a)
{
	elt[0] *= a[0];
	elt[1] *= a[1];
	elt[2] *= a[2];
	
	return(SELF);
}

inline TVec3 &TVec3::operator *= (TVReal s)
{
	elt[0] *= s;
	elt[1] *= s;
	elt[2] *= s;
	
	return(SELF);
}

inline TVec3 &TVec3::operator /= (const TVec3 &a)
{
	elt[0] /= a[0];
	elt[1] /= a[1];
	elt[2] /= a[2];
	
	return(SELF);
}

inline TVec3 &TVec3::operator /= (TVReal s)
{
	elt[0] /= s;
	elt[1] /= s;
	elt[2] /= s;
	
	return(SELF);
}

inline TVec3 TVec3::operator + (const TVec3 &a) const
{
	TVec3 result;
	
	result[0] = elt[0] + a[0];
	result[1] = elt[1] + a[1];
	result[2] = elt[2] + a[2];
	
	return(result);
}

inline TVec3 TVec3::operator - (const TVec3 &a) const
{
	TVec3 result;
	
	result[0] = elt[0] - a[0];
	result[1] = elt[1] - a[1];
	result[2] = elt[2] - a[2];
	
	return(result);
}

inline TVec3 TVec3::operator - () const
{
	TVec3 result;
	
	result[0] = -elt[0];
	result[1] = -elt[1];
	result[2] = -elt[2];
	
	return(result);
}

inline TVec3 TVec3::operator * (const TVec3 &a) const
{
	TVec3 result;
	
	result[0] = elt[0] * a[0];
	result[1] = elt[1] * a[1];
	result[2] = elt[2] * a[2];
	
	return(result);
}

inline TVec3 TVec3::operator * (TVReal s) const
{
	TVec3 result;
	
	result[0] = elt[0] * s;
	result[1] = elt[1] * s;
	result[2] = elt[2] * s;
	
	return(result);
}

inline TVec3 TVec3::operator / (const TVec3 &a) const
{
	TVec3 result;
	
	result[0] = elt[0] / a[0];
	result[1] = elt[1] / a[1];
	result[2] = elt[2] / a[2];
	
	return(result);
}

inline TVec3 TVec3::operator / (TVReal s) const
{
	TVec3 result;
	
	result[0] = elt[0] / s;
	result[1] = elt[1] / s;
	result[2] = elt[2] / s;
	
	return(result);
}

inline TVec3 operator * (TVReal s, const TVec3 &v)
{
	return(v * s);
}

inline TVec3 &TVec3::MakeUnit(Int n, TVReal k)
{
	if (n == 0)
	{ elt[0] = k; elt[1] = vl_zero; elt[2] = vl_zero; }
	else if (n == 1)
	{ elt[0] = vl_zero; elt[1] = k; elt[2] = vl_zero; }
	else if (n == 2)
	{ elt[0] = vl_zero; elt[1] = vl_zero; elt[2] = k; }
	else 
		_Error("(Vec3::Unit) illegal unit vector");
	return(SELF);
}

inline TVec3 &TVec3::MakeZero()
{
	elt[0] = vl_zero; elt[1] = vl_zero; elt[2] = vl_zero;
	return(SELF);
}

inline TVec3 &TVec3::MakeBlock(TVReal k)
{
	elt[0] = k; elt[1] = k; elt[2] = k;
	return(SELF);
}

inline TVec3 &TVec3::Normalise()
{
	Assert(sqrlen(SELF) > 0.0, "normalising length-zero vector");
	SELF /= len(SELF);
	return(SELF);
}


inline TVec3::TVec3(ZeroOrOne k) 
{
	elt[0] = k; elt[1] = k; elt[2] = k;
}

inline TVec3 &TVec3::operator = (ZeroOrOne k)
{
	elt[0] = k; elt[1] = k; elt[2] = k;
	
	return(SELF);
}

inline TVec3::TVec3(Axis a)
{
	MakeUnit(a, vl_one);
}


inline Bool TVec3::operator == (const TVec3 &a) const
{
	return(elt[0] == a[0] && elt[1] == a[1] && elt[2] == a[2]);
}

inline Bool TVec3::operator != (const TVec3 &a) const
{
	return(elt[0] != a[0] || elt[1] != a[1] || elt[2] != a[2]);
}

inline Bool TVec3::operator < (const TVec3 &a) const
{
	return(elt[0] < a[0] && elt[1] < a[1] && elt[2] < a[2]);
}

inline Bool TVec3::operator >= (const TVec3 &a) const
{
	return(elt[0] >= a[0] && elt[1] >= a[1] && elt[2] >= a[2]);
}


inline TVReal dot(const TVec3 &a, const TVec3 &b)
{
	return(a[0] * b[0] + a[1] * b[1] + a[2] * b[2]);
}

inline TVReal len(const TVec3 &v)
{
	return(sqrt(dot(v, v)));
}

inline TVReal sqrlen(const TVec3 &v)
{
	return(dot(v, v));
}

inline TVec3 norm(const TVec3 &v)	
{
	Assert(sqrlen(v) > 0.0, "normalising length-zero vector");
	return(v / len(v));
}

inline Void normalise(TVec3 &v)	
{
	v /= len(v);
}

inline TVec3 cross(const TVec3 &a, const TVec3 &b)
{
	TVec3 result;

	result[0] = a[1] * b[2] - a[2] * b[1];	
	result[1] = a[2] * b[0] - a[0] * b[2];	
	result[2] = a[0] * b[1] - a[1] * b[0];	

	return(result);
}

inline TVec2 proj(const TVec3 &v) 
{
	TVec2 result;
	
	Assert(v[2] != 0, "(Vec3/proj) last elt. is zero");
	
	result[0] = v[0] / v[2];
	result[1] = v[1] / v[2];
	
	return(result);
}

#endif
