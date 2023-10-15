/*
* Copyright 2023 Daniel S. Buckstein
*
*   Licensed under the Apache License, Version 2.0 (the "License");
*   you may not use this file except in compliance with the License.
*   You may obtain a copy of the License at
*
*       http://www.apache.org/licenses/LICENSE-2.0
*
*   Unless required by applicable law or agreed to in writing, software
*   distributed under the License is distributed on an "AS IS" BASIS,
*   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
*   See the License for the specific language governing permissions and
*   limitations under the License.
*/

/*
* cdrawVec3d.inl
* Definitions of double-precision 3D vector functions.
*/

#if (!(defined _CDRAW_VEC3D_INL_) && (defined _CDRAW_VECTOR_INL_))
#define _CDRAW_VEC3D_INL_


CDRAW_INL doubleN_t vecZero3d(double3_t v_out)
{
	failassert(v_out, NULL);
	vx(v_out) = 0.0;
	vy(v_out) = 0.0;
	vz(v_out) = 0.0;
	return v_out;
}

CDRAW_INL doubleN_t vecInit3d(double3_t v_out, vecd_t const x, vecd_t const y, vecd_t const z)
{
	failassert(v_out, NULL);
	vx(v_out) = x;
	vy(v_out) = y;
	vz(v_out) = z;
	return v_out;
}

CDRAW_INL doubleN_t vecInitS3d(double3_t v_out, vecd_t const x)
{
	failassert(v_out, NULL);
	vx(v_out) = x;
	vy(v_out) = x;
	vz(v_out) = x;
	return v_out;
}

CDRAW_INL doubleN_t vecCopy2z3d(double3_t v_out, double2_t const xy, vecd_t const z)
{
	failassert(v_out && xy, NULL);
	vx(v_out) = vx(xy);
	vy(v_out) = vy(xy);
	vz(v_out) = z;
	return v_out;
}

CDRAW_INL doubleN_t vecCopy3d(double3_t v_out, double3_t const v)
{
	failassert(v_out && v, NULL);
	vx(v_out) = vx(v);
	vy(v_out) = vy(v);
	vz(v_out) = vz(v);
	return v_out;
}

CDRAW_INL doubleN_t vecCopy3b3d(double3_t v_out, bool3_t const v)
{
	failassert(v_out && v, NULL);
	vx(v_out) = (vx(v) ? 1.0 : 0.0);
	vy(v_out) = (vy(v) ? 1.0 : 0.0);
	vz(v_out) = (vz(v) ? 1.0 : 0.0);
	return v_out;
}

CDRAW_INL doubleN_t vecCopy3i3d(double3_t v_out, int3_t const v)
{
	failassert(v_out && v, NULL);
	vx(v_out) = (vecd_t)(vx(v));
	vy(v_out) = (vecd_t)(vy(v));
	vz(v_out) = (vecd_t)(vz(v));
	return v_out;
}

CDRAW_INL doubleN_t vecCopy3u3d(double3_t v_out, uint3_t const v)
{
	failassert(v_out && v, NULL);
	vx(v_out) = (vecd_t)(vx(v));
	vy(v_out) = (vecd_t)(vy(v));
	vz(v_out) = (vecd_t)(vz(v));
	return v_out;
}

CDRAW_INL doubleN_t vecCopy3f3d(double3_t v_out, float3_t const v)
{
	failassert(v_out && v, NULL);
	vx(v_out) = (vecd_t)(vx(v));
	vy(v_out) = (vecd_t)(vy(v));
	vz(v_out) = (vecd_t)(vz(v));
	return v_out;
}

CDRAW_INL boolN_t vecEq3d(bool3_t v_out, double3_t const v_lh, double3_t const v_rh)
{
	failassert(v_out && v_lh && v_rh, NULL);
	vx(v_out) = (vx(v_lh) == vx(v_rh));
	vy(v_out) = (vy(v_lh) == vy(v_rh));
	vz(v_out) = (vz(v_lh) == vz(v_rh));
	return v_out;
}

