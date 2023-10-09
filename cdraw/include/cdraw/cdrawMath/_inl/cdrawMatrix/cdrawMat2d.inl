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
* cdrawMat2d.inl
* Definitions of double-precision 2D matrix functions.
*/

#if (!(defined _CDRAW_MAT2D_INL_) && (defined _CDRAW_MATRIX_INL_))
#define _CDRAW_MAT2D_INL_


CDRAW_INL doubleNx2_t matIdentity2d(double2x2_t m_out)
{
	failassert(m_out, NULL);
	vecCopy2d(vx(m_out), vecX2d.v);
	vecCopy2d(vy(m_out), vecY2d.v);
	return m_out;
}

CDRAW_INL doubleNx2_t matInit2d(double2x2_t m_out, double2_t const x, double2_t const y)
{
	failassert(m_out && x && y, NULL);
	vecCopy2d(vx(m_out), x);
	vecCopy2d(vy(m_out), y);
	return m_out;
}

CDRAW_INL doubleNx2_t matInitS2d(double2x2_t m_out, vecd_t const x)
{
	failassert(m_out, NULL);
	vecInit2d(vx(m_out), x, sc0D);
	vecInit2d(vy(m_out), sc0D, x);
	return m_out;
}

CDRAW_INL doubleNx2_t matCopy32d(double2x2_t m_out, double3x3_t const m)
{
	failassert(m_out && m, NULL);
	vecCopy2d(vx(m_out), vx(m));
	vecCopy2d(vy(m_out), vy(m));
	return m_out;
}

CDRAW_INL doubleNx2_t matCopy42d(double2x2_t m_out, double4x4_t const m)
{
	failassert(m_out && m, NULL);
	vecCopy2d(vx(m_out), vx(m));
	vecCopy2d(vy(m_out), vy(m));
	return m_out;
}

CDRAW_INL doubleNx2_t matCopy2d(double2x2_t m_out, double2x2_t const m)
{
	failassert(m_out && m, NULL);
	vecCopy2d(vx(m_out), vx(m));
	vecCopy2d(vy(m_out), vy(m));
	return m_out;
}

CDRAW_INL doubleNx2_t matCopy2f2d(double2x2_t m_out, float2x2_t const m)
{
	failassert(m_out && m, NULL);
	vecCopy2f2d(vx(m_out), vx(m));
	vecCopy2f2d(vy(m_out), vy(m));
	return m_out;
}

CDRAW_INL doubleNx2_t matNegate2d(double2x2_t m_out, double2x2_t const m)
{
	failassert(m_out && m, NULL);
	vecNegate2d(vx(m_out), vx(m));
	vecNegate2d(vy(m_out), vy(m));
	return m_out;
}

CDRAW_INL vecd_t matDet2d(double2x2_t const m)
{
	failassert(m, sc0D);
	// 2D determinant is the 2D cross product of columns
	return vecCross2d(vx(m), vy(m));
}

CDRAW_INL vecd_t matDetInv2d(double2x2_t const m)
{
	failassert(m, sc0D);
	vecd_t const det = matDet2d(m);
	failassert(scIsNonZeroApproxD(det), sc0D);
	return (sc1D / det);
}

CDRAW_INL vecb_t matIsIdentity2d(double2x2_t const m)
{
	failassert(m, false);
	return scIsUnityApproxD(mxx(m)) && scIsNonPositiveApproxD(gSq(mxy(m)))
		&& scIsNonPositiveApproxD(gSq(myx(m))) && scIsUnityApproxD(myy(m));
}

CDRAW_INL vecb_t matIsOrtho2d(vecb_t* isMirrored_out_opt, double2x2_t const m)
{
	failassert(m, false);
	vecb_t const isOrtho = scIsZeroApproxD(vecDot2d(vx(m), vy(m)));
	if (isMirrored_out_opt)
	{
		*isMirrored_out_opt = matDet2d(m) < sc0D;
	}
	return isOrtho;
}

