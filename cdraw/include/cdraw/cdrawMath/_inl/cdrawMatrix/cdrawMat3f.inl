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
* cdrawMat3f.inl
* Definitions of single-precision 3D matrix functions.
*/

#if (!(defined _CDRAW_MAT3F_INL_) && (defined _CDRAW_MATRIX_INL_))
#define _CDRAW_MAT3F_INL_


CDRAW_INL floatNx3_t matIdentity3f(float3x3_t m_out)
{
	failassert(m_out, NULL);
	vecCopy3f(vx(m_out), vecX4f.v);
	vecCopy3f(vy(m_out), vecY4f.v);
	vecCopy3f(vz(m_out), vecZ4f.v);
	return m_out;
}

CDRAW_INL floatNx3_t matInit3f(float3x3_t m_out, float3_t const x, float3_t const y, float3_t const z)
{
	failassert(m_out && x && y && z, NULL);
	vecCopy3f(vx(m_out), x);
	vecCopy3f(vy(m_out), y);
	vecCopy3f(vz(m_out), z);
	return m_out;
}

CDRAW_INL floatNx3_t matInitS3f(float3x3_t m_out, vecf_t const x)
{
	failassert(m_out, NULL);
	vecInit3f(vx(m_out), x, sc0F, sc0F);
	vecInit3f(vy(m_out), sc0F, x, sc0F);
	vecInit3f(vz(m_out), sc0F, sc0F, x);
	return m_out;
}

CDRAW_INL floatNx3_t matCopy2z3f(float3x3_t m_out, float2x2_t const xy, float3_t const z)
{
	failassert(m_out && xy && z, NULL);
	vecCopy2z3f(vx(m_out), vx(xy), sc0F);
	vecCopy2z3f(vy(m_out), vy(xy), sc0F);
	vecCopy3f(vz(m_out), z);
	return m_out;
}

CDRAW_INL floatNx3_t matCopy43f(float3x3_t m_out, float4x4_t const m)
{
	failassert(m_out && m, NULL);
	vecCopy3f(vx(m_out), vx(m));
	vecCopy3f(vy(m_out), vy(m));
	vecCopy3f(vz(m_out), vz(m));
	return m_out;
}

CDRAW_INL floatNx3_t matCopy3f(float3x3_t m_out, float3x3_t const m)
{
	failassert(m_out && m, NULL);
	vecCopy3f(vx(m_out), vx(m));
	vecCopy3f(vy(m_out), vy(m));
	vecCopy3f(vz(m_out), vz(m));
	return m_out;
}

CDRAW_INL floatNx3_t matCopy3d3f(float3x3_t m_out, double3x3_t const m)
{
	failassert(m_out && m, NULL);
	vecCopy3d3f(vx(m_out), vx(m));
	vecCopy3d3f(vy(m_out), vy(m));
	vecCopy3d3f(vz(m_out), vz(m));
	return m_out;
}

CDRAW_INL floatNx3_t matNegate3f(float3x3_t m_out, float3x3_t const m)
{
	failassert(m_out && m, NULL);
	vecNegate3f(vx(m_out), vx(m));
	vecNegate3f(vy(m_out), vy(m));
	vecNegate3f(vz(m_out), vz(m));
	return m_out;
}

CDRAW_INL vecf_t matDet3f(float3x3_t const m)
{
	failassert(m, sc0F);
	// 3D determinant is the scalar triple product
	float3_t cross;
	return vecDot3f(vecCross3f(cross, vx(m), vy(m)), vz(m));
}

CDRAW_INL vecf_t matDetInv3f(float3x3_t const m)
{
	failassert(m, sc0F);
	vecf_t const det = matDet3f(m);
	failassert(scIsNonZeroApproxF(det), sc0F);
	return (sc1F / det);
}

CDRAW_INL vecb_t matIsIdentity3f(float3x3_t const m)
{
	failassert(m, false);
	return scIsUnityApproxF(mxx(m)) && scIsNonPositiveApproxF(gSq(mxy(m))) && scIsNonPositiveApproxF(gSq(mxz(m)))
		&& scIsNonPositiveApproxF(gSq(myx(m))) && scIsUnityApproxF(myy(m)) && scIsNonPositiveApproxF(gSq(myz(m)))
		&& scIsNonPositiveApproxF(gSq(mzx(m))) && scIsNonPositiveApproxF(gSq(mzy(m))) && scIsUnityApproxF(mzz(m));
}

CDRAW_INL vecb_t matIsOrtho3f(vecb_t* isMirrored_out_opt, float3x3_t const m)
{
	failassert(m, false);
	vecb_t const isOrtho = scIsZeroApproxF(vecDot3f(vx(m), vy(m))) && scIsZeroApproxF(vecDot3f(vy(m), vz(m))) && scIsZeroApproxF(vecDot3f(vz(m), vx(m)));
	if (isMirrored_out_opt)
	{
		*isMirrored_out_opt = matDet3f(m) < sc0F;
	}
	return isOrtho;
}

