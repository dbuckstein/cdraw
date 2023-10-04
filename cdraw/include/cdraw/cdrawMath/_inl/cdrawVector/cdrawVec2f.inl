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
	vx(v_out) = 0;
	vy(v_out) = 0;
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
	return (gSq(vx(disp)) + gSq(vy(disp)));
}

CDRAW_INL vecf_t vecDispDistSq2f(float2_t v_disp_out, float2_t const v_lh, float2_t const v_rh)
{
	failassert(v_disp_out && v_lh && v_rh, 0);
	vx(v_disp_out) = (vx(v_lh) - vx(v_rh));
	vy(v_disp_out) = (vy(v_lh) - vy(v_rh));
	return (gSq(vx(v_disp_out)) + gSq(vy(v_disp_out)));
}

CDRAW_INL vecb_t vecIsZero2f(float2_t const v)
{
	return ((vx(v) == 0) && (vy(v) == 0));
}

CDRAW_INL vecb_t vecIsNonZero2f(float2_t const v)
{
	return ((vx(v) != 0) || (vy(v) != 0));
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


#endif // #if (!(defined _CDRAW_VEC2F_INL_) && (defined _CDRAW_VECTOR_INL_))