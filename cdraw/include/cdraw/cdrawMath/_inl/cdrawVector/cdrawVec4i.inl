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
* cdrawVec4i.inl
* Definitions of integer 4D vector functions.
*/

#if (!(defined _CDRAW_VEC4I_INL_) && (defined _CDRAW_VECTOR_INL_))
#define _CDRAW_VEC4I_INL_


CDRAW_INL intN_t vecZero4i(int4_t v_out)
{
	failassert(v_out, NULL);
	vx(v_out) = 0;
	vy(v_out) = 0;
	vz(v_out) = 0;
	vw(v_out) = 0;
	return v_out;
}

CDRAW_INL intN_t vecInit4i(int4_t v_out, veci_t const x, veci_t const y, veci_t const z, veci_t const w)
{
	failassert(v_out, NULL);
	vx(v_out) = x;
	vy(v_out) = y;
	vz(v_out) = z;
	vw(v_out) = w;
	return v_out;
}

CDRAW_INL intN_t vecInitS4i(int4_t v_out, veci_t const x)
{
	failassert(v_out, NULL);
	vx(v_out) = x;
	vy(v_out) = x;
	vz(v_out) = x;
	vw(v_out) = x;
	return v_out;
}

CDRAW_INL intN_t vecCopy2zw4i(int4_t v_out, int2_t const xy, veci_t const z, veci_t const w)
{
	failassert(v_out && xy, NULL);
	vx(v_out) = vx(xy);
	vy(v_out) = vy(xy);
	vz(v_out) = z;
	vw(v_out) = w;
	return v_out;
}

CDRAW_INL intN_t vecCopy3w4i(int4_t v_out, int3_t const xyz, veci_t const w)
{
	failassert(v_out && xyz, NULL);
	vx(v_out) = vx(xyz);
	vy(v_out) = vy(xyz);
	vz(v_out) = vz(xyz);
	vw(v_out) = w;
	return v_out;
}

CDRAW_INL intN_t vecCopy4i(int4_t v_out, int4_t const v)
{
	failassert(v_out && v, NULL);
	vx(v_out) = vx(v);
	vy(v_out) = vy(v);
	vz(v_out) = vz(v);
	vw(v_out) = vw(v);
	return v_out;
}

CDRAW_INL intN_t vecCopy4b4i(int4_t v_out, bool4_t const v)
{
	failassert(v_out && v, NULL);
	vx(v_out) = (vx(v) ? 1 : 0);
	vy(v_out) = (vy(v) ? 1 : 0);
	vz(v_out) = (vz(v) ? 1 : 0);
	vw(v_out) = (vw(v) ? 1 : 0);
	return v_out;
}

CDRAW_INL intN_t vecCopy4u4i(int4_t v_out, uint4_t const v)
{
	failassert(v_out && v, NULL);
	vx(v_out) = (veci_t)(vx(v));
	vy(v_out) = (veci_t)(vy(v));
	vz(v_out) = (veci_t)(vz(v));
	vw(v_out) = (veci_t)(vw(v));
	return v_out;
}

CDRAW_INL intN_t vecCopy4f4i(int4_t v_out, float4_t const v)
{
	failassert(v_out && v, NULL);
	vx(v_out) = (veci_t)(vx(v));
	vy(v_out) = (veci_t)(vy(v));
	vz(v_out) = (veci_t)(vz(v));
	vw(v_out) = (veci_t)(vw(v));
	return v_out;
}

CDRAW_INL intN_t vecCopy4d4i(int4_t v_out, double4_t const v)
{
	failassert(v_out && v, NULL);
	vx(v_out) = (veci_t)(vx(v));
	vy(v_out) = (veci_t)(vy(v));
	vz(v_out) = (veci_t)(vz(v));
	vw(v_out) = (veci_t)(vw(v));
	return v_out;
}

CDRAW_INL intN_t vecNot4i(int4_t v_out, int4_t const v)
{
	failassert(v_out && v, NULL);
	vx(v_out) = ~vx(v);
	vy(v_out) = ~vy(v);
	vz(v_out) = ~vz(v);
	vw(v_out) = ~vw(v);
	return v_out;
}

