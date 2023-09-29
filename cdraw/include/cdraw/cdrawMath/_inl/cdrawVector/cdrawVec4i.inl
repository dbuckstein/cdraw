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


#endif // #if (!(defined _CDRAW_VEC4I_INL_) && (defined _CDRAW_VECTOR_INL_))