CDRAW_INL boolN_t vecNeq3d(bool3_t v_out, double3_t const v_lh, double3_t const v_rh)
{
	failassert(v_out && v_lh && v_rh, NULL);
	vx(v_out) = (vx(v_lh) != vx(v_rh));
	vy(v_out) = (vy(v_lh) != vy(v_rh));
	vz(v_out) = (vz(v_lh) != vz(v_rh));
	return v_out;
}

CDRAW_INL boolN_t vecGreater3d(bool3_t v_out, double3_t const v_lh, double3_t const v_rh)
{
	failassert(v_out && v_lh && v_rh, NULL);
	vx(v_out) = (vx(v_lh) > vx(v_rh));
	vy(v_out) = (vy(v_lh) > vy(v_rh));
	vz(v_out) = (vz(v_lh) > vz(v_rh));
	return v_out;
}

CDRAW_INL boolN_t vecGreaterEq3d(bool3_t v_out, double3_t const v_lh, double3_t const v_rh)
{
	failassert(v_out && v_lh && v_rh, NULL);
	vx(v_out) = (vx(v_lh) >= vx(v_rh));
	vy(v_out) = (vy(v_lh) >= vy(v_rh));
	vz(v_out) = (vz(v_lh) >= vz(v_rh));
	return v_out;
}

CDRAW_INL boolN_t vecLess3d(bool3_t v_out, double3_t const v_lh, double3_t const v_rh)
{
	failassert(v_out && v_lh && v_rh, NULL);
	vx(v_out) = (vx(v_lh) < vx(v_rh));
	vy(v_out) = (vy(v_lh) < vy(v_rh));
	vz(v_out) = (vz(v_lh) < vz(v_rh));
	return v_out;
}

CDRAW_INL boolN_t vecLessEq3d(bool3_t v_out, double3_t const v_lh, double3_t const v_rh)
{
	failassert(v_out && v_lh && v_rh, NULL);
	vx(v_out) = (vx(v_lh) <= vx(v_rh));
	vy(v_out) = (vy(v_lh) <= vy(v_rh));
	vz(v_out) = (vz(v_lh) <= vz(v_rh));
	return v_out;
}

CDRAW_INL doubleN_t vecNegate3d(double3_t v_out, double3_t const v)
{
	failassert(v_out && v, NULL);
	vx(v_out) = -vx(v);
	vy(v_out) = -vy(v);
	vz(v_out) = -vz(v);
	return v_out;
}

CDRAW_INL doubleN_t vecAdd3d(double3_t v_out, double3_t const v_lh, double3_t const v_rh)
{
	failassert(v_out && v_lh && v_rh, NULL);
	vx(v_out) = vx(v_lh) + vx(v_rh);
	vy(v_out) = vy(v_lh) + vy(v_rh);
	vz(v_out) = vz(v_lh) + vz(v_rh);
	return v_out;
}

CDRAW_INL doubleN_t vecSub3d(double3_t v_out, double3_t const v_lh, double3_t const v_rh)
{
	failassert(v_out && v_lh && v_rh, NULL);
	vx(v_out) = vx(v_lh) - vx(v_rh);
	vy(v_out) = vy(v_lh) - vy(v_rh);
	vz(v_out) = vz(v_lh) - vz(v_rh);
	return v_out;
}

CDRAW_INL doubleN_t vecMul3d(double3_t v_out, double3_t const v_lh, double3_t const v_rh)
{
	failassert(v_out && v_lh && v_rh, NULL);
	vx(v_out) = vx(v_lh) * vx(v_rh);
	vy(v_out) = vy(v_lh) * vy(v_rh);
	vz(v_out) = vz(v_lh) * vz(v_rh);
	return v_out;
}

CDRAW_INL doubleN_t vecDiv3d(double3_t v_out, double3_t const v_lh, double3_t const v_rh)
{
	failassert(v_out && v_lh && v_rh, NULL);
	failassert(scIsNonZeroApproxD(vx(v_rh)) && scIsNonZeroApproxD(vy(v_rh)) && scIsNonZeroApproxD(vz(v_rh)), vecInit3d(v_out,
		gDivSafeD(vx(v_lh), vx(v_rh)),
		gDivSafeD(vy(v_lh), vy(v_rh)),
		gDivSafeD(vz(v_lh), vz(v_rh))));
	vx(v_out) = gDiv(vx(v_lh), vx(v_rh));
	vy(v_out) = gDiv(vy(v_lh), vy(v_rh));
	vz(v_out) = gDiv(vz(v_lh), vz(v_rh));
	return v_out;
}