CDRAW_INL vecf_t matInverse3f(float3x3_t m_out, float3x3_t const m)
{
	failassert(m_out && m, sc0F);
	// 3D inverse is transpose of column cross products
	vecf_t const det = matDet3f(m);
	failassert(scIsNonZeroApproxF(det), matIdentity3f(m_out), sc0F);
	vecf_t const detInv = (sc1F / det);
	float3_t BxC, CxA, AxB;
	vecCross3f(BxC, vy(m), vz(m));
	vecCross3f(CxA, vz(m), vx(m));
	vecCross3f(AxB, vx(m), vy(m));
	mxx(m_out) = vx(BxC) * detInv;	myx(m_out) = vy(BxC) * detInv;	mzx(m_out) = vz(BxC) * detInv;
	mxy(m_out) = vx(CxA) * detInv;	myy(m_out) = vy(CxA) * detInv;	mzy(m_out) = vz(CxA) * detInv;
	mxz(m_out) = vx(AxB) * detInv;	myz(m_out) = vy(AxB) * detInv;	mzz(m_out) = vz(AxB) * detInv;
	return det;
}

CDRAW_INL floatNx3_t matTranspose3f(float3x3_t m_out, float3x3_t const m)
{
	failassert(m_out && m, NULL);
	vecf_t tmp;
	mxx(m_out) = mxx(m);	myy(m_out) = myy(m);	mzz(m_out) = mzz(m);
	tmp = mxy(m);	mxy(m_out) = myx(m);	myx(m_out) = tmp;
	tmp = mxz(m);	mxz(m_out) = mzx(m);	mzx(m_out) = tmp;
	tmp = myz(m);	myz(m_out) = mzy(m);	mzy(m_out) = tmp;
	return m_out;
}

CDRAW_INL vecf_t matInverseTranspose3f(float3x3_t m_out, float3x3_t const m)
{
	failassert(m_out && m, sc0F);
	// 3D inverse transpose is column cross products
	vecf_t const det = matDet3f(m);
	failassert(scIsNonZeroApproxF(det), matIdentity3f(m_out), sc0F);
	vecf_t const detInv = (sc1F / det);
	vecMulS3f(vx(m_out), vecCross3f(vx(m_out), vy(m), vz(m)), detInv);
	vecMulS3f(vy(m_out), vecCross3f(vy(m_out), vz(m), vx(m)), detInv);
	vecMulS3f(vz(m_out), vecCross3f(vz(m_out), vx(m), vy(m)), detInv);
	return det;
}

CDRAW_INL floatN_t matMulVec3f(float3_t v_out, float3x3_t const m_lh, float3_t const v_rh)
{
	failassert(v_out && m_lh && v_rh, NULL);
	failassert((v_out != v_rh) && (v_out != vx(m_lh)) && (v_out != vy(m_lh)) && (v_out != vz(m_lh)), NULL);
	vx(v_out) = mxx(m_lh) * vx(v_rh) + myx(m_lh) * vy(v_rh) + mzx(m_lh) * vz(v_rh);
	vy(v_out) = mxy(m_lh) * vx(v_rh) + myy(m_lh) * vy(v_rh) + mzy(m_lh) * vz(v_rh);
	vz(v_out) = mxz(m_lh) * vx(v_rh) + myz(m_lh) * vy(v_rh) + mzz(m_lh) * vz(v_rh);
	return v_out;
}

CDRAW_INL floatN_t matTransposeMulVec3f(float3_t v_out, float3x3_t const m_lh, float3_t const v_rh)
{
	failassert(v_out && m_lh && v_rh, NULL);
	failassert((v_out != v_rh) && (v_out != vx(m_lh)) && (v_out != vy(m_lh)) && (v_out != vz(m_lh)), NULL);
	vx(v_out) = vecDot3f(vx(m_lh), v_rh);
	vy(v_out) = vecDot3f(vy(m_lh), v_rh);
	vz(v_out) = vecDot3f(vz(m_lh), v_rh);
	return v_out;
}

CDRAW_INL floatNx3_t matTransposeMul3f(float3x3_t m_out, float3x3_t const m_lh, float3x3_t const m_rh)
{
	failassert(m_out && m_lh && m_rh, NULL);
	matTransposeMulVec3f(vx(m_out), m_lh, vx(m_rh));
	matTransposeMulVec3f(vy(m_out), m_lh, vy(m_rh));
	matTransposeMulVec3f(vz(m_out), m_lh, vz(m_rh));
	return m_out;
}

CDRAW_INL floatNx3_t matAdd3f(float3x3_t m_out, float3x3_t const m_lh, float3x3_t const m_rh)
{
	failassert(m_out && m_lh && m_rh, NULL);
	vecAdd3f(vx(m_out), vx(m_lh), vx(m_rh));
	vecAdd3f(vy(m_out), vy(m_lh), vy(m_rh));
	vecAdd3f(vz(m_out), vz(m_lh), vz(m_rh));
	return m_out;
}

