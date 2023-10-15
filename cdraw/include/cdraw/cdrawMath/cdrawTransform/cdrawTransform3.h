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
* cdrawTransform3.h
* Declaration of preferred floating point precision 3D transform functions.
*/

#if (!(defined _CDRAW_TRANSFORM3_H_) && (defined _CDRAW_TRANSFORM_H_))
#define _CDRAW_TRANSFORM3_H_


#ifdef __cplusplus
extern "C" {
#endif // #ifdef __cplusplus

	/// <summary>
	/// Initialize rotation matrix to identity.
	/// </summary>
	/// <param name="R_out">Result matrix.</param>
	/// <returns><paramref name="R_out"/></returns>
#define RmatID3 cdraw_transform_base(RmatID3)

	/// <summary>
	/// Calculate the inverse of a rotation matrix (transpose).
	/// </summary>
	/// <param name="R_out">Result matrix (inverse).</param>
	/// <param name="R">Input matrix.</param>
	/// <returns><paramref name="R_out"/></returns>
#define RmatInv3 cdraw_transform_base(RmatInv3)

	/// <summary>
	/// Initialize rotation matrix by encoding angles in XYZ order.
	/// </summary>
	/// <param name="R_out">Result matrix.</param>
	/// <param name="anglesXYZ_actual_out_opt">Result actual angles used for encoding (optional).</param>
	/// <param name="anglesXYZ">Desired angles for encoding.</param>
	/// <returns><paramref name="R_out"/></returns>
#define RmatFromAnglesXYZ3 cdraw_transform_base(RmatFromAnglesXYZ3)

	/// <summary>
	/// Decode rotation matrix encoded in XYZ order to extract angles.
	/// </summary>
	/// <param name="anglesXYZ_actual_out">Result actual encoded angles.</param>
	/// <param name="R">Input matrix.</param>
	/// <returns><paramref name="anglesXYZ_actual_out"/></returns>
#define RmatToAnglesXYZ3 cdraw_transform_base(RmatToAnglesXYZ3)

	/// <summary>
	/// Initialize rotation matrix by encoding angles in YZX order.
	/// </summary>
	/// <param name="R_out">Result matrix.</param>
	/// <param name="anglesXYZ_actual_out_opt">Result actual angles used for encoding (optional).</param>
	/// <param name="anglesXYZ">Desired angles for encoding.</param>
	/// <returns><paramref name="R_out"/></returns>
#define RmatFromAnglesYZX3 cdraw_transform_base(RmatFromAnglesYZX3)

	/// <summary>
	/// Decode rotation matrix encoded in YZX order to extract angles.
	/// </summary>
	/// <param name="anglesXYZ_actual_out">Result actual encoded angles.</param>
	/// <param name="R">Input matrix.</param>
	/// <returns><paramref name="anglesXYZ_actual_out"/></returns>
#define RmatToAnglesYZX3 cdraw_transform_base(RmatToAnglesYZX3)

	/// <summary>
	/// Initialize rotation matrix by encoding angles in ZXY order.
	/// </summary>
	/// <param name="R_out">Result matrix.</param>
	/// <param name="anglesXYZ_actual_out_opt">Result actual angles used for encoding (optional).</param>
	/// <param name="anglesXYZ">Desired angles for encoding.</param>
	/// <returns><paramref name="R_out"/></returns>
#define RmatFromAnglesZXY3 cdraw_transform_base(RmatFromAnglesZXY3)

	/// <summary>
	/// Decode rotation matrix encoded in ZXY order to extract angles.
	/// </summary>
	/// <param name="anglesXYZ_actual_out">Result actual encoded angles.</param>
	/// <param name="R">Input matrix.</param>
	/// <returns><paramref name="anglesXYZ_actual_out"/></returns>
#define RmatToAnglesZXY3 cdraw_transform_base(RmatToAnglesZXY3)

	/// <summary>
	/// Initialize rotation matrix by encoding angles in YXZ order.
	/// </summary>
	/// <param name="R_out">Result matrix.</param>
	/// <param name="anglesXYZ_actual_out_opt">Result actual angles used for encoding (optional).</param>
	/// <param name="anglesXYZ">Desired angles for encoding.</param>
	/// <returns><paramref name="R_out"/></returns>
#define RmatFromAnglesYXZ3 cdraw_transform_base(RmatFromAnglesYXZ3)

	/// <summary>
	/// Decode rotation matrix encoded in YXZ order to extract angles.
	/// </summary>
	/// <param name="anglesXYZ_actual_out">Result actual encoded angles.</param>
	/// <param name="R">Input matrix.</param>
	/// <returns><paramref name="anglesXYZ_actual_out"/></returns>
#define RmatToAnglesYXZ3 cdraw_transform_base(RmatToAnglesYXZ3)

	/// <summary>
	/// Initialize rotation matrix by encoding angles in XZY order.
	/// </summary>
	/// <param name="R_out">Result matrix.</param>
	/// <param name="anglesXYZ_actual_out_opt">Result actual angles used for encoding (optional).</param>
	/// <param name="anglesXYZ">Desired angles for encoding.</param>
	/// <returns><paramref name="R_out"/></returns>
#define RmatFromAnglesXZY3 cdraw_transform_base(RmatFromAnglesXZY3)

	/// <summary>
	/// Decode rotation matrix encoded in XZY order to extract angles.
	/// </summary>
	/// <param name="anglesXYZ_actual_out">Result actual encoded angles.</param>
	/// <param name="R">Input matrix.</param>
	/// <returns><paramref name="anglesXYZ_actual_out"/></returns>
#define RmatToAnglesXZY3 cdraw_transform_base(RmatToAnglesXZY3)

	/// <summary>
	/// Initialize rotation matrix by encoding angles in ZYX order.
	/// </summary>
	/// <param name="R_out">Result matrix.</param>
	/// <param name="anglesXYZ_actual_out_opt">Result actual angles used for encoding (optional).</param>
	/// <param name="anglesXYZ">Desired angles for encoding.</param>
	/// <returns><paramref name="R_out"/></returns>
#define RmatFromAnglesZYX3 cdraw_transform_base(RmatFromAnglesZYX3)

	/// <summary>
	/// Decode rotation matrix encoded in ZYX order to extract angles.
	/// </summary>
	/// <param name="anglesXYZ_actual_out">Result actual encoded angles.</param>
	/// <param name="R">Input matrix.</param>
	/// <returns><paramref name="anglesXYZ_actual_out"/></returns>
#define RmatToAnglesZYX3 cdraw_transform_base(RmatToAnglesZYX3)

	/// <summary>
	/// Initialize rotation matrix by encoding angles in specified order.
	/// </summary>
	/// <param name="R_out">Result matrix.</param>
	/// <param name="anglesXYZ_actual_out_opt">Result actual angles used for encoding (optional).</param>
	/// <param name="anglesXYZ">Desired angles for encoding.</param>
	/// <param name="encodeOrder">Specified order used to encode matrix.</param>
	/// <returns><paramref name="R_out"/></returns>
#define RmatFromAngles3 cdraw_transform_base(RmatFromAngles3)

	/// <summary>
	/// Decode rotation matrix encoded in specified order to extract angles.
	/// </summary>
	/// <param name="anglesXYZ_actual_out">Result actual encoded angles.</param>
	/// <param name="R">Input matrix.</param>
	/// <param name="encodeOrder">Specified order used to encode matrix.</param>
	/// <returns><paramref name="anglesXYZ_actual_out"/></returns>
#define RmatToAngles3 cdraw_transform_base(RmatToAngles3)

	/// <summary>
	/// Copy the forward axis from the input basis given reference frame.
	/// </summary>
	/// <param name="v_out">Result forward axis vector.</param>
	/// <param name="R">Input basis matrix.</param>
	/// <param name="ref">Desired reference frame.</param>
	/// <returns><paramref name="v_out"/></returns>
#define RmatForward3 cdraw_transform_base(RmatForward3)

	/// <summary>
	/// Copy the backward axis from the input basis given reference frame.
	/// </summary>
	/// <param name="v_out">Result backward axis vector.</param>
	/// <param name="R">Input basis matrix.</param>
	/// <param name="ref">Desired reference frame.</param>
	/// <returns><paramref name="v_out"/></returns>
#define RmatBackward3 cdraw_transform_base(RmatBackward3)

	/// <summary>
	/// Copy the right axis from the input basis given reference frame.
	/// </summary>
	/// <param name="v_out">Result right axis vector.</param>
	/// <param name="R">Input basis matrix.</param>
	/// <param name="ref">Desired reference frame.</param>
	/// <returns><paramref name="v_out"/></returns>
#define RmatRight3 cdraw_transform_base(RmatRight3)

	/// <summary>
	/// Copy the left axis from the input basis given reference frame.
	/// </summary>
	/// <param name="v_out">Result left axis vector.</param>
	/// <param name="R">Input basis matrix.</param>
	/// <param name="ref">Desired reference frame.</param>
	/// <returns><paramref name="v_out"/></returns>
#define RmatLeft3 cdraw_transform_base(RmatLeft3)

	/// <summary>
	/// Copy the down axis from the input basis given reference frame.
	/// </summary>
	/// <param name="v_out">Result down axis vector.</param>
	/// <param name="R">Input basis matrix.</param>
	/// <param name="ref">Desired reference frame.</param>
	/// <returns><paramref name="v_out"/></returns>
#define RmatDown3 cdraw_transform_base(RmatDown3)

	/// <summary>
	/// Copy the up axis from the input basis given reference frame.
	/// </summary>
	/// <param name="v_out">Result up axis vector.</param>
	/// <param name="R">Input basis matrix.</param>
	/// <param name="ref">Desired reference frame.</param>
	/// <returns><paramref name="v_out"/></returns>
#define RmatUp3 cdraw_transform_base(RmatUp3)

	/// <summary>
	/// Copy the reference frame's absolute forward direction.
	/// </summary>
	/// <param name="v_out">Result forward axis vector.</param>
	/// <param name="ref">Desired reference frame.</param>
	/// <returns><paramref name="v_out"/></returns>
#define RmatRefForward3 cdraw_transform_base(RmatRefForward3)

	/// <summary>
	/// Copy the reference frame's absolute backward direction.
	/// </summary>
	/// <param name="v_out">Result backward axis vector.</param>
	/// <param name="ref">Desired reference frame.</param>
	/// <returns><paramref name="v_out"/></returns>
#define RmatRefBackward3 cdraw_transform_base(RmatRefBackward3)

	/// <summary>
	/// Copy the reference frame's absolute right direction.
	/// </summary>
	/// <param name="v_out">Result right axis vector.</param>
	/// <param name="ref">Desired reference frame.</param>
	/// <returns><paramref name="v_out"/></returns>
#define RmatRefRight3 cdraw_transform_base(RmatRefRight3)

	/// <summary>
	/// Copy the reference frame's absolute left direction.
	/// </summary>
	/// <param name="v_out">Result left axis vector.</param>
	/// <param name="ref">Desired reference frame.</param>
	/// <returns><paramref name="v_out"/></returns>
#define RmatRefLeft3 cdraw_transform_base(RmatRefLeft3)

	/// <summary>
	/// Copy the reference frame's absolute up direction.
	/// </summary>
	/// <param name="v_out">Result up axis vector.</param>
	/// <param name="ref">Desired reference frame.</param>
	/// <returns><paramref name="v_out"/></returns>
#define RmatRefUp3 cdraw_transform_base(RmatRefUp3)

	/// <summary>
	/// Copy the reference frame's absolute down direction.
	/// </summary>
	/// <param name="v_out">Result down axis vector.</param>
	/// <param name="ref">Desired reference frame.</param>
	/// <returns><paramref name="v_out"/></returns>
#define RmatRefDown3 cdraw_transform_base(RmatRefDown3)

	/// <summary>
	/// Calculate a look-at matrix given a center and target in reference frame.
	/// Center to target is absolute forward, uses reference frame's absolute yaw axis as basis.
	/// </summary>
	/// <param name="R_out">Result rotation matrix.</param>
	/// <param name="v_center">Center of reference frame.</param>
	/// <param name="v_target">Target in reference frame</param>
	/// <param name="ref">Desired reference frame.</param>
	/// <returns><paramref name="R_out"/></returns>
#define RmatLookAt3 cdraw_transform_base(RmatLookAt3)

	/// <summary>
	/// Convert absolute axis representation (FRD/RPY) to relative axis representation (XYZ) given reference frame.
	/// </summary>
	/// <param name="v_rel_out">Result relative vector.</param>
	/// <param name="v_abs">Input absolute vector.</param>
	/// <param name="ref">Desired reference frame.</param>
	/// <returns><paramref name="v_rel_out"/></returns>
#define vecAbsToRel3 cdraw_transform_base(vecAbsToRel3)

	/// <summary>
	/// Convert relative axis representation (XYZ) to absolute axis representation (FRD/RPY) given reference frame.
	/// </summary>
	/// <param name="v_abs_out">Result absolute vector.</param>
	/// <param name="v_rel">Input relative vector.</param>
	/// <param name="ref">Desired reference frame.</param>
	/// <returns><paramref name="v_abs_out"/></returns>
#define vecRelToAbs3 cdraw_transform_base(vecRelToAbs3)

	/// <summary>
	/// Get roll component of relative axis in absolute terms.
	/// </summary>
	/// <param name="v_rel">Input relative axis.</param>
	/// <param name="ref">Desired reference frame.</param>
	/// <returns>Roll component of relative input vector.</returns>
#define vecAbsRoll3 cdraw_transform_base(vecAbsRoll3)

	/// <summary>
	/// Get pitch component of relative axis in absolute terms.
	/// </summary>
	/// <param name="v_rel">Input relative axis.</param>
	/// <param name="ref">Desired reference frame.</param>
	/// <returns>Pitch component of relative input vector.</returns>
#define vecAbsPitch3 cdraw_transform_base(vecAbsPitch3)

	/// <summary>
	/// Get yaw component of relative axis in absolute terms.
	/// </summary>
	/// <param name="v_rel">Input relative axis.</param>
	/// <param name="ref">Desired reference frame.</param>
	/// <returns>Yaw component of relative input vector.</returns>
#define vecAbsYaw3 cdraw_transform_base(vecAbsYaw3)

	/// <summary>
	/// Set absolute axis values to relative axis base.
	/// </summary>
	/// <param name="v_rel_out">Result relative vector.</param>
	/// <param name="x_roll">Absolute roll axis value.</param>
	/// <param name="x_pitch">Absolute pitch axis value.</param>
	/// <param name="x_yaw">Absolute yaw axis value.</param>
	/// <param name="ref">Desired reference frame.</param>
	/// <returns><paramref name="v_rel_out"/></returns>
#define vecRelSetAbs3 cdraw_transform_base(vecRelSetAbs3)

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
#define vecRelAddAbs3 cdraw_transform_base(vecRelAddAbs3)

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
#define vecRelScaleAbs3 cdraw_transform_base(vecRelScaleAbs3)

	/// <summary>
	/// Calculate transformed vector by multiplying transform matrix by input vector; translation ignored.
	/// </summary>
	/// <param name="v_out">Result product vector.</param>
	/// <param name="T_lh">Left-hand input transform matrix.</param>
	/// <param name="v_rh">Right-hand input vector.</param>
	/// <returns><paramref name="v_out"/></returns>
#define TmatMulVec3 cdraw_transform_base(TmatMulVec3)

	/// <summary>
	/// Calculate transformed point by multiplying transform matrix by input point.
	/// </summary>
	/// <param name="p_out">Result product point.</param>
	/// <param name="T_lh">Left-hand input transform matrix.</param>
	/// <param name="p_rh">Right-hand input point.</param>
	/// <returns><paramref name="v_out"/></returns>
#define TmatMulPoint3 cdraw_transform_base(TmatMulPoint3)

	/// <summary>
	/// Calculate optimized product of transform matrices.
	/// </summary>
	/// <param name="T_out">Result product transform matrix.</param>
	/// <param name="T_lh">Left-hand input transform matrix.</param>
	/// <param name="T_rh">Right-hand input transform matrix.</param>
	/// <returns><paramref name="v_out"/></returns>
#define TmatMul3 cdraw_transform_base(TmatMul3)

	/// <summary>
	/// Reset rotation descriptor.
	/// </summary>
	/// <param name="rotate_out">Result rotation descriptor.</param>
	/// <returns><paramref name="rotate_out"/></returns>
#define rotateReset3 cdraw_transform_base(rotateReset3)

	/// <summary>
	/// Update rotation descriptor encoding based on raw components.
	/// </summary>
	/// <param name="rotate_out">Result rotation descriptor.</param>
	/// <param name="ref">Desired reference frame.</param>
	/// <returns><paramref name="rotate_out"/></returns>
#define rotateUpdate3 cdraw_transform_base(rotateUpdate3)

	/// <summary>
	/// Calculate inverted rotation descriptor based on inverted encoded components.
	/// </summary>
	/// <param name="rotate_out">Result rotation descriptor.</param>
	/// <param name="rotate">Input rotation descriptor to invert.</param>
	/// <param name="ref">Desired reference frame.</param>
	/// <returns><paramref name="rotate_out"/></returns>
#define rotateInvert3 cdraw_transform_base(rotateInvert3)

	/// <summary>
	/// Calculate concatenated rotation descriptor based on the concatenated encoded components of two inputs.
	/// </summary>
	/// <param name="rotate_out">Result rotation descriptor.</param>
	/// <param name="rotate_lh">Left-hand input rotation descriptor to concatenate.</param>
	/// <param name="rotate_rh">Right-hand input rotation descriptor to concatenate.</param>
	/// <param name="ref">Desired reference frame.</param>
	/// <returns><paramref name="rotate_out"/></returns>
#define rotateConcat3 cdraw_transform_base(rotateConcat3)

	/// <summary>
	/// Update rotation descriptor raw components based on encoding.
	/// </summary>
	/// <param name="rotate_out">Result rotation descriptor.</param>
	/// <param name="ref">Desired reference frame.</param>
	/// <returns><paramref name="rotate_out"/></returns>
#define rotateUpdateAngles3 cdraw_transform_base(rotateUpdateAngles3)

	/// <summary>
	/// Set rotation descriptor based on absolute angles.
	/// </summary>
	/// <param name="rotate_out">Result rotation descriptor.</param>
	/// <param name="angles">Input absolute angles.</param>
	/// <param name="ref">Desired reference frame.</param>
	/// <returns><paramref name="rotate_out"/></returns>
#define rotateSetAngles3 cdraw_transform_base(rotateSetAngles3)

	/// <summary>
	/// Get absolute angles from rotation descriptor.
	/// </summary>
	/// <param name="angles_out">Result absolute angles.</param>
	/// <param name="rotate">Input rotation descriptor.</param>
	/// <param name="ref">Desired reference frame.</param>
	/// <returns><paramref name="angles_out"/></returns>
#define rotateGetAngles3 cdraw_transform_base(rotateGetAngles3)

	/// <summary>
	/// Calculate inverted rotation descriptor based on inverted raw angles.
	/// </summary>
	/// <param name="rotate_out">Result rotation descriptor.</param>
	/// <param name="rotate">Input rotation descriptor to invert.</param>
	/// <param name="ref">Desired reference frame.</param>
	/// <returns><paramref name="rotate_out"/></returns>
#define rotateInvertAngles3 cdraw_transform_base(rotateInvertAngles3)

	/// <summary>
	/// Calculate concatenated rotation descriptor based on the concatenated raw angles of two inputs.
	/// </summary>
	/// <param name="rotate_out">Result rotation descriptor.</param>
	/// <param name="rotate_lh">Left-hand input rotation descriptor to concatenate.</param>
	/// <param name="rotate_rh">Left-hand input rotation descriptor to concatenate.</param>
	/// <param name="ref">Desired reference frame.</param>
	/// <returns><paramref name="rotate_out"/></returns>
#define rotateConcatAngles3 cdraw_transform_base(rotateConcatAngles3)

	/// <summary>
	/// Reset translation descriptor.
	/// </summary>
	/// <param name="translate_out">Result translation descriptor.</param>
	/// <returns><paramref name="translate_out"/></returns>
#define translateReset3 cdraw_transform_base(translateReset3)

	/// <summary>
	/// Set translation descriptor based on absolute translation axis.
	/// </summary>
	/// <param name="translate_out">Result translation descriptor.</param>
	/// <param name="axis">Input absolute translation axis.</param>
	/// <param name="ref">Desired reference frame.</param>
	/// <returns><paramref name="translate_out"/></returns>
#define translateSetAxis3 cdraw_transform_base(translateSetAxis3)

	/// <summary>
	/// Get absolute translation axis from translation descriptor.
	/// </summary>
	/// <param name="axis_out">Result absolute translation axis.</param>
	/// <param name="translate">Input translation descriptor.</param>
	/// <param name="ref">Desired reference frame.</param>
	/// <returns><paramref name="axis_out"/></returns>
#define translateGetAxis3 cdraw_transform_base(translateGetAxis3)

	/// <summary>
	/// Calculate inverted translation descriptor based on raw axis.
	/// </summary>
	/// <param name="translate_out">Result translation descriptor.</param>
	/// <param name="translate">Input translation descriptor.</param>
	/// <returns><paramref name="translate_out"/></returns>
#define translateInvertAxis3 cdraw_transform_base(translateInvertAxis3)

	/// <summary>
	/// Calculate concatenated translation descriptor based on concatenated raw axes.
	/// </summary>
	/// <param name="translate_out">Result translation descriptor.</param>
	/// <param name="translate_lh">Left-hand input translation descriptor.</param>
	/// <param name="translate_rh">Right-hand input translation descriptor.</param>
	/// <returns><paramref name="translate_out"/></returns>
#define translateConcatAxis3 cdraw_transform_base(translateConcatAxis3)

	/// <summary>
	/// Reset scale descriptor.
	/// </summary>
	/// <param name="scale_out">Result scale descriptor.</param>
	/// <returns><paramref name="scale_out"/></returns>
#define scaleReset3 cdraw_transform_base(scaleReset3)

	/// <summary>
	/// Calculate inverted scale descriptor based on raw axis.
	/// </summary>
	/// <param name="scale_out">Result scale descriptor.</param>
	/// <param name="scale">Input scale descriptor.</param>
	/// <returns><paramref name="scale_out"/></returns>
#define scaleInvertAxis3 cdraw_transform_base(scaleInvertAxis3)

	/// <summary>
	/// Calculate concatenated scale descriptor based on concatenated raw axes.
	/// </summary>
	/// <param name="scale_out">Result scale descriptor.</param>
	/// <param name="scale_lh">Left-hand input scale descriptor.</param>
	/// <param name="scale_rh">Right-hand input scale descriptor.</param>
	/// <returns><paramref name="scale_out"/></returns>
#define scaleConcatAxis3 cdraw_transform_base(scaleConcatAxis3)

	/// <summary>
	/// Reset transformation descriptor.
	/// </summary>
	/// <param name="transform_out">Result transformation descriptor.</param>
	/// <returns><paramref name="transform_out"/></returns>
#define transformReset3 cdraw_transform_base(transformReset3)

	/// <summary>
	/// Update transformation encoded matrix based on updated descriptor components.
	/// </summary>
	/// <param name="transform_out">Result transformation descriptor.</param>
	/// <returns><paramref name="transform_out"/></returns>
#define transformUpdate3 cdraw_transform_base(transformUpdate3)

	/// <summary>
	/// Calculate inverted transformation descriptor based on encoded matrix.
	/// </summary>
	/// <param name="transform_out">Result transformation descriptor.</param>
	/// <param name="transform">Input transformation descriptor.</param>
	/// <param name="ref">Desired reference frame.</param>
	/// <returns><paramref name="transform_out"/></returns>
#define transformInvert3 cdraw_transform_base(transformInvert3)

	/// <summary>
	/// Calculate concatenated transformation descriptor based on encoded matrix.
	/// </summary>
	/// <param name="transform_out">Result transformation descriptor.</param>
	/// <param name="transform_lh">Left-hand input transformation descriptor.</param>
	/// <param name="transform_rh">Right-hand input transformation descriptor.</param>
	/// <param name="ref">Desired reference frame.</param>
	/// <returns><paramref name="transform_out"/></returns>
#define transformConcat3 cdraw_transform_base(transformConcat3)

	/// <summary>
	/// Update transformation descriptor components based on encoded matrix.
	/// </summary>
	/// <param name="transform_out">Result transformation descriptor.</param>
	/// <param name="ref">Desired reference frame.</param>
	/// <returns><paramref name="transform_out"/></returns>
#define transformUpdateComponents3 cdraw_transform_base(transformUpdateComponents3)

	/// <summary>
	/// Calculate inverted transformation descriptor based on updated components.
	/// </summary>
	/// <param name="transform_out">Result transformation descriptor.</param>
	/// <param name="transform">Input transformation descriptor.</param>
	/// <param name="ref">Desired reference frame.</param>
	/// <returns><paramref name="transform_out"/></returns>
#define transformInvertComponents3 cdraw_transform_base(transformInvertComponents3)

	/// <summary>
	/// Calculate concatenated transformation descriptor based on updated components.
	/// </summary>
	/// <param name="transform_out">Result transformation descriptor.</param>
	/// <param name="transform_lh">Left-hand input transformation descriptor.</param>
	/// <param name="transform_rh">Right-hand input transformation descriptor.</param>
	/// <param name="ref">Desired reference frame.</param>
	/// <returns><paramref name="transform_out"/></returns>
#define transformConcatComponents3 cdraw_transform_base(transformConcatComponents3)


#ifdef __cplusplus
}
#endif // #ifdef __cplusplus


#endif // #if (!(defined _CDRAW_TRANSFORM3_H_) && (defined _CDRAW_TRANSFORM_H_))