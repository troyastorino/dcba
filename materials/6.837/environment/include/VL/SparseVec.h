/*
	File:			SparseVec.h

	Function:		Defines a sparse vector.
					
	Author(s):		Andrew Willmott

	Copyright:		(c) 1995-2000, Andrew Willmott
 */

#ifndef __SparseVec__
#define __SparseVec__

#include "vl/VL.h"
#include "vl/Vec.h"
#include "vl/SubSVec.h"
#include "cl/Array.h"
#include <iostream>

#define TSVList Array< TSparsePair >

class TSparsePair
{	
public:
	
	TSparsePair() {};
	TSparsePair(Int i, TVReal elt) : index(i), elt(elt) {};
	
	Int		index;		// Index
	TVReal	elt;		// Non-zero element at that index
	
	Bool operator == (const TSparsePair &sp) const
	{	return(index == sp.index && elt == sp.elt); };
	Bool operator != (const TSparsePair &sp) const
	{	return(index != sp.index || elt != sp.elt); };
	
	friend std::ostream &operator << (std::ostream &s, const TSparsePair &sp);
	friend std::istream &operator >> (std::istream &s, TSparsePair &sp);
};

class TSubSVec;
class TSVIter;

class TSparseVec : public TSVList
/*
	Function:	Provides a sparse vector class

	NOTE: SparseVecs can be manipulated as follows:
	
	1] Use SetElts() -- e.g., 
		sv.SetSize(5); sv.SetElts(1, 1.0, 4, 4.0, VL_SV_END);
		Sets sv to [0.0, 1.0, 0.0, 0.0 4.0]
	
	2] Use the SVIter iterator. (See description below.)
			
	3] Use the Overlay method: a.Overlay(b) performs a[i] = b[i] for all non-zero b[i].

	4] Direct access:
	 	SetNumElts(size), Begin(), AddElt()/AddNZElt() new element pairs in
	 	order, then End(). (Use AddNZElt if you know the element will 
	 	be non-zero.)
	
	5] As a last resort, use Get()/Set(). 
		These calls are not efficient for multiple accesses. 
*/
{
public:			
	TSparseVec();						// Null vector: space allocated later
	TSparseVec(Int n);										
	TSparseVec(Int n, Int indices[], TVReal elts[]);	
	TSparseVec(Int n, Int idx0, double elt0, ...);				
	TSparseVec(const TSparseVec &v);	// Copy constructor
	TSparseVec(const TSubSVec &v);			 	
	TSparseVec(const TVec &v);			 	
	TSparseVec(Int n, ZeroOrOne);		// Zero or all-ones vector	
	TSparseVec(Int n, Axis a);			// Unit vector
	
   ~TSparseVec();			 			// Destructor

	// Accessor functions
		
	Int			Elts() const { return elts; };

	TSparseVec  &operator =  (const TSparseVec &v);
	TSparseVec	&operator =  (ZeroOrOne k) { MakeBlock(k); return(SELF); };	    				
	TSparseVec	&operator =  (Axis k) { MakeUnit(k); return(SELF); };	    				
	TSparseVec  &operator =  (const TSubSVec &v);
	TSparseVec  &operator =  (const TVec &v);
	
	Void		SetSize(Int n);

	//	Sparse methods
	
	TSparseVec	Overlay(const TSparseVec &a) const;
	Void		SetElts(Int idx0, double elt0, ...);
	Void		Set(Int index, TVReal elt);
	TVReal		Get(Int index) const;
	
	//	Vector initialisers
	
	Void		MakeZero();
	Void		MakeUnit(Int i, TVReal k = vl_one);
	Void		MakeBlock(TVReal k = vl_one);

	//	Low level Utils

	Void		SetNumElts(Int n)
				{ elts = n; };
	Void		Begin()
				{ Clear(); };
	Void		AddElt(Int i, TVReal a)
				{ if (len(a) > fuzz) {Add(); Last().index = i; Last().elt = a;} };
	Void		AddNZElt(Int i, TVReal a)
				{ Add(); Last().index = i; Last().elt = a; };
	Void		AddNZElt(const TSparsePair &p)
				{ Append(p); };
	Void		End()
				{ Add(); Last().index = VL_SV_MAX_INDEX; };

	//	Settings

	static Void	SetCompactPrint(Bool on);
	static Bool IsCompact() { return compactPrint; };
	static Void	SetFuzz(TVReal fuzz);
	static Bool	IsNonZero(TVReal a)
				{ return(len(a) > fuzz); };

protected: 
	// Private...
	static Bool		compactPrint;	//	 Print in normal or compact (only non-zero elts) style
	static TVReal	fuzz;			//   x s.t. |x| <= fuzz is considered zero.
	Int				elts;
};
	
