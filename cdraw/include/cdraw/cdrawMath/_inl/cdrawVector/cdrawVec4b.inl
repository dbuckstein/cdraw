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
* cdrawVec4b.inl
* Definitions of boolean 4D vector functions.
*/

#if (!(defined _CDRAW_VEC4B_INL_) && (defined _CDRAW_VECTOR_INL_))
#define _CDRAW_VEC4B_INL_


CDRAW_INL boolN_t vecZero4b(bool4_t v_out)
{
	failassert(v_out, NULL);
	vx(v_out) = false;
	vy(v_out) = false;
	vz(v_out) = false;
	vw(v_out) = false;
	return v_out;
}

CDRAW_INL boolN_t vecInit4b(bool4_t v_out, vecb_t const x, vecb_t const y, vecb_t const z, vecb_t const w)
{
	failassert(v_out, NULL);
	vx(v_out) = x;
	vy(v_out) = y;
	vz(v_out) = z;
	vw(v_out) = w;
	return v_out;
}

CDRAW_INL boolN_t vecCopy2zw4b(bool4_t v_out, bool2_t const xy, vecb_t const z, vecb_t const w)
{
	failassert(v_out && xy, NULL);
	vx(v_out) = vx(xy);
	vy(v_out) = vy(xy);
	vz(v_out) = z;
	vw(v_out) = w;
	return v_out;
}

CDRAW_INL boolN_t vecCopy3w4b(bool4_t v_out, bool3_t const xyz, vecb_t const w)
{
	failassert(v_out && xyz, NULL);
	vx(v_out) = vx(xyz);
	vy(v_out) = vy(xyz);
	vz(v_out) = vz(xyz);
	vw(v_out) = w;
	return v_out;
}

CDRAW_INL boolN_t vecCopy4b(bool4_t v_out, bool4_t const v)
{
	failassert(v_out && v, NULL);
	vx(v_out) = vx(v);
	vy(v_out) = vy(v);
	vz(v_out) = vz(v);
	vw(v_out) = vw(v);
	return v_out;
}

CDRAW_INL boolN_t vecCopy4i4b(bool4_t v_out, int4_t const v)
{
	failassert(v_out && v, NULL);
	vx(v_out) = (vx(v) != 0);
	vy(v_out) = (vy(v) != 0);
	vz(v_out) = (vz(v) != 0);
	vw(v_out) = (vw(v) != 0);
	return v_out;
}

CDRAW_INL boolN_t vecCopy4u4b(bool4_t v_out, uint4_t const v)
{
	failassert(v_out && v, NULL);
	vx(v_out) = (vx(v) != 0u);
	vy(v_out) = (vy(v) != 0u);
	vz(v_out) = (vz(v) != 0u);
	vw(v_out) = (vw(v) != 0u);
	return v_out;
}

CDRAW_INL boolN_t vecCopy4f4b(bool4_t v_out, float4_t const v)
{
	failassert(v_out && v, NULL);
	vx(v_out) = scIsNonZeroApproxF(vx(v));
	vy(v_out) = scIsNonZeroApproxF(vy(v));
	vz(v_out) = scIsNonZeroApproxF(vz(v));
	vw(v_out) = scIsNonZeroApproxF(vw(v));
	return v_out;
}

CDRAW_INL boolN_t vecCopy4d4b(bool4_t v_out, double4_t const v)
{
	failassert(v_out && v, NULL);
	vx(v_out) = scIsNonZeroApproxD(vx(v));
	vy(v_out) = scIsNonZeroApproxD(vy(v));
	vz(v_out) = scIsNonZeroApproxD(vz(v));
	vw(v_out) = scIsNonZeroApproxD(vw(v));
	return v_out;
}

CDRAW_INL boolN_t vecNot4b(bool4_t v_out, bool4_t const v)
{
	failassert(v_out && v, NULL);
	vx(v_out) = !vx(v);
	vy(v_out) = !vy(v);
	vz(v_out) = !vz(v);
	vw(v_out) = !vw(v);
	return v_out;
}

CDRAW_INL boolN_t vecAnd4b(bool4_t v_out, bool4_t const v_lh, bool4_t const v_rh)
{
	failassert(v_out && v_lh && v_rh, NULL);
	vx(v_out) = vx(v_lh) && vx(v_rh);
	vy(v_out) = vy(v_lh) && vy(v_rh);
	vz(v_out) = vz(v_lh) && vz(v_rh);
	vw(v_out) = vw(v_lh) && vw(v_rh);
	return v_out;
}