CDRAW_INL doubleN_t vecMod3d(double3_t v_out, double3_t const v_lh, double3_t const v_rh)
{
	failassert(v_out && v_lh && v_rh, NULL);
	failassert(scIsNonZeroApproxD(vx(v_rh)) && scIsNonZeroApproxD(vy(v_rh)) && scIsNonZeroApproxD(vz(v_rh)), vecInit3d(v_out,
		gModSafeD(vx(v_lh), vx(v_rh)),
		gModSafeD(vy(v_lh), vy(v_rh)),
		gModSafeD(vz(v_lh), vz(v_rh))));
	vx(v_out) = gModD(vx(v_lh), vx(v_rh));
	vy(v_out) = gModD(vy(v_lh), vy(v_rh));
	vz(v_out) = gModD(vz(v_lh), vz(v_rh));
	return v_out;
}

CDRAW_INL doubleN_t vecDivMod3d(double3_t v_out, double3_t v_mod_out, double3_t const v_lh, double3_t const v_rh)
{
	failassert(v_out && v_mod_out && v_lh && v_rh, NULL);
	failassert((v_mod_out != v_out) && (v_mod_out != v_lh) && (v_mod_out != v_rh), NULL);
	failassert(scIsNonZeroApproxD(vx(v_rh)) && scIsNonZeroApproxD(vy(v_rh)) && scIsNonZeroApproxD(vz(v_rh)), vecInit3d(v_out,
		gDivModSafeD(vx(v_mod_out), vx(v_lh), vx(v_rh)),
		gDivModSafeD(vy(v_mod_out), vy(v_lh), vy(v_rh)),
		gDivModSafeD(vz(v_mod_out), vz(v_lh), vz(v_rh))));
	vx(v_out) = gDiv(vx(v_lh), vx(v_rh));
	vy(v_out) = gDiv(vy(v_lh), vy(v_rh));
	vz(v_out) = gDiv(vz(v_lh), vz(v_rh));
	vx(v_mod_out) = gModQD(vx(v_lh), vx(v_rh), vx(v_out));
	vy(v_mod_out) = gModQD(vy(v_lh), vy(v_rh), vy(v_out));
	vz(v_mod_out) = gModQD(vz(v_lh), vz(v_rh), vz(v_out));
	return v_out;
}

CDRAW_INL doubleN_t vecAddS3d(double3_t v_out, double3_t const v_lh, vecd_t const s_rh)
{
	failassert(v_out && v_lh, NULL);
	vx(v_out) = vx(v_lh) + s_rh;
	vy(v_out) = vy(v_lh) + s_rh;
	vz(v_out) = vz(v_lh) + s_rh;
	return v_out;
}

CDRAW_INL doubleN_t vecSubS3d(double3_t v_out, double3_t const v_lh, vecd_t const s_rh)
{
	failassert(v_out && v_lh, NULL);
	vx(v_out) = vx(v_lh) - s_rh;
	vy(v_out) = vy(v_lh) - s_rh;
	vz(v_out) = vz(v_lh) - s_rh;
	return v_out;
}

CDRAW_INL doubleN_t vecMulS3d(double3_t v_out, double3_t const v_lh, vecd_t const s_rh)
{
	failassert(v_out && v_lh, NULL);
	vx(v_out) = vx(v_lh) * s_rh;
	vy(v_out) = vy(v_lh) * s_rh;
	vz(v_out) = vz(v_lh) * s_rh;
	return v_out;
}

CDRAW_INL doubleN_t vecDivS3d(double3_t v_out, double3_t const v_lh, vecd_t const s_rh)
{
	failassert(v_out && v_lh, NULL);
	failassert(scIsNonZeroApproxD(s_rh), vecZero3d(v_out));
	vecd_t const recip = sc1D / s_rh;
	vx(v_out) = vx(v_lh) * recip;
	vy(v_out) = vy(v_lh) * recip;
	vz(v_out) = vz(v_lh) * recip;
	return v_out;
}

