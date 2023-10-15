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
* cdrawMat2f.inl
* Definitions of single-precision 2D matrix functions.
*/

#if (!(defined _CDRAW_MAT2F_INL_) && (defined _CDRAW_MATRIX_INL_))
#define _CDRAW_MAT2F_INL_


CDRAW_INL floatNx2_t matIdentity2f(float2x2_t m_out)
{
	failassert(m_out, NULL);
	vecCopy2f(vx(m_out), vecX2f.v);
	vecCopy2f(vy(m_out), vecY2f.v);
	return m_out;
}

CDRAW_INL floatNx2_t matInit2f(float2x2_t m_out, float2_t const x, float2_t const y)
{
	failassert(m_out && x && y, NULL);
	vecCopy2f(vx(m_out), x);
	vecCopy2f(vy(m_out), y);
	return m_out;
}

CDRAW_INL floatNx2_t matInitS2f(float2x2_t m_out, vecf_t const x)
{
	failassert(m_out, NULL);
	vecInit2f(vx(m_out), x, sc0F);
	vecInit2f(vy(m_out), sc0F, x);
	return m_out;
}

CDRAW_INL floatNx2_t matCopy32f(float2x2_t m_out, float3x3_t const m)
{
	failassert(m_out && m, NULL);
	vecCopy2f(vx(m_out), vx(m));
	vecCopy2f(vy(m_out), vy(m));
	return m_out;
}

CDRAW_INL floatNx2_t matCopy42f(float2x2_t m_out, float4x4_t const m)
{
	failassert(m_out && m, NULL);
	vecCopy2f(vx(m_out), vx(m));
	vecCopy2f(vy(m_out), vy(m));
	return m_out;
}

CDRAW_INL floatNx2_t matCopy2f(float2x2_t m_out, float2x2_t const m)
{
	failassert(m_out && m, NULL);
	vecCopy2f(vx(m_out), vx(m));
	vecCopy2f(vy(m_out), vy(m));
	return m_out;
}

CDRAW_INL floatNx2_t matCopy2d2f(float2x2_t m_out, double2x2_t const m)
{
	failassert(m_out && m, NULL);
	vecCopy2d2f(vx(m_out), vx(m));
	vecCopy2d2f(vy(m_out), vy(m));
	return m_out;
}

CDRAW_INL floatNx2_t matNegate2f(float2x2_t m_out, float2x2_t const m)
{
	failassert(m_out && m, NULL);
	vecNegate2f(vx(m_out), vx(m));
	vecNegate2f(vy(m_out), vy(m));
	return m_out;
}

CDRAW_INL vecf_t matDet2f(float2x2_t const m)
{
	failassert(m, sc0F);
	// 2D determinant is the 2D cross product of columns
	return vecCross2f(vx(m), vy(m));
}

CDRAW_INL vecf_t matDetInv2f(float2x2_t const m)
{
	failassert(m, sc0F);
	vecf_t const det = matDet2f(m);
	failassert(scIsNonZeroApproxF(det), sc0F);
	return (sc1F / det);
}

CDRAW_INL vecb_t matIsIdentity2f(float2x2_t const m)
{
	failassert(m, false);
	return scIsUnityApproxF(mxx(m)) && scIsNonPositiveApproxF(gSq(mxy(m)))
		&& scIsNonPositiveApproxF(gSq(myx(m))) && scIsUnityApproxF(myy(m));
}

CDRAW_INL vecb_t matIsOrtho2f(vecb_t* isMirrored_out_opt, float2x2_t const m)
{
	failassert(m, false);
	vecb_t const isOrtho = scIsZeroApproxF(vecDot2f(vx(m), vy(m)));
	if (isMirrored_out_opt)
	{
		*isMirrored_out_opt = matDet2f(m) < sc0F;
	}
	return isOrtho;
}

CDRAW_INL vecf_t matInverse2f(float2x2_t m_out, float2x2_t const m)
{
	failassert(m_out && m, sc0F);
	// 2D inverse is trivial
	vecf_t const det = matDet2f(m);
	failassert(scIsNonZeroApproxF(det), matIdentity2f(m_out), sc0F);
	vecf_t const detInv = (sc1F / det);
	vecf_t const tmp_xx = mxx(m);
	mxx(m_out) = +myy(m) * detInv;
	mxy(m_out) = -mxy(m) * detInv;
	myx(m_out) = -myx(m) * detInv;
	myy(m_out) = +tmp_xx * detInv;
	return det;
}

CDRAW_INL floatNx2_t matTranspose2f(float2x2_t m_out, float2x2_t const m)
{
	failassert(m_out && m, NULL);
	vecf_t const tmp = mxy(m);
	mxx(m_out) = mxx(m);
	myy(m_out) = myy(m);
	mxy(m_out) = myx(m);
	myx(m_out) = tmp;
	return m_out;
}

CDRAW_INL vecf_t matInverseTranspose2f(float2x2_t m_out, float2x2_t const m)
{
	failassert(m_out && m, sc0F);
	vecf_t const det = matDet2f(m);
	failassert(scIsNonZeroApproxF(det), matIdentity2f(m_out), sc0F);
	vecf_t const detInv = (sc1F / det);
	vecf_t const tmp_xx = mxx(m);
	vecf_t const tmp_xy = mxy(m);
	mxx(m_out) = +myy(m) * detInv;
	mxy(m_out) = -myx(m) * detInv;
	myx(m_out) = -tmp_xy * detInv;
	myy(m_out) = +tmp_xx * detInv;
	return det;
}

