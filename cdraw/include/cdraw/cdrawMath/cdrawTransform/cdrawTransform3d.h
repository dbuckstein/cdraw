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
* cdrawTransform3d.h
* Declaration of double-precision 3D transform functions.
*/

#if (!(defined _CDRAW_TRANSFORM3D_H_) && (defined _CDRAW_TRANSFORM_H_))
#define _CDRAW_TRANSFORM3D_H_


#ifdef __cplusplus
extern "C" {
#endif // #ifdef __cplusplus

	/// <summary>
	/// Initialize rotation matrix to identity.
	/// </summary>
	/// <param name="R_out">Result matrix.</param>
	/// <returns><paramref name="R_out"/></returns>
	doubleNx3_t RmatID3d(Rmat3d_t R_out);

	/// <summary>
	/// Calculate the inverse of a rotation matrix (transpose).
	/// </summary>
	/// <param name="R_out">Result matrix (inverse).</param>
	/// <param name="R">Input matrix.</param>
	/// <returns><paramref name="R_out"/></returns>
	doubleNx3_t RmatInv3d(Rmat3d_t R_out, Rmat3d_t const R);

	/// <summary>
	/// Initialize rotation matrix by encoding angles in XYZ order.
	/// </summary>
	/// <param name="R_out">Result matrix.</param>
	/// <param name="anglesXYZ_actual_out_opt">Result actual angles used for encoding (optional).</param>
	/// <param name="anglesXYZ">Desired angles for encoding.</param>
	/// <returns><paramref name="R_out"/></returns>
	doubleNx3_t RmatFromAnglesXYZ3d(Rmat3d_t R_out, double3_t anglesXYZ_actual_out_opt, double3_t const anglesXYZ);

	/// <summary>
	/// Decode rotation matrix encoded in XYZ order to extract angles.
	/// </summary>
	/// <param name="anglesXYZ_actual_out">Result actual encoded angles.</param>
	/// <param name="R">Input matrix.</param>
	/// <returns><paramref name="anglesXYZ_actual_out"/></returns>
	doubleN_t RmatToAnglesXYZ3d(double3_t anglesXYZ_actual_out, Rmat3d_t const R);

	/// <summary>
	/// Initialize rotation matrix by encoding angles in YZX order.
	/// </summary>
	/// <param name="R_out">Result matrix.</param>
	/// <param name="anglesXYZ_actual_out_opt">Result actual angles used for encoding (optional).</param>
	/// <param name="anglesXYZ">Desired angles for encoding.</param>
	/// <returns><paramref name="R_out"/></returns>
	doubleNx3_t RmatFromAnglesYZX3d(Rmat3d_t R_out, double3_t anglesXYZ_actual_out_opt, double3_t const anglesXYZ);

	/// <summary>
	/// Decode rotation matrix encoded in YZX order to extract angles.
	/// </summary>
	/// <param name="anglesXYZ_actual_out">Result actual encoded angles.</param>
	/// <param name="R">Input matrix.</param>
	/// <returns><paramref name="anglesXYZ_actual_out"/></returns>
	doubleN_t RmatToAnglesYZX3d(double3_t anglesXYZ_actual_out, Rmat3d_t const R);

	/// <summary>
	/// Initialize rotation matrix by encoding angles in ZXY order.
	/// </summary>
	/// <param name="R_out">Result matrix.</param>
	/// <param name="anglesXYZ_actual_out_opt">Result actual angles used for encoding (optional).</param>
	/// <param name="anglesXYZ">Desired angles for encoding.</param>
	/// <returns><paramref name="R_out"/></returns>
	doubleNx3_t RmatFromAnglesZXY3d(Rmat3d_t R_out, double3_t anglesXYZ_actual_out_opt, double3_t const anglesXYZ);

	/// <summary>
	/// Decode rotation matrix encoded in ZXY order to extract angles.
	/// </summary>
	/// <param name="anglesXYZ_actual_out">Result actual encoded angles.</param>
	/// <param name="R">Input matrix.</param>
	/// <returns><paramref name="anglesXYZ_actual_out"/></returns>
	doubleN_t RmatToAnglesZXY3d(double3_t anglesXYZ_actual_out, Rmat3d_t const R);

	/// <summary>
	/// Initialize rotation matrix by encoding angles in YXZ order.
	/// </summary>
	/// <param name="R_out">Result matrix.</param>
	/// <param name="anglesXYZ_actual_out_opt">Result actual angles used for encoding (optional).</param>
	/// <param name="anglesXYZ">Desired angles for encoding.</param>
	/// <returns><paramref name="R_out"/></returns>
	doubleNx3_t RmatFromAnglesYXZ3d(Rmat3d_t R_out, double3_t anglesXYZ_actual_out_opt, double3_t const anglesXYZ);

	/// <summary>
	/// Decode rotation matrix encoded in YXZ order to extract angles.
	/// </summary>
	/// <param name="anglesXYZ_actual_out">Result actual encoded angles.</param>
	/// <param name="R">Input matrix.</param>
	/// <returns><paramref name="anglesXYZ_actual_out"/></returns>
	doubleN_t RmatToAnglesYXZ3d(double3_t anglesXYZ_actual_out, Rmat3d_t const R);

	/// <summary>
	/// Initialize rotation matrix by encoding angles in XZY order.
	/// </summary>
	/// <param name="R_out">Result matrix.</param>
	/// <param name="anglesXYZ_actual_out_opt">Result actual angles used for encoding (optional).</param>
	/// <param name="anglesXYZ">Desired angles for encoding.</param>
	/// <returns><paramref name="R_out"/></returns>
	doubleNx3_t RmatFromAnglesXZY3d(Rmat3d_t R_out, double3_t anglesXYZ_actual_out_opt, double3_t const anglesXYZ);

	/// <summary>
	/// Decode rotation matrix encoded in XZY order to extract angles.
	/// </summary>
	/// <param name="anglesXYZ_actual_out">Result actual encoded angles.</param>
	/// <param name="R">Input matrix.</param>
	/// <returns><paramref name="anglesXYZ_actual_out"/></returns>
	doubleN_t RmatToAnglesXZY3d(double3_t anglesXYZ_actual_out, Rmat3d_t const R);

	/// <summary>
	/// Initialize rotation matrix by encoding angles in ZYX order.
	/// </summary>
	/// <param name="R_out">Result matrix.</param>
	/// <param name="anglesXYZ_actual_out_opt">Result actual angles used for encoding (optional).</param>
	/// <param name="anglesXYZ">Desired angles for encoding.</param>
	/// <returns><paramref name="R_out"/></returns>
	doubleNx3_t RmatFromAnglesZYX3d(Rmat3d_t R_out, double3_t anglesXYZ_actual_out_opt, double3_t const anglesXYZ);

	/// <summary>
	/// Decode rotation matrix encoded in ZYX order to extract angles.
	/// </summary>
	/// <param name="anglesXYZ_actual_out">Result actual encoded angles.</param>
	/// <param name="R">Input matrix.</param>
	/// <returns><paramref name="anglesXYZ_actual_out"/></returns>
	doubleN_t RmatToAnglesZYX3d(double3_t anglesXYZ_actual_out, Rmat3d_t const R);

	/// <summary>
	/// Initialize rotation matrix by encoding angles in specified order.
	/// </summary>
	/// <param name="R_out">Result matrix.</param>
	/// <param name="anglesXYZ_actual_out_opt">Result actual angles used for encoding (optional).</param>
	/// <param name="anglesXYZ">Desired angles for encoding.</param>
	/// <param name="encodeOrder">Specified order used to encode matrix.</param>
	/// <returns><paramref name="R_out"/></returns>
	doubleNx3_t RmatFromAngles3d(Rmat3d_t R_out, double3_t anglesXYZ_actual_out_opt, double3_t const anglesXYZ, RotateAxisOrder_t const encodeOrder);

	/// <summary>
	/// Decode rotation matrix encoded in specified order to extract angles.
	/// </summary>
	/// <param name="anglesXYZ_actual_out">Result actual encoded angles.</param>
	/// <param name="R">Input matrix.</param>
	/// <param name="encodeOrder">Specified order used to encode matrix.</param>
	/// <returns><paramref name="anglesXYZ_actual_out"/></returns>
	doubleN_t RmatToAngles3d(double3_t anglesXYZ_actual_out, Rmat3d_t const R, RotateAxisOrder_t const encodeOrder);

	/// <summary>
	/// Copy the forward axis from the input basis given reference frame.
	/// </summary>
	/// <param name="v_out">Result forward axis vector.</param>
	/// <param name="R">Input basis matrix.</param>
	/// <param name="ref">Desired reference frame.</param>
	/// <returns><paramref name="v_out"/></returns>
	doubleN_t RmatForward3d(double3_t v_out, Rmat3d_t const R, ReferenceFrame_t const ref);

	/// <summary>
	/// Copy the backward axis from the input basis given reference frame.
	/// </summary>
	/// <param name="v_out">Result backward axis vector.</param>
	/// <param name="R">Input basis matrix.</param>
	/// <param name="ref">Desired reference frame.</param>
	/// <returns><paramref name="v_out"/></returns>
	doubleN_t RmatBackward3d(double3_t v_out, Rmat3d_t const R, ReferenceFrame_t const ref);

	/// <summary>
	/// Copy the right axis from the input basis given reference frame.
	/// </summary>
	/// <param name="v_out">Result right axis vector.</param>
	/// <param name="R">Input basis matrix.</param>
	/// <param name="ref">Desired reference frame.</param>
	/// <returns><paramref name="v_out"/></returns>
	doubleN_t RmatRight3d(double3_t v_out, Rmat3d_t const R, ReferenceFrame_t const ref);

	/// <summary>
	/// Copy the left axis from the input basis given reference frame.
	/// </summary>
	/// <param name="v_out">Result left axis vector.</param>
	/// <param name="R">Input basis matrix.</param>
	/// <param name="ref">Desired reference frame.</param>
	/// <returns><paramref name="v_out"/></returns>
	doubleN_t RmatLeft3d(double3_t v_out, Rmat3d_t const R, ReferenceFrame_t const ref);

	/// <summary>
	/// Copy the down axis from the input basis given reference frame.
	/// </summary>
	/// <param name="v_out">Result down axis vector.</param>
	/// <param name="R">Input basis matrix.</param>
	/// <param name="ref">Desired reference frame.</param>
	/// <returns><paramref name="v_out"/></returns>
	doubleN_t RmatDown3d(double3_t v_out, Rmat3d_t const R, ReferenceFrame_t const ref);

	/// <summary>
	/// Copy the up axis from the input basis given reference frame.
	/// </summary>
	/// <param name="v_out">Result up axis vector.</param>
	/// <param name="R">Input basis matrix.</param>
	/// <param name="ref">Desired reference frame.</param>
	/// <returns><paramref name="v_out"/></returns>
	doubleN_t RmatUp3d(double3_t v_out, Rmat3d_t const R, ReferenceFrame_t const ref);

	/// <summary>
	/// Copy the reference frame's absolute forward direction.
	/// </summary>
	/// <param name="v_out">Result forward axis vector.</param>
	/// <param name="ref">Desired reference frame.</param>
	/// <returns><paramref name="v_out"/></returns>
	doubleN_t RmatRefForward3d(double3_t v_out, ReferenceFrame_t const ref);

	/// <summary>
	/// Copy the reference frame's absolute backward direction.
	/// </summary>
	/// <param name="v_out">Result backward axis vector.</param>
	/// <param name="ref">Desired reference frame.</param>
	/// <returns><paramref name="v_out"/></returns>
	doubleN_t RmatRefBackward3d(double3_t v_out, ReferenceFrame_t const ref);

	/// <summary>
	/// Copy the reference frame's absolute right direction.
	/// </summary>
	/// <param name="v_out">Result right axis vector.</param>
	/// <param name="ref">Desired reference frame.</param>
	/// <returns><paramref name="v_out"/></returns>
	doubleN_t RmatRefRight3d(double3_t v_out, ReferenceFrame_t const ref);

	/// <summary>
	/// Copy the reference frame's absolute left direction.
	/// </summary>
	/// <param name="v_out">Result left axis vector.</param>
	/// <param name="ref">Desired reference frame.</param>
	/// <returns><paramref name="v_out"/></returns>
	doubleN_t RmatRefLeft3d(double3_t v_out, ReferenceFrame_t const ref);

	/// <summary>
	/// Copy the reference frame's absolute up direction.
	/// </summary>
	/// <param name="v_out">Result up axis vector.</param>
	/// <param name="ref">Desired reference frame.</param>
	/// <returns><paramref name="v_out"/></returns>
	doubleN_t RmatRefUp3d(double3_t v_out, ReferenceFrame_t const ref);

	/// <summary>
	/// Copy the reference frame's absolute down direction.
	/// </summary>
	/// <param name="v_out">Result down axis vector.</param>
	/// <param name="ref">Desired reference frame.</param>
	/// <returns><paramref name="v_out"/></returns>
	doubleN_t RmatRefDown3d(double3_t v_out, ReferenceFrame_t const ref);

	/// <summary>
	/// Calculate a look-at matrix given a center and target in reference frame.
	/// Center to target is absolute forward, uses reference frame's absolute yaw axis as basis.
	/// </summary>
	/// <param name="R_out">Result rotation matrix.</param>
	/// <param name="v_center">Center of reference frame.</param>
	/// <param name="v_target">Target in reference frame</param>
	/// <param name="ref">Desired reference frame.</param>
	/// <returns><paramref name="R_out"/></returns>
	doubleNx3_t RmatLookAt3d(Rmat3d_t R_out, double3_t const v_center, double3_t const v_target, ReferenceFrame_t const ref);

	/// <summary>
	/// Convert absolute axis representation (FRD/RPY) to relative axis representation (XYZ) given reference frame.
	/// </summary>
	/// <param name="v_rel_out">Result relative vector.</param>
	/// <param name="v_abs">Input absolute vector.</param>
	/// <param name="ref">Desired reference frame.</param>
	/// <returns><paramref name="v_rel_out"/></returns>
	doubleN_t vecAbsToRel3d(double3_t v_rel_out, double3_t const v_abs, ReferenceFrame_t const ref);

	/// <summary>
	/// Convert relative axis representation (XYZ) to absolute axis representation (FRD/RPY) given reference frame.
	/// </summary>
	/// <param name="v_abs_out">Result absolute vector.</param>
	/// <param name="v_rel">Input relative vector.</param>
	/// <param name="ref">Desired reference frame.</param>
	/// <returns><paramref name="v_abs_out"/></returns>
	doubleN_t vecRelToAbs3d(double3_t v_abs_out, double3_t const v_rel, ReferenceFrame_t const ref);

	/// <summary>
	/// Get roll component of relative axis in absolute terms.
	/// </summary>
	/// <param name="v_rel">Input relative axis.</param>
	/// <param name="ref">Desired reference frame.</param>
	/// <returns>Roll component of relative input vector.</returns>
	vecd_t vecAbsRoll3d(double3_t const v_rel, ReferenceFrame_t const ref);

	/// <summary>
	/// Get pitch component of relative axis in absolute terms.
	/// </summary>
	/// <param name="v_rel">Input relative axis.</param>
	/// <param name="ref">Desired reference frame.</param>
	/// <returns>Pitch component of relative input vector.</returns>
	vecd_t vecAbsPitch3d(double3_t const v_rel, ReferenceFrame_t const ref);

	/// <summary>
	/// Get yaw component of relative axis in absolute terms.
	/// </summary>
	/// <param name="v_rel">Input relative axis.</param>
	/// <param name="ref">Desired reference frame.</param>
	/// <returns>Yaw component of relative input vector.</returns>
	vecd_t vecAbsYaw3d(double3_t const v_rel, ReferenceFrame_t const ref);

	/// <summary>
	/// Set absolute axis values to relative axis base.
	/// </summary>
	/// <param name="v_rel_out">Result relative vector.</param>
	/// <param name="x_roll">Absolute roll axis value.</param>
	/// <param name="x_pitch">Absolute pitch axis value.</param>
	/// <param name="x_yaw">Absolute yaw axis value.</param>
	/// <param name="ref">Desired reference frame.</param>
	/// <returns><paramref name="v_rel_out"/></returns>
	doubleN_t vecRelSetAbs3d(double3_t v_rel_out, vecd_t const x_roll, vecd_t const x_pitch, vecd_t const x_yaw, ReferenceFrame_t const ref);

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
	doubleN_t vecRelAddAbs3d(double3_t v_rel_out, double3_t const v_rel, vecd_t const d_roll, vecd_t const d_pitch, vecd_t const d_yaw, ReferenceFrame_t const ref);

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
	doubleN_t vecRelScaleAbs3d(double3_t v_rel_out, double3_t const v_rel, vecd_t const s_roll, vecd_t const s_pitch, vecd_t const s_yaw, ReferenceFrame_t const ref);

	/// <summary>
	/// Calculate transformed vector by multiplying transform matrix by input vector; translation ignored.
	/// </summary>
	/// <param name="v_out">Result product vector.</param>
	/// <param name="T_lh">Left-hand input transform matrix.</param>
	/// <param name="v_rh">Right-hand input vector.</param>
	/// <returns><paramref name="v_out"/></returns>
	doubleN_t TmatMulVec3d(double3_t v_out, Tmat3d_t const T_lh, double3_t const v_rh);

	/// <summary>
	/// Calculate transformed point by multiplying transform matrix by input point.
	/// </summary>
	/// <param name="p_out">Result product point.</param>
	/// <param name="T_lh">Left-hand input transform matrix.</param>
	/// <param name="p_rh">Right-hand input point.</param>
	/// <returns><paramref name="v_out"/></returns>
	doubleN_t TmatMulPoint3d(double3_t p_out, Tmat3d_t const T_lh, double3_t const p_rh);

	/// <summary>
	/// Calculate optimized product of transform matrices.
	/// </summary>
	/// <param name="T_out">Result product transform matrix.</param>
	/// <param name="T_lh">Left-hand input transform matrix.</param>
	/// <param name="T_rh">Right-hand input transform matrix.</param>
	/// <returns><paramref name="v_out"/></returns>
	doubleNx3_t TmatMul3d(Tmat3d_t T_out, Tmat3d_t const T_lh, Tmat3d_t const T_rh);

	/// <summary>
	/// Reset rotation descriptor.
	/// </summary>
	/// <param name="rotate_out">Result rotation descriptor.</param>
	/// <returns><paramref name="rotate_out"/></returns>
	rotate3d_t* rotateReset3d(rotate3d_t* rotate_out);

	/// <summary>
	/// Update rotation descriptor encoding based on raw components.
	/// </summary>
	/// <param name="rotate_out">Result rotation descriptor.</param>
	/// <param name="ref">Desired reference frame.</param>
	/// <returns><paramref name="rotate_out"/></returns>
	rotate3d_t* rotateUpdate3d(rotate3d_t* rotate_out, ReferenceFrame_t const ref);

	/// <summary>
	/// Calculate inverted rotation descriptor based on inverted encoded components.
	/// </summary>
	/// <param name="rotate_out">Result rotation descriptor.</param>
	/// <param name="rotate">Input rotation descriptor to invert.</param>
	/// <param name="ref">Desired reference frame.</param>
	/// <returns><paramref name="rotate_out"/></returns>
	rotate3d_t* rotateInvert3d(rotate3d_t* rotate_out, rotate3d_t const* rotate, ReferenceFrame_t const ref);

	/// <summary>
	/// Calculate concatenated rotation descriptor based on the concatenated encoded components of two inputs.
	/// </summary>
	/// <param name="rotate_out">Result rotation descriptor.</param>
	/// <param name="rotate_lh">Left-hand input rotation descriptor to concatenate.</param>
	/// <param name="rotate_rh">Right-hand input rotation descriptor to concatenate.</param>
	/// <param name="ref">Desired reference frame.</param>
	/// <returns><paramref name="rotate_out"/></returns>
	rotate3d_t* rotateConcat3d(rotate3d_t* rotate_out, rotate3d_t const* rotate_lh, rotate3d_t const* rotate_rh, ReferenceFrame_t const ref);

	/// <summary>
	/// Update rotation descriptor raw components based on encoding.
	/// </summary>
	/// <param name="rotate_out">Result rotation descriptor.</param>
	/// <param name="ref">Desired reference frame.</param>
	/// <returns><paramref name="rotate_out"/></returns>
	rotate3d_t* rotateUpdateAngles3d(rotate3d_t* rotate_out, ReferenceFrame_t const ref);

	/// <summary>
	/// Set rotation descriptor based on absolute angles.
	/// </summary>
	/// <param name="rotate_out">Result rotation descriptor.</param>
	/// <param name="angles">Input absolute angles.</param>
	/// <param name="ref">Desired reference frame.</param>
	/// <returns><paramref name="rotate_out"/></returns>
	rotate3d_t* rotateSetAngles3d(rotate3d_t* rotate_out, angle3d_t const* angles, ReferenceFrame_t const ref);

	/// <summary>
	/// Get absolute angles from rotation descriptor.
	/// </summary>
	/// <param name="angles_out">Result absolute angles.</param>
	/// <param name="rotate">Input rotation descriptor.</param>
	/// <param name="ref">Desired reference frame.</param>
	/// <returns><paramref name="angles_out"/></returns>
	angle3d_t* rotateGetAngles3d(angle3d_t* angles_out, rotate3d_t const* rotate, ReferenceFrame_t const ref);

	/// <summary>
	/// Calculate inverted rotation descriptor based on inverted raw angles.
	/// </summary>
	/// <param name="rotate_out">Result rotation descriptor.</param>
	/// <param name="rotate">Input rotation descriptor to invert.</param>
	/// <param name="ref">Desired reference frame.</param>
	/// <returns><paramref name="rotate_out"/></returns>
	rotate3d_t* rotateInvertAngles3d(rotate3d_t* rotate_out, rotate3d_t const* rotate, ReferenceFrame_t const ref);

	/// <summary>
	/// Calculate concatenated rotation descriptor based on the concatenated raw angles of two inputs.
	/// </summary>
	/// <param name="rotate_out">Result rotation descriptor.</param>
	/// <param name="rotate_lh">Left-hand input rotation descriptor to concatenate.</param>
	/// <param name="rotate_rh">Left-hand input rotation descriptor to concatenate.</param>
	/// <param name="ref">Desired reference frame.</param>
	/// <returns><paramref name="rotate_out"/></returns>
	rotate3d_t* rotateConcatAngles3d(rotate3d_t* rotate_out, rotate3d_t const* rotate_lh, rotate3d_t const* rotate_rh, ReferenceFrame_t const ref);

	/// <summary>
	/// Reset translation descriptor.
	/// </summary>
	/// <param name="translate_out">Result translation descriptor.</param>
	/// <returns><paramref name="translate_out"/></returns>
	translate3d_t* translateReset3d(translate3d_t* translate_out);

	/// <summary>
	/// Set translation descriptor based on absolute translation axis.
	/// </summary>
	/// <param name="translate_out">Result translation descriptor.</param>
	/// <param name="axis">Input absolute translation axis.</param>
	/// <param name="ref">Desired reference frame.</param>
	/// <returns><paramref name="translate_out"/></returns>
	translate3d_t* translateSetAxis3d(translate3d_t* translate_out, axis3d_t const* axis, ReferenceFrame_t const ref);

	/// <summary>
	/// Get absolute translation axis from translation descriptor.
	/// </summary>
	/// <param name="axis_out">Result absolute translation axis.</param>
	/// <param name="translate">Input translation descriptor.</param>
	/// <param name="ref">Desired reference frame.</param>
	/// <returns><paramref name="axis_out"/></returns>
	axis3d_t* translateGetAxis3d(axis3d_t* axis_out, translate3d_t const* translate, ReferenceFrame_t const ref);

	/// <summary>
	/// Calculate inverted translation descriptor based on raw axis.
	/// </summary>
	/// <param name="translate_out">Result translation descriptor.</param>
	/// <param name="translate">Input translation descriptor.</param>
	/// <returns><paramref name="translate_out"/></returns>
	translate3d_t* translateInvertAxis3d(translate3d_t* translate_out, translate3d_t const* translate);

	/// <summary>
	/// Calculate concatenated translation descriptor based on concatenated raw axes.
	/// </summary>
	/// <param name="translate_out">Result translation descriptor.</param>
	/// <param name="translate_lh">Left-hand input translation descriptor.</param>
	/// <param name="translate_rh">Right-hand input translation descriptor.</param>
	/// <returns><paramref name="translate_out"/></returns>
	translate3d_t* translateConcatAxis3d(translate3d_t* translate_out, translate3d_t const* translate_lh, translate3d_t const* translate_rh);

	/// <summary>
	/// Reset scale descriptor.
	/// </summary>
	/// <param name="scale_out">Result scale descriptor.</param>
	/// <returns><paramref name="scale_out"/></returns>
	scale3d_t* scaleReset3d(scale3d_t* scale_out);

	/// <summary>
	/// Calculate inverted scale descriptor based on raw axis.
	/// </summary>
	/// <param name="scale_out">Result scale descriptor.</param>
	/// <param name="scale">Input scale descriptor.</param>
	/// <returns><paramref name="scale_out"/></returns>
	scale3d_t* scaleInvertAxis3d(scale3d_t* scale_out, scale3d_t const* scale);

	/// <summary>
	/// Calculate concatenated scale descriptor based on concatenated raw axes.
	/// </summary>
	/// <param name="scale_out">Result scale descriptor.</param>
	/// <param name="scale_lh">Left-hand input scale descriptor.</param>
	/// <param name="scale_rh">Right-hand input scale descriptor.</param>
	/// <returns><paramref name="scale_out"/></returns>
	scale3d_t* scaleConcatAxis3d(scale3d_t* scale_out, scale3d_t const* scale_lh, scale3d_t const* scale_rh);

	/// <summary>
	/// Reset transformation descriptor.
	/// </summary>
	/// <param name="transform_out">Result transformation descriptor.</param>
	/// <returns><paramref name="transform_out"/></returns>
	transform3d_t* transformReset3d(transform3d_t* transform_out);

	/// <summary>
	/// Update transformation encoded matrix based on updated descriptor components.
	/// </summary>
	/// <param name="transform_out">Result transformation descriptor.</param>
	/// <returns><paramref name="transform_out"/></returns>
	transform3d_t* transformUpdate3d(transform3d_t* transform_out);

	/// <summary>
	/// Calculate inverted transformation descriptor based on encoded matrix.
	/// </summary>
	/// <param name="transform_out">Result transformation descriptor.</param>
	/// <param name="transform">Input transformation descriptor.</param>
	/// <param name="ref">Desired reference frame.</param>
	/// <returns><paramref name="transform_out"/></returns>
	transform3d_t* transformInvert3d(transform3d_t* transform_out, transform3d_t const* transform, ReferenceFrame_t const ref);

	/// <summary>
	/// Calculate concatenated transformation descriptor based on encoded matrix.
	/// </summary>
	/// <param name="transform_out">Result transformation descriptor.</param>
	/// <param name="transform_lh">Left-hand input transformation descriptor.</param>
	/// <param name="transform_rh">Right-hand input transformation descriptor.</param>
	/// <param name="ref">Desired reference frame.</param>
	/// <returns><paramref name="transform_out"/></returns>
	transform3d_t* transformConcat3d(transform3d_t* transform_out, transform3d_t const* transform_lh, transform3d_t const* transform_rh, ReferenceFrame_t const ref);

	/// <summary>
	/// Update transformation descriptor components based on encoded matrix.
	/// </summary>
	/// <param name="transform_out">Result transformation descriptor.</param>
	/// <param name="ref">Desired reference frame.</param>
	/// <returns><paramref name="transform_out"/></returns>
	transform3d_t* transformUpdateComponents3d(transform3d_t* transform_out, ReferenceFrame_t const ref);

	/// <summary>
	/// Calculate inverted transformation descriptor based on updated components.
	/// </summary>
	/// <param name="transform_out">Result transformation descriptor.</param>
	/// <param name="transform">Input transformation descriptor.</param>
	/// <param name="ref">Desired reference frame.</param>
	/// <returns><paramref name="transform_out"/></returns>
	transform3d_t* transformInvertComponents3d(transform3d_t* transform_out, transform3d_t const* transform, ReferenceFrame_t const ref);

	/// <summary>
	/// Calculate concatenated transformation descriptor based on updated components.
	/// </summary>
	/// <param name="transform_out">Result transformation descriptor.</param>
	/// <param name="transform_lh">Left-hand input transformation descriptor.</param>
	/// <param name="transform_rh">Right-hand input transformation descriptor.</param>
	/// <param name="ref">Desired reference frame.</param>
	/// <returns><paramref name="transform_out"/></returns>
	transform3d_t* transformConcatComponents3d(transform3d_t* transform_out, transform3d_t const* transform_lh, transform3d_t const* transform_rh, ReferenceFrame_t const ref);


#ifdef __cplusplus
}
#endif // #ifdef __cplusplus


#endif // #if (!(defined _CDRAW_TRANSFORM3D_H_) && (defined _CDRAW_TRANSFORM_H_))