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
* cdrawMat4d.inl
* Definitions of double-precision 4D matrix functions.
*/

#if (!(defined _CDRAW_MAT4D_INL_) && (defined _CDRAW_MATRIX_INL_))
#define _CDRAW_MAT4D_INL_


CDRAW_INL doubleNx4_t matIdentity4d(double4x4_t m_out)
{
	failassert(m_out, NULL);
	vecCopy4d(vx(m_out), vecX4d.v);
	vecCopy4d(vy(m_out), vecY4d.v);
	vecCopy4d(vz(m_out), vecZ4d.v);
	vecCopy4d(vw(m_out), vecW4d.v);
	return m_out;
}

CDRAW_INL doubleNx4_t matInit4d(double4x4_t m_out, double4_t const x, double4_t const y, double4_t const z, double4_t const w)
{
	failassert(m_out && x && y && z && w, NULL);
	vecCopy4d(vx(m_out), x);
	vecCopy4d(vy(m_out), y);
	vecCopy4d(vz(m_out), z);
	vecCopy4d(vw(m_out), w);
	return m_out;
}

CDRAW_INL doubleNx4_t matInitS4d(double4x4_t m_out, vecd_t const x)
{
	failassert(m_out, NULL);
	vecInit4d(vx(m_out), x, sc0D, sc0D, sc0D);
	vecInit4d(vy(m_out), sc0D, x, sc0D, sc0D);
	vecInit4d(vz(m_out), sc0D, sc0D, x, sc0D);
	vecInit4d(vw(m_out), sc0D, sc0D, sc0D, x);
	return m_out;
}

CDRAW_INL doubleNx4_t matCopy2zw4d(double4x4_t m_out, double2x2_t const xy, double4_t const z, double4_t const w)
{
	failassert(m_out && xy && z && w, NULL);
	vecCopy2zw4d(vx(m_out), vx(xy), sc0D, sc0D);
	vecCopy2zw4d(vy(m_out), vy(xy), sc0D, sc0D);
	vecCopy4d(vz(m_out), z);
	vecCopy4d(vw(m_out), w);
	return m_out;
}

CDRAW_INL doubleNx4_t matCopy3w4d(double4x4_t m_out, double3x3_t const xyz, double4_t const w)
{
	failassert(m_out && xyz && w, NULL);
	vecCopy3w4d(vx(m_out), vx(xyz), sc0D);
	vecCopy3w4d(vy(m_out), vy(xyz), sc0D);
	vecCopy3w4d(vz(m_out), vz(xyz), sc0D);
	vecCopy4d(vw(m_out), w);
	return m_out;
}

CDRAW_INL doubleNx4_t matCopy4d(double4x4_t m_out, double4x4_t const m)
{
	failassert(m_out && m, NULL);
	vecCopy4d(vx(m_out), vx(m));
	vecCopy4d(vy(m_out), vy(m));
	vecCopy4d(vz(m_out), vz(m));
	vecCopy4d(vw(m_out), vw(m));
	return m_out;
}

CDRAW_INL doubleNx4_t matCopy4f4d(double4x4_t m_out, float4x4_t const m)
{
	failassert(m_out && m, NULL);
	vecCopy4f4d(vx(m_out), vx(m));
	vecCopy4f4d(vy(m_out), vy(m));
	vecCopy4f4d(vz(m_out), vz(m));
	vecCopy4f4d(vw(m_out), vw(m));
	return m_out;
}

CDRAW_INL doubleNx4_t matNegate4d(double4x4_t m_out, double4x4_t const m)
{
	failassert(m_out && m, NULL);
	vecNegate4d(vx(m_out), vx(m));
	vecNegate4d(vy(m_out), vy(m));
	vecNegate4d(vz(m_out), vz(m));
	vecNegate4d(vw(m_out), vw(m));
	return m_out;
}

CDRAW_INL vecd_t matDet4d(double4x4_t const m)
{
	failassert(m, sc0D);
	// 4D determinant is the determinant of 2x2 block determinants
	vecd_t const det_A = (mxx(m) * myy(m) - myx(m) * mxy(m));
	vecd_t const det_C = (mxz(m) * myw(m) - myz(m) * mxw(m));
	vecd_t const det_B = (mzx(m) * mwy(m) - mwx(m) * mzy(m));
	vecd_t const det_D = (mzz(m) * mww(m) - mwz(m) * mzw(m));
	return (det_A * det_D - det_B * det_C);
}

