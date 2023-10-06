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
* cdrawVec4.h
* Declaration of preferred floating point precision 4D vector functions.
*/

#if (!(defined _CDRAW_VEC4_H_) && (defined _CDRAW_VECTOR_H_))
#define _CDRAW_VEC4_H_


#ifdef __cplusplus
extern "C" {
#endif // #ifdef __cplusplus

	/// <summary>
	/// Initialize vector to zero.
	/// </summary>
	/// <param name="v_out">Result vector.</param>
	/// <returns><paramref name="v_out"/></returns>
#define vecZero4 cdraw_vector_base(vecZero4)

	/// <summary>
	/// Initialize vector given individual components.
	/// </summary>
	/// <param name="v_out">Result vector.</param>
	/// <param name="x">First component.</param>
	/// <param name="y">Second component.</param>
	/// <param name="z">Third component.</param>
	/// <param name="w">Fourth component.</param>
	/// <returns><paramref name="v_out"/></returns>
#define vecInit4 cdraw_vector_base(vecInit4)

	/// <summary>
	/// Initialize all components of vector given single value.
	/// </summary>
	/// <param name="v_out">Result vector.</param>
	/// <param name="x">Value to assign to all components.</param>
	/// <returns><paramref name="v_out"/></returns>
#define vecInitS4 cdraw_vector_base(vecInitS4)

	/// <summary>
	/// Initialize vector by copying another and additional components.
	/// </summary>
	/// <param name="v_out">Result vector.</param>
	/// <param name="xy">Vector to copy.</param>
	/// <param name="z">Third component.</param>
	/// <param name="w">Fourth component.</param>
	/// <returns><paramref name="v_out"/></returns>
#define vecCopy2zw4 cdraw_vector_base(vecCopy2zw4)

	/// <summary>
	/// Initialize vector by copying another and additional components.
	/// </summary>
	/// <param name="v_out">Result vector.</param>
	/// <param name="xyz">Vector to copy.</param>
	/// <param name="w">Fourth component.</param>
	/// <returns><paramref name="v_out"/></returns>
#define vecCopy3w4 cdraw_vector_base(vecCopy3w4)

	/// <summary>
	/// Initialize vector by copying the leading components of another.
	/// </summary>
	/// <param name="v_out">Result vector.</param>
	/// <param name="v">Vector to copy.</param>
	/// <returns><paramref name="v_out"/></returns>
#define vecCopy4 cdraw_vector_base(vecCopy4)

	/// <summary>
	/// Initialize vector by copying the leading components of another.
	/// </summary>
	/// <param name="v_out">Result vector.</param>
	/// <param name="v">Vector to copy.</param>
	/// <returns><paramref name="v_out"/></returns>
#define vecCopy4b4 cdraw_vector_base(vecCopy4b4)

	/// <summary>
	/// Initialize vector by copying the leading components of another.
	/// </summary>
	/// <param name="v_out">Result vector.</param>
	/// <param name="v">Vector to copy.</param>
	/// <returns><paramref name="v_out"/></returns>
#define vecCopy4i4 cdraw_vector_base(vecCopy4i4)

	/// <summary>
	/// Initialize vector by copying the leading components of another.
	/// </summary>
	/// <param name="v_out">Result vector.</param>
	/// <param name="v">Vector to copy.</param>
	/// <returns><paramref name="v_out"/></returns>
#define vecCopy4u4 cdraw_vector_base(vecCopy4u4)

	/// <summary>
	/// Initialize vector by copying the leading components of another.
	/// </summary>
	/// <param name="v_out">Result vector.</param>
	/// <param name="v">Vector to copy.</param>
	/// <returns><paramref name="v_out"/></returns>
#define vecCopy44 tokencat(tokencat(vecCopy4, vector_other), tokencat(4, vector_suffix))

	/// <summary>
	/// Test component-wise equality (left-hand input equals right-hand input).
	/// </summary>
	/// <param name="v_out">Result vector.</param>
	/// <param name="v_lh">Left-hand input vector.</param>
	/// <param name="v_rh">Right-hand input vector.</param>
	/// <returns><paramref name="v_out"/></returns>
#define vecEq4 cdraw_vector_base(vecEq4)

	/// <summary>
	/// Test component-wise inequality (left-hand input does not equal right-hand input).
	/// </summary>
	/// <param name="v_out">Result vector.</param>
	/// <param name="v_lh">Left-hand input vector.</param>
	/// <param name="v_rh">Right-hand input vector.</param>
	/// <returns><paramref name="v_out"/></returns>
#define vecNeq4 cdraw_vector_base(vecNeq4)

	/// <summary>
	/// Test component-wise greater-than (left-hand input is greater than right-hand input).
	/// </summary>
	/// <param name="v_out">Result vector.</param>
	/// <param name="v_lh">Left-hand input vector.</param>
	/// <param name="v_rh">Right-hand input vector.</param>
	/// <returns><paramref name="v_out"/></returns>
#define vecGreater4 cdraw_vector_base(vecGreater4)

	/// <summary>
	/// Test component-wise greater-than-or-equal-to (left-hand input is greater than or equal to right-hand input).
	/// </summary>
	/// <param name="v_out">Result vector.</param>
	/// <param name="v_lh">Left-hand input vector.</param>
	/// <param name="v_rh">Right-hand input vector.</param>
	/// <returns><paramref name="v_out"/></returns>
#define vecGreaterEq4 cdraw_vector_base(vecGreaterEq4)

	/// <summary>
	/// Test component-wise less-than (left-hand input is less than right-hand input).
	/// </summary>
	/// <param name="v_out">Result vector.</param>
	/// <param name="v_lh">Left-hand input vector.</param>
	/// <param name="v_rh">Right-hand input vector.</param>
	/// <returns><paramref name="v_out"/></returns>
#define vecLess4 cdraw_vector_base(vecLess4)

	/// <summary>
	/// Test component-wise less-than-or-equal-to (left-hand input is less than or equal to right-hand input).
	/// </summary>
	/// <param name="v_out">Result vector.</param>
	/// <param name="v_lh">Left-hand input vector.</param>
	/// <param name="v_rh">Right-hand input vector.</param>
	/// <returns><paramref name="v_out"/></returns>
#define vecLessEq4 cdraw_vector_base(vecLessEq4)

	/// <summary>
	/// Calculate component-wise negative of input vector.
	/// </summary>
	/// <param name="v_out">Result vector.</param>
	/// <param name="v">Input vector.</param>
	/// <returns><paramref name="v_out"/></returns>
#define vecNegate4 cdraw_vector_base(vecNegate4)

	/// <summary>
	/// Calculate component-wise sum of input vectors (left-hand plus right-hand).
	/// </summary>
	/// <param name="v_out">Result vector.</param>
	/// <param name="v_lh">Left-hand input vector.</param>
	/// <param name="v_rh">Right-hand input vector.</param>
	/// <returns><paramref name="v_out"/></returns>
#define vecAdd4 cdraw_vector_base(vecAdd4)

	/// <summary>
	/// Calculate component-wise difference of input vectors (left-hand minus right-hand).
	/// </summary>
	/// <param name="v_out">Result vector.</param>
	/// <param name="v_lh">Left-hand input vector.</param>
	/// <param name="v_rh">Right-hand input vector.</param>
	/// <returns><paramref name="v_out"/></returns>
#define vecSub4 cdraw_vector_base(vecSub4)

	/// <summary>
	/// Calculate component-wise product of input vectors (left-hand times right-hand).
	/// </summary>
	/// <param name="v_out">Result vector.</param>
	/// <param name="v_lh">Left-hand input vector.</param>
	/// <param name="v_rh">Right-hand input vector.</param>
	/// <returns><paramref name="v_out"/></returns>
#define vecMul4 cdraw_vector_base(vecMul4)

	/// <summary>
	/// Calculate component-wise quotient of input vectors (left-hand over right-hand).
	/// </summary>
	/// <param name="v_out">Result vector.</param>
	/// <param name="v_lh">Left-hand input vector.</param>
	/// <param name="v_rh">Right-hand input vector.</param>
	/// <returns><paramref name="v_out"/></returns>
#define vecDiv4 cdraw_vector_base(vecDiv4)

	/// <summary>
	/// Calculate component-wise modulo of input vectors (left-hand over right-hand).
	/// </summary>
	/// <param name="v_out">Result vector.</param>
	/// <param name="v_lh">Left-hand input vector.</param>
	/// <param name="v_rh">Right-hand input vector.</param>
	/// <returns><paramref name="v_out"/></returns>
#define vecMod4 cdraw_vector_base(vecMod4)

	/// <summary>
	/// Calculate component-wise quotient and modulo of input vectors (left-hand over right-hand).
	/// </summary>
	/// <param name="v_out">Result vector (quotient).</param>
	/// <param name="v_mod_out">Result vector (modulo).</param>
	/// <param name="v_lh">Left-hand input vector.</param>
	/// <param name="v_rh">Right-hand input vector.</param>
	/// <returns><paramref name="v_out"/></returns>
#define vecDivMod4 cdraw_vector_base(vecDivMod4)

	/// <summary>
	/// Calculate component-wise sum of input vector and scalar (left-hand times right-hand).
	/// </summary>
	/// <param name="v_out">Result vector.</param>
	/// <param name="v_lh">Left-hand input vector.</param>
	/// <param name="s_rh">Right-hand input scalar.</param>
	/// <returns><paramref name="v_out"/></returns>
#define vecAddS4 cdraw_vector_base(vecAddS4)

	/// <summary>
	/// Calculate component-wise difference of input vector and scalar (left-hand times right-hand).
	/// </summary>
	/// <param name="v_out">Result vector.</param>
	/// <param name="v_lh">Left-hand input vector.</param>
	/// <param name="s_rh">Right-hand input scalar.</param>
	/// <returns><paramref name="v_out"/></returns>
#define vecSubS4 cdraw_vector_base(vecSubS4)

	/// <summary>
	/// Calculate product of input vector and scalar (left-hand times right-hand).
	/// </summary>
	/// <param name="v_out">Result vector.</param>
	/// <param name="v_lh">Left-hand input vector.</param>
	/// <param name="s_rh">Right-hand input scalar.</param>
	/// <returns><paramref name="v_out"/></returns>
#define vecMulS4 cdraw_vector_base(vecMulS4)

	/// <summary>
	/// Calculate quotient of input vector and scalar (left-hand over right-hand).
	/// </summary>
	/// <param name="v_out">Result vector.</param>
	/// <param name="v_lh">Left-hand input vector.</param>
	/// <param name="s_rh">Right-hand input scalar.</param>
	/// <returns><paramref name="v_out"/></returns>
#define vecDivS4 cdraw_vector_base(vecDivS4)

	/// <summary>
	/// Calculate modulo of input vector and scalar (left-hand over right-hand).
	/// </summary>
	/// <param name="v_out">Result vector.</param>
	/// <param name="v_lh">Left-hand input vector.</param>
	/// <param name="s_rh">Right-hand input scalar.</param>
	/// <returns><paramref name="v_out"/></returns>
#define vecModS4 cdraw_vector_base(vecModS4)

	/// <summary>
	/// Calculate quotient and modulo of input vector and scalar (left-hand over right-hand).
	/// </summary>
	/// <param name="v_out">Result vector (quotient).</param>
	/// <param name="v_mod_out">Result vector (modulo).</param>
	/// <param name="v_lh">Left-hand input vector.</param>
	/// <param name="s_rh">Right-hand input scalar.</param>
	/// <returns><paramref name="v_out"/></returns>
#define vecDivModS4 cdraw_vector_base(vecDivModS4)

	/// <summary>
	/// Calculate dot product of input vectors (the scalar sum of component products).
	/// </summary>
	/// <param name="v_lh">Left-hand input vector.</param>
	/// <param name="v_rh">Right-hand input vector.</param>
	/// <returns>Dot product of inputs.</returns>
#define vecDot4 cdraw_vector_base(vecDot4)

	/// <summary>
	/// Calculate cross product of input vectors (for 4D, the fourth component is zero).
	/// </summary>
	/// <param name="v_out">Result vector.</param>
	/// <param name="v_lh">Left-hand input vector.</param>
	/// <param name="v_rh">Right-hand input vector.</param>
	/// <returns><paramref name="v_out"/></returns>
#define vecCross4 cdraw_vector_base(vecCross4)

	/// <summary>
	/// Calculate squared length or magnitude of vector.
	/// </summary>
	/// <param name="v">Input vector.</param>
	/// <returns>Squared length of input.</returns>
#define vecLenSq4 cdraw_vector_base(vecLenSq4)

	/// <summary>
	/// Calculate the squared distance between two vectors.
	/// </summary>
	/// <param name="v_lh">Left-hand input vector.</param>
	/// <param name="v_rh">Right-hand input vector.</param>
	/// <returns>Squared distance between inputs.</returns>
#define vecDistSq4 cdraw_vector_base(vecDistSq4)

	/// <summary>
	/// Calculate the squared distance between two vectors and store the displacement.
	/// </summary>
	/// <param name="v_disp_out">Result vector (displacement).</param>
	/// <param name="v_lh">Left-hand input vector.</param>
	/// <param name="v_rh">Right-hand input vector.</param>
	/// <returns>Squared distance between inputs.</returns>
#define vecDispDistSq4 cdraw_vector_base(vecDispDistSq4)

	/// <summary>
	/// Determine if input is zero vector.
	/// </summary>
	/// <param name="v">Input vector.</param>
	/// <returns>True if input is considered zero vector.</returns>
#define vecIsZero4 cdraw_vector_base(vecIsZero4)

	/// <summary>
	/// Determine if input is not zero vector.
	/// </summary>
	/// <param name="v">Input vector.</param>
	/// <returns>True if input is not considered zero vector.</returns>
#define vecIsNonZero4 cdraw_vector_base(vecIsNonZero4)

	/// <summary>
	/// Calculate component-wise multiply-add of origin vector, delta vector and scaling parameter.
	/// </summary>
	/// <param name="v_out">Result vector.</param>
	/// <param name="u">Scaling parameter applied to delta.</param>
	/// <param name="v_origin">Origin vector (value at u=0).</param>
	/// <param name="v_delta">Delta vector.</param>
	/// <returns><paramref name="v_out"/></returns>
#define vecMad4 cdraw_vector_base(vecMad4)

	/// <summary>
	/// Calculate component-wise linear interprolation between origin and target given parameter.
	/// </summary>
	/// <param name="v_out">Result vector.</param>
	/// <param name="u">Interpolation parameter in [0,1] for interpolation, otherwise extrapolation.</param>
	/// <param name="v_min">Minimum (origin) vector for interpolation interval (value at u=0).</param>
	/// <param name="v_max">Maximum (target) vector for interpolation interval (value at u=1).</param>
	/// <returns><paramref name="v_out"/></returns>
#define vecLerp4 cdraw_vector_base(vecLerp4)

	/// <summary>
	/// Calculate the length or magnitude of the vector (analogous to scalar absolute value).
	/// </summary>
	/// <param name="v">Input vector.</param>
	/// <returns>Length of vector.</returns>
#define vecLen4 cdraw_vector_base(vecLen4)

	/// <summary>
	/// Calculate the inverse (reciprocal) length or magnitude of the vector.
	/// </summary>
	/// <param name="v">Input vector.</param>
	/// <returns>Inverse length of vector.</returns>
#define vecLenInv4 cdraw_vector_base(vecLenInv4)

	/// <summary>
	/// Calculate the distance between two vectors.
	/// </summary>
	/// <param name="v_lh">Left-hand input vector.</param>
	/// <param name="v_rh">Right-hand input vector.</param>
	/// <returns>Distance between inputs.</returns>
#define vecDist4 cdraw_vector_base(vecDist4)

	/// <summary>
	/// Calculate the distance between two vectors and store the displacement.
	/// </summary>
	/// <param name="v_disp_out">Result vector (displacement).</param>
	/// <param name="v_lh">Left-hand input vector.</param>
	/// <param name="v_rh">Right-hand input vector.</param>
	/// <returns>Distance between inputs.</returns>
#define vecDispDist4 cdraw_vector_base(vecDispDist4)

	/// <summary>
	/// Calculate the unit-length direction of the vector (analogous to scalar sign).
	/// </summary>
	/// <param name="v_out">Result vector (unit direction).</param>
	/// <param name="v">Input vector.</param>
	/// <returns>Length of input vector.</returns>
#define vecNormalize4 cdraw_vector_base(vecNormalize4)

	/// <summary>
	/// Calculate a resized vector in the same direction as the input.
	/// </summary>
	/// <param name="v_out">Result vector (same direction, new length).</param>
	/// <param name="v">Input vector.</param>
	/// <param name="newLen">Desired length of vector.</param>
	/// <returns>Length of input vector.</returns>
#define vecResize4 cdraw_vector_base(vecResize4)

	/// <summary>
	/// Determine if vector is unit length (normalized).
	/// </summary>
	/// <param name="v">Input vector.</param>
	/// <returns>True if vector is considered unit length.</returns>
#define vecIsUnit4 cdraw_vector_base(vecIsUnit4)

	/// <summary>
	/// Determine if vector is non-unit length (normalized).
	/// </summary>
	/// <param name="v">Input vector.</param>
	/// <returns>True if vector is not considered unit length.</returns>
#define vecIsNonUnit4 cdraw_vector_base(vecIsNonUnit4)

	/// <summary>
	/// Calculate projection factor of base vector required to project input onto base.
	/// </summary>
	/// <param name="v">Input vector.</param>
	/// <param name="v_base">Base vector (direction of projected vector).</param>
	/// <returns>Factor of base for projection.</returns>
#define vecProjS4 cdraw_vector_base(vecProjS4)

	/// <summary>
	/// Calculate projection of input vector onto base.
	/// </summary>
	/// <param name="v_out">Result vector (input projected onto base).</param>
	/// <param name="v">Input vector.</param>
	/// <param name="v_base">Base vector (direction of projected vector).</param>
	/// <returns>Factor of base for projection.</returns>
#define vecProj4 cdraw_vector_base(vecProj4)

	/// <summary>
	/// Calculate interpolation parameter given potential interpolation result and ends of range.
	/// </summary>
	/// <param name="v"></param>
	/// <param name="v_min"></param>
	/// <param name="v_max"></param>
	/// <returns></returns>
#define vecLerpInv4 cdraw_vector_base(vecLerpInv4)

	/// <summary>
	/// Calculate orthogonalized vector given input and base using Gram-Schmidt process.
	/// </summary>
	/// <param name="v_out">Result vector (input orthogonalized relative to base).</param>
	/// <param name="v">Input vector.</param>
	/// <param name="v_base">Base vector.</param>
	/// <returns>Factor of base for projection.</returns>
#define vecOrtho4 cdraw_vector_base(vecOrtho4)

	/// <summary>
	/// Calculate orthogonized basis given two inputs and base using Gram-Schmidt process.
	/// </summary>
	/// <param name="v2_out">Second result vector (second input orthogonalized relative to base and first input).</param>
	/// <param name="v1_out">First result vector (first input orthogonalized relative to base).</param>
	/// <param name="v_base_out">Corrected base vector for 4D.</param>
	/// <param name="v2_basefactor_out_opt">Factor of base for projection of second input (optional).</param>
	/// <param name="v1_basefactor_out_opt">Factor of base for projection of first input (optional).</param>
	/// <param name="v2">Second input vector.</param>
	/// <param name="v1">First input vector.</param>
	/// <param name="v_base">Base vector.</param>
	/// <returns>Factor of first input for projection.</returns>
#define vecOrthoBasis4 cdraw_vector_base(vecOrthoBasis4)

	/// <summary>
	/// Initialize vector by copying the leading components of another.
	/// </summary>
	/// <param name="v_out">Result vector.</param>
	/// <param name="v">Vector to copy.</param>
	/// <returns><paramref name="v_out"/></returns>
#define vecCopy44b tokencat(vecCopy4, tokencat(vector_suffix, 4b))

	/// <summary>
	/// Initialize vector by copying the leading components of another.
	/// </summary>
	/// <param name="v_out">Result vector.</param>
	/// <param name="v">Vector to copy.</param>
	/// <returns><paramref name="v_out"/></returns>
#define vecCopy44i tokencat(vecCopy4, tokencat(vector_suffix, 4i))

	/// <summary>
	/// Initialize vector by copying the leading components of another.
	/// </summary>
	/// <param name="v_out">Result vector.</param>
	/// <param name="v">Vector to copy.</param>
	/// <returns><paramref name="v_out"/></returns>
#define vecCopy44u tokencat(vecCopy4, tokencat(vector_suffix, 4u))

	/// <summary>
	/// Initialize vector by copying the leading components of another.
	/// </summary>
	/// <param name="v_out">Result vector.</param>
	/// <param name="v">Vector to copy.</param>
	/// <returns><paramref name="v_out"/></returns>
#define vecCopy44f tokencat(vecCopy4, tokencat(vector_suffix, 4f))

	/// <summary>
	/// Initialize vector by copying the leading components of another.
	/// </summary>
	/// <param name="v_out">Result vector.</param>
	/// <param name="v">Vector to copy.</param>
	/// <returns><paramref name="v_out"/></returns>
#define vecCopy44d tokencat(vecCopy4, tokencat(vector_suffix, 4d))


#ifdef __cplusplus
}
#endif // #ifdef __cplusplus


#endif // #if (!(defined _CDRAW_VEC4_H_) && (defined _CDRAW_VECTOR_H_))