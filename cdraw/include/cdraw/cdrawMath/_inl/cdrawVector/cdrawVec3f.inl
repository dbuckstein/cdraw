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
* cdrawVec3f.inl
* Definitions of single-precision 3D vector functions.
*/

#if (!(defined _CDRAW_VEC3F_INL_) && (defined _CDRAW_VECTOR_INL_))
#define _CDRAW_VEC3F_INL_


CDRAW_INL floatN_t vecZero3f(float3_t v_out)
{
	failassert(v_out, NULL);
	vx(v_out) = 0.0f;
	vy(v_out) = 0.0f;
	vz(v_out) = 0.0f;
	return v_out;
}

CDRAW_INL floatN_t vecInit3f(float3_t v_out, vecf_t const x, vecf_t const y, vecf_t const z)
{
	failassert(v_out, NULL);
	vx(v_out) = x;
	vy(v_out) = y;
	vz(v_out) = z;
	return v_out;
}

CDRAW_INL floatN_t vecInitS3f(float3_t v_out, vecf_t const x)
{
	failassert(v_out, NULL);
	vx(v_out) = x;
	vy(v_out) = x;
	vz(v_out) = x;
	return v_out;
}

CDRAW_INL floatN_t vecCopy2z3f(float3_t v_out, float2_t const xy, vecf_t const z)
{
	failassert(v_out && xy, NULL);
	vx(v_out) = vx(xy);
	vy(v_out) = vy(xy);
	vz(v_out) = z;
	return v_out;
}

CDRAW_INL floatN_t vecCopy3f(float3_t v_out, float3_t const v)
{
	failassert(v_out && v, NULL);
	vx(v_out) = vx(v);
	vy(v_out) = vy(v);
	vz(v_out) = vz(v);
	return v_out;
}

CDRAW_INL floatN_t vecCopy3b3f(float3_t v_out, bool3_t const v)
{
	failassert(v_out && v, NULL);
	vx(v_out) = (vx(v) ? 1.0f : 0.0f);
	vy(v_out) = (vy(v) ? 1.0f : 0.0f);
	vz(v_out) = (vz(v) ? 1.0f : 0.0f);
	return v_out;
}

CDRAW_INL floatN_t vecCopy3i3f(float3_t v_out, int3_t const v)
{
	failassert(v_out && v, NULL);
	vx(v_out) = (vecf_t)(vx(v));
	vy(v_out) = (vecf_t)(vy(v));
	vz(v_out) = (vecf_t)(vz(v));
	return v_out;
}

CDRAW_INL floatN_t vecCopy3u3f(float3_t v_out, uint3_t const v)
{
	failassert(v_out && v, NULL);
	vx(v_out) = (vecf_t)(vx(v));
	vy(v_out) = (vecf_t)(vy(v));
	vz(v_out) = (vecf_t)(vz(v));
	return v_out;
}

CDRAW_INL floatN_t vecCopy3d3f(float3_t v_out, double3_t const v)
{
	failassert(v_out && v, NULL);
	vx(v_out) = (vecf_t)(vx(v));
	vy(v_out) = (vecf_t)(vy(v));
	vz(v_out) = (vecf_t)(vz(v));
	return v_out;
}

CDRAW_INL boolN_t vecEq3f(bool3_t v_out, float3_t const v_lh, float3_t const v_rh)
{
	failassert(v_out && v_lh && v_rh, NULL);
	vx(v_out) = (vx(v_lh) == vx(v_rh));
	vy(v_out) = (vy(v_lh) == vy(v_rh));
	vz(v_out) = (vz(v_lh) == vz(v_rh));
	return v_out;
}

CDRAW_INL boolN_t vecNeq3f(bool3_t v_out, float3_t const v_lh, float3_t const v_rh)
{
	failassert(v_out && v_lh && v_rh, NULL);
	vx(v_out) = (vx(v_lh) != vx(v_rh));
	vy(v_out) = (vy(v_lh) != vy(v_rh));
	vz(v_out) = (vz(v_lh) != vz(v_rh));
	return v_out;
}

