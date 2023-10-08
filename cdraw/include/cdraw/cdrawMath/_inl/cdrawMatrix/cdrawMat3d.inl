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
* cdrawMat3d.inl
* Definitions of double-precision 3D matrix functions.
*/

#if (!(defined _CDRAW_MAT3D_INL_) && (defined _CDRAW_MATRIX_INL_))
#define _CDRAW_MAT3D_INL_


CDRAW_INL doubleNx3_t matIdentity3d(double3x3_t m_out)
{
	failassert(m_out, NULL);
	vecCopy3d(vx(m_out), vecX4d.v);
	vecCopy3d(vy(m_out), vecY4d.v);
	vecCopy3d(vz(m_out), vecZ4d.v);
	return m_out;
}

CDRAW_INL doubleNx3_t matInit3d(double3x3_t m_out, double3_t const x, double3_t const y, double3_t const z)
{
	failassert(m_out && x && y && z, NULL);
	vecCopy3d(vx(m_out), x);
	vecCopy3d(vy(m_out), y);
	vecCopy3d(vz(m_out), z);
	return m_out;
}

CDRAW_INL doubleNx3_t matInitS3d(double3x3_t m_out, vecd_t const x)
{
	failassert(m_out, NULL);
	vecInit3d(vx(m_out), x, sc0D, sc0D);
	vecInit3d(vy(m_out), sc0D, x, sc0D);
	vecInit3d(vz(m_out), sc0D, sc0D, x);
	return m_out;
}

CDRAW_INL doubleNx3_t matCopy2z3d(double3x3_t m_out, double2x2_t const xy, double3_t const z)
{
	failassert(m_out && xy && z, NULL);
	vecCopy2z3d(vx(m_out), vx(xy), sc0D);
	vecCopy2z3d(vy(m_out), vy(xy), sc0D);
	vecCopy3d(vz(m_out), z);
	return m_out;
}

CDRAW_INL doubleNx3_t matCopy43d(double3x3_t m_out, double4x4_t const m)
{
	failassert(m_out && m, NULL);
	vecCopy3d(vx(m_out), vx(m));
	vecCopy3d(vy(m_out), vy(m));
	vecCopy3d(vz(m_out), vz(m));
	return m_out;
}

CDRAW_INL doubleNx3_t matCopy3d(double3x3_t m_out, double3x3_t const m)
{
	failassert(m_out && m, NULL);
	vecCopy3d(vx(m_out), vx(m));
	vecCopy3d(vy(m_out), vy(m));
	vecCopy3d(vz(m_out), vz(m));
	return m_out;
}

CDRAW_INL doubleNx3_t matCopy3f3d(double3x3_t m_out, float3x3_t const m)
{
	failassert(m_out && m, NULL);
	vecCopy3f3d(vx(m_out), vx(m));
	vecCopy3f3d(vy(m_out), vy(m));
	vecCopy3f3d(vz(m_out), vz(m));
	return m_out;
}

CDRAW_INL doubleNx3_t matNegate3d(double3x3_t m_out, double3x3_t const m)
{
	failassert(m_out && m, NULL);
	vecNegate3d(vx(m_out), vx(m));
	vecNegate3d(vy(m_out), vy(m));
	vecNegate3d(vz(m_out), vz(m));
	return m_out;
}

CDRAW_INL vecd_t matDet3d(double3x3_t const m)
{
	failassert(m, sc0D);
	// 3D determinant is the scalar triple product
	double3_t cross;
	return vecDot3d(vecCross3d(cross, vx(m), vy(m)), vz(m));
}

CDRAW_INL vecd_t matDetInv3d(double3x3_t const m)
{
	failassert(m, sc0D);
	vecd_t const det = matDet3d(m);
	failassert(scIsNonZeroApproxD(det), sc0D);
	return (sc1D / det);
}

CDRAW_INL vecb_t matIsIdentity3d(double3x3_t const m)
{
	failassert(m, false);
	return scIsUnityApproxD(mxx(m)) && scIsNonPositiveApproxD(gSq(mxy(m))) && scIsNonPositiveApproxD(gSq(mxz(m)))
		&& scIsNonPositiveApproxD(gSq(myx(m))) && scIsUnityApproxD(myy(m)) && scIsNonPositiveApproxD(gSq(myz(m)))
		&& scIsNonPositiveApproxD(gSq(mzx(m))) && scIsNonPositiveApproxD(gSq(mzy(m))) && scIsUnityApproxD(mzz(m));
}

CDRAW_INL vecb_t matIsOrtho3d(vecb_t* isMirrored_out_opt, double3x3_t const m)
{
	failassert(m, false);
	vecb_t const isOrtho = scIsZeroApproxD(vecDot3d(vx(m), vy(m))) && scIsZeroApproxD(vecDot3d(vy(m), vz(m))) && scIsZeroApproxD(vecDot3d(vz(m), vx(m)));
	if (isMirrored_out_opt)
	{
		*isMirrored_out_opt = matDet3d(m) < sc0D;
	}
	return isOrtho;
}

