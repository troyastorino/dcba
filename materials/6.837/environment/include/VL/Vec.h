/*
	File:			Vec.h

	Function:		Defines a generic resizeable vector.
					
	Author(s):		Andrew Willmott

	Copyright:		(c) 1995-2000, Andrew Willmott
 */

#ifndef __Vec__
#define __Vec__

#include "vl/VL.h"
// Defines the actual type for TVec etc.

#include "vl/Vec2.h"
#include "vl/Vec3.h"
#include "vl/Vec4.h"
#ifndef __SVL__
#include "vl/SubVec.h"
#endif
#include <iostream>


// --- Vec Class --------------------------------------------------------------


class TVec
{
public:

	// Constructors
	
	inline TVec();						// Null vector: space allocated later
	inline TVec(Int n);										
	TVec(Int n, double elt0, ...);		// Vec(3, 1.1, 2.0, 3.4)
	inline TVec(Int n, TVReal *data);	// Vector pointer...
	TVec(const TVec &v);			 	// Copy constructor
#ifndef __SVL__
	TVec(const TSubVec &v);
#endif
	TVec(const TVec2 &v);			 	
	TVec(const TVec3 &v);			 	
	TVec(const TVec4 &v);			 	
	TVec(Int n, ZeroOrOne);				// Zero or all-ones vector	
	TVec(Int n, Axis a);				// Unit vector
	
   ~TVec();			 					// Destructor
	
	// Accessor functions
		
	inline Int		Elts() const;

	inline TVReal    &operator [] (Int i);	
	inline TVReal 	operator [] (Int i) const;			  

	Void			SetSize(Int n);					// resize the vector

	inline TVReal	*Ref() const;					// Return pointer to data
	
	// Assignment operators
	
	TVec	 		&operator =  (const TVec &v);	// v = a etc.
#ifndef __SVL__
	TVec			&operator =  (const TSubVec &v);		
#endif
	TVec			&operator =  (ZeroOrOne k);
	TVec			&operator =  (Axis a);
	TVec 			&operator =  (const TVec2 &v);			
	TVec	 		&operator =  (const TVec3 &v);			
	TVec			&operator =  (const TVec4 &v);				
		
	// In-Place operators

	TVec			&operator += (const TVec &v);
	TVec			&operator -= (const TVec &v);
	TVec			&operator *= (const TVec &v);
	TVec			&operator *= (TVReal s);
	TVec			&operator /= (const TVec &v);
	TVec			&operator /= (TVReal s);
	
	//	Vector initialisers
	
	TVec			&MakeZero();
	TVec			&MakeUnit(Int i, TVReal k = vl_one);
	TVec			&MakeBlock(TVReal k = vl_one);

	inline TVec		&Normalise();					// normalise vector
	TVec			&Clamp(Real fuzz);
	TVec			&Clamp();
	
	Bool			IsRef() const { return(elts & VL_REF_FLAG); };

protected:
	TVReal			*data; 
	UInt32			elts;
};



// --- Vec Comparison Operators -----------------------------------------------

Bool		operator == (const TVec &a, const TVec &b);
Bool		operator != (const TVec &a, const TVec &b);

// --- Vec Arithmetic Operators -----------------------------------------------

TVec		operator + (const TVec &a, const TVec &b);
TVec		operator - (const TVec &a, const TVec &b);
TVec		operator - (const TVec &v);
TVec		operator * (const TVec &a, const TVec &b);		
TVec		operator * (const TVec &v, TVReal s);
TVec		operator / (const TVec &a, const TVec &b);
TVec		operator / (const TVec &v, TVReal s);
TVec		operator * (TVReal s, const TVec &v);

TVReal			dot(const TVec &a, const TVec &b);	// v . a
inline TVReal	len(const TVec &v);					// || v ||
inline TVReal	sqrlen(const TVec &v);				// v . v
inline TVec		norm(const TVec &v);				// v / || v ||
inline Void		normalise(TVec &v);					// v = norm(v)
TVec 			clamped(const TVec &v, Real fuzz);
TVec 			clamped(const TVec &v);

// --- Vec Input & Output -----------------------------------------------------

std::ostream		&operator << (std::ostream &s, const TVec &v);
std::istream		&operator >> (std::istream &s, TVec &v);

// --- Sub-vector functions ---------------------------------------------------

inline TVec	sub(const TVec &v, Int start, Int length); 	
inline TVec	first(const TVec &v, Int length); 	
inline TVec	last(const TVec &v, Int length); 	


// --- Vec inlines ------------------------------------------------------------


inline TVec::TVec() : data(0), elts(0)
{
}

inline TVec::TVec(Int n) : elts(n)
{
	Assert(n > 0,"(Vec) illegal vector size");

	data = new TVReal[n];
}

inline TVec::TVec(Int n, TVReal *data) : data(data), elts(n | VL_REF_FLAG)
{
}

inline Int TVec::Elts() const
{
	return(elts & VL_REF_MASK);
}

inline TVReal &TVec::operator [] (Int i)
{
	CheckRange(i, 0, Elts(), "Vec::[i]");
	
    return(data[i]);
}

inline TVReal TVec::operator [] (Int i) const
{
	CheckRange(i, 0, Elts(), "Vec::[i]");

    return(data[i]);
}

inline TVReal *TVec::Ref() const
{
	return(data);
}

inline TVec &TVec::operator = (ZeroOrOne k)
{
	MakeBlock(k);

	return(SELF);
}

inline TVec &TVec::operator = (Axis a)
{
	MakeUnit(a);

	return(SELF);
}

inline TVReal len(const TVec &v)
{
	return(sqrt(dot(v, v)));
}

inline TVReal sqrlen(const TVec &v)
{
	return(dot(v, v));
}

inline TVec norm(const TVec &v)	
{
	Assert(sqrlen(v) > 0.0, "normalising length-zero vector");
	return(v / len(v));
}

inline Void normalise(TVec &v)
{
	v /= len(v);
}

inline TVec sub(const TVec &v, Int start, Int length)
{
	Assert(start >= 0 && length > 0 && start + length <= v.Elts(),
		"(sub(Vec)) illegal subset of vector");

	return(TVec(length, v.Ref() + start));
}

inline TVec first(const TVec &v, Int length)
{
	Assert(length > 0 && length <= v.Elts(),
		"(first(Vec)) illegal subset of vector");

	return(TVec(length, v.Ref()));
}

inline TVec last(const TVec &v, Int length)
{
	Assert(length > 0 && length <= v.Elts(),
		"(last(Vec)) illegal subset of vector");

	return(TVec(length, v.Ref() + v.Elts() - length));
}

inline TVec &TVec::Normalise()
{
	Assert(sqrlen(SELF) > 0.0, "normalising length-zero vector");
	SELF /= len(SELF);
	return(SELF);
}


#endif

