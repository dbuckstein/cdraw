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

	/// <summary>
	/// Copy the forward axis from the input basis given reference frame.
	/// </summary>
	/// <param name="v_out">Result forward axis vector.</param>
	/// <param name="R">Input basis matrix.</param>
	/// <param name="ref">Desired reference frame.</param>
	/// <returns><paramref name="v_out"/></returns>
	floatN_t RmatForward3f(float3_t v_out, Rmat3f_t const R, ReferenceFrame_t const ref);

	/// <summary>
	/// Copy the backward axis from the input basis given reference frame.
	/// </summary>
	/// <param name="v_out">Result backward axis vector.</param>
	/// <param name="R">Input basis matrix.</param>
	/// <param name="ref">Desired reference frame.</param>
	/// <returns><paramref name="v_out"/></returns>
	floatN_t RmatBackward3f(float3_t v_out, Rmat3f_t const R, ReferenceFrame_t const ref);

	/// <summary>
	/// Copy the right axis from the input basis given reference frame.
	/// </summary>
	/// <param name="v_out">Result right axis vector.</param>
	/// <param name="R">Input basis matrix.</param>
	/// <param name="ref">Desired reference frame.</param>
	/// <returns><paramref name="v_out"/></returns>
	floatN_t RmatRight3f(float3_t v_out, Rmat3f_t const R, ReferenceFrame_t const ref);

	/// <summary>
	/// Copy the left axis from the input basis given reference frame.
	/// </summary>
	/// <param name="v_out">Result left axis vector.</param>
	/// <param name="R">Input basis matrix.</param>
	/// <param name="ref">Desired reference frame.</param>
	/// <returns><paramref name="v_out"/></returns>
	floatN_t RmatLeft3f(float3_t v_out, Rmat3f_t const R, ReferenceFrame_t const ref);

	/// <summary>
	/// Copy the down axis from the input basis given reference frame.
	/// </summary>
	/// <param name="v_out">Result down axis vector.</param>
	/// <param name="R">Input basis matrix.</param>
	/// <param name="ref">Desired reference frame.</param>
	/// <returns><paramref name="v_out"/></returns>
	floatN_t RmatDown3f(float3_t v_out, Rmat3f_t const R, ReferenceFrame_t const ref);

	/// <summary>
	/// Copy the up axis from the input basis given reference frame.
	/// </summary>
	/// <param name="v_out">Result up axis vector.</param>
	/// <param name="R">Input basis matrix.</param>
	/// <param name="ref">Desired reference frame.</param>
	/// <returns><paramref name="v_out"/></returns>
	floatN_t RmatUp3f(float3_t v_out, Rmat3f_t const R, ReferenceFrame_t const ref);

	/// <summary>
	/// Copy the reference frame's absolute forward direction.
	/// </summary>
	/// <param name="v_out">Result forward axis vector.</param>
	/// <param name="ref">Desired reference frame.</param>
	/// <returns><paramref name="v_out"/></returns>
	floatN_t RmatRefForward3f(float3_t v_out, ReferenceFrame_t const ref);

	/// <summary>
	/// Copy the reference frame's absolute backward direction.
	/// </summary>
	/// <param name="v_out">Result backward axis vector.</param>
	/// <param name="ref">Desired reference frame.</param>
	/// <returns><paramref name="v_out"/></returns>
	floatN_t RmatRefBackward3f(float3_t v_out, ReferenceFrame_t const ref);

	/// <summary>
	/// Copy the reference frame's absolute right direction.
	/// </summary>
	/// <param name="v_out">Result right axis vector.</param>
	/// <param name="ref">Desired reference frame.</param>
	/// <returns><paramref name="v_out"/></returns>
	floatN_t RmatRefRight3f(float3_t v_out, ReferenceFrame_t const ref);

	/// <summary>
	/// Copy the reference frame's absolute left direction.
	/// </summary>
	/// <param name="v_out">Result left axis vector.</param>
	/// <param name="ref">Desired reference frame.</param>
	/// <returns><paramref name="v_out"/></returns>
	floatN_t RmatRefLeft3f(float3_t v_out, ReferenceFrame_t const ref);

	/// <summary>
	/// Copy the reference frame's absolute up direction.
	/// </summary>
	/// <param name="v_out">Result up axis vector.</param>
	/// <param name="ref">Desired reference frame.</param>
	/// <returns><paramref name="v_out"/></returns>
	floatN_t RmatRefUp3f(float3_t v_out, ReferenceFrame_t const ref);

	/// <summary>
	/// Copy the reference frame's absolute down direction.
	/// </summary>
	/// <param name="v_out">Result down axis vector.</param>
	/// <param name="ref">Desired reference frame.</param>
	/// <returns><paramref name="v_out"/></returns>
	floatN_t RmatRefDown3f(float3_t v_out, ReferenceFrame_t const ref);

	/// <summary>
	/// Calculate a look-at matrix given a center and target in reference frame.
	/// Center to target is absolute forward, uses reference frame's absolute yaw axis as basis.
	/// </summary>
	/// <param name="R_out">Result rotation matrix.</param>
	/// <param name="v_center">Center of reference frame.</param>
	/// <param name="v_target">Target in reference frame</param>
	/// <param name="ref">Desired reference frame.</param>
	/// <returns><paramref name="R_out"/></returns>
	floatNx3_t RmatLookAt3f(Rmat3f_t R_out, float3_t const v_center, float3_t const v_target, ReferenceFrame_t const ref);

	/// <summary>
	/// Convert absolute axis representation (FRD/RPY) to relative axis representation (XYZ) given reference frame.
	/// </summary>
	/// <param name="v_rel_out">Result relative vector.</param>
	/// <param name="v_abs">Input absolute vector.</param>
	/// <param name="ref">Desired reference frame.</param>
	/// <returns><paramref name="v_rel_out"/></returns>
	floatN_t vecAbsToRel3f(float3_t v_rel_out, float3_t const v_abs, ReferenceFrame_t const ref);

	/// <summary>
	/// Convert relative axis representation (XYZ) to absolute axis representation (FRD/RPY) given reference frame.
	/// </summary>
	/// <param name="v_abs_out">Result absolute vector.</param>
	/// <param name="v_rel">Input relative vector.</param>
	/// <param name="ref">Desired reference frame.</param>
	/// <returns><paramref name="v_abs_out"/></returns>
	floatN_t vecRelToAbs3f(float3_t v_abs_out, float3_t const v_rel, ReferenceFrame_t const ref);

	/// <summary>
	/// Get roll component of relative axis in absolute terms.
	/// </summary>
	/// <param name="v_rel">Input relative axis.</param>
	/// <param name="ref">Desired reference frame.</param>
	/// <returns>Roll component of relative input vector.</returns>
	vecf_t vecAbsRoll3f(float3_t const v_rel, ReferenceFrame_t const ref);

	/// <summary>
	/// Get pitch component of relative axis in absolute terms.
	/// </summary>
	/// <param name="v_rel">Input relative axis.</param>
	/// <param name="ref">Desired reference frame.</param>
	/// <returns>Pitch component of relative input vector.</returns>
	vecf_t vecAbsPitch3f(float3_t const v_rel, ReferenceFrame_t const ref);
	
	/// <summary>
	/// Get yaw component of relative axis in absolute terms.
	/// </summary>
	/// <param name="v_rel">Input relative axis.</param>
	/// <param name="ref">Desired reference frame.</param>
	/// <returns>Yaw component of relative input vector.</returns>
	vecf_t vecAbsYaw3f(float3_t const v_rel, ReferenceFrame_t const ref);

	/// <summary>
	/// Set absolute axis values to relative axis base.
	/// </summary>
	/// <param name="v_rel_out">Result relative vector.</param>
	/// <param name="x_roll">Absolute roll axis value.</param>
	/// <param name="x_pitch">Absolute pitch axis value.</param>
	/// <param name="x_yaw">Absolute yaw axis value.</param>
	/// <param name="ref">Desired reference frame.</param>
	/// <returns><paramref name="v_rel_out"/></returns>
	floatN_t vecRelSetAbs3f(float3_t v_rel_out, vecf_t const x_roll, vecf_t const x_pitch, vecf_t const x_yaw, ReferenceFrame_t const ref);

	/// <summary>
	/// Add absolute axis deltas to relative axis base.
	/// </summary>
	/// <param name="v_rel_out">Result relative vector.</param>
	/// <param name="v_rel">Input relative vector.</param>
	/// <param name="d_roll">Absolute roll axis delta.</param>
	/// <param name="d_pitch">Absolute pitch axis delta.</param>
	/// <param name="d_yaw">Absolute yaw axis delta.</param>
	/// <param name="ref">Desired reference frame.</param>
	/// <returns><paramref name="v_rel_out"/></returns>
	floatN_t vecRelAddAbs3f(float3_t v_rel_out, float3_t const v_rel, vecf_t const d_roll, vecf_t const d_pitch, vecf_t const d_yaw, ReferenceFrame_t const ref);

	/// <summary>
	/// Multiply absolute axis scalars by relative axis base.
	/// </summary>
	/// <param name="v_rel_out">Result relative vector.</param>
	/// <param name="v_rel">Input relative vector.</param>
	/// <param name="s_roll">Absolute roll axis scalar.</param>
	/// <param name="s_pitch">Absolute pitch axis scalar.</param>
	/// <param name="s_yaw">Absolute yaw axis scalar.</param>
	/// <param name="ref">Desired reference frame.</param>
	/// <returns><paramref name="v_rel_out"/></returns>
	floatN_t vecRelScaleAbs3f(float3_t v_rel_out, float3_t const v_rel, vecf_t const s_roll, vecf_t const s_pitch, vecf_t const s_yaw, ReferenceFrame_t const ref);


#ifdef __cplusplus
}
#endif // #ifdef __cplusplus


#endif // #if (!(defined _CDRAW_TRANSFORM3F_H_) && (defined _CDRAW_TRANSFORM_H_))