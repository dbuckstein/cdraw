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

	return m_out;
}

CDRAW_INL doubleNx4_t matInit4d(double4x4_t m_out, vecd_t const x, vecd_t const y, vecd_t const z, vecd_t const w)
{
	failassert(m_out, NULL);

	return m_out;
}

CDRAW_INL doubleNx4_t matInitS4d(double4x4_t m_out, vecd_t const x)
{
	failassert(m_out, NULL);

	return m_out;
}

CDRAW_INL doubleNx4_t matCopy2zw4d(double4x4_t m_out, double2x2_t const xy, vecd_t const z, vecd_t const w)
{
	failassert(m_out && xy, NULL);

	return m_out;
}

CDRAW_INL doubleNx4_t matCopy3w4d(double4x4_t m_out, double3x3_t const xyz, vecd_t const w)
{
	failassert(m_out && xyz, NULL);

	return m_out;
}

CDRAW_INL doubleNx4_t matCopy4d(double4x4_t m_out, double4x4_t const m)
{
	failassert(m_out && m, NULL);

	return m_out;
}

CDRAW_INL doubleNx4_t matCopy4f4d(double4x4_t m_out, float4x4_t const m)
{
	failassert(m_out && m, NULL);

	return m_out;
}

CDRAW_INL doubleNx4_t matNegate4d(double4x4_t m_out, double4x4_t const m)
{
	failassert(m_out && m, NULL);

	return m_out;
}

CDRAW_INL doubleNx4_t matAdd4d(double4x4_t m_out, double4x4_t const m_lh, double4x4_t const m_rh)
{
	failassert(m_out && m_lh && m_rh, NULL);

	return m_out;
}

CDRAW_INL doubleNx4_t matSub4d(double4x4_t m_out, double4x4_t const m_lh, double4x4_t const m_rh)
{
	failassert(m_out && m_lh && m_rh, NULL);

	return m_out;
}

CDRAW_INL doubleNx4_t matMul4d(double4x4_t m_out, double4x4_t const m_lh, double4x4_t const m_rh)
{
	failassert(m_out && m_lh && m_rh, NULL);

	return m_out;
}

CDRAW_INL doubleNx4_t matDiv4d(double4x4_t m_out, double4x4_t const m_lh, double4x4_t const m_rh)
{
	failassert(m_out && m_lh && m_rh, NULL);

	return m_out;
}

CDRAW_INL doubleNx4_t matAddS4d(double4x4_t m_out, double4x4_t const m_lh, vecd_t const s_rh)
{
	failassert(m_out && m_lh, NULL);

	return m_out;
}

CDRAW_INL doubleNx4_t matSubS4d(double4x4_t m_out, double4x4_t const m_lh, vecd_t const s_rh)
{
	failassert(m_out && m_lh, NULL);

	return m_out;
}

CDRAW_INL doubleNx4_t matMulS4d(double4x4_t m_out, double4x4_t const m_lh, vecd_t const s_rh)
{
	failassert(m_out && m_lh, NULL);

	return m_out;
}

CDRAW_INL doubleNx4_t matDivS4d(double4x4_t m_out, double4x4_t const m_lh, vecd_t const s_rh)
{
	failassert(m_out && m_lh, NULL);
	failassert(scIsNonZeroApproxD(s_rh), matIdentity4d(m_out));

	return m_out;
}


#endif // #if (!(defined _CDRAW_MAT4D_INL_) && (defined _CDRAW_MATRIX_INL_))