CDRAW_INL intN_t vecAnd4i(int4_t v_out, int4_t const v_lh, int4_t const v_rh)
{
	failassert(v_out && v_lh && v_rh, NULL);
	vx(v_out) = vx(v_lh) & vx(v_rh);
	vy(v_out) = vy(v_lh) & vy(v_rh);
	vz(v_out) = vz(v_lh) & vz(v_rh);
	vw(v_out) = vw(v_lh) & vw(v_rh);
	return v_out;
}

CDRAW_INL intN_t vecNand4i(int4_t v_out, int4_t const v_lh, int4_t const v_rh)
{
	failassert(v_out && v_lh && v_rh, NULL);
	vx(v_out) = ~(vx(v_lh) & vx(v_rh));
	vy(v_out) = ~(vy(v_lh) & vy(v_rh));
	vz(v_out) = ~(vz(v_lh) & vz(v_rh));
	vw(v_out) = ~(vw(v_lh) & vw(v_rh));
	return v_out;
}

CDRAW_INL intN_t vecOr4i(int4_t v_out, int4_t const v_lh, int4_t const v_rh)
{
	failassert(v_out && v_lh && v_rh, NULL);
	vx(v_out) = vx(v_lh) | vx(v_rh);
	vy(v_out) = vy(v_lh) | vy(v_rh);
	vz(v_out) = vz(v_lh) | vz(v_rh);
	vw(v_out) = vw(v_lh) | vw(v_rh);
	return v_out;
}

CDRAW_INL intN_t vecNor4i(int4_t v_out, int4_t const v_lh, int4_t const v_rh)
{
	failassert(v_out && v_lh && v_rh, NULL);
	vx(v_out) = ~(vx(v_lh) | vx(v_rh));
	vy(v_out) = ~(vy(v_lh) | vy(v_rh));
	vz(v_out) = ~(vz(v_lh) | vz(v_rh));
	vw(v_out) = ~(vw(v_lh) | vw(v_rh));
	return v_out;
}

CDRAW_INL intN_t vecXor4i(int4_t v_out, int4_t const v_lh, int4_t const v_rh)
{
	failassert(v_out && v_lh && v_rh, NULL);
	vx(v_out) = vx(v_lh) ^ vx(v_rh);
	vy(v_out) = vy(v_lh) ^ vy(v_rh);
	vz(v_out) = vz(v_lh) ^ vz(v_rh);
	vw(v_out) = vw(v_lh) ^ vw(v_rh);
	return v_out;
}

CDRAW_INL intN_t vecXnor4i(int4_t v_out, int4_t const v_lh, int4_t const v_rh)
{
	failassert(v_out && v_lh && v_rh, NULL);
	vx(v_out) = ~(vx(v_lh) ^ vx(v_rh));
	vy(v_out) = ~(vy(v_lh) ^ vy(v_rh));
	vz(v_out) = ~(vz(v_lh) ^ vz(v_rh));
	vw(v_out) = ~(vw(v_lh) ^ vw(v_rh));
	return v_out;
}

CDRAW_INL boolN_t vecEq4i(bool4_t v_out, int4_t const v_lh, int4_t const v_rh)
{
	failassert(v_out && v_lh && v_rh, NULL);
	vx(v_out) = (vx(v_lh) == vx(v_rh));
	vy(v_out) = (vy(v_lh) == vy(v_rh));
	vz(v_out) = (vz(v_lh) == vz(v_rh));
	vw(v_out) = (vw(v_lh) == vw(v_rh));
	return v_out;
}

CDRAW_INL boolN_t vecNeq4i(bool4_t v_out, int4_t const v_lh, int4_t const v_rh)
{
	failassert(v_out && v_lh && v_rh, NULL);
	vx(v_out) = (vx(v_lh) != vx(v_rh));
	vy(v_out) = (vy(v_lh) != vy(v_rh));
	vz(v_out) = (vz(v_lh) != vz(v_rh));
	vw(v_out) = (vw(v_lh) != vw(v_rh));
	return v_out;
}

CDRAW_INL boolN_t vecGreater4i(bool4_t v_out, int4_t const v_lh, int4_t const v_rh)
{
	failassert(v_out && v_lh && v_rh, NULL);
	vx(v_out) = (vx(v_lh) > vx(v_rh));
	vy(v_out) = (vy(v_lh) > vy(v_rh));
	vz(v_out) = (vz(v_lh) > vz(v_rh));
	vw(v_out) = (vw(v_lh) > vw(v_rh));
	return v_out;
}

