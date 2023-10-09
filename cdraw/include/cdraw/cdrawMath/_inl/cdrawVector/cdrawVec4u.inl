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
* cdrawVec4u.inl
* Definitions of unsigned 4D vector functions.
*/

#if (!(defined _CDRAW_VEC4U_INL_) && (defined _CDRAW_VECTOR_INL_))
#define _CDRAW_VEC4U_INL_


CDRAW_INL uintN_t vecZero4u(uint4_t v_out)
{
	failassert(v_out, NULL);
	vx(v_out) = 0u;
	vy(v_out) = 0u;
	vz(v_out) = 0u;
	vw(v_out) = 0u;
	return v_out;
}

CDRAW_INL uintN_t vecInit4u(uint4_t v_out, vecu_t const x, vecu_t const y, vecu_t const z, vecu_t const w)
{
	failassert(v_out, NULL);
	vx(v_out) = x;
	vy(v_out) = y;
	vz(v_out) = z;
	vw(v_out) = w;
	return v_out;
}

CDRAW_INL uintN_t vecInitS4u(uint4_t v_out, vecu_t const x)
{
	failassert(v_out, NULL);
	vx(v_out) = x;
	vy(v_out) = x;
	vz(v_out) = x;
	vw(v_out) = x;
	return v_out;
}

CDRAW_INL uintN_t vecCopy2zw4u(uint4_t v_out, uint2_t const xy, vecu_t const z, vecu_t const w)
{
	failassert(v_out && xy, NULL);
	vx(v_out) = vx(xy);
	vy(v_out) = vy(xy);
	vz(v_out) = z;
	vw(v_out) = w;
	return v_out;
}

CDRAW_INL uintN_t vecCopy3w4u(uint4_t v_out, uint3_t const xyz, vecu_t const w)
{
	failassert(v_out && xyz, NULL);
	vx(v_out) = vx(xyz);
	vy(v_out) = vy(xyz);
	vz(v_out) = vz(xyz);
	vw(v_out) = w;
	return v_out;
}

CDRAW_INL uintN_t vecCopy4u(uint4_t v_out, uint4_t const v)
{
	failassert(v_out && v, NULL);
	vx(v_out) = vx(v);
	vy(v_out) = vy(v);
	vz(v_out) = vz(v);
	vw(v_out) = vw(v);
	return v_out;
}

CDRAW_INL uintN_t vecCopy4b4u(uint4_t v_out, bool4_t const v)
{
	failassert(v_out && v, NULL);
	vx(v_out) = (vx(v) ? 1u : 0u);
	vy(v_out) = (vy(v) ? 1u : 0u);
	vz(v_out) = (vz(v) ? 1u : 0u);
	vw(v_out) = (vw(v) ? 1u : 0u);
	return v_out;
}

CDRAW_INL uintN_t vecCopy4i4u(uint4_t v_out, int4_t const v)
{
	failassert(v_out && v, NULL);
	vx(v_out) = (vecu_t)(vx(v));
	vy(v_out) = (vecu_t)(vy(v));
	vz(v_out) = (vecu_t)(vz(v));
	vw(v_out) = (vecu_t)(vw(v));
	return v_out;
}

CDRAW_INL uintN_t vecCopy4f4u(uint4_t v_out, float4_t const v)
{
	failassert(v_out && v, NULL);
	vx(v_out) = (vecu_t)(vx(v));
	vy(v_out) = (vecu_t)(vy(v));
	vz(v_out) = (vecu_t)(vz(v));
	vw(v_out) = (vecu_t)(vw(v));
	return v_out;
}

CDRAW_INL uintN_t vecCopy4d4u(uint4_t v_out, double4_t const v)
{
	failassert(v_out && v, NULL);
	vx(v_out) = (vecu_t)(vx(v));
	vy(v_out) = (vecu_t)(vy(v));
	vz(v_out) = (vecu_t)(vz(v));
	vw(v_out) = (vecu_t)(vw(v));
	return v_out;
}

CDRAW_INL uintN_t vecNot4u(uint4_t v_out, uint4_t const v)
{
	failassert(v_out && v, NULL);
	vx(v_out) = ~vx(v);
	vy(v_out) = ~vy(v);
	vz(v_out) = ~vz(v);
	vw(v_out) = ~vw(v);
	return v_out;
}

CDRAW_INL uintN_t vecAnd4u(uint4_t v_out, uint4_t const v_lh, uint4_t const v_rh)
{
	failassert(v_out && v_lh && v_rh, NULL);
	vx(v_out) = vx(v_lh) & vx(v_rh);
	vy(v_out) = vy(v_lh) & vy(v_rh);
	vz(v_out) = vz(v_lh) & vz(v_rh);
	vw(v_out) = vw(v_lh) & vw(v_rh);
	return v_out;
}