CDRAW_INL doubleN_t vecModS3d(double3_t v_out, double3_t const v_lh, vecd_t const s_rh)
{
	failassert(v_out && v_lh, NULL);
	failassert(scIsNonZeroApproxD(s_rh), vecCopy3d(v_out, v_lh));
	vecd_t const recip = sc1D / s_rh;
	vx(v_out) = gModQD(vx(v_lh), s_rh, vx(v_lh) * recip);
	vy(v_out) = gModQD(vy(v_lh), s_rh, vy(v_lh) * recip);
	vz(v_out) = gModQD(vz(v_lh), s_rh, vz(v_lh) * recip);
	return v_out;
}

CDRAW_INL doubleN_t vecDivModS3d(double3_t v_out, double3_t v_mod_out, double3_t const v_lh, vecd_t const s_rh)
{
	failassert(v_out && v_mod_out && v_lh, NULL);
	failassert((v_mod_out != v_out) && (v_mod_out != v_lh), NULL);
	failassert(scIsNonZeroApproxD(s_rh), vecCopy3d(v_mod_out, v_lh), vecZero3d(v_out));
	vecd_t const recip = sc1D / s_rh;
	vx(v_out) = vx(v_lh) * recip;
	vy(v_out) = vy(v_lh) * recip;
	vz(v_out) = vz(v_lh) * recip;
	vx(v_mod_out) = gModQD(vx(v_lh), s_rh, vx(v_out));
	vy(v_mod_out) = gModQD(vy(v_lh), s_rh, vy(v_out));
	vz(v_mod_out) = gModQD(vz(v_lh), s_rh, vz(v_out));
	return v_out;
}

CDRAW_INL vecd_t vecDot3d(double3_t const v_lh, double3_t const v_rh)
{
	failassert(v_lh && v_rh, 0);
	return (vx(v_lh) * vx(v_rh) + vy(v_lh) * vy(v_rh) + vz(v_lh) * vz(v_rh));
}

CDRAW_INL doubleN_t vecCross3d(double3_t v_out, double3_t const v_lh, double3_t const v_rh)
{
	failassert(v_out && v_lh && v_rh, NULL);
	vx(v_out) = (vy(v_lh) * vz(v_rh) - vz(v_lh) * vy(v_rh));
	vy(v_out) = (vz(v_lh) * vx(v_rh) - vx(v_lh) * vz(v_rh));
	vz(v_out) = (vx(v_lh) * vy(v_rh) - vy(v_lh) * vx(v_rh));
	return v_out;
}

CDRAW_INL vecd_t vecLenSq3d(double3_t const v)
{
	failassert(v, 0);
	return (gSq(vx(v)) + gSq(vy(v)) + gSq(vz(v)));
}

CDRAW_INL vecd_t vecDistSq3d(double3_t const v_lh, double3_t const v_rh)
{
	failassert(v_lh && v_rh, 0);
	double3_t const disp = {
		(vx(v_lh) - vx(v_rh)),
		(vy(v_lh) - vy(v_rh)),
		(vz(v_lh) - vz(v_rh)),
	};
	return vecLenSq3d(disp);
}

CDRAW_INL vecd_t vecDispDistSq3d(double3_t v_disp_out, double3_t const v_lh, double3_t const v_rh)
{
	failassert(v_disp_out && v_lh && v_rh, 0);
	vx(v_disp_out) = (vx(v_lh) - vx(v_rh));
	vy(v_disp_out) = (vy(v_lh) - vy(v_rh));
	vz(v_disp_out) = (vz(v_lh) - vz(v_rh));
	return vecLenSq3d(v_disp_out);
}

CDRAW_INL vecb_t vecIsZero3d(double3_t const v)
{
	failassert(v, true);
	return scIsNonPositiveApproxD(vecLenSq3d(v));
}

CDRAW_INL vecb_t vecIsNonZero3d(double3_t const v)
{
	failassert(v, false);
	return scIsPositiveApproxD(vecLenSq3d(v));
}