CDRAW_INL boolN_t vecGreaterEq4i(bool4_t v_out, int4_t const v_lh, int4_t const v_rh)
{
	failassert(v_out && v_lh && v_rh, NULL);
	vx(v_out) = (vx(v_lh) >= vx(v_rh));
	vy(v_out) = (vy(v_lh) >= vy(v_rh));
	vz(v_out) = (vz(v_lh) >= vz(v_rh));
	vw(v_out) = (vw(v_lh) >= vw(v_rh));
	return v_out;
}

CDRAW_INL boolN_t vecLess4i(bool4_t v_out, int4_t const v_lh, int4_t const v_rh)
{
	failassert(v_out && v_lh && v_rh, NULL);
	vx(v_out) = (vx(v_lh) < vx(v_rh));
	vy(v_out) = (vy(v_lh) < vy(v_rh));
	vz(v_out) = (vz(v_lh) < vz(v_rh));
	vw(v_out) = (vw(v_lh) < vw(v_rh));
	return v_out;
}

CDRAW_INL boolN_t vecLessEq4i(bool4_t v_out, int4_t const v_lh, int4_t const v_rh)
{
	failassert(v_out && v_lh && v_rh, NULL);
	vx(v_out) = (vx(v_lh) <= vx(v_rh));
	vy(v_out) = (vy(v_lh) <= vy(v_rh));
	vz(v_out) = (vz(v_lh) <= vz(v_rh));
	vw(v_out) = (vw(v_lh) <= vw(v_rh));
	return v_out;
}

CDRAW_INL intN_t vecNegate4i(int4_t v_out, int4_t const v)
{
	failassert(v_out && v, NULL);
	vx(v_out) = -vx(v);
	vy(v_out) = -vy(v);
	vz(v_out) = -vz(v);
	vw(v_out) = -vw(v);
	return v_out;
}

CDRAW_INL intN_t vecAdd4i(int4_t v_out, int4_t const v_lh, int4_t const v_rh)
{
	failassert(v_out && v_lh && v_rh, NULL);
	vx(v_out) = vx(v_lh) + vx(v_rh);
	vy(v_out) = vy(v_lh) + vy(v_rh);
	vz(v_out) = vz(v_lh) + vz(v_rh);
	vw(v_out) = vw(v_lh) + vw(v_rh);
	return v_out;
}

CDRAW_INL intN_t vecSub4i(int4_t v_out, int4_t const v_lh, int4_t const v_rh)
{
	failassert(v_out && v_lh && v_rh, NULL);
	vx(v_out) = vx(v_lh) - vx(v_rh);
	vy(v_out) = vy(v_lh) - vy(v_rh);
	vz(v_out) = vz(v_lh) - vz(v_rh);
	vw(v_out) = vw(v_lh) - vw(v_rh);
	return v_out;
}

CDRAW_INL intN_t vecMul4i(int4_t v_out, int4_t const v_lh, int4_t const v_rh)
{
	failassert(v_out && v_lh && v_rh, NULL);
	vx(v_out) = vx(v_lh) * vx(v_rh);
	vy(v_out) = vy(v_lh) * vy(v_rh);
	vz(v_out) = vz(v_lh) * vz(v_rh);
	vw(v_out) = vw(v_lh) * vw(v_rh);
	return v_out;
}

CDRAW_INL intN_t vecDiv4i(int4_t v_out, int4_t const v_lh, int4_t const v_rh)
{
	failassert(v_out && v_lh && v_rh, NULL);
	failassert(vx(v_rh) && vy(v_rh), vecInit4i(v_out,
		gDivSafe(vx(v_lh), vx(v_rh)),
		gDivSafe(vy(v_lh), vy(v_rh)),
		gDivSafe(vz(v_lh), vz(v_rh)),
		gDivSafe(vw(v_lh), vw(v_rh))));
	vx(v_out) = vx(v_lh) / vx(v_rh);
	vy(v_out) = vy(v_lh) / vy(v_rh);
	vz(v_out) = vz(v_lh) / vz(v_rh);
	vw(v_out) = vw(v_lh) / vw(v_rh);
	return v_out;
}

