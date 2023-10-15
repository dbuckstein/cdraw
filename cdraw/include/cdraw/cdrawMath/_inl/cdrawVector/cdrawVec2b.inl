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
* cdrawVec2b.inl
* Definitions of boolean 2D vector functions.
*/

#if (!(defined _CDRAW_VEC2B_INL_) && (defined _CDRAW_VECTOR_INL_))
#define _CDRAW_VEC2B_INL_


CDRAW_INL boolN_t vecZero2b(bool2_t v_out)
{
	failassert(v_out, NULL);
	vx(v_out) = false;
	vy(v_out) = false;
	return v_out;
}

CDRAW_INL boolN_t vecInit2b(bool2_t v_out, vecb_t const x, vecb_t const y)
{
	failassert(v_out, NULL);
	vx(v_out) = x;
	vy(v_out) = y;
	return v_out;
}

CDRAW_INL boolN_t vecInitS2b(bool2_t v_out, vecb_t const x)
{
	failassert(v_out, NULL);
	vx(v_out) = x;
	vy(v_out) = x;
	return v_out;
}

CDRAW_INL boolN_t vecCopy2b(bool2_t v_out, bool2_t const v)
{
	failassert(v_out && v, NULL);
	vx(v_out) = vx(v);
	vy(v_out) = vy(v);
	return v_out;
}

CDRAW_INL boolN_t vecCopy2i2b(bool2_t v_out, int2_t const v)
{
	failassert(v_out && v, NULL);
	vx(v_out) = (vx(v) != 0);
	vy(v_out) = (vy(v) != 0);
	return v_out;
}

CDRAW_INL boolN_t vecCopy2u2b(bool2_t v_out, uint2_t const v)
{
	failassert(v_out && v, NULL);
	vx(v_out) = (vx(v) != 0u);
	vy(v_out) = (vy(v) != 0u);
	return v_out;
}

CDRAW_INL boolN_t vecCopy2f2b(bool2_t v_out, float2_t const v)
{
	failassert(v_out && v, NULL);
	vx(v_out) = scIsNonZeroApproxF(vx(v));
	vy(v_out) = scIsNonZeroApproxF(vy(v));
	return v_out;
}

CDRAW_INL boolN_t vecCopy2d2b(bool2_t v_out, double2_t const v)
{
	failassert(v_out && v, NULL);
	vx(v_out) = scIsNonZeroApproxD(vx(v));
	vy(v_out) = scIsNonZeroApproxD(vy(v));
	return v_out;
}

CDRAW_INL boolN_t vecNot2b(bool2_t v_out, bool2_t const v)
{
	failassert(v_out && v, NULL);
	vx(v_out) = !vx(v);
	vy(v_out) = !vy(v);
	return v_out;
}

CDRAW_INL boolN_t vecAnd2b(bool2_t v_out, bool2_t const v_lh, bool2_t const v_rh)
{
	failassert(v_out && v_lh && v_rh, NULL);
	vx(v_out) = vx(v_lh) && vx(v_rh);
	vy(v_out) = vy(v_lh) && vy(v_rh);
	return v_out;
}

CDRAW_INL boolN_t vecNand2b(bool2_t v_out, bool2_t const v_lh, bool2_t const v_rh)
{
	failassert(v_out && v_lh && v_rh, NULL);
	vx(v_out) = !(vx(v_lh) && vx(v_rh));
	vy(v_out) = !(vy(v_lh) && vy(v_rh));
	return v_out;
}

CDRAW_INL boolN_t vecOr2b(bool2_t v_out, bool2_t const v_lh, bool2_t const v_rh)
{
	failassert(v_out && v_lh && v_rh, NULL);
	vx(v_out) = vx(v_lh) || vx(v_rh);
	vy(v_out) = vy(v_lh) || vy(v_rh);
	return v_out;
}

CDRAW_INL boolN_t vecNor2b(bool2_t v_out, bool2_t const v_lh, bool2_t const v_rh)
{
	failassert(v_out && v_lh && v_rh, NULL);
	vx(v_out) = !(vx(v_lh) || vx(v_rh));
	vy(v_out) = !(vy(v_lh) || vy(v_rh));
	return v_out;
}

CDRAW_INL boolN_t vecXor2b(bool2_t v_out, bool2_t const v_lh, bool2_t const v_rh)
{
	failassert(v_out && v_lh && v_rh, NULL);
	vx(v_out) = (vx(v_lh) && !vx(v_rh)) || (!vx(v_lh) && vx(v_rh));
	vy(v_out) = (vy(v_lh) && !vy(v_rh)) || (!vy(v_lh) && vy(v_rh));
	return v_out;
}

CDRAW_INL boolN_t vecXnor2b(bool2_t v_out, bool2_t const v_lh, bool2_t const v_rh)
{
	failassert(v_out && v_lh && v_rh, NULL);
	vx(v_out) = !((vx(v_lh) && !vx(v_rh)) || (!vx(v_lh) && vx(v_rh)));
	vy(v_out) = !((vy(v_lh) && !vy(v_rh)) || (!vy(v_lh) && vy(v_rh)));
	return v_out;
}

CDRAW_INL boolN_t vecEq2b(bool2_t v_out, bool2_t const v_lh, bool2_t const v_rh)
{
	failassert(v_out && v_lh && v_rh, NULL);
	vx(v_out) = (vx(v_lh) == vx(v_rh));
	vy(v_out) = (vy(v_lh) == vy(v_rh));
	return v_out;
}

CDRAW_INL boolN_t vecNeq2b(bool2_t v_out, bool2_t const v_lh, bool2_t const v_rh)
{
	failassert(v_out && v_lh && v_rh, NULL);
	vx(v_out) = (vx(v_lh) != vx(v_rh));
	vy(v_out) = (vy(v_lh) != vy(v_rh));
	return v_out;
}


#endif // #if (!(defined _CDRAW_VEC2B_INL_) && (defined _CDRAW_VECTOR_INL_))