CDRAW_INL boolN_t vecGreater3f(bool3_t v_out, float3_t const v_lh, float3_t const v_rh)
{
	failassert(v_out && v_lh && v_rh, NULL);
	vx(v_out) = (vx(v_lh) > vx(v_rh));
	vy(v_out) = (vy(v_lh) > vy(v_rh));
	vz(v_out) = (vz(v_lh) > vz(v_rh));
	return v_out;
}

CDRAW_INL boolN_t vecGreaterEq3f(bool3_t v_out, float3_t const v_lh, float3_t const v_rh)
{
	failassert(v_out && v_lh && v_rh, NULL);
	vx(v_out) = (vx(v_lh) >= vx(v_rh));
	vy(v_out) = (vy(v_lh) >= vy(v_rh));
	vz(v_out) = (vz(v_lh) >= vz(v_rh));
	return v_out;
}

CDRAW_INL boolN_t vecLess3f(bool3_t v_out, float3_t const v_lh, float3_t const v_rh)
{
	failassert(v_out && v_lh && v_rh, NULL);
	vx(v_out) = (vx(v_lh) < vx(v_rh));
	vy(v_out) = (vy(v_lh) < vy(v_rh));
	vz(v_out) = (vz(v_lh) < vz(v_rh));
	return v_out;
}

CDRAW_INL boolN_t vecLessEq3f(bool3_t v_out, float3_t const v_lh, float3_t const v_rh)
{
	failassert(v_out && v_lh && v_rh, NULL);
	vx(v_out) = (vx(v_lh) <= vx(v_rh));
	vy(v_out) = (vy(v_lh) <= vy(v_rh));
	vz(v_out) = (vz(v_lh) <= vz(v_rh));
	return v_out;
}

CDRAW_INL floatN_t vecNegate3f(float3_t v_out, float3_t const v)
{
	failassert(v_out && v, NULL);
	vx(v_out) = -vx(v);
	vy(v_out) = -vy(v);
	vz(v_out) = -vz(v);
	return v_out;
}

CDRAW_INL floatN_t vecAdd3f(float3_t v_out, float3_t const v_lh, float3_t const v_rh)
{
	failassert(v_out && v_lh && v_rh, NULL);
	vx(v_out) = vx(v_lh) + vx(v_rh);
	vy(v_out) = vy(v_lh) + vy(v_rh);
	vz(v_out) = vz(v_lh) + vz(v_rh);
	return v_out;
}

CDRAW_INL floatN_t vecSub3f(float3_t v_out, float3_t const v_lh, float3_t const v_rh)
{
	failassert(v_out && v_lh && v_rh, NULL);
	vx(v_out) = vx(v_lh) - vx(v_rh);
	vy(v_out) = vy(v_lh) - vy(v_rh);
	vz(v_out) = vz(v_lh) - vz(v_rh);
	return v_out;
}

CDRAW_INL floatN_t vecMul3f(float3_t v_out, float3_t const v_lh, float3_t const v_rh)
{
	failassert(v_out && v_lh && v_rh, NULL);
	vx(v_out) = vx(v_lh) * vx(v_rh);
	vy(v_out) = vy(v_lh) * vy(v_rh);
	vz(v_out) = vz(v_lh) * vz(v_rh);
	return v_out;
}

CDRAW_INL floatN_t vecDiv3f(float3_t v_out, float3_t const v_lh, float3_t const v_rh)
{
	failassert(v_out && v_lh && v_rh, NULL);
	failassert(scIsNonZeroApproxF(vx(v_rh)) && scIsNonZeroApproxF(vy(v_rh)) && scIsNonZeroApproxF(vz(v_rh)), vecInit3f(v_out,
		gDivSafeF(vx(v_lh), vx(v_rh)),
		gDivSafeF(vy(v_lh), vy(v_rh)),
		gDivSafeF(vz(v_lh), vz(v_rh))));
	vx(v_out) = gDiv(vx(v_lh), vx(v_rh));
	vy(v_out) = gDiv(vy(v_lh), vy(v_rh));
	vz(v_out) = gDiv(vz(v_lh), vz(v_rh));
	return v_out;
}

