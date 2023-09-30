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
* cdrawVec3i.inl
* Definitions of integer 3D vector functions.
*/

#if (!(defined _CDRAW_VEC3I_INL_) && (defined _CDRAW_VECTOR_INL_))
#define _CDRAW_VEC3I_INL_


CDRAW_INL intN_t vecZero3i(int3_t v_out)
{
	failassert(v_out, NULL);
	vx(v_out) = 0;
	vy(v_out) = 0;
	vz(v_out) = 0;
	return v_out;
}

CDRAW_INL intN_t vecInit3i(int3_t v_out, veci_t const x, veci_t const y, veci_t const z)
{
	failassert(v_out, NULL);
	vx(v_out) = x;
	vy(v_out) = y;
	vz(v_out) = z;
	return v_out;
}

CDRAW_INL intN_t vecInitS3i(int3_t v_out, veci_t const x)
{
	failassert(v_out, NULL);
	vx(v_out) = x;
	vy(v_out) = x;
	vz(v_out) = x;
	return v_out;
}

CDRAW_INL intN_t vecCopy2z3i(int3_t v_out, int2_t const xy, veci_t const z)
{
	failassert(v_out && xy, NULL);
	vx(v_out) = vx(xy);
	vy(v_out) = vy(xy);
	vz(v_out) = z;
	return v_out;
}

CDRAW_INL intN_t vecCopy3i(int3_t v_out, int3_t const v)
{
	failassert(v_out && v, NULL);
	vx(v_out) = vx(v);
	vy(v_out) = vy(v);
	vz(v_out) = vz(v);
	return v_out;
}

CDRAW_INL intN_t vecCopy3b3i(int3_t v_out, bool3_t const v)
{
	failassert(v_out && v, NULL);
	vx(v_out) = (vx(v) ? 1 : 0);
	vy(v_out) = (vy(v) ? 1 : 0);
	vz(v_out) = (vz(v) ? 1 : 0);
	return v_out;
}

CDRAW_INL intN_t vecCopy3u3i(int3_t v_out, uint3_t const v)
{
	failassert(v_out && v, NULL);
	vx(v_out) = (veci_t)(vx(v));
	vy(v_out) = (veci_t)(vy(v));
	vz(v_out) = (veci_t)(vz(v));
	return v_out;
}

CDRAW_INL intN_t vecCopy3f3i(int3_t v_out, float3_t const v)
{
	failassert(v_out && v, NULL);
	vx(v_out) = (veci_t)(vx(v));
	vy(v_out) = (veci_t)(vy(v));
	vz(v_out) = (veci_t)(vz(v));
	return v_out;
}

CDRAW_INL intN_t vecCopy3d3i(int3_t v_out, double3_t const v)
{
	failassert(v_out && v, NULL);
	vx(v_out) = (veci_t)(vx(v));
	vy(v_out) = (veci_t)(vy(v));
	vz(v_out) = (veci_t)(vz(v));
	return v_out;
}

CDRAW_INL intN_t vecNot3i(int3_t v_out, int3_t const v)
{
	failassert(v_out && v, NULL);
	vx(v_out) = ~vx(v);
	vy(v_out) = ~vy(v);
	vz(v_out) = ~vz(v);
	return v_out;
}

CDRAW_INL intN_t vecAnd3i(int3_t v_out, int3_t const v_lh, int3_t const v_rh)
{
	failassert(v_out && v_lh && v_rh, NULL);
	vx(v_out) = vx(v_lh) & vx(v_rh);
	vy(v_out) = vy(v_lh) & vy(v_rh);
	vz(v_out) = vz(v_lh) & vz(v_rh);
	return v_out;
}

CDRAW_INL intN_t vecNand3i(int3_t v_out, int3_t const v_lh, int3_t const v_rh)
{
	failassert(v_out && v_lh && v_rh, NULL);
	vx(v_out) = ~(vx(v_lh) & vx(v_rh));
	vy(v_out) = ~(vy(v_lh) & vy(v_rh));
	vz(v_out) = ~(vz(v_lh) & vz(v_rh));
	return v_out;
}

CDRAW_INL intN_t vecOr3i(int3_t v_out, int3_t const v_lh, int3_t const v_rh)
{
	failassert(v_out && v_lh && v_rh, NULL);
	vx(v_out) = vx(v_lh) | vx(v_rh);
	vy(v_out) = vy(v_lh) | vy(v_rh);
	vz(v_out) = vz(v_lh) | vz(v_rh);
	return v_out;
}