CDRAW_INL vecd_t matDetInv4d(double4x4_t const m)
{
	failassert(m, sc0D);
	vecd_t const det = matDet4d(m);
	failassert(scIsNonZeroApproxD(det), sc0D);
	return (sc1D / det);
}

CDRAW_INL vecb_t matIsIdentity4d(double4x4_t const m)
{
	failassert(m, false);
	return scIsUnityApproxD(mxx(m)) && scIsNonPositiveApproxD(gSq(mxy(m))) && scIsNonPositiveApproxD(gSq(mxz(m))) && scIsNonPositiveApproxD(gSq(mxw(m)))
		&& scIsNonPositiveApproxD(gSq(myx(m))) && scIsUnityApproxD(myy(m)) && scIsNonPositiveApproxD(gSq(myz(m))) && scIsNonPositiveApproxD(gSq(myw(m)))
		&& scIsNonPositiveApproxD(gSq(mzx(m))) && scIsNonPositiveApproxD(gSq(mzy(m))) && scIsUnityApproxD(mzz(m)) && scIsNonPositiveApproxD(gSq(mzw(m)))
		&& scIsNonPositiveApproxD(gSq(mwx(m))) && scIsNonPositiveApproxD(gSq(mwy(m))) && scIsNonPositiveApproxD(gSq(mwz(m))) && scIsUnityApproxD(mww(m));
}

CDRAW_INL vecb_t matIsOrtho4d(vecb_t* isMirrored_out_opt, double4x4_t const m)
{
	failassert(m, false);
	vecb_t const isOrtho = scIsZeroApproxD(vecDot3d(vx(m), vy(m))) && scIsZeroApproxD(vecDot3d(vy(m), vz(m))) && scIsZeroApproxD(vecDot3d(vz(m), vx(m)));
	if (isMirrored_out_opt)
	{
		double3_t compare;
		*isMirrored_out_opt = vecDot3d(vecCross3d(compare, vx(m), vy(m)), vz(m)) < sc0D;
	}
	return isOrtho;
}