CDRAW_INL floatN_t matMulVec2f(float2_t v_out, float2x2_t const m_lh, float2_t const v_rh)
{
	failassert(v_out && m_lh && v_rh, NULL);
	failassert((v_out != v_rh) && (v_out != vx(m_lh)) && (v_out != vy(m_lh)), NULL);
	vx(v_out) = mxx(m_lh) * vx(v_rh) + myx(m_lh) * vy(v_rh);
	vy(v_out) = mxy(m_lh) * vx(v_rh) + myy(m_lh) * vy(v_rh);
	return v_out;
}

CDRAW_INL floatN_t matTransposeMulVec2f(float2_t v_out, float2x2_t const m_lh, float2_t const v_rh)
{
	failassert(v_out && m_lh && v_rh, NULL);
	failassert((v_out != v_rh) && (v_out != vx(m_lh)) && (v_out != vy(m_lh)), NULL);
	vx(v_out) = vecDot2f(vx(m_lh), v_rh);
	vy(v_out) = vecDot2f(vy(m_lh), v_rh);
	return v_out;
}

CDRAW_INL floatNx2_t matTransposeMul2f(float2x2_t m_out, float2x2_t const m_lh, float2x2_t const m_rh)
{
	failassert(m_out && m_lh && m_rh, NULL);
	matTransposeMulVec2f(vx(m_out), m_lh, vx(m_rh));
	matTransposeMulVec2f(vy(m_out), m_lh, vy(m_rh));
	return m_out;
}

CDRAW_INL floatNx2_t matAdd2f(float2x2_t m_out, float2x2_t const m_lh, float2x2_t const m_rh)
{
	failassert(m_out && m_lh && m_rh, NULL);
	vecAdd2f(vx(m_out), vx(m_lh), vx(m_rh));
	vecAdd2f(vy(m_out), vy(m_lh), vy(m_rh));
	return m_out;
}

CDRAW_INL floatNx2_t matSub2f(float2x2_t m_out, float2x2_t const m_lh, float2x2_t const m_rh)
{
	failassert(m_out && m_lh && m_rh, NULL);
	vecSub2f(vx(m_out), vx(m_lh), vx(m_rh));
	vecSub2f(vy(m_out), vy(m_lh), vy(m_rh));
	return m_out;
}

CDRAW_INL floatNx2_t matMul2f(float2x2_t m_out, float2x2_t const m_lh, float2x2_t const m_rh)
{
	failassert(m_out && m_lh && m_rh, NULL);
	matMulVec2f(vx(m_out), m_lh, vx(m_rh));
	matMulVec2f(vy(m_out), m_lh, vy(m_rh));
	return m_out;
}

CDRAW_INL floatNx2_t matDiv2f(float2x2_t m_out, float2x2_t const m_lh, float2x2_t const m_rh)
{
	failassert(m_out && m_lh && m_rh, NULL);
	float2x2_t m_rh_inv;
	matInverse2f(m_rh_inv, m_rh);
	return matMul2f(m_out, m_lh, m_rh_inv);
}

CDRAW_INL floatNx2_t matAddS2f(float2x2_t m_out, float2x2_t const m_lh, vecf_t const s_rh)
{
	failassert(m_out && m_lh, NULL);
	vecAddS2f(vx(m_out), vx(m_lh), s_rh);
	vecAddS2f(vy(m_out), vy(m_lh), s_rh);
	return m_out;
}

CDRAW_INL floatNx2_t matSubS2f(float2x2_t m_out, float2x2_t const m_lh, vecf_t const s_rh)
{
	failassert(m_out && m_lh, NULL);
	vecSubS2f(vx(m_out), vx(m_lh), s_rh);
	vecSubS2f(vy(m_out), vy(m_lh), s_rh);
	return m_out;
}

CDRAW_INL floatNx2_t matMulS2f(float2x2_t m_out, float2x2_t const m_lh, vecf_t const s_rh)
{
	failassert(m_out && m_lh, NULL);
	vecMulS2f(vx(m_out), vx(m_lh), s_rh);
	vecMulS2f(vy(m_out), vy(m_lh), s_rh);
	return m_out;
}

CDRAW_INL floatNx2_t matDivS2f(float2x2_t m_out, float2x2_t const m_lh, vecf_t const s_rh)
{
	failassert(m_out && m_lh, NULL);
	failassert(scIsNonZeroApproxF(s_rh), matIdentity2f(m_out));
	matMulS2f(m_out, m_lh, (sc1F / s_rh));
	return m_out;
}

CDRAW_INL vecf_t matInvMulVec2f(float2_t v_out, float2x2_t const m_lh, float2_t const v_rh)
{
	failassert(v_out && m_lh && v_rh, sc0F);
	failassert((v_out != vx(m_lh)) && (v_out != vy(m_lh)), sc0F);
	vecf_t const det = matDet2f(m_lh);
	failassert(scIsNonZeroApproxF(det), vecZero2f(v_out), sc0F);
	vecf_t const detInv = (sc1F / det);
	vx(v_out) = (myy(m_lh) * vx(v_rh) - myx(m_lh) * vy(v_rh)) * detInv;
	vy(v_out) = (myy(m_lh) * vy(v_rh) - mxy(m_lh) * vx(v_rh)) * detInv;
	return det;
}

CDRAW_INL vecf_t matInvMul2f(float2x2_t m_out, float2x2_t const m_lh, float2x2_t const m_rh)
{
	failassert(m_out && m_lh && m_rh, sc0F);
	float2x2_t m_lh_inv;
	vecf_t const det = matInverseTranspose2f(m_lh_inv, m_lh);
	failassert(scIsNonZeroApproxF(det), matIdentity2f(m_out), sc0F);
	matTransposeMul2f(m_out, m_lh_inv, m_rh);
	return det;
}


#endif // #if (!(defined _CDRAW_MAT2F_INL_) && (defined _CDRAW_MATRIX_INL_))