CDRAW_INL floatN_t vecMod3f(float3_t v_out, float3_t const v_lh, float3_t const v_rh)
{
	failassert(v_out && v_lh && v_rh, NULL);
	failassert(scIsNonZeroApproxF(vx(v_rh)) && scIsNonZeroApproxF(vy(v_rh)) && scIsNonZeroApproxF(vz(v_rh)), vecInit3f(v_out,
		gModSafeF(vx(v_lh), vx(v_rh)),
		gModSafeF(vy(v_lh), vy(v_rh)),
		gModSafeF(vz(v_lh), vz(v_rh))));
	vx(v_out) = gModF(vx(v_lh), vx(v_rh));
	vy(v_out) = gModF(vy(v_lh), vy(v_rh));
	vz(v_out) = gModF(vz(v_lh), vz(v_rh));
	return v_out;
}

CDRAW_INL floatN_t vecDivMod3f(float3_t v_out, float3_t v_mod_out, float3_t const v_lh, float3_t const v_rh)
{
	failassert(v_out && v_mod_out && v_lh && v_rh, NULL);
	failassert(scIsNonZeroApproxF(vx(v_rh)) && scIsNonZeroApproxF(vy(v_rh)) && scIsNonZeroApproxF(vz(v_rh)), vecInit3f(v_out,
		gDivModSafeF(vx(v_mod_out), vx(v_lh), vx(v_rh)),
		gDivModSafeF(vy(v_mod_out), vy(v_lh), vy(v_rh)),
		gDivModSafeF(vz(v_mod_out), vz(v_lh), vz(v_rh))));
	vx(v_out) = gDiv(vx(v_lh), vx(v_rh));
	vy(v_out) = gDiv(vy(v_lh), vy(v_rh));
	vz(v_out) = gDiv(vz(v_lh), vz(v_rh));
	vx(v_mod_out) = gModQF(vx(v_lh), vx(v_rh), vx(v_out));
	vy(v_mod_out) = gModQF(vy(v_lh), vy(v_rh), vy(v_out));
	vz(v_mod_out) = gModQF(vz(v_lh), vz(v_rh), vz(v_out));
	return v_out;
}

CDRAW_INL floatN_t vecAddS3f(float3_t v_out, float3_t const v_lh, vecf_t const s_rh)
{
	failassert(v_out && v_lh, NULL);
	vx(v_out) = vx(v_lh) + s_rh;
	vy(v_out) = vy(v_lh) + s_rh;
	vz(v_out) = vz(v_lh) + s_rh;
	return v_out;
}

CDRAW_INL floatN_t vecSubS3f(float3_t v_out, float3_t const v_lh, vecf_t const s_rh)
{
	failassert(v_out && v_lh, NULL);
	vx(v_out) = vx(v_lh) - s_rh;
	vy(v_out) = vy(v_lh) - s_rh;
	vz(v_out) = vz(v_lh) - s_rh;
	return v_out;
}

CDRAW_INL floatN_t vecMulS3f(float3_t v_out, float3_t const v_lh, vecf_t const s_rh)
{
	failassert(v_out && v_lh, NULL);
	vx(v_out) = vx(v_lh) * s_rh;
	vy(v_out) = vy(v_lh) * s_rh;
	vz(v_out) = vz(v_lh) * s_rh;
	return v_out;
}