CDRAW_INL uintN_t vecNand4u(uint4_t v_out, uint4_t const v_lh, uint4_t const v_rh)
{
	failassert(v_out && v_lh && v_rh, NULL);
	vx(v_out) = ~(vx(v_lh) & vx(v_rh));
	vy(v_out) = ~(vy(v_lh) & vy(v_rh));
	vz(v_out) = ~(vz(v_lh) & vz(v_rh));
	vw(v_out) = ~(vw(v_lh) & vw(v_rh));
	return v_out;
}

CDRAW_INL uintN_t vecOr4u(uint4_t v_out, uint4_t const v_lh, uint4_t const v_rh)
{
	failassert(v_out && v_lh && v_rh, NULL);
	vx(v_out) = vx(v_lh) | vx(v_rh);
	vy(v_out) = vy(v_lh) | vy(v_rh);
	vz(v_out) = vz(v_lh) | vz(v_rh);
	vw(v_out) = vw(v_lh) | vw(v_rh);
	return v_out;
}

CDRAW_INL uintN_t vecNor4u(uint4_t v_out, uint4_t const v_lh, uint4_t const v_rh)
{
	failassert(v_out && v_lh && v_rh, NULL);
	vx(v_out) = ~(vx(v_lh) | vx(v_rh));
	vy(v_out) = ~(vy(v_lh) | vy(v_rh));
	vz(v_out) = ~(vz(v_lh) | vz(v_rh));
	vw(v_out) = ~(vw(v_lh) | vw(v_rh));
	return v_out;
}

CDRAW_INL uintN_t vecXor4u(uint4_t v_out, uint4_t const v_lh, uint4_t const v_rh)
{
	failassert(v_out && v_lh && v_rh, NULL);
	vx(v_out) = vx(v_lh) ^ vx(v_rh);
	vy(v_out) = vy(v_lh) ^ vy(v_rh);
	vz(v_out) = vz(v_lh) ^ vz(v_rh);
	vw(v_out) = vw(v_lh) ^ vw(v_rh);
	return v_out;
}

CDRAW_INL uintN_t vecXnor4u(uint4_t v_out, uint4_t const v_lh, uint4_t const v_rh)
{
	failassert(v_out && v_lh && v_rh, NULL);
	vx(v_out) = ~(vx(v_lh) ^ vx(v_rh));
	vy(v_out) = ~(vy(v_lh) ^ vy(v_rh));
	vz(v_out) = ~(vz(v_lh) ^ vz(v_rh));
	vw(v_out) = ~(vw(v_lh) ^ vw(v_rh));
	return v_out;
}

CDRAW_INL boolN_t vecEq4u(bool4_t v_out, uint4_t const v_lh, uint4_t const v_rh)
{
	failassert(v_out && v_lh && v_rh, NULL);
	vx(v_out) = (vx(v_lh) == vx(v_rh));
	vy(v_out) = (vy(v_lh) == vy(v_rh));
	vz(v_out) = (vz(v_lh) == vz(v_rh));
	vw(v_out) = (vw(v_lh) == vw(v_rh));
	return v_out;
}

CDRAW_INL boolN_t vecNeq4u(bool4_t v_out, uint4_t const v_lh, uint4_t const v_rh)
{
	failassert(v_out && v_lh && v_rh, NULL);
	vx(v_out) = (vx(v_lh) != vx(v_rh));
	vy(v_out) = (vy(v_lh) != vy(v_rh));
	vz(v_out) = (vz(v_lh) != vz(v_rh));
	vw(v_out) = (vw(v_lh) != vw(v_rh));
	return v_out;
}

CDRAW_INL boolN_t vecGreater4u(bool4_t v_out, uint4_t const v_lh, uint4_t const v_rh)
{
	failassert(v_out && v_lh && v_rh, NULL);
	vx(v_out) = (vx(v_lh) > vx(v_rh));
	vy(v_out) = (vy(v_lh) > vy(v_rh));
	vz(v_out) = (vz(v_lh) > vz(v_rh));
	vw(v_out) = (vw(v_lh) > vw(v_rh));
	return v_out;
}