CDRAW_INL intN_t vecNor3i(int3_t v_out, int3_t const v_lh, int3_t const v_rh)
{
	failassert(v_out && v_lh && v_rh, NULL);
	vx(v_out) = ~(vx(v_lh) | vx(v_rh));
	vy(v_out) = ~(vy(v_lh) | vy(v_rh));
	vz(v_out) = ~(vz(v_lh) | vz(v_rh));
	return v_out;
}

CDRAW_INL intN_t vecXor3i(int3_t v_out, int3_t const v_lh, int3_t const v_rh)
{
	failassert(v_out && v_lh && v_rh, NULL);
	vx(v_out) = vx(v_lh) ^ vx(v_rh);
	vy(v_out) = vy(v_lh) ^ vy(v_rh);
	vz(v_out) = vz(v_lh) ^ vz(v_rh);
	return v_out;
}

CDRAW_INL intN_t vecXnor3i(int3_t v_out, int3_t const v_lh, int3_t const v_rh)
{
	failassert(v_out && v_lh && v_rh, NULL);
	vx(v_out) = ~(vx(v_lh) ^ vx(v_rh));
	vy(v_out) = ~(vy(v_lh) ^ vy(v_rh));
	vz(v_out) = ~(vz(v_lh) ^ vz(v_rh));
	return v_out;
}

CDRAW_INL boolN_t vecEq3i(bool3_t v_out, int3_t const v_lh, int3_t const v_rh)
{
	failassert(v_out && v_lh && v_rh, NULL);
	vx(v_out) = (vx(v_lh) == vx(v_rh));
	vy(v_out) = (vy(v_lh) == vy(v_rh));
	vz(v_out) = (vz(v_lh) == vz(v_rh));
	return v_out;
}

CDRAW_INL boolN_t vecNeq3i(bool3_t v_out, int3_t const v_lh, int3_t const v_rh)
{
	failassert(v_out && v_lh && v_rh, NULL);
	vx(v_out) = (vx(v_lh) != vx(v_rh));
	vy(v_out) = (vy(v_lh) != vy(v_rh));
	vz(v_out) = (vz(v_lh) != vz(v_rh));
	return v_out;
}

CDRAW_INL boolN_t vecGreater3i(bool3_t v_out, int3_t const v_lh, int3_t const v_rh)
{
	failassert(v_out && v_lh && v_rh, NULL);
	vx(v_out) = (vx(v_lh) > vx(v_rh));
	vy(v_out) = (vy(v_lh) > vy(v_rh));
	vz(v_out) = (vz(v_lh) > vz(v_rh));
	return v_out;
}

CDRAW_INL boolN_t vecGreaterEq3i(bool3_t v_out, int3_t const v_lh, int3_t const v_rh)
{
	failassert(v_out && v_lh && v_rh, NULL);
	vx(v_out) = (vx(v_lh) >= vx(v_rh));
	vy(v_out) = (vy(v_lh) >= vy(v_rh));
	vz(v_out) = (vz(v_lh) >= vz(v_rh));
	return v_out;
}

CDRAW_INL boolN_t vecLess3i(bool3_t v_out, int3_t const v_lh, int3_t const v_rh)
{
	failassert(v_out && v_lh && v_rh, NULL);
	vx(v_out) = (vx(v_lh) < vx(v_rh));
	vy(v_out) = (vy(v_lh) < vy(v_rh));
	vz(v_out) = (vz(v_lh) < vz(v_rh));
	return v_out;
}

CDRAW_INL boolN_t vecLessEq3i(bool3_t v_out, int3_t const v_lh, int3_t const v_rh)
{
	failassert(v_out && v_lh && v_rh, NULL);
	vx(v_out) = (vx(v_lh) <= vx(v_rh));
	vy(v_out) = (vy(v_lh) <= vy(v_rh));
	vz(v_out) = (vz(v_lh) <= vz(v_rh));
	return v_out;
}

CDRAW_INL intN_t vecNegate3i(int3_t v_out, int3_t const v)
{
	failassert(v_out && v, NULL);
	vx(v_out) = -vx(v);
	vy(v_out) = -vy(v);
	vz(v_out) = -vz(v);
	return v_out;
}

