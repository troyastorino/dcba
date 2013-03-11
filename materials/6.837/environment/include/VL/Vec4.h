/*
	File:			Vec4.h

	Function:		Defines a length-4 vector.
					
	Author(s):		Andrew Willmott

	Copyright:		(c) 1995-2000, Andrew Willmott
 */

#ifndef __Vec4__
#define __Vec4__

#include "vl/VL.h"
// Defines the actual type for TVec4 etc.


// --- Vec4 Class -------------------------------------------------------------


class TVec3;

class TVec4
{
public:

	// Constructors
	
	inline 		TVec4();
	inline 		TVec4(TVReal x, TVReal y, TVReal z, TVReal w);	// [x, y, z, w]
	inline 		TVec4(const TVec4 &v);			 			// Copy constructor
	inline 		TVec4(const TVec3 &v, TVReal w);			// Hom. 3D vector
	inline 		TVec4(ZeroOrOne k);	    				
	inline 		TVec4(Axis k);	    				
	
	// Accessor functions
	
	inline Int			Elts() const { return(4); };
	
	inline TVReal 		&operator [] (Int i);		    
	inline const TVReal	&operator [] (Int i) const;					  

	inline TVReal		*Ref() const;					// Return pointer to data

	// Assignment operators
	
	inline TVec4		&operator =  (const TVec4 &a);	
	inline TVec4		&operator =  (ZeroOrOne k);
	inline TVec4		&operator =  (Axis k);
	inline TVec4		&operator += (const TVec4 &a);
	inline TVec4		&operator -= (const TVec4 &a);
	inline TVec4		&operator *= (const TVec4 &a);
	inline TVec4		&operator *= (TVReal s);
	inline TVec4		&operator /= (const TVec4 &a);
	inline TVec4		&operator /= (TVReal s);
	
	// Comparison operators

	Bool				operator == (const TVec4 &a) const;	// v == a ?
	Bool				operator != (const TVec4 &a) const;	// v != a ?

	// Arithmetic operators
		
	inline TVec4		operator + (const TVec4 &a) const;	// v + a
	inline TVec4		operator - (const TVec4 &a) const;	// v - a
	inline TVec4		operator - () const;				// -v
	inline TVec4		operator * (const TVec4 &a) const;	// v * a (vx * ax, ...)
	inline TVec4		operator * (TVReal s) const;		// v * s
	inline TVec4		operator / (const TVec4 &a) const;	// v / a (vx / ax, ...)
	inline TVec4		operator / (TVReal s) const;		// v / s

	
	// Initialisers
	
	inline TVec4		&MakeZero();						// Zero vector
	TVec4				&MakeUnit(Int i, TVReal k = vl_one);// kI[i]
	inline TVec4		&MakeBlock(TVReal k = vl_one);		// All-k vector

	inline TVec4		&Normalise();						// normalise vector

	// Private...

protected:	
	
	TVReal elt[4]; 
};


// --- Vec operators ----------------------------------------------------------

inline TVec4	operator * (TVReal s, const TVec4 &v);// Left mult. by s
inline TVReal	dot(const TVec4 &a, const TVec4 &b);// v . a	
inline TVReal	len(const TVec4 &v);				// || v ||
inline TVReal	sqrlen(const TVec4 &v);             // v . v
inline TVec4	norm(const TVec4 &v);               // v / || v ||
inline Void  	normalise(TVec4 &v);				// v = norm(v)
TVec4			cross(const TVec4 &a, const TVec4 &b, const TVec4 &c);
                                                    // 4D cross prod.
TVec3			proj(const TVec4 &v);               // hom. projection

std::ostream	&operator << (std::ostream &s, const TVec4 &v);
std::istream	&operator >> (std::istream &s, TVec4 &v);
	

// --- Inlines ----------------------------------------------------------------


#include "vl/Vec3.h"

inline TVReal &TVec4::operator [] (Int i)
{
	CheckRange(i, 0, 4, "(Vec4::[i]) index out of range");
    return(elt[i]);
}

inline const TVReal &TVec4::operator [] (Int i) const
{
	CheckRange(i, 0, 4, "(Vec4::[i]) index out of range");
    return(elt[i]);
}


inline TVec4::TVec4()
{
}

inline TVec4::TVec4(TVReal x, TVReal y, TVReal z, TVReal w)
{
	elt[0] = x;
	elt[1] = y;
	elt[2] = z;
	elt[3] = w;
}

inline TVec4::TVec4(const TVec4 &v) 
{
	elt[0] = v[0];
	elt[1] = v[1];
	elt[2] = v[2];
	elt[3] = v[3];
}

inline TVec4::TVec4(const TVec3 &v, TVReal w)  	
{
	elt[0] = v[0];
	elt[1] = v[1];
	elt[2] = v[2];
	elt[3] = w;
}

inline TVReal *TVec4::Ref() const
{
	return((TVReal *) elt);
}

inline TVec4 &TVec4::operator = (const TVec4 &v)
{
	elt[0] = v[0];
	elt[1] = v[1];
	elt[2] = v[2];
	elt[3] = v[3];
	
	return(SELF);
}

