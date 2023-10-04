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
* cdrawVec3u.inl
* Definitions of unsigned 3D vector functions.
*/

#if (!(defined _CDRAW_VEC3U_INL_) && (defined _CDRAW_VECTOR_INL_))
#define _CDRAW_VEC3U_INL_


CDRAW_INL uintN_t vecZero3u(uint3_t v_out)
{
	failassert(v_out, NULL);
	vx(v_out) = 0;
	vy(v_out) = 0;
	vz(v_out) = 0;
	return v_out;
}

CDRAW_INL uintN_t vecInit3u(uint3_t v_out, vecu_t const x, vecu_t const y, vecu_t const z)
{
	failassert(v_out, NULL);
	vx(v_out) = x;
	vy(v_out) = y;
	vz(v_out) = z;
	return v_out;
}

CDRAW_INL uintN_t vecInitS3u(uint3_t v_out, vecu_t const x)
{
	failassert(v_out, NULL);
	vx(v_out) = x;
	vy(v_out) = x;
	vz(v_out) = x;
	return v_out;
}

CDRAW_INL uintN_t vecCopy2z3u(uint3_t v_out, uint2_t const xy, vecu_t const z)
{
	failassert(v_out && xy, NULL);
	vx(v_out) = vx(xy);
	vy(v_out) = vy(xy);
	vz(v_out) = z;
	return v_out;
}

CDRAW_INL uintN_t vecCopy3u(uint3_t v_out, uint3_t const v)
{
	failassert(v_out && v, NULL);
	vx(v_out) = vx(v);
	vy(v_out) = vy(v);
	vz(v_out) = vz(v);
	return v_out;
}

CDRAW_INL uintN_t vecCopy3b3u(uint3_t v_out, bool3_t const v)
{
	failassert(v_out && v, NULL);
	vx(v_out) = (vx(v) ? 1 : 0);
	vy(v_out) = (vy(v) ? 1 : 0);
	vz(v_out) = (vz(v) ? 1 : 0);
	return v_out;
}

CDRAW_INL uintN_t vecCopy3i3u(uint3_t v_out, int3_t const v)
{
	failassert(v_out && v, NULL);
	vx(v_out) = (vecu_t)(vx(v));
	vy(v_out) = (vecu_t)(vy(v));
	vz(v_out) = (vecu_t)(vz(v));
	return v_out;
}

CDRAW_INL uintN_t vecCopy3f3u(uint3_t v_out, float3_t const v)
{
	failassert(v_out && v, NULL);
	vx(v_out) = (vecu_t)(vx(v));
	vy(v_out) = (vecu_t)(vy(v));
	vz(v_out) = (vecu_t)(vz(v));
	return v_out;
}

CDRAW_INL uintN_t vecCopy3d3u(uint3_t v_out, double3_t const v)
{
	failassert(v_out && v, NULL);
	vx(v_out) = (vecu_t)(vx(v));
	vy(v_out) = (vecu_t)(vy(v));
	vz(v_out) = (vecu_t)(vz(v));
	return v_out;
}

CDRAW_INL uintN_t vecNot3u(uint3_t v_out, uint3_t const v)
{
	failassert(v_out && v, NULL);
	vx(v_out) = ~vx(v);
	vy(v_out) = ~vy(v);
	vz(v_out) = ~vz(v);
	return v_out;
}

CDRAW_INL uintN_t vecAnd3u(uint3_t v_out, uint3_t const v_lh, uint3_t const v_rh)
{
	failassert(v_out && v_lh && v_rh, NULL);
	vx(v_out) = vx(v_lh) & vx(v_rh);
	vy(v_out) = vy(v_lh) & vy(v_rh);
	vz(v_out) = vz(v_lh) & vz(v_rh);
	return v_out;
}

CDRAW_INL uintN_t vecNand3u(uint3_t v_out, uint3_t const v_lh, uint3_t const v_rh)
{
	failassert(v_out && v_lh && v_rh, NULL);
	vx(v_out) = ~(vx(v_lh) & vx(v_rh));
	vy(v_out) = ~(vy(v_lh) & vy(v_rh));
	vz(v_out) = ~(vz(v_lh) & vz(v_rh));
	return v_out;
}

CDRAW_INL uintN_t vecOr3u(uint3_t v_out, uint3_t const v_lh, uint3_t const v_rh)
{
	failassert(v_out && v_lh && v_rh, NULL);
	vx(v_out) = vx(v_lh) | vx(v_rh);
	vy(v_out) = vy(v_lh) | vy(v_rh);
	vz(v_out) = vz(v_lh) | vz(v_rh);
	return v_out;
}

