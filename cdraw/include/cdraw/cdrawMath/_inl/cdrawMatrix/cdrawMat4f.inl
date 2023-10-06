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

	return m_out;
}

CDRAW_INL floatNx4_t matInit4f(float4x4_t m_out, vecf_t const x, vecf_t const y, vecf_t const z, vecf_t const w)
{
	failassert(m_out, NULL);

	return m_out;
}

CDRAW_INL floatNx4_t matInitS4f(float4x4_t m_out, vecf_t const x)
{
	failassert(m_out, NULL);

	return m_out;
}

CDRAW_INL floatNx4_t matCopy2zw4f(float4x4_t m_out, float2x2_t const xy, vecf_t const z, vecf_t const w)
{
	failassert(m_out && xy, NULL);

	return m_out;
}

CDRAW_INL floatNx4_t matCopy3w4f(float4x4_t m_out, float3x3_t const xyz, vecf_t const w)
{
	failassert(m_out && xyz, NULL);

	return m_out;
}

CDRAW_INL floatNx4_t matCopy4f(float4x4_t m_out, float4x4_t const m)
{
	failassert(m_out && m, NULL);

	return m_out;
}

CDRAW_INL floatNx4_t matCopy4d4f(float4x4_t m_out, double4x4_t const m)
{
	failassert(m_out && m, NULL);

	return m_out;
}

CDRAW_INL floatNx4_t matNegate4f(float4x4_t m_out, float4x4_t const m)
{
	failassert(m_out && m, NULL);

	return m_out;
}

CDRAW_INL floatNx4_t matAdd4f(float4x4_t m_out, float4x4_t const m_lh, float4x4_t const m_rh)
{
	failassert(m_out && m_lh && m_rh, NULL);

	return m_out;
}

CDRAW_INL floatNx4_t matSub4f(float4x4_t m_out, float4x4_t const m_lh, float4x4_t const m_rh)
{
	failassert(m_out && m_lh && m_rh, NULL);

	return m_out;
}

CDRAW_INL floatNx4_t matMul4f(float4x4_t m_out, float4x4_t const m_lh, float4x4_t const m_rh)
{
	failassert(m_out && m_lh && m_rh, NULL);

	return m_out;
}

CDRAW_INL floatNx4_t matDiv4f(float4x4_t m_out, float4x4_t const m_lh, float4x4_t const m_rh)
{
	failassert(m_out && m_lh && m_rh, NULL);

	return m_out;
}

CDRAW_INL floatNx4_t matAddS4f(float4x4_t m_out, float4x4_t const m_lh, vecf_t const s_rh)
{
	failassert(m_out && m_lh, NULL);

	return m_out;
}

CDRAW_INL floatNx4_t matSubS4f(float4x4_t m_out, float4x4_t const m_lh, vecf_t const s_rh)
{
	failassert(m_out && m_lh, NULL);

	return m_out;
}

CDRAW_INL floatNx4_t matMulS4f(float4x4_t m_out, float4x4_t const m_lh, vecf_t const s_rh)
{
	failassert(m_out && m_lh, NULL);

	return m_out;
}

CDRAW_INL floatNx4_t matDivS4f(float4x4_t m_out, float4x4_t const m_lh, vecf_t const s_rh)
{
	failassert(m_out && m_lh, NULL);
	failassert(scIsNonZeroApproxF(s_rh), matIdentity4f(m_out));

	return m_out;
}


#endif // #if (!(defined _CDRAW_MAT4F_INL_) && (defined _CDRAW_MATRIX_INL_))