CDRAW_INL floatNx3_t matSub3f(float3x3_t m_out, float3x3_t const m_lh, float3x3_t const m_rh)
{
	failassert(m_out && m_lh && m_rh, NULL);
	vecSub3f(vx(m_out), vx(m_lh), vx(m_rh));
	vecSub3f(vy(m_out), vy(m_lh), vy(m_rh));
	vecSub3f(vz(m_out), vz(m_lh), vz(m_rh));
	return m_out;
}

CDRAW_INL floatNx3_t matMul3f(float3x3_t m_out, float3x3_t const m_lh, float3x3_t const m_rh)
{
	failassert(m_out && m_lh && m_rh, NULL);
	matMulVec3f(vx(m_out), m_lh, vx(m_rh));
	matMulVec3f(vy(m_out), m_lh, vy(m_rh));
	matMulVec3f(vz(m_out), m_lh, vz(m_rh));
	return m_out;
}

CDRAW_INL floatNx3_t matDiv3f(float3x3_t m_out, float3x3_t const m_lh, float3x3_t const m_rh)
{
	failassert(m_out && m_lh && m_rh, NULL);
	float3x3_t m_rh_inv;
	matInverse3f(m_rh_inv, m_rh);
	return matMul3f(m_out, m_lh, m_rh_inv);
}

CDRAW_INL floatNx3_t matAddS3f(float3x3_t m_out, float3x3_t const m_lh, vecf_t const s_rh)
{
	failassert(m_out && m_lh, NULL);
	vecAddS3f(vx(m_out), vx(m_lh), s_rh);
	vecAddS3f(vy(m_out), vy(m_lh), s_rh);
	vecAddS3f(vz(m_out), vz(m_lh), s_rh);
	return m_out;
}

CDRAW_INL floatNx3_t matSubS3f(float3x3_t m_out, float3x3_t const m_lh, vecf_t const s_rh)
{
	failassert(m_out && m_lh, NULL);
	vecSubS3f(vx(m_out), vx(m_lh), s_rh);
	vecSubS3f(vy(m_out), vy(m_lh), s_rh);
	vecSubS3f(vz(m_out), vz(m_lh), s_rh);
	return m_out;
}

CDRAW_INL floatNx3_t matMulS3f(float3x3_t m_out, float3x3_t const m_lh, vecf_t const s_rh)
{
	failassert(m_out && m_lh, NULL);
	vecMulS3f(vx(m_out), vx(m_lh), s_rh);
	vecMulS3f(vy(m_out), vy(m_lh), s_rh);
	vecMulS3f(vz(m_out), vz(m_lh), s_rh);
	return m_out;
}

CDRAW_INL floatNx3_t matDivS3f(float3x3_t m_out, float3x3_t const m_lh, vecf_t const s_rh)
{
	failassert(m_out && m_lh, NULL);
	failassert(scIsNonZeroApproxF(s_rh), matIdentity3f(m_out));
	matMulS3f(m_out, m_lh, (sc1F / s_rh));
	return m_out;
}

CDRAW_INL vecf_t matInvMulVec3f(float3_t v_out, float3x3_t const m_lh, float3_t const v_rh)
{
	failassert(v_out && m_lh && v_rh, sc0F);
	failassert((v_out != vx(m_lh)) && (v_out != vy(m_lh)) && (v_out != vz(m_lh)), sc0F);
	vecf_t const det = matDet3f(m_lh);
	failassert(scIsNonZeroApproxF(det), vecZero3f(v_out), sc0F);
	vecf_t const detInv = (sc1F / det);
	float3_t cross;
	vx(v_out) = vecDot3f(vecCross3f(cross, vy(m_lh), vz(m_lh)), v_rh) * detInv;
	vy(v_out) = vecDot3f(vecCross3f(cross, vz(m_lh), vx(m_lh)), v_rh) * detInv;
	vz(v_out) = vecDot3f(vecCross3f(cross, vx(m_lh), vy(m_lh)), v_rh) * detInv;
	return det;
}

CDRAW_INL vecf_t matInvMul3f(float3x3_t m_out, float3x3_t const m_lh, float3x3_t const m_rh)
{
	failassert(m_out && m_lh && m_rh, sc0F);
	float3x3_t m_lh_inv;
	vecf_t const det = matInverseTranspose3f(m_lh_inv, m_lh);
	failassert(scIsNonZeroApproxF(det), matIdentity3f(m_out), sc0F);
	matTransposeMul3f(m_out, m_lh_inv, m_rh);
	return det;
}


#endif // #if (!(defined _CDRAW_MAT3F_INL_) && (defined _CDRAW_MATRIX_INL_))