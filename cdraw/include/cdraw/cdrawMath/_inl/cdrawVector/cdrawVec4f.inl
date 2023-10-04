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
* cdrawVec4f.inl
* Definitions of single-precision 4D vector functions.
*/

#if (!(defined _CDRAW_VEC4F_INL_) && (defined _CDRAW_VECTOR_INL_))
#define _CDRAW_VEC4F_INL_


CDRAW_INL floatN_t vecZero4f(float4_t v_out)
{
	failassert(v_out, NULL);
	vx(v_out) = 0;
	vy(v_out) = 0;
	vz(v_out) = 0;
	vw(v_out) = 0;
	return v_out;
}

CDRAW_INL floatN_t vecInit4f(float4_t v_out, vecf_t const x, vecf_t const y, vecf_t const z, vecf_t const w)
{
	failassert(v_out, NULL);
	vx(v_out) = x;
	vy(v_out) = y;
	vz(v_out) = z;
	vw(v_out) = w;
	return v_out;
}

CDRAW_INL floatN_t vecInitS4f(float4_t v_out, vecf_t const x)
{
	failassert(v_out, NULL);
	vx(v_out) = x;
	vy(v_out) = x;
	vz(v_out) = x;
	vw(v_out) = x;
	return v_out;
}

CDRAW_INL floatN_t vecCopy2zw4f(float4_t v_out, float2_t const xy, vecf_t const z, vecf_t const w)
{
	failassert(v_out && xy, NULL);
	vx(v_out) = vx(xy);
	vy(v_out) = vy(xy);
	vz(v_out) = z;
	vw(v_out) = w;
	return v_out;
}

CDRAW_INL floatN_t vecCopy3w4f(float4_t v_out, float3_t const xyz, vecf_t const w)
{
	failassert(v_out && xyz, NULL);
	vx(v_out) = vx(xyz);
	vy(v_out) = vy(xyz);
	vz(v_out) = vz(xyz);
	vw(v_out) = w;
	return v_out;
}

CDRAW_INL floatN_t vecCopy4f(float4_t v_out, float4_t const v)
{
	failassert(v_out && v, NULL);
	vx(v_out) = vx(v);
	vy(v_out) = vy(v);
	vz(v_out) = vz(v);
	vw(v_out) = vw(v);
	return v_out;
}

CDRAW_INL floatN_t vecCopy4b4f(float4_t v_out, bool4_t const v)
{
	failassert(v_out && v, NULL);
	vx(v_out) = (vx(v) ? 1.0f : 0.0f);
	vy(v_out) = (vy(v) ? 1.0f : 0.0f);
	vz(v_out) = (vz(v) ? 1.0f : 0.0f);
	vw(v_out) = (vw(v) ? 1.0f : 0.0f);
	return v_out;
}

CDRAW_INL floatN_t vecCopy4i4f(float4_t v_out, int4_t const v)
{
	failassert(v_out && v, NULL);
	vx(v_out) = (vecf_t)(vx(v));
	vy(v_out) = (vecf_t)(vy(v));
	vz(v_out) = (vecf_t)(vz(v));
	vw(v_out) = (vecf_t)(vw(v));
	return v_out;
}

CDRAW_INL floatN_t vecCopy4u4f(float4_t v_out, uint4_t const v)
{
	failassert(v_out && v, NULL);
	vx(v_out) = (vecf_t)(vx(v));
	vy(v_out) = (vecf_t)(vy(v));
	vz(v_out) = (vecf_t)(vz(v));
	vw(v_out) = (vecf_t)(vw(v));
	return v_out;
}

CDRAW_INL floatN_t vecCopy4d4f(float4_t v_out, double4_t const v)
{
	failassert(v_out && v, NULL);
	vx(v_out) = (vecf_t)(vx(v));
	vy(v_out) = (vecf_t)(vy(v));
	vz(v_out) = (vecf_t)(vz(v));
	vw(v_out) = (vecf_t)(vw(v));
	return v_out;
}

