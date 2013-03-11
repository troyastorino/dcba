/*
	File:			Transform.h

	Function:		Provides transformation constructors.
	
	Author(s):		Andrew Willmott

	Copyright:		(c) 1995-2000, Andrew Willmott
*/


inline TMat2	VL_M_SUFF(Rot2)(Real theta)
				{ TMat2 result; result.MakeRot(theta); return(result); }
inline TMat2	VL_M_SUFF(Scale2)(const TVec2 &s)
				{ TMat2 result; result.MakeScale(s); return(result); }

inline TMat3	VL_M_SUFF(Rot3)(const TVec3 &axis, Real theta)
				{ TMat3 result; result.MakeRot(axis, theta); return(result); }
inline TMat3	VL_M_SUFF(Rot3)(const TVec4 &q)
				{ TMat3 result; result.MakeRot(q); return(result); }
inline TMat3	VL_M_SUFF(Scale3)(const TVec3 &s)
				{ TMat3 result; result.MakeScale(s); return(result); }
inline TMat3	VL_M_SUFF(HRot3)(Real theta)
				{ TMat3 result; result.MakeHRot(theta); return(result); }
inline TMat3	VL_M_SUFF(HScale3)(const TVec2 &s)
				{ TMat3 result; result.MakeHScale(s); return(result); }
inline TMat3	VL_M_SUFF(HTrans3)(const TVec2 &t)
				{ TMat3 result; result.MakeHTrans(t); return(result); }

inline TMat4	VL_M_SUFF(HRot4)(const TVec3 &axis, Real theta)
				{ TMat4 result; result.MakeHRot(axis, theta); return(result); }
inline TMat4	VL_M_SUFF(HRot4)(const TVec4 &q)
				{ TMat4 result; result.MakeHRot(q); return(result); }
inline TMat4	VL_M_SUFF(HScale4)(const TVec3 &s)
				{ TMat4 result; result.MakeHScale(s); return(result); }
inline TMat4	VL_M_SUFF(HTrans4)(const TVec3 &t)
				{ TMat4 result; result.MakeHTrans(t); return(result); }
