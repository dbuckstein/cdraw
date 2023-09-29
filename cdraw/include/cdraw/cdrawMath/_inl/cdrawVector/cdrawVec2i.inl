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
* cdrawVec2i.inl
* Definitions of integer 2D vector functions.
*/

#if (!(defined _CDRAW_VEC2I_INL_) && (defined _CDRAW_VECTOR_INL_))
#define _CDRAW_VEC2I_INL_


CDRAW_INL intN_t vecZero2i(int2_t v_out)
{
	failassert(v_out, NULL);
	vx(v_out) = 0;
	vy(v_out) = 0;
	return v_out;
}

CDRAW_INL intN_t vecInit2i(int2_t v_out, veci_t const x, veci_t const y)
{
	failassert(v_out, NULL);
	vx(v_out) = x;
	vy(v_out) = y;
	return v_out;
}

CDRAW_INL intN_t vecCopy2i(int2_t v_out, int2_t const v)
{
	failassert(v_out && v, NULL);
	vx(v_out) = vx(v);
	vy(v_out) = vy(v);
	return v_out;
}

CDRAW_INL intN_t vecCopy2b2i(int2_t v_out, bool2_t const v)
{
	failassert(v_out && v, NULL);
	vx(v_out) = (vx(v) ? 1 : 0);
	vy(v_out) = (vy(v) ? 1 : 0);
	return v_out;
}

CDRAW_INL intN_t vecCopy2u2i(int2_t v_out, uint2_t const v)
{
	failassert(v_out && v, NULL);
	vx(v_out) = (veci_t)(vx(v));
	vy(v_out) = (veci_t)(vy(v));
	return v_out;
}

CDRAW_INL intN_t vecCopy2f2i(int2_t v_out, float2_t const v)
{
	failassert(v_out && v, NULL);
	vx(v_out) = (veci_t)(vx(v));
	vy(v_out) = (veci_t)(vy(v));
	return v_out;
}

CDRAW_INL intN_t vecCopy2d2i(int2_t v_out, double2_t const v)
{
	failassert(v_out && v, NULL);
	vx(v_out) = (veci_t)(vx(v));
	vy(v_out) = (veci_t)(vy(v));
	return v_out;
}

CDRAW_INL intN_t vecNot2i(int2_t v_out, int2_t const v)
{
	failassert(v_out && v, NULL);
	vx(v_out) = ~vx(v);
	vy(v_out) = ~vy(v);
	return v_out;
}

CDRAW_INL intN_t vecAnd2i(int2_t v_out, int2_t const v_lh, int2_t const v_rh)
{
	failassert(v_out && v_lh && v_rh, NULL);
	vx(v_out) = vx(v_lh) & vx(v_rh);
	vy(v_out) = vy(v_lh) & vy(v_rh);
	return v_out;
}

CDRAW_INL intN_t vecNand2i(int2_t v_out, int2_t const v_lh, int2_t const v_rh)
{
	failassert(v_out && v_lh && v_rh, NULL);
	vx(v_out) = ~(vx(v_lh) & vx(v_rh));
	vy(v_out) = ~(vy(v_lh) & vy(v_rh));
	return v_out;
}

CDRAW_INL intN_t vecOr2i(int2_t v_out, int2_t const v_lh, int2_t const v_rh)
{
	failassert(v_out && v_lh && v_rh, NULL);
	vx(v_out) = vx(v_lh) | vx(v_rh);
	vy(v_out) = vy(v_lh) | vy(v_rh);
	return v_out;
}

CDRAW_INL intN_t vecNor2i(int2_t v_out, int2_t const v_lh, int2_t const v_rh)
{
	failassert(v_out && v_lh && v_rh, NULL);
	vx(v_out) = ~(vx(v_lh) | vx(v_rh));
	vy(v_out) = ~(vy(v_lh) | vy(v_rh));
	return v_out;
}

CDRAW_INL intN_t vecXor2i(int2_t v_out, int2_t const v_lh, int2_t const v_rh)
{
	failassert(v_out && v_lh && v_rh, NULL);
	vx(v_out) = vx(v_lh) ^ vx(v_rh);
	vy(v_out) = vy(v_lh) ^ vy(v_rh);
	return v_out;
}

CDRAW_INL intN_t vecXnor2i(int2_t v_out, int2_t const v_lh, int2_t const v_rh)
{
	failassert(v_out && v_lh && v_rh, NULL);
	vx(v_out) = ~(vx(v_lh) ^ vx(v_rh));
	vy(v_out) = ~(vy(v_lh) ^ vy(v_rh));
	return v_out;
}

CDRAW_INL boolN_t vecEq2i(bool2_t v_out, int2_t const v_lh, int2_t const v_rh)
{
	failassert(v_out && v_lh && v_rh, NULL);
	vx(v_out) = (vx(v_lh) == vx(v_rh));
	vy(v_out) = (vy(v_lh) == vy(v_rh));
	return v_out;
}


CDRAW_INL boolN_t vecNeq2i(bool2_t v_out, int2_t const v_lh, int2_t const v_rh)
{
	failassert(v_out && v_lh && v_rh, NULL);
	vx(v_out) = (vx(v_lh) != vx(v_rh));
	vy(v_out) = (vy(v_lh) != vy(v_rh));
	return v_out;
}


#endif // #if (!(defined _CDRAW_VEC2I_INL_) && (defined _CDRAW_VECTOR_INL_))