CDRAW_INL doubleN_t vecMad3d(double3_t v_out, vecd_t const u, double3_t const v_origin, double3_t const v_delta)
{
	failassert(v_out && v_origin && v_delta, NULL);
	vx(v_out) = gMad(vx(v_origin), vx(v_delta), u);
	vy(v_out) = gMad(vy(v_origin), vy(v_delta), u);
	vz(v_out) = gMad(vz(v_origin), vz(v_delta), u);
	return v_out;
}

CDRAW_INL doubleN_t vecLerp3d(double3_t v_out, vecd_t const u, double3_t const v_min, double3_t const v_max)
{
	failassert(v_out && v_min && v_max, NULL);
	vx(v_out) = gLerp(vx(v_min), vx(v_max), u);
	vy(v_out) = gLerp(vy(v_min), vy(v_max), u);
	vz(v_out) = gLerp(vz(v_min), vz(v_max), u);
	return v_out;
}

CDRAW_INL vecd_t vecLen3d(double3_t const v)
{
	failassert(v, sc0D);
	vecd_t const lenSq = vecLenSq3d(v);
	return scSqrtD(lenSq);
}

CDRAW_INL vecd_t vecLenInv3d(double3_t const v)
{
	failassert(v, sc0D);
	vecd_t const lenSq = vecLenSq3d(v);
	failassert(scIsPositiveApproxD(lenSq), sc0D);
	return gSafeSqrtInvD(lenSq);
}

CDRAW_INL vecd_t vecDist3d(double3_t const v_lh, double3_t const v_rh)
{
	failassert(v_lh && v_rh, sc0D);
	return scSqrtD(vecDistSq3d(v_lh, v_rh));
}

CDRAW_INL vecd_t vecDispDist3d(double3_t v_disp_out, double3_t const v_lh, double3_t const v_rh)
{
	failassert(v_disp_out && v_lh && v_rh, sc0D);
	return scSqrtD(vecDispDistSq3d(v_disp_out, v_lh, v_rh));
}

CDRAW_INL vecd_t vecNormalize3d(double3_t v_out, double3_t const v)
{
	failassert(v_out && v, sc0D);
	vecd_t len = vecLenSq3d(v), ratio;
	if (scIsNonPositiveApproxD(len))
		return (vecZero3d(v_out), sc0D);
	len = gSafeSqrtD(len);
	ratio = sc1D / len;
	vx(v_out) = vx(v) * ratio;
	vy(v_out) = vy(v) * ratio;
	vz(v_out) = vz(v) * ratio;
	return len;
}

CDRAW_INL vecd_t vecResize3d(double3_t v_out, double3_t const v, vecd_t const newLen)
{
	failassert(v_out && v, sc0D);
	vecd_t len = vecLenSq3d(v), ratio;
	if (scIsNonPositiveApproxD(len))
		return (vecZero3d(v_out), sc0D);
	len = gSafeSqrtD(len);
	ratio = newLen / len;
	vx(v_out) = vx(v) * ratio;
	vy(v_out) = vy(v) * ratio;
	vz(v_out) = vz(v) * ratio;
	return len;
}

CDRAW_INL vecb_t vecIsUnit3d(double3_t const v)
{
	failassert(v, false);
	vecd_t const lenSq = vecLenSq3d(v);
	return scIsUnityApproxD(lenSq);
}

CDRAW_INL vecb_t vecIsNonUnit3d(double3_t const v)
{
	failassert(v, true);
	vecd_t const lenSq = vecLenSq3d(v);
	return scIsNonUnityApproxD(lenSq);
}

CDRAW_INL vecd_t vecProjS3d(double3_t const v, double3_t const v_base)
{
	failassert(v && v_base, sc0D);
	vecd_t const lenSq = vecLenSq3d(v_base);
	if (scIsNonPositiveApproxD(lenSq))
		return sc0D;
	return (vecDot3d(v, v_base) / lenSq);
}

