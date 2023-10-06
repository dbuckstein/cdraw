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
* cdrawVec4d.inl
* Definitions of double-precision 4D vector functions.
*/

#if (!(defined _CDRAW_VEC4D_INL_) && (defined _CDRAW_VECTOR_INL_))
#define _CDRAW_VEC4D_INL_


CDRAW_INL doubleN_t vecZero4d(double4_t v_out)
{
	failassert(v_out, NULL);
	vx(v_out) = 0.0;
	vy(v_out) = 0.0;
	vz(v_out) = 0.0;
	vw(v_out) = 0.0;
	return v_out;
}

CDRAW_INL doubleN_t vecInit4d(double4_t v_out, vecd_t const x, vecd_t const y, vecd_t const z, vecd_t const w)
{
	failassert(v_out, NULL);
	vx(v_out) = x;
	vy(v_out) = y;
	vz(v_out) = z;
	vw(v_out) = w;
	return v_out;
}

CDRAW_INL doubleN_t vecInitS4d(double4_t v_out, vecd_t const x)
{
	failassert(v_out, NULL);
	vx(v_out) = x;
	vy(v_out) = x;
	vz(v_out) = x;
	vw(v_out) = x;
	return v_out;
}

CDRAW_INL doubleN_t vecCopy2zw4d(double4_t v_out, double2_t const xy, vecd_t const z, vecd_t const w)
{
	failassert(v_out && xy, NULL);
	vx(v_out) = vx(xy);
	vy(v_out) = vy(xy);
	vz(v_out) = z;
	vw(v_out) = w;
	return v_out;
}

CDRAW_INL doubleN_t vecCopy3w4d(double4_t v_out, double3_t const xyz, vecd_t const w)
{
	failassert(v_out && xyz, NULL);
	vx(v_out) = vx(xyz);
	vy(v_out) = vy(xyz);
	vz(v_out) = vz(xyz);
	vw(v_out) = w;
	return v_out;
}

CDRAW_INL doubleN_t vecCopy4d(double4_t v_out, double4_t const v)
{
	failassert(v_out && v, NULL);
	vx(v_out) = vx(v);
	vy(v_out) = vy(v);
	vz(v_out) = vz(v);
	vw(v_out) = vw(v);
	return v_out;
}

CDRAW_INL doubleN_t vecCopy4b4d(double4_t v_out, bool4_t const v)
{
	failassert(v_out && v, NULL);
	vx(v_out) = (vx(v) ? 1.0 : 0.0);
	vy(v_out) = (vy(v) ? 1.0 : 0.0);
	vz(v_out) = (vz(v) ? 1.0 : 0.0);
	vw(v_out) = (vw(v) ? 1.0 : 0.0);
	return v_out;
}

CDRAW_INL doubleN_t vecCopy4i4d(double4_t v_out, int4_t const v)
{
	failassert(v_out && v, NULL);
	vx(v_out) = (vecd_t)(vx(v));
	vy(v_out) = (vecd_t)(vy(v));
	vz(v_out) = (vecd_t)(vz(v));
	vw(v_out) = (vecd_t)(vw(v));
	return v_out;
}

CDRAW_INL doubleN_t vecCopy4u4d(double4_t v_out, uint4_t const v)
{
	failassert(v_out && v, NULL);
	vx(v_out) = (vecd_t)(vx(v));
	vy(v_out) = (vecd_t)(vy(v));
	vz(v_out) = (vecd_t)(vz(v));
	vw(v_out) = (vecd_t)(vw(v));
	return v_out;
}

CDRAW_INL doubleN_t vecCopy4f4d(double4_t v_out, float4_t const v)
{
	failassert(v_out && v, NULL);
	vx(v_out) = (vecd_t)(vx(v));
	vy(v_out) = (vecd_t)(vy(v));
	vz(v_out) = (vecd_t)(vz(v));
	vw(v_out) = (vecd_t)(vw(v));
	return v_out;
}

