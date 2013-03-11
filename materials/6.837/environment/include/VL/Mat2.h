/*
	File:			Mat2.h

	Function:		Defines a 2 x 2 matrix.
					
	Author(s):		Andrew Willmott

	Copyright:		(c) 1995-2000, Andrew Willmott
 */

#ifndef __Mat2__
#define __Mat2__

#include "vl/VL.h"
// Defines the actual type for TMat2 etc.
#include "vl/Vec2.h"


// --- Mat2 Class -------------------------------------------------------------

class TMat2 
{
public:
	
	// Constructors

	inline 		TMat2();
	inline 		TMat2(TMReal a, TMReal b, TMReal c, TMReal d);// Create from rows
	inline 		TMat2(const TMat2 &m);						// Copy constructor
	inline		TMat2(ZeroOrOne k);
	inline		TMat2(Block k);
	
	// Accessor functions
	
	inline Int			Rows() const { return(2); };
	inline Int			Cols() const { return(2); };
		
	inline TMVec2    	&operator [] (Int i);
	inline const TMVec2	&operator [] (Int i) const;

	inline TMReal 		*Ref() const;				// Return pointer to data

	// Assignment operators

	inline TMat2 		&operator =  (const TMat2 &m); 
	inline TMat2 		&operator =  (ZeroOrOne k); 
	inline TMat2 		&operator =  (Block k); 
	inline TMat2 		&operator += (const TMat2 &m);	   
	inline TMat2 		&operator -= (const TMat2 &m);	    
	inline TMat2 		&operator *= (const TMat2 &m);	    
	inline TMat2 		&operator *= (TMReal s);	    		
	inline TMat2 		&operator /= (TMReal s);	    		
	
	// Comparison operators
	
	Bool				operator == (const TMat2 &m) const;	// M == N?
	Bool				operator != (const TMat2 &m) const;	// M != N?
	
	// Arithmetic operators
	
	inline TMat2		operator + (const TMat2 &m) const;	// M + N
	inline TMat2		operator - (const TMat2 &m) const;	// M - N
	inline TMat2		operator - () const;				// -M
	inline TMat2		operator * (const TMat2 &m) const;	// M * N
	inline TMat2		operator * (TMReal s) const;		// M * s
	inline TMat2		operator / (TMReal s) const;		// M / s
	
	// Initialisers
	
	inline Void			MakeZero();							// Zero matrix
	inline Void			MakeDiag(TMReal k = vl_one);		// I
	inline Void			MakeBlock(TMReal k = vl_one);		// all elts=k
	
	// Vector Transformations
	
	TMat2& 				MakeRot(Real theta);	  		  
	TMat2& 				MakeScale(const TMVec2 &s);

protected:	
	// Private...
	TMVec2	row[2];		// Rows of the matrix
};


// --- Matrix operators -------------------------------------------------------


inline TMVec2	&operator *= (TMVec2 &v, const TMat2 &m);	// v *= m
inline TMVec2	operator * (const TMat2 &m, const TMVec2 &v);// m * v
inline TMVec2	operator * (const TMVec2 &v, const TMat2 &m);// v * m
inline TMat2	operator * (TMReal s, const TMat2 &m);		// s * m

inline TMat2	trans(const TMat2 &m);				// Transpose
inline TMReal	trace(const TMat2 &m);				// Trace
inline TMat2	adj(const TMat2 &m);				// Adjoint
TMReal			det(const TMat2 &m);				// Determinant
TMat2			inv(const TMat2 &m);				// Inverse
TMat2			oprod(const TMVec2 &a, const TMVec2 &b);
													// Outer product

// The xform functions help avoid dependence on whether row or column
// vectors are used to represent points and vectors.
inline TVec2	xform(const TMat2 &m, const TVec2 &v); // Transform of v by m
inline TMat2	xform(const TMat2 &m, const TMat2 &n); // Xform v -> m(n(v))

std::ostream			&operator << (std::ostream &s, const TMat2 &m);
std::istream			&operator >> (std::istream &s, TMat2 &m);


// --- Inlines ----------------------------------------------------------------


inline TMVec2 &TMat2::operator [] (Int i)
{
	CheckRange(i, 0, 2, "(Mat2::[i]) index out of range");
    return(row[i]);
}

inline const TMVec2 &TMat2::operator [] (Int i) const
{
	CheckRange(i, 0, 2, "(Mat2::[i]) index out of range");
    return(row[i]);
}

inline TMReal *TMat2::Ref() const
{
	return((TMReal*) row);
}

inline TMat2::TMat2()
{
}

inline TMat2::TMat2(TMReal a, TMReal b, TMReal c, TMReal d)
{
	row[0][0] = a;	row[0][1] = b;
	row[1][0] = c;	row[1][1] = d;
}

inline TMat2::TMat2(const TMat2 &m)
{
	row[0] = m[0];
	row[1] = m[1];
}


inline Void TMat2::MakeZero()
{
	row[0][0] = vl_zero; row[0][1] = vl_zero;
	row[1][0] = vl_zero; row[1][1] = vl_zero;
}

inline Void TMat2::MakeDiag(TMReal k)
{
	row[0][0] = k; 			row[0][1] = vl_zero;
	row[1][0] = vl_zero;	row[1][1] = k;
}