CDRAW_INL boolN_t vecGreaterEq4u(bool4_t v_out, uint4_t const v_lh, uint4_t const v_rh)
{
	failassert(v_out && v_lh && v_rh, NULL);
	vx(v_out) = (vx(v_lh) >= vx(v_rh));
	vy(v_out) = (vy(v_lh) >= vy(v_rh));
	vz(v_out) = (vz(v_lh) >= vz(v_rh));
	vw(v_out) = (vw(v_lh) >= vw(v_rh));
	return v_out;
}

CDRAW_INL boolN_t vecLess4u(bool4_t v_out, uint4_t const v_lh, uint4_t const v_rh)
{
	failassert(v_out && v_lh && v_rh, NULL);
	vx(v_out) = (vx(v_lh) < vx(v_rh));
	vy(v_out) = (vy(v_lh) < vy(v_rh));
	vz(v_out) = (vz(v_lh) < vz(v_rh));
	vw(v_out) = (vw(v_lh) < vw(v_rh));
	return v_out;
}

CDRAW_INL boolN_t vecLessEq4u(bool4_t v_out, uint4_t const v_lh, uint4_t const v_rh)
{
	failassert(v_out && v_lh && v_rh, NULL);
	vx(v_out) = (vx(v_lh) <= vx(v_rh));
	vy(v_out) = (vy(v_lh) <= vy(v_rh));
	vz(v_out) = (vz(v_lh) <= vz(v_rh));
	vw(v_out) = (vw(v_lh) <= vw(v_rh));
	return v_out;
}

CDRAW_INL intN_t vecNegate4u(int4_t v_out, uint4_t const v)
{
	failassert(v_out && v, NULL);
	vx(v_out) = -(veci_t)vx(v);
	vy(v_out) = -(veci_t)vy(v);
	vz(v_out) = -(veci_t)vz(v);
	vw(v_out) = -(veci_t)vw(v);
	return v_out;
}

CDRAW_INL uintN_t vecAdd4u(uint4_t v_out, uint4_t const v_lh, uint4_t const v_rh)
{
	failassert(v_out && v_lh && v_rh, NULL);
	vx(v_out) = vx(v_lh) + vx(v_rh);
	vy(v_out) = vy(v_lh) + vy(v_rh);
	vz(v_out) = vz(v_lh) + vz(v_rh);
	vw(v_out) = vw(v_lh) + vw(v_rh);
	return v_out;
}

CDRAW_INL intN_t vecSub4u(int4_t v_out, uint4_t const v_lh, uint4_t const v_rh)
{
	failassert(v_out && v_lh && v_rh, NULL);
	vx(v_out) = vx(v_lh) - vx(v_rh);
	vy(v_out) = vy(v_lh) - vy(v_rh);
	vz(v_out) = vz(v_lh) - vz(v_rh);
	vw(v_out) = vw(v_lh) - vw(v_rh);
	return v_out;
}

CDRAW_INL uintN_t vecMul4u(uint4_t v_out, uint4_t const v_lh, uint4_t const v_rh)
{
	failassert(v_out && v_lh && v_rh, NULL);
	vx(v_out) = vx(v_lh) * vx(v_rh);
	vy(v_out) = vy(v_lh) * vy(v_rh);
	vz(v_out) = vz(v_lh) * vz(v_rh);
	vw(v_out) = vw(v_lh) * vw(v_rh);
	return v_out;
}

CDRAW_INL uintN_t vecDiv4u(uint4_t v_out, uint4_t const v_lh, uint4_t const v_rh)
{
	failassert(v_out && v_lh && v_rh, NULL);
	failassert(vx(v_rh) && vy(v_rh) && vz(v_rh) && vw(v_rh), vecInit4u(v_out,
		gDivSafe(vx(v_lh), vx(v_rh)),
		gDivSafe(vy(v_lh), vy(v_rh)),
		gDivSafe(vz(v_lh), vz(v_rh)),
		gDivSafe(vw(v_lh), vw(v_rh))));
	vx(v_out) = gDiv(vx(v_lh), vx(v_rh));
	vy(v_out) = gDiv(vy(v_lh), vy(v_rh));
	vz(v_out) = gDiv(vz(v_lh), vz(v_rh));
	vw(v_out) = gDiv(vw(v_lh), vw(v_rh));
	return v_out;
}

CDRAW_INL uintN_t vecMod4u(uint4_t v_out, uint4_t const v_lh, uint4_t const v_rh)
{
	failassert(v_out && v_lh && v_rh, NULL);
	failassert(vx(v_rh) && vy(v_rh) && vz(v_rh) && vw(v_rh), vecInit4u(v_out,
		gModSafe(vx(v_lh), vx(v_rh)),
		gModSafe(vy(v_lh), vy(v_rh)),
		gModSafe(vz(v_lh), vz(v_rh)),
		gModSafe(vw(v_lh), vw(v_rh))));
	vx(v_out) = gMod(vx(v_lh), vx(v_rh));
	vy(v_out) = gMod(vy(v_lh), vy(v_rh));
	vz(v_out) = gMod(vz(v_lh), vz(v_rh));
	vw(v_out) = gMod(vw(v_lh), vw(v_rh));
	return v_out;
}

