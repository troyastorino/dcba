/*
	File:			VL.h

	Function:		Header file for the VL library. Provides macro definitions
					so that the library can be templated by type.
					
	Author(s):		Andrew Willmott

	Copyright:		(c) 1995-2000, Andrew Willmott
 */

#ifndef __VL__
#define __VL__

#define VL_VERSION "1.3.2"
#define VL_VER_NUM 10302

/*
	This header file is included before any other when compiling VL. Its
	purpose is to translate the types that VL is templated on (TReal, TVec2,
	TVec3, etc.), replastd::cing them with real types. In this way the same
	code can be used to compile float-based, double-based, and other
	type-based versions of the vector and matrix routines.

	VL.h expects VL_V_REAL and VL_M_REAL to be set to the element types for
	vectors and matrices respectively, and the VL_V_SUFF and VL_M_SUFF
	macros to append a suffix indicating the vector or matrix type to their
	arguments. If these macros are not defined, vectors and matrices will
	be based on the type 'Real', and there will be no suffixes.

	See VLf.h, VLfd.h, and SVL.h for examples of how this facility is used.

	The VL library used to use C++ templates rather than the current cpp
	macros. The state of both the C++ languange and its compilers made this
	unworkable, however, especially when targetting more than one compiler.
*/

#include "cl/Basics.h"
#include "vl/VLConfig.h"
#include "vl/VLConstants.h"
#include "vl/VLMath.h"

#ifndef VL_V_REAL
#define VL_V_REAL Real
#define VL_M_REAL Real
#endif

#ifndef VL_V_SUFF
#define VL_V_SUFF(X) X
#define VL_M_SUFF(X) X
#endif

#ifndef VL_M_REAL
#define VL_M_REAL VL_V_REAL
#define VL_M_SUFF(X) VL_V_SUFF(X)
#endif

#define TVReal		VL_V_REAL
#define TMReal		VL_M_REAL

#define TVec2 		VL_V_SUFF(Vec2)
#define TMVec2		VL_M_SUFF(Vec2)
#define TMat2 		VL_M_SUFF(Mat2)

#define TVec3		VL_V_SUFF(Vec3)
#define TMVec3		VL_M_SUFF(Vec3)
#define TMat3		VL_M_SUFF(Mat3)

#define TVec4		VL_V_SUFF(Vec4)
#define TQuaternion	VL_M_SUFF(Vec4)
#define TMVec4		VL_M_SUFF(Vec4)
#define TMat4		VL_M_SUFF(Mat4)

#define TVec		VL_V_SUFF(Vec)
#define TMVec 		VL_M_SUFF(Vec)
#define TMat		VL_M_SUFF(Mat)
#define TSubVec		VL_V_SUFF(SubVec)
#define TMSubVec	VL_M_SUFF(SubVec)
#define TSubMat		VL_M_SUFF(SubMat)

#define TSparseVec	VL_V_SUFF(SparseVec)
#define TSparseMat	VL_M_SUFF(SparseMat)
#define TSubSVec	VL_V_SUFF(SubSVec)
#define TMSubSVec	VL_M_SUFF(SubSVec)
#define TSubSMat	VL_M_SUFF(SubSMat)

#define TSparsePair	VL_V_SUFF(SparsePair)
#define TMSparseVec	VL_M_SUFF(SparseVec)
#define TSVIter		VL_V_SUFF(SVIter)
#define TMSVIter	VL_M_SUFF(SVIter)

#endif	