CDRAW_INL intN_t vecAdd3i(int3_t v_out, int3_t const v_lh, int3_t const v_rh)
{
	failassert(v_out && v_lh && v_rh, NULL);
	vx(v_out) = vx(v_lh) + vx(v_rh);
	vy(v_out) = vy(v_lh) + vy(v_rh);
	vz(v_out) = vz(v_lh) + vz(v_rh);
	return v_out;
}

CDRAW_INL intN_t vecSub3i(int3_t v_out, int3_t const v_lh, int3_t const v_rh)
{
	failassert(v_out && v_lh && v_rh, NULL);
	vx(v_out) = vx(v_lh) - vx(v_rh);
	vy(v_out) = vy(v_lh) - vy(v_rh);
	vz(v_out) = vz(v_lh) - vz(v_rh);
	return v_out;
}

CDRAW_INL intN_t vecMul3i(int3_t v_out, int3_t const v_lh, int3_t const v_rh)
{
	failassert(v_out && v_lh && v_rh, NULL);
	vx(v_out) = vx(v_lh) * vx(v_rh);
	vy(v_out) = vy(v_lh) * vy(v_rh);
	vz(v_out) = vz(v_lh) * vz(v_rh);
	return v_out;
}

CDRAW_INL intN_t vecDiv3i(int3_t v_out, int3_t const v_lh, int3_t const v_rh)
{
	failassert(v_out && v_lh && v_rh, NULL);
	failassert(vx(v_rh) && vy(v_rh), vecInit3i(v_out,
		gDivSafe(vx(v_lh), vx(v_rh)),
		gDivSafe(vy(v_lh), vy(v_rh)),
		gDivSafe(vz(v_lh), vz(v_rh))));
	vx(v_out) = vx(v_lh) / vx(v_rh);
	vy(v_out) = vy(v_lh) / vy(v_rh);
	vz(v_out) = vz(v_lh) / vz(v_rh);
	return v_out;
}

CDRAW_INL intN_t vecMod3i(int3_t v_out, int3_t const v_lh, int3_t const v_rh)
{
	failassert(v_out && v_lh && v_rh, NULL);
	failassert(vx(v_rh) && vy(v_rh), vecInit3i(v_out,
		gModSafe(vx(v_lh), vx(v_rh)),
		gModSafe(vy(v_lh), vy(v_rh)),
		gModSafe(vz(v_lh), vz(v_rh))));
	vx(v_out) = vx(v_lh) % vx(v_rh);
	vy(v_out) = vy(v_lh) % vy(v_rh);
	vz(v_out) = vz(v_lh) % vz(v_rh);
	return v_out;
}

CDRAW_INL intN_t vecDivMod3i(int3_t v_out, int3_t v_mod_out, int3_t const v_lh, int3_t const v_rh)
{
	failassert(v_out && v_mod_out && v_lh && v_rh, NULL);
	failassert(vx(v_rh) && vy(v_rh), vecInit3i(v_out,
		gDivModSafe(vx(v_mod_out), vx(v_lh), vx(v_rh)),
		gDivModSafe(vy(v_mod_out), vy(v_lh), vy(v_rh)),
		gDivModSafe(vz(v_mod_out), vz(v_lh), vz(v_rh))));
	vx(v_out) = vx(v_lh) / vx(v_rh);
	vx(v_mod_out) = vx(v_lh) % vx(v_rh);
	vy(v_out) = vy(v_lh) / vy(v_rh);
	vy(v_mod_out) = vy(v_lh) % vy(v_rh);
	vz(v_out) = vz(v_lh) / vz(v_rh);
	vz(v_mod_out) = vz(v_lh) % vz(v_rh);
	return v_out;
}

CDRAW_INL intN_t vecAddS3i(int3_t v_out, int3_t const v_lh, veci_t const s_rh)
{
	failassert(v_out && v_lh, NULL);
	vx(v_out) = vx(v_lh) + s_rh;
	vy(v_out) = vy(v_lh) + s_rh;
	vz(v_out) = vz(v_lh) + s_rh;
	return v_out;
}

