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
* cdrawMat2f.h
* Declaration of single-precision 2D matrix functions.
*/

#if (!(defined _CDRAW_MAT2F_H_) && (defined _CDRAW_MATRIX_H_))
#define _CDRAW_MAT2F_H_


#ifdef __cplusplus
extern "C" {
#endif // #ifdef __cplusplus

	/// <summary>
	/// Initialize matrix to identity.
	/// </summary>
	/// <param name="m_out">Result matrix.</param>
	/// <returns><paramref name="m_out"/></returns>
	floatNx2_t matIdentity2f(float2x2_t m_out);

	/// <summary>
	/// Initialize matrix given individual columns.
	/// </summary>
	/// <param name="m_out">Result matrix.</param>
	/// <param name="x">First column.</param>
	/// <param name="y">Second column.</param>
	/// <returns><paramref name="m_out"/></returns>
	floatNx2_t matInit2f(float2x2_t m_out, float2_t const x, float2_t const y);

	/// <summary>
	/// Initialize diagonal of matrix given single value.
	/// </summary>
	/// <param name="m_out">Result matrix.</param>
	/// <param name="x">Value to assign to diagonal.</param>
	/// <returns><paramref name="m_out"/></returns>
	floatNx2_t matInitS2f(float2x2_t m_out, vecf_t const x);

	/// <summary>
	/// Initialize matrix by leading block of a larger matrix.
	/// </summary>
	/// <param name="m_out">Result matrix.</param>
	/// <param name="m">Matrix to copy.</param>
	/// <returns><paramref name="m_out"/></returns>
	floatNx2_t matCopy32f(float2x2_t m_out, float3x3_t const m);

	/// <summary>
	/// Initialize matrix by leading block of a larger matrix.
	/// </summary>
	/// <param name="m_out">Result matrix.</param>
	/// <param name="m">Matrix to copy.</param>
	/// <returns><paramref name="m_out"/></returns>
	floatNx2_t matCopy42f(float2x2_t m_out, float4x4_t const m);

	/// <summary>
	/// Initialize matrix by copying the leading columns of another.
	/// </summary>
	/// <param name="m_out">Result matrix.</param>
	/// <param name="m">Matrix to copy.</param>
	/// <returns><paramref name="m_out"/></returns>
	floatNx2_t matCopy2f(float2x2_t m_out, float2x2_t const m);

	/// <summary>
	/// Initialize matrix by copying the leading columns of another.
	/// </summary>
	/// <param name="m_out">Result matrix.</param>
	/// <param name="m">Matrix to copy.</param>
	/// <returns><paramref name="m_out"/></returns>
	floatNx2_t matCopy2d2f(float2x2_t m_out, double2x2_t const m);

	/// <summary>
	/// Calculate column-wise negative of input matrix.
	/// </summary>
	/// <param name="m_out">Result matrix.</param>
	/// <param name="m">Input matrix.</param>
	/// <returns><paramref name="m_out"/></returns>
	floatNx2_t matNegate2f(float2x2_t m_out, float2x2_t const m);

	/// <summary>
	/// Calculate the determinant of input matrix.
	/// </summary>
	/// <param name="m">Input matrix.</param>
	/// <returns>Determinant of matrix.</returns>
	vecf_t matDet2f(float2x2_t const m);

	/// <summary>
	/// Calculate the inverse determinant of input matrix.
	/// </summary>
	/// <param name="m">Input matrix.</param>
	/// <returns>Inverse determinant of matrix.</returns>
	vecf_t matDetInv2f(float2x2_t const m);

	/// <summary>
	/// Test if input matrix is considered identity (I).
	/// </summary>
	/// <param name="m">Input matrix.</param>
	/// <returns>True if matrix is considered identity.</returns>
	vecb_t matIsIdentity2f(float2x2_t const m);

	/// <summary>
	/// Test if input matrix contains orthogonal basis (for 4D, upper 3x3 only).
	/// </summary>
	/// <param name="isMirrored_out_opt">Resulting mirrored test flag (optional).</param>
	/// <param name="m">Input matrix.</param>
	/// <returns>True if input matrix is considered orthogonal basis.</returns>
	vecb_t matIsOrtho2f(vecb_t* isMirrored_out_opt, float2x2_t const m);

	/// <summary>
	/// Calculate the inverse of the input matrix, such that the product of the inverse and the input is identity.
	/// </summary>
	/// <param name="m_out">Result matrix.</param>
	/// <param name="m">Input matrix.</param>
	/// <returns>Determinant of matrix.</returns>
	vecf_t matInverse2f(float2x2_t m_out, float2x2_t const m);

	/// <summary>
	/// Calculate the transpose of a matrix (flip elements along diagonal).
	/// </summary>
	/// <param name="m_out">Result matrix.</param>
	/// <param name="m">Input matrix.</param>
	/// <returns><paramref name="m_out"/></returns>
	floatNx2_t matTranspose2f(float2x2_t m_out, float2x2_t const m);

	/// <summary>
	/// Calculate the inverse transpose of the input matrix (transpose of inverse).
	/// </summary>
	/// <param name="m_out">Result matrix.</param>
	/// <param name="m">Input matrix.</param>
	/// <returns>Determinant of matrix.</returns>
	vecf_t matInverseTranspose2f(float2x2_t m_out, float2x2_t const m);

	/// <summary>
	/// Calculate the product of a vector multiplied by a matrix.
	/// </summary>
	/// <param name="v_out">Result vector.</param>
	/// <param name="m_lh">Left-hand input matrix.</param>
	/// <param name="v_rh">Right-hand input vector.</param>
	/// <returns><paramref name="v_out"/></returns>
	floatN_t matMulVec2f(float2_t v_out, float2x2_t const m_lh, float2_t const v_rh);

	/// <summary>
	/// Calculate the product of a vector multiplied by a transposed matrix (this is more optimized if ever needed).
	/// </summary>
	/// <param name="v_out">Result vector.</param>
	/// <param name="m_lh">Left-hand input matrix (original, transpose used for product).</param>
	/// <param name="v_rh">Right-hand input vector.</param>
	/// <returns><paramref name="v_out"/></returns>
	floatN_t matTransposeMulVec2f(float2_t v_out, float2x2_t const m_lh, float2_t const v_rh);

	/// <summary>
	/// Calculate the matrix product with the left-hand transpose (this is more optimized if ever needed).
	/// </summary>
	/// <param name="m_out">Result matrix.</param>
	/// <param name="m_lh">Left-hand input matrix (original, transpose used for product).</param>
	/// <param name="m_rh">Right-hand input matrix.</param>
	/// <returns><paramref name="m_out"/></returns>
	floatNx2_t matTransposeMul2f(float2x2_t m_out, float2x2_t const m_lh, float2x2_t const m_rh);

	/// <summary>
	/// Calculate column-wise sum of input matrices (left-hand plus right-hand).
	/// </summary>
	/// <param name="m_out">Result matrix.</param>
	/// <param name="m_lh">Left-hand input matrix.</param>
	/// <param name="m_rh">Right-hand input matrix.</param>
	/// <returns><paramref name="m_out"/></returns>
	floatNx2_t matAdd2f(float2x2_t m_out, float2x2_t const m_lh, float2x2_t const m_rh);

	/// <summary>
	/// Calculate column-wise difference of input matrices (left-hand minus right-hand).
	/// </summary>
	/// <param name="m_out">Result matrix.</param>
	/// <param name="m_lh">Left-hand input matrix.</param>
	/// <param name="m_rh">Right-hand input matrix.</param>
	/// <returns><paramref name="m_out"/></returns>
	floatNx2_t matSub2f(float2x2_t m_out, float2x2_t const m_lh, float2x2_t const m_rh);

	/// <summary>
	/// Calculate matrix product of input matrices (left-hand times right-hand).
	/// </summary>
	/// <param name="m_out">Result matrix.</param>
	/// <param name="m_lh">Left-hand input matrix.</param>
	/// <param name="m_rh">Right-hand input matrix.</param>
	/// <returns><paramref name="m_out"/></returns>
	floatNx2_t matMul2f(float2x2_t m_out, float2x2_t const m_lh, float2x2_t const m_rh);

	/// <summary>
	/// Calculate matrix quotient of input matrices (left-hand over right-hand).
	/// </summary>
	/// <param name="m_out">Result matrix.</param>
	/// <param name="m_lh">Left-hand input matrix.</param>
	/// <param name="m_rh">Right-hand input matrix.</param>
	/// <returns><paramref name="m_out"/></returns>
	floatNx2_t matDiv2f(float2x2_t m_out, float2x2_t const m_lh, float2x2_t const m_rh);

	/// <summary>
	/// Calculate column-wise sum of input matrix and scalar (left-hand times right-hand).
	/// </summary>
	/// <param name="m_out">Result matrix.</param>
	/// <param name="m_lh">Left-hand input matrix.</param>
	/// <param name="s_rh">Right-hand input scalar.</param>
	/// <returns><paramref name="m_out"/></returns>
	floatNx2_t matAddS2f(float2x2_t m_out, float2x2_t const m_lh, vecf_t const s_rh);

	/// <summary>
	/// Calculate column-wise difference of input matrix and scalar (left-hand times right-hand).
	/// </summary>
	/// <param name="m_out">Result matrix.</param>
	/// <param name="m_lh">Left-hand input matrix.</param>
	/// <param name="s_rh">Right-hand input scalar.</param>
	/// <returns><paramref name="m_out"/></returns>
	floatNx2_t matSubS2f(float2x2_t m_out, float2x2_t const m_lh, vecf_t const s_rh);

	/// <summary>
	/// Calculate product of input matrix and scalar (left-hand times right-hand).
	/// </summary>
	/// <param name="m_out">Result matrix.</param>
	/// <param name="m_lh">Left-hand input matrix.</param>
	/// <param name="s_rh">Right-hand input scalar.</param>
	/// <returns><paramref name="m_out"/></returns>
	floatNx2_t matMulS2f(float2x2_t m_out, float2x2_t const m_lh, vecf_t const s_rh);

	/// <summary>
	/// Calculate quotient of input matrix and scalar (left-hand over right-hand).
	/// </summary>
	/// <param name="m_out">Result matrix.</param>
	/// <param name="m_lh">Left-hand input matrix.</param>
	/// <param name="s_rh">Right-hand input scalar.</param>
	/// <returns><paramref name="m_out"/></returns>
	floatNx2_t matDivS2f(float2x2_t m_out, float2x2_t const m_lh, vecf_t const s_rh);

	/// <summary>
	/// Calculate optimized product of left-hand matrix inverse and right-hand vector.
	/// </summary>
	/// <param name="v_out">Result vector.</param>
	/// <param name="m_lh">Left-hand input matrix.</param>
	/// <param name="v_rh">Right-hand input vector.</param>
	/// <returns>Determinant of left-hand input matrix.</returns>
	vecf_t matInvMulVec2f(float2_t v_out, float2x2_t const m_lh, float2_t const v_rh);

	/// <summary>
	/// Calculate optimized product of left-hand matrix inverse and right-hand matrix.
	/// </summary>
	/// <param name="m_out">Result matrix.</param>
	/// <param name="m_lh">Left-hand input matrix.</param>
	/// <param name="m_rh">Right-hand input matrix.</param>
	/// <returns>Determinant of left-hand input matrix.</returns>
	vecf_t matInvMul2f(float2x2_t m_out, float2x2_t const m_lh, float2x2_t const m_rh);

	/// <summary>
	/// Initialize matrix by copying the leading columns of another.
	/// </summary>
	/// <param name="m_out">Result matrix.</param>
	/// <param name="m">Matrix to copy.</param>
	/// <returns><paramref name="m_out"/></returns>
#define matCopy3f3f matCopy3f


#ifdef __cplusplus
}
#endif // #ifdef __cplusplus


#endif // #if (!(defined _CDRAW_MAT2F_H_) && (defined _CDRAW_MATRIX_H_))