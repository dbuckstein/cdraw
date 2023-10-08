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
* cdrawMat3.h
* Declaration of preferred floating point precision 3D matrix functions.
*/

#if (!(defined _CDRAW_MAT3_H_) && (defined _CDRAW_MATRIX_H_))
#define _CDRAW_MAT3_H_


#ifdef __cplusplus
extern "C" {
#endif // #ifdef __cplusplus

	/// <summary>
	/// Initialize matrix to identity.
	/// </summary>
	/// <param name="m_out">Result matrix.</param>
	/// <returns><paramref name="m_out"/></returns>
#define matIdentity3 cdraw_matrix_base(matIdentity3)

	/// <summary>
	/// Initialize matrix given individual columns.
	/// </summary>
	/// <param name="m_out">Result matrix.</param>
	/// <param name="x">First column.</param>
	/// <param name="y">Second column.</param>
	/// <param name="z">Third column.</param>
	/// <returns><paramref name="m_out"/></returns>
#define matInit3 cdraw_matrix_base(matInit3)

	/// <summary>
	/// Initialize diagonal of matrix given single value.
	/// </summary>
	/// <param name="m_out">Result matrix.</param>
	/// <param name="x">Value to assign to diagonal.</param>
	/// <returns><paramref name="m_out"/></returns>
#define matInitS3 cdraw_matrix_base(matInitS3)

	/// <summary>
	/// Initialize matrix by copying another and additional columns.
	/// </summary>
	/// <param name="m_out">Result matrix.</param>
	/// <param name="xy">Matrix to copy.</param>
	/// <param name="z">Third column.</param>
	/// <returns><paramref name="m_out"/></returns>
#define matCopy2z3 cdraw_matrix_base(matCopy2z3)

	/// <summary>
	/// Initialize matrix by leading block of a larger matrix.
	/// </summary>
	/// <param name="m_out">Result matrix.</param>
	/// <param name="m">Matrix to copy.</param>
	/// <returns><paramref name="m_out"/></returns>
#define matCopy43 cdraw_matrix_base(matCopy43)

	/// <summary>
	/// Initialize matrix by copying the leading columns of another.
	/// </summary>
	/// <param name="m_out">Result matrix.</param>
	/// <param name="m">Matrix to copy.</param>
	/// <returns><paramref name="m_out"/></returns>
#define matCopy3 cdraw_matrix_base(matCopy3)

	/// <summary>
	/// Initialize matrix by copying the leading columns of another.
	/// </summary>
	/// <param name="m_out">Result matrix.</param>
	/// <param name="m">Matrix to copy.</param>
	/// <returns><paramref name="m_out"/></returns>
#define matCopy33 tokencat(tokencat(matCopy3, matrix_other), tokencat(3, matrix_suffix))

	/// <summary>
	/// Calculate column-wise negative of input matrix.
	/// </summary>
	/// <param name="m_out">Result matrix.</param>
	/// <param name="m">Input matrix.</param>
	/// <returns><paramref name="m_out"/></returns>
#define matNegate3 cdraw_matrix_base(matNegate3)

	/// <summary>
	/// Calculate the determinant of input matrix.
	/// </summary>
	/// <param name="m">Input matrix.</param>
	/// <returns>Determinant of matrix.</returns>
#define matDet3 cdraw_matrix_base(matDet3)

	/// <summary>
	/// Calculate the inverse determinant of input matrix.
	/// </summary>
	/// <param name="m">Input matrix.</param>
	/// <returns>Inverse determinant of matrix.</returns>
#define matDetInv3 cdraw_matrix_base(matDetInv3)

	/// <summary>
	/// Test if input matrix is considered identity (I).
	/// </summary>
	/// <param name="m">Input matrix.</param>
	/// <returns>True if matrix is considered identity.</returns>
#define matIsIdentity3 cdraw_matrix_base(matIsIdentity3)

	/// <summary>
	/// Test if input matrix contains orthogonal basis (for 4D, upper 3x3 only).
	/// </summary>
	/// <param name="isMirrored_out_opt">Resulting mirrored test flag (optional).</param>
	/// <param name="m">Input matrix.</param>
	/// <returns>True if input matrix is considered orthogonal basis.</returns>
#define matIsOrtho3 cdraw_matrix_base(matIsOrtho3)

	/// <summary>
	/// Calculate the inverse of the input matrix, such that the product of the inverse and the input is identity.
	/// </summary>
	/// <param name="m_out">Result matrix.</param>
	/// <param name="m">Input matrix.</param>
	/// <returns>Determinant of matrix.</returns>
#define matInverse3 cdraw_matrix_base(matInverse3)

	/// <summary>
	/// Calculate the transpose of a matrix (flip elements along diagonal).
	/// </summary>
	/// <param name="m_out">Result matrix.</param>
	/// <param name="m">Input matrix.</param>
	/// <returns><paramref name="m_out"/></returns>
#define matTranspose3 cdraw_matrix_base(matTranspose3)

	/// <summary>
	/// Calculate the inverse transpose of the input matrix (transpose of inverse).
	/// </summary>
	/// <param name="m_out">Result matrix.</param>
	/// <param name="m">Input matrix.</param>
	/// <returns>Determinant of matrix.</returns>
#define matInverseTranspose3 cdraw_matrix_base(matInverseTranspose3)

	/// <summary>
	/// Calculate the product of a vector multiplied by a matrix.
	/// </summary>
	/// <param name="v_out">Result vector.</param>
	/// <param name="m_lh">Left-hand input matrix.</param>
	/// <param name="v_rh">Right-hand input vector.</param>
	/// <returns><paramref name="v_out"/></returns>
#define matMulVec3 cdraw_matrix_base(matMulVec3)

	/// <summary>
	/// Calculate the product of a vector multiplied by a transposed matrix (this is more optimized if ever needed).
	/// </summary>
	/// <param name="v_out">Result vector.</param>
	/// <param name="m_lh">Left-hand input matrix (original, transpose used for product).</param>
	/// <param name="v_rh">Right-hand input vector.</param>
	/// <returns><paramref name="v_out"/></returns>
#define matTransposeMulVec3 cdraw_matrix_base(matTransposeMulVec3)

	/// <summary>
	/// Calculate the matrix product with the left-hand transpose (this is more optimized if ever needed).
	/// </summary>
	/// <param name="m_out">Result matrix.</param>
	/// <param name="m_lh">Left-hand input matrix (original, transpose used for product).</param>
	/// <param name="m_rh">Right-hand input matrix.</param>
	/// <returns><paramref name="m_out"/></returns>
#define matTransposeMul3 cdraw_matrix_base(matTransposeMul3)

	/// <summary>
	/// Calculate column-wise sum of input matrices (left-hand plus right-hand).
	/// </summary>
	/// <param name="m_out">Result matrix.</param>
	/// <param name="m_lh">Left-hand input matrix.</param>
	/// <param name="m_rh">Right-hand input matrix.</param>
	/// <returns><paramref name="m_out"/></returns>
#define matAdd3 cdraw_matrix_base(matAdd3)

	/// <summary>
	/// Calculate column-wise difference of input matrices (left-hand minus right-hand).
	/// </summary>
	/// <param name="m_out">Result matrix.</param>
	/// <param name="m_lh">Left-hand input matrix.</param>
	/// <param name="m_rh">Right-hand input matrix.</param>
	/// <returns><paramref name="m_out"/></returns>
#define matSub3 cdraw_matrix_base(matSub3)

	/// <summary>
	/// Calculate matrix product of input matrices (left-hand times right-hand).
	/// </summary>
	/// <param name="m_out">Result matrix.</param>
	/// <param name="m_lh">Left-hand input matrix.</param>
	/// <param name="m_rh">Right-hand input matrix.</param>
	/// <returns><paramref name="m_out"/></returns>
#define matMul3 cdraw_matrix_base(matMul3)

	/// <summary>
	/// Calculate matrix quotient of input matrices (left-hand over right-hand).
	/// </summary>
	/// <param name="m_out">Result matrix.</param>
	/// <param name="m_lh">Left-hand input matrix.</param>
	/// <param name="m_rh">Right-hand input matrix.</param>
	/// <returns><paramref name="m_out"/></returns>
#define matDiv3 cdraw_matrix_base(matDiv3)

	/// <summary>
	/// Calculate column-wise sum of input matrix and scalar (left-hand times right-hand).
	/// </summary>
	/// <param name="m_out">Result matrix.</param>
	/// <param name="m_lh">Left-hand input matrix.</param>
	/// <param name="s_rh">Right-hand input scalar.</param>
	/// <returns><paramref name="m_out"/></returns>
#define matAddS3 cdraw_matrix_base(matAddS3)

	/// <summary>
	/// Calculate column-wise difference of input matrix and scalar (left-hand times right-hand).
	/// </summary>
	/// <param name="m_out">Result matrix.</param>
	/// <param name="m_lh">Left-hand input matrix.</param>
	/// <param name="s_rh">Right-hand input scalar.</param>
	/// <returns><paramref name="m_out"/></returns>
#define matSubS3 cdraw_matrix_base(matSubS3)

	/// <summary>
	/// Calculate product of input matrix and scalar (left-hand times right-hand).
	/// </summary>
	/// <param name="m_out">Result matrix.</param>
	/// <param name="m_lh">Left-hand input matrix.</param>
	/// <param name="s_rh">Right-hand input scalar.</param>
	/// <returns><paramref name="m_out"/></returns>
#define matMulS3 cdraw_matrix_base(matMulS3)

	/// <summary>
	/// Calculate quotient of input matrix and scalar (left-hand over right-hand).
	/// </summary>
	/// <param name="m_out">Result matrix.</param>
	/// <param name="m_lh">Left-hand input matrix.</param>
	/// <param name="s_rh">Right-hand input scalar.</param>
	/// <returns><paramref name="m_out"/></returns>
#define matDivS3 cdraw_matrix_base(matDivS3)

	/// <summary>
	/// Calculate optimized product of left-hand matrix inverse and right-hand vector.
	/// </summary>
	/// <param name="v_out">Result vector.</param>
	/// <param name="m_lh">Left-hand input matrix.</param>
	/// <param name="v_rh">Right-hand input vector.</param>
	/// <returns>Determinant of left-hand input matrix.</returns>
#define matInvMulVec3 cdraw_matrix_base(matInvMulVec3)

	/// <summary>
	/// Calculate optimized product of left-hand matrix inverse and right-hand matrix.
	/// </summary>
	/// <param name="m_out">Result matrix.</param>
	/// <param name="m_lh">Left-hand input matrix.</param>
	/// <param name="m_rh">Right-hand input matrix.</param>
	/// <returns>Determinant of left-hand input matrix.</returns>
#define matInvMul3 cdraw_matrix_base(matInvMul3)

	/// <summary>
	/// Initialize matrix by copying the leading columns of another.
	/// </summary>
	/// <param name="m_out">Result matrix.</param>
	/// <param name="m">Matrix to copy.</param>
	/// <returns><paramref name="m_out"/></returns>
#define matCopy33f tokencat(matCopy3, tokencat(matrix_suffix, 3f))

	/// <summary>
	/// Initialize matrix by copying the leading columns of another.
	/// </summary>
	/// <param name="m_out">Result matrix.</param>
	/// <param name="m">Matrix to copy.</param>
	/// <returns><paramref name="m_out"/></returns>
#define matCopy33d tokencat(matCopy3, tokencat(matrix_suffix, 3d))


#ifdef __cplusplus
}
#endif // #ifdef __cplusplus


#endif // #if (!(defined _CDRAW_MAT3_H_) && (defined _CDRAW_MATRIX_H_))