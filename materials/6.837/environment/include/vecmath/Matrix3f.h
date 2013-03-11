#ifndef MATRIX3F_H
#define MATRIX3F_H

#include <cstdio>

class Matrix2f;
class Quat4f;
class Vector3f;

// 3x3 Matrix, stored in column major order (OpenGL style)
class Matrix3f
{
public:

    // 3x3 zero matrix
	Matrix3f();
	Matrix3f( float m00, float m01, float m02,
		float m10, float m11, float m12,
		float m20, float m21, float m22 );

	// setColumns = true ==> sets the columns of the matrix to be [v0 v1 v2]
	// otherwise, sets the rows
	Matrix3f( const Vector3f& v0, const Vector3f& v1, const Vector3f& v2, bool setColumns = true );

	Matrix3f( const Matrix3f& rm ); // copy constructor
	Matrix3f& operator = ( const Matrix3f& rm ); // assignment operator
	// no destructor necessary

	// access element at (i,j)
	const float& operator () ( int i, int j ) const;
	float& operator () ( int i, int j );

	Vector3f getRow( int i ) const;
	void setRow( int i, const Vector3f& v );

	Vector3f getCol( int j ) const;
	void setCol( int j, const Vector3f& v );

	// gets the 2x2 submatrix of this matrix to m
	// starting with upper left corner at (i0, j0)
	Matrix2f getSubmatrix2x2( int i0, int j0 ) const;

	// sets a 2x2 submatrix of this matrix to m
	// starting with upper left corner at (i0, j0)
	void setSubmatrix2x2( int i0, int j0, const Matrix2f& m );

    // Computes the 3x3 matrix determinant
	float determinant() const;

    // Returns the inverse of this matrix.
    // if pbIsSingular is not NULL, then
    // it is set to whether or not this matrix was singular.
    // A matrix is considered singular if its determinant has
    // absolute value less than epsilon.
	Matrix3f inverse( bool* pbIsSingular = NULL, float epsilon = 0.f ) const; 

    // in-place transpose
	void transpose();

    // Returns the transpose of this matrix
	Matrix3f transposed() const;

	// ---- Utility ----
	operator const float* () const; // automatic type conversion for GL
	operator float* (); // automatic type conversion for GL
	void print();

	static float determinant3x3( float m00, float m01, float m02,
		float m10, float m11, float m12,
		float m20, float m21, float m22 );

    // a 3x3 matrix of all ones
	static Matrix3f ones();

    // a 3x3 identity matrix
	static Matrix3f identity();

    // Returns the rotation matrix for a rotation of
    // "radians" radians, counterclockwise about the x-axis.
	static Matrix3f rotateX( float radians );
	
    // Returns the rotation matrix for a rotation of
    // "radians" radians, counterclockwise about the y-axis.
	static Matrix3f rotateY( float radians );

	// Returns the rotation matrix for a rotation of
    // "radians" radians, counterclockwise about the z-axis.
    static Matrix3f rotateZ( float radians );

    // Returns a matrix that non-uniformsly scales
    // the three axes by sx, sy, and sz.
	static Matrix3f scaling( float sx, float sy, float sz );

    // Returns a matrix that uniformly scales
    // each axis by "s".
	static Matrix3f uniformScaling( float s );

    // Returns the rotation matrix for a rotation of
    // "radians" radians, counterclockwise
    // about the axis "rDirection"
	static Matrix3f rotation( const Vector3f& rDirection, float radians );

	// Returns the rotation matrix represented by a unit quaternion
	// if q is not normalized, it is normalized first.
	static Matrix3f rotation( const Quat4f& rq );

private:

	float m_elements[ 9 ];

};

// Scalar-Matrix multiplication
Matrix3f operator * ( float f, const Matrix3f& a );
Matrix3f operator * ( const Matrix3f& a, float f );

// Matrix-Matrix sum, difference
Matrix3f operator + ( const Matrix3f& a, const Matrix3f& b );
Matrix3f operator - ( const Matrix3f& a, const Matrix3f& b );

// Matrix-Vector multiplication
// 3x3 * 3x1 ==> 3x1
Vector3f operator * ( const Matrix3f& m, const Vector3f& v );

// Matrix-Matrix multiplication
Matrix3f operator * ( const Matrix3f& x, const Matrix3f& y );

#endif // MATRIX3F_H
