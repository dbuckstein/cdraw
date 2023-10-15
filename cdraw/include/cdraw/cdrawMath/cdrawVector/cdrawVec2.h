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
* cdrawVec2.h
* Declaration of preferred floating point precision 2D vector functions.
*/

#if (!(defined _CDRAW_VEC2_H_) && (defined _CDRAW_VECTOR_H_))
#define _CDRAW_VEC2_H_


#ifdef __cplusplus
extern "C" {
#endif // #ifdef __cplusplus

	/// <summary>
	/// Initialize vector to zero.
	/// </summary>
	/// <param name="v_out">Result vector.</param>
	/// <returns><paramref name="v_out"/></returns>
#define vecZero2 cdraw_vector_base(vecZero2)

	/// <summary>
	/// Initialize vector given individual components.
	/// </summary>
	/// <param name="v_out">Result vector.</param>
	/// <param name="x">First component.</param>
	/// <param name="y">Second component.</param>
	/// <returns><paramref name="v_out"/></returns>
#define vecInit2 cdraw_vector_base(vecInit2)

	/// <summary>
	/// Initialize all components of vector given single value.
	/// </summary>
	/// <param name="v_out">Result vector.</param>
	/// <param name="x">Value to assign to all components.</param>
	/// <returns><paramref name="v_out"/></returns>
#define vecInitS2 cdraw_vector_base(vecInitS2)

	/// <summary>
	/// Initialize vector by copying the leading components of another.
	/// </summary>
	/// <param name="v_out">Result vector.</param>
	/// <param name="v">Vector to copy.</param>
	/// <returns><paramref name="v_out"/></returns>
#define vecCopy2 cdraw_vector_base(vecCopy2)

	/// <summary>
	/// Initialize vector by copying the leading components of another.
	/// </summary>
	/// <param name="v_out">Result vector.</param>
	/// <param name="v">Vector to copy.</param>
	/// <returns><paramref name="v_out"/></returns>
#define vecCopy2b2 cdraw_vector_base(vecCopy2b2)

	/// <summary>
	/// Initialize vector by copying the leading components of another.
	/// </summary>
	/// <param name="v_out">Result vector.</param>
	/// <param name="v">Vector to copy.</param>
	/// <returns><paramref name="v_out"/></returns>
#define vecCopy2i2 cdraw_vector_base(vecCopy2i2)

	/// <summary>
	/// Initialize vector by copying the leading components of another.
	/// </summary>
	/// <param name="v_out">Result vector.</param>
	/// <param name="v">Vector to copy.</param>
	/// <returns><paramref name="v_out"/></returns>
#define vecCopy2u2 cdraw_vector_base(vecCopy2u2)

	/// <summary>
	/// Initialize vector by copying the leading components of another.
	/// </summary>
	/// <param name="v_out">Result vector.</param>
	/// <param name="v">Vector to copy.</param>
	/// <returns><paramref name="v_out"/></returns>
#define vecCopy22 tokencat(tokencat(vecCopy2, vector_other), tokencat(2, vector_suffix))

	/// <summary>
	/// Test component-wise equality (left-hand input equals right-hand input).
	/// </summary>
	/// <param name="v_out">Result vector.</param>
	/// <param name="v_lh">Left-hand input vector.</param>
	/// <param name="v_rh">Right-hand input vector.</param>
	/// <returns><paramref name="v_out"/></returns>
#define vecEq2 cdraw_vector_base(vecEq2)

	/// <summary>
	/// Test component-wise inequality (left-hand input does not equal right-hand input).
	/// </summary>
	/// <param name="v_out">Result vector.</param>
	/// <param name="v_lh">Left-hand input vector.</param>
	/// <param name="v_rh">Right-hand input vector.</param>
	/// <returns><paramref name="v_out"/></returns>
#define vecNeq2 cdraw_vector_base(vecNeq2)

	/// <summary>
	/// Test component-wise greater-than (left-hand input is greater than right-hand input).
	/// </summary>
	/// <param name="v_out">Result vector.</param>
	/// <param name="v_lh">Left-hand input vector.</param>
	/// <param name="v_rh">Right-hand input vector.</param>
	/// <returns><paramref name="v_out"/></returns>
#define vecGreater2 cdraw_vector_base(vecGreater2)

	/// <summary>
	/// Test component-wise greater-than-or-equal-to (left-hand input is greater than or equal to right-hand input).
	/// </summary>
	/// <param name="v_out">Result vector.</param>
	/// <param name="v_lh">Left-hand input vector.</param>
	/// <param name="v_rh">Right-hand input vector.</param>
	/// <returns><paramref name="v_out"/></returns>
#define vecGreaterEq2 cdraw_vector_base(vecGreaterEq2)

	/// <summary>
	/// Test component-wise less-than (left-hand input is less than right-hand input).
	/// </summary>
	/// <param name="v_out">Result vector.</param>
	/// <param name="v_lh">Left-hand input vector.</param>
	/// <param name="v_rh">Right-hand input vector.</param>
	/// <returns><paramref name="v_out"/></returns>
#define vecLess2 cdraw_vector_base(vecLess2)

	/// <summary>
	/// Test component-wise less-than-or-equal-to (left-hand input is less than or equal to right-hand input).
	/// </summary>
	/// <param name="v_out">Result vector.</param>
	/// <param name="v_lh">Left-hand input vector.</param>
	/// <param name="v_rh">Right-hand input vector.</param>
	/// <returns><paramref name="v_out"/></returns>
#define vecLessEq2 cdraw_vector_base(vecLessEq2)

	/// <summary>
	/// Calculate component-wise negative of input vector.
	/// </summary>
	/// <param name="v_out">Result vector.</param>
	/// <param name="v">Input vector.</param>
	/// <returns><paramref name="v_out"/></returns>
#define vecNegate2 cdraw_vector_base(vecNegate2)

	/// <summary>
	/// Calculate component-wise sum of input vectors (left-hand plus right-hand).
	/// </summary>
	/// <param name="v_out">Result vector.</param>
	/// <param name="v_lh">Left-hand input vector.</param>
	/// <param name="v_rh">Right-hand input vector.</param>
	/// <returns><paramref name="v_out"/></returns>
#define vecAdd2 cdraw_vector_base(vecAdd2)

	/// <summary>
	/// Calculate component-wise difference of input vectors (left-hand minus right-hand).
	/// </summary>
	/// <param name="v_out">Result vector.</param>
	/// <param name="v_lh">Left-hand input vector.</param>
	/// <param name="v_rh">Right-hand input vector.</param>
	/// <returns><paramref name="v_out"/></returns>
#define vecSub2 cdraw_vector_base(vecSub2)

	/// <summary>
	/// Calculate component-wise product of input vectors (left-hand times right-hand).
	/// </summary>
	/// <param name="v_out">Result vector.</param>
	/// <param name="v_lh">Left-hand input vector.</param>
	/// <param name="v_rh">Right-hand input vector.</param>
	/// <returns><paramref name="v_out"/></returns>
#define vecMul2 cdraw_vector_base(vecMul2)

	/// <summary>
	/// Calculate component-wise quotient of input vectors (left-hand over right-hand).
	/// </summary>
	/// <param name="v_out">Result vector.</param>
	/// <param name="v_lh">Left-hand input vector.</param>
	/// <param name="v_rh">Right-hand input vector.</param>
	/// <returns><paramref name="v_out"/></returns>
#define vecDiv2 cdraw_vector_base(vecDiv2)

	/// <summary>
	/// Calculate component-wise modulo of input vectors (left-hand over right-hand).
	/// </summary>
	/// <param name="v_out">Result vector.</param>
	/// <param name="v_lh">Left-hand input vector.</param>
	/// <param name="v_rh">Right-hand input vector.</param>
	/// <returns><paramref name="v_out"/></returns>
#define vecMod2 cdraw_vector_base(vecMod2)

	/// <summary>
	/// Calculate component-wise quotient and modulo of input vectors (left-hand over right-hand).
	/// </summary>
	/// <param name="v_out">Result vector (quotient).</param>
	/// <param name="v_mod_out">Result vector (modulo).</param>
	/// <param name="v_lh">Left-hand input vector.</param>
	/// <param name="v_rh">Right-hand input vector.</param>
	/// <returns><paramref name="v_out"/></returns>
#define vecDivMod2 cdraw_vector_base(vecDivMod2)

	/// <summary>
	/// Calculate component-wise sum of input vector and scalar (left-hand times right-hand).
	/// </summary>
	/// <param name="v_out">Result vector.</param>
	/// <param name="v_lh">Left-hand input vector.</param>
	/// <param name="s_rh">Right-hand input scalar.</param>
	/// <returns><paramref name="v_out"/></returns>
#define vecAddS2 cdraw_vector_base(vecAddS2)

	/// <summary>
	/// Calculate component-wise difference of input vector and scalar (left-hand times right-hand).
	/// </summary>
	/// <param name="v_out">Result vector.</param>
	/// <param name="v_lh">Left-hand input vector.</param>
	/// <param name="s_rh">Right-hand input scalar.</param>
	/// <returns><paramref name="v_out"/></returns>
#define vecSubS2 cdraw_vector_base(vecSubS2)

	/// <summary>
	/// Calculate product of input vector and scalar (left-hand times right-hand).
	/// </summary>
	/// <param name="v_out">Result vector.</param>
	/// <param name="v_lh">Left-hand input vector.</param>
	/// <param name="s_rh">Right-hand input scalar.</param>
	/// <returns><paramref name="v_out"/></returns>
#define vecMulS2 cdraw_vector_base(vecMulS2)

	/// <summary>
	/// Calculate quotient of input vector and scalar (left-hand over right-hand).
	/// </summary>
	/// <param name="v_out">Result vector.</param>
	/// <param name="v_lh">Left-hand input vector.</param>
	/// <param name="s_rh">Right-hand input scalar.</param>
	/// <returns><paramref name="v_out"/></returns>
#define vecDivS2 cdraw_vector_base(vecDivS2)

	/// <summary>
	/// Calculate modulo of input vector and scalar (left-hand over right-hand).
	/// </summary>
	/// <param name="v_out">Result vector.</param>
	/// <param name="v_lh">Left-hand input vector.</param>
	/// <param name="s_rh">Right-hand input scalar.</param>
	/// <returns><paramref name="v_out"/></returns>
#define vecModS2 cdraw_vector_base(vecModS2)

	/// <summary>
	/// Calculate quotient and modulo of input vector and scalar (left-hand over right-hand).
	/// </summary>
	/// <param name="v_out">Result vector (quotient).</param>
	/// <param name="v_mod_out">Result vector (modulo).</param>
	/// <param name="v_lh">Left-hand input vector.</param>
	/// <param name="s_rh">Right-hand input scalar.</param>
	/// <returns><paramref name="v_out"/></returns>
#define vecDivModS2 cdraw_vector_base(vecDivModS2)

	/// <summary>
	/// Calculate dot product of input vectors (the scalar sum of component products).
	/// </summary>
	/// <param name="v_lh">Left-hand input vector.</param>
	/// <param name="v_rh">Right-hand input vector.</param>
	/// <returns>Dot product of inputs.</returns>
#define vecDot2 cdraw_vector_base(vecDot2)

	/// <summary>
	/// Calculate cross product of input vectors (for 2D, just the singular perpendicular component).
	/// </summary>
	/// <param name="v_lh">Left-hand input vector.</param>
	/// <param name="v_rh">Right-hand input vector.</param>
	/// <returns>Cross product of inputs.</returns>
#define vecCross2 cdraw_vector_base(vecCross2)

	/// <summary>
	/// Calculate squared length or magnitude of vector.
	/// </summary>
	/// <param name="v">Input vector.</param>
	/// <returns>Squared length of input.</returns>
#define vecLenSq2 cdraw_vector_base(vecLenSq2)

	/// <summary>
	/// Calculate the squared distance between two vectors.
	/// </summary>
	/// <param name="v_lh">Left-hand input vector.</param>
	/// <param name="v_rh">Right-hand input vector.</param>
	/// <returns>Squared distance between inputs.</returns>
#define vecDistSq2 cdraw_vector_base(vecDistSq2)

	/// <summary>
	/// Calculate the squared distance between two vectors and store the displacement.
	/// </summary>
	/// <param name="v_disp_out">Result vector (displacement).</param>
	/// <param name="v_lh">Left-hand input vector.</param>
	/// <param name="v_rh">Right-hand input vector.</param>
	/// <returns>Squared distance between inputs.</returns>
#define vecDispDistSq2 cdraw_vector_base(vecDispDistSq2)

	/// <summary>
	/// Determine if input is zero vector.
	/// </summary>
	/// <param name="v">Input vector.</param>
	/// <returns>True if input is considered zero vector.</returns>
#define vecIsZero2 cdraw_vector_base(vecIsZero2)

	/// <summary>
	/// Determine if input is not zero vector.
	/// </summary>
	/// <param name="v">Input vector.</param>
	/// <returns>True if input is not considered zero vector.</returns>
#define vecIsNonZero2 cdraw_vector_base(vecIsNonZero2)

	/// <summary>
	/// Calculate component-wise multiply-add of origin vector, delta vector and scaling parameter.
	/// </summary>
	/// <param name="v_out">Result vector.</param>
	/// <param name="u">Scaling parameter applied to delta.</param>
	/// <param name="v_origin">Origin vector (value at u=0).</param>
	/// <param name="v_delta">Delta vector.</param>
	/// <returns><paramref name="v_out"/></returns>
#define vecMad2 cdraw_vector_base(vecMad2)

	/// <summary>
	/// Calculate component-wise linear interprolation between origin and target given parameter.
	/// </summary>
	/// <param name="v_out">Result vector.</param>
	/// <param name="u">Interpolation parameter in [0,1] for interpolation, otherwise extrapolation.</param>
	/// <param name="v_min">Minimum (origin) vector for interpolation interval (value at u=0).</param>
	/// <param name="v_max">Maximum (target) vector for interpolation interval (value at u=1).</param>
	/// <returns><paramref name="v_out"/></returns>
#define vecLerp2 cdraw_vector_base(vecLerp2)

	/// <summary>
	/// Calculate the length or magnitude of the vector (analogous to scalar absolute value).
	/// </summary>
	/// <param name="v">Input vector.</param>
	/// <returns>Length of vector.</returns>
#define vecLen2 cdraw_vector_base(vecLen2)

	/// <summary>
	/// Calculate the inverse (reciprocal) length or magnitude of the vector.
	/// </summary>
	/// <param name="v">Input vector.</param>
	/// <returns>Inverse length of vector.</returns>
#define vecLenInv2 cdraw_vector_base(vecLenInv2)

	/// <summary>
	/// Calculate the distance between two vectors.
	/// </summary>
	/// <param name="v_lh">Left-hand input vector.</param>
	/// <param name="v_rh">Right-hand input vector.</param>
	/// <returns>Distance between inputs.</returns>
#define vecDist2 cdraw_vector_base(vecDist2)

	/// <summary>
	/// Calculate the distance between two vectors and store the displacement.
	/// </summary>
	/// <param name="v_disp_out">Result vector (displacement).</param>
	/// <param name="v_lh">Left-hand input vector.</param>
	/// <param name="v_rh">Right-hand input vector.</param>
	/// <returns>Distance between inputs.</returns>
#define vecDispDist2 cdraw_vector_base(vecDispDist2)

	/// <summary>
	/// Calculate the unit-length direction of the vector (analogous to scalar sign).
	/// </summary>
	/// <param name="v_out">Result vector (unit direction).</param>
	/// <param name="v">Input vector.</param>
	/// <returns>Length of input vector.</returns>
#define vecNormalize2 cdraw_vector_base(vecNormalize2)

	/// <summary>
	/// Calculate a resized vector in the same direction as the input.
	/// </summary>
	/// <param name="v_out">Result vector (same direction, new length).</param>
	/// <param name="v">Input vector.</param>
	/// <param name="newLen">Desired length of vector.</param>
	/// <returns>Length of input vector.</returns>
#define vecResize2 cdraw_vector_base(vecResize2)

	/// <summary>
	/// Determine if vector is unit length (normalized).
	/// </summary>
	/// <param name="v">Input vector.</param>
	/// <returns>True if vector is considered unit length.</returns>
#define vecIsUnit2 cdraw_vector_base(vecIsUnit2)

	/// <summary>
	/// Determine if vector is non-unit length (normalized).
	/// </summary>
	/// <param name="v">Input vector.</param>
	/// <returns>True if vector is not considered unit length.</returns>
#define vecIsNonUnit2 cdraw_vector_base(vecIsNonUnit2)

	/// <summary>
	/// Calculate projection factor of base vector required to project input onto base.
	/// </summary>
	/// <param name="v">Input vector.</param>
	/// <param name="v_base">Base vector (direction of projected vector).</param>
	/// <returns>Factor of base for projection.</returns>
#define vecProjS2 cdraw_vector_base(vecProjS2)

	/// <summary>
	/// Calculate projection of input vector onto base.
	/// </summary>
	/// <param name="v_out">Result vector (input projected onto base).</param>
	/// <param name="v">Input vector.</param>
	/// <param name="v_base">Base vector (direction of projected vector).</param>
	/// <returns>Factor of base for projection.</returns>
#define vecProj2 cdraw_vector_base(vecProj2)

	/// <summary>
	/// Calculate interpolation parameter given potential interpolation result and ends of range.
	/// </summary>
	/// <param name="v"></param>
	/// <param name="v_min"></param>
	/// <param name="v_max"></param>
	/// <returns></returns>
#define vecLerpInv2 cdraw_vector_base(vecLerpInv2)

	/// <summary>
	/// Calculate orthogonalized vector given input and base using Gram-Schmidt process.
	/// </summary>
	/// <param name="v_out">Result vector (input orthogonalized relative to base).</param>
	/// <param name="v">Input vector.</param>
	/// <param name="v_base">Base vector.</param>
	/// <returns>Factor of base for projection.</returns>
#define vecOrtho2 cdraw_vector_base(vecOrtho2)

	/// <summary>
	/// Initialize vector by copying the leading components of another.
	/// </summary>
	/// <param name="v_out">Result vector.</param>
	/// <param name="v">Vector to copy.</param>
	/// <returns><paramref name="v_out"/></returns>
#define vecCopy22b tokencat(vecCopy2, tokencat(vector_suffix, 2b))

	/// <summary>
	/// Initialize vector by copying the leading components of another.
	/// </summary>
	/// <param name="v_out">Result vector.</param>
	/// <param name="v">Vector to copy.</param>
	/// <returns><paramref name="v_out"/></returns>
#define vecCopy22i tokencat(vecCopy2, tokencat(vector_suffix, 2i))

	/// <summary>
	/// Initialize vector by copying the leading components of another.
	/// </summary>
	/// <param name="v_out">Result vector.</param>
	/// <param name="v">Vector to copy.</param>
	/// <returns><paramref name="v_out"/></returns>
#define vecCopy22u tokencat(vecCopy2, tokencat(vector_suffix, 2u))

	/// <summary>
	/// Initialize vector by copying the leading components of another.
	/// </summary>
	/// <param name="v_out">Result vector.</param>
	/// <param name="v">Vector to copy.</param>
	/// <returns><paramref name="v_out"/></returns>
#define vecCopy22f tokencat(vecCopy2, tokencat(vector_suffix, 2f))

	/// <summary>
	/// Initialize vector by copying the leading components of another.
	/// </summary>
	/// <param name="v_out">Result vector.</param>
	/// <param name="v">Vector to copy.</param>
	/// <returns><paramref name="v_out"/></returns>
#define vecCopy22d tokencat(vecCopy2, tokencat(vector_suffix, 2d))


#ifdef __cplusplus
}
#endif // #ifdef __cplusplus


#endif // #if (!(defined _CDRAW_VEC2_H_) && (defined _CDRAW_VECTOR_H_))