CDRAW_INL boolN_t vecEq4d(bool4_t v_out, double4_t const v_lh, double4_t const v_rh)
{
	failassert(v_out && v_lh && v_rh, NULL);
	vx(v_out) = (vx(v_lh) == vx(v_rh));
	vy(v_out) = (vy(v_lh) == vy(v_rh));
	vz(v_out) = (vz(v_lh) == vz(v_rh));
	vw(v_out) = (vw(v_lh) == vw(v_rh));
	return v_out;
}

CDRAW_INL boolN_t vecNeq4d(bool4_t v_out, double4_t const v_lh, double4_t const v_rh)
{
	failassert(v_out && v_lh && v_rh, NULL);
	vx(v_out) = (vx(v_lh) != vx(v_rh));
	vy(v_out) = (vy(v_lh) != vy(v_rh));
	vz(v_out) = (vz(v_lh) != vz(v_rh));
	vw(v_out) = (vw(v_lh) != vw(v_rh));
	return v_out;
}

CDRAW_INL boolN_t vecGreater4d(bool4_t v_out, double4_t const v_lh, double4_t const v_rh)
{
	failassert(v_out && v_lh && v_rh, NULL);
	vx(v_out) = (vx(v_lh) > vx(v_rh));
	vy(v_out) = (vy(v_lh) > vy(v_rh));
	vz(v_out) = (vz(v_lh) > vz(v_rh));
	vw(v_out) = (vw(v_lh) > vw(v_rh));
	return v_out;
}

CDRAW_INL boolN_t vecGreaterEq4d(bool4_t v_out, double4_t const v_lh, double4_t const v_rh)
{
	failassert(v_out && v_lh && v_rh, NULL);
	vx(v_out) = (vx(v_lh) >= vx(v_rh));
	vy(v_out) = (vy(v_lh) >= vy(v_rh));
	vz(v_out) = (vz(v_lh) >= vz(v_rh));
	vw(v_out) = (vw(v_lh) >= vw(v_rh));
	return v_out;
}

CDRAW_INL boolN_t vecLess4d(bool4_t v_out, double4_t const v_lh, double4_t const v_rh)
{
	failassert(v_out && v_lh && v_rh, NULL);
	vx(v_out) = (vx(v_lh) < vx(v_rh));
	vy(v_out) = (vy(v_lh) < vy(v_rh));
	vz(v_out) = (vz(v_lh) < vz(v_rh));
	vw(v_out) = (vw(v_lh) < vw(v_rh));
	return v_out;
}

CDRAW_INL boolN_t vecLessEq4d(bool4_t v_out, double4_t const v_lh, double4_t const v_rh)
{
	failassert(v_out && v_lh && v_rh, NULL);
	vx(v_out) = (vx(v_lh) <= vx(v_rh));
	vy(v_out) = (vy(v_lh) <= vy(v_rh));
	vz(v_out) = (vz(v_lh) <= vz(v_rh));
	vw(v_out) = (vw(v_lh) <= vw(v_rh));
	return v_out;
}

CDRAW_INL doubleN_t vecNegate4d(double4_t v_out, double4_t const v)
{
	failassert(v_out && v, NULL);
	vx(v_out) = -vx(v);
	vy(v_out) = -vy(v);
	vz(v_out) = -vz(v);
	vw(v_out) = -vw(v);
	return v_out;
}

CDRAW_INL doubleN_t vecAdd4d(double4_t v_out, double4_t const v_lh, double4_t const v_rh)
{
	failassert(v_out && v_lh && v_rh, NULL);
	vx(v_out) = vx(v_lh) + vx(v_rh);
	vy(v_out) = vy(v_lh) + vy(v_rh);
	vz(v_out) = vz(v_lh) + vz(v_rh);
	vw(v_out) = vw(v_lh) + vw(v_rh);
	return v_out;
}

CDRAW_INL doubleN_t vecSub4d(double4_t v_out, double4_t const v_lh, double4_t const v_rh)
{
	failassert(v_out && v_lh && v_rh, NULL);
	vx(v_out) = vx(v_lh) - vx(v_rh);
	vy(v_out) = vy(v_lh) - vy(v_rh);
	vz(v_out) = vz(v_lh) - vz(v_rh);
	vw(v_out) = vw(v_lh) - vw(v_rh);
	return v_out;
}