CDRAW_INL boolN_t vecEq4f(bool4_t v_out, float4_t const v_lh, float4_t const v_rh)
{
	failassert(v_out && v_lh && v_rh, NULL);
	vx(v_out) = (vx(v_lh) == vx(v_rh));
	vy(v_out) = (vy(v_lh) == vy(v_rh));
	vz(v_out) = (vz(v_lh) == vz(v_rh));
	vw(v_out) = (vw(v_lh) == vw(v_rh));
	return v_out;
}

CDRAW_INL boolN_t vecNeq4f(bool4_t v_out, float4_t const v_lh, float4_t const v_rh)
{
	failassert(v_out && v_lh && v_rh, NULL);
	vx(v_out) = (vx(v_lh) != vx(v_rh));
	vy(v_out) = (vy(v_lh) != vy(v_rh));
	vz(v_out) = (vz(v_lh) != vz(v_rh));
	vw(v_out) = (vw(v_lh) != vw(v_rh));
	return v_out;
}

CDRAW_INL boolN_t vecGreater4f(bool4_t v_out, float4_t const v_lh, float4_t const v_rh)
{
	failassert(v_out && v_lh && v_rh, NULL);
	vx(v_out) = (vx(v_lh) > vx(v_rh));
	vy(v_out) = (vy(v_lh) > vy(v_rh));
	vz(v_out) = (vz(v_lh) > vz(v_rh));
	vw(v_out) = (vw(v_lh) > vw(v_rh));
	return v_out;
}

CDRAW_INL boolN_t vecGreaterEq4f(bool4_t v_out, float4_t const v_lh, float4_t const v_rh)
{
	failassert(v_out && v_lh && v_rh, NULL);
	vx(v_out) = (vx(v_lh) >= vx(v_rh));
	vy(v_out) = (vy(v_lh) >= vy(v_rh));
	vz(v_out) = (vz(v_lh) >= vz(v_rh));
	vw(v_out) = (vw(v_lh) >= vw(v_rh));
	return v_out;
}

CDRAW_INL boolN_t vecLess4f(bool4_t v_out, float4_t const v_lh, float4_t const v_rh)
{
	failassert(v_out && v_lh && v_rh, NULL);
	vx(v_out) = (vx(v_lh) < vx(v_rh));
	vy(v_out) = (vy(v_lh) < vy(v_rh));
	vz(v_out) = (vz(v_lh) < vz(v_rh));
	vw(v_out) = (vw(v_lh) < vw(v_rh));
	return v_out;
}

CDRAW_INL boolN_t vecLessEq4f(bool4_t v_out, float4_t const v_lh, float4_t const v_rh)
{
	failassert(v_out && v_lh && v_rh, NULL);
	vx(v_out) = (vx(v_lh) <= vx(v_rh));
	vy(v_out) = (vy(v_lh) <= vy(v_rh));
	vz(v_out) = (vz(v_lh) <= vz(v_rh));
	vw(v_out) = (vw(v_lh) <= vw(v_rh));
	return v_out;
}

CDRAW_INL floatN_t vecNegate4f(float4_t v_out, float4_t const v)
{
	failassert(v_out && v, NULL);
	vx(v_out) = -vx(v);
	vy(v_out) = -vy(v);
	vz(v_out) = -vz(v);
	vw(v_out) = -vw(v);
	return v_out;
}

CDRAW_INL floatN_t vecAdd4f(float4_t v_out, float4_t const v_lh, float4_t const v_rh)
{
	failassert(v_out && v_lh && v_rh, NULL);
	vx(v_out) = vx(v_lh) + vx(v_rh);
	vy(v_out) = vy(v_lh) + vy(v_rh);
	vz(v_out) = vz(v_lh) + vz(v_rh);
	vw(v_out) = vw(v_lh) + vw(v_rh);
	return v_out;
}

CDRAW_INL floatN_t vecSub4f(float4_t v_out, float4_t const v_lh, float4_t const v_rh)
{
	failassert(v_out && v_lh && v_rh, NULL);
	vx(v_out) = vx(v_lh) - vx(v_rh);
	vy(v_out) = vy(v_lh) - vy(v_rh);
	vz(v_out) = vz(v_lh) - vz(v_rh);
	vw(v_out) = vw(v_lh) - vw(v_rh);
	return v_out;
}

