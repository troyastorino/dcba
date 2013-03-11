/*
	File:			VLfd.h

	Function:		Master header for a version of the VL library based
					on floats and doubles: vectors are made up of
					floats, and matrices of doubles. The various classes
					are named Vecf, Mat3d, SparseVecf, etc. To use this
					header you should link with -lvl.
					
	Author(s):		Andrew Willmott

	Copyright:		(c) 1995-2000, Andrew Willmott
 */

#ifndef __VLfd__
#define __VLfd__

#ifdef __VL__
#include "vl/VLUndef.h"
#endif

#ifndef __VLd__
#define VL_V_REAL Double
#define VL_V_SUFF(X) X ## d

#include "vl/Mat2.h"
#include "vl/Mat3.h"
#include "vl/Mat4.h"
#include "vl/Mat.h"
#include "vl/SparseMat.h"
#include "vl/Solve.h"
#include "vl/Factor.h"
#include "vl/Transform.h"

#include "vl/VLUndef.h"
#endif

#define VL_V_REAL Float
#define VL_V_SUFF(X) X ## f
#define VL_M_REAL Double
#define VL_M_SUFF(X) X ## d

#ifndef __VLf__
#include "vl/Vec2.h"
#include "vl/Vec3.h"
#include "vl/Vec4.h"
#include "vl/Vec.h"
#include "vl/SparseVec.h"
#endif
#include "vl/Mixed.h"
#include "vl/Solve.h"
#include "vl/Factor.h"

#endif
