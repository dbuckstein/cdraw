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
* cdrawVec2f.inl
* Definitions of single-precision 2D vector functions.
*/

#if (!(defined _CDRAW_VEC2F_INL_) && (defined _CDRAW_VECTOR_INL_))
#define _CDRAW_VEC2F_INL_


CDRAW_INL floatN_t vecZero2f(float2_t v_out)
{
	failassert(v_out, NULL);
	vx(v_out) = 0.0f;
	vy(v_out) = 0.0f;
	return v_out;
}

CDRAW_INL floatN_t vecInit2f(float2_t v_out, vecf_t const x, vecf_t const y)
{
	failassert(v_out, NULL);
	vx(v_out) = x;
	vy(v_out) = y;
	return v_out;
}

CDRAW_INL floatN_t vecInitS2f(float2_t v_out, vecf_t const x)
{
	failassert(v_out, NULL);
	vx(v_out) = x;
	vy(v_out) = x;
	return v_out;
}

CDRAW_INL floatN_t vecCopy2f(float2_t v_out, float2_t const v)
{
	failassert(v_out && v, NULL);
	vx(v_out) = vx(v);
	vy(v_out) = vy(v);
	return v_out;
}

CDRAW_INL floatN_t vecCopy2b2f(float2_t v_out, bool2_t const v)
{
	failassert(v_out && v, NULL);
	vx(v_out) = (vx(v) ? 1.0f : 0.0f);
	vy(v_out) = (vy(v) ? 1.0f : 0.0f);
	return v_out;
}

CDRAW_INL floatN_t vecCopy2i2f(float2_t v_out, int2_t const v)
{
	failassert(v_out && v, NULL);
	vx(v_out) = (vecf_t)(vx(v));
	vy(v_out) = (vecf_t)(vy(v));
	return v_out;
}

CDRAW_INL floatN_t vecCopy2u2f(float2_t v_out, uint2_t const v)
{
	failassert(v_out && v, NULL);
	vx(v_out) = (vecf_t)(vx(v));
	vy(v_out) = (vecf_t)(vy(v));
	return v_out;
}

CDRAW_INL floatN_t vecCopy2d2f(float2_t v_out, double2_t const v)
{
	failassert(v_out && v, NULL);
	vx(v_out) = (vecf_t)(vx(v));
	vy(v_out) = (vecf_t)(vy(v));
	return v_out;
}

CDRAW_INL boolN_t vecEq2f(bool2_t v_out, float2_t const v_lh, float2_t const v_rh)
{
	failassert(v_out && v_lh && v_rh, NULL);
	vx(v_out) = (vx(v_lh) == vx(v_rh));
	vy(v_out) = (vy(v_lh) == vy(v_rh));
	return v_out;
}

CDRAW_INL boolN_t vecNeq2f(bool2_t v_out, float2_t const v_lh, float2_t const v_rh)
{
	failassert(v_out && v_lh && v_rh, NULL);
	vx(v_out) = (vx(v_lh) != vx(v_rh));
	vy(v_out) = (vy(v_lh) != vy(v_rh));
	return v_out;
}

CDRAW_INL boolN_t vecGreater2f(bool2_t v_out, float2_t const v_lh, float2_t const v_rh)
{
	failassert(v_out && v_lh && v_rh, NULL);
	vx(v_out) = (vx(v_lh) > vx(v_rh));
	vy(v_out) = (vy(v_lh) > vy(v_rh));
	return v_out;
}

CDRAW_INL boolN_t vecGreaterEq2f(bool2_t v_out, float2_t const v_lh, float2_t const v_rh)
{
	failassert(v_out && v_lh && v_rh, NULL);
	vx(v_out) = (vx(v_lh) >= vx(v_rh));
	vy(v_out) = (vy(v_lh) >= vy(v_rh));
	return v_out;
}

CDRAW_INL boolN_t vecLess2f(bool2_t v_out, float2_t const v_lh, float2_t const v_rh)
{
	failassert(v_out && v_lh && v_rh, NULL);
	vx(v_out) = (vx(v_lh) < vx(v_rh));
	vy(v_out) = (vy(v_lh) < vy(v_rh));
	return v_out;
}

CDRAW_INL boolN_t vecLessEq2f(bool2_t v_out, float2_t const v_lh, float2_t const v_rh)
{
	failassert(v_out && v_lh && v_rh, NULL);
	vx(v_out) = (vx(v_lh) <= vx(v_rh));
	vy(v_out) = (vy(v_lh) <= vy(v_rh));
	return v_out;
}

