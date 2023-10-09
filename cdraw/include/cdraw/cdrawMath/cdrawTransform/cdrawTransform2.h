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
* cdrawTransform2.h
* Declaration of preferred floating point precision 2D transform functions.
*/

#if (!(defined _CDRAW_TRANSFORM2_H_) && (defined _CDRAW_TRANSFORM_H_))
#define _CDRAW_TRANSFORM2_H_


#ifdef __cplusplus
extern "C" {
#endif // #ifdef __cplusplus

	/// <summary>
	/// Initialize rotation matrix to identity.
	/// </summary>
	/// <param name="R_out">Result matrix.</param>
	/// <returns><paramref name="R_out"/></returns>
#define RmatID2d cdraw_transform_base(RmatID2d)

	/// <summary>
	/// Initialize rotation matrix from encoded angle.
	/// </summary>
	/// <param name="R_out">Result matrix.</param>
	/// <param name="angle_actual_out_opt">Storage for resulting actual angle used for encoding (optional).</param>
	/// <param name="angle">Desired angle to encode.</param>
	/// <returns><paramref name="R_out"/></returns>
#define RmatFromAngle2 cdraw_transform_base(RmatFromAngle2)

	/// <summary>
	/// Decode rotation matrix to extract angle.
	/// </summary>
	/// <param name="R">Input matrix to be converted.</param>
	/// <returns>Actual angle.</returns>
#define RmatToAngle2 cdraw_transform_base(RmatToAngle2)


#ifdef __cplusplus
}
#endif // #ifdef __cplusplus


#endif // #if (!(defined _CDRAW_TRANSFORM2_H_) && (defined _CDRAW_TRANSFORM_H_))