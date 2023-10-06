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
* cdrawVec2d.inl
* Definitions of double-precision 2D vector functions.
*/

#if (!(defined _CDRAW_VEC2D_INL_) && (defined _CDRAW_VECTOR_INL_))
#define _CDRAW_VEC2D_INL_


CDRAW_INL doubleN_t vecZero2d(double2_t v_out)
{
	failassert(v_out, NULL);
	vx(v_out) = 0.0;
	vy(v_out) = 0.0;
	return v_out;
}

CDRAW_INL doubleN_t vecInit2d(double2_t v_out, vecd_t const x, vecd_t const y)
{
	failassert(v_out, NULL);
	vx(v_out) = x;
	vy(v_out) = y;
	return v_out;
}

CDRAW_INL doubleN_t vecInitS2d(double2_t v_out, vecd_t const x)
{
	failassert(v_out, NULL);
	vx(v_out) = x;
	vy(v_out) = x;
	return v_out;
}

CDRAW_INL doubleN_t vecCopy2d(double2_t v_out, double2_t const v)
{
	failassert(v_out && v, NULL);
	vx(v_out) = vx(v);
	vy(v_out) = vy(v);
	return v_out;
}

CDRAW_INL doubleN_t vecCopy2b2d(double2_t v_out, bool2_t const v)
{
	failassert(v_out && v, NULL);
	vx(v_out) = (vx(v) ? 1.0 : 0.0);
	vy(v_out) = (vy(v) ? 1.0 : 0.0);
	return v_out;
}

CDRAW_INL doubleN_t vecCopy2i2d(double2_t v_out, int2_t const v)
{
	failassert(v_out && v, NULL);
	vx(v_out) = (vecd_t)(vx(v));
	vy(v_out) = (vecd_t)(vy(v));
	return v_out;
}

CDRAW_INL doubleN_t vecCopy2u2d(double2_t v_out, uint2_t const v)
{
	failassert(v_out && v, NULL);
	vx(v_out) = (vecd_t)(vx(v));
	vy(v_out) = (vecd_t)(vy(v));
	return v_out;
}

CDRAW_INL doubleN_t vecCopy2f2d(double2_t v_out, float2_t const v)
{
	failassert(v_out && v, NULL);
	vx(v_out) = (vecd_t)(vx(v));
	vy(v_out) = (vecd_t)(vy(v));
	return v_out;
}

CDRAW_INL boolN_t vecEq2d(bool2_t v_out, double2_t const v_lh, double2_t const v_rh)
{
	failassert(v_out && v_lh && v_rh, NULL);
	vx(v_out) = (vx(v_lh) == vx(v_rh));
	vy(v_out) = (vy(v_lh) == vy(v_rh));
	return v_out;
}

CDRAW_INL boolN_t vecNeq2d(bool2_t v_out, double2_t const v_lh, double2_t const v_rh)
{
	failassert(v_out && v_lh && v_rh, NULL);
	vx(v_out) = (vx(v_lh) != vx(v_rh));
	vy(v_out) = (vy(v_lh) != vy(v_rh));
	return v_out;
}

CDRAW_INL boolN_t vecGreater2d(bool2_t v_out, double2_t const v_lh, double2_t const v_rh)
{
	failassert(v_out && v_lh && v_rh, NULL);
	vx(v_out) = (vx(v_lh) > vx(v_rh));
	vy(v_out) = (vy(v_lh) > vy(v_rh));
	return v_out;
}

CDRAW_INL boolN_t vecGreaterEq2d(bool2_t v_out, double2_t const v_lh, double2_t const v_rh)
{
	failassert(v_out && v_lh && v_rh, NULL);
	vx(v_out) = (vx(v_lh) >= vx(v_rh));
	vy(v_out) = (vy(v_lh) >= vy(v_rh));
	return v_out;
}

CDRAW_INL boolN_t vecLess2d(bool2_t v_out, double2_t const v_lh, double2_t const v_rh)
{
	failassert(v_out && v_lh && v_rh, NULL);
	vx(v_out) = (vx(v_lh) < vx(v_rh));
	vy(v_out) = (vy(v_lh) < vy(v_rh));
	return v_out;
}

CDRAW_INL boolN_t vecLessEq2d(bool2_t v_out, double2_t const v_lh, double2_t const v_rh)
{
	failassert(v_out && v_lh && v_rh, NULL);
	vx(v_out) = (vx(v_lh) <= vx(v_rh));
	vy(v_out) = (vy(v_lh) <= vy(v_rh));
	return v_out;
}

