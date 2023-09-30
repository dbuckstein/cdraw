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
* cdrawVec2u.inl
* Definitions of unsigned 2D vector functions.
*/

#if (!(defined _CDRAW_VEC2U_INL_) && (defined _CDRAW_VECTOR_INL_))
#define _CDRAW_VEC2U_INL_


CDRAW_INL uintN_t vecZero2u(uint2_t v_out)
{
	failassert(v_out, NULL);
	vx(v_out) = 0;
	vy(v_out) = 0;
	return v_out;
}

CDRAW_INL uintN_t vecInit2u(uint2_t v_out, vecu_t const x, vecu_t const y)
{
	failassert(v_out, NULL);
	vx(v_out) = x;
	vy(v_out) = y;
	return v_out;
}

CDRAW_INL uintN_t vecInitS2u(uint2_t v_out, vecu_t const x)
{
	failassert(v_out, NULL);
	vx(v_out) = x;
	vy(v_out) = x;
	return v_out;
}

CDRAW_INL uintN_t vecCopy2u(uint2_t v_out, uint2_t const v)
{
	failassert(v_out && v, NULL);
	vx(v_out) = vx(v);
	vy(v_out) = vy(v);
	return v_out;
}

CDRAW_INL uintN_t vecCopy2b2u(uint2_t v_out, bool2_t const v)
{
	failassert(v_out && v, NULL);
	vx(v_out) = (vx(v) ? 1 : 0);
	vy(v_out) = (vy(v) ? 1 : 0);
	return v_out;
}

CDRAW_INL uintN_t vecCopy2i2u(uint2_t v_out, int2_t const v)
{
	failassert(v_out && v, NULL);
	vx(v_out) = (vecu_t)(vx(v));
	vy(v_out) = (vecu_t)(vy(v));
	return v_out;
}

CDRAW_INL uintN_t vecCopy2f2u(uint2_t v_out, float2_t const v)
{
	failassert(v_out && v, NULL);
	vx(v_out) = (vecu_t)(vx(v));
	vy(v_out) = (vecu_t)(vy(v));
	return v_out;
}

CDRAW_INL uintN_t vecCopy2d2u(uint2_t v_out, double2_t const v)
{
	failassert(v_out && v, NULL);
	vx(v_out) = (vecu_t)(vx(v));
	vy(v_out) = (vecu_t)(vy(v));
	return v_out;
}

CDRAW_INL uintN_t vecNot2u(uint2_t v_out, uint2_t const v)
{
	failassert(v_out && v, NULL);
	vx(v_out) = ~vx(v);
	vy(v_out) = ~vy(v);
	return v_out;
}

CDRAW_INL uintN_t vecAnd2u(uint2_t v_out, uint2_t const v_lh, uint2_t const v_rh)
{
	failassert(v_out && v_lh && v_rh, NULL);
	vx(v_out) = vx(v_lh) & vx(v_rh);
	vy(v_out) = vy(v_lh) & vy(v_rh);
	return v_out;
}

CDRAW_INL uintN_t vecNand2u(uint2_t v_out, uint2_t const v_lh, uint2_t const v_rh)
{
	failassert(v_out && v_lh && v_rh, NULL);
	vx(v_out) = ~(vx(v_lh) & vx(v_rh));
	vy(v_out) = ~(vy(v_lh) & vy(v_rh));
	return v_out;
}

CDRAW_INL uintN_t vecOr2u(uint2_t v_out, uint2_t const v_lh, uint2_t const v_rh)
{
	failassert(v_out && v_lh && v_rh, NULL);
	vx(v_out) = vx(v_lh) | vx(v_rh);
	vy(v_out) = vy(v_lh) | vy(v_rh);
	return v_out;
}

CDRAW_INL uintN_t vecNor2u(uint2_t v_out, uint2_t const v_lh, uint2_t const v_rh)
{
	failassert(v_out && v_lh && v_rh, NULL);
	vx(v_out) = ~(vx(v_lh) | vx(v_rh));
	vy(v_out) = ~(vy(v_lh) | vy(v_rh));
	return v_out;
}

CDRAW_INL uintN_t vecXor2u(uint2_t v_out, uint2_t const v_lh, uint2_t const v_rh)
{
	failassert(v_out && v_lh && v_rh, NULL);
	vx(v_out) = vx(v_lh) ^ vx(v_rh);
	vy(v_out) = vy(v_lh) ^ vy(v_rh);
	return v_out;
}

CDRAW_INL uintN_t vecXnor2u(uint2_t v_out, uint2_t const v_lh, uint2_t const v_rh)
{
	failassert(v_out && v_lh && v_rh, NULL);
	vx(v_out) = ~(vx(v_lh) ^ vx(v_rh));
	vy(v_out) = ~(vy(v_lh) ^ vy(v_rh));
	return v_out;
}

CDRAW_INL boolN_t vecEq2u(bool2_t v_out, uint2_t const v_lh, uint2_t const v_rh)
{
	failassert(v_out && v_lh && v_rh, NULL);
	vx(v_out) = (vx(v_lh) == vx(v_rh));
	vy(v_out) = (vy(v_lh) == vy(v_rh));
	return v_out;
}