CDRAW_INL vecd_t matInverse3d(double3x3_t m_out, double3x3_t const m)
{
	failassert(m_out && m, sc0D);
	// 3D inverse is transpose of column cross products
	vecd_t const det = matDet3d(m);
	failassert(scIsNonZeroApproxD(det), matIdentity3d(m_out), sc0D);
	vecd_t const detInv = (sc1D / det);
	double3_t BxC, CxA, AxB;
	vecCross3d(BxC, vy(m), vz(m));
	vecCross3d(CxA, vz(m), vx(m));
	vecCross3d(AxB, vx(m), vy(m));
	mxx(m_out) = vx(BxC) * detInv;	myx(m_out) = vy(BxC) * detInv;	mzx(m_out) = vz(BxC) * detInv;
	mxy(m_out) = vx(CxA) * detInv;	myy(m_out) = vy(CxA) * detInv;	mzy(m_out) = vz(CxA) * detInv;
	mxz(m_out) = vx(AxB) * detInv;	myz(m_out) = vy(AxB) * detInv;	mzz(m_out) = vz(AxB) * detInv;
	return det;
}

CDRAW_INL doubleNx3_t matTranspose3d(double3x3_t m_out, double3x3_t const m)
{
	failassert(m_out && m, NULL);
	vecd_t tmp;
	mxx(m_out) = mxx(m);	myy(m_out) = myy(m);	mzz(m_out) = mzz(m);
	tmp = mxy(m);	mxy(m_out) = myx(m);	myx(m_out) = tmp;
	tmp = mxz(m);	mxz(m_out) = mzx(m);	mzx(m_out) = tmp;
	tmp = myz(m);	myz(m_out) = mzy(m);	mzy(m_out) = tmp;
	return m_out;
}

CDRAW_INL vecd_t matInverseTranspose3d(double3x3_t m_out, double3x3_t const m)
{
	failassert(m_out && m, sc0D);
	// 3D inverse transpose is column cross products
	vecd_t const det = matDet3d(m);
	failassert(scIsNonZeroApproxD(det), matIdentity3d(m_out), sc0D);
	vecd_t const detInv = (sc1D / det);
	vecMulS3d(vx(m_out), vecCross3d(vx(m_out), vy(m), vz(m)), detInv);
	vecMulS3d(vy(m_out), vecCross3d(vy(m_out), vz(m), vx(m)), detInv);
	vecMulS3d(vz(m_out), vecCross3d(vz(m_out), vx(m), vy(m)), detInv);
	return det;
}

CDRAW_INL doubleN_t matMulVec3d(double3_t v_out, double3x3_t const m_lh, double3_t const v_rh)
{
	failassert(v_out && m_lh && v_rh, NULL);
	failassert((v_out != v_rh) && (v_out != vx(m_lh)) && (v_out != vy(m_lh)) && (v_out != vz(m_lh)), NULL);
	vx(v_out) = mxx(m_lh) * vx(v_rh) + myx(m_lh) * vy(v_rh) + mzx(m_lh) * vz(v_rh);
	vy(v_out) = mxy(m_lh) * vx(v_rh) + myy(m_lh) * vy(v_rh) + mzy(m_lh) * vz(v_rh);
	vz(v_out) = mxz(m_lh) * vx(v_rh) + myz(m_lh) * vy(v_rh) + mzz(m_lh) * vz(v_rh);
	return v_out;
}

CDRAW_INL doubleN_t matTransposeMulVec3d(double3_t v_out, double3x3_t const m_lh, double3_t const v_rh)
{
	failassert(v_out && m_lh && v_rh, NULL);
	failassert((v_out != v_rh) && (v_out != vx(m_lh)) && (v_out != vy(m_lh)) && (v_out != vz(m_lh)), NULL);
	vx(v_out) = vecDot3d(vx(m_lh), v_rh);
	vy(v_out) = vecDot3d(vy(m_lh), v_rh);
	vz(v_out) = vecDot3d(vz(m_lh), v_rh);
	return v_out;
}

CDRAW_INL doubleNx3_t matTransposeMul3d(double3x3_t m_out, double3x3_t const m_lh, double3x3_t const m_rh)
{
	failassert(m_out && m_lh && m_rh, NULL);
	matTransposeMulVec3d(vx(m_out), m_lh, vx(m_rh));
	matTransposeMulVec3d(vy(m_out), m_lh, vy(m_rh));
	matTransposeMulVec3d(vz(m_out), m_lh, vz(m_rh));
	return m_out;
}

CDRAW_INL doubleNx3_t matAdd3d(double3x3_t m_out, double3x3_t const m_lh, double3x3_t const m_rh)
{
	failassert(m_out && m_lh && m_rh, NULL);
	vecAdd3d(vx(m_out), vx(m_lh), vx(m_rh));
	vecAdd3d(vy(m_out), vy(m_lh), vy(m_rh));
	vecAdd3d(vz(m_out), vz(m_lh), vz(m_rh));
	return m_out;
}