CDRAW_INL intN_t vecMod4i(int4_t v_out, int4_t const v_lh, int4_t const v_rh)
{
	failassert(v_out && v_lh && v_rh, NULL);
	failassert(vx(v_rh) && vy(v_rh), vecInit4i(v_out,
		gModSafe(vx(v_lh), vx(v_rh)),
		gModSafe(vy(v_lh), vy(v_rh)),
		gModSafe(vz(v_lh), vz(v_rh)),
		gModSafe(vw(v_lh), vw(v_rh))));
	vx(v_out) = vx(v_lh) % vx(v_rh);
	vy(v_out) = vy(v_lh) % vy(v_rh);
	vz(v_out) = vz(v_lh) % vz(v_rh);
	vw(v_out) = vw(v_lh) % vw(v_rh);
	return v_out;
}

CDRAW_INL intN_t vecDivMod4i(int4_t v_out, int4_t v_mod_out, int4_t const v_lh, int4_t const v_rh)
{
	failassert(v_out && v_mod_out && v_lh && v_rh, NULL);
	failassert(vx(v_rh) && vy(v_rh), vecInit4i(v_out,
		gDivModSafe(vx(v_mod_out), vx(v_lh), vx(v_rh)),
		gDivModSafe(vy(v_mod_out), vy(v_lh), vy(v_rh)),
		gDivModSafe(vz(v_mod_out), vz(v_lh), vz(v_rh)),
		gDivModSafe(vw(v_mod_out), vw(v_lh), vw(v_rh))));
	vx(v_out) = vx(v_lh) / vx(v_rh);
	vx(v_mod_out) = vx(v_lh) % vx(v_rh);
	vy(v_out) = vy(v_lh) / vy(v_rh);
	vy(v_mod_out) = vy(v_lh) % vy(v_rh);
	vz(v_out) = vz(v_lh) / vz(v_rh);
	vz(v_mod_out) = vz(v_lh) % vz(v_rh);
	vw(v_out) = vw(v_lh) / vw(v_rh);
	vw(v_mod_out) = vw(v_lh) % vw(v_rh);
	return v_out;
}

CDRAW_INL intN_t vecAddS4i(int4_t v_out, int4_t const v_lh, veci_t const s_rh)
{
	failassert(v_out && v_lh, NULL);
	vx(v_out) = vx(v_lh) + s_rh;
	vy(v_out) = vy(v_lh) + s_rh;
	vz(v_out) = vz(v_lh) + s_rh;
	vw(v_out) = vw(v_lh) + s_rh;
	return v_out;
}

CDRAW_INL intN_t vecSubS4i(int4_t v_out, int4_t const v_lh, veci_t const s_rh)
{
	failassert(v_out && v_lh, NULL);
	vx(v_out) = vx(v_lh) - s_rh;
	vy(v_out) = vy(v_lh) - s_rh;
	vz(v_out) = vz(v_lh) - s_rh;
	vw(v_out) = vw(v_lh) - s_rh;
	return v_out;
}

CDRAW_INL intN_t vecMulS4i(int4_t v_out, int4_t const v_lh, veci_t const s_rh)
{
	failassert(v_out && v_lh, NULL);
	vx(v_out) = vx(v_lh) * s_rh;
	vy(v_out) = vy(v_lh) * s_rh;
	vz(v_out) = vz(v_lh) * s_rh;
	vw(v_out) = vw(v_lh) * s_rh;
	return v_out;
}

CDRAW_INL intN_t vecDivS4i(int4_t v_out, int4_t const v_lh, veci_t const s_rh)
{
	failassert(v_out && v_lh, NULL);
	failassert(s_rh, vecZero4i(v_out));
	vx(v_out) = vx(v_lh) / s_rh;
	vy(v_out) = vy(v_lh) / s_rh;
	vz(v_out) = vz(v_lh) / s_rh;
	vw(v_out) = vw(v_lh) / s_rh;
	return v_out;
}

CDRAW_INL intN_t vecModS4i(int4_t v_out, int4_t const v_lh, veci_t const s_rh)
{
	failassert(v_out && v_lh, NULL);
	failassert(s_rh, vecCopy4i(v_out, v_lh));
	vx(v_out) = vx(v_lh) % s_rh;
	vy(v_out) = vy(v_lh) % s_rh;
	vz(v_out) = vz(v_lh) % s_rh;
	vw(v_out) = vw(v_lh) % s_rh;
	return v_out;
}