CDRAW_INL doubleN_t vecNegate2d(double2_t v_out, double2_t const v)
{
	failassert(v_out && v, NULL);
	vx(v_out) = -vx(v);
	vy(v_out) = -vy(v);
	return v_out;
}

CDRAW_INL doubleN_t vecAdd2d(double2_t v_out, double2_t const v_lh, double2_t const v_rh)
{
	failassert(v_out && v_lh && v_rh, NULL);
	vx(v_out) = vx(v_lh) + vx(v_rh);
	vy(v_out) = vy(v_lh) + vy(v_rh);
	return v_out;
}

CDRAW_INL doubleN_t vecSub2d(double2_t v_out, double2_t const v_lh, double2_t const v_rh)
{
	failassert(v_out && v_lh && v_rh, NULL);
	vx(v_out) = vx(v_lh) - vx(v_rh);
	vy(v_out) = vy(v_lh) - vy(v_rh);
	return v_out;
}

CDRAW_INL doubleN_t vecMul2d(double2_t v_out, double2_t const v_lh, double2_t const v_rh)
{
	failassert(v_out && v_lh && v_rh, NULL);
	vx(v_out) = vx(v_lh) * vx(v_rh);
	vy(v_out) = vy(v_lh) * vy(v_rh);
	return v_out;
}

CDRAW_INL doubleN_t vecDiv2d(double2_t v_out, double2_t const v_lh, double2_t const v_rh)
{
	failassert(v_out && v_lh && v_rh, NULL);
	failassert(scIsNonZeroApproxD(vx(v_rh)) && scIsNonZeroApproxD(vy(v_rh)), vecInit2d(v_out,
		gDivSafeD(vx(v_lh), vx(v_rh)),
		gDivSafeD(vy(v_lh), vy(v_rh))));
	vx(v_out) = gDiv(vx(v_lh), vx(v_rh));
	vy(v_out) = gDiv(vy(v_lh), vy(v_rh));
	return v_out;
}

CDRAW_INL doubleN_t vecMod2d(double2_t v_out, double2_t const v_lh, double2_t const v_rh)
{
	failassert(v_out && v_lh && v_rh, NULL);
	failassert(scIsNonZeroApproxD(vx(v_rh)) && scIsNonZeroApproxD(vy(v_rh)), vecInit2d(v_out,
		gModSafeD(vx(v_lh), vx(v_rh)),
		gModSafeD(vy(v_lh), vy(v_rh))));
	vx(v_out) = gModD(vx(v_lh), vx(v_rh));
	vy(v_out) = gModD(vy(v_lh), vy(v_rh));
	return v_out;
}

CDRAW_INL doubleN_t vecDivMod2d(double2_t v_out, double2_t v_mod_out, double2_t const v_lh, double2_t const v_rh)
{
	failassert(v_out && v_mod_out && v_lh && v_rh, NULL);
	failassert(scIsNonZeroApproxD(vx(v_rh)) && scIsNonZeroApproxD(vy(v_rh)), vecInit2d(v_out,
		gDivModSafeD(vx(v_mod_out), vx(v_lh), vx(v_rh)),
		gDivModSafeD(vy(v_mod_out), vy(v_lh), vy(v_rh))));
	vx(v_out) = gDiv(vx(v_lh), vx(v_rh));
	vy(v_out) = gDiv(vy(v_lh), vy(v_rh));
	vx(v_mod_out) = gModQD(vx(v_lh), vx(v_rh), vx(v_out));
	vy(v_mod_out) = gModQD(vy(v_lh), vy(v_rh), vy(v_out));
	return v_out;
}

CDRAW_INL doubleN_t vecAddS2d(double2_t v_out, double2_t const v_lh, vecd_t const s_rh)
{
	failassert(v_out && v_lh, NULL);
	vx(v_out) = vx(v_lh) + s_rh;
	vy(v_out) = vy(v_lh) + s_rh;
	return v_out;
}

CDRAW_INL doubleN_t vecSubS2d(double2_t v_out, double2_t const v_lh, vecd_t const s_rh)
{
	failassert(v_out && v_lh, NULL);
	vx(v_out) = vx(v_lh) - s_rh;
	vy(v_out) = vy(v_lh) - s_rh;
	return v_out;
}

CDRAW_INL doubleN_t vecMulS2d(double2_t v_out, double2_t const v_lh, vecd_t const s_rh)
{
	failassert(v_out && v_lh, NULL);
	vx(v_out) = vx(v_lh) * s_rh;
	vy(v_out) = vy(v_lh) * s_rh;
	return v_out;
}