CDRAW_INL floatN_t vecMul4f(float4_t v_out, float4_t const v_lh, float4_t const v_rh)
{
	failassert(v_out && v_lh && v_rh, NULL);
	vx(v_out) = vx(v_lh) * vx(v_rh);
	vy(v_out) = vy(v_lh) * vy(v_rh);
	vz(v_out) = vz(v_lh) * vz(v_rh);
	vw(v_out) = vw(v_lh) * vw(v_rh);
	return v_out;
}

CDRAW_INL floatN_t vecDiv4f(float4_t v_out, float4_t const v_lh, float4_t const v_rh)
{
	failassert(v_out && v_lh && v_rh, NULL);
	failassert(scIsNonZeroApproxF(vx(v_rh)) && scIsNonZeroApproxF(vy(v_rh)) && scIsNonZeroApproxF(vz(v_rh)) && scIsNonZeroApproxF(vw(v_rh)), vecInit4f(v_out,
		gDivSafeF(vx(v_lh), vx(v_rh)),
		gDivSafeF(vy(v_lh), vy(v_rh)),
		gDivSafeF(vz(v_lh), vz(v_rh)),
		gDivSafeF(vw(v_lh), vw(v_rh))));
	vx(v_out) = gDiv(vx(v_lh), vx(v_rh));
	vy(v_out) = gDiv(vy(v_lh), vy(v_rh));
	vz(v_out) = gDiv(vz(v_lh), vz(v_rh));
	vw(v_out) = gDiv(vw(v_lh), vw(v_rh));
	return v_out;
}

CDRAW_INL floatN_t vecMod4f(float4_t v_out, float4_t const v_lh, float4_t const v_rh)
{
	failassert(v_out && v_lh && v_rh, NULL);
	failassert(scIsNonZeroApproxF(vx(v_rh)) && scIsNonZeroApproxF(vy(v_rh)) && scIsNonZeroApproxF(vz(v_rh)) && scIsNonZeroApproxF(vw(v_rh)), vecInit4f(v_out,
		gModSafeF(vx(v_lh), vx(v_rh)),
		gModSafeF(vy(v_lh), vy(v_rh)),
		gModSafeF(vz(v_lh), vz(v_rh)),
		gModSafeF(vw(v_lh), vw(v_rh))));
	vx(v_out) = gModF(vx(v_lh), vx(v_rh));
	vy(v_out) = gModF(vy(v_lh), vy(v_rh));
	vz(v_out) = gModF(vz(v_lh), vz(v_rh));
	vw(v_out) = gModF(vw(v_lh), vw(v_rh));
	return v_out;
}

CDRAW_INL floatN_t vecDivMod4f(float4_t v_out, float4_t v_mod_out, float4_t const v_lh, float4_t const v_rh)
{
	failassert(v_out && v_mod_out && v_lh && v_rh, NULL);
	failassert(scIsNonZeroApproxF(vx(v_rh)) && scIsNonZeroApproxF(vy(v_rh)) && scIsNonZeroApproxF(vz(v_rh)) && scIsNonZeroApproxF(vw(v_rh)), vecInit4f(v_out,
		gDivModSafeF(vx(v_mod_out), vx(v_lh), vx(v_rh)),
		gDivModSafeF(vy(v_mod_out), vy(v_lh), vy(v_rh)),
		gDivModSafeF(vz(v_mod_out), vz(v_lh), vz(v_rh)),
		gDivModSafeF(vw(v_mod_out), vw(v_lh), vw(v_rh))));
	vx(v_out) = gDiv(vx(v_lh), vx(v_rh));
	vy(v_out) = gDiv(vy(v_lh), vy(v_rh));
	vz(v_out) = gDiv(vz(v_lh), vz(v_rh));
	vw(v_out) = gDiv(vw(v_lh), vw(v_rh));
	vx(v_mod_out) = gModQF(vx(v_lh), vx(v_rh), vx(v_out));
	vy(v_mod_out) = gModQF(vy(v_lh), vy(v_rh), vy(v_out));
	vz(v_mod_out) = gModQF(vz(v_lh), vz(v_rh), vz(v_out));
	vw(v_mod_out) = gModQF(vw(v_lh), vw(v_rh), vw(v_out));
	return v_out;
}

