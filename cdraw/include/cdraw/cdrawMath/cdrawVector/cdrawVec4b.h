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
* cdrawVec4b.h
* Declaration of boolean 4D vector functions.
*/

#if (!(defined _CDRAW_VEC4B_H_) && (defined _CDRAW_VECTOR_H_))
#define _CDRAW_VEC4B_H_


#ifdef __cplusplus
extern "C" {
#endif // #ifdef __cplusplus

	/// <summary>
	/// Initialize vector to zero.
	/// </summary>
	/// <param name="v_out">Result vector.</param>
	/// <returns><paramref name="v_out"/></returns>
	boolN_t vecZero4b(bool4_t v_out);

	/// <summary>
	/// Initialize vector given individual components.
	/// </summary>
	/// <param name="v_out">Result vector.</param>
	/// <param name="x">First component.</param>
	/// <param name="y">Second component.</param>
	/// <param name="z">Third component.</param>
	/// <param name="w">Fourth component.</param>
	/// <returns><paramref name="v_out"/></returns>
	boolN_t vecInit4b(bool4_t v_out, vecb_t const x, vecb_t const y, vecb_t const z, vecb_t const w);

	/// <summary>
	/// Initialize vector by copying another and additional components.
	/// </summary>
	/// <param name="v_out">Result vector.</param>
	/// <param name="xy">Vector to copy.</param>
	/// <param name="z">Third component.</param>
	/// <param name="w">Fourth component.</param>
	/// <returns><paramref name="v_out"/></returns>
	boolN_t vecCopy2zw4b(bool4_t v_out, bool2_t const xy, vecb_t const z, vecb_t const w);

	/// <summary>
	/// Initialize vector by copying another and additional components.
	/// </summary>
	/// <param name="v_out">Result vector.</param>
	/// <param name="xyz">Vector to copy.</param>
	/// <param name="w">Fourth component.</param>
	/// <returns><paramref name="v_out"/></returns>
	boolN_t vecCopy3w4b(bool4_t v_out, bool3_t const xyz, vecb_t const w);

	/// <summary>
	/// Initialize vector by copying the leading components of another.
	/// </summary>
	/// <param name="v_out">Result vector.</param>
	/// <param name="v">Vector to copy.</param>
	/// <returns><paramref name="v_out"/></returns>
	boolN_t vecCopy4b(bool4_t v_out, bool4_t const v);

	/// <summary>
	/// Initialize vector by copying the leading components of another.
	/// </summary>
	/// <param name="v_out">Result vector.</param>
	/// <param name="v">Vector to copy.</param>
	/// <returns><paramref name="v_out"/></returns>
	boolN_t vecCopy4i4b(bool4_t v_out, int4_t const v);

	/// <summary>
	/// Initialize vector by copying the leading components of another.
	/// </summary>
	/// <param name="v_out">Result vector.</param>
	/// <param name="v">Vector to copy.</param>
	/// <returns><paramref name="v_out"/></returns>
	boolN_t vecCopy4u4b(bool4_t v_out, uint4_t const v);

	/// <summary>
	/// Initialize vector by copying the leading components of another.
	/// </summary>
	/// <param name="v_out">Result vector.</param>
	/// <param name="v">Vector to copy.</param>
	/// <returns><paramref name="v_out"/></returns>
	boolN_t vecCopy4f4b(bool4_t v_out, float4_t const v);

	/// <summary>
	/// Initialize vector by copying the leading components of another.
	/// </summary>
	/// <param name="v_out">Result vector.</param>
	/// <param name="v">Vector to copy.</param>
	/// <returns><paramref name="v_out"/></returns>
	boolN_t vecCopy4d4b(bool4_t v_out, double4_t const v);

	/// <summary>
	/// Evaluate component-wise NOT operator.
	/// </summary>
	/// <param name="v_out">Result vector</param>
	/// <param name="v">Input vector.</param>
	/// <returns><paramref name="v_out"/></returns>
	boolN_t vecNot4b(bool4_t v_out, bool4_t const v);

	/// <summary>
	/// Evaluate component-wise AND operator.
	/// </summary>
	/// <param name="v_out">Result vector</param>
	/// <param name="v_lh">Left-hand input vector.</param>
	/// <param name="v_rh">Right-hand input vector.</param>
	/// <returns><paramref name="v_out"/></returns>
	boolN_t vecAnd4b(bool4_t v_out, bool4_t const v_lh, bool4_t const v_rh);

	/// <summary>
	/// Evaluate component-wise NAND (NOT AND) operator.
	/// </summary>
	/// <param name="v_out">Result vector</param>
	/// <param name="v_lh">Left-hand input vector.</param>
	/// <param name="v_rh">Right-hand input vector.</param>
	/// <returns><paramref name="v_out"/></returns>
	boolN_t vecNand4b(bool4_t v_out, bool4_t const v_lh, bool4_t const v_rh);

	/// <summary>
	/// Evaluate component-wise OR operator.
	/// </summary>
	/// <param name="v_out">Result vector</param>
	/// <param name="v_lh">Left-hand input vector.</param>
	/// <param name="v_rh">Right-hand input vector.</param>
	/// <returns><paramref name="v_out"/></returns>
	boolN_t vecOr4b(bool4_t v_out, bool4_t const v_lh, bool4_t const v_rh);

	/// <summary>
	/// Evaluate component-wise NOR (NOT OR) operator.
	/// </summary>
	/// <param name="v_out">Result vector</param>
	/// <param name="v_lh">Left-hand input vector.</param>
	/// <param name="v_rh">Right-hand input vector.</param>
	/// <returns><paramref name="v_out"/></returns>
	boolN_t vecNor4b(bool4_t v_out, bool4_t const v_lh, bool4_t const v_rh);

	/// <summary>
	/// Evaluate component-wise XOR (exclusive or) operator.
	/// </summary>
	/// <param name="v_out">Result vector</param>
	/// <param name="v_lh">Left-hand input vector.</param>
	/// <param name="v_rh">Right-hand input vector.</param>
	/// <returns><paramref name="v_out"/></returns>
	boolN_t vecXor4b(bool4_t v_out, bool4_t const v_lh, bool4_t const v_rh);

	/// <summary>
	/// Evaluate component-wise NXOR (NOT XOR) operator.
	/// </summary>
	/// <param name="v_out">Result vector</param>
	/// <param name="v_lh">Left-hand input vector.</param>
	/// <param name="v_rh">Right-hand input vector.</param>
	/// <returns><paramref name="v_out"/></returns>
	boolN_t vecNxor4b(bool4_t v_out, bool4_t const v_lh, bool4_t const v_rh);

	/// <summary>
	/// Test component-wise equality (left-hand input equals right-hand input).
	/// </summary>
	/// <param name="v_out">Result vector</param>
	/// <param name="v_lh">Left-hand input vector.</param>
	/// <param name="v_rh">Right-hand input vector.</param>
	/// <returns><paramref name="v_out"/></returns>
	boolN_t vecEq4b(bool4_t v_out, bool4_t const v_lh, bool4_t const v_rh);

	/// <summary>
	/// Test component-wise inequality (left-hand input does not equal right-hand input).
	/// </summary>
	/// <param name="v_out">Result vector</param>
	/// <param name="v_lh">Left-hand input vector.</param>
	/// <param name="v_rh">Right-hand input vector.</param>
	/// <returns><paramref name="v_out"/></returns>
	boolN_t vecNeq4b(bool4_t v_out, bool4_t const v_lh, bool4_t const v_rh);


#ifdef __cplusplus
}
#endif // #ifdef __cplusplus


#endif // #if (!(defined _CDRAW_VEC4B_H_) && (defined _CDRAW_VECTOR_H_))