CDRAW_INL boolN_t vecNand4b(bool4_t v_out, bool4_t const v_lh, bool4_t const v_rh)
{
	failassert(v_out && v_lh && v_rh, NULL);
	vx(v_out) = !(vx(v_lh) && vx(v_rh));
	vy(v_out) = !(vy(v_lh) && vy(v_rh));
	vz(v_out) = !(vz(v_lh) && vz(v_rh));
	vw(v_out) = !(vw(v_lh) && vw(v_rh));
	return v_out;
}

CDRAW_INL boolN_t vecOr4b(bool4_t v_out, bool4_t const v_lh, bool4_t const v_rh)
{
	failassert(v_out && v_lh && v_rh, NULL);
	vx(v_out) = vx(v_lh) || vx(v_rh);
	vy(v_out) = vy(v_lh) || vy(v_rh);
	vz(v_out) = vz(v_lh) || vz(v_rh);
	vw(v_out) = vw(v_lh) || vw(v_rh);
	return v_out;
}

CDRAW_INL boolN_t vecNor4b(bool4_t v_out, bool4_t const v_lh, bool4_t const v_rh)
{
	failassert(v_out && v_lh && v_rh, NULL);
	vx(v_out) = !(vx(v_lh) || vx(v_rh));
	vy(v_out) = !(vy(v_lh) || vy(v_rh));
	vz(v_out) = !(vz(v_lh) || vz(v_rh));
	vw(v_out) = !(vw(v_lh) || vw(v_rh));
	return v_out;
}

CDRAW_INL boolN_t vecXor4b(bool4_t v_out, bool4_t const v_lh, bool4_t const v_rh)
{
	failassert(v_out && v_lh && v_rh, NULL);
	vx(v_out) = (vx(v_lh) && !vx(v_rh)) || (!vx(v_lh) && vx(v_rh));
	vy(v_out) = (vy(v_lh) && !vy(v_rh)) || (!vy(v_lh) && vy(v_rh));
	vz(v_out) = (vz(v_lh) && !vz(v_rh)) || (!vz(v_lh) && vz(v_rh));
	vw(v_out) = (vw(v_lh) && !vw(v_rh)) || (!vw(v_lh) && vw(v_rh));
	return v_out;
}

CDRAW_INL boolN_t vecNxor4b(bool4_t v_out, bool4_t const v_lh, bool4_t const v_rh)
{
	failassert(v_out && v_lh && v_rh, NULL);
	vx(v_out) = !((vx(v_lh) && !vx(v_rh)) || (!vx(v_lh) && vx(v_rh)));
	vy(v_out) = !((vy(v_lh) && !vy(v_rh)) || (!vy(v_lh) && vy(v_rh)));
	vz(v_out) = !((vz(v_lh) && !vz(v_rh)) || (!vz(v_lh) && vz(v_rh)));
	vw(v_out) = !((vw(v_lh) && !vw(v_rh)) || (!vw(v_lh) && vw(v_rh)));
	return v_out;
}

CDRAW_INL boolN_t vecEq4b(bool4_t v_out, bool4_t const v_lh, bool4_t const v_rh)
{
	failassert(v_out && v_lh && v_rh, NULL);
	vx(v_out) = (vx(v_lh) == vx(v_rh));
	vy(v_out) = (vy(v_lh) == vy(v_rh));
	vz(v_out) = (vz(v_lh) == vz(v_rh));
	vw(v_out) = (vw(v_lh) == vw(v_rh));
	return v_out;
}


CDRAW_INL boolN_t vecNeq4b(bool4_t v_out, bool4_t const v_lh, bool4_t const v_rh)
{
	failassert(v_out && v_lh && v_rh, NULL);
	vx(v_out) = (vx(v_lh) != vx(v_rh));
	vy(v_out) = (vy(v_lh) != vy(v_rh));
	vz(v_out) = (vz(v_lh) != vz(v_rh));
	vw(v_out) = (vw(v_lh) != vw(v_rh));
	return v_out;
}


#endif // #if (!(defined _CDRAW_VEC4B_INL_) && (defined _CDRAW_VECTOR_INL_))