CDRAW_INL doubleN_t vecMul4d(double4_t v_out, double4_t const v_lh, double4_t const v_rh)
{
	failassert(v_out && v_lh && v_rh, NULL);
	vx(v_out) = vx(v_lh) * vx(v_rh);
	vy(v_out) = vy(v_lh) * vy(v_rh);
	vz(v_out) = vz(v_lh) * vz(v_rh);
	vw(v_out) = vw(v_lh) * vw(v_rh);
	return v_out;
}

CDRAW_INL doubleN_t vecDiv4d(double4_t v_out, double4_t const v_lh, double4_t const v_rh)
{
	failassert(v_out && v_lh && v_rh, NULL);
	failassert(scIsNonZeroApproxD(vx(v_rh)) && scIsNonZeroApproxD(vy(v_rh)) && scIsNonZeroApproxD(vz(v_rh)) && scIsNonZeroApproxD(vw(v_rh)), vecInit4d(v_out,
		gDivSafeD(vx(v_lh), vx(v_rh)),
		gDivSafeD(vy(v_lh), vy(v_rh)),
		gDivSafeD(vz(v_lh), vz(v_rh)),
		gDivSafeD(vw(v_lh), vw(v_rh))));
	vx(v_out) = gDiv(vx(v_lh), vx(v_rh));
	vy(v_out) = gDiv(vy(v_lh), vy(v_rh));
	vz(v_out) = gDiv(vz(v_lh), vz(v_rh));
	vw(v_out) = gDiv(vw(v_lh), vw(v_rh));
	return v_out;
}

CDRAW_INL doubleN_t vecMod4d(double4_t v_out, double4_t const v_lh, double4_t const v_rh)
{
	failassert(v_out && v_lh && v_rh, NULL);
	failassert(scIsNonZeroApproxD(vx(v_rh)) && scIsNonZeroApproxD(vy(v_rh)) && scIsNonZeroApproxD(vz(v_rh)) && scIsNonZeroApproxD(vw(v_rh)), vecInit4d(v_out,
		gModSafeD(vx(v_lh), vx(v_rh)),
		gModSafeD(vy(v_lh), vy(v_rh)),
		gModSafeD(vz(v_lh), vz(v_rh)),
		gModSafeD(vw(v_lh), vw(v_rh))));
	vx(v_out) = gModD(vx(v_lh), vx(v_rh));
	vy(v_out) = gModD(vy(v_lh), vy(v_rh));
	vz(v_out) = gModD(vz(v_lh), vz(v_rh));
	vw(v_out) = gModD(vw(v_lh), vw(v_rh));
	return v_out;
}

CDRAW_INL doubleN_t vecDivMod4d(double4_t v_out, double4_t v_mod_out, double4_t const v_lh, double4_t const v_rh)
{
	failassert(v_out && v_mod_out && v_lh && v_rh, NULL);
	failassert(scIsNonZeroApproxD(vx(v_rh)) && scIsNonZeroApproxD(vy(v_rh)) && scIsNonZeroApproxD(vz(v_rh)) && scIsNonZeroApproxD(vw(v_rh)), vecInit4d(v_out,
		gDivModSafeD(vx(v_mod_out), vx(v_lh), vx(v_rh)),
		gDivModSafeD(vy(v_mod_out), vy(v_lh), vy(v_rh)),
		gDivModSafeD(vz(v_mod_out), vz(v_lh), vz(v_rh)),
		gDivModSafeD(vw(v_mod_out), vw(v_lh), vw(v_rh))));
	vx(v_out) = gDiv(vx(v_lh), vx(v_rh));
	vy(v_out) = gDiv(vy(v_lh), vy(v_rh));
	vz(v_out) = gDiv(vz(v_lh), vz(v_rh));
	vw(v_out) = gDiv(vw(v_lh), vw(v_rh));
	vx(v_mod_out) = gModQD(vx(v_lh), vx(v_rh), vx(v_out));
	vy(v_mod_out) = gModQD(vy(v_lh), vy(v_rh), vy(v_out));
	vz(v_mod_out) = gModQD(vz(v_lh), vz(v_rh), vz(v_out));
	vw(v_mod_out) = gModQD(vw(v_lh), vw(v_rh), vw(v_out));
	return v_out;
}