CDRAW_INL vecd_t matInverse2d(double2x2_t m_out, double2x2_t const m)
{
	failassert(m_out && m, sc0D);
	// 2D inverse is trivial
	vecd_t const det = matDet2d(m);
	failassert(scIsNonZeroApproxD(det), matIdentity2d(m_out), sc0D);
	vecd_t const detInv = (sc1D / det);
	vecd_t const tmp_xx = mxx(m);
	mxx(m_out) = +myy(m) * detInv;
	mxy(m_out) = -mxy(m) * detInv;
	myx(m_out) = -myx(m) * detInv;
	myy(m_out) = +tmp_xx * detInv;
	return det;
}

CDRAW_INL doubleNx2_t matTranspose2d(double2x2_t m_out, double2x2_t const m)
{
	failassert(m_out && m, NULL);
	vecd_t const tmp = mxy(m);
	mxx(m_out) = mxx(m);
	myy(m_out) = myy(m);
	mxy(m_out) = myx(m);
	myx(m_out) = tmp;
	return m_out;
}

CDRAW_INL vecd_t matInverseTranspose2d(double2x2_t m_out, double2x2_t const m)
{
	failassert(m_out && m, sc0D);
	vecd_t const det = matDet2d(m);
	failassert(scIsNonZeroApproxD(det), matIdentity2d(m_out), sc0D);
	vecd_t const detInv = (sc1D / det);
	vecd_t const tmp_xx = mxx(m);
	vecd_t const tmp_xy = mxy(m);
	mxx(m_out) = +myy(m) * detInv;
	mxy(m_out) = -myx(m) * detInv;
	myx(m_out) = -tmp_xy * detInv;
	myy(m_out) = +tmp_xx * detInv;
	return det;
}

CDRAW_INL doubleN_t matMulVec2d(double2_t v_out, double2x2_t const m_lh, double2_t const v_rh)
{
	failassert(v_out && m_lh && v_rh, NULL);
	failassert((v_out != v_rh) && (v_out != vx(m_lh)) && (v_out != vy(m_lh)), NULL);
	vx(v_out) = mxx(m_lh) * vx(v_rh) + myx(m_lh) * vy(v_rh);
	vy(v_out) = mxy(m_lh) * vx(v_rh) + myy(m_lh) * vy(v_rh);
	return v_out;
}

CDRAW_INL doubleN_t matTransposeMulVec2d(double2_t v_out, double2x2_t const m_lh, double2_t const v_rh)
{
	failassert(v_out && m_lh && v_rh, NULL);
	failassert((v_out != v_rh) && (v_out != vx(m_lh)) && (v_out != vy(m_lh)), NULL);
	vx(v_out) = vecDot2d(vx(m_lh), v_rh);
	vy(v_out) = vecDot2d(vy(m_lh), v_rh);
	return v_out;
}

CDRAW_INL doubleNx2_t matTransposeMul2d(double2x2_t m_out, double2x2_t const m_lh, double2x2_t const m_rh)
{
	failassert(m_out && m_lh && m_rh, NULL);
	matTransposeMulVec2d(vx(m_out), m_lh, vx(m_rh));
	matTransposeMulVec2d(vy(m_out), m_lh, vy(m_rh));
	return m_out;
}

CDRAW_INL doubleNx2_t matAdd2d(double2x2_t m_out, double2x2_t const m_lh, double2x2_t const m_rh)
{
	failassert(m_out && m_lh && m_rh, NULL);
	vecAdd2d(vx(m_out), vx(m_lh), vx(m_rh));
	vecAdd2d(vy(m_out), vy(m_lh), vy(m_rh));
	return m_out;
}