CDRAW_INL floatN_t vecDivS3f(float3_t v_out, float3_t const v_lh, vecf_t const s_rh)
{
	failassert(v_out && v_lh, NULL);
	failassert(scIsNonZeroApproxF(s_rh), vecZero3f(v_out));
	vecf_t const recip = sc1F / s_rh;
	vx(v_out) = vx(v_lh) * recip;
	vy(v_out) = vy(v_lh) * recip;
	vz(v_out) = vz(v_lh) * recip;
	return v_out;
}

CDRAW_INL floatN_t vecModS3f(float3_t v_out, float3_t const v_lh, vecf_t const s_rh)
{
	failassert(v_out && v_lh, NULL);
	failassert(scIsNonZeroApproxF(s_rh), vecCopy3f(v_out, v_lh));
	vecf_t const recip = sc1F / s_rh;
	vx(v_out) = gModQF(vx(v_lh), s_rh, vx(v_lh) * recip);
	vy(v_out) = gModQF(vy(v_lh), s_rh, vy(v_lh) * recip);
	vz(v_out) = gModQF(vz(v_lh), s_rh, vz(v_lh) * recip);
	return v_out;
}

CDRAW_INL floatN_t vecDivModS3f(float3_t v_out, float3_t v_mod_out, float3_t const v_lh, vecf_t const s_rh)
{
	failassert(v_out && v_mod_out && v_lh, NULL);
	failassert(scIsNonZeroApproxF(s_rh), vecCopy3f(v_mod_out, v_lh), vecZero3f(v_out));
	vecf_t const recip = sc1F / s_rh;
	vx(v_out) = vx(v_lh) * recip;
	vy(v_out) = vy(v_lh) * recip;
	vz(v_out) = vz(v_lh) * recip;
	vx(v_mod_out) = gModQF(vx(v_lh), s_rh, vx(v_out));
	vy(v_mod_out) = gModQF(vy(v_lh), s_rh, vy(v_out));
	vz(v_mod_out) = gModQF(vz(v_lh), s_rh, vz(v_out));
	return v_out;
}

CDRAW_INL vecf_t vecDot3f(float3_t const v_lh, float3_t const v_rh)
{
	failassert(v_lh && v_rh, 0);
	return (vx(v_lh) * vx(v_rh) + vy(v_lh) * vy(v_rh) + vz(v_lh) * vz(v_rh));
}

CDRAW_INL floatN_t vecCross3f(float3_t v_out, float3_t const v_lh, float3_t const v_rh)
{
	failassert(v_out && v_lh && v_rh, NULL);
	vx(v_out) = (vy(v_lh) * vz(v_rh) - vz(v_lh) * vy(v_rh));
	vy(v_out) = (vz(v_lh) * vx(v_rh) - vx(v_lh) * vz(v_rh));
	vz(v_out) = (vx(v_lh) * vy(v_rh) - vy(v_lh) * vx(v_rh));
	return v_out;
}

CDRAW_INL vecf_t vecLenSq3f(float3_t const v)
{
	failassert(v, 0);
	return (gSq(vx(v)) + gSq(vy(v)) + gSq(vz(v)));
}

CDRAW_INL vecf_t vecDistSq3f(float3_t const v_lh, float3_t const v_rh)
{
	failassert(v_lh && v_rh, 0);
	float3_t const disp = {
		(vx(v_lh) - vx(v_rh)),
		(vy(v_lh) - vy(v_rh)),
		(vz(v_lh) - vz(v_rh)),
	};
	return vecLenSq3f(disp);
}

CDRAW_INL vecf_t vecDispDistSq3f(float3_t v_disp_out, float3_t const v_lh, float3_t const v_rh)
{
	failassert(v_disp_out && v_lh && v_rh, 0);
	vx(v_disp_out) = (vx(v_lh) - vx(v_rh));
	vy(v_disp_out) = (vy(v_lh) - vy(v_rh));
	vz(v_disp_out) = (vz(v_lh) - vz(v_rh));
	return vecLenSq3f(v_disp_out);
}

CDRAW_INL vecb_t vecIsZero3f(float3_t const v)
{
	failassert(v, true);
	return scIsNonPositiveApproxF(vecLenSq3f(v));
}

