/*
    File:       Factor.h

    Function:   Provides routines for factoring a matrix:

                + SVD decomposes A into three matrices, A = U D Vt, where:

                    U is m x n, and orthogonal

                    D is n x n, and is diagonal; its elements are the
                    eigenvalues of AtA.
    
                    V is n x n, and orthogonal.

                + QR factors A into A = Q R, where R is upper-triangular,
                and Q is orthogonal.
                    
    Author:     Andrew Willmott

    Copyright:  (c) 1999-2000, Andrew Willmott
*/

#ifndef __Factor__
#define __Factor__


// --- Factorization routines--------------------------------------------------

Void SVDFactorization(TMat &A, TMat &U, TMat &V, TVec &diagonal);
// Factor A into U D V
// Destroys A.

TMReal QRFactorization(TMat &A, TMat &Q, TMat &R);
// Factor A into an orthogonal matrix Q and an upper-triangular matrix R.
// Destroys A.

// --- Utility routines--------------------------------------------------------

TMReal  LeftHouseholder (TMat &A, TMat &U, const Int i);
TMReal  RightHouseholder(TMat &A, TMat &V, const Int i);
void    Bidiagonalize(TMat &A, TMat &U, TMat &V, TVec &diagonal, TVec &superDiag);
void    Diagonalize(TVec &diagonal, TVec &superDiag, TMat &U, TMat &V);

#endif
