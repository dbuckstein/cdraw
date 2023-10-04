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

CDRAW_INL intN_t vecInitS2i(int2_t v_out, veci_t const x)
{
	failassert(v_out, NULL);
	vx(v_out) = x;
	vy(v_out) = x;
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

CDRAW_INL boolN_t vecGreater2i(bool2_t v_out, int2_t const v_lh, int2_t const v_rh)
{
	failassert(v_out && v_lh && v_rh, NULL);
	vx(v_out) = (vx(v_lh) > vx(v_rh));
	vy(v_out) = (vy(v_lh) > vy(v_rh));
	return v_out;
}

CDRAW_INL boolN_t vecGreaterEq2i(bool2_t v_out, int2_t const v_lh, int2_t const v_rh)
{
	failassert(v_out && v_lh && v_rh, NULL);
	vx(v_out) = (vx(v_lh) >= vx(v_rh));
	vy(v_out) = (vy(v_lh) >= vy(v_rh));
	return v_out;
}

CDRAW_INL boolN_t vecLess2i(bool2_t v_out, int2_t const v_lh, int2_t const v_rh)
{
	failassert(v_out && v_lh && v_rh, NULL);
	vx(v_out) = (vx(v_lh) < vx(v_rh));
	vy(v_out) = (vy(v_lh) < vy(v_rh));
	return v_out;
}

CDRAW_INL boolN_t vecLessEq2i(bool2_t v_out, int2_t const v_lh, int2_t const v_rh)
{
	failassert(v_out && v_lh && v_rh, NULL);
	vx(v_out) = (vx(v_lh) <= vx(v_rh));
	vy(v_out) = (vy(v_lh) <= vy(v_rh));
	return v_out;
}

CDRAW_INL intN_t vecNegate2i(int2_t v_out, int2_t const v)
{
	failassert(v_out && v, NULL);
	vx(v_out) = -vx(v);
	vy(v_out) = -vy(v);
	return v_out;
}

CDRAW_INL intN_t vecAdd2i(int2_t v_out, int2_t const v_lh, int2_t const v_rh)
{
	failassert(v_out && v_lh && v_rh, NULL);
	vx(v_out) = vx(v_lh) + vx(v_rh);
	vy(v_out) = vy(v_lh) + vy(v_rh);
	return v_out;
}

CDRAW_INL intN_t vecSub2i(int2_t v_out, int2_t const v_lh, int2_t const v_rh)
{
	failassert(v_out && v_lh && v_rh, NULL);
	vx(v_out) = vx(v_lh) - vx(v_rh);
	vy(v_out) = vy(v_lh) - vy(v_rh);
	return v_out;
}

CDRAW_INL intN_t vecMul2i(int2_t v_out, int2_t const v_lh, int2_t const v_rh)
{
	failassert(v_out && v_lh && v_rh, NULL);
	vx(v_out) = vx(v_lh) * vx(v_rh);
	vy(v_out) = vy(v_lh) * vy(v_rh);
	return v_out;
}

CDRAW_INL intN_t vecDiv2i(int2_t v_out, int2_t const v_lh, int2_t const v_rh)
{
	failassert(v_out && v_lh && v_rh, NULL);
	failassert(vx(v_rh) && vy(v_rh), vecInit2i(v_out,
		gDivSafe(vx(v_lh), vx(v_rh)),
		gDivSafe(vy(v_lh), vy(v_rh))));
	vx(v_out) = gDiv(vx(v_lh), vx(v_rh));
	vy(v_out) = gDiv(vy(v_lh), vy(v_rh));
	return v_out;
}

CDRAW_INL intN_t vecMod2i(int2_t v_out, int2_t const v_lh, int2_t const v_rh)
{
	failassert(v_out && v_lh && v_rh, NULL);
	failassert(vx(v_rh) && vy(v_rh), vecInit2i(v_out,
		gModSafe(vx(v_lh), vx(v_rh)),
		gModSafe(vy(v_lh), vy(v_rh))));
	vx(v_out) = gMod(vx(v_lh), vx(v_rh));
	vy(v_out) = gMod(vy(v_lh), vy(v_rh));
	return v_out;
}

CDRAW_INL intN_t vecDivMod2i(int2_t v_out, int2_t v_mod_out, int2_t const v_lh, int2_t const v_rh)
{
	failassert(v_out && v_mod_out && v_lh && v_rh, NULL);
	failassert(vx(v_rh) && vy(v_rh), vecInit2i(v_out,
		gDivModSafe(vx(v_mod_out), vx(v_lh), vx(v_rh)),
		gDivModSafe(vy(v_mod_out), vy(v_lh), vy(v_rh))));
	vx(v_out) = gDivMod(vx(v_mod_out), vx(v_lh), vx(v_rh));
	vy(v_out) = gDivMod(vy(v_mod_out), vy(v_lh), vy(v_rh));
	return v_out;
}

CDRAW_INL intN_t vecAddS2i(int2_t v_out, int2_t const v_lh, veci_t const s_rh)
{
	failassert(v_out && v_lh, NULL);
	vx(v_out) = vx(v_lh) + s_rh;
	vy(v_out) = vy(v_lh) + s_rh;
	return v_out;
}

CDRAW_INL intN_t vecSubS2i(int2_t v_out, int2_t const v_lh, veci_t const s_rh)
{
	failassert(v_out && v_lh, NULL);
	vx(v_out) = vx(v_lh) - s_rh;
	vy(v_out) = vy(v_lh) - s_rh;
	return v_out;
}

CDRAW_INL intN_t vecMulS2i(int2_t v_out, int2_t const v_lh, veci_t const s_rh)
{
	failassert(v_out && v_lh, NULL);
	vx(v_out) = vx(v_lh) * s_rh;
	vy(v_out) = vy(v_lh) * s_rh;
	return v_out;
}

CDRAW_INL intN_t vecDivS2i(int2_t v_out, int2_t const v_lh, veci_t const s_rh)
{
	failassert(v_out && v_lh, NULL);
	failassert(s_rh, vecZero2i(v_out));
	vx(v_out) = gDiv(vx(v_lh), s_rh);
	vy(v_out) = gDiv(vy(v_lh), s_rh);
	return v_out;
}

CDRAW_INL intN_t vecModS2i(int2_t v_out, int2_t const v_lh, veci_t const s_rh)
{
	failassert(v_out && v_lh, NULL);
	failassert(s_rh, vecCopy2i(v_out, v_lh));
	vx(v_out) = gMod(vx(v_lh), s_rh);
	vy(v_out) = gMod(vy(v_lh), s_rh);
	return v_out;
}

CDRAW_INL intN_t vecDivModS2i(int2_t v_out, int2_t v_mod_out, int2_t const v_lh, veci_t const s_rh)
{
	failassert(v_out && v_mod_out && v_lh, NULL);
	failassert(s_rh, vecCopy2i(v_mod_out, v_lh), vecZero2i(v_out));
	vx(v_out) = gDivMod(vx(v_mod_out), vx(v_lh), s_rh);
	vy(v_out) = gDivMod(vy(v_mod_out), vy(v_lh), s_rh);
	return v_out;
}

CDRAW_INL veci_t vecDot2i(int2_t const v_lh, int2_t const v_rh)
{
	failassert(v_lh && v_rh, 0);
	return (vx(v_lh) * vx(v_rh) + vy(v_lh) * vy(v_rh));
}

CDRAW_INL veci_t vecCross2i(int2_t const v_lh, int2_t const v_rh)
{
	failassert(v_lh && v_rh, 0);
	return (vx(v_lh) * vy(v_rh) - vy(v_lh) * vx(v_rh));
}

CDRAW_INL vecu_t vecLenSq2i(int2_t const v)
{
	failassert(v, 0);
	return (gSq(vx(v)) + gSq(vy(v)));
}

CDRAW_INL vecu_t vecDistSq2i(int2_t const v_lh, int2_t const v_rh)
{
	failassert(v_lh && v_rh, 0);
	int2_t const disp = {
		(vx(v_lh) - vx(v_rh)),
		(vy(v_lh) - vy(v_rh)),
	};
	return (gSq(vx(disp)) + gSq(vy(disp)));
}

CDRAW_INL vecu_t vecDispDistSq2i(int2_t v_disp_out, int2_t const v_lh, int2_t const v_rh)
{
	failassert(v_disp_out && v_lh && v_rh, 0);
	vx(v_disp_out) = (vx(v_lh) - vx(v_rh));
	vy(v_disp_out) = (vy(v_lh) - vy(v_rh));
	return (gSq(vx(v_disp_out)) + gSq(vy(v_disp_out)));
}

CDRAW_INL vecb_t vecIsZero2i(int2_t const v)
{
	return ((vx(v) == 0) && (vy(v) == 0));
}

CDRAW_INL vecb_t vecIsNonZero2i(int2_t const v)
{
	return ((vx(v) != 0) || (vy(v) != 0));
}

CDRAW_INL intN_t vecMad2i(int2_t v_out, veci_t const u, int2_t const v_origin, int2_t const v_delta)
{
	failassert(v_out && v_origin && v_delta, NULL);
	vx(v_out) = gMad(vx(v_origin), vx(v_delta), u);
	vy(v_out) = gMad(vy(v_origin), vy(v_delta), u);
	return v_out;
}

CDRAW_INL intN_t vecLerp2i(int2_t v_out, veci_t const u, int2_t const v_min, int2_t const v_max)
{
	failassert(v_out && v_min && v_max, NULL);
	vx(v_out) = gLerp(vx(v_min), vx(v_max), u);
	vy(v_out) = gLerp(vy(v_min), vy(v_max), u);
	return v_out;
}


#endif // #if (!(defined _CDRAW_VEC2I_INL_) && (defined _CDRAW_VECTOR_INL_))