CDRAW_INL vecd_t matInverse4d(double4x4_t m_out, double4x4_t const m)
{
	failassert(m_out && m, sc0D);
	// blockwise inversion on 2x2 quadrants
	//	m = / A	 C \
	//		\ B	 D /
	//	m_inv = / (A - (CD^-1)B)^-1			0		   \/	I		-(CD^-1) \
	//			\		0			 (D - (BA^-1)C)^-1 /\ -(BA^-1)		I	 /
	double2x2_t const
		A = { mxx(m), mxy(m), myx(m), myy(m) },
		B = { mxz(m), mxw(m), myz(m), myw(m) },
		C = { mzx(m), mzy(m), mwx(m), mwy(m) },
		D = { mzz(m), mzw(m), mwz(m), mww(m) };
	vecd_t const detN_A = (myx(A) * mxy(A) - mxx(A) * myy(A));
	vecd_t const detN_B = (myx(B) * mxy(B) - mxx(B) * myy(B));
	vecd_t const detN_C = (myx(C) * mxy(C) - mxx(C) * myy(C));
	vecd_t const detN_D = (myx(D) * mxy(D) - mxx(D) * myy(D));
	vecd_t const det = (detN_A * detN_D - detN_B * detN_C);
	failassert(scIsNonZeroApproxD(det), matIdentity4d(m_out), sc0D);
	vecd_t const detInvN_A = (sc1D / detN_A);
	vecd_t const detInvN_D = (sc1D / detN_D);
	double2x2_t const nBAinv = {
		(mxx(B) * myy(A) - myx(B) * mxy(A)) * detInvN_A,
		(mxy(B) * myy(A) - myy(B) * mxy(A)) * detInvN_A,
		(myx(B) * mxx(A) - mxx(B) * myx(A)) * detInvN_A,
		(myy(B) * mxx(A) - mxy(B) * myx(A)) * detInvN_A,
	}, nCDinv = {
		(mxx(C) * myy(D) - myx(C) * mxy(D)) * detInvN_D,
		(mxy(C) * myy(D) - myy(C) * mxy(D)) * detInvN_D,
		(myx(C) * mxx(D) - mxx(C) * myx(D)) * detInvN_D,
		(myy(C) * mxx(D) - mxy(C) * myx(D)) * detInvN_D,
	}, AmCDinvB = {
		(mxx(A) + mxx(nCDinv) * mxx(B) + myx(nCDinv) * mxy(B)),
		(mxy(A) + mxy(nCDinv) * mxx(B) + myy(nCDinv) * mxy(B)),
		(myx(A) + mxx(nCDinv) * myx(B) + myx(nCDinv) * myy(B)),
		(myy(A) + mxy(nCDinv) * myx(B) + myy(nCDinv) * myy(B)),
	}, DmBAinvC = {
		(mxx(D) + mxx(nBAinv) * mxx(C) + myx(nBAinv) * mxy(C)),
		(mxy(D) + mxy(nBAinv) * mxx(C) + myy(nBAinv) * mxy(C)),
		(myx(D) + mxx(nBAinv) * myx(C) + myx(nBAinv) * myy(C)),
		(myy(D) + mxy(nBAinv) * myx(C) + myy(nBAinv) * myy(C)),
	};
	vecd_t const detInv_AmCDinvB = sc1D / (mxx(AmCDinvB) * myy(AmCDinvB) - myx(AmCDinvB) * mxy(AmCDinvB));
	vecd_t const detInv_DmBAinvC = sc1D / (mxx(DmBAinvC) * myy(DmBAinvC) - myx(DmBAinvC) * mxy(DmBAinvC));
	double2x2_t const inv_AmCDinvB = {
		+myy(AmCDinvB) * detInv_AmCDinvB,
		-mxy(AmCDinvB) * detInv_AmCDinvB,
		-myx(AmCDinvB) * detInv_AmCDinvB,
		+mxx(AmCDinvB) * detInv_AmCDinvB,
	}, inv_DmBAinvC = {
		+myy(DmBAinvC) * detInv_DmBAinvC,
		-mxy(DmBAinvC) * detInv_DmBAinvC,
		-myx(DmBAinvC) * detInv_DmBAinvC,
		+mxx(DmBAinvC) * detInv_DmBAinvC,
	};
	mxx(m_out) = mxx(inv_AmCDinvB);														myx(m_out) = myx(inv_AmCDinvB);														mzx(m_out) = mxx(inv_AmCDinvB) * mxx(nCDinv) + myx(inv_AmCDinvB) * mxy(nCDinv);		mwx(m_out) = mxx(inv_AmCDinvB) * myx(nCDinv) + myx(inv_AmCDinvB) * myy(nCDinv);
	mxy(m_out) = mxy(inv_AmCDinvB);														myy(m_out) = myy(inv_AmCDinvB);														mzy(m_out) = mxy(inv_AmCDinvB) * mxx(nCDinv) + myy(inv_AmCDinvB) * mxy(nCDinv);		mwy(m_out) = mxy(inv_AmCDinvB) * myx(nCDinv) + myy(inv_AmCDinvB) * myy(nCDinv);
	mxz(m_out) = mxx(inv_DmBAinvC) * mxx(nBAinv) + myx(inv_DmBAinvC) * mxy(nBAinv);		myz(m_out) = mxx(inv_DmBAinvC) * myx(nBAinv) + myx(inv_DmBAinvC) * myy(nBAinv);		mzz(m_out) = mxx(inv_DmBAinvC);														mwz(m_out) = myx(inv_DmBAinvC);
	mxw(m_out) = mxy(inv_DmBAinvC) * mxx(nBAinv) + myy(inv_DmBAinvC) * mxy(nBAinv);		myw(m_out) = mxy(inv_DmBAinvC) * myx(nBAinv) + myy(inv_DmBAinvC) * myy(nBAinv);		mzw(m_out) = mxy(inv_DmBAinvC);														mww(m_out) = myy(inv_DmBAinvC);
	return det;
}