CDRAW_INL boolN_t vecNeq2u(bool2_t v_out, uint2_t const v_lh, uint2_t const v_rh)
{
	failassert(v_out && v_lh && v_rh, NULL);
	vx(v_out) = (vx(v_lh) != vx(v_rh));
	vy(v_out) = (vy(v_lh) != vy(v_rh));
	return v_out;
}

CDRAW_INL boolN_t vecGreater2u(bool2_t v_out, uint2_t const v_lh, uint2_t const v_rh)
{
	failassert(v_out && v_lh && v_rh, NULL);
	vx(v_out) = (vx(v_lh) > vx(v_rh));
	vy(v_out) = (vy(v_lh) > vy(v_rh));
	return v_out;
}

CDRAW_INL boolN_t vecGreaterEq2u(bool2_t v_out, uint2_t const v_lh, uint2_t const v_rh)
{
	failassert(v_out && v_lh && v_rh, NULL);
	vx(v_out) = (vx(v_lh) >= vx(v_rh));
	vy(v_out) = (vy(v_lh) >= vy(v_rh));
	return v_out;
}

CDRAW_INL boolN_t vecLess2u(bool2_t v_out, uint2_t const v_lh, uint2_t const v_rh)
{
	failassert(v_out && v_lh && v_rh, NULL);
	vx(v_out) = (vx(v_lh) < vx(v_rh));
	vy(v_out) = (vy(v_lh) < vy(v_rh));
	return v_out;
}

CDRAW_INL boolN_t vecLessEq2u(bool2_t v_out, uint2_t const v_lh, uint2_t const v_rh)
{
	failassert(v_out && v_lh && v_rh, NULL);
	vx(v_out) = (vx(v_lh) <= vx(v_rh));
	vy(v_out) = (vy(v_lh) <= vy(v_rh));
	return v_out;
}

CDRAW_INL intN_t vecNegate2u(int2_t v_out, uint2_t const v)
{
	failassert(v_out && v, NULL);
	vx(v_out) = -(veci_t)vx(v);
	vy(v_out) = -(veci_t)vy(v);
	return v_out;
}

CDRAW_INL uintN_t vecAdd2u(uint2_t v_out, uint2_t const v_lh, uint2_t const v_rh)
{
	failassert(v_out && v_lh && v_rh, NULL);
	vx(v_out) = vx(v_lh) + vx(v_rh);
	vy(v_out) = vy(v_lh) + vy(v_rh);
	return v_out;
}

CDRAW_INL intN_t vecSub2u(int2_t v_out, uint2_t const v_lh, uint2_t const v_rh)
{
	failassert(v_out && v_lh && v_rh, NULL);
	vx(v_out) = vx(v_lh) - vx(v_rh);
	vy(v_out) = vy(v_lh) - vy(v_rh);
	return v_out;
}

CDRAW_INL uintN_t vecMul2u(uint2_t v_out, uint2_t const v_lh, uint2_t const v_rh)
{
	failassert(v_out && v_lh && v_rh, NULL);
	vx(v_out) = vx(v_lh) * vx(v_rh);
	vy(v_out) = vy(v_lh) * vy(v_rh);
	return v_out;
}

CDRAW_INL uintN_t vecDiv2u(uint2_t v_out, uint2_t const v_lh, uint2_t const v_rh)
{
	failassert(v_out && v_lh && v_rh, NULL);
	failassert(vx(v_rh) && vy(v_rh), vecInit2u(v_out,
		gDivSafe(vx(v_lh), vx(v_rh)),
		gDivSafe(vy(v_lh), vy(v_rh))));
	vx(v_out) = vx(v_lh) / vx(v_rh);
	vy(v_out) = vy(v_lh) / vy(v_rh);
	return v_out;
}

CDRAW_INL uintN_t vecMod2u(uint2_t v_out, uint2_t const v_lh, uint2_t const v_rh)
{
	failassert(v_out && v_lh && v_rh, NULL);
	failassert(vx(v_rh) && vy(v_rh), vecInit2u(v_out,
		gModSafe(vx(v_lh), vx(v_rh)),
		gModSafe(vy(v_lh), vy(v_rh))));
	vx(v_out) = vx(v_lh) % vx(v_rh);
	vy(v_out) = vy(v_lh) % vy(v_rh);
	return v_out;
}

CDRAW_INL uintN_t vecDivMod2u(uint2_t v_out, uint2_t v_mod_out, uint2_t const v_lh, uint2_t const v_rh)
{
	failassert(v_out && v_mod_out && v_lh && v_rh, NULL);
	failassert(vx(v_rh) && vy(v_rh), vecInit2u(v_out,
		gDivModSafe(vx(v_mod_out), vx(v_lh), vx(v_rh)),
		gDivModSafe(vy(v_mod_out), vy(v_lh), vy(v_rh))));
	vx(v_out) = vx(v_lh) / vx(v_rh);
	vx(v_mod_out) = vx(v_lh) % vx(v_rh);
	vy(v_out) = vy(v_lh) / vy(v_rh);
	vy(v_mod_out) = vy(v_lh) % vy(v_rh);
	return v_out;
}