CDRAW_INL floatN_t vecNegate2f(float2_t v_out, float2_t const v)
{
	failassert(v_out && v, NULL);
	vx(v_out) = -vx(v);
	vy(v_out) = -vy(v);
	return v_out;
}

CDRAW_INL floatN_t vecAdd2f(float2_t v_out, float2_t const v_lh, float2_t const v_rh)
{
	failassert(v_out && v_lh && v_rh, NULL);
	vx(v_out) = vx(v_lh) + vx(v_rh);
	vy(v_out) = vy(v_lh) + vy(v_rh);
	return v_out;
}

CDRAW_INL floatN_t vecSub2f(float2_t v_out, float2_t const v_lh, float2_t const v_rh)
{
	failassert(v_out && v_lh && v_rh, NULL);
	vx(v_out) = vx(v_lh) - vx(v_rh);
	vy(v_out) = vy(v_lh) - vy(v_rh);
	return v_out;
}

CDRAW_INL floatN_t vecMul2f(float2_t v_out, float2_t const v_lh, float2_t const v_rh)
{
	failassert(v_out && v_lh && v_rh, NULL);
	vx(v_out) = vx(v_lh) * vx(v_rh);
	vy(v_out) = vy(v_lh) * vy(v_rh);
	return v_out;
}

CDRAW_INL floatN_t vecDiv2f(float2_t v_out, float2_t const v_lh, float2_t const v_rh)
{
	failassert(v_out && v_lh && v_rh, NULL);
	failassert(scIsNonZeroApproxF(vx(v_rh)) && scIsNonZeroApproxF(vy(v_rh)), vecInit2f(v_out,
		gDivSafeF(vx(v_lh), vx(v_rh)),
		gDivSafeF(vy(v_lh), vy(v_rh))));
	vx(v_out) = gDiv(vx(v_lh), vx(v_rh));
	vy(v_out) = gDiv(vy(v_lh), vy(v_rh));
	return v_out;
}

CDRAW_INL floatN_t vecMod2f(float2_t v_out, float2_t const v_lh, float2_t const v_rh)
{
	failassert(v_out && v_lh && v_rh, NULL);
	failassert(scIsNonZeroApproxF(vx(v_rh)) && scIsNonZeroApproxF(vy(v_rh)), vecInit2f(v_out,
		gModSafeF(vx(v_lh), vx(v_rh)),
		gModSafeF(vy(v_lh), vy(v_rh))));
	vx(v_out) = gModF(vx(v_lh), vx(v_rh));
	vy(v_out) = gModF(vy(v_lh), vy(v_rh));
	return v_out;
}

CDRAW_INL floatN_t vecDivMod2f(float2_t v_out, float2_t v_mod_out, float2_t const v_lh, float2_t const v_rh)
{
	failassert(v_out && v_mod_out && v_lh && v_rh, NULL);
	failassert((v_mod_out != v_out) && (v_mod_out != v_lh) && (v_mod_out != v_rh), NULL);
	failassert(scIsNonZeroApproxF(vx(v_rh)) && scIsNonZeroApproxF(vy(v_rh)), vecInit2f(v_out,
		gDivModSafeF(vx(v_mod_out), vx(v_lh), vx(v_rh)),
		gDivModSafeF(vy(v_mod_out), vy(v_lh), vy(v_rh))));
	vx(v_out) = gDiv(vx(v_lh), vx(v_rh));
	vy(v_out) = gDiv(vy(v_lh), vy(v_rh));
	vx(v_mod_out) = gModQF(vx(v_lh), vx(v_rh), vx(v_out));
	vy(v_mod_out) = gModQF(vy(v_lh), vy(v_rh), vy(v_out));
	return v_out;
}

CDRAW_INL floatN_t vecAddS2f(float2_t v_out, float2_t const v_lh, vecf_t const s_rh)
{
	failassert(v_out && v_lh, NULL);
	vx(v_out) = vx(v_lh) + s_rh;
	vy(v_out) = vy(v_lh) + s_rh;
	return v_out;
}

CDRAW_INL floatN_t vecSubS2f(float2_t v_out, float2_t const v_lh, vecf_t const s_rh)
{
	failassert(v_out && v_lh, NULL);
	vx(v_out) = vx(v_lh) - s_rh;
	vy(v_out) = vy(v_lh) - s_rh;
	return v_out;
}

CDRAW_INL floatN_t vecMulS2f(float2_t v_out, float2_t const v_lh, vecf_t const s_rh)
{
	failassert(v_out && v_lh, NULL);
	vx(v_out) = vx(v_lh) * s_rh;
	vy(v_out) = vy(v_lh) * s_rh;
	return v_out;
}

