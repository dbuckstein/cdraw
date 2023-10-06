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
* cdrawVec3f.h
* Declaration of single-precision 3D vector functions.
*/

#if (!(defined _CDRAW_VEC3F_H_) && (defined _CDRAW_VECTOR_H_))
#define _CDRAW_VEC3F_H_


#ifdef __cplusplus
extern "C" {
#endif // #ifdef __cplusplus

	/// <summary>
	/// Initialize vector to zero.
	/// </summary>
	/// <param name="v_out">Result vector.</param>
	/// <returns><paramref name="v_out"/></returns>
	floatN_t vecZero3f(float3_t v_out);

	/// <summary>
	/// Initialize vector given individual components.
	/// </summary>
	/// <param name="v_out">Result vector.</param>
	/// <param name="x">First component.</param>
	/// <param name="y">Second component.</param>
	/// <param name="z">Third component.</param>
	/// <returns><paramref name="v_out"/></returns>
	floatN_t vecInit3f(float3_t v_out, vecf_t const x, vecf_t const y, vecf_t const z);

	/// <summary>
	/// Initialize all components of vector given single value.
	/// </summary>
	/// <param name="v_out">Result vector.</param>
	/// <param name="x">Value to assign to all components.</param>
	/// <returns><paramref name="v_out"/></returns>
	floatN_t vecInitS3f(float3_t v_out, vecf_t const x);

	/// <summary>
	/// Initialize vector by copying another and additional components.
	/// </summary>
	/// <param name="v_out">Result vector.</param>
	/// <param name="xy">Vector to copy.</param>
	/// <param name="z">Third component.</param>
	/// <returns><paramref name="v_out"/></returns>
	floatN_t vecCopy2z3f(float3_t v_out, float2_t const xy, vecf_t const z);

	/// <summary>
	/// Initialize vector by copying the leading components of another.
	/// </summary>
	/// <param name="v_out">Result vector.</param>
	/// <param name="v">Vector to copy.</param>
	/// <returns><paramref name="v_out"/></returns>
	floatN_t vecCopy3f(float3_t v_out, float3_t const v);

	/// <summary>
	/// Initialize vector by copying the leading components of another.
	/// </summary>
	/// <param name="v_out">Result vector.</param>
	/// <param name="v">Vector to copy.</param>
	/// <returns><paramref name="v_out"/></returns>
	floatN_t vecCopy3b3f(float3_t v_out, bool3_t const v);

	/// <summary>
	/// Initialize vector by copying the leading components of another.
	/// </summary>
	/// <param name="v_out">Result vector.</param>
	/// <param name="v">Vector to copy.</param>
	/// <returns><paramref name="v_out"/></returns>
	floatN_t vecCopy3i3f(float3_t v_out, int3_t const v);

	/// <summary>
	/// Initialize vector by copying the leading components of another.
	/// </summary>
	/// <param name="v_out">Result vector.</param>
	/// <param name="v">Vector to copy.</param>
	/// <returns><paramref name="v_out"/></returns>
	floatN_t vecCopy3u3f(float3_t v_out, uint3_t const v);

	/// <summary>
	/// Initialize vector by copying the leading components of another.
	/// </summary>
	/// <param name="v_out">Result vector.</param>
	/// <param name="v">Vector to copy.</param>
	/// <returns><paramref name="v_out"/></returns>
	floatN_t vecCopy3d3f(float3_t v_out, double3_t const v);

	/// <summary>
	/// Test component-wise equality (left-hand input equals right-hand input).
	/// </summary>
	/// <param name="v_out">Result vector.</param>
	/// <param name="v_lh">Left-hand input vector.</param>
	/// <param name="v_rh">Right-hand input vector.</param>
	/// <returns><paramref name="v_out"/></returns>
	boolN_t vecEq3f(bool3_t v_out, float3_t const v_lh, float3_t const v_rh);

	/// <summary>
	/// Test component-wise inequality (left-hand input does not equal right-hand input).
	/// </summary>
	/// <param name="v_out">Result vector.</param>
	/// <param name="v_lh">Left-hand input vector.</param>
	/// <param name="v_rh">Right-hand input vector.</param>
	/// <returns><paramref name="v_out"/></returns>
	boolN_t vecNeq3f(bool3_t v_out, float3_t const v_lh, float3_t const v_rh);

	/// <summary>
	/// Test component-wise greater-than (left-hand input is greater than right-hand input).
	/// </summary>
	/// <param name="v_out">Result vector.</param>
	/// <param name="v_lh">Left-hand input vector.</param>
	/// <param name="v_rh">Right-hand input vector.</param>
	/// <returns><paramref name="v_out"/></returns>
	boolN_t vecGreater3f(bool3_t v_out, float3_t const v_lh, float3_t const v_rh);

	/// <summary>
	/// Test component-wise greater-than-or-equal-to (left-hand input is greater than or equal to right-hand input).
	/// </summary>
	/// <param name="v_out">Result vector.</param>
	/// <param name="v_lh">Left-hand input vector.</param>
	/// <param name="v_rh">Right-hand input vector.</param>
	/// <returns><paramref name="v_out"/></returns>
	boolN_t vecGreaterEq3f(bool3_t v_out, float3_t const v_lh, float3_t const v_rh);

	/// <summary>
	/// Test component-wise less-than (left-hand input is less than right-hand input).
	/// </summary>
	/// <param name="v_out">Result vector.</param>
	/// <param name="v_lh">Left-hand input vector.</param>
	/// <param name="v_rh">Right-hand input vector.</param>
	/// <returns><paramref name="v_out"/></returns>
	boolN_t vecLess3f(bool3_t v_out, float3_t const v_lh, float3_t const v_rh);

	/// <summary>
	/// Test component-wise less-than-or-equal-to (left-hand input is less than or equal to right-hand input).
	/// </summary>
	/// <param name="v_out">Result vector.</param>
	/// <param name="v_lh">Left-hand input vector.</param>
	/// <param name="v_rh">Right-hand input vector.</param>
	/// <returns><paramref name="v_out"/></returns>
	boolN_t vecLessEq3f(bool3_t v_out, float3_t const v_lh, float3_t const v_rh);

	/// <summary>
	/// Calculate component-wise negative of input vector.
	/// </summary>
	/// <param name="v_out">Result vector.</param>
	/// <param name="v">Input vector.</param>
	/// <returns><paramref name="v_out"/></returns>
	floatN_t vecNegate3f(float3_t v_out, float3_t const v);

	/// <summary>
	/// Calculate component-wise sum of input vectors (left-hand plus right-hand).
	/// </summary>
	/// <param name="v_out">Result vector.</param>
	/// <param name="v_lh">Left-hand input vector.</param>
	/// <param name="v_rh">Right-hand input vector.</param>
	/// <returns><paramref name="v_out"/></returns>
	floatN_t vecAdd3f(float3_t v_out, float3_t const v_lh, float3_t const v_rh);

	/// <summary>
	/// Calculate component-wise difference of input vectors (left-hand minus right-hand).
	/// </summary>
	/// <param name="v_out">Result vector.</param>
	/// <param name="v_lh">Left-hand input vector.</param>
	/// <param name="v_rh">Right-hand input vector.</param>
	/// <returns><paramref name="v_out"/></returns>
	floatN_t vecSub3f(float3_t v_out, float3_t const v_lh, float3_t const v_rh);

	/// <summary>
	/// Calculate component-wise product of input vectors (left-hand times right-hand).
	/// </summary>
	/// <param name="v_out">Result vector.</param>
	/// <param name="v_lh">Left-hand input vector.</param>
	/// <param name="v_rh">Right-hand input vector.</param>
	/// <returns><paramref name="v_out"/></returns>
	floatN_t vecMul3f(float3_t v_out, float3_t const v_lh, float3_t const v_rh);

	/// <summary>
	/// Calculate component-wise quotient of input vectors (left-hand over right-hand).
	/// </summary>
	/// <param name="v_out">Result vector.</param>
	/// <param name="v_lh">Left-hand input vector.</param>
	/// <param name="v_rh">Right-hand input vector.</param>
	/// <returns><paramref name="v_out"/></returns>
	floatN_t vecDiv3f(float3_t v_out, float3_t const v_lh, float3_t const v_rh);

	/// <summary>
	/// Calculate component-wise modulo of input vectors (left-hand over right-hand).
	/// </summary>
	/// <param name="v_out">Result vector.</param>
	/// <param name="v_lh">Left-hand input vector.</param>
	/// <param name="v_rh">Right-hand input vector.</param>
	/// <returns><paramref name="v_out"/></returns>
	floatN_t vecMod3f(float3_t v_out, float3_t const v_lh, float3_t const v_rh);

	/// <summary>
	/// Calculate component-wise quotient and modulo of input vectors (left-hand over right-hand).
	/// </summary>
	/// <param name="v_out">Result vector (quotient).</param>
	/// <param name="v_mod_out">Result vector (modulo).</param>
	/// <param name="v_lh">Left-hand input vector.</param>
	/// <param name="v_rh">Right-hand input vector.</param>
	/// <returns><paramref name="v_out"/></returns>
	floatN_t vecDivMod3f(float3_t v_out, float3_t v_mod_out, float3_t const v_lh, float3_t const v_rh);

	/// <summary>
	/// Calculate component-wise sum of input vector and scalar (left-hand times right-hand).
	/// </summary>
	/// <param name="v_out">Result vector.</param>
	/// <param name="v_lh">Left-hand input vector.</param>
	/// <param name="s_rh">Right-hand input scalar.</param>
	/// <returns><paramref name="v_out"/></returns>
	floatN_t vecAddS3f(float3_t v_out, float3_t const v_lh, vecf_t const s_rh);

	/// <summary>
	/// Calculate component-wise difference of input vector and scalar (left-hand times right-hand).
	/// </summary>
	/// <param name="v_out">Result vector.</param>
	/// <param name="v_lh">Left-hand input vector.</param>
	/// <param name="s_rh">Right-hand input scalar.</param>
	/// <returns><paramref name="v_out"/></returns>
	floatN_t vecSubS3f(float3_t v_out, float3_t const v_lh, vecf_t const s_rh);

	/// <summary>
	/// Calculate product of input vector and scalar (left-hand times right-hand).
	/// </summary>
	/// <param name="v_out">Result vector.</param>
	/// <param name="v_lh">Left-hand input vector.</param>
	/// <param name="s_rh">Right-hand input scalar.</param>
	/// <returns><paramref name="v_out"/></returns>
	floatN_t vecMulS3f(float3_t v_out, float3_t const v_lh, vecf_t const s_rh);

	/// <summary>
	/// Calculate quotient of input vector and scalar (left-hand over right-hand).
	/// </summary>
	/// <param name="v_out">Result vector.</param>
	/// <param name="v_lh">Left-hand input vector.</param>
	/// <param name="s_rh">Right-hand input scalar.</param>
	/// <returns><paramref name="v_out"/></returns>
	floatN_t vecDivS3f(float3_t v_out, float3_t const v_lh, vecf_t const s_rh);

	/// <summary>
	/// Calculate modulo of input vector and scalar (left-hand over right-hand).
	/// </summary>
	/// <param name="v_out">Result vector.</param>
	/// <param name="v_lh">Left-hand input vector.</param>
	/// <param name="s_rh">Right-hand input scalar.</param>
	/// <returns><paramref name="v_out"/></returns>
	floatN_t vecModS3f(float3_t v_out, float3_t const v_lh, vecf_t const s_rh);

	/// <summary>
	/// Calculate quotient and modulo of input vector and scalar (left-hand over right-hand).
	/// </summary>
	/// <param name="v_out">Result vector (quotient).</param>
	/// <param name="v_mod_out">Result vector (modulo).</param>
	/// <param name="v_lh">Left-hand input vector.</param>
	/// <param name="s_rh">Right-hand input scalar.</param>
	/// <returns><paramref name="v_out"/></returns>
	floatN_t vecDivModS3f(float3_t v_out, float3_t v_mod_out, float3_t const v_lh, vecf_t const s_rh);

	/// <summary>
	/// Calculate dot product of input vectors (the scalar sum of component products).
	/// </summary>
	/// <param name="v_lh">Left-hand input vector.</param>
	/// <param name="v_rh">Right-hand input vector.</param>
	/// <returns>Dot product of inputs.</returns>
	vecf_t vecDot3f(float3_t const v_lh, float3_t const v_rh);

	/// <summary>
	/// Calculate cross product of input vectors.
	/// </summary>
	/// <param name="v_out">Result vector.</param>
	/// <param name="v_lh">Left-hand input vector.</param>
	/// <param name="v_rh">Right-hand input vector.</param>
	/// <returns><paramref name="v_out"/></returns>
	floatN_t vecCross3f(float3_t v_out, float3_t const v_lh, float3_t const v_rh);

	/// <summary>
	/// Calculate squared length or magnitude of vector.
	/// </summary>
	/// <param name="v">Input vector.</param>
	/// <returns>Squared length of input.</returns>
	vecf_t vecLenSq3f(float3_t const v);

	/// <summary>
	/// Calculate the squared distance between two vectors.
	/// </summary>
	/// <param name="v_lh">Left-hand input vector.</param>
	/// <param name="v_rh">Right-hand input vector.</param>
	/// <returns>Squared distance between inputs.</returns>
	vecf_t vecDistSq3f(float3_t const v_lh, float3_t const v_rh);

	/// <summary>
	/// Calculate the squared distance between two vectors and store the displacement.
	/// </summary>
	/// <param name="v_disp_out">Result vector (displacement).</param>
	/// <param name="v_lh">Left-hand input vector.</param>
	/// <param name="v_rh">Right-hand input vector.</param>
	/// <returns>Squared distance between inputs.</returns>
	vecf_t vecDispDistSq3f(float3_t v_disp_out, float3_t const v_lh, float3_t const v_rh);

	/// <summary>
	/// Determine if input is zero vector.
	/// </summary>
	/// <param name="v">Input vector.</param>
	/// <returns>True if input is considered zero vector.</returns>
	vecb_t vecIsZero3f(float3_t const v);

	/// <summary>
	/// Determine if input is not zero vector.
	/// </summary>
	/// <param name="v">Input vector.</param>
	/// <returns>True if input is not considered zero vector.</returns>
	vecb_t vecIsNonZero3f(float3_t const v);

	/// <summary>
	/// Calculate component-wise multiply-add of origin vector, delta vector and scaling parameter.
	/// </summary>
	/// <param name="v_out">Result vector.</param>
	/// <param name="u">Scaling parameter applied to delta.</param>
	/// <param name="v_origin">Origin vector (value at u=0).</param>
	/// <param name="v_delta">Delta vector.</param>
	/// <returns><paramref name="v_out"/></returns>
	floatN_t vecMad3f(float3_t v_out, vecf_t const u, float3_t const v_origin, float3_t const v_delta);

	/// <summary>
	/// Calculate component-wise linear interprolation between origin and target given parameter.
	/// </summary>
	/// <param name="v_out">Result vector.</param>
	/// <param name="u">Interpolation parameter in [0,1] for interpolation, otherwise extrapolation.</param>
	/// <param name="v_min">Minimum (origin) vector for interpolation interval (value at u=0).</param>
	/// <param name="v_max">Maximum (target) vector for interpolation interval (value at u=1).</param>
	/// <returns><paramref name="v_out"/></returns>
	floatN_t vecLerp3f(float3_t v_out, vecf_t const u, float3_t const v_min, float3_t const v_max);

	/// <summary>
	/// Calculate the length or magnitude of the vector (analogous to scalar absolute value).
	/// </summary>
	/// <param name="v">Input vector.</param>
	/// <returns>Length of vector.</returns>
	vecf_t vecLen3f(float3_t const v);

	/// <summary>
	/// Calculate the inverse (reciprocal) length or magnitude of the vector.
	/// </summary>
	/// <param name="v">Input vector.</param>
	/// <returns>Inverse length of vector.</returns>
	vecf_t vecLenInv3f(float3_t const v);

	/// <summary>
	/// Calculate the distance between two vectors.
	/// </summary>
	/// <param name="v_lh">Left-hand input vector.</param>
	/// <param name="v_rh">Right-hand input vector.</param>
	/// <returns>Distance between inputs.</returns>
	vecf_t vecDist3f(float3_t const v_lh, float3_t const v_rh);

	/// <summary>
	/// Calculate the distance between two vectors and store the displacement.
	/// </summary>
	/// <param name="v_disp_out">Result vector (displacement).</param>
	/// <param name="v_lh">Left-hand input vector.</param>
	/// <param name="v_rh">Right-hand input vector.</param>
	/// <returns>Distance between inputs.</returns>
	vecf_t vecDispDist3f(float3_t v_disp_out, float3_t const v_lh, float3_t const v_rh);

	/// <summary>
	/// Calculate the unit-length direction of the vector (analogous to scalar sign).
	/// </summary>
	/// <param name="v_out">Result vector (unit direction).</param>
	/// <param name="v">Input vector.</param>
	/// <returns>Length of input vector.</returns>
	vecf_t vecNormalize3f(float3_t v_out, float3_t const v);

	/// <summary>
	/// Calculate a resized vector in the same direction as the input.
	/// </summary>
	/// <param name="v_out">Result vector (same direction, new length).</param>
	/// <param name="v">Input vector.</param>
	/// <param name="newLen">Desired length of vector.</param>
	/// <returns>Length of input vector.</returns>
	vecf_t vecResize3f(float3_t v_out, float3_t const v, vecf_t const newLen);

	/// <summary>
	/// Determine if vector is unit length (normalized).
	/// </summary>
	/// <param name="v">Input vector.</param>
	/// <returns>True if vector is considered unit length.</returns>
	vecb_t vecIsUnit3f(float3_t const v);

	/// <summary>
	/// Determine if vector is non-unit length (normalized).
	/// </summary>
	/// <param name="v">Input vector.</param>
	/// <returns>True if vector is not considered unit length.</returns>
	vecb_t vecIsNonUnit3f(float3_t const v);

	/// <summary>
	/// Calculate projection factor of base vector required to project input onto base.
	/// </summary>
	/// <param name="v">Input vector.</param>
	/// <param name="v_base">Base vector (direction of projected vector).</param>
	/// <returns>Factor of base for projection.</returns>
	vecf_t vecProjS3f(float3_t const v, float3_t const v_base);

	/// <summary>
	/// Calculate projection of input vector onto base.
	/// </summary>
	/// <param name="v_out">Result vector (input projected onto base).</param>
	/// <param name="v">Input vector.</param>
	/// <param name="v_base">Base vector (direction of projected vector).</param>
	/// <returns>Factor of base for projection.</returns>
	vecf_t vecProj3f(float3_t v_out, float3_t const v, float3_t const v_base);

	/// <summary>
	/// Calculate interpolation parameter given potential interpolation result and ends of range.
	/// </summary>
	/// <param name="v"></param>
	/// <param name="v_min"></param>
	/// <param name="v_max"></param>
	/// <returns></returns>
	vecf_t vecLerpInv3f(float3_t const v, float3_t const v_min, float3_t const v_max);

	/// <summary>
	/// Calculate orthogonalized vector given input and base using Gram-Schmidt process.
	/// </summary>
	/// <param name="v_out">Result vector (input orthogonalized relative to base).</param>
	/// <param name="v">Input vector.</param>
	/// <param name="v_base">Base vector.</param>
	/// <returns>Factor of base for projection.</returns>
	vecf_t vecOrtho3f(float3_t v_out, float3_t const v, float3_t const v_base);

	/// <summary>
	/// Calculate orthogonized basis given two inputs and base using Gram-Schmidt process.
	/// </summary>
	/// <param name="v2_out">Second result vector (second input orthogonalized relative to base and first input).</param>
	/// <param name="v1_out">First result vector (first input orthogonalized relative to base).</param>
	/// <param name="v2_basefactor_out_opt">Factor of base for projection of second input (optional).</param>
	/// <param name="v1_basefactor_out_opt">Factor of base for projection of first input (optional).</param>
	/// <param name="v2">Second input vector.</param>
	/// <param name="v1">First input vector.</param>
	/// <param name="v_base">Base vector.</param>
	/// <returns>Factor of first input for projection.</returns>
	vecf_t vecOrthoBasis3f(float3_t v2_out, float3_t v1_out, vecf_t* v2_basefactor_out_opt, vecf_t* v1_basefactor_out_opt, float3_t const v2, float3_t const v1, float3_t const v_base);


#ifdef __cplusplus
}
#endif // #ifdef __cplusplus


#endif // #if (!(defined _CDRAW_VEC3F_H_) && (defined _CDRAW_VECTOR_H_))