CDRAW_INL uintN_t vecDivMod4u(uint4_t v_out, uint4_t v_mod_out, uint4_t const v_lh, uint4_t const v_rh)
{
	failassert(v_out && v_mod_out && v_lh && v_rh, NULL);
	failassert((v_mod_out != v_out) && (v_mod_out != v_lh) && (v_mod_out != v_rh), NULL);
	failassert(vx(v_rh) && vy(v_rh) && vz(v_rh) && vw(v_rh), vecInit4u(v_out,
		gDivModSafe(vx(v_mod_out), vx(v_lh), vx(v_rh)),
		gDivModSafe(vy(v_mod_out), vy(v_lh), vy(v_rh)),
		gDivModSafe(vz(v_mod_out), vz(v_lh), vz(v_rh)),
		gDivModSafe(vw(v_mod_out), vw(v_lh), vw(v_rh))));
	vx(v_out) = gDivMod(vx(v_mod_out), vx(v_lh), vx(v_rh));
	vy(v_out) = gDivMod(vy(v_mod_out), vy(v_lh), vy(v_rh));
	vz(v_out) = gDivMod(vz(v_mod_out), vz(v_lh), vz(v_rh));
	vw(v_out) = gDivMod(vw(v_mod_out), vw(v_lh), vw(v_rh));
	return v_out;
}

CDRAW_INL uintN_t vecAddS4u(uint4_t v_out, uint4_t const v_lh, vecu_t const s_rh)
{
	failassert(v_out && v_lh, NULL);
	vx(v_out) = vx(v_lh) + s_rh;
	vy(v_out) = vy(v_lh) + s_rh;
	vz(v_out) = vz(v_lh) + s_rh;
	vw(v_out) = vw(v_lh) + s_rh;
	return v_out;
}

CDRAW_INL intN_t vecSubS4u(int4_t v_out, uint4_t const v_lh, vecu_t const s_rh)
{
	failassert(v_out && v_lh, NULL);
	vx(v_out) = vx(v_lh) - s_rh;
	vy(v_out) = vy(v_lh) - s_rh;
	vz(v_out) = vz(v_lh) - s_rh;
	vw(v_out) = vw(v_lh) - s_rh;
	return v_out;
}

CDRAW_INL uintN_t vecMulS4u(uint4_t v_out, uint4_t const v_lh, vecu_t const s_rh)
{
	failassert(v_out && v_lh, NULL);
	vx(v_out) = vx(v_lh) * s_rh;
	vy(v_out) = vy(v_lh) * s_rh;
	vz(v_out) = vz(v_lh) * s_rh;
	vw(v_out) = vw(v_lh) * s_rh;
	return v_out;
}

CDRAW_INL uintN_t vecDivS4u(uint4_t v_out, uint4_t const v_lh, vecu_t const s_rh)
{
	failassert(v_out && v_lh, NULL);
	failassert(s_rh, vecZero4u(v_out));
	vx(v_out) = gDiv(vx(v_lh), s_rh);
	vy(v_out) = gDiv(vy(v_lh), s_rh);
	vz(v_out) = gDiv(vz(v_lh), s_rh);
	vw(v_out) = gDiv(vw(v_lh), s_rh);
	return v_out;
}

CDRAW_INL uintN_t vecModS4u(uint4_t v_out, uint4_t const v_lh, vecu_t const s_rh)
{
	failassert(v_out && v_lh, NULL);
	failassert(s_rh, vecCopy4u(v_out, v_lh));
	vx(v_out) = gMod(vx(v_lh), s_rh);
	vy(v_out) = gMod(vy(v_lh), s_rh);
	vz(v_out) = gMod(vz(v_lh), s_rh);
	vw(v_out) = gMod(vw(v_lh), s_rh);
	return v_out;
}

