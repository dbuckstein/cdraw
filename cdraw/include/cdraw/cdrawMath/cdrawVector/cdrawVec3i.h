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
* cdrawVec3i.h
* Declaration of integer 3D vector functions.
*/

#if (!(defined _CDRAW_VEC3I_H_) && (defined _CDRAW_VECTOR_H_))
#define _CDRAW_VEC3I_H_


#ifdef __cplusplus
extern "C" {
#endif // #ifdef __cplusplus

	/// <summary>
	/// Initialize vector to zero.
	/// </summary>
	/// <param name="v_out">Result vector.</param>
	/// <returns><paramref name="v_out"/></returns>
	intN_t vecZero3i(int3_t v_out);

	/// <summary>
	/// Initialize vector given individual components.
	/// </summary>
	/// <param name="v_out">Result vector.</param>
	/// <param name="x">First component.</param>
	/// <param name="y">Second component.</param>
	/// <param name="z">Third component.</param>
	/// <returns><paramref name="v_out"/></returns>
	intN_t vecInit3i(int3_t v_out, veci_t const x, veci_t const y, veci_t const z);

	/// <summary>
	/// Initialize all components of vector given single value.
	/// </summary>
	/// <param name="v_out">Result vector.</param>
	/// <param name="x">Value to assign to all components.</param>
	/// <returns><paramref name="v_out"/></returns>
	intN_t vecInitS3i(int3_t v_out, veci_t const x);

	/// <summary>
	/// Initialize vector by copying another and additional components.
	/// </summary>
	/// <param name="v_out">Result vector.</param>
	/// <param name="xy">Vector to copy.</param>
	/// <param name="z">Third component.</param>
	/// <returns><paramref name="v_out"/></returns>
	intN_t vecCopy2z3i(int3_t v_out, int2_t const xy, veci_t const z);

	/// <summary>
	/// Initialize vector by copying the leading components of another.
	/// </summary>
	/// <param name="v_out">Result vector.</param>
	/// <param name="v">Vector to copy.</param>
	/// <returns><paramref name="v_out"/></returns>
	intN_t vecCopy3i(int3_t v_out, int3_t const v);

	/// <summary>
	/// Initialize vector by copying the leading components of another.
	/// </summary>
	/// <param name="v_out">Result vector.</param>
	/// <param name="v">Vector to copy.</param>
	/// <returns><paramref name="v_out"/></returns>
	intN_t vecCopy3b3i(int3_t v_out, bool3_t const v);

	/// <summary>
	/// Initialize vector by copying the leading components of another.
	/// </summary>
	/// <param name="v_out">Result vector.</param>
	/// <param name="v">Vector to copy.</param>
	/// <returns><paramref name="v_out"/></returns>
	intN_t vecCopy3u3i(int3_t v_out, uint3_t const v);

	/// <summary>
	/// Initialize vector by copying the leading components of another.
	/// </summary>
	/// <param name="v_out">Result vector.</param>
	/// <param name="v">Vector to copy.</param>
	/// <returns><paramref name="v_out"/></returns>
	intN_t vecCopy3f3i(int3_t v_out, float3_t const v);

	/// <summary>
	/// Initialize vector by copying the leading components of another.
	/// </summary>
	/// <param name="v_out">Result vector.</param>
	/// <param name="v">Vector to copy.</param>
	/// <returns><paramref name="v_out"/></returns>
	intN_t vecCopy3d3i(int3_t v_out, double3_t const v);

	/// <summary>
	/// Evaluate component-wise bitwise NOT operator.
	/// </summary>
	/// <param name="v_out">Result vector.</param>
	/// <param name="v">Input vector.</param>
	/// <returns><paramref name="v_out"/></returns>
	intN_t vecNot3i(int3_t v_out, int3_t const v);

	/// <summary>
	/// Evaluate component-wise bitwise AND operator.
	/// </summary>
	/// <param name="v_out">Result vector.</param>
	/// <param name="v_lh">Left-hand input vector.</param>
	/// <param name="v_rh">Right-hand input vector.</param>
	/// <returns><paramref name="v_out"/></returns>
	intN_t vecAnd3i(int3_t v_out, int3_t const v_lh, int3_t const v_rh);

	/// <summary>
	/// Evaluate component-wise bitwise NAND (NOT AND) operator.
	/// </summary>
	/// <param name="v_out">Result vector.</param>
	/// <param name="v_lh">Left-hand input vector.</param>
	/// <param name="v_rh">Right-hand input vector.</param>
	/// <returns><paramref name="v_out"/></returns>
	intN_t vecNand3i(int3_t v_out, int3_t const v_lh, int3_t const v_rh);

	/// <summary>
	/// Evaluate component-wise bitwise OR operator.
	/// </summary>
	/// <param name="v_out">Result vector.</param>
	/// <param name="v_lh">Left-hand input vector.</param>
	/// <param name="v_rh">Right-hand input vector.</param>
	/// <returns><paramref name="v_out"/></returns>
	intN_t vecOr3i(int3_t v_out, int3_t const v_lh, int3_t const v_rh);

	/// <summary>
	/// Evaluate component-wise bitwise NOR (NOT OR) operator.
	/// </summary>
	/// <param name="v_out">Result vector.</param>
	/// <param name="v_lh">Left-hand input vector.</param>
	/// <param name="v_rh">Right-hand input vector.</param>
	/// <returns><paramref name="v_out"/></returns>
	intN_t vecNor3i(int3_t v_out, int3_t const v_lh, int3_t const v_rh);

	/// <summary>
	/// Evaluate component-wise bitwise XOR (exclusive or) operator.
	/// </summary>
	/// <param name="v_out">Result vector.</param>
	/// <param name="v_lh">Left-hand input vector.</param>
	/// <param name="v_rh">Right-hand input vector.</param>
	/// <returns><paramref name="v_out"/></returns>
	intN_t vecXor3i(int3_t v_out, int3_t const v_lh, int3_t const v_rh);

	/// <summary>
	/// Evaluate component-wise bitwise XNOR (exclusive nor, NOT XOR) operator.
	/// </summary>
	/// <param name="v_out">Result vector.</param>
	/// <param name="v_lh">Left-hand input vector.</param>
	/// <param name="v_rh">Right-hand input vector.</param>
	/// <returns><paramref name="v_out"/></returns>
	intN_t vecXnor3i(int3_t v_out, int3_t const v_lh, int3_t const v_rh);

	/// <summary>
	/// Test component-wise equality (left-hand input equals right-hand input).
	/// </summary>
	/// <param name="v_out">Result vector.</param>
	/// <param name="v_lh">Left-hand input vector.</param>
	/// <param name="v_rh">Right-hand input vector.</param>
	/// <returns><paramref name="v_out"/></returns>
	boolN_t vecEq3i(bool3_t v_out, int3_t const v_lh, int3_t const v_rh);

	/// <summary>
	/// Test component-wise inequality (left-hand input does not equal right-hand input).
	/// </summary>
	/// <param name="v_out">Result vector.</param>
	/// <param name="v_lh">Left-hand input vector.</param>
	/// <param name="v_rh">Right-hand input vector.</param>
	/// <returns><paramref name="v_out"/></returns>
	boolN_t vecNeq3i(bool3_t v_out, int3_t const v_lh, int3_t const v_rh);


#ifdef __cplusplus
}
#endif // #ifdef __cplusplus


#endif // #if (!(defined _CDRAW_VEC3I_H_) && (defined _CDRAW_VECTOR_H_))