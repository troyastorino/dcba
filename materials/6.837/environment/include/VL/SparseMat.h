/*
	File:			SparseMat.h

	Function:		Defines a sparse matrix.
					
	Author(s):		Andrew Willmott

	Copyright:		(c) 1995-2000, Andrew Willmott
 */

#ifndef __SparseMat__
#define __SparseMat__

#include "vl/VL.h"
#include "vl/Mat.h"
#include "vl/SparseVec.h"
#include "vl/SubSMat.h"
#include <iostream>


// --- SparseMat Class --------------------------------------------------------


class TSparseMat
{
public:
	
	// Constructors
	
	TSparseMat();							// Null matrix: no space allocated
	TSparseMat(Int rows, Int cols);			// Ordinary uninitialised matrix
	TSparseMat(Int rows, Int cols, ZeroOrOne k); // I * k
	TSparseMat(Int rows, Int cols, Block k);// block matrix (m[i][j] = k)
	TSparseMat(const TSparseMat &m);		// Copy constructor	
	TSparseMat(const TSubSMat &m);			// Conversion constructors...	
	TSparseMat(const TMat &m);			

 	~TSparseMat();			
  	
	// Accessor functions
	
	inline Int			Rows() const { return rows; };
	inline Int			Cols() const { return cols; };

	inline TMSparseVec	&operator [] (Int i);		// Indexing by row
	inline const TMSparseVec
						&operator [] (Int i) const;	// no checking	  

	inline TMSparseVec	*Ref() const;				// Return pointer to data

	// Assignment operators
	
	TSparseMat 	   		&operator =  (const TSparseMat &m);		
	TSparseMat 	   		&operator =  (const TSubSMat &m);	
	TSparseMat 	   		&operator =  (const TMat &m);	
	inline TSparseMat	&operator =  (ZeroOrOne k);
	inline TSparseMat	&operator =  (Block k);

	Void				SetSize(Int m, Int n);
	Bool				IsSquare() const { return(rows == cols); };

	//	Matrix initialisers
	
	Void				MakeZero();
	Void				MakeDiag(TMReal k = vl_one);
	Void				MakeBlock(TMReal k = vl_one);

protected:
	//	Private ...
	TMSparseVec	*row;
	Int			rows;
	Int			cols;
};


// --- SparseMat In-Place Operators -------------------------------------------

TSparseMat	&operator += (TSparseMat &m, const TSparseMat &n);
TSparseMat	&operator -= (TSparseMat &m, const TSparseMat &n);
TSparseMat	&operator *= (TSparseMat &m, const TSparseMat &n);
TSparseMat	&operator *= (TSparseMat &m, TMReal s);
TSparseMat	&operator /= (TSparseMat &m, TMReal s);

// --- SparseMat Comparison Operators -----------------------------------------

Bool		operator == (const TSparseMat &m, const TSparseMat &n);
Bool		operator != (const TSparseMat &m, const TSparseMat &n);

// --- SparseMat Arithmetic Operators -----------------------------------------

TSparseMat	operator + (const TSparseMat &m, const TSparseMat &n);
TSparseMat	operator - (const TSparseMat &m, const TSparseMat &n);
TSparseMat	operator - (const TSparseMat &m);
TSparseMat	operator * (const TSparseMat &m, const TSparseMat &n);
TSparseMat	operator * (const TSparseMat &m, TMReal s);
TSparseMat	operator / (const TSparseMat &m, TMReal s);

TSparseVec	&operator *= (TSparseVec &v, const TSparseMat &m);
TMSparseVec	operator * (const TSparseVec &v, const TSparseMat &m);
TSparseVec	operator * (const TSparseMat &m, const TSparseVec &v);
TMVec 		&operator *= (TMVec &v, const TSparseMat &m);
TMVec		operator * (const TMVec &v, const TSparseMat &m);
TMVec		operator * (const TSparseMat &m, const TMVec &v);

TSparseMat	trans(const TSparseMat &m);
TMReal		trace(const TSparseMat &m);
TSparseMat	oprod(const TSparseVec &a, const TSparseVec &b);
TSparseMat	oprods(const TVec &a, const TVec &b);
TSparseMat	inv(const TSparseMat &m, TMReal *determinant = 0, TMReal pEps = 1e-20);

// --- Mat Input & Output -----------------------------------------------------

std::ostream		&operator << (std::ostream &s, const TSparseMat &m);
std::istream		&operator >> (std::istream &s, TSparseMat &m);

// --- SparseMat Inlines ------------------------------------------------------


inline TMSparseVec &TSparseMat::operator [] (Int i)
{
	CheckRange(i, 0, Rows(), "(SparseMat::[i]) i index out of range");
	
    return(row[i]);
}

inline const TMSparseVec &TSparseMat::operator [] (Int i) const
{
	CheckRange(i, 0, Rows(), "(SparseMat::[i]) i index out of range");
	
    return(row[i]);
}

inline TMSparseVec *TSparseMat::Ref() const
{
	return(row);
}

inline TSparseMat &TSparseMat::operator = (ZeroOrOne k)
{
	MakeDiag(k);
	return(SELF);
};

inline TSparseMat &TSparseMat::operator = (Block k)
{
	MakeBlock((ZeroOrOne) k);
	return(SELF);
};

#endif