class TSVIter
/*
	Function:	Useful for iterating over a sparse vector.
	
	Data() :	returns the current element's data
	Index() :	returns the current element's index

	1] Use Begin(), Inc(), AtEnd() to iterate over the non-zero elements 
	    of the vector:
	    
		// sv = sv * 2
		SVIter[fd]    j;
		SparseVec[fd] sv;
		for (j.Begin(sv); !j.AtEnd(); j.Inc())
			j.Data() *= 2.0;		

	2]	Use one of the following:

		Inc(Int i)	  moves on to elt i, where i will increase by 1 on each call
		IncTo(Int i)  moves on to elt i, where i will increase monotonically

		within another for-loop to access the elements of the sparse vector
		corresponding to i. For example:

		// v = v + sv
		for (j.Begin(sv), i = 0; i < v.NumItems(); i++)
		{
			j.Inc(i);
			if (j.Exists())
				v[i] += j.Data();
		}

		// a += dot(sv1, sv2)
		for (j.Begin(sv2), k.Begin(sv1); !k.AtEnd(); k.Inc())
		{
			j.IncTo(k.Index());		// find corresponding elt in sv2
			if (j.Exists())
				a += j.Data() * k.Data();
		}
*/
{
public:
	TSVIter() {};
	TSVIter(const TSparseVec &sv) : pairIdx(0), list(&sv) {};

	Void	Begin()
			{ pairIdx = 0; };
			// move to the beginning of the current sparse vector
			
	Void	Begin(const TSparseVec &sv)
			{ pairIdx = 0; list = &sv; };
			// move to the beginning of sparse vector sv

	Void	Inc()
			{ pairIdx++; };
			// move to the next non-zero element in the vector
			
	Bool	AtEnd()
			{ return(pairIdx >= list->NumItems() - 1); };
			// returns true if we're at the end of the vector

	TVReal	Data()
			{ return(list->Item(pairIdx).elt); };
			// WARNING: only call this if you *know* the element is non-zero
	Int		Index()
			{ return(list->Item(pairIdx).index); };
	TSparsePair	Pair()
			{ return(list->Item(pairIdx)); };

	Void	Inc(Int i)
			{ if (i > list->Item(pairIdx).index) pairIdx++; };
			// Move on to the element indicated by i. i must increase by
			// 1 on each subsequent call.

	Bool	IncTo(Int i)
			{ OrdFindBinary(i); return(i == list->Item(pairIdx).index); };
			// Move on to the element with index i.
			// returns true if element i is non-zero.
	
	Bool	Exists(Int i)
			{ return(i == list->Item(pairIdx).index); };

	TVReal	Elt(Int i)
			{ 
				return(Exists(i) ? Data() : 0.0);
			};
			// call this if you don't want to bother with Exists()/Data()

	Int		PairIdx()
			{ return(pairIdx); };
			
	Void OrdFindBinary(Int i);	// Find the pair with index i using binary search
	Void OrdFindLinear(Int i);	// Find the pair with index i using linear search

protected:
	Int				pairIdx;	// current index into sparse list
	const TSVList	*list;		// sparse list
};


// --- Vec In-Place operators -------------------------------------------------

TSparseVec		&operator += (TSparseVec &a, const TSparseVec &b);
TSparseVec		&operator -= (TSparseVec &a, const TSparseVec &b);
TSparseVec		&operator *= (TSparseVec &a, const TSparseVec &b);
TSparseVec		&operator *= (TSparseVec &v, TVReal s);
TSparseVec		&operator /= (TSparseVec &a, const TSparseVec &b);
TSparseVec		&operator /= (TSparseVec &v, TVReal s);

// --- Vec Comparison Operators -----------------------------------------------

Bool	 		operator == (const TSparseVec &a, const TSparseVec &b);
Bool			operator != (const TSparseVec &a, const TSparseVec &b);

// --- Vec Arithmetic Operators -----------------------------------------------

TSparseVec		operator + (const TSparseVec &a, const TSparseVec &b);
TSparseVec		operator - (const TSparseVec &a, const TSparseVec &b);
TSparseVec		operator - (const TSparseVec &v);
TSparseVec 		operator * (const TSparseVec &a, const TSparseVec &b);		
TSparseVec		operator * (const TSparseVec &v, TVReal s);
TSparseVec		operator / (const TSparseVec &a, const TSparseVec &b);
TSparseVec		operator / (const TSparseVec &v, TVReal s);
TSparseVec 		operator * (TVReal s, const TSparseVec &v);


TVReal			dot(const TSparseVec &a, const TSparseVec &b);
TVReal			dot(const TSparseVec &a, const TVec &b);
TVReal 			len(const TSparseVec &v);
TVReal 			sqrlen(const TSparseVec &v);
TSparseVec 		norm(const TSparseVec &v);

// --- Vec Input & Output -----------------------------------------------------

std::ostream 		&operator << (std::ostream &s, const TSparseVec &v);
std::istream 		&operator >> (std::istream &s, TSparseVec &v);


#endif
