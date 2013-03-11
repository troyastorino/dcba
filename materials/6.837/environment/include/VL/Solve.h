/*
	File:			Solve.h

	Function:		Contains routines for solving a system of linear equations.
					Includes the overrelaxation (a more general version of 
					Gauss Seidel) and conjugate gradient methods, for both
					normal and sparse matrices.

	Author(s):		Andrew Willmott

	Copyright:		(c) 1995-2000, Andrew Willmott
 */

#ifndef __Solve__
#define __Solve__

TMReal SolveOverRelax(const TMat &A, TVec &x, const TVec &b,
			TMReal epsilon, TMReal omega = 1.0, Int *steps = 0);
TMReal SolveOverRelax(const TSparseMat &A, TVec &x, const TVec &b, 
			TMReal epsilon, TMReal omega = 1.0, Int *steps = 0);

TMReal SolveConjGrad(const TMat &A, TVec &x, const TVec &b,
			TMReal epsilon, Int *steps = 0); 
TMReal SolveConjGrad(const TSparseMat &A, TVec &x, const TVec &b,
			TMReal epsilon, Int *steps = 0);

#endif
