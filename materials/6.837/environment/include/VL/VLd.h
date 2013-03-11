/*
	File:			VLd.h

	Function:		Master header for a version of the VL library based
					on doubles. The various classes are named Vecd,
					Mat3d, SparseVecd, etc.
					
	Author(s):		Andrew Willmott

	Copyright:		(c) 1995-2000, Andrew Willmott
 */


#ifndef __VLd__
#define __VLd__

#ifdef __VL__
#include "vl/VLUndef.h"
#endif

#define VL_V_REAL Double
#define VL_V_SUFF(X) X ## d

#include "vl/Vec2.h"
#include "vl/Vec3.h"
#include "vl/Vec4.h"
#include "vl/Vec.h"
#include "vl/SparseVec.h"

#include "vl/Mat2.h"
#include "vl/Mat3.h"
#include "vl/Mat4.h"
#include "vl/Mat.h"
#include "vl/SparseMat.h"
#include "vl/Solve.h"
#include "vl/Factor.h"
#include "vl/Transform.h"

#endif