CDRAW_INL doubleN_t vecDivS2d(double2_t v_out, double2_t const v_lh, vecd_t const s_rh)
{
	failassert(v_out && v_lh, NULL);
	failassert(s_rh, vecZero2d(v_out));
	vecd_t const recip = sc1F / s_rh;
	vx(v_out) = vx(v_lh) * recip;
	vy(v_out) = vy(v_lh) * recip;
	return v_out;
}

CDRAW_INL doubleN_t vecModS2d(double2_t v_out, double2_t const v_lh, vecd_t const s_rh)
{
	failassert(v_out && v_lh, NULL);
	failassert(s_rh, vecCopy2d(v_out, v_lh));
	vecd_t const recip = sc1F / s_rh;
	vx(v_out) = gModQD(vx(v_lh), s_rh, vx(v_lh) * recip);
	vy(v_out) = gModQD(vy(v_lh), s_rh, vy(v_lh) * recip);
	return v_out;
}

CDRAW_INL doubleN_t vecDivModS2d(double2_t v_out, double2_t v_mod_out, double2_t const v_lh, vecd_t const s_rh)
{
	failassert(v_out && v_mod_out && v_lh, NULL);
	failassert(s_rh, vecCopy2d(v_mod_out, v_lh), vecZero2d(v_out));
	vecd_t const recip = sc1F / s_rh;
	vx(v_out) = vx(v_lh) * recip;
	vy(v_out) = vy(v_lh) * recip;
	vx(v_mod_out) = gModQD(vx(v_lh), s_rh, vx(v_out));
	vy(v_mod_out) = gModQD(vy(v_lh), s_rh, vy(v_out));
	return v_out;
}

CDRAW_INL vecd_t vecDot2d(double2_t const v_lh, double2_t const v_rh)
{
	failassert(v_lh && v_rh, 0);
	return (vx(v_lh) * vx(v_rh) + vy(v_lh) * vy(v_rh));
}

CDRAW_INL vecd_t vecCross2d(double2_t const v_lh, double2_t const v_rh)
{
	failassert(v_lh && v_rh, 0);
	return (vx(v_lh) * vy(v_rh) - vy(v_lh) * vx(v_rh));
}

CDRAW_INL vecd_t vecLenSq2d(double2_t const v)
{
	failassert(v, 0);
	return (gSq(vx(v)) + gSq(vy(v)));
}

CDRAW_INL vecd_t vecDistSq2d(double2_t const v_lh, double2_t const v_rh)
{
	failassert(v_lh && v_rh, 0);
	double2_t const disp = {
		(vx(v_lh) - vx(v_rh)),
		(vy(v_lh) - vy(v_rh)),
	};
	return vecLenSq2d(disp);
}

CDRAW_INL vecd_t vecDispDistSq2d(double2_t v_disp_out, double2_t const v_lh, double2_t const v_rh)
{
	failassert(v_disp_out && v_lh && v_rh, 0);
	vx(v_disp_out) = (vx(v_lh) - vx(v_rh));
	vy(v_disp_out) = (vy(v_lh) - vy(v_rh));
	return vecLenSq2d(v_disp_out);
}

CDRAW_INL vecb_t vecIsZero2d(double2_t const v)
{
	failassert(v, true);
	return scIsNonPositiveApproxD(vecLenSq2d(v));
}

CDRAW_INL vecb_t vecIsNonZero2d(double2_t const v)
{
	failassert(v, false);
	return scIsPositiveApproxD(vecLenSq2d(v));
}

CDRAW_INL doubleN_t vecMad2d(double2_t v_out, vecd_t const u, double2_t const v_origin, double2_t const v_delta)
{
	failassert(v_out && v_origin && v_delta, NULL);
	vx(v_out) = gMad(vx(v_origin), vx(v_delta), u);
	vy(v_out) = gMad(vy(v_origin), vy(v_delta), u);
	return v_out;
}

CDRAW_INL doubleN_t vecLerp2d(double2_t v_out, vecd_t const u, double2_t const v_min, double2_t const v_max)
{
	failassert(v_out && v_min && v_max, NULL);
	vx(v_out) = gLerp(vx(v_min), vx(v_max), u);
	vy(v_out) = gLerp(vy(v_min), vy(v_max), u);
	return v_out;
}

CDRAW_INL vecd_t vecLen2d(double2_t const v)
{
	failassert(v, sc0D);
	vecd_t const lenSq = vecLenSq2d(v);
	return scSqrtD(lenSq);
}

