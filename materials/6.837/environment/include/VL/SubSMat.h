/*
	File:			SubSMat.h

	Function:		Defines a scatter-gather sparse matrix, i.e., a submatrix of another sparse 
					matrix.
					
	Author(s):		Andrew Willmott

	Copyright:		(c) 1995-2000, Andrew Willmott
 */


#ifndef __SubSMat__
#define __SubSMat__

#include "vl/VL.h"
#include "vl/Mat.h"


// --- SubSMat Class ----------------------------------------------------------


class TSubSVec;
class TSparseVec;
class TSparseMat;

class TSubSMat
{
public:
	
	// Constructors
	
	TSubSMat(Int m, Int n, Int start, TMSparseVec *target);
	TSubSMat(const TSubSMat &m);			
  	
	// Accessor functions
	
	inline Int				Rows() const { return rows; };
	inline Int				Cols() const { return cols; };

	inline TMSubSVec		operator [] (Int i) const;		  

	// Assignment operators
	
	TSubSMat				&operator = (const TSubSMat &m);
	TSubSMat				&operator = (const TSparseMat &m);	
	TSubSMat				&operator = (const TMat &m);	

protected:

	Int						rows;
	Int						cols;
	Int						start;
	TMSparseVec				*target; 
};

TMSubSVec 	col(const TSparseMat &m, Int i);
TMSubSVec 	diag(const TSparseMat &m, Int diagNum);
					//	-i = diag. starting on row i, +i = diag. starting on col i
TSubSMat 	sub(const TSparseMat &m, Int top, Int left, Int nrows, Int ncols);
TSubSMat 	sub(const TSparseMat &m, Int nrows, Int ncols);


// --- SubSMat Inlines --------------------------------------------------------

#include "vl/SubSVec.h"
#include "vl/SparseVec.h"

inline TMSubSVec TSubSMat::operator [] (Int i) const
{
	Assert(i >= 0 && i < Rows(), "(SubSMat::[]) index out of range");
    return(sub(target[i], start, Cols()));
}

#endif