CDRAW_INL doubleN_t vecAddS4d(double4_t v_out, double4_t const v_lh, vecd_t const s_rh)
{
	failassert(v_out && v_lh, NULL);
	vx(v_out) = vx(v_lh) + s_rh;
	vy(v_out) = vy(v_lh) + s_rh;
	vz(v_out) = vz(v_lh) + s_rh;
	vw(v_out) = vw(v_lh) + s_rh;
	return v_out;
}

CDRAW_INL doubleN_t vecSubS4d(double4_t v_out, double4_t const v_lh, vecd_t const s_rh)
{
	failassert(v_out && v_lh, NULL);
	vx(v_out) = vx(v_lh) - s_rh;
	vy(v_out) = vy(v_lh) - s_rh;
	vz(v_out) = vz(v_lh) - s_rh;
	vw(v_out) = vw(v_lh) - s_rh;
	return v_out;
}

CDRAW_INL doubleN_t vecMulS4d(double4_t v_out, double4_t const v_lh, vecd_t const s_rh)
{
	failassert(v_out && v_lh, NULL);
	vx(v_out) = vx(v_lh) * s_rh;
	vy(v_out) = vy(v_lh) * s_rh;
	vz(v_out) = vz(v_lh) * s_rh;
	vw(v_out) = vw(v_lh) * s_rh;
	return v_out;
}

CDRAW_INL doubleN_t vecDivS4d(double4_t v_out, double4_t const v_lh, vecd_t const s_rh)
{
	failassert(v_out && v_lh, NULL);
	failassert(scIsNonZeroApproxD(s_rh), vecZero4d(v_out));
	vecd_t const recip = sc1F / s_rh;
	vx(v_out) = vx(v_lh) * recip;
	vy(v_out) = vy(v_lh) * recip;
	vz(v_out) = vz(v_lh) * recip;
	vw(v_out) = vw(v_lh) * recip;
	return v_out;
}

CDRAW_INL doubleN_t vecModS4d(double4_t v_out, double4_t const v_lh, vecd_t const s_rh)
{
	failassert(v_out && v_lh, NULL);
	failassert(scIsNonZeroApproxD(s_rh), vecCopy4d(v_out, v_lh));
	vecd_t const recip = sc1F / s_rh;
	vx(v_out) = gModQD(vx(v_lh), s_rh, vx(v_lh) * recip);
	vy(v_out) = gModQD(vy(v_lh), s_rh, vy(v_lh) * recip);
	vz(v_out) = gModQD(vz(v_lh), s_rh, vz(v_lh) * recip);
	vw(v_out) = gModQD(vw(v_lh), s_rh, vw(v_lh) * recip);
	return v_out;
}

CDRAW_INL doubleN_t vecDivModS4d(double4_t v_out, double4_t v_mod_out, double4_t const v_lh, vecd_t const s_rh)
{
	failassert(v_out && v_mod_out && v_lh, NULL);
	failassert(scIsNonZeroApproxD(s_rh), vecCopy4d(v_mod_out, v_lh), vecZero4d(v_out));
	vecd_t const recip = sc1F / s_rh;
	vx(v_out) = vx(v_lh) * recip;
	vy(v_out) = vy(v_lh) * recip;
	vz(v_out) = vz(v_lh) * recip;
	vw(v_out) = vw(v_lh) * recip;
	vx(v_mod_out) = gModQD(vx(v_lh), s_rh, vx(v_out));
	vy(v_mod_out) = gModQD(vy(v_lh), s_rh, vy(v_out));
	vz(v_mod_out) = gModQD(vz(v_lh), s_rh, vz(v_out));
	vw(v_mod_out) = gModQD(vw(v_lh), s_rh, vw(v_out));
	return v_out;
}