CDRAW_INL intN_t vecDivModS4i(int4_t v_out, int4_t v_mod_out, int4_t const v_lh, veci_t const s_rh)
{
	failassert(v_out && v_lh, NULL);
	failassert(s_rh, vecCopy4i(v_mod_out, v_lh), vecZero4i(v_out));
	vx(v_out) = vx(v_lh) / s_rh;
	vx(v_mod_out) = vx(v_lh) % s_rh;
	vy(v_out) = vy(v_lh) / s_rh;
	vy(v_mod_out) = vy(v_lh) % s_rh;
	vz(v_out) = vz(v_lh) / s_rh;
	vz(v_mod_out) = vz(v_lh) % s_rh;
	vw(v_out) = vw(v_lh) / s_rh;
	vw(v_mod_out) = vw(v_lh) % s_rh;
	return v_out;
}

CDRAW_INL veci_t vecDot4i(int4_t const v_lh, int4_t const v_rh)
{
	failassert(v_lh && v_rh, 0);
	return (vx(v_lh) * vx(v_rh) + vy(v_lh) * vy(v_rh) + vz(v_lh) * vz(v_rh) + vw(v_lh) * vw(v_rh));
}

CDRAW_INL intN_t vecCross4i(int4_t v_out, int4_t const v_lh, int4_t const v_rh)
{
	failassert(v_out && v_lh && v_rh, NULL);
	vx(v_out) = (vy(v_lh) * vz(v_rh) - vz(v_lh) * vy(v_rh));
	vy(v_out) = (vz(v_lh) * vx(v_rh) - vx(v_lh) * vz(v_rh));
	vz(v_out) = (vx(v_lh) * vy(v_rh) - vy(v_lh) * vx(v_rh));
	vw(v_out) = 0;
	return v_out;
}

CDRAW_INL vecu_t vecLenSq4i(int4_t const v)
{
	failassert(v, 0);
	return (gSq(vx(v)) + gSq(vy(v)) + gSq(vz(v)) + gSq(vw(v)));
}

CDRAW_INL vecu_t vecDistSq4i(int4_t const v_lh, int4_t const v_rh)
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

CDRAW_INL vecu_t vecDispDistSq4i(int4_t v_disp_out, int4_t const v_lh, int4_t const v_rh)
{
	failassert(v_disp_out && v_lh && v_rh, 0);
	vx(v_disp_out) = vx(v_lh) - vx(v_rh);
	vy(v_disp_out) = vy(v_lh) - vy(v_rh);
	vz(v_disp_out) = vz(v_lh) - vz(v_rh);
	vw(v_disp_out) = vw(v_lh) - vw(v_rh);
	return (gSq(vx(v_disp_out)) + gSq(vy(v_disp_out)) + gSq(vz(v_disp_out)) + gSq(vw(v_disp_out)));
}

CDRAW_INL vecb_t vecIsZero4i(int4_t const v)
{
	return ((vx(v) == 0) && (vy(v) == 0) && (vz(v) == 0) && (vw(v) == 0));
}

CDRAW_INL vecb_t vecIsNonZero4i(int4_t const v)
{
	return ((vx(v) != 0) || (vy(v) != 0) || (vz(v) != 0) || (vw(v) != 0));
}

CDRAW_INL intN_t vecMad4i(int4_t v_out, veci_t const u, int4_t const v_origin, int4_t const v_delta)
{
	failassert(v_out && v_origin && v_delta, NULL);
	vx(v_out) = gMad(vx(v_origin), vx(v_delta), u);
	vy(v_out) = gMad(vy(v_origin), vy(v_delta), u);
	vz(v_out) = gMad(vz(v_origin), vz(v_delta), u);
	vw(v_out) = gMad(vw(v_origin), vw(v_delta), u);
	return v_out;
}

CDRAW_INL intN_t vecLerp4i(int4_t v_out, veci_t const u, int4_t const v_min, int4_t const v_max)
{
	failassert(v_out && v_min && v_max, NULL);
	vx(v_out) = gLerp(vx(v_min), vx(v_max), u);
	vy(v_out) = gLerp(vy(v_min), vy(v_max), u);
	vz(v_out) = gLerp(vz(v_min), vz(v_max), u);
	vw(v_out) = gLerp(vw(v_min), vw(v_max), u);
	return v_out;
}


#endif // #if (!(defined _CDRAW_VEC4I_INL_) && (defined _CDRAW_VECTOR_INL_))