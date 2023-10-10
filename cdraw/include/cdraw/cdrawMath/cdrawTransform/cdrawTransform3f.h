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
* cdrawTransform3f.h
* Declaration of single-precision 3D transform functions.
*/

#if (!(defined _CDRAW_TRANSFORM3F_H_) && (defined _CDRAW_TRANSFORM_H_))
#define _CDRAW_TRANSFORM3F_H_


#ifdef __cplusplus
extern "C" {
#endif // #ifdef __cplusplus

	/// <summary>
	/// Initialize rotation matrix to identity.
	/// </summary>
	/// <param name="R_out">Result matrix.</param>
	/// <returns><paramref name="R_out"/></returns>
	floatNx3_t RmatID3f(Rmat3f_t R_out);

	/// <summary>
	/// Initialize rotation matrix by encoding angles in XYZ order.
	/// </summary>
	/// <param name="R_out">Result matrix.</param>
	/// <param name="anglesXYZ_actual_out_opt">Result actual angles used for encoding (optional).</param>
	/// <param name="anglesXYZ">Desired angles for encoding.</param>
	/// <returns><paramref name="R_out"/></returns>
	floatNx3_t RmatFromAnglesXYZ3f(Rmat3f_t R_out, float3_t anglesXYZ_actual_out_opt, float3_t const anglesXYZ);

	/// <summary>
	/// Decode rotation matrix encoded in XYZ order to extract angles.
	/// </summary>
	/// <param name="anglesXYZ_actual_out">Result actual encoded angles.</param>
	/// <param name="R">Input matrix.</param>
	/// <returns><paramref name="anglesXYZ_actual_out"/></returns>
	floatN_t RmatToAnglesXYZ3f(float3_t anglesXYZ_actual_out, Rmat3f_t const R);

	/// <summary>
	/// Initialize rotation matrix by encoding angles in YZX order.
	/// </summary>
	/// <param name="R_out">Result matrix.</param>
	/// <param name="anglesXYZ_actual_out_opt">Result actual angles used for encoding (optional).</param>
	/// <param name="anglesXYZ">Desired angles for encoding.</param>
	/// <returns><paramref name="R_out"/></returns>
	floatNx3_t RmatFromAnglesYZX3f(Rmat3f_t R_out, float3_t anglesXYZ_actual_out_opt, float3_t const anglesXYZ);

	/// <summary>
	/// Decode rotation matrix encoded in YZX order to extract angles.
	/// </summary>
	/// <param name="anglesXYZ_actual_out">Result actual encoded angles.</param>
	/// <param name="R">Input matrix.</param>
	/// <returns><paramref name="anglesXYZ_actual_out"/></returns>
	floatN_t RmatToAnglesYZX3f(float3_t anglesXYZ_actual_out, Rmat3f_t const R);

	/// <summary>
	/// Initialize rotation matrix by encoding angles in ZXY order.
	/// </summary>
	/// <param name="R_out">Result matrix.</param>
	/// <param name="anglesXYZ_actual_out_opt">Result actual angles used for encoding (optional).</param>
	/// <param name="anglesXYZ">Desired angles for encoding.</param>
	/// <returns><paramref name="R_out"/></returns>
	floatNx3_t RmatFromAnglesZXY3f(Rmat3f_t R_out, float3_t anglesXYZ_actual_out_opt, float3_t const anglesXYZ);

	/// <summary>
	/// Decode rotation matrix encoded in ZXY order to extract angles.
	/// </summary>
	/// <param name="anglesXYZ_actual_out">Result actual encoded angles.</param>
	/// <param name="R">Input matrix.</param>
	/// <returns><paramref name="anglesXYZ_actual_out"/></returns>
	floatN_t RmatToAnglesZXY3f(float3_t anglesXYZ_actual_out, Rmat3f_t const R);

	/// <summary>
	/// Initialize rotation matrix by encoding angles in YXZ order.
	/// </summary>
	/// <param name="R_out">Result matrix.</param>
	/// <param name="anglesXYZ_actual_out_opt">Result actual angles used for encoding (optional).</param>
	/// <param name="anglesXYZ">Desired angles for encoding.</param>
	/// <returns><paramref name="R_out"/></returns>
	floatNx3_t RmatFromAnglesYXZ3f(Rmat3f_t R_out, float3_t anglesXYZ_actual_out_opt, float3_t const anglesXYZ);

	/// <summary>
	/// Decode rotation matrix encoded in YXZ order to extract angles.
	/// </summary>
	/// <param name="anglesXYZ_actual_out">Result actual encoded angles.</param>
	/// <param name="R">Input matrix.</param>
	/// <returns><paramref name="anglesXYZ_actual_out"/></returns>
	floatN_t RmatToAnglesYXZ3f(float3_t anglesXYZ_actual_out, Rmat3f_t const R);

	/// <summary>
	/// Initialize rotation matrix by encoding angles in XZY order.
	/// </summary>
	/// <param name="R_out">Result matrix.</param>
	/// <param name="anglesXYZ_actual_out_opt">Result actual angles used for encoding (optional).</param>
	/// <param name="anglesXYZ">Desired angles for encoding.</param>
	/// <returns><paramref name="R_out"/></returns>
	floatNx3_t RmatFromAnglesXZY3f(Rmat3f_t R_out, float3_t anglesXYZ_actual_out_opt, float3_t const anglesXYZ);

	/// <summary>
	/// Decode rotation matrix encoded in XZY order to extract angles.
	/// </summary>
	/// <param name="anglesXYZ_actual_out">Result actual encoded angles.</param>
	/// <param name="R">Input matrix.</param>
	/// <returns><paramref name="anglesXYZ_actual_out"/></returns>
	floatN_t RmatToAnglesXZY3f(float3_t anglesXYZ_actual_out, Rmat3f_t const R);

	/// <summary>
	/// Initialize rotation matrix by encoding angles in ZYX order.
	/// </summary>
	/// <param name="R_out">Result matrix.</param>
	/// <param name="anglesXYZ_actual_out_opt">Result actual angles used for encoding (optional).</param>
	/// <param name="anglesXYZ">Desired angles for encoding.</param>
	/// <returns><paramref name="R_out"/></returns>
	floatNx3_t RmatFromAnglesZYX3f(Rmat3f_t R_out, float3_t anglesXYZ_actual_out_opt, float3_t const anglesXYZ);

	/// <summary>
	/// Decode rotation matrix encoded in ZYX order to extract angles.
	/// </summary>
	/// <param name="anglesXYZ_actual_out">Result actual encoded angles.</param>
	/// <param name="R">Input matrix.</param>
	/// <returns><paramref name="anglesXYZ_actual_out"/></returns>
	floatN_t RmatToAnglesZYX3f(float3_t anglesXYZ_actual_out, Rmat3f_t const R);

	/// <summary>
	/// Initialize rotation matrix by encoding angles in specified order.
	/// </summary>
	/// <param name="R_out">Result matrix.</param>
	/// <param name="anglesXYZ_actual_out_opt">Result actual angles used for encoding (optional).</param>
	/// <param name="anglesXYZ">Desired angles for encoding.</param>
	/// <param name="encodeOrder">Specified order used to encode matrix.</param>
	/// <returns><paramref name="R_out"/></returns>
	floatNx3_t RmatFromAngles3f(Rmat3f_t R_out, float3_t anglesXYZ_actual_out_opt, float3_t const anglesXYZ, RotateAxisOrder_t const encodeOrder);

	/// <summary>
	/// Decode rotation matrix encoded in specified order to extract angles.
	/// </summary>
	/// <param name="anglesXYZ_actual_out">Result actual encoded angles.</param>
	/// <param name="R">Input matrix.</param>
	/// <param name="encodeOrder">Specified order used to encode matrix.</param>
	/// <returns><paramref name="anglesXYZ_actual_out"/></returns>
	floatN_t RmatToAngles3f(float3_t anglesXYZ_actual_out, Rmat3f_t const R, RotateAxisOrder_t const encodeOrder);


#ifdef __cplusplus
}
#endif // #ifdef __cplusplus


#endif // #if (!(defined _CDRAW_TRANSFORM3F_H_) && (defined _CDRAW_TRANSFORM_H_))