CDRAW_INL intN_t vecSubS3i(int3_t v_out, int3_t const v_lh, veci_t const s_rh)
{
	failassert(v_out && v_lh, NULL);
	vx(v_out) = vx(v_lh) - s_rh;
	vy(v_out) = vy(v_lh) - s_rh;
	vz(v_out) = vz(v_lh) - s_rh;
	return v_out;
}

CDRAW_INL intN_t vecMulS3i(int3_t v_out, int3_t const v_lh, veci_t const s_rh)
{
	failassert(v_out && v_lh, NULL);
	vx(v_out) = vx(v_lh) * s_rh;
	vy(v_out) = vy(v_lh) * s_rh;
	vz(v_out) = vz(v_lh) * s_rh;
	return v_out;
}

CDRAW_INL intN_t vecDivS3i(int3_t v_out, int3_t const v_lh, veci_t const s_rh)
{
	failassert(v_out && v_lh, NULL);
	failassert(s_rh, vecZero3i(v_out));
	vx(v_out) = vx(v_lh) / s_rh;
	vy(v_out) = vy(v_lh) / s_rh;
	vz(v_out) = vz(v_lh) / s_rh;
	return v_out;
}

CDRAW_INL intN_t vecModS3i(int3_t v_out, int3_t const v_lh, veci_t const s_rh)
{
	failassert(v_out && v_lh, NULL);
	failassert(s_rh, vecCopy3i(v_out, v_lh));
	vx(v_out) = vx(v_lh) % s_rh;
	vy(v_out) = vy(v_lh) % s_rh;
	vz(v_out) = vz(v_lh) % s_rh;
	return v_out;
}

CDRAW_INL intN_t vecDivModS3i(int3_t v_out, int3_t v_mod_out, int3_t const v_lh, veci_t const s_rh)
{
	failassert(v_out && v_lh, NULL);
	failassert(s_rh, vecCopy3i(v_mod_out, v_lh), vecZero3i(v_out));
	vx(v_out) = vx(v_lh) / s_rh;
	vx(v_mod_out) = vx(v_lh) % s_rh;
	vy(v_out) = vy(v_lh) / s_rh;
	vy(v_mod_out) = vy(v_lh) % s_rh;
	vz(v_out) = vz(v_lh) / s_rh;
	vz(v_mod_out) = vz(v_lh) % s_rh;
	return v_out;
}

CDRAW_INL veci_t vecDot3i(int3_t const v_lh, int3_t const v_rh)
{
	failassert(v_lh && v_rh, 0);
	return (vx(v_lh) * vx(v_rh) + vy(v_lh) * vy(v_rh) + vz(v_lh) * vz(v_rh));
}

CDRAW_INL intN_t vecCross3i(int3_t v_out, int3_t const v_lh, int3_t const v_rh)
{
	failassert(v_out && v_lh && v_rh, NULL);
	vx(v_out) = vy(v_lh) * vz(v_rh) - vz(v_lh) * vy(v_rh);
	vy(v_out) = vz(v_lh) * vx(v_rh) - vx(v_lh) * vz(v_rh);
	vz(v_out) = vx(v_lh) * vy(v_rh) - vy(v_lh) * vx(v_rh);
	return v_out;
}

CDRAW_INL veci_t vecLenSq3i(int3_t const v)
{
	failassert(v, 0);
	return (gSq(vx(v)) + gSq(vy(v)) + gSq(vz(v)));
}

CDRAW_INL intN_t vecMad3i(int3_t v_out, veci_t const u, int3_t const v_origin, int3_t const v_delta)
{
	failassert(v_out && v_origin && v_delta, NULL);
	vx(v_out) = gMad(vx(v_origin), vx(v_delta), u);
	vy(v_out) = gMad(vy(v_origin), vy(v_delta), u);
	vz(v_out) = gMad(vz(v_origin), vz(v_delta), u);
	return v_out;
}

CDRAW_INL intN_t vecLerp3i(int3_t v_out, veci_t const u, int3_t const v_min, int3_t const v_max)
{
	failassert(v_out && v_min && v_max, NULL);
	vx(v_out) = gLerp(vx(v_min), vx(v_max), u);
	vy(v_out) = gLerp(vy(v_min), vy(v_max), u);
	vz(v_out) = gLerp(vz(v_min), vz(v_max), u);
	return v_out;
}


#endif // #if (!(defined _CDRAW_VEC3I_INL_) && (defined _CDRAW_VECTOR_INL_))