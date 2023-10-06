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
* cdrawVec3b.inl
* Definitions of boolean 3D vector functions.
*/

#if (!(defined _CDRAW_VEC3B_INL_) && (defined _CDRAW_VECTOR_INL_))
#define _CDRAW_VEC3B_INL_


CDRAW_INL boolN_t vecZero3b(bool3_t v_out)
{
	failassert(v_out, NULL);
	vx(v_out) = false;
	vy(v_out) = false;
	vz(v_out) = false;
	return v_out;
}

CDRAW_INL boolN_t vecInit3b(bool3_t v_out, vecb_t const x, vecb_t const y, vecb_t const z)
{
	failassert(v_out, NULL);
	vx(v_out) = x;
	vy(v_out) = y;
	vz(v_out) = z;
	return v_out;
}

CDRAW_INL boolN_t vecInitS3b(bool3_t v_out, vecb_t const x)
{
	failassert(v_out, NULL);
	vx(v_out) = x;
	vy(v_out) = x;
	vz(v_out) = x;
	return v_out;
}

CDRAW_INL boolN_t vecCopy2z3b(bool3_t v_out, bool2_t const xy, vecb_t const z)
{
	failassert(v_out && xy, NULL);
	vx(v_out) = vx(xy);
	vy(v_out) = vy(xy);
	vz(v_out) = z;
	return v_out;
}

CDRAW_INL boolN_t vecCopy3b(bool3_t v_out, bool3_t const v)
{
	failassert(v_out && v, NULL);
	vx(v_out) = vx(v);
	vy(v_out) = vy(v);
	vz(v_out) = vz(v);
	return v_out;
}

CDRAW_INL boolN_t vecCopy3i3b(bool3_t v_out, int3_t const v)
{
	failassert(v_out && v, NULL);
	vx(v_out) = (vx(v) != 0);
	vy(v_out) = (vy(v) != 0);
	vz(v_out) = (vz(v) != 0);
	return v_out;
}

CDRAW_INL boolN_t vecCopy3u3b(bool3_t v_out, uint3_t const v)
{
	failassert(v_out && v, NULL);
	vx(v_out) = (vx(v) != 0u);
	vy(v_out) = (vy(v) != 0u);
	vz(v_out) = (vz(v) != 0u);
	return v_out;
}

CDRAW_INL boolN_t vecCopy3f3b(bool3_t v_out, float3_t const v)
{
	failassert(v_out && v, NULL);
	vx(v_out) = scIsNonZeroApproxF(vx(v));
	vy(v_out) = scIsNonZeroApproxF(vy(v));
	vz(v_out) = scIsNonZeroApproxF(vz(v));
	return v_out;
}

CDRAW_INL boolN_t vecCopy3d3b(bool3_t v_out, double3_t const v)
{
	failassert(v_out && v, NULL);
	vx(v_out) = scIsNonZeroApproxD(vx(v));
	vy(v_out) = scIsNonZeroApproxD(vy(v));
	vz(v_out) = scIsNonZeroApproxD(vz(v));
	return v_out;
}

CDRAW_INL boolN_t vecNot3b(bool3_t v_out, bool3_t const v)
{
	failassert(v_out && v, NULL);
	vx(v_out) = !vx(v);
	vy(v_out) = !vy(v);
	vz(v_out) = !vz(v);
	return v_out;
}

CDRAW_INL boolN_t vecAnd3b(bool3_t v_out, bool3_t const v_lh, bool3_t const v_rh)
{
	failassert(v_out && v_lh && v_rh, NULL);
	vx(v_out) = vx(v_lh) && vx(v_rh);
	vy(v_out) = vy(v_lh) && vy(v_rh);
	vz(v_out) = vz(v_lh) && vz(v_rh);
	return v_out;
}

CDRAW_INL boolN_t vecNand3b(bool3_t v_out, bool3_t const v_lh, bool3_t const v_rh)
{
	failassert(v_out && v_lh && v_rh, NULL);
	vx(v_out) = !(vx(v_lh) && vx(v_rh));
	vy(v_out) = !(vy(v_lh) && vy(v_rh));
	vz(v_out) = !(vz(v_lh) && vz(v_rh));
	return v_out;
}

CDRAW_INL boolN_t vecOr3b(bool3_t v_out, bool3_t const v_lh, bool3_t const v_rh)
{
	failassert(v_out && v_lh && v_rh, NULL);
	vx(v_out) = vx(v_lh) || vx(v_rh);
	vy(v_out) = vy(v_lh) || vy(v_rh);
	vz(v_out) = vz(v_lh) || vz(v_rh);
	return v_out;
}

CDRAW_INL boolN_t vecNor3b(bool3_t v_out, bool3_t const v_lh, bool3_t const v_rh)
{
	failassert(v_out && v_lh && v_rh, NULL);
	vx(v_out) = !(vx(v_lh) || vx(v_rh));
	vy(v_out) = !(vy(v_lh) || vy(v_rh));
	vz(v_out) = !(vz(v_lh) || vz(v_rh));
	return v_out;
}

CDRAW_INL boolN_t vecXor3b(bool3_t v_out, bool3_t const v_lh, bool3_t const v_rh)
{
	failassert(v_out && v_lh && v_rh, NULL);
	vx(v_out) = (vx(v_lh) && !vx(v_rh)) || (!vx(v_lh) && vx(v_rh));
	vy(v_out) = (vy(v_lh) && !vy(v_rh)) || (!vy(v_lh) && vy(v_rh));
	vz(v_out) = (vz(v_lh) && !vz(v_rh)) || (!vz(v_lh) && vz(v_rh));
	return v_out;
}

CDRAW_INL boolN_t vecXnor3b(bool3_t v_out, bool3_t const v_lh, bool3_t const v_rh)
{
	failassert(v_out && v_lh && v_rh, NULL);
	vx(v_out) = !((vx(v_lh) && !vx(v_rh)) || (!vx(v_lh) && vx(v_rh)));
	vy(v_out) = !((vy(v_lh) && !vy(v_rh)) || (!vy(v_lh) && vy(v_rh)));
	vz(v_out) = !((vz(v_lh) && !vz(v_rh)) || (!vz(v_lh) && vz(v_rh)));
	return v_out;
}

CDRAW_INL boolN_t vecEq3b(bool3_t v_out, bool3_t const v_lh, bool3_t const v_rh)
{
	failassert(v_out && v_lh && v_rh, NULL);
	vx(v_out) = (vx(v_lh) == vx(v_rh));
	vy(v_out) = (vy(v_lh) == vy(v_rh));
	vz(v_out) = (vz(v_lh) == vz(v_rh));
	return v_out;
}

CDRAW_INL boolN_t vecNeq3b(bool3_t v_out, bool3_t const v_lh, bool3_t const v_rh)
{
	failassert(v_out && v_lh && v_rh, NULL);
	vx(v_out) = (vx(v_lh) != vx(v_rh));
	vy(v_out) = (vy(v_lh) != vy(v_rh));
	vz(v_out) = (vz(v_lh) != vz(v_rh));
	return v_out;
}


#endif // #if (!(defined _CDRAW_VEC3B_INL_) && (defined _CDRAW_VECTOR_INL_))