CDRAW_INL floatN_t vecAddS4f(float4_t v_out, float4_t const v_lh, vecf_t const s_rh)
{
	failassert(v_out && v_lh, NULL);
	vx(v_out) = vx(v_lh) + s_rh;
	vy(v_out) = vy(v_lh) + s_rh;
	vz(v_out) = vz(v_lh) + s_rh;
	vw(v_out) = vw(v_lh) + s_rh;
	return v_out;
}

CDRAW_INL floatN_t vecSubS4f(float4_t v_out, float4_t const v_lh, vecf_t const s_rh)
{
	failassert(v_out && v_lh, NULL);
	vx(v_out) = vx(v_lh) - s_rh;
	vy(v_out) = vy(v_lh) - s_rh;
	vz(v_out) = vz(v_lh) - s_rh;
	vw(v_out) = vw(v_lh) - s_rh;
	return v_out;
}

CDRAW_INL floatN_t vecMulS4f(float4_t v_out, float4_t const v_lh, vecf_t const s_rh)
{
	failassert(v_out && v_lh, NULL);
	vx(v_out) = vx(v_lh) * s_rh;
	vy(v_out) = vy(v_lh) * s_rh;
	vz(v_out) = vz(v_lh) * s_rh;
	vw(v_out) = vw(v_lh) * s_rh;
	return v_out;
}

CDRAW_INL floatN_t vecDivS4f(float4_t v_out, float4_t const v_lh, vecf_t const s_rh)
{
	failassert(v_out && v_lh, NULL);
	failassert(scIsNonZeroApproxF(s_rh), vecZero4f(v_out));
	vecf_t const recip = sc1F / s_rh;
	vx(v_out) = vx(v_lh) * recip;
	vy(v_out) = vy(v_lh) * recip;
	vz(v_out) = vz(v_lh) * recip;
	vw(v_out) = vw(v_lh) * recip;
	return v_out;
}

CDRAW_INL floatN_t vecModS4f(float4_t v_out, float4_t const v_lh, vecf_t const s_rh)
{
	failassert(v_out && v_lh, NULL);
	failassert(scIsNonZeroApproxF(s_rh), vecCopy4f(v_out, v_lh));
	vecf_t const recip = sc1F / s_rh;
	vx(v_out) = gModQF(vx(v_lh), s_rh, vx(v_lh) * recip);
	vy(v_out) = gModQF(vy(v_lh), s_rh, vy(v_lh) * recip);
	vz(v_out) = gModQF(vz(v_lh), s_rh, vz(v_lh) * recip);
	vw(v_out) = gModQF(vw(v_lh), s_rh, vw(v_lh) * recip);
	return v_out;
}

CDRAW_INL floatN_t vecDivModS4f(float4_t v_out, float4_t v_mod_out, float4_t const v_lh, vecf_t const s_rh)
{
	failassert(v_out && v_mod_out && v_lh, NULL);
	failassert(scIsNonZeroApproxF(s_rh), vecCopy4f(v_mod_out, v_lh), vecZero4f(v_out));
	vecf_t const recip = sc1F / s_rh;
	vx(v_out) = vx(v_lh) * recip;
	vy(v_out) = vy(v_lh) * recip;
	vz(v_out) = vz(v_lh) * recip;
	vw(v_out) = vw(v_lh) * recip;
	vx(v_mod_out) = gModQF(vx(v_lh), s_rh, vx(v_out));
	vy(v_mod_out) = gModQF(vy(v_lh), s_rh, vy(v_out));
	vz(v_mod_out) = gModQF(vz(v_lh), s_rh, vz(v_out));
	vw(v_mod_out) = gModQF(vw(v_lh), s_rh, vw(v_out));
	return v_out;
}