CDRAW_INL uintN_t vecNor3u(uint3_t v_out, uint3_t const v_lh, uint3_t const v_rh)
{
	failassert(v_out && v_lh && v_rh, NULL);
	vx(v_out) = ~(vx(v_lh) | vx(v_rh));
	vy(v_out) = ~(vy(v_lh) | vy(v_rh));
	vz(v_out) = ~(vz(v_lh) | vz(v_rh));
	return v_out;
}

CDRAW_INL uintN_t vecXor3u(uint3_t v_out, uint3_t const v_lh, uint3_t const v_rh)
{
	failassert(v_out && v_lh && v_rh, NULL);
	vx(v_out) = vx(v_lh) ^ vx(v_rh);
	vy(v_out) = vy(v_lh) ^ vy(v_rh);
	vz(v_out) = vz(v_lh) ^ vz(v_rh);
	return v_out;
}

CDRAW_INL uintN_t vecXnor3u(uint3_t v_out, uint3_t const v_lh, uint3_t const v_rh)
{
	failassert(v_out && v_lh && v_rh, NULL);
	vx(v_out) = ~(vx(v_lh) ^ vx(v_rh));
	vy(v_out) = ~(vy(v_lh) ^ vy(v_rh));
	vz(v_out) = ~(vz(v_lh) ^ vz(v_rh));
	return v_out;
}

CDRAW_INL boolN_t vecEq3u(bool3_t v_out, uint3_t const v_lh, uint3_t const v_rh)
{
	failassert(v_out && v_lh && v_rh, NULL);
	vx(v_out) = (vx(v_lh) == vx(v_rh));
	vy(v_out) = (vy(v_lh) == vy(v_rh));
	vz(v_out) = (vz(v_lh) == vz(v_rh));
	return v_out;
}

CDRAW_INL boolN_t vecNeq3u(bool3_t v_out, uint3_t const v_lh, uint3_t const v_rh)
{
	failassert(v_out && v_lh && v_rh, NULL);
	vx(v_out) = (vx(v_lh) != vx(v_rh));
	vy(v_out) = (vy(v_lh) != vy(v_rh));
	vz(v_out) = (vz(v_lh) != vz(v_rh));
	return v_out;
}

CDRAW_INL boolN_t vecGreater3u(bool3_t v_out, uint3_t const v_lh, uint3_t const v_rh)
{
	failassert(v_out && v_lh && v_rh, NULL);
	vx(v_out) = (vx(v_lh) > vx(v_rh));
	vy(v_out) = (vy(v_lh) > vy(v_rh));
	vz(v_out) = (vz(v_lh) > vz(v_rh));
	return v_out;
}

CDRAW_INL boolN_t vecGreaterEq3u(bool3_t v_out, uint3_t const v_lh, uint3_t const v_rh)
{
	failassert(v_out && v_lh && v_rh, NULL);
	vx(v_out) = (vx(v_lh) >= vx(v_rh));
	vy(v_out) = (vy(v_lh) >= vy(v_rh));
	vz(v_out) = (vz(v_lh) >= vz(v_rh));
	return v_out;
}

CDRAW_INL boolN_t vecLess3u(bool3_t v_out, uint3_t const v_lh, uint3_t const v_rh)
{
	failassert(v_out && v_lh && v_rh, NULL);
	vx(v_out) = (vx(v_lh) < vx(v_rh));
	vy(v_out) = (vy(v_lh) < vy(v_rh));
	vz(v_out) = (vz(v_lh) < vz(v_rh));
	return v_out;
}

CDRAW_INL boolN_t vecLessEq3u(bool3_t v_out, uint3_t const v_lh, uint3_t const v_rh)
{
	failassert(v_out && v_lh && v_rh, NULL);
	vx(v_out) = (vx(v_lh) <= vx(v_rh));
	vy(v_out) = (vy(v_lh) <= vy(v_rh));
	vz(v_out) = (vz(v_lh) <= vz(v_rh));
	return v_out;
}

CDRAW_INL intN_t vecNegate3u(int3_t v_out, uint3_t const v)
{
	failassert(v_out && v, NULL);
	vx(v_out) = -(veci_t)vx(v);
	vy(v_out) = -(veci_t)vy(v);
	vz(v_out) = -(veci_t)vz(v);
	return v_out;
}

CDRAW_INL uintN_t vecAdd3u(uint3_t v_out, uint3_t const v_lh, uint3_t const v_rh)
{
	failassert(v_out && v_lh && v_rh, NULL);
	vx(v_out) = vx(v_lh) + vx(v_rh);
	vy(v_out) = vy(v_lh) + vy(v_rh);
	vz(v_out) = vz(v_lh) + vz(v_rh);
	return v_out;
}