CDRAW_INL floatN_t vecDivS2f(float2_t v_out, float2_t const v_lh, vecf_t const s_rh)
{
	failassert(v_out && v_lh, NULL);
	failassert(s_rh, vecZero2f(v_out));
	vecf_t const recip = sc1F / s_rh;
	vx(v_out) = vx(v_lh) * recip;
	vy(v_out) = vy(v_lh) * recip;
	return v_out;
}

CDRAW_INL floatN_t vecModS2f(float2_t v_out, float2_t const v_lh, vecf_t const s_rh)
{
	failassert(v_out && v_lh, NULL);
	failassert(s_rh, vecCopy2f(v_out, v_lh));
	vecf_t const recip = sc1F / s_rh;
	vx(v_out) = gModQF(vx(v_lh), s_rh, vx(v_lh) * recip);
	vy(v_out) = gModQF(vy(v_lh), s_rh, vy(v_lh) * recip);
	return v_out;
}

CDRAW_INL floatN_t vecDivModS2f(float2_t v_out, float2_t v_mod_out, float2_t const v_lh, vecf_t const s_rh)
{
	failassert(v_out && v_mod_out && v_lh, NULL);
	failassert((v_mod_out != v_out) && (v_mod_out != v_lh), NULL);
	failassert(s_rh, vecCopy2f(v_mod_out, v_lh), vecZero2f(v_out));
	vecf_t const recip = sc1F / s_rh;
	vx(v_out) = vx(v_lh) * recip;
	vy(v_out) = vy(v_lh) * recip;
	vx(v_mod_out) = gModQF(vx(v_lh), s_rh, vx(v_out));
	vy(v_mod_out) = gModQF(vy(v_lh), s_rh, vy(v_out));
	return v_out;
}

CDRAW_INL vecf_t vecDot2f(float2_t const v_lh, float2_t const v_rh)
{
	failassert(v_lh && v_rh, 0);
	return (vx(v_lh) * vx(v_rh) + vy(v_lh) * vy(v_rh));
}

CDRAW_INL vecf_t vecCross2f(float2_t const v_lh, float2_t const v_rh)
{
	failassert(v_lh && v_rh, 0);
	return (vx(v_lh) * vy(v_rh) - vy(v_lh) * vx(v_rh));
}

CDRAW_INL vecf_t vecLenSq2f(float2_t const v)
{
	failassert(v, 0);
	return (gSq(vx(v)) + gSq(vy(v)));
}

CDRAW_INL vecf_t vecDistSq2f(float2_t const v_lh, float2_t const v_rh)
{
	failassert(v_lh && v_rh, 0);
	float2_t const disp = {
		(vx(v_lh) - vx(v_rh)),
		(vy(v_lh) - vy(v_rh)),
	};
	return vecLenSq2f(disp);
}

CDRAW_INL vecf_t vecDispDistSq2f(float2_t v_disp_out, float2_t const v_lh, float2_t const v_rh)
{
	failassert(v_disp_out && v_lh && v_rh, 0);
	vx(v_disp_out) = (vx(v_lh) - vx(v_rh));
	vy(v_disp_out) = (vy(v_lh) - vy(v_rh));
	return vecLenSq2f(v_disp_out);
}

CDRAW_INL vecb_t vecIsZero2f(float2_t const v)
{
	failassert(v, true);
	return scIsNonPositiveApproxF(vecLenSq2f(v));
}

CDRAW_INL vecb_t vecIsNonZero2f(float2_t const v)
{
	failassert(v, false);
	return scIsPositiveApproxF(vecLenSq2f(v));
}

CDRAW_INL floatN_t vecMad2f(float2_t v_out, vecf_t const u, float2_t const v_origin, float2_t const v_delta)
{
	failassert(v_out && v_origin && v_delta, NULL);
	vx(v_out) = gMad(vx(v_origin), vx(v_delta), u);
	vy(v_out) = gMad(vy(v_origin), vy(v_delta), u);
	return v_out;
}

CDRAW_INL floatN_t vecLerp2f(float2_t v_out, vecf_t const u, float2_t const v_min, float2_t const v_max)
{
	failassert(v_out && v_min && v_max, NULL);
	vx(v_out) = gLerp(vx(v_min), vx(v_max), u);
	vy(v_out) = gLerp(vy(v_min), vy(v_max), u);
	return v_out;
}

CDRAW_INL vecf_t vecLen2f(float2_t const v)
{
	failassert(v, sc0F);
	vecf_t const lenSq = vecLenSq2f(v);
	return scSqrtF(lenSq);
}