inline Void TMat2::MakeBlock(TMReal k)
{
	row[0][0] = k; row[0][1] = k;
	row[1][0] = k; row[1][1] = k;
}

inline TMat2::TMat2(ZeroOrOne k)
{
	MakeDiag(k);
}

inline TMat2::TMat2(Block k)
{
	MakeBlock((ZeroOrOne) k);
}

inline TMat2 &TMat2::operator = (ZeroOrOne k)
{
	MakeDiag(k);

	return(SELF);
}
	  
inline TMat2 &TMat2::operator = (Block k)
{
	MakeBlock((ZeroOrOne) k);

	return(SELF);
}
	  
inline TMat2 &TMat2::operator = (const TMat2 &m)
{
	row[0] = m[0];
	row[1] = m[1];
	
	return(SELF);
}
	  
inline TMat2 &TMat2::operator += (const TMat2 &m)
{
	row[0] += m[0];
	row[1] += m[1];
	
	return(SELF);
}

inline TMat2 &TMat2::operator -= (const TMat2 &m)
{
	row[0] -= m[0];
	row[1] -= m[1];

	return(SELF);
}

inline TMat2 &TMat2::operator *= (const TMat2 &m)
{
	SELF = SELF * m; 

	return(SELF);
}

inline TMat2 &TMat2::operator *= (TMReal s)
{
	row[0] *= s;
	row[1] *= s;

	return(SELF);
}

inline TMat2 &TMat2::operator /= (TMReal s)
{
	row[0] /= s;
	row[1] /= s;

	return(SELF);
}


inline TMat2 TMat2::operator + (const TMat2 &m) const
{
	TMat2 result;
	
	result[0] = row[0] + m[0];
	result[1] = row[1] + m[1];

	return(result);	
}

inline TMat2 TMat2::operator - (const TMat2 &m) const
{
	TMat2 result;
	
	result[0] = row[0] - m[0];
	result[1] = row[1] - m[1];

	return(result);	
}

inline TMat2 TMat2::operator - () const
{
	TMat2 result;
	
	result[0] = -row[0];
	result[1] = -row[1];

	return(result);	
}

inline TMat2 TMat2::operator * (const TMat2 &m) const
{
#define N(x,y) row[x][y]
#define M(x,y) m.row[x][y]
#define R(x,y) result[x][y]

	TMat2 result;
	
	R(0,0) = N(0,0) * M(0,0) + N(0,1) * M(1,0);
	R(0,1) = N(0,0) * M(0,1) + N(0,1) * M(1,1);
	R(1,0) = N(1,0) * M(0,0) + N(1,1) * M(1,0);
	R(1,1) = N(1,0) * M(0,1) + N(1,1) * M(1,1);

	return(result);	
	
#undef N
#undef M
#undef R
}

inline TMat2 TMat2::operator * (TMReal s) const
{
	TMat2 result;
	
	result[0] = row[0] * s;
	result[1] = row[1] * s;

	return(result);	
}

inline TMat2 TMat2::operator / (TMReal s) const
{
	TMat2 result;
	
	result[0] = row[0] / s;
	result[1] = row[1] / s;

	return(result);	
}

inline TMat2  operator *  (TMReal s, const TMat2 &m)
{
	return(m * s);
}

inline TMVec2 operator * (const TMat2 &m, const TMVec2 &v)
{
	TMVec2 result;

	result[0] = m[0][0] * v[0] + m[0][1] * v[1];
	result[1] = m[1][0] * v[0] + m[1][1] * v[1];

	return(result);
}

inline TMVec2 operator * (const TMVec2 &v, const TMat2 &m)			
{
	TMVec2 result;

	result[0] = v[0] * m[0][0] + v[1] * m[1][0];
	result[1] = v[0] * m[0][1] + v[1] * m[1][1];

	return(result);
}

inline TMVec2 &operator *= (TMVec2 &v, const TMat2 &m)		
{
	TMReal t;
	
	t    = v[0] * m[0][0] + v[1] * m[1][0];
	v[1] = v[0] * m[0][1] + v[1] * m[1][1];
	v[0] = t;

	return(v);
}


inline TMat2 trans(const TMat2 &m)
{
	TMat2 result;

	result[0][0] = m[0][0]; result[0][1] = m[1][0]; 
	result[1][0] = m[0][1]; result[1][1] = m[1][1]; 
		
	return(result);
}

inline TMReal trace(const TMat2 &m)
{
	return(m[0][0] + m[1][1]);
}
			
inline TMat2 adj(const TMat2 &m)			
{
	TMat2 result;

	result[0] =  cross(m[1]);
	result[1] = -cross(m[0]);
			
	return(result);
}

#ifdef VL_ROW_ORIENT
inline TVec2 xform(const TMat2 &m, const TVec2 &v)
{ return(v * m); }
inline TMat2 xform(const TMat2 &m, const TMat2 &n)
{ return(n * m); }
#else
inline TVec2 xform(const TMat2 &m, const TVec2 &v)
{ return(m * v); }
inline TMat2 xform(const TMat2 &m, const TMat2 &n)
{ return(m * n); }
#endif

#endif
