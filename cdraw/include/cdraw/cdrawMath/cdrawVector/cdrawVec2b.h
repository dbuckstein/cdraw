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
* cdrawVec2b.h
* Declaration of boolean 2D vector functions.
*/

#if (!(defined _CDRAW_VEC2B_H_) && (defined _CDRAW_VECTOR_H_))
#define _CDRAW_VEC2B_H_


#ifdef __cplusplus
extern "C" {
#endif // #ifdef __cplusplus

	/// <summary>
	/// Initialize vector to zero.
	/// </summary>
	/// <param name="v_out">Result vector.</param>
	/// <returns><paramref name="v_out"/></returns>
	boolN_t vecZero2b(bool2_t v_out);

	/// <summary>
	/// Initialize vector given individual components.
	/// </summary>
	/// <param name="v_out">Result vector.</param>
	/// <param name="x">First component.</param>
	/// <param name="y">Second component.</param>
	/// <returns><paramref name="v_out"/></returns>
	boolN_t vecInit2b(bool2_t v_out, vecb_t const x, vecb_t const y);

	/// <summary>
	/// Initialize vector by copying the leading components of another.
	/// </summary>
	/// <param name="v_out">Result vector.</param>
	/// <param name="v">Vector to copy.</param>
	/// <returns><paramref name="v_out"/></returns>
	boolN_t vecCopy2b(bool2_t v_out, bool2_t const v);

	/// <summary>
	/// Initialize vector by copying the leading components of another.
	/// </summary>
	/// <param name="v_out">Result vector.</param>
	/// <param name="v">Vector to copy.</param>
	/// <returns><paramref name="v_out"/></returns>
	boolN_t vecCopy2i2b(bool2_t v_out, int2_t const v);

	/// <summary>
	/// Initialize vector by copying the leading components of another.
	/// </summary>
	/// <param name="v_out">Result vector.</param>
	/// <param name="v">Vector to copy.</param>
	/// <returns><paramref name="v_out"/></returns>
	boolN_t vecCopy2u2b(bool2_t v_out, uint2_t const v);

	/// <summary>
	/// Initialize vector by copying the leading components of another.
	/// </summary>
	/// <param name="v_out">Result vector.</param>
	/// <param name="v">Vector to copy.</param>
	/// <returns><paramref name="v_out"/></returns>
	boolN_t vecCopy2f2b(bool2_t v_out, float2_t const v);

	/// <summary>
	/// Initialize vector by copying the leading components of another.
	/// </summary>
	/// <param name="v_out">Result vector.</param>
	/// <param name="v">Vector to copy.</param>
	/// <returns><paramref name="v_out"/></returns>
	boolN_t vecCopy2d2b(bool2_t v_out, double2_t const v);

	/// <summary>
	/// Evaluate component-wise NOT operator.
	/// </summary>
	/// <param name="v_out">Result vector</param>
	/// <param name="v">Input vector.</param>
	/// <returns><paramref name="v_out"/></returns>
	boolN_t vecNot2b(bool2_t v_out, bool2_t const v);

	/// <summary>
	/// Evaluate component-wise AND operator.
	/// </summary>
	/// <param name="v_out">Result vector</param>
	/// <param name="v_lh">Left-hand input vector.</param>
	/// <param name="v_rh">Right-hand input vector.</param>
	/// <returns><paramref name="v_out"/></returns>
	boolN_t vecAnd2b(bool2_t v_out, bool2_t const v_lh, bool2_t const v_rh);

	/// <summary>
	/// Evaluate component-wise NAND (NOT AND) operator.
	/// </summary>
	/// <param name="v_out">Result vector</param>
	/// <param name="v_lh">Left-hand input vector.</param>
	/// <param name="v_rh">Right-hand input vector.</param>
	/// <returns><paramref name="v_out"/></returns>
	boolN_t vecNand2b(bool2_t v_out, bool2_t const v_lh, bool2_t const v_rh);

	/// <summary>
	/// Evaluate component-wise OR operator.
	/// </summary>
	/// <param name="v_out">Result vector</param>
	/// <param name="v_lh">Left-hand input vector.</param>
	/// <param name="v_rh">Right-hand input vector.</param>
	/// <returns><paramref name="v_out"/></returns>
	boolN_t vecOr2b(bool2_t v_out, bool2_t const v_lh, bool2_t const v_rh);

	/// <summary>
	/// Evaluate component-wise NOR (NOT OR) operator.
	/// </summary>
	/// <param name="v_out">Result vector</param>
	/// <param name="v_lh">Left-hand input vector.</param>
	/// <param name="v_rh">Right-hand input vector.</param>
	/// <returns><paramref name="v_out"/></returns>
	boolN_t vecNor2b(bool2_t v_out, bool2_t const v_lh, bool2_t const v_rh);

	/// <summary>
	/// Evaluate component-wise XOR (exclusive or) operator.
	/// </summary>
	/// <param name="v_out">Result vector</param>
	/// <param name="v_lh">Left-hand input vector.</param>
	/// <param name="v_rh">Right-hand input vector.</param>
	/// <returns><paramref name="v_out"/></returns>
	boolN_t vecXor2b(bool2_t v_out, bool2_t const v_lh, bool2_t const v_rh);

	/// <summary>
	/// Evaluate component-wise NXOR (NOT XOR) operator.
	/// </summary>
	/// <param name="v_out">Result vector</param>
	/// <param name="v_lh">Left-hand input vector.</param>
	/// <param name="v_rh">Right-hand input vector.</param>
	/// <returns><paramref name="v_out"/></returns>
	boolN_t vecNxor2b(bool2_t v_out, bool2_t const v_lh, bool2_t const v_rh);

	/// <summary>
	/// Test component-wise equality (left-hand input equals right-hand input).
	/// </summary>
	/// <param name="v_out">Result vector</param>
	/// <param name="v_lh">Left-hand input vector.</param>
	/// <param name="v_rh">Right-hand input vector.</param>
	/// <returns><paramref name="v_out"/></returns>
	boolN_t vecEq2b(bool2_t v_out, bool2_t const v_lh, bool2_t const v_rh);

	/// <summary>
	/// Test component-wise inequality (left-hand input does not equal right-hand input).
	/// </summary>
	/// <param name="v_out">Result vector</param>
	/// <param name="v_lh">Left-hand input vector.</param>
	/// <param name="v_rh">Right-hand input vector.</param>
	/// <returns><paramref name="v_out"/></returns>
	boolN_t vecNeq2b(bool2_t v_out, bool2_t const v_lh, bool2_t const v_rh);


#ifdef __cplusplus
}
#endif // #ifdef __cplusplus


#endif // #if (!(defined _CDRAW_VEC2B_H_) && (defined _CDRAW_VECTOR_H_))