CDRAW_INL vecd_t vecProj3d(double3_t v_out, double3_t const v, double3_t const v_base)
{
	failassert(v_out && v && v_base, sc0D);
	vecd_t ratio = vecLenSq3d(v_base);
	if (scIsNonPositiveApproxD(ratio))
		return sc0D;
	ratio = vecDot3d(v, v_base) / ratio;
	vx(v_out) = vx(v_base) * ratio;
	vy(v_out) = vy(v_base) * ratio;
	vz(v_out) = vz(v_base) * ratio;
	return ratio;
}

CDRAW_INL vecd_t vecLerpInv3d(double3_t const v, double3_t const v_min, double3_t const v_max)
{
	failassert(v && v_min && v_max, sc0D);
	double3_t v_delta;
	vecd_t const distSq = vecDispDistSq3d(v_delta, v_max, v_min);
	if (scIsNonPositiveApproxD(distSq))
		return sc0D;
	return ((vx(v) - vx(v_min)) * vx(v_delta) + (vy(v) - vy(v_min)) * vy(v_delta) + (vz(v) - vz(v_min)) * vz(v_delta)) / distSq;
}

CDRAW_INL vecd_t vecOrtho3d(double3_t v_out, double3_t const v, double3_t const v_base)
{
	failassert(v_out && v && v_base, sc0D);
	vecd_t ratio = vecLenSq3d(v_base);
	if (scIsNonPositiveApproxD(ratio))
		return (vecZero3d(v_out), ratio);
	ratio = vecDot3d(v, v_base) / ratio;
	vx(v_out) = (vx(v) - vx(v_base) * ratio);
	vy(v_out) = (vy(v) - vy(v_base) * ratio);
	vz(v_out) = (vz(v) - vz(v_base) * ratio);
	return ratio;
}


CDRAW_INL vecd_t vecOrthoBasis3d(double3_t v2_out, double3_t v1_out, vecd_t* v2_basefactor_out_opt, vecd_t* v1_basefactor_out_opt, double3_t const v2, double3_t const v1, double3_t const v_base)
{
	failassert(v2_out && v1_out && v2 && v1 && v_base, sc0D);
	failassert((v1_out != v2_out) && (v1_out != v_base) && (v1_out != v2), sc0D);
	failassert((!v2_basefactor_out_opt) || (v2_basefactor_out_opt != v2_out) && (v2_basefactor_out_opt != v1_out) && (v2_basefactor_out_opt != v2) && (v2_basefactor_out_opt != v1) && (v2_basefactor_out_opt != v_base) && (v2_basefactor_out_opt != v1_basefactor_out_opt), sc0D);
	failassert((!v1_basefactor_out_opt) || (v1_basefactor_out_opt != v2_out) && (v1_basefactor_out_opt != v1_out) && (v1_basefactor_out_opt != v2) && (v1_basefactor_out_opt != v1) && (v1_basefactor_out_opt != v_base), sc0D);
	vecd_t ratio = vecLenSq3d(v_base), ratio1, ratio2;
	if (scIsNonPositiveApproxD(ratio))
		return (vecZero3d(v2_out), vecZero3d(v1_out), sc0D);
	ratio = sc1D / ratio;
	ratio1 = vecDot3d(v1, v_base) * ratio;
	ratio2 = vecDot3d(v2, v_base) * ratio;
	vx(v1_out) = (vx(v1) - vx(v_base) * ratio1);
	vy(v1_out) = (vy(v1) - vy(v_base) * ratio1);
	vz(v1_out) = (vz(v1) - vz(v_base) * ratio1);
	ratio = vecDot3d(v2, v1_out) / vecLenSq3d(v1_out);
	vx(v2_out) = (vx(v2) - vx(v_base) * ratio2 - vx(v1_out) * ratio);
	vy(v2_out) = (vy(v2) - vy(v_base) * ratio2 - vy(v1_out) * ratio);
	vz(v2_out) = (vz(v2) - vz(v_base) * ratio2 - vz(v1_out) * ratio);
	if (v1_basefactor_out_opt) *v1_basefactor_out_opt = ratio1;
	if (v2_basefactor_out_opt) *v2_basefactor_out_opt = ratio2;
	return ratio;
}


#endif // #if (!(defined _CDRAW_VEC3D_INL_) && (defined _CDRAW_VECTOR_INL_))