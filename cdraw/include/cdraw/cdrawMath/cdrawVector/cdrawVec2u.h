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
* cdrawVec2u.h
* Declaration of unsigned 2D vector functions.
*/

#if (!(defined _CDRAW_VEC2U_H_) && (defined _CDRAW_VECTOR_H_))
#define _CDRAW_VEC2U_H_


#ifdef __cplusplus
extern "C" {
#endif // #ifdef __cplusplus

	/// <summary>
	/// Initialize vector to zero.
	/// </summary>
	/// <param name="v_out">Result vector.</param>
	/// <returns><paramref name="v_out"/></returns>
	uintN_t vecZero2u(uint2_t v_out);

	/// <summary>
	/// Initialize vector given individual components.
	/// </summary>
	/// <param name="v_out">Result vector.</param>
	/// <param name="x">First component.</param>
	/// <param name="y">Second component.</param>
	/// <returns><paramref name="v_out"/></returns>
	uintN_t vecInit2u(uint2_t v_out, vecu_t const x, vecu_t const y);

	/// <summary>
	/// Initialize all components of vector given single value.
	/// </summary>
	/// <param name="v_out">Result vector.</param>
	/// <param name="x">Value to assign to all components.</param>
	/// <returns><paramref name="v_out"/></returns>
	uintN_t vecInitS2u(uint2_t v_out, vecu_t const x);

	/// <summary>
	/// Initialize vector by copying the leading components of another.
	/// </summary>
	/// <param name="v_out">Result vector.</param>
	/// <param name="v">Vector to copy.</param>
	/// <returns><paramref name="v_out"/></returns>
	uintN_t vecCopy2u(uint2_t v_out, uint2_t const v);

	/// <summary>
	/// Initialize vector by copying the leading components of another.
	/// </summary>
	/// <param name="v_out">Result vector.</param>
	/// <param name="v">Vector to copy.</param>
	/// <returns><paramref name="v_out"/></returns>
	uintN_t vecCopy2b2u(uint2_t v_out, bool2_t const v);

	/// <summary>
	/// Initialize vector by copying the leading components of another.
	/// </summary>
	/// <param name="v_out">Result vector.</param>
	/// <param name="v">Vector to copy.</param>
	/// <returns><paramref name="v_out"/></returns>
	uintN_t vecCopy2i2u(uint2_t v_out, int2_t const v);

	/// <summary>
	/// Initialize vector by copying the leading components of another.
	/// </summary>
	/// <param name="v_out">Result vector.</param>
	/// <param name="v">Vector to copy.</param>
	/// <returns><paramref name="v_out"/></returns>
	uintN_t vecCopy2f2u(uint2_t v_out, float2_t const v);

	/// <summary>
	/// Initialize vector by copying the leading components of another.
	/// </summary>
	/// <param name="v_out">Result vector.</param>
	/// <param name="v">Vector to copy.</param>
	/// <returns><paramref name="v_out"/></returns>
	uintN_t vecCopy2d2u(uint2_t v_out, double2_t const v);

	/// <summary>
	/// Evaluate component-wise bitwise NOT operator.
	/// </summary>
	/// <param name="v_out">Result vector.</param>
	/// <param name="v">Input vector.</param>
	/// <returns><paramref name="v_out"/></returns>
	uintN_t vecNot2u(uint2_t v_out, uint2_t const v);

	/// <summary>
	/// Evaluate component-wise bitwise AND operator.
	/// </summary>
	/// <param name="v_out">Result vector.</param>
	/// <param name="v_lh">Left-hand input vector.</param>
	/// <param name="v_rh">Right-hand input vector.</param>
	/// <returns><paramref name="v_out"/></returns>
	uintN_t vecAnd2u(uint2_t v_out, uint2_t const v_lh, uint2_t const v_rh);

	/// <summary>
	/// Evaluate component-wise bitwise NAND (NOT AND) operator.
	/// </summary>
	/// <param name="v_out">Result vector.</param>
	/// <param name="v_lh">Left-hand input vector.</param>
	/// <param name="v_rh">Right-hand input vector.</param>
	/// <returns><paramref name="v_out"/></returns>
	uintN_t vecNand2u(uint2_t v_out, uint2_t const v_lh, uint2_t const v_rh);

	/// <summary>
	/// Evaluate component-wise bitwise OR operator.
	/// </summary>
	/// <param name="v_out">Result vector.</param>
	/// <param name="v_lh">Left-hand input vector.</param>
	/// <param name="v_rh">Right-hand input vector.</param>
	/// <returns><paramref name="v_out"/></returns>
	uintN_t vecOr2u(uint2_t v_out, uint2_t const v_lh, uint2_t const v_rh);

	/// <summary>
	/// Evaluate component-wise bitwise NOR (NOT OR) operator.
	/// </summary>
	/// <param name="v_out">Result vector.</param>
	/// <param name="v_lh">Left-hand input vector.</param>
	/// <param name="v_rh">Right-hand input vector.</param>
	/// <returns><paramref name="v_out"/></returns>
	uintN_t vecNor2u(uint2_t v_out, uint2_t const v_lh, uint2_t const v_rh);

	/// <summary>
	/// Evaluate component-wise bitwise XOR (exclusive or) operator.
	/// </summary>
	/// <param name="v_out">Result vector.</param>
	/// <param name="v_lh">Left-hand input vector.</param>
	/// <param name="v_rh">Right-hand input vector.</param>
	/// <returns><paramref name="v_out"/></returns>
	uintN_t vecXor2u(uint2_t v_out, uint2_t const v_lh, uint2_t const v_rh);

	/// <summary>
	/// Evaluate component-wise bitwise XNOR (exclusive nor, NOT XOR) operator.
	/// </summary>
	/// <param name="v_out">Result vector.</param>
	/// <param name="v_lh">Left-hand input vector.</param>
	/// <param name="v_rh">Right-hand input vector.</param>
	/// <returns><paramref name="v_out"/></returns>
	uintN_t vecXnor2u(uint2_t v_out, uint2_t const v_lh, uint2_t const v_rh);

	/// <summary>
	/// Test component-wise equality (left-hand input equals right-hand input).
	/// </summary>
	/// <param name="v_out">Result vector.</param>
	/// <param name="v_lh">Left-hand input vector.</param>
	/// <param name="v_rh">Right-hand input vector.</param>
	/// <returns><paramref name="v_out"/></returns>
	boolN_t vecEq2u(bool2_t v_out, uint2_t const v_lh, uint2_t const v_rh);

	/// <summary>
	/// Test component-wise inequality (left-hand input does not equal right-hand input).
	/// </summary>
	/// <param name="v_out">Result vector.</param>
	/// <param name="v_lh">Left-hand input vector.</param>
	/// <param name="v_rh">Right-hand input vector.</param>
	/// <returns><paramref name="v_out"/></returns>
	boolN_t vecNeq2u(bool2_t v_out, uint2_t const v_lh, uint2_t const v_rh);

	/// <summary>
	/// Test component-wise greater-than (left-hand input is greater than right-hand input).
	/// </summary>
	/// <param name="v_out">Result vector.</param>
	/// <param name="v_lh">Left-hand input vector.</param>
	/// <param name="v_rh">Right-hand input vector.</param>
	/// <returns><paramref name="v_out"/></returns>
	boolN_t vecGreater2u(bool2_t v_out, uint2_t const v_lh, uint2_t const v_rh);

	/// <summary>
	/// Test component-wise greater-than-or-equal-to (left-hand input is greater than or equal to right-hand input).
	/// </summary>
	/// <param name="v_out">Result vector.</param>
	/// <param name="v_lh">Left-hand input vector.</param>
	/// <param name="v_rh">Right-hand input vector.</param>
	/// <returns><paramref name="v_out"/></returns>
	boolN_t vecGreaterEq2u(bool2_t v_out, uint2_t const v_lh, uint2_t const v_rh);

	/// <summary>
	/// Test component-wise less-than (left-hand input is less than right-hand input).
	/// </summary>
	/// <param name="v_out">Result vector.</param>
	/// <param name="v_lh">Left-hand input vector.</param>
	/// <param name="v_rh">Right-hand input vector.</param>
	/// <returns><paramref name="v_out"/></returns>
	boolN_t vecLess2u(bool2_t v_out, uint2_t const v_lh, uint2_t const v_rh);

	/// <summary>
	/// Test component-wise less-than-or-equal-to (left-hand input is less than or equal to right-hand input).
	/// </summary>
	/// <param name="v_out">Result vector.</param>
	/// <param name="v_lh">Left-hand input vector.</param>
	/// <param name="v_rh">Right-hand input vector.</param>
	/// <returns><paramref name="v_out"/></returns>
	boolN_t vecLessEq2u(bool2_t v_out, uint2_t const v_lh, uint2_t const v_rh);

	/// <summary>
	/// Calculate component-wise negative of input vector.
	/// </summary>
	/// <param name="v_out">Result vector.</param>
	/// <param name="v">Input vector.</param>
	/// <returns><paramref name="v_out"/></returns>
	intN_t vecNegate2u(int2_t v_out, uint2_t const v);

	/// <summary>
	/// Calculate component-wise sum of input vectors (left-hand plus right-hand).
	/// </summary>
	/// <param name="v_out">Result vector.</param>
	/// <param name="v_lh">Left-hand input vector.</param>
	/// <param name="v_rh">Right-hand input vector.</param>
	/// <returns><paramref name="v_out"/></returns>
	uintN_t vecAdd2u(uint2_t v_out, uint2_t const v_lh, uint2_t const v_rh);

	/// <summary>
	/// Calculate component-wise difference of input vectors (left-hand minus right-hand).
	/// </summary>
	/// <param name="v_out">Result vector.</param>
	/// <param name="v_lh">Left-hand input vector.</param>
	/// <param name="v_rh">Right-hand input vector.</param>
	/// <returns><paramref name="v_out"/></returns>
	intN_t vecSub2u(int2_t v_out, uint2_t const v_lh, uint2_t const v_rh);

	/// <summary>
	/// Calculate component-wise product of input vectors (left-hand times right-hand).
	/// </summary>
	/// <param name="v_out">Result vector.</param>
	/// <param name="v_lh">Left-hand input vector.</param>
	/// <param name="v_rh">Right-hand input vector.</param>
	/// <returns><paramref name="v_out"/></returns>
	uintN_t vecMul2u(uint2_t v_out, uint2_t const v_lh, uint2_t const v_rh);

	/// <summary>
	/// Calculate component-wise quotient of input vectors (left-hand over right-hand).
	/// </summary>
	/// <param name="v_out">Result vector.</param>
	/// <param name="v_lh">Left-hand input vector.</param>
	/// <param name="v_rh">Right-hand input vector.</param>
	/// <returns><paramref name="v_out"/></returns>
	uintN_t vecDiv2u(uint2_t v_out, uint2_t const v_lh, uint2_t const v_rh);

	/// <summary>
	/// Calculate component-wise modulo of input vectors (left-hand over right-hand).
	/// </summary>
	/// <param name="v_out">Result vector.</param>
	/// <param name="v_lh">Left-hand input vector.</param>
	/// <param name="v_rh">Right-hand input vector.</param>
	/// <returns><paramref name="v_out"/></returns>
	uintN_t vecMod2u(uint2_t v_out, uint2_t const v_lh, uint2_t const v_rh);

	/// <summary>
	/// Calculate component-wise quotient and modulo of input vectors (left-hand over right-hand).
	/// </summary>
	/// <param name="v_out">Result vector (quotient).</param>
	/// <param name="v_mod_out">Result vector (modulo).</param>
	/// <param name="v_lh">Left-hand input vector.</param>
	/// <param name="v_rh">Right-hand input vector.</param>
	/// <returns><paramref name="v_out"/></returns>
	uintN_t vecDivMod2u(uint2_t v_out, uint2_t v_mod_out, uint2_t const v_lh, uint2_t const v_rh);

	/// <summary>
	/// Calculate component-wise sum of input vector and scalar (left-hand times right-hand).
	/// </summary>
	/// <param name="v_out">Result vector.</param>
	/// <param name="v_lh">Left-hand input vector.</param>
	/// <param name="s_rh">Right-hand input scalar.</param>
	/// <returns><paramref name="v_out"/></returns>
	uintN_t vecAddS2u(uint2_t v_out, uint2_t const v_lh, vecu_t const s_rh);

	/// <summary>
	/// Calculate component-wise difference of input vector and scalar (left-hand times right-hand).
	/// </summary>
	/// <param name="v_out">Result vector.</param>
	/// <param name="v_lh">Left-hand input vector.</param>
	/// <param name="s_rh">Right-hand input scalar.</param>
	/// <returns><paramref name="v_out"/></returns>
	intN_t vecSubS2u(int2_t v_out, uint2_t const v_lh, vecu_t const s_rh);

	/// <summary>
	/// Calculate product of input vector and scalar (left-hand times right-hand).
	/// </summary>
	/// <param name="v_out">Result vector.</param>
	/// <param name="v_lh">Left-hand input vector.</param>
	/// <param name="s_rh">Right-hand input scalar.</param>
	/// <returns><paramref name="v_out"/></returns>
	uintN_t vecMulS2u(uint2_t v_out, uint2_t const v_lh, vecu_t const s_rh);

	/// <summary>
	/// Calculate quotient of input vector and scalar (left-hand over right-hand).
	/// </summary>
	/// <param name="v_out">Result vector.</param>
	/// <param name="v_lh">Left-hand input vector.</param>
	/// <param name="s_rh">Right-hand input scalar.</param>
	/// <returns><paramref name="v_out"/></returns>
	uintN_t vecDivS2u(uint2_t v_out, uint2_t const v_lh, vecu_t const s_rh);

	/// <summary>
	/// Calculate modulo of input vector and scalar (left-hand over right-hand).
	/// </summary>
	/// <param name="v_out">Result vector.</param>
	/// <param name="v_lh">Left-hand input vector.</param>
	/// <param name="s_rh">Right-hand input scalar.</param>
	/// <returns><paramref name="v_out"/></returns>
	uintN_t vecModS2u(uint2_t v_out, uint2_t const v_lh, vecu_t const s_rh);

	/// <summary>
	/// Calculate quotient and modulo of input vector and scalar (left-hand over right-hand).
	/// </summary>
	/// <param name="v_out">Result vector (quotient).</param>
	/// <param name="v_mod_out">Result vector (modulo).</param>
	/// <param name="v_lh">Left-hand input vector.</param>
	/// <param name="s_rh">Right-hand input scalar.</param>
	/// <returns><paramref name="v_out"/></returns>
	uintN_t vecDivModS2u(uint2_t v_out, uint2_t v_mod_out, uint2_t const v_lh, vecu_t const s_rh);

	/// <summary>
	/// Calculate dot product of input vectors (the scalar sum of component products).
	/// </summary>
	/// <param name="v_lh">Left-hand input vector.</param>
	/// <param name="v_rh">Right-hand input vector.</param>
	/// <returns>Dot product of inputs.</returns>
	vecu_t vecDot2u(uint2_t const v_lh, uint2_t const v_rh);

	/// <summary>
	/// Calculate cross product of input vectors (for 2D, just the singular perpendicular component).
	/// </summary>
	/// <param name="v_lh">Left-hand input vector.</param>
	/// <param name="v_rh">Right-hand input vector.</param>
	/// <returns>Cross product of inputs.</returns>
	veci_t vecCross2u(uint2_t const v_lh, uint2_t const v_rh);

	/// <summary>
	/// Calculate squared length or magnitude of vector.
	/// </summary>
	/// <param name="v">Input vector.</param>
	/// <returns>Squared length of input.</returns>
	vecu_t vecLenSq2u(uint2_t const v);

	/// <summary>
	/// Calculate the squared distance between two vectors.
	/// </summary>
	/// <param name="v_lh">Left-hand input vector.</param>
	/// <param name="v_rh">Right-hand input vector.</param>
	/// <returns>Squared distance between inputs.</returns>
	vecu_t vecDistSq2u(uint2_t const v_lh, uint2_t const v_rh);

	/// <summary>
	/// Calculate the squared distance between two vectors and store the displacement.
	/// </summary>
	/// <param name="v_disp_out">Result vector (displacement).</param>
	/// <param name="v_lh">Left-hand input vector.</param>
	/// <param name="v_rh">Right-hand input vector.</param>
	/// <returns>Squared distance between inputs.</returns>
	vecu_t vecDispDistSq2u(int2_t v_disp_out, uint2_t const v_lh, uint2_t const v_rh);

	/// <summary>
	/// Determine if input is zero vector.
	/// </summary>
	/// <param name="v">Input vector.</param>
	/// <returns>True if input is considered zero vector.</returns>
	vecb_t vecIsZero2u(uint2_t const v);

	/// <summary>
	/// Determine if input is not zero vector.
	/// </summary>
	/// <param name="v">Input vector.</param>
	/// <returns>True if input is not considered zero vector.</returns>
	vecb_t vecIsNonZero2u(uint2_t const v);

	/// <summary>
	/// Calculate component-wise multiply-add of origin vector, delta vector and scaling parameter.
	/// </summary>
	/// <param name="v_out">Result vector.</param>
	/// <param name="u">Scaling parameter applied to delta.</param>
	/// <param name="v_origin">Origin vector (value at u=0).</param>
	/// <param name="v_delta">Delta vector.</param>
	/// <returns><paramref name="v_out"/></returns>
	uintN_t vecMad2u(uint2_t v_out, vecu_t const u, uint2_t const v_origin, uint2_t const v_delta);

	/// <summary>
	/// Calculate component-wise linear uinterprolation between origin and target given parameter.
	/// </summary>
	/// <param name="v_out">Result vector.</param>
	/// <param name="u">uinterpolation parameter in [0,1] for uinterpolation, otherwise extrapolation.</param>
	/// <param name="v_min">Minimum (origin) vector for uinterpolation uinterval (value at u=0).</param>
	/// <param name="v_max">Maximum (target) vector for uinterpolation uinterval (value at u=1).</param>
	/// <returns><paramref name="v_out"/></returns>
	uintN_t vecLerp2u(uint2_t v_out, vecu_t const u, uint2_t const v_min, uint2_t const v_max);


#ifdef __cplusplus
}
#endif // #ifdef __cplusplus


#endif // #if (!(defined _CDRAW_VEC2U_H_) && (defined _CDRAW_VECTOR_H_))