CDRAW_INL uintN_t vecDivModS4u(uint4_t v_out, uint4_t v_mod_out, uint4_t const v_lh, vecu_t const s_rh)
{
	failassert(v_out && v_mod_out && v_lh, NULL);
	failassert((v_mod_out != v_out) && (v_mod_out != v_lh), NULL);
	failassert(s_rh, vecCopy4u(v_mod_out, v_lh), vecZero4u(v_out));
	vx(v_out) = gDivMod(vx(v_mod_out), vx(v_lh), s_rh);
	vy(v_out) = gDivMod(vy(v_mod_out), vy(v_lh), s_rh);
	vz(v_out) = gDivMod(vz(v_mod_out), vz(v_lh), s_rh);
	vw(v_out) = gDivMod(vw(v_mod_out), vw(v_lh), s_rh);
	return v_out;
}

CDRAW_INL vecu_t vecDot4u(uint4_t const v_lh, uint4_t const v_rh)
{
	failassert(v_lh && v_rh, 0);
	return (vx(v_lh) * vx(v_rh) + vy(v_lh) * vy(v_rh) + vz(v_lh) * vz(v_rh) + vw(v_lh) * vw(v_rh));
}

CDRAW_INL intN_t vecCross4u(int4_t v_out, uint4_t const v_lh, uint4_t const v_rh)
{
	failassert(v_out && v_lh && v_rh, NULL);
	vx(v_out) = (vy(v_lh) * vz(v_rh) - vz(v_lh) * vy(v_rh));
	vy(v_out) = (vz(v_lh) * vx(v_rh) - vx(v_lh) * vz(v_rh));
	vz(v_out) = (vx(v_lh) * vy(v_rh) - vy(v_lh) * vx(v_rh));
	vw(v_out) = 0;
	return v_out;
}

CDRAW_INL vecu_t vecLenSq4u(uint4_t const v)
{
	failassert(v, 0);
	return (gSq(vx(v)) + gSq(vy(v)) + gSq(vz(v)) + gSq(vw(v)));
}

CDRAW_INL vecu_t vecDistSq4u(uint4_t const v_lh, uint4_t const v_rh)
{
	failassert(v_lh && v_rh, 0);
	int4_t const disp = {
		(vx(v_lh) - vx(v_rh)),
		(vy(v_lh) - vy(v_rh)),
		(vz(v_lh) - vz(v_rh)),
		(vw(v_lh) - vw(v_rh)),
	};
	return (gSq(vx(disp)) + gSq(vy(disp)) + gSq(vz(disp)) + gSq(vw(disp)));
}

CDRAW_INL vecu_t vecDispDistSq4u(int4_t v_disp_out, uint4_t const v_lh, uint4_t const v_rh)
{
	failassert(v_disp_out && v_lh && v_rh, 0);
	vx(v_disp_out) = (vx(v_lh) - vx(v_rh));
	vy(v_disp_out) = (vy(v_lh) - vy(v_rh));
	vz(v_disp_out) = (vz(v_lh) - vz(v_rh));
	vw(v_disp_out) = (vw(v_lh) - vw(v_rh));
	return (gSq(vx(v_disp_out)) + gSq(vy(v_disp_out)) + gSq(vz(v_disp_out)) + gSq(vw(v_disp_out)));
}

CDRAW_INL vecb_t vecIsZero4u(uint4_t const v)
{
	failassert(v, true);
	return ((vx(v) == 0) && (vy(v) == 0) && (vz(v) == 0) && (vw(v) == 0));
}

CDRAW_INL vecb_t vecIsNonZero4u(uint4_t const v)
{
	failassert(v, false);
	return ((vx(v) != 0) || (vy(v) != 0) || (vz(v) != 0) || (vw(v) != 0));
}

CDRAW_INL uintN_t vecMad4u(uint4_t v_out, vecu_t const u, uint4_t const v_origin, uint4_t const v_delta)
{
	failassert(v_out && v_origin && v_delta, NULL);
	vx(v_out) = gMad(vx(v_origin), vx(v_delta), u);
	vy(v_out) = gMad(vy(v_origin), vy(v_delta), u);
	vz(v_out) = gMad(vz(v_origin), vz(v_delta), u);
	vw(v_out) = gMad(vw(v_origin), vw(v_delta), u);
	return v_out;
}

CDRAW_INL uintN_t vecLerp4u(uint4_t v_out, vecu_t const u, uint4_t const v_min, uint4_t const v_max)
{
	failassert(v_out && v_min && v_max, NULL);
	vx(v_out) = gLerp(vx(v_min), vx(v_max), u);
	vy(v_out) = gLerp(vy(v_min), vy(v_max), u);
	vz(v_out) = gLerp(vz(v_min), vz(v_max), u);
	vw(v_out) = gLerp(vw(v_min), vw(v_max), u);
	return v_out;
}


#endif // #if (!(defined _CDRAW_VEC4U_INL_) && (defined _CDRAW_VECTOR_INL_))