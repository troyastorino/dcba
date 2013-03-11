/*
	File:			SubMat.h

	Function:		Defines a scatter-gather matrix, i.e., a submatrix of another matrix.
					
	Author(s):		Andrew Willmott

	Copyright:		(c) 1995-2000, Andrew Willmott
 */

#ifndef __SubMat__
#define __SubMat__

#include "vl/VL.h"
#include "vl/SubVec.h"

// --- SubMat Class -----------------------------------------------------------

class TMat;
class TVec;

class TSubMat
{
public:
	
	// Constructors
	
						TSubMat(Int m, Int n, Int span, TMReal data[]);
						TSubMat(const TSubMat &m);			
  	
	// Accessor functions
	
	Int					Rows() const { return rows; };
	Int					Cols() const { return cols; };

	inline TMVec		operator [] (Int i) const;		  

	inline TMReal		&Elt(Int i, Int j);
	inline TMReal		Elt(Int i, Int j) const;

	// Assignment operators
	
	TSubMat				&operator = (const TSubMat &m);	// Assignment of a matrix
	TSubMat				&operator = (const TMat &m);	

	Int					rows;
	Int					cols;
	Int					span;
	TMReal				*data; 
};


// --- Submatrix functions ----------------------------------------------------
	
TSubMat 	sub(const TMat &m, Int top, Int left, Int height, Int width);
TSubMat 	sub(const TMat &m, Int rows, Int cols);
TMSubVec	row(const TMat &m, Int i);
TMSubVec	col(const TMat &m, Int i);
TMSubVec	diag(const TMat &m, Int diagNum = 0);
//	-i = diag. starting on row i, +i = diag. starting on col i

TSubMat 	sub(const TSubMat &m, Int top, Int left, Int height, Int width);
TSubMat 	sub(const TSubMat &m, Int rows, Int cols);
TMSubVec	row(const TSubMat &m, Int i);
TMSubVec	col(const TSubMat &m, Int i);
TMSubVec	diag(const TSubMat &m, Int diagNum = 0);

// --- SubMat Inlines ---------------------------------------------------------


#include "vl/Vec.h"

inline TMVec TSubMat::operator [] (Int i) const
{
	CheckRange(i, 0, Rows(), "(SubMat::(i)) index out of range");
    return(TMVec(cols, data + i * span));
}

inline TMReal &TSubMat::Elt(Int i, Int j)
{
	CheckRange(i, 0, Rows(), "(SubMat::(i,j)) i index out of range");
	CheckRange(j, 0, Cols(), "(SubMat::(i,j)) j index out of range");
    return(data[i * span + j]);
}

inline TMReal TSubMat::Elt(Int i, Int j) const
{
	CheckRange(i, 0, Rows(), "(SubMat::(i,j)) i index out of range");
	CheckRange(j, 0, Cols(), "(SubMat::(i,j)) j index out of range");
    return(data[i * span + j]);
}

#endif