CDRAW_INL doubleNx2_t matSub2d(double2x2_t m_out, double2x2_t const m_lh, double2x2_t const m_rh)
{
	failassert(m_out && m_lh && m_rh, NULL);
	vecSub2d(vx(m_out), vx(m_lh), vx(m_rh));
	vecSub2d(vy(m_out), vy(m_lh), vy(m_rh));
	return m_out;
}

CDRAW_INL doubleNx2_t matMul2d(double2x2_t m_out, double2x2_t const m_lh, double2x2_t const m_rh)
{
	failassert(m_out && m_lh && m_rh, NULL);
	matMulVec2d(vx(m_out), m_lh, vx(m_rh));
	matMulVec2d(vy(m_out), m_lh, vy(m_rh));
	return m_out;
}

CDRAW_INL doubleNx2_t matDiv2d(double2x2_t m_out, double2x2_t const m_lh, double2x2_t const m_rh)
{
	failassert(m_out && m_lh && m_rh, NULL);
	double2x2_t m_rh_inv;
	matInverse2d(m_rh_inv, m_rh);
	return matMul2d(m_out, m_lh, m_rh_inv);
}

CDRAW_INL doubleNx2_t matAddS2d(double2x2_t m_out, double2x2_t const m_lh, vecd_t const s_rh)
{
	failassert(m_out && m_lh, NULL);
	vecAddS2d(vx(m_out), vx(m_lh), s_rh);
	vecAddS2d(vy(m_out), vy(m_lh), s_rh);
	return m_out;
}

CDRAW_INL doubleNx2_t matSubS2d(double2x2_t m_out, double2x2_t const m_lh, vecd_t const s_rh)
{
	failassert(m_out && m_lh, NULL);
	vecSubS2d(vx(m_out), vx(m_lh), s_rh);
	vecSubS2d(vy(m_out), vy(m_lh), s_rh);
	return m_out;
}

CDRAW_INL doubleNx2_t matMulS2d(double2x2_t m_out, double2x2_t const m_lh, vecd_t const s_rh)
{
	failassert(m_out && m_lh, NULL);
	vecMulS2d(vx(m_out), vx(m_lh), s_rh);
	vecMulS2d(vy(m_out), vy(m_lh), s_rh);
	return m_out;
}

CDRAW_INL doubleNx2_t matDivS2d(double2x2_t m_out, double2x2_t const m_lh, vecd_t const s_rh)
{
	failassert(m_out && m_lh, NULL);
	failassert(scIsNonZeroApproxD(s_rh), matIdentity2d(m_out));
	matMulS2d(m_out, m_lh, (sc1D / s_rh));
	return m_out;
}

CDRAW_INL vecd_t matInvMulVec2d(double2_t v_out, double2x2_t const m_lh, double2_t const v_rh)
{
	failassert(v_out && m_lh && v_rh, sc0D);
	failassert((v_out != vx(m_lh)) && (v_out != vy(m_lh)), sc0D);
	vecd_t const det = matDet2d(m_lh);
	failassert(scIsNonZeroApproxD(det), vecZero2d(v_out), sc0D);
	vecd_t const detInv = (sc1D / det);
	vx(v_out) = (myy(m_lh) * vx(v_rh) - myx(m_lh) * vy(v_rh)) * detInv;
	vy(v_out) = (myy(m_lh) * vy(v_rh) - mxy(m_lh) * vx(v_rh)) * detInv;
	return det;
}

CDRAW_INL vecd_t matInvMul2d(double2x2_t m_out, double2x2_t const m_lh, double2x2_t const m_rh)
{
	failassert(m_out && m_lh && m_rh, sc0D);
	double2x2_t m_lh_inv;
	vecd_t const det = matInverseTranspose2d(m_lh_inv, m_lh);
	failassert(scIsNonZeroApproxD(det), matIdentity2d(m_out), sc0D);
	matTransposeMul2d(m_out, m_lh_inv, m_rh);
	return det;
}


#endif // #if (!(defined _CDRAW_MAT2D_INL_) && (defined _CDRAW_MATRIX_INL_))