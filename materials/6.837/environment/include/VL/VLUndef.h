/*
	File:			VLUndef.h

	Function:		Undefines the type macros used by VL.h, so that that
					file can be re-included, usually with different types.
					See VLfd.h, for example.

	Author(s):		Andrew Willmott

 */

#undef __VL__
#undef VL_VERSION

#undef  TVReal	
#undef  TMReal	

#undef  TVec2
#undef  TMVec2	
#undef  TMat2

#undef  TVec3	
#undef  TMVec3	
#undef  TMat3	

#undef  TVec4	
#undef  TQuaternion
#undef  TMVec4	
#undef  TMat4	

#undef  TVec	
#undef  TMVec 	
#undef  TMat	
#undef  TSubVec	
#undef  TMSubVec
#undef  TSubMat	

#undef  TSparseVec
#undef  TSparseMat
#undef  TSubSVec
#undef  TMSubSVec
#undef  TSubSMat

#undef  TSparsePair
#undef  TMSparseVec
#undef  TSVIter	

#undef  Vec
#undef  Vec2
#undef  Vec3
#undef  Vec4

#undef  Mat
#undef  Mat2
#undef  Mat3
#undef  Mat4

#undef  SparseMat
#undef  SparseVec
#undef  SubMat
#undef  SubVec

#undef  SubSMat
#undef  SubSVec

#undef  VL_V_REAL
#undef  VL_M_REAL
#undef  VL_V_SUFF
#undef  VL_M_SUFF

#undef __Vec__
#undef __Vec2__
#undef __Vec3__
#undef __Vec4__
#undef __Mat__
#undef __Mat2__
#undef __Mat3__
#undef __Mat4__
#undef __SparseVec__
#undef __SparseMat__
#undef __SubVec__
#undef __SubMat__
#undef __SubSVec__
#undef __SubSMat__
#undef __Solve__
#undef __Factor__