CDRAW_INL vecf_t vecDot4f(float4_t const v_lh, float4_t const v_rh)
{
	failassert(v_lh && v_rh, 0);
	return (vx(v_lh) * vx(v_rh) + vy(v_lh) * vy(v_rh) + vz(v_lh) * vz(v_rh) + vw(v_lh) * vw(v_rh));
}

CDRAW_INL floatN_t vecCross4f(float4_t v_out, float4_t const v_lh, float4_t const v_rh)
{
	failassert(v_out && v_lh && v_rh, NULL);
	vx(v_out) = (vy(v_lh) * vz(v_rh) - vz(v_lh) * vy(v_rh));
	vy(v_out) = (vz(v_lh) * vx(v_rh) - vx(v_lh) * vz(v_rh));
	vz(v_out) = (vx(v_lh) * vy(v_rh) - vy(v_lh) * vx(v_rh));
	vw(v_out) = 0;
	return v_out;
}

CDRAW_INL vecf_t vecLenSq4f(float4_t const v)
{
	failassert(v, 0);
	return (gSq(vx(v)) + gSq(vy(v)) + gSq(vz(v)) + gSq(vw(v)));
}

CDRAW_INL vecf_t vecDistSq4f(float4_t const v_lh, float4_t const v_rh)
{
	failassert(v_lh && v_rh, 0);
	float4_t const disp = {
		(vx(v_lh) - vx(v_rh)),
		(vy(v_lh) - vy(v_rh)),
		(vz(v_lh) - vz(v_rh)),
		(vw(v_lh) - vw(v_rh)),
	};
	return (gSq(vx(disp)) + gSq(vy(disp)) + gSq(vz(disp)) + gSq(vw(disp)));
}

CDRAW_INL vecf_t vecDispDistSq4f(float4_t v_disp_out, float4_t const v_lh, float4_t const v_rh)
{
	failassert(v_disp_out && v_lh && v_rh, 0);
	vx(v_disp_out) = (vx(v_lh) - vx(v_rh));
	vy(v_disp_out) = (vy(v_lh) - vy(v_rh));
	vz(v_disp_out) = (vz(v_lh) - vz(v_rh));
	vw(v_disp_out) = (vw(v_lh) - vw(v_rh));
	return (gSq(vx(v_disp_out)) + gSq(vy(v_disp_out)) + gSq(vz(v_disp_out)) + gSq(vw(v_disp_out)));
}

CDRAW_INL vecb_t vecIsZero4f(float4_t const v)
{
	return ((vx(v) == 0) && (vy(v) == 0) && (vz(v) == 0) && (vw(v) == 0));
}

CDRAW_INL vecb_t vecIsNonZero4f(float4_t const v)
{
	return ((vx(v) != 0) || (vy(v) != 0) || (vz(v) != 0) || (vw(v) != 0));
}

CDRAW_INL floatN_t vecMad4f(float4_t v_out, vecf_t const u, float4_t const v_origin, float4_t const v_delta)
{
	failassert(v_out && v_origin && v_delta, NULL);
	vx(v_out) = gMad(vx(v_origin), vx(v_delta), u);
	vy(v_out) = gMad(vy(v_origin), vy(v_delta), u);
	vz(v_out) = gMad(vz(v_origin), vz(v_delta), u);
	vw(v_out) = gMad(vw(v_origin), vw(v_delta), u);
	return v_out;
}

CDRAW_INL floatN_t vecLerp4f(float4_t v_out, vecf_t const u, float4_t const v_min, float4_t const v_max)
{
	failassert(v_out && v_min && v_max, NULL);
	vx(v_out) = gLerp(vx(v_min), vx(v_max), u);
	vy(v_out) = gLerp(vy(v_min), vy(v_max), u);
	vz(v_out) = gLerp(vz(v_min), vz(v_max), u);
	vw(v_out) = gLerp(vw(v_min), vw(v_max), u);
	return v_out;
}


#endif // #if (!(defined _CDRAW_VEC4F_INL_) && (defined _CDRAW_VECTOR_INL_))