CDRAW_INL vecb_t vecIsNonZero3f(float3_t const v)
{
	failassert(v, false);
	return scIsPositiveApproxF(vecLenSq3f(v));
}

CDRAW_INL floatN_t vecMad3f(float3_t v_out, vecf_t const u, float3_t const v_origin, float3_t const v_delta)
{
	failassert(v_out && v_origin && v_delta, NULL);
	vx(v_out) = gMad(vx(v_origin), vx(v_delta), u);
	vy(v_out) = gMad(vy(v_origin), vy(v_delta), u);
	vz(v_out) = gMad(vz(v_origin), vz(v_delta), u);
	return v_out;
}

CDRAW_INL floatN_t vecLerp3f(float3_t v_out, vecf_t const u, float3_t const v_min, float3_t const v_max)
{
	failassert(v_out && v_min && v_max, NULL);
	vx(v_out) = gLerp(vx(v_min), vx(v_max), u);
	vy(v_out) = gLerp(vy(v_min), vy(v_max), u);
	vz(v_out) = gLerp(vz(v_min), vz(v_max), u);
	return v_out;
}

CDRAW_INL vecf_t vecLen3f(float3_t const v)
{
	failassert(v, sc0F);
	vecf_t const lenSq = vecLenSq3f(v);
	return scSqrtF(lenSq);
}

CDRAW_INL vecf_t vecLenInv3f(float3_t const v)
{
	failassert(v, sc0F);
	vecf_t const lenSq = vecLenSq3f(v);
	failassert(scIsPositiveApproxF(lenSq), sc0F);
	return (sc1F / gSafeSqrtF(lenSq));
}

CDRAW_INL vecf_t vecDist3f(float3_t const v_lh, float3_t const v_rh)
{
	failassert(v_lh && v_rh, sc0F);
	return scSqrtF(vecDistSq3f(v_lh, v_rh));
}

CDRAW_INL vecf_t vecDispDist3f(float3_t v_disp_out, float3_t const v_lh, float3_t const v_rh)
{
	failassert(v_disp_out && v_lh && v_rh, sc0F);
	return scSqrtF(vecDispDistSq3f(v_disp_out, v_lh, v_rh));
}

CDRAW_INL vecf_t vecNormalize3f(float3_t v_out, float3_t const v)
{
	failassert(v_out && v, sc0F);
	vecf_t len = vecLenSq3f(v), ratio;
	if (scIsNonPositiveApproxF(len))
		return (vecZero3f(v_out), sc0F);
	len = gSafeSqrtF(len);
	ratio = sc1F / len;
	vx(v_out) = vx(v) * ratio;
	vy(v_out) = vy(v) * ratio;
	vz(v_out) = vz(v) * ratio;
	return len;
}

CDRAW_INL vecf_t vecResize3f(float3_t v_out, float3_t const v, vecf_t const newLen)
{
	failassert(v_out && v, sc0F);
	vecf_t len = vecLenSq3f(v), ratio;
	if (scIsNonPositiveApproxF(len))
		return (vecZero3f(v_out), sc0F);
	len = gSafeSqrtF(len);
	ratio = newLen / len;
	vx(v_out) = vx(v) * ratio;
	vy(v_out) = vy(v) * ratio;
	vz(v_out) = vz(v) * ratio;
	return len;
}

CDRAW_INL vecb_t vecIsUnit3f(float3_t const v)
{
	failassert(v, false);
	vecf_t const lenSq = vecLenSq3f(v);
	return ((lenSq >= scEpsL1F) && (lenSq <= scEpsG1F));
}

CDRAW_INL vecb_t vecIsNonUnit3f(float3_t const v)
{
	failassert(v, true);
	vecf_t const lenSq = vecLenSq3f(v);
	return ((lenSq < scEpsL1F) || (lenSq > scEpsG1F));
}