inline TVec4 &TVec4::operator += (const TVec4 &v)
{
	elt[0] += v[0];
	elt[1] += v[1];
	elt[2] += v[2];
	elt[3] += v[3];
	
	return(SELF);
}

inline TVec4 &TVec4::operator -= (const TVec4 &v)
{
	elt[0] -= v[0];
	elt[1] -= v[1];
	elt[2] -= v[2];
	elt[3] -= v[3];
	
	return(SELF);
}

inline TVec4 &TVec4::operator *= (const TVec4 &v)
{
	elt[0] *= v[0];
	elt[1] *= v[1];
	elt[2] *= v[2];
	elt[3] *= v[3];
	
	return(SELF);
}

inline TVec4 &TVec4::operator *= (TVReal s)
{
	elt[0] *= s;
	elt[1] *= s;
	elt[2] *= s;
	elt[3] *= s;
	
	return(SELF);
}

inline TVec4 &TVec4::operator /= (const TVec4 &v)
{
	elt[0] /= v[0];
	elt[1] /= v[1];
	elt[2] /= v[2];
	elt[3] /= v[3];
	
	return(SELF);
}

inline TVec4 &TVec4::operator /= (TVReal s)
{
	elt[0] /= s;
	elt[1] /= s;
	elt[2] /= s;
	elt[3] /= s;
	
	return(SELF);
}


inline TVec4 TVec4::operator + (const TVec4 &a) const
{
	TVec4 result;
	
	result[0] = elt[0] + a[0];
	result[1] = elt[1] + a[1];
	result[2] = elt[2] + a[2];
	result[3] = elt[3] + a[3];
	
	return(result);
}

inline TVec4 TVec4::operator - (const TVec4 &a) const
{
	TVec4 result;
	
	result[0] = elt[0] - a[0];
	result[1] = elt[1] - a[1];
	result[2] = elt[2] - a[2];
	result[3] = elt[3] - a[3];
	
	return(result);
}

inline TVec4 TVec4::operator - () const
{
	TVec4 result;
	
	result[0] = -elt[0];
	result[1] = -elt[1];
	result[2] = -elt[2];
	result[3] = -elt[3];
	
	return(result);
}

inline TVec4 TVec4::operator * (const TVec4 &a) const			
{
	TVec4 result;

	result[0] = elt[0] * a[0];
	result[1] = elt[1] * a[1];
	result[2] = elt[2] * a[2];
	result[3] = elt[3] * a[3];
	
	return(result);
}

inline TVec4 TVec4::operator * (TVReal s) const
{
	TVec4 result;
	
	result[0] = elt[0] * s;
	result[1] = elt[1] * s;
	result[2] = elt[2] * s;
	result[3] = elt[3] * s;
	
	return(result);
}

inline TVec4 TVec4::operator / (const TVec4 &a) const
{
	TVec4 result;
	
	result[0] = elt[0] / a[0];
	result[1] = elt[1] / a[1];
	result[2] = elt[2] / a[2];
	result[3] = elt[3] / a[3];
	
	return(result);
}

inline TVec4 TVec4::operator / (TVReal s) const
{
	TVec4 result;
	
	result[0] = elt[0] / s;
	result[1] = elt[1] / s;
	result[2] = elt[2] / s;
	result[3] = elt[3] / s;
	
	return(result);
}

inline TVec4 operator * (TVReal s, const TVec4 &v)
{
	return(v * s);
}

inline TVec4 &TVec4::MakeZero()
{
	elt[0] = vl_zero; elt[1] = vl_zero; elt[2] = vl_zero; elt[3] = vl_zero;
	return(SELF);
}

inline TVec4 &TVec4::MakeBlock(TVReal k)
{
	elt[0] = k; elt[1] = k; elt[2] = k; elt[3] = k;
	return(SELF);
}

inline TVec4 &TVec4::Normalise()
{
	Assert(sqrlen(SELF) > 0.0, "normalising length-zero vector");
	SELF /= len(SELF);
	return(SELF);
}

inline TVec4::TVec4(ZeroOrOne k)  	
{
	MakeBlock(k);
}

inline TVec4::TVec4(Axis k)  	
{
	MakeUnit(k, vl_1);
}

inline TVec4 &TVec4::operator = (ZeroOrOne k)
{
	MakeBlock(k);
	
	return(SELF);
}

inline TVec4 &TVec4::operator = (Axis k)
{
	MakeUnit(k, vl_1);
	
	return(SELF);
}


inline TVReal dot(const TVec4 &a, const TVec4 &b)
{
	return(a[0] * b[0] + a[1] * b[1] + a[2] * b[2] + a[3] * b[3]);
}

inline TVReal len(const TVec4 &v)
{
	return(sqrt(dot(v, v)));
}

inline TVReal sqrlen(const TVec4 &v)
{
	return(dot(v, v));
}

inline TVec4 norm(const TVec4 &v)	
{
	Assert(sqrlen(v) > 0.0, "normalising length-zero vector");
	return(v / len(v));
}

inline Void normalise(TVec4 &v)	
{
	v /= len(v);
}


#endif