CDRAW_INL doubleNx4_t matTranspose4d(double4x4_t m_out, double4x4_t const m)
{
	failassert(m_out && m, NULL);
	vecd_t tmp;
	mxx(m_out) = mxx(m);	myy(m_out) = myy(m);	mzz(m_out) = mzz(m);	mww(m_out) = mww(m);
	tmp = mxy(m);	mxy(m_out) = myx(m);	myx(m_out) = tmp;
	tmp = mxz(m);	mxz(m_out) = mzx(m);	mzx(m_out) = tmp;
	tmp = mxw(m);	mxw(m_out) = mwx(m);	mwx(m_out) = tmp;
	tmp = myz(m);	myz(m_out) = mzy(m);	mzy(m_out) = tmp;
	tmp = myw(m);	myw(m_out) = mwy(m);	mwy(m_out) = tmp;
	tmp = mzw(m);	mzw(m_out) = mwz(m);	mwz(m_out) = tmp;
	return m_out;
}

CDRAW_INL vecd_t matInverseTranspose4d(double4x4_t m_out, double4x4_t const m)
{
	failassert(m_out && m, sc0D);
	return matInverse4d(m_out, matTranspose4d(m_out, m));
}

CDRAW_INL doubleN_t matMulVec4d(double4_t v_out, double4x4_t const m_lh, double4_t const v_rh)
{
	failassert(v_out && m_lh && v_rh, NULL);
	failassert((v_out != v_rh) && (v_out != vx(m_lh)) && (v_out != vy(m_lh)) && (v_out != vz(m_lh)) && (v_out != vw(m_lh)), NULL);
	vx(v_out) = mxx(m_lh) * vx(v_rh) + myx(m_lh) * vy(v_rh) + mzx(m_lh) * vz(v_rh) + mwx(m_lh) * vw(v_rh);
	vy(v_out) = mxy(m_lh) * vx(v_rh) + myy(m_lh) * vy(v_rh) + mzy(m_lh) * vz(v_rh) + mwy(m_lh) * vw(v_rh);
	vz(v_out) = mxz(m_lh) * vx(v_rh) + myz(m_lh) * vy(v_rh) + mzz(m_lh) * vz(v_rh) + mwz(m_lh) * vw(v_rh);
	vw(v_out) = mxw(m_lh) * vx(v_rh) + myw(m_lh) * vy(v_rh) + mzw(m_lh) * vz(v_rh) + mww(m_lh) * vw(v_rh);
	return v_out;
}

CDRAW_INL doubleN_t matTransposeMulVec4d(double4_t v_out, double4x4_t const m_lh, double4_t const v_rh)
{
	failassert(v_out && m_lh && v_rh, NULL);
	failassert((v_out != v_rh) && (v_out != vx(m_lh)) && (v_out != vy(m_lh)) && (v_out != vz(m_lh)) && (v_out != vw(m_lh)), NULL);
	vx(v_out) = vecDot4d(vx(m_lh), v_rh);
	vy(v_out) = vecDot4d(vy(m_lh), v_rh);
	vz(v_out) = vecDot4d(vz(m_lh), v_rh);
	vw(v_out) = vecDot4d(vw(m_lh), v_rh);
	return v_out;
}

CDRAW_INL doubleNx4_t matTransposeMul4d(double4x4_t m_out, double4x4_t const m_lh, double4x4_t const m_rh)
{
	failassert(m_out && m_lh && m_rh, NULL);
	matTransposeMulVec4d(vx(m_out), m_lh, vx(m_rh));
	matTransposeMulVec4d(vy(m_out), m_lh, vy(m_rh));
	matTransposeMulVec4d(vz(m_out), m_lh, vz(m_rh));
	matTransposeMulVec4d(vw(m_out), m_lh, vw(m_rh));
	return m_out;
}

CDRAW_INL doubleNx4_t matAdd4d(double4x4_t m_out, double4x4_t const m_lh, double4x4_t const m_rh)
{
	failassert(m_out && m_lh && m_rh, NULL);
	vecAdd4d(vx(m_out), vx(m_lh), vx(m_rh));
	vecAdd4d(vy(m_out), vy(m_lh), vy(m_rh));
	vecAdd4d(vz(m_out), vz(m_lh), vz(m_rh));
	vecAdd4d(vw(m_out), vw(m_lh), vw(m_rh));
	return m_out;
}