CDRAW_INL doubleNx3_t matSub3d(double3x3_t m_out, double3x3_t const m_lh, double3x3_t const m_rh)
{
	failassert(m_out && m_lh && m_rh, NULL);
	vecSub3d(vx(m_out), vx(m_lh), vx(m_rh));
	vecSub3d(vy(m_out), vy(m_lh), vy(m_rh));
	vecSub3d(vz(m_out), vz(m_lh), vz(m_rh));
	return m_out;
}

CDRAW_INL doubleNx3_t matMul3d(double3x3_t m_out, double3x3_t const m_lh, double3x3_t const m_rh)
{
	failassert(m_out && m_lh && m_rh, NULL);
	matMulVec3d(vx(m_out), m_lh, vx(m_rh));
	matMulVec3d(vy(m_out), m_lh, vy(m_rh));
	matMulVec3d(vz(m_out), m_lh, vz(m_rh));
	return m_out;
}

CDRAW_INL doubleNx3_t matDiv3d(double3x3_t m_out, double3x3_t const m_lh, double3x3_t const m_rh)
{
	failassert(m_out && m_lh && m_rh, NULL);
	double3x3_t m_rh_inv;
	matInverse3d(m_rh_inv, m_rh);
	return matMul3d(m_out, m_lh, m_rh_inv);
}

CDRAW_INL doubleNx3_t matAddS3d(double3x3_t m_out, double3x3_t const m_lh, vecd_t const s_rh)
{
	failassert(m_out && m_lh, NULL);
	vecAddS3d(vx(m_out), vx(m_lh), s_rh);
	vecAddS3d(vy(m_out), vy(m_lh), s_rh);
	vecAddS3d(vz(m_out), vz(m_lh), s_rh);
	return m_out;
}

CDRAW_INL doubleNx3_t matSubS3d(double3x3_t m_out, double3x3_t const m_lh, vecd_t const s_rh)
{
	failassert(m_out && m_lh, NULL);
	vecSubS3d(vx(m_out), vx(m_lh), s_rh);
	vecSubS3d(vy(m_out), vy(m_lh), s_rh);
	vecSubS3d(vz(m_out), vz(m_lh), s_rh);
	return m_out;
}

CDRAW_INL doubleNx3_t matMulS3d(double3x3_t m_out, double3x3_t const m_lh, vecd_t const s_rh)
{
	failassert(m_out && m_lh, NULL);
	vecMulS3d(vx(m_out), vx(m_lh), s_rh);
	vecMulS3d(vy(m_out), vy(m_lh), s_rh);
	vecMulS3d(vz(m_out), vz(m_lh), s_rh);
	return m_out;
}

CDRAW_INL doubleNx3_t matDivS3d(double3x3_t m_out, double3x3_t const m_lh, vecd_t const s_rh)
{
	failassert(m_out && m_lh, NULL);
	failassert(scIsNonZeroApproxD(s_rh), matIdentity3d(m_out));
	matMulS3d(m_out, m_lh, (sc1D / s_rh));
	return m_out;
}

CDRAW_INL vecd_t matInvMulVec3d(double3_t v_out, double3x3_t const m_lh, double3_t const v_rh)
{
	failassert(v_out && m_lh && v_rh, sc0D);
	failassert((v_out != vx(m_lh)) && (v_out != vy(m_lh)) && (v_out != vz(m_lh)), sc0D);
	vecd_t const det = matDet3d(m_lh);
	failassert(scIsNonZeroApproxD(det), vecZero3d(v_out), sc0D);
	vecd_t const detInv = (sc1D / det);
	double3_t cross;
	vx(v_out) = vecDot3d(vecCross3d(cross, vy(m_lh), vz(m_lh)), v_rh) * detInv;
	vy(v_out) = vecDot3d(vecCross3d(cross, vz(m_lh), vx(m_lh)), v_rh) * detInv;
	vz(v_out) = vecDot3d(vecCross3d(cross, vx(m_lh), vy(m_lh)), v_rh) * detInv;
	return det;
}

CDRAW_INL vecd_t matInvMul3d(double3x3_t m_out, double3x3_t const m_lh, double3x3_t const m_rh)
{
	failassert(m_out && m_lh && m_rh, sc0D);
	double3x3_t m_lh_inv;
	vecd_t const det = matInverseTranspose3d(m_lh_inv, m_lh);
	failassert(scIsNonZeroApproxD(det), matIdentity3d(m_out), sc0D);
	matTransposeMul3d(m_out, m_lh_inv, m_rh);
	return det;
}


#endif // #if (!(defined _CDRAW_MAT3D_INL_) && (defined _CDRAW_MATRIX_INL_))