/*
	File:			Mat.h

	Function:		Defines a generic resizeable matrix.
					
	Author(s):		Andrew Willmott

	Copyright:		(c) 1995-2000, Andrew Willmott
 */

#ifndef __Mat__
#define __Mat__

#include "vl/VL.h"
// Defines the actual type for TMat etc.

#include "vl/Vec.h"
#include "vl/Mat2.h"
#include "vl/Mat3.h"
#include "vl/Mat4.h"
#ifndef __SVL__
#include "vl/SubMat.h"
#endif

#include <iostream>


// --- Mat Class --------------------------------------------------------------


class TMat
{
public:
	
	// Constructors
	
	inline TMat();							// Null matrix: no space allocated
	inline TMat(Int rows, Int cols);		// Ordinary uninitialised matrix
	TMat(Int rows, Int cols, Double elt0 ...);// Mat(2, 2, 1.0, 2.0, 3.0, 4.0)
	inline TMat(Int nrows, Int ncols, TMReal *ndata);// Create reference matrix
	TMat(const TMat &m);					// Copy constructor
#ifndef __SVL__
	TMat(const TSubMat &m);					// Conversion constructors...
#endif
	TMat(const TMat2 &m);			
	TMat(const TMat3 &m);			
	TMat(const TMat4 &m);			
	TMat(Int rows, Int cols, ZeroOrOne k);	// I * k
	TMat(Int rows, Int cols, Block k);		// block matrix (m[i][j] = k)

	inline ~TMat();			
  	
	// Accessor methods
	
	inline Int	Rows() const { return(rows & VL_REF_MASK); };
	inline Int	Cols() const { return(cols); };

	inline TMVec   	operator [] (Int i);		// Indexing by row
	inline TMVec   	operator [] (Int i) const;	// Indexing by row

	inline TMReal	&Elt(Int i, Int j);	// Indexing by Elt; no checking
	inline TMReal	Elt(Int i, Int j) const;
	
	Void 			SetSize(Int nrows, Int ncols);
	Void 			SetSize(const TMat &m);
	Bool			IsSquare() const
					{ return((rows & VL_REF_MASK) == cols); };

	inline TMReal 	*Ref() const;				// Return pointer to data

	// Assignment operators

	TMat 	   		&operator =  (const TMat &m);	// Assignment of a matrix
#ifndef __SVL__
	TMat 	   		&operator =  (const TSubMat &m);// Assignment of submatrix
#endif
	inline TMat		&operator =	 (ZeroOrOne k);		// Set to k * I...
	inline TMat		&operator =	 (Block k);			// Set to a block matrix...
	TMat 	   		&operator =  (const TMat2 &m);	
	TMat 	   		&operator =  (const TMat3 &m);	
	TMat 	   		&operator =  (const TMat4 &m);	

	// In-Place Operators

	TMat			&operator += (const TMat &m);
	TMat			&operator -= (const TMat &m);
	TMat			&operator *= (const TMat &m);
	TMat			&operator *= (TMReal s);
	TMat			&operator /= (TMReal s);

	//	Matrix initialisers
	
	Void			MakeZero();
	Void			MakeDiag(TMReal k);
	Void			MakeDiag();
	Void			MakeBlock(TMReal k);
	Void			MakeBlock();

	TMat			&Clamp(Real fuzz);
	TMat			&Clamp();

protected:
	// Private...
	TMReal			*data; 
	UInt			rows;
	UInt			cols;

	Bool			IsRef() { return(rows & VL_REF_FLAG); };
};


// --- Mat Comparison Operators -----------------------------------------------

Bool		operator == (const TMat &m, const TMat &n);
Bool		operator != (const TMat &m, const TMat &n);

// --- Mat Arithmetic Operators -----------------------------------------------

TMat		operator + (const TMat &m, const TMat &n);
TMat		operator - (const TMat &m, const TMat &n);
TMat		operator - (const TMat &m);
TMat		operator * (const TMat &m, const TMat &n);
TMat		operator * (const TMat &m, TMReal s);
inline TMat	operator * (TMReal s, const TMat &m);
TMat		operator / (const TMat &m, TMReal s);

TMVec		operator * (const TMat &m, const TMVec &v);
TMVec		operator * (const TMVec &v, const TMat &m);

TMat 		trans(const TMat &m);					// Transpose
TMReal		trace(const TMat &m);					// Trace
TMat		inv(const TMat &m, TMReal *determinant = 0, TMReal pEps = 1e-20);
													// Inverse
TMat		oprod(const TMVec &a, const TMVec &b);	// Outer product

TMat 		clamped(const TMat &m, Real fuzz);
TMat 		clamped(const TMat &m);

// --- Mat Input & Output -----------------------------------------------------

std::ostream		&operator << (std::ostream &s, const TMat &m);
std::istream		&operator >> (std::istream &s, TMat &m);


// --- Mat Inlines ------------------------------------------------------------


inline TMat::TMat() : data(0), rows(0), cols(0)
{
}

inline TMat::TMat(Int rows, Int cols) : rows(rows), cols(cols)
{
	Assert(rows > 0 && cols > 0, "(Mat) illegal matrix size");
	
	data = new TMReal[rows * cols];
}

inline TMat::TMat(Int nrows, Int ncols, TMReal *ndata) : 
	data(ndata), rows(nrows | VL_REF_FLAG), cols(ncols)
{
}

inline TMVec TMat::operator [] (Int i)
{
	CheckRange(i, 0, Rows(), "(Mat::[i]) i index out of range");
	
    return(TMVec(cols, data + i * cols));
}

inline TMVec TMat::operator [] (Int i) const
{
	CheckRange(i, 0, Rows(), "(Mat::[i]) i index out of range");
	
    return(TMVec(cols, data + i * cols));
}

inline TMReal &TMat::Elt(Int i, Int j)
{
	CheckRange(i, 0, Rows(), "(Mat::e(i,j)) i index out of range");
	CheckRange(j, 0, Cols(), "(Mat::e(i,j)) j index out of range");
	
    return(data[i * cols + j]);
}

inline TMReal TMat::Elt(Int i, Int j) const
{
	CheckRange(i, 0, Rows(), "(Mat::e(i,j)) i index out of range");
	CheckRange(j, 0, Cols(), "(Mat::e(i,j)) j index out of range");
	
    return(data[i * cols + j]);
}

inline TMReal *TMat::Ref() const
{
	return(data);
}

inline TMat operator * (TMReal s, const TMat &m)
{
	return(m * s);
}

inline TMat &TMat::operator = (ZeroOrOne k)
{
	MakeDiag(k);
	
	return(SELF);	
}

inline TMat &TMat::operator = (Block k)
{
	MakeBlock((ZeroOrOne) k);
	
	return(SELF);	
}

inline TMat::~TMat()
{
	if (!IsRef())
		delete[] data;
}

#endif