CDRAW_INL intN_t vecSub3u(int3_t v_out, uint3_t const v_lh, uint3_t const v_rh)
{
	failassert(v_out && v_lh && v_rh, NULL);
	vx(v_out) = vx(v_lh) - vx(v_rh);
	vy(v_out) = vy(v_lh) - vy(v_rh);
	vz(v_out) = vz(v_lh) - vz(v_rh);
	return v_out;
}

CDRAW_INL uintN_t vecMul3u(uint3_t v_out, uint3_t const v_lh, uint3_t const v_rh)
{
	failassert(v_out && v_lh && v_rh, NULL);
	vx(v_out) = vx(v_lh) * vx(v_rh);
	vy(v_out) = vy(v_lh) * vy(v_rh);
	vz(v_out) = vz(v_lh) * vz(v_rh);
	return v_out;
}

CDRAW_INL uintN_t vecDiv3u(uint3_t v_out, uint3_t const v_lh, uint3_t const v_rh)
{
	failassert(v_out && v_lh && v_rh, NULL);
	failassert(vx(v_rh) && vy(v_rh) && vz(v_rh), vecInit3u(v_out,
		gDivSafe(vx(v_lh), vx(v_rh)),
		gDivSafe(vy(v_lh), vy(v_rh)),
		gDivSafe(vz(v_lh), vz(v_rh))));
	vx(v_out) = gDiv(vx(v_lh), vx(v_rh));
	vy(v_out) = gDiv(vy(v_lh), vy(v_rh));
	vz(v_out) = gDiv(vz(v_lh), vz(v_rh));
	return v_out;
}

CDRAW_INL uintN_t vecMod3u(uint3_t v_out, uint3_t const v_lh, uint3_t const v_rh)
{
	failassert(v_out && v_lh && v_rh, NULL);
	failassert(vx(v_rh) && vy(v_rh) && vz(v_rh), vecInit3u(v_out,
		gModSafe(vx(v_lh), vx(v_rh)),
		gModSafe(vy(v_lh), vy(v_rh)),
		gModSafe(vz(v_lh), vz(v_rh))));
	vx(v_out) = gMod(vx(v_lh), vx(v_rh));
	vy(v_out) = gMod(vy(v_lh), vy(v_rh));
	vz(v_out) = gMod(vz(v_lh), vz(v_rh));
	return v_out;
}

CDRAW_INL uintN_t vecDivMod3u(uint3_t v_out, uint3_t v_mod_out, uint3_t const v_lh, uint3_t const v_rh)
{
	failassert(v_out && v_mod_out && v_lh && v_rh, NULL);
	failassert(vx(v_rh) && vy(v_rh) && vz(v_rh), vecInit3u(v_out,
		gDivModSafe(vx(v_mod_out), vx(v_lh), vx(v_rh)),
		gDivModSafe(vy(v_mod_out), vy(v_lh), vy(v_rh)),
		gDivModSafe(vz(v_mod_out), vz(v_lh), vz(v_rh))));
	vx(v_out) = gDivMod(vx(v_mod_out), vx(v_lh), vx(v_rh));
	vy(v_out) = gDivMod(vy(v_mod_out), vy(v_lh), vy(v_rh));
	vz(v_out) = gDivMod(vz(v_mod_out), vz(v_lh), vz(v_rh));
	return v_out;
}

CDRAW_INL uintN_t vecAddS3u(uint3_t v_out, uint3_t const v_lh, vecu_t const s_rh)
{
	failassert(v_out && v_lh, NULL);
	vx(v_out) = vx(v_lh) + s_rh;
	vy(v_out) = vy(v_lh) + s_rh;
	vz(v_out) = vz(v_lh) + s_rh;
	return v_out;
}

CDRAW_INL intN_t vecSubS3u(int3_t v_out, uint3_t const v_lh, vecu_t const s_rh)
{
	failassert(v_out && v_lh, NULL);
	vx(v_out) = vx(v_lh) - s_rh;
	vy(v_out) = vy(v_lh) - s_rh;
	vz(v_out) = vz(v_lh) - s_rh;
	return v_out;
}

CDRAW_INL uintN_t vecMulS3u(uint3_t v_out, uint3_t const v_lh, vecu_t const s_rh)
{
	failassert(v_out && v_lh, NULL);
	vx(v_out) = vx(v_lh) * s_rh;
	vy(v_out) = vy(v_lh) * s_rh;
	vz(v_out) = vz(v_lh) * s_rh;
	return v_out;
}

CDRAW_INL uintN_t vecDivS3u(uint3_t v_out, uint3_t const v_lh, vecu_t const s_rh)
{
	failassert(v_out && v_lh, NULL);
	failassert(s_rh, vecZero3u(v_out));
	vx(v_out) = gDiv(vx(v_lh), s_rh);
	vy(v_out) = gDiv(vy(v_lh), s_rh);
	vz(v_out) = gDiv(vz(v_lh), s_rh);
	return v_out;
}

