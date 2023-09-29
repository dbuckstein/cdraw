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
* cdrawVec2i.h
* Declaration of integer 2D vector functions.
*/

#if (!(defined _CDRAW_VEC2I_H_) && (defined _CDRAW_VECTOR_H_))
#define _CDRAW_VEC2I_H_


#ifdef __cplusplus
extern "C" {
#endif // #ifdef __cplusplus

	/// <summary>
	/// Initialize vector to zero.
	/// </summary>
	/// <param name="v_out">Result vector.</param>
	/// <returns><paramref name="v_out"/></returns>
	intN_t vecZero2i(int2_t v_out);

	/// <summary>
	/// Initialize vector given individual components.
	/// </summary>
	/// <param name="v_out">Result vector.</param>
	/// <param name="x">First component.</param>
	/// <param name="y">Second component.</param>
	/// <returns><paramref name="v_out"/></returns>
	intN_t vecInit2i(int2_t v_out, veci_t const x, veci_t const y);

	/// <summary>
	/// Initialize vector by copying the leading components of another.
	/// </summary>
	/// <param name="v_out">Result vector.</param>
	/// <param name="v">Vector to copy.</param>
	/// <returns><paramref name="v_out"/></returns>
	intN_t vecCopy2i(int2_t v_out, int2_t const v);

	/// <summary>
	/// Initialize vector by copying the leading components of another.
	/// </summary>
	/// <param name="v_out">Result vector.</param>
	/// <param name="v">Vector to copy.</param>
	/// <returns><paramref name="v_out"/></returns>
	intN_t vecCopy2b2i(int2_t v_out, bool2_t const v);

	/// <summary>
	/// Initialize vector by copying the leading components of another.
	/// </summary>
	/// <param name="v_out">Result vector.</param>
	/// <param name="v">Vector to copy.</param>
	/// <returns><paramref name="v_out"/></returns>
	intN_t vecCopy2u2i(int2_t v_out, uint2_t const v);

	/// <summary>
	/// Initialize vector by copying the leading components of another.
	/// </summary>
	/// <param name="v_out">Result vector.</param>
	/// <param name="v">Vector to copy.</param>
	/// <returns><paramref name="v_out"/></returns>
	intN_t vecCopy2f2i(int2_t v_out, float2_t const v);

	/// <summary>
	/// Initialize vector by copying the leading components of another.
	/// </summary>
	/// <param name="v_out">Result vector.</param>
	/// <param name="v">Vector to copy.</param>
	/// <returns><paramref name="v_out"/></returns>
	intN_t vecCopy2d2i(int2_t v_out, double2_t const v);

	/// <summary>
	/// Evaluate component-wise bitwise NOT operator.
	/// </summary>
	/// <param name="v_out">Result vector.</param>
	/// <param name="v">Input vector.</param>
	/// <returns><paramref name="v_out"/></returns>
	intN_t vecNot2i(int2_t v_out, int2_t const v);

	/// <summary>
	/// Evaluate component-wise bitwise AND operator.
	/// </summary>
	/// <param name="v_out">Result vector.</param>
	/// <param name="v_lh">Left-hand input vector.</param>
	/// <param name="v_rh">Right-hand input vector.</param>
	/// <returns><paramref name="v_out"/></returns>
	intN_t vecAnd2i(int2_t v_out, int2_t const v_lh, int2_t const v_rh);

	/// <summary>
	/// Evaluate component-wise bitwise NAND (NOT AND) operator.
	/// </summary>
	/// <param name="v_out">Result vector.</param>
	/// <param name="v_lh">Left-hand input vector.</param>
	/// <param name="v_rh">Right-hand input vector.</param>
	/// <returns><paramref name="v_out"/></returns>
	intN_t vecNand2i(int2_t v_out, int2_t const v_lh, int2_t const v_rh);

	/// <summary>
	/// Evaluate component-wise bitwise OR operator.
	/// </summary>
	/// <param name="v_out">Result vector.</param>
	/// <param name="v_lh">Left-hand input vector.</param>
	/// <param name="v_rh">Right-hand input vector.</param>
	/// <returns><paramref name="v_out"/></returns>
	intN_t vecOr2i(int2_t v_out, int2_t const v_lh, int2_t const v_rh);

	/// <summary>
	/// Evaluate component-wise bitwise NOR (NOT OR) operator.
	/// </summary>
	/// <param name="v_out">Result vector.</param>
	/// <param name="v_lh">Left-hand input vector.</param>
	/// <param name="v_rh">Right-hand input vector.</param>
	/// <returns><paramref name="v_out"/></returns>
	intN_t vecNor2i(int2_t v_out, int2_t const v_lh, int2_t const v_rh);

	/// <summary>
	/// Evaluate component-wise bitwise XOR (exclusive or) operator.
	/// </summary>
	/// <param name="v_out">Result vector.</param>
	/// <param name="v_lh">Left-hand input vector.</param>
	/// <param name="v_rh">Right-hand input vector.</param>
	/// <returns><paramref name="v_out"/></returns>
	intN_t vecXor2i(int2_t v_out, int2_t const v_lh, int2_t const v_rh);

	/// <summary>
	/// Evaluate component-wise bitwise XNOR (exclusive nor, NOT XOR) operator.
	/// </summary>
	/// <param name="v_out">Result vector.</param>
	/// <param name="v_lh">Left-hand input vector.</param>
	/// <param name="v_rh">Right-hand input vector.</param>
	/// <returns><paramref name="v_out"/></returns>
	intN_t vecXnor2i(int2_t v_out, int2_t const v_lh, int2_t const v_rh);

	/// <summary>
	/// Test component-wise equality (left-hand input equals right-hand input).
	/// </summary>
	/// <param name="v_out">Result vector.</param>
	/// <param name="v_lh">Left-hand input vector.</param>
	/// <param name="v_rh">Right-hand input vector.</param>
	/// <returns><paramref name="v_out"/></returns>
	boolN_t vecEq2i(bool2_t v_out, int2_t const v_lh, int2_t const v_rh);

	/// <summary>
	/// Test component-wise inequality (left-hand input does not equal right-hand input).
	/// </summary>
	/// <param name="v_out">Result vector.</param>
	/// <param name="v_lh">Left-hand input vector.</param>
	/// <param name="v_rh">Right-hand input vector.</param>
	/// <returns><paramref name="v_out"/></returns>
	boolN_t vecNeq2i(bool2_t v_out, int2_t const v_lh, int2_t const v_rh);


#ifdef __cplusplus
}
#endif // #ifdef __cplusplus


#endif // #if (!(defined _CDRAW_VEC2I_H_) && (defined _CDRAW_VECTOR_H_))