CDRAW_INL vecf_t vecProjS3f(float3_t const v, float3_t const v_base)
{
	failassert(v && v_base, sc0F);
	vecf_t const lenSq = vecLenSq3f(v);
	if (scIsNonPositiveApproxF(lenSq))
		return sc0F;
	return (vecDot3f(v, v_base) / lenSq);
}

CDRAW_INL vecf_t vecProj3f(float3_t v_out, float3_t const v, float3_t const v_base)
{
	failassert(v_out && v && v_base, sc0F);
	failassert(v && v_base, sc0F);
	vecf_t ratio = vecLenSq3f(v);
	if (scIsNonPositiveApproxF(ratio))
		return sc0F;
	ratio = vecDot3f(v, v_base) / ratio;
	vx(v_out) = vx(v_base) * ratio;
	vy(v_out) = vy(v_base) * ratio;
	vz(v_out) = vz(v_base) * ratio;
	return ratio;
}

CDRAW_INL vecf_t vecLerpInv3f(float3_t const v, float3_t const v_min, float3_t const v_max)
{
	failassert(v && v_min && v_max, sc0F);
	float3_t v_delta;
	vecf_t const distSq = vecDispDistSq3f(v_delta, v_max, v_min);
	if (scIsNonPositiveApproxF(distSq))
		return sc0F;
	return ((vx(v) - vx(v_min)) * vx(v_delta) + (vy(v) - vy(v_min)) * vy(v_delta) + (vz(v) - vz(v_min)) * vz(v_delta)) / distSq;
}

CDRAW_INL vecf_t vecOrtho3f(float3_t v_out, float3_t const v, float3_t const v_base)
{
	failassert(v_out && v && v_base, sc0F);
	vecf_t ratio = vecLenSq3f(v_base);
	if (scIsNonPositiveApproxF(ratio))
		return (vecZero3f(v_out), ratio);
	ratio = vecDot3f(v, v_base) / ratio;
	vx(v_out) = (vx(v) - vx(v_base) * ratio);
	vy(v_out) = (vy(v) - vy(v_base) * ratio);
	vz(v_out) = (vz(v) - vz(v_base) * ratio);
	return ratio;
}


CDRAW_INL vecf_t vecOrthoBasis3f(float3_t v2_out, float3_t v1_out, vecf_t* v2_basefactor_out_opt, vecf_t* v1_basefactor_out_opt, float3_t const v2, float3_t const v1, float3_t const v_base)
{
	failassert(v2_out && v1_out && v2 && v1 && v_base, sc0F);
	vecf_t ratio = vecLenSq3f(v_base), ratio1, ratio2;
	if (scIsNonPositiveApproxF(ratio))
		return (vecZero3f(v2_out), vecZero3f(v1_out), sc0F);
	ratio = sc1F / ratio;
	ratio1 = vecDot3f(v1, v_base) * ratio;
	ratio2 = vecDot3f(v2, v_base) * ratio;
	vx(v1_out) = (vx(v1) - vx(v_base) * ratio1);
	vy(v1_out) = (vy(v1) - vy(v_base) * ratio1);
	vz(v1_out) = (vz(v1) - vz(v_base) * ratio1);
	ratio = vecDot3f(v2, v1_out) / vecLenSq3f(v1_out);
	vx(v2_out) = (vx(v2) - vx(v_base) * ratio2 - vx(v1_out) * ratio);
	vy(v2_out) = (vy(v2) - vy(v_base) * ratio2 - vy(v1_out) * ratio);
	vz(v2_out) = (vz(v2) - vz(v_base) * ratio2 - vz(v1_out) * ratio);
	if (v1_basefactor_out_opt) *v1_basefactor_out_opt = ratio1;
	if (v2_basefactor_out_opt) *v2_basefactor_out_opt = ratio2;
	return ratio;
}


#endif // #if (!(defined _CDRAW_VEC3F_INL_) && (defined _CDRAW_VECTOR_INL_))