CDRAW_INL uintN_t vecModS3u(uint3_t v_out, uint3_t const v_lh, vecu_t const s_rh)
{
	failassert(v_out && v_lh, NULL);
	failassert(s_rh, vecCopy3u(v_out, v_lh));
	vx(v_out) = gMod(vx(v_lh), s_rh);
	vy(v_out) = gMod(vy(v_lh), s_rh);
	vz(v_out) = gMod(vz(v_lh), s_rh);
	return v_out;
}

CDRAW_INL uintN_t vecDivModS3u(uint3_t v_out, uint3_t v_mod_out, uint3_t const v_lh, vecu_t const s_rh)
{
	failassert(v_out && v_mod_out && v_lh, NULL);
	failassert(s_rh, vecCopy3u(v_mod_out, v_lh), vecZero3u(v_out));
	vx(v_out) = gDivMod(vx(v_mod_out), vx(v_lh), s_rh);
	vy(v_out) = gDivMod(vy(v_mod_out), vy(v_lh), s_rh);
	vz(v_out) = gDivMod(vz(v_mod_out), vz(v_lh), s_rh);
	return v_out;
}

CDRAW_INL vecu_t vecDot3u(uint3_t const v_lh, uint3_t const v_rh)
{
	failassert(v_lh && v_rh, 0);
	return (vx(v_lh) * vx(v_rh) + vy(v_lh) * vy(v_rh) + vz(v_lh) * vz(v_rh));
}

CDRAW_INL intN_t vecCross3u(int3_t v_out, uint3_t const v_lh, uint3_t const v_rh)
{
	failassert(v_out && v_lh && v_rh, NULL);
	vx(v_out) = (vy(v_lh) * vz(v_rh) - vz(v_lh) * vy(v_rh));
	vy(v_out) = (vz(v_lh) * vx(v_rh) - vx(v_lh) * vz(v_rh));
	vz(v_out) = (vx(v_lh) * vy(v_rh) - vy(v_lh) * vx(v_rh));
	return v_out;
}

CDRAW_INL vecu_t vecLenSq3u(uint3_t const v)
{
	failassert(v, 0);
	return (gSq(vx(v)) + gSq(vy(v)) + gSq(vz(v)));
}

CDRAW_INL vecu_t vecDistSq3u(uint3_t const v_lh, uint3_t const v_rh)
{
	failassert(v_lh && v_rh, 0);
	int3_t const disp = {
		(vx(v_lh) - vx(v_rh)),
		(vy(v_lh) - vy(v_rh)),
		(vz(v_lh) - vz(v_rh)),
	};
	return (gSq(vx(disp)) + gSq(vy(disp)) + gSq(vz(disp)));
}

CDRAW_INL vecu_t vecDispDistSq3u(int3_t v_disp_out, uint3_t const v_lh, uint3_t const v_rh)
{
	failassert(v_disp_out && v_lh && v_rh, 0);
	vx(v_disp_out) = (vx(v_lh) - vx(v_rh));
	vy(v_disp_out) = (vy(v_lh) - vy(v_rh));
	vz(v_disp_out) = (vz(v_lh) - vz(v_rh));
	return (gSq(vx(v_disp_out)) + gSq(vy(v_disp_out)) + gSq(vz(v_disp_out)));
}

CDRAW_INL vecb_t vecIsZero3u(uint3_t const v)
{
	return ((vx(v) == 0) && (vy(v) == 0) && (vz(v) == 0));
}

CDRAW_INL vecb_t vecIsNonZero3u(uint3_t const v)
{
	return ((vx(v) != 0) || (vy(v) != 0) || (vz(v) != 0));
}

CDRAW_INL uintN_t vecMad3u(uint3_t v_out, vecu_t const u, uint3_t const v_origin, uint3_t const v_delta)
{
	failassert(v_out && v_origin && v_delta, NULL);
	vx(v_out) = gMad(vx(v_origin), vx(v_delta), u);
	vy(v_out) = gMad(vy(v_origin), vy(v_delta), u);
	vz(v_out) = gMad(vz(v_origin), vz(v_delta), u);
	return v_out;
}

CDRAW_INL uintN_t vecLerp3u(uint3_t v_out, vecu_t const u, uint3_t const v_min, uint3_t const v_max)
{
	failassert(v_out && v_min && v_max, NULL);
	vx(v_out) = gLerp(vx(v_min), vx(v_max), u);
	vy(v_out) = gLerp(vy(v_min), vy(v_max), u);
	vz(v_out) = gLerp(vz(v_min), vz(v_max), u);
	return v_out;
}


#endif // #if (!(defined _CDRAW_VEC3U_INL_) && (defined _CDRAW_VECTOR_INL_))