CDRAW_INL vecf_t vecLenInv2f(float2_t const v)
{
	failassert(v, sc0F);
	vecf_t const lenSq = vecLenSq2f(v);
	failassert(scIsPositiveApproxF(lenSq), sc0F);
	return (sc1F / gSafeSqrtF(lenSq));
}

CDRAW_INL vecf_t vecDist2f(float2_t const v_lh, float2_t const v_rh)
{
	failassert(v_lh && v_rh, sc0F);
	return scSqrtF(vecDistSq2f(v_lh, v_rh));
}

CDRAW_INL vecf_t vecDispDist2f(float2_t v_disp_out, float2_t const v_lh, float2_t const v_rh)
{
	failassert(v_disp_out && v_lh && v_rh, sc0F);
	return scSqrtF(vecDispDistSq2f(v_disp_out, v_lh, v_rh));
}

CDRAW_INL vecf_t vecNormalize2f(float2_t v_out, float2_t const v)
{
	failassert(v_out && v, sc0F);
	vecf_t len = vecLenSq2f(v), ratio;
	if (scIsNonPositiveApproxF(len))
		return (vecZero2f(v_out), sc0F);
	len = gSafeSqrtF(len);
	ratio = sc1F / len;
	vx(v_out) = vx(v) * ratio;
	vy(v_out) = vy(v) * ratio;
	return len;
}

CDRAW_INL vecf_t vecResize2f(float2_t v_out, float2_t const v, vecf_t const newLen)
{
	failassert(v_out && v, sc0F);
	vecf_t len = vecLenSq2f(v), ratio;
	if (scIsNonPositiveApproxF(len))
		return (vecZero2f(v_out), sc0F);
	len = gSafeSqrtF(len);
	ratio = newLen / len;
	vx(v_out) = vx(v) * ratio;
	vy(v_out) = vy(v) * ratio;
	return len;
}

CDRAW_INL vecb_t vecIsUnit2f(float2_t const v)
{
	failassert(v, false);
	vecf_t const lenSq = vecLenSq2f(v);
	return ((lenSq >= scEpsL1F) && (lenSq <= scEpsG1F));
}

CDRAW_INL vecb_t vecIsNonUnit2f(float2_t const v)
{
	failassert(v, true);
	vecf_t const lenSq = vecLenSq2f(v);
	return ((lenSq < scEpsL1F) || (lenSq > scEpsG1F));
}

CDRAW_INL vecf_t vecProjS2f(float2_t const v, float2_t const v_base)
{
	failassert(v && v_base, sc0F);
	vecf_t const lenSq = vecLenSq2f(v_base);
	if (scIsNonPositiveApproxF(lenSq))
		return sc0F;
	return (vecDot2f(v, v_base) / lenSq);
}

CDRAW_INL vecf_t vecProj2f(float2_t v_out, float2_t const v, float2_t const v_base)
{
	failassert(v_out && v && v_base, sc0F);
	vecf_t ratio = vecLenSq2f(v_base);
	if (scIsNonPositiveApproxF(ratio))
		return sc0F;
	ratio = vecDot2f(v, v_base) / ratio;
	vx(v_out) = vx(v_base) * ratio;
	vy(v_out) = vy(v_base) * ratio;
	return ratio;
}

CDRAW_INL vecf_t vecLerpInv2f(float2_t const v, float2_t const v_min, float2_t const v_max)
{
	failassert(v && v_min && v_max, sc0F);
	float2_t v_delta;
	vecf_t const distSq = vecDispDistSq2f(v_delta, v_max, v_min);
	if (scIsNonPositiveApproxF(distSq))
		return sc0F;
	return ((vx(v) - vx(v_min)) * vx(v_delta) + (vy(v) - vy(v_min)) * vy(v_delta)) / distSq;
}

CDRAW_INL vecf_t vecOrtho2f(float2_t v_out, float2_t const v, float2_t const v_base)
{
	failassert(v_out && v && v_base, sc0F);
	vecf_t ratio = vecLenSq2f(v_base);
	if (scIsNonPositiveApproxF(ratio))
		return (vecZero2f(v_out), sc0F);
	ratio = vecDot2f(v, v_base) / ratio;
	vx(v_out) = (vx(v) - vx(v_base) * ratio);
	vy(v_out) = (vy(v) - vy(v_base) * ratio);
	return ratio;
}


#endif // #if (!(defined _CDRAW_VEC2F_INL_) && (defined _CDRAW_VECTOR_INL_))