CDRAW_INL uintN_t vecAddS2u(uint2_t v_out, uint2_t const v_lh, vecu_t const s_rh)
{
	failassert(v_out && v_lh, NULL);
	vx(v_out) = vx(v_lh) + s_rh;
	vy(v_out) = vy(v_lh) + s_rh;
	return v_out;
}

CDRAW_INL intN_t vecSubS2u(int2_t v_out, uint2_t const v_lh, vecu_t const s_rh)
{
	failassert(v_out && v_lh, NULL);
	vx(v_out) = vx(v_lh) - s_rh;
	vy(v_out) = vy(v_lh) - s_rh;
	return v_out;
}

CDRAW_INL uintN_t vecMulS2u(uint2_t v_out, uint2_t const v_lh, vecu_t const s_rh)
{
	failassert(v_out && v_lh, NULL);
	vx(v_out) = vx(v_lh) * s_rh;
	vy(v_out) = vy(v_lh) * s_rh;
	return v_out;
}

CDRAW_INL uintN_t vecDivS2u(uint2_t v_out, uint2_t const v_lh, vecu_t const s_rh)
{
	failassert(v_out && v_lh, NULL);
	failassert(s_rh, vecZero2u(v_out));
	vx(v_out) = vx(v_lh) / s_rh;
	vy(v_out) = vy(v_lh) / s_rh;
	return v_out;
}

CDRAW_INL uintN_t vecModS2u(uint2_t v_out, uint2_t const v_lh, vecu_t const s_rh)
{
	failassert(v_out && v_lh, NULL);
	failassert(s_rh, vecCopy2u(v_out, v_lh));
	vx(v_out) = vx(v_lh) % s_rh;
	vy(v_out) = vy(v_lh) % s_rh;
	return v_out;
}

CDRAW_INL uintN_t vecDivModS2u(uint2_t v_out, uint2_t v_mod_out, uint2_t const v_lh, vecu_t const s_rh)
{
	failassert(v_out && v_lh, NULL);
	failassert(s_rh, vecCopy2u(v_mod_out, v_lh), vecZero2u(v_out));
	vx(v_out) = vx(v_lh) / s_rh;
	vx(v_mod_out) = vx(v_lh) % s_rh;
	vy(v_out) = vy(v_lh) / s_rh;
	vy(v_mod_out) = vy(v_lh) % s_rh;
	return v_out;
}

CDRAW_INL vecu_t vecDot2u(uint2_t const v_lh, uint2_t const v_rh)
{
	failassert(v_lh && v_rh, 0);
	return (vx(v_lh) * vx(v_rh) + vy(v_lh) * vy(v_rh));
}

CDRAW_INL veci_t vecCross2u(uint2_t const v_lh, uint2_t const v_rh)
{
	failassert(v_lh && v_rh, 0);
	return (vx(v_lh) * vy(v_lh) - vy(v_lh) * vx(v_rh));
}

CDRAW_INL vecu_t vecLenSq2u(uint2_t const v)
{
	failassert(v, 0);
	return (gSq(vx(v)) + gSq(vy(v)));
}

CDRAW_INL vecu_t vecDistSq2u(uint2_t const v_lh, uint2_t const v_rh)
{
	failassert(v_lh && v_rh, 0);
	uint2_t const disp = {
		vx(v_lh) - vx(v_rh),
		vy(v_lh) - vy(v_rh),
	};
	return (gSq(vx(disp)) + gSq(vy(disp)));
}

CDRAW_INL vecu_t vecDispDistSq2u(int2_t v_disp_out, uint2_t const v_lh, uint2_t const v_rh)
{
	failassert(v_disp_out && v_lh && v_rh, 0);
	vx(v_disp_out) = vx(v_lh) - vx(v_rh);
	vy(v_disp_out) = vy(v_lh) - vy(v_rh);
	return (gSq(vx(v_disp_out)) + gSq(vy(v_disp_out)));
}

CDRAW_INL vecb_t vecIsZero2u(uint2_t const v)
{
	return ((vx(v) == 0) && (vy(v) == 0));
}

CDRAW_INL vecb_t vecIsNonZero2u(uint2_t const v)
{
	return ((vx(v) != 0) || (vy(v) != 0));
}

CDRAW_INL uintN_t vecMad2u(uint2_t v_out, vecu_t const u, uint2_t const v_origin, uint2_t const v_delta)
{
	failassert(v_out && v_origin && v_delta, NULL);
	vx(v_out) = gMad(vx(v_origin), vx(v_delta), u);
	vy(v_out) = gMad(vy(v_origin), vy(v_delta), u);
	return v_out;
}

CDRAW_INL uintN_t vecLerp2u(uint2_t v_out, vecu_t const u, uint2_t const v_min, uint2_t const v_max)
{
	failassert(v_out && v_min && v_max, NULL);
	vx(v_out) = gLerp(vx(v_min), vx(v_max), u);
	vy(v_out) = gLerp(vy(v_min), vy(v_max), u);
	return v_out;
}


#endif // #if (!(defined _CDRAW_VEC2U_INL_) && (defined _CDRAW_VECTOR_INL_))