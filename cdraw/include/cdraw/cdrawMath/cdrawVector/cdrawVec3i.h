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

	/// <summary>
	/// Test component-wise greater-than (left-hand input is greater than right-hand input).
	/// </summary>
	/// <param name="v_out">Result vector.</param>
	/// <param name="v_lh">Left-hand input vector.</param>
	/// <param name="v_rh">Right-hand input vector.</param>
	/// <returns><paramref name="v_out"/></returns>
	boolN_t vecGreater3i(bool3_t v_out, int3_t const v_lh, int3_t const v_rh);

	/// <summary>
	/// Test component-wise greater-than-or-equal-to (left-hand input is greater than or equal to right-hand input).
	/// </summary>
	/// <param name="v_out">Result vector.</param>
	/// <param name="v_lh">Left-hand input vector.</param>
	/// <param name="v_rh">Right-hand input vector.</param>
	/// <returns><paramref name="v_out"/></returns>
	boolN_t vecGreaterEq3i(bool3_t v_out, int3_t const v_lh, int3_t const v_rh);

	/// <summary>
	/// Test component-wise less-than (left-hand input is less than right-hand input).
	/// </summary>
	/// <param name="v_out">Result vector.</param>
	/// <param name="v_lh">Left-hand input vector.</param>
	/// <param name="v_rh">Right-hand input vector.</param>
	/// <returns><paramref name="v_out"/></returns>
	boolN_t vecLess3i(bool3_t v_out, int3_t const v_lh, int3_t const v_rh);

	/// <summary>
	/// Test component-wise less-than-or-equal-to (left-hand input is less than or equal to right-hand input).
	/// </summary>
	/// <param name="v_out">Result vector.</param>
	/// <param name="v_lh">Left-hand input vector.</param>
	/// <param name="v_rh">Right-hand input vector.</param>
	/// <returns><paramref name="v_out"/></returns>
	boolN_t vecLessEq3i(bool3_t v_out, int3_t const v_lh, int3_t const v_rh);

	/// <summary>
	/// Calculate component-wise negative of input vector.
	/// </summary>
	/// <param name="v_out">Result vector.</param>
	/// <param name="v">Input vector.</param>
	/// <returns><paramref name="v_out"/></returns>
	intN_t vecNegate3i(int3_t v_out, int3_t const v);

	/// <summary>
	/// Calculate component-wise sum of input vectors (left-hand plus right-hand).
	/// </summary>
	/// <param name="v_out">Result vector.</param>
	/// <param name="v_lh">Left-hand input vector.</param>
	/// <param name="v_rh">Right-hand input vector.</param>
	/// <returns><paramref name="v_out"/></returns>
	intN_t vecAdd3i(int3_t v_out, int3_t const v_lh, int3_t const v_rh);

	/// <summary>
	/// Calculate component-wise difference of input vectors (left-hand minus right-hand).
	/// </summary>
	/// <param name="v_out">Result vector.</param>
	/// <param name="v_lh">Left-hand input vector.</param>
	/// <param name="v_rh">Right-hand input vector.</param>
	/// <returns><paramref name="v_out"/></returns>
	intN_t vecSub3i(int3_t v_out, int3_t const v_lh, int3_t const v_rh);

	/// <summary>
	/// Calculate component-wise product of input vectors (left-hand times right-hand).
	/// </summary>
	/// <param name="v_out">Result vector.</param>
	/// <param name="v_lh">Left-hand input vector.</param>
	/// <param name="v_rh">Right-hand input vector.</param>
	/// <returns><paramref name="v_out"/></returns>
	intN_t vecMul3i(int3_t v_out, int3_t const v_lh, int3_t const v_rh);

	/// <summary>
	/// Calculate component-wise quotient of input vectors (left-hand over right-hand).
	/// </summary>
	/// <param name="v_out">Result vector.</param>
	/// <param name="v_lh">Left-hand input vector.</param>
	/// <param name="v_rh">Right-hand input vector.</param>
	/// <returns><paramref name="v_out"/></returns>
	intN_t vecDiv3i(int3_t v_out, int3_t const v_lh, int3_t const v_rh);

	/// <summary>
	/// Calculate component-wise modulo of input vectors (left-hand over right-hand).
	/// </summary>
	/// <param name="v_out">Result vector.</param>
	/// <param name="v_lh">Left-hand input vector.</param>
	/// <param name="v_rh">Right-hand input vector.</param>
	/// <returns><paramref name="v_out"/></returns>
	intN_t vecMod3i(int3_t v_out, int3_t const v_lh, int3_t const v_rh);

	/// <summary>
	/// Calculate component-wise quotient and modulo of input vectors (left-hand over right-hand).
	/// </summary>
	/// <param name="v_out">Result vector (quotient).</param>
	/// <param name="v_mod_out">Result vector (modulo).</param>
	/// <param name="v_lh">Left-hand input vector.</param>
	/// <param name="v_rh">Right-hand input vector.</param>
	/// <returns><paramref name="v_out"/></returns>
	intN_t vecDivMod3i(int3_t v_out, int3_t v_mod_out, int3_t const v_lh, int3_t const v_rh);

	/// <summary>
	/// Calculate component-wise sum of input vector and scalar (left-hand times right-hand).
	/// </summary>
	/// <param name="v_out">Result vector.</param>
	/// <param name="v_lh">Left-hand input vector.</param>
	/// <param name="s_rh">Right-hand input scalar.</param>
	/// <returns><paramref name="v_out"/></returns>
	intN_t vecAddS3i(int3_t v_out, int3_t const v_lh, veci_t const s_rh);

	/// <summary>
	/// Calculate component-wise difference of input vector and scalar (left-hand times right-hand).
	/// </summary>
	/// <param name="v_out">Result vector.</param>
	/// <param name="v_lh">Left-hand input vector.</param>
	/// <param name="s_rh">Right-hand input scalar.</param>
	/// <returns><paramref name="v_out"/></returns>
	intN_t vecSubS3i(int3_t v_out, int3_t const v_lh, veci_t const s_rh);

	/// <summary>
	/// Calculate product of input vector and scalar (left-hand times right-hand).
	/// </summary>
	/// <param name="v_out">Result vector.</param>
	/// <param name="v_lh">Left-hand input vector.</param>
	/// <param name="s_rh">Right-hand input scalar.</param>
	/// <returns><paramref name="v_out"/></returns>
	intN_t vecMulS3i(int3_t v_out, int3_t const v_lh, veci_t const s_rh);

	/// <summary>
	/// Calculate quotient of input vector and scalar (left-hand over right-hand).
	/// </summary>
	/// <param name="v_out">Result vector.</param>
	/// <param name="v_lh">Left-hand input vector.</param>
	/// <param name="s_rh">Right-hand input scalar.</param>
	/// <returns><paramref name="v_out"/></returns>
	intN_t vecDivS3i(int3_t v_out, int3_t const v_lh, veci_t const s_rh);

	/// <summary>
	/// Calculate modulo of input vector and scalar (left-hand over right-hand).
	/// </summary>
	/// <param name="v_out">Result vector.</param>
	/// <param name="v_lh">Left-hand input vector.</param>
	/// <param name="s_rh">Right-hand input scalar.</param>
	/// <returns><paramref name="v_out"/></returns>
	intN_t vecModS3i(int3_t v_out, int3_t const v_lh, veci_t const s_rh);

	/// <summary>
	/// Calculate quotient and modulo of input vector and scalar (left-hand over right-hand).
	/// </summary>
	/// <param name="v_out">Result vector (quotient).</param>
	/// <param name="v_mod_out">Result vector (modulo).</param>
	/// <param name="v_lh">Left-hand input vector.</param>
	/// <param name="s_rh">Right-hand input scalar.</param>
	/// <returns><paramref name="v_out"/></returns>
	intN_t vecDivModS3i(int3_t v_out, int3_t v_mod_out, int3_t const v_lh, veci_t const s_rh);

	/// <summary>
	/// Calculate dot product of input vectors (the scalar sum of component products).
	/// </summary>
	/// <param name="v_lh">Left-hand input vector.</param>
	/// <param name="v_rh">Right-hand input vector.</param>
	/// <returns>Dot product of inputs.</returns>
	veci_t vecDot3i(int3_t const v_lh, int3_t const v_rh);

	/// <summary>
	/// Calculate cross product of input vectors.
	/// </summary>
	/// <param name="v_out">Result vector.</param>
	/// <param name="v_lh">Left-hand input vector.</param>
	/// <param name="v_rh">Right-hand input vector.</param>
	/// <returns><paramref name="v_out"/></returns>
	intN_t vecCross3i(int3_t v_out, int3_t const v_lh, int3_t const v_rh);

	/// <summary>
	/// Calculate squared length or magnitude of vector.
	/// </summary>
	/// <param name="v">Input vector.</param>
	/// <returns>Squared length of input.</returns>
	veci_t vecLenSq3i(int3_t const v);

	/// <summary>
	/// Calculate component-wise multiply-add of origin vector, delta vector and scaling parameter.
	/// </summary>
	/// <param name="v_out">Result vector.</param>
	/// <param name="u">Scaling parameter applied to delta.</param>
	/// <param name="v_origin">Origin vector (value at u=0).</param>
	/// <param name="v_delta">Delta vector.</param>
	/// <returns><paramref name="v_out"/></returns>
	intN_t vecMad3i(int3_t v_out, veci_t const u, int3_t const v_origin, int3_t const v_delta);

	/// <summary>
	/// Calculate component-wise linear interprolation between origin and target given parameter.
	/// </summary>
	/// <param name="v_out">Result vector.</param>
	/// <param name="u">Interpolation parameter in [0,1] for interpolation, otherwise extrapolation.</param>
	/// <param name="v_min">Minimum (origin) vector for interpolation interval (value at u=0).</param>
	/// <param name="v_max">Maximum (target) vector for interpolation interval (value at u=1).</param>
	/// <returns><paramref name="v_out"/></returns>
	intN_t vecLerp3i(int3_t v_out, veci_t const u, int3_t const v_min, int3_t const v_max);


#ifdef __cplusplus
}
#endif // #ifdef __cplusplus


#endif // #if (!(defined _CDRAW_VEC3I_H_) && (defined _CDRAW_VECTOR_H_))