CDRAW_INL vecd_t vecDot4d(double4_t const v_lh, double4_t const v_rh)
{
	failassert(v_lh && v_rh, 0);
	return (vx(v_lh) * vx(v_rh) + vy(v_lh) * vy(v_rh) + vz(v_lh) * vz(v_rh) + vw(v_lh) * vw(v_rh));
}

CDRAW_INL doubleN_t vecCross4d(double4_t v_out, double4_t const v_lh, double4_t const v_rh)
{
	failassert(v_out && v_lh && v_rh, NULL);
	vx(v_out) = (vy(v_lh) * vz(v_rh) - vz(v_lh) * vy(v_rh));
	vy(v_out) = (vz(v_lh) * vx(v_rh) - vx(v_lh) * vz(v_rh));
	vz(v_out) = (vx(v_lh) * vy(v_rh) - vy(v_lh) * vx(v_rh));
	vw(v_out) = 0;
	return v_out;
}

CDRAW_INL vecd_t vecLenSq4d(double4_t const v)
{
	failassert(v, 0);
	return (gSq(vx(v)) + gSq(vy(v)) + gSq(vz(v)) + gSq(vw(v)));
}

CDRAW_INL vecd_t vecDistSq4d(double4_t const v_lh, double4_t const v_rh)
{
	failassert(v_lh && v_rh, 0);
	double4_t const disp = {
		(vx(v_lh) - vx(v_rh)),
		(vy(v_lh) - vy(v_rh)),
		(vz(v_lh) - vz(v_rh)),
		(vw(v_lh) - vw(v_rh)),
	};
	return vecLenSq4d(disp);
}

CDRAW_INL vecd_t vecDispDistSq4d(double4_t v_disp_out, double4_t const v_lh, double4_t const v_rh)
{
	failassert(v_disp_out && v_lh && v_rh, 0);
	vx(v_disp_out) = (vx(v_lh) - vx(v_rh));
	vy(v_disp_out) = (vy(v_lh) - vy(v_rh));
	vz(v_disp_out) = (vz(v_lh) - vz(v_rh));
	vw(v_disp_out) = (vw(v_lh) - vw(v_rh));
	return vecLenSq4d(v_disp_out);
}

CDRAW_INL vecb_t vecIsZero4d(double4_t const v)
{
	failassert(v, true);
	return scIsNonPositiveApproxD(vecLenSq4d(v));
}

CDRAW_INL vecb_t vecIsNonZero4d(double4_t const v)
{
	failassert(v, false);
	return scIsPositiveApproxD(vecLenSq4d(v));
}

CDRAW_INL doubleN_t vecMad4d(double4_t v_out, vecd_t const u, double4_t const v_origin, double4_t const v_delta)
{
	failassert(v_out && v_origin && v_delta, NULL);
	vx(v_out) = gMad(vx(v_origin), vx(v_delta), u);
	vy(v_out) = gMad(vy(v_origin), vy(v_delta), u);
	vz(v_out) = gMad(vz(v_origin), vz(v_delta), u);
	vw(v_out) = gMad(vw(v_origin), vw(v_delta), u);
	return v_out;
}

CDRAW_INL doubleN_t vecLerp4d(double4_t v_out, vecd_t const u, double4_t const v_min, double4_t const v_max)
{
	failassert(v_out && v_min && v_max, NULL);
	vx(v_out) = gLerp(vx(v_min), vx(v_max), u);
	vy(v_out) = gLerp(vy(v_min), vy(v_max), u);
	vz(v_out) = gLerp(vz(v_min), vz(v_max), u);
	vw(v_out) = gLerp(vw(v_min), vw(v_max), u);
	return v_out;
}


#endif // #if (!(defined _CDRAW_VEC4D_INL_) && (defined _CDRAW_VECTOR_INL_))