CDRAW_INL doubleNx4_t matSub4d(double4x4_t m_out, double4x4_t const m_lh, double4x4_t const m_rh)
{
	failassert(m_out && m_lh && m_rh, NULL);
	vecSub4d(vx(m_out), vx(m_lh), vx(m_rh));
	vecSub4d(vy(m_out), vy(m_lh), vy(m_rh));
	vecSub4d(vz(m_out), vz(m_lh), vz(m_rh));
	vecSub4d(vw(m_out), vw(m_lh), vw(m_rh));
	return m_out;
}

CDRAW_INL doubleNx4_t matMul4d(double4x4_t m_out, double4x4_t const m_lh, double4x4_t const m_rh)
{
	failassert(m_out && m_lh && m_rh, NULL);
	matMulVec4d(vx(m_out), m_lh, vx(m_rh));
	matMulVec4d(vy(m_out), m_lh, vy(m_rh));
	matMulVec4d(vz(m_out), m_lh, vz(m_rh));
	matMulVec4d(vw(m_out), m_lh, vw(m_rh));
	return m_out;
}

CDRAW_INL doubleNx4_t matDiv4d(double4x4_t m_out, double4x4_t const m_lh, double4x4_t const m_rh)
{
	failassert(m_out && m_lh && m_rh, NULL);
	double4x4_t m_rh_inv;
	matInverse4d(m_rh_inv, m_rh);
	return matMul4d(m_out, m_lh, m_rh_inv);
}

CDRAW_INL doubleNx4_t matAddS4d(double4x4_t m_out, double4x4_t const m_lh, vecd_t const s_rh)
{
	failassert(m_out && m_lh, NULL);
	vecAddS4d(vx(m_out), vx(m_lh), s_rh);
	vecAddS4d(vy(m_out), vy(m_lh), s_rh);
	vecAddS4d(vz(m_out), vz(m_lh), s_rh);
	vecAddS4d(vw(m_out), vw(m_lh), s_rh);
	return m_out;
}

CDRAW_INL doubleNx4_t matSubS4d(double4x4_t m_out, double4x4_t const m_lh, vecd_t const s_rh)
{
	failassert(m_out && m_lh, NULL);
	vecSubS4d(vx(m_out), vx(m_lh), s_rh);
	vecSubS4d(vy(m_out), vy(m_lh), s_rh);
	vecSubS4d(vz(m_out), vz(m_lh), s_rh);
	vecSubS4d(vw(m_out), vw(m_lh), s_rh);
	return m_out;
}

CDRAW_INL doubleNx4_t matMulS4d(double4x4_t m_out, double4x4_t const m_lh, vecd_t const s_rh)
{
	failassert(m_out && m_lh, NULL);
	vecMulS4d(vx(m_out), vx(m_lh), s_rh);
	vecMulS4d(vy(m_out), vy(m_lh), s_rh);
	vecMulS4d(vz(m_out), vz(m_lh), s_rh);
	vecMulS4d(vw(m_out), vw(m_lh), s_rh);
	return m_out;
}

CDRAW_INL doubleNx4_t matDivS4d(double4x4_t m_out, double4x4_t const m_lh, vecd_t const s_rh)
{
	failassert(m_out && m_lh, NULL);
	failassert(scIsNonZeroApproxD(s_rh), matIdentity4d(m_out));
	matMulS4d(m_out, m_lh, (sc1D / s_rh));
	return m_out;
}

CDRAW_INL vecd_t matInvMulVec4d(double4_t v_out, double4x4_t const m_lh, double4_t const v_rh)
{
	failassert(v_out && m_lh && v_rh, sc0D);
	double4x4_t m_lh_inv;
	vecd_t const det = matInverse4d(m_lh_inv, m_lh);
	failassert(scIsNonZeroApproxD(det), vecZero4d(v_out), sc0D);
	matMulVec4d(v_out, m_lh_inv, v_rh);
	return det;
}

CDRAW_INL vecd_t matInvMul4d(double4x4_t m_out, double4x4_t const m_lh, double4x4_t const m_rh)
{
	failassert(m_out && m_lh && m_rh, sc0D);
	double4x4_t m_lh_inv;
	vecd_t const det = matInverse4d(m_lh_inv, m_lh);
	failassert(scIsNonZeroApproxD(det), matIdentity4d(m_out), sc0D);
	matMul4d(m_out, m_lh_inv, m_rh);
	return det;
}


#endif // #if (!(defined _CDRAW_MAT4D_INL_) && (defined _CDRAW_MATRIX_INL_))