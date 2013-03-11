/*
	File:			SubVec.h

	Function:		Defines a scatter-gather vector, i.e., a subvector of another vector, 
					or the row, column or diagonal of a matrix.
					
	Author(s):		Andrew Willmott

	Copyright:		(c) 1995-2000, Andrew Willmott
 */

#ifndef __SubVec__
#define __SubVec__

#include "vl/VL.h"

class TVec;
 
class TSubVec
{
public:

					TSubVec(Int n, Int span, TVReal data[]);
					TSubVec(const TSubVec &v);
					TSubVec(const TVec &v);

	inline Int		Elts() const { return(elts); };

	inline TVReal	&operator [] (Int i);
	inline TVReal	operator [] (Int i) const;

	TSubVec			&operator = (const TSubVec &v);
	TSubVec			&operator = (const TVec &v);

	// In-Place operators

	TSubVec			&operator += (const TSubVec &b);
	TSubVec			&operator -= (const TSubVec &b);
	TSubVec			&operator *= (const TSubVec &b);
	TSubVec			&operator *= (TVReal s);
	TSubVec			&operator /= (const TSubVec &b);
	TSubVec			&operator /= (TVReal s);

	Int				elts;
	Int				span;
	TVReal			*data; 
};

inline TSubVec	sub(const TSubVec &v, Int start, Int length); 	
inline TSubVec	first(const TSubVec &v, Int length); 	
inline TSubVec	last(const TSubVec &v, Int length); 	


// --- Inlines ----------------------------------------------------------------

inline TVReal &TSubVec::operator [] (Int i)
{
	CheckRange(i, 0, elts, "Vec::[i]");
	
    return(data[i * span]);
}

inline TVReal TSubVec::operator [] (Int i) const
{
	CheckRange(i, 0, elts, "Vec::[i]");

    return(data[i * span]);
}

inline TSubVec sub(const TSubVec &v, Int start, Int length)
{
	Assert(start >= 0 && length > 0 && start + length <= v.Elts(),
		"(sub(Vec)) illegal subset of vector");

	return(TSubVec(length, v.span, v.data + start * v.span));
}

inline TSubVec first(const TSubVec &v, Int length)
{
	Assert(length > 0 && length <= v.Elts(),
		"(first(Vec)) illegal subset of vector");

	return(TSubVec(length, v.span, v.data));
}

inline TSubVec last(const TSubVec &v, Int length)
{
	Assert(length > 0 && length <= v.Elts(),
		"(last(Vec)) illegal subset of vector");

	return(TSubVec(length, v.span, v.data + (v.elts - length) * v.span));
}

#endif