CDRAW_INL vecd_t vecLenInv2d(double2_t const v)
{
	failassert(v, sc0D);
	vecd_t const lenSq = vecLenSq2d(v);
	failassert(scIsPositiveApproxD(lenSq), sc0D);
	return (sc1F / gSafeSqrtD(lenSq));
}

CDRAW_INL vecd_t vecDist2d(double2_t const v_lh, double2_t const v_rh)
{
	failassert(v_lh && v_rh, sc0D);
	return scSqrtD(vecDistSq2d(v_lh, v_rh));
}

CDRAW_INL vecd_t vecDispDist2d(double2_t v_disp_out, double2_t const v_lh, double2_t const v_rh)
{
	failassert(v_disp_out && v_lh && v_rh, sc0D);
	return scSqrtD(vecDispDistSq2d(v_disp_out, v_lh, v_rh));
}

CDRAW_INL vecd_t vecNormalize2d(double2_t v_out, double2_t const v)
{
	failassert(v_out && v, sc0D);
	vecd_t len = vecLenSq2d(v), ratio;
	if (scIsNonPositiveApproxD(len))
		return (vecZero2d(v_out), sc0D);
	len = gSafeSqrtD(len);
	ratio = sc1F / len;
	vx(v_out) = vx(v) * ratio;
	vy(v_out) = vy(v) * ratio;
	return len;
}

CDRAW_INL vecd_t vecResize2d(double2_t v_out, double2_t const v, vecd_t const newLen)
{
	failassert(v_out && v, sc0D);
	vecd_t len = vecLenSq2d(v), ratio;
	if (scIsNonPositiveApproxD(len))
		return (vecZero2d(v_out), sc0D);
	len = gSafeSqrtD(len);
	ratio = newLen / len;
	vx(v_out) = vx(v) * ratio;
	vy(v_out) = vy(v) * ratio;
	return len;
}

CDRAW_INL vecb_t vecIsUnit2d(double2_t const v)
{
	failassert(v, false);
	vecd_t const lenSq = vecLenSq2d(v);
	return ((lenSq >= scEpsL1F) && (lenSq <= scEpsG1F));
}

CDRAW_INL vecb_t vecIsNonUnit2d(double2_t const v)
{
	failassert(v, true);
	vecd_t const lenSq = vecLenSq2d(v);
	return ((lenSq < scEpsL1F) || (lenSq > scEpsG1F));
}

CDRAW_INL vecd_t vecProjS2d(double2_t const v, double2_t const v_base)
{
	failassert(v && v_base, sc0D);
	vecd_t const lenSq = vecLenSq2d(v_base);
	if (scIsNonPositiveApproxD(lenSq))
		return sc0D;
	return (vecDot2d(v, v_base) / lenSq);
}

CDRAW_INL vecd_t vecProj2d(double2_t v_out, double2_t const v, double2_t const v_base)
{
	failassert(v_out && v && v_base, sc0D);
	vecd_t ratio = vecLenSq2d(v_base);
	if (scIsNonPositiveApproxD(ratio))
		return sc0D;
	ratio = vecDot2d(v, v_base) / ratio;
	vx(v_out) = vx(v_base) * ratio;
	vy(v_out) = vy(v_base) * ratio;
	return ratio;
}

CDRAW_INL vecd_t vecLerpInv2d(double2_t const v, double2_t const v_min, double2_t const v_max)
{
	failassert(v && v_min && v_max, sc0D);
	double2_t v_delta;
	vecd_t const distSq = vecDispDistSq2d(v_delta, v_max, v_min);
	if (scIsNonPositiveApproxD(distSq))
		return sc0D;
	return ((vx(v) - vx(v_min)) * vx(v_delta) + (vy(v) - vy(v_min)) * vy(v_delta)) / distSq;
}

CDRAW_INL vecd_t vecOrtho2d(double2_t v_out, double2_t const v, double2_t const v_base)
{
	failassert(v_out && v && v_base, sc0D);
	vecd_t ratio = vecLenSq2d(v_base);
	if (scIsNonPositiveApproxD(ratio))
		return (vecZero2d(v_out), sc0D);
	ratio = vecDot2d(v, v_base) / ratio;
	vx(v_out) = (vx(v) - vx(v_base) * ratio);
	vy(v_out) = (vy(v) - vy(v_base) * ratio);
	return ratio;
}


#endif // #if (!(defined _CDRAW_VEC2D_INL_) && (defined _CDRAW_VECTOR_INL_))