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
* cdrawMat4f.inl
* Definitions of single-precision 4D matrix functions.
*/

#if (!(defined _CDRAW_MAT4F_INL_) && (defined _CDRAW_MATRIX_INL_))
#define _CDRAW_MAT4F_INL_


CDRAW_INL floatNx4_t matIdentity4f(float4x4_t m_out)
{
	failassert(m_out, NULL);
	vecCopy4f(vx(m_out), vecX4f.v);
	vecCopy4f(vy(m_out), vecY4f.v);
	vecCopy4f(vz(m_out), vecZ4f.v);
	vecCopy4f(vw(m_out), vecW4f.v);
	return m_out;
}

CDRAW_INL floatNx4_t matInit4f(float4x4_t m_out, float4_t const x, float4_t const y, float4_t const z, float4_t const w)
{
	failassert(m_out && x && y && z && w, NULL);
	vecCopy4f(vx(m_out), x);
	vecCopy4f(vy(m_out), y);
	vecCopy4f(vz(m_out), z);
	vecCopy4f(vw(m_out), w);
	return m_out;
}

CDRAW_INL floatNx4_t matInitS4f(float4x4_t m_out, vecf_t const x)
{
	failassert(m_out, NULL);
	vecInit4f(vx(m_out), x, sc0F, sc0F, sc0F);
	vecInit4f(vy(m_out), sc0F, x, sc0F, sc0F);
	vecInit4f(vz(m_out), sc0F, sc0F, x, sc0F);
	vecInit4f(vw(m_out), sc0F, sc0F, sc0F, x);
	return m_out;
}

CDRAW_INL floatNx4_t matCopy2zw4f(float4x4_t m_out, float2x2_t const xy, float4_t const z, float4_t const w)
{
	failassert(m_out && xy && z && w, NULL);
	vecCopy2zw4f(vx(m_out), vx(xy), sc0F, sc0F);
	vecCopy2zw4f(vy(m_out), vy(xy), sc0F, sc0F);
	vecCopy4f(vz(m_out), z);
	vecCopy4f(vw(m_out), w);
	return m_out;
}

CDRAW_INL floatNx4_t matCopy3w4f(float4x4_t m_out, float3x3_t const xyz, float4_t const w)
{
	failassert(m_out && xyz && w, NULL);
	vecCopy3w4f(vx(m_out), vx(xyz), sc0F);
	vecCopy3w4f(vy(m_out), vy(xyz), sc0F);
	vecCopy3w4f(vz(m_out), vz(xyz), sc0F);
	vecCopy4f(vw(m_out), w);
	return m_out;
}

CDRAW_INL floatNx4_t matCopy4f(float4x4_t m_out, float4x4_t const m)
{
	failassert(m_out && m, NULL);
	vecCopy4f(vx(m_out), vx(m));
	vecCopy4f(vy(m_out), vy(m));
	vecCopy4f(vz(m_out), vz(m));
	vecCopy4f(vw(m_out), vw(m));
	return m_out;
}

CDRAW_INL floatNx4_t matCopy4d4f(float4x4_t m_out, double4x4_t const m)
{
	failassert(m_out && m, NULL);
	vecCopy4d4f(vx(m_out), vx(m));
	vecCopy4d4f(vy(m_out), vy(m));
	vecCopy4d4f(vz(m_out), vz(m));
	vecCopy4d4f(vw(m_out), vw(m));
	return m_out;
}

CDRAW_INL floatNx4_t matNegate4f(float4x4_t m_out, float4x4_t const m)
{
	failassert(m_out && m, NULL);
	vecNegate4f(vx(m_out), vx(m));
	vecNegate4f(vy(m_out), vy(m));
	vecNegate4f(vz(m_out), vz(m));
	vecNegate4f(vw(m_out), vw(m));
	return m_out;
}

CDRAW_INL vecf_t matDet4f(float4x4_t const m)
{
	failassert(m, sc0F);
	// 4D determinant is the determinant of 2x2 block determinants
	vecf_t const det_A = (mxx(m) * myy(m) - myx(m) * mxy(m));
	vecf_t const det_C = (mxz(m) * myw(m) - myz(m) * mxw(m));
	vecf_t const det_B = (mzx(m) * mwy(m) - mwx(m) * mzy(m));
	vecf_t const det_D = (mzz(m) * mww(m) - mwz(m) * mzw(m));
	return (det_A * det_D - det_B * det_C);
}

CDRAW_INL vecf_t matDetInv4f(float4x4_t const m)
{
	failassert(m, sc0F);
	vecf_t const det = matDet4f(m);
	failassert(scIsNonZeroApproxF(det), sc0F);
	return (sc1F / det);
}

CDRAW_INL vecb_t matIsIdentity4f(float4x4_t const m)
{
	failassert(m, false);
	return scIsUnityApproxF(mxx(m)) && scIsNonPositiveApproxF(gSq(mxy(m))) && scIsNonPositiveApproxF(gSq(mxz(m))) && scIsNonPositiveApproxF(gSq(mxw(m)))
		&& scIsNonPositiveApproxF(gSq(myx(m))) && scIsUnityApproxF(myy(m)) && scIsNonPositiveApproxF(gSq(myz(m))) && scIsNonPositiveApproxF(gSq(myw(m)))
		&& scIsNonPositiveApproxF(gSq(mzx(m))) && scIsNonPositiveApproxF(gSq(mzy(m))) && scIsUnityApproxF(mzz(m)) && scIsNonPositiveApproxF(gSq(mzw(m)))
		&& scIsNonPositiveApproxF(gSq(mwx(m))) && scIsNonPositiveApproxF(gSq(mwy(m))) && scIsNonPositiveApproxF(gSq(mwz(m))) && scIsUnityApproxF(mww(m));
}

CDRAW_INL vecb_t matIsOrtho4f(vecb_t* isMirrored_out_opt, float4x4_t const m)
{
	failassert(m, false);
	vecb_t const isOrtho = scIsZeroApproxF(vecDot3f(vx(m), vy(m))) && scIsZeroApproxF(vecDot3f(vy(m), vz(m))) && scIsZeroApproxF(vecDot3f(vz(m), vx(m)));
	if (isMirrored_out_opt)
	{
		float3_t compare;
		*isMirrored_out_opt = vecDot3f(vecCross3f(compare, vx(m), vy(m)), vz(m)) < sc0F;
	}
	return isOrtho;
}

CDRAW_INL vecf_t matInverse4f(float4x4_t m_out, float4x4_t const m)
{
	failassert(m_out && m, sc0F);
	// blockwise inversion on 2x2 quadrants
	//	m = / A	 C \
	//		\ B	 D /
	//	m_inv = / (A - (CD^-1)B)^-1			0		   \/	I		-(CD^-1) \
	//			\		0			 (D - (BA^-1)C)^-1 /\ -(BA^-1)		I	 /
	float2x2_t const
		A = { mxx(m), mxy(m), myx(m), myy(m) },
		B = { mxz(m), mxw(m), myz(m), myw(m) },
		C = { mzx(m), mzy(m), mwx(m), mwy(m) },
		D = { mzz(m), mzw(m), mwz(m), mww(m) };
	vecf_t const detN_A = (myx(A) * mxy(A) - mxx(A) * myy(A));
	vecf_t const detN_B = (myx(B) * mxy(B) - mxx(B) * myy(B));
	vecf_t const detN_C = (myx(C) * mxy(C) - mxx(C) * myy(C));
	vecf_t const detN_D = (myx(D) * mxy(D) - mxx(D) * myy(D));
	vecf_t const det = (detN_A * detN_D - detN_B * detN_C);
	failassert(scIsNonZeroApproxF(det), matIdentity4f(m_out), sc0F);
	vecf_t const detInvN_A = (sc1F / detN_A);
	vecf_t const detInvN_D = (sc1F / detN_D);
	float2x2_t const nBAinv = {
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
	vecf_t const detInv_AmCDinvB = sc1F / (mxx(AmCDinvB) * myy(AmCDinvB) - myx(AmCDinvB) * mxy(AmCDinvB));
	vecf_t const detInv_DmBAinvC = sc1F / (mxx(DmBAinvC) * myy(DmBAinvC) - myx(DmBAinvC) * mxy(DmBAinvC));
	float2x2_t const inv_AmCDinvB = {
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

CDRAW_INL floatNx4_t matTranspose4f(float4x4_t m_out, float4x4_t const m)
{
	failassert(m_out && m, NULL);
	vecf_t tmp;
	mxx(m_out) = mxx(m);	myy(m_out) = myy(m);	mzz(m_out) = mzz(m);	mww(m_out) = mww(m);
	tmp = mxy(m);	mxy(m_out) = myx(m);	myx(m_out) = tmp;
	tmp = mxz(m);	mxz(m_out) = mzx(m);	mzx(m_out) = tmp;
	tmp = mxw(m);	mxw(m_out) = mwx(m);	mwx(m_out) = tmp;
	tmp = myz(m);	myz(m_out) = mzy(m);	mzy(m_out) = tmp;
	tmp = myw(m);	myw(m_out) = mwy(m);	mwy(m_out) = tmp;
	tmp = mzw(m);	mzw(m_out) = mwz(m);	mwz(m_out) = tmp;
	return m_out;
}

CDRAW_INL vecf_t matInverseTranspose4f(float4x4_t m_out, float4x4_t const m)
{
	failassert(m_out && m, sc0F);
	return matInverse4f(m_out, matTranspose4f(m_out, m));
}

CDRAW_INL floatN_t matMulVec4f(float4_t v_out, float4x4_t const m_lh, float4_t const v_rh)
{
	failassert(v_out && m_lh && v_rh, NULL);
	failassert((v_out != v_rh) && (v_out != vx(m_lh)) && (v_out != vy(m_lh)) && (v_out != vz(m_lh)) && (v_out != vw(m_lh)), NULL);
	vx(v_out) = mxx(m_lh) * vx(v_rh) + myx(m_lh) * vy(v_rh) + mzx(m_lh) * vz(v_rh) + mwx(m_lh) * vw(v_rh);
	vy(v_out) = mxy(m_lh) * vx(v_rh) + myy(m_lh) * vy(v_rh) + mzy(m_lh) * vz(v_rh) + mwy(m_lh) * vw(v_rh);
	vz(v_out) = mxz(m_lh) * vx(v_rh) + myz(m_lh) * vy(v_rh) + mzz(m_lh) * vz(v_rh) + mwz(m_lh) * vw(v_rh);
	vw(v_out) = mxw(m_lh) * vx(v_rh) + myw(m_lh) * vy(v_rh) + mzw(m_lh) * vz(v_rh) + mww(m_lh) * vw(v_rh);
	return v_out;
}

CDRAW_INL floatN_t matTransposeMulVec4f(float4_t v_out, float4x4_t const m_lh, float4_t const v_rh)
{
	failassert(v_out && m_lh && v_rh, NULL);
	failassert((v_out != v_rh) && (v_out != vx(m_lh)) && (v_out != vy(m_lh)) && (v_out != vz(m_lh)) && (v_out != vw(m_lh)), NULL);
	vx(v_out) = vecDot4f(vx(m_lh), v_rh);
	vy(v_out) = vecDot4f(vy(m_lh), v_rh);
	vz(v_out) = vecDot4f(vz(m_lh), v_rh);
	vw(v_out) = vecDot4f(vw(m_lh), v_rh);
	return v_out;
}

CDRAW_INL floatNx4_t matTransposeMul4f(float4x4_t m_out, float4x4_t const m_lh, float4x4_t const m_rh)
{
	failassert(m_out && m_lh && m_rh, NULL);
	matTransposeMulVec4f(vx(m_out), m_lh, vx(m_rh));
	matTransposeMulVec4f(vy(m_out), m_lh, vy(m_rh));
	matTransposeMulVec4f(vz(m_out), m_lh, vz(m_rh));
	matTransposeMulVec4f(vw(m_out), m_lh, vw(m_rh));
	return m_out;
}

CDRAW_INL floatNx4_t matAdd4f(float4x4_t m_out, float4x4_t const m_lh, float4x4_t const m_rh)
{
	failassert(m_out && m_lh && m_rh, NULL);
	vecAdd4f(vx(m_out), vx(m_lh), vx(m_rh));
	vecAdd4f(vy(m_out), vy(m_lh), vy(m_rh));
	vecAdd4f(vz(m_out), vz(m_lh), vz(m_rh));
	vecAdd4f(vw(m_out), vw(m_lh), vw(m_rh));
	return m_out;
}

CDRAW_INL floatNx4_t matSub4f(float4x4_t m_out, float4x4_t const m_lh, float4x4_t const m_rh)
{
	failassert(m_out && m_lh && m_rh, NULL);
	vecSub4f(vx(m_out), vx(m_lh), vx(m_rh));
	vecSub4f(vy(m_out), vy(m_lh), vy(m_rh));
	vecSub4f(vz(m_out), vz(m_lh), vz(m_rh));
	vecSub4f(vw(m_out), vw(m_lh), vw(m_rh));
	return m_out;
}

CDRAW_INL floatNx4_t matMul4f(float4x4_t m_out, float4x4_t const m_lh, float4x4_t const m_rh)
{
	failassert(m_out && m_lh && m_rh, NULL);
	matMulVec4f(vx(m_out), m_lh, vx(m_rh));
	matMulVec4f(vy(m_out), m_lh, vy(m_rh));
	matMulVec4f(vz(m_out), m_lh, vz(m_rh));
	matMulVec4f(vw(m_out), m_lh, vw(m_rh));
	return m_out;
}

CDRAW_INL floatNx4_t matDiv4f(float4x4_t m_out, float4x4_t const m_lh, float4x4_t const m_rh)
{
	failassert(m_out && m_lh && m_rh, NULL);
	float4x4_t m_rh_inv;
	matInverse4f(m_rh_inv, m_rh);
	return matMul4f(m_out, m_lh, m_rh_inv);
}

CDRAW_INL floatNx4_t matAddS4f(float4x4_t m_out, float4x4_t const m_lh, vecf_t const s_rh)
{
	failassert(m_out && m_lh, NULL);
	vecAddS4f(vx(m_out), vx(m_lh), s_rh);
	vecAddS4f(vy(m_out), vy(m_lh), s_rh);
	vecAddS4f(vz(m_out), vz(m_lh), s_rh);
	vecAddS4f(vw(m_out), vw(m_lh), s_rh);
	return m_out;
}

CDRAW_INL floatNx4_t matSubS4f(float4x4_t m_out, float4x4_t const m_lh, vecf_t const s_rh)
{
	failassert(m_out && m_lh, NULL);
	vecSubS4f(vx(m_out), vx(m_lh), s_rh);
	vecSubS4f(vy(m_out), vy(m_lh), s_rh);
	vecSubS4f(vz(m_out), vz(m_lh), s_rh);
	vecSubS4f(vw(m_out), vw(m_lh), s_rh);
	return m_out;
}

CDRAW_INL floatNx4_t matMulS4f(float4x4_t m_out, float4x4_t const m_lh, vecf_t const s_rh)
{
	failassert(m_out && m_lh, NULL);
	vecMulS4f(vx(m_out), vx(m_lh), s_rh);
	vecMulS4f(vy(m_out), vy(m_lh), s_rh);
	vecMulS4f(vz(m_out), vz(m_lh), s_rh);
	vecMulS4f(vw(m_out), vw(m_lh), s_rh);
	return m_out;
}

CDRAW_INL floatNx4_t matDivS4f(float4x4_t m_out, float4x4_t const m_lh, vecf_t const s_rh)
{
	failassert(m_out && m_lh, NULL);
	failassert(scIsNonZeroApproxF(s_rh), matIdentity4f(m_out));
	matMulS4f(m_out, m_lh, (sc1F / s_rh));
	return m_out;
}

CDRAW_INL vecf_t matInvMulVec4f(float4_t v_out, float4x4_t const m_lh, float4_t const v_rh)
{
	failassert(v_out && m_lh && v_rh, sc0F);
	float4x4_t m_lh_inv;
	vecf_t const det = matInverse4f(m_lh_inv, m_lh);
	failassert(scIsNonZeroApproxF(det), vecZero4f(v_out), sc0F);
	matMulVec4f(v_out, m_lh_inv, v_rh);
	return det;
}

CDRAW_INL vecf_t matInvMul4f(float4x4_t m_out, float4x4_t const m_lh, float4x4_t const m_rh)
{
	failassert(m_out && m_lh && m_rh, sc0F);
	float4x4_t m_lh_inv;
	vecf_t const det = matInverse4f(m_lh_inv, m_lh);
	failassert(scIsNonZeroApproxF(det), matIdentity4f(m_out), sc0F);
	matMul4f(m_out, m_lh_inv, m_rh);
	return det;
}


#endif // #if (!(defined _CDRAW_MAT4F_INL_) && (defined _CDRAW_MATRIX_INL_))