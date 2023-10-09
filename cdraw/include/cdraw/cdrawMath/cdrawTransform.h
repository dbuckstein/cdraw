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
* cdrawTransform.h
* Definition of transform types.
*/

#ifndef _CDRAW_TRANSFORM_H_
#define _CDRAW_TRANSFORM_H_


#include "cdrawMatrix.h"


/******************************************************************************
* Type definitions.
******************************************************************************/

/// <summary>
/// Encoded reference frame.
/// </summary>
typedef uint32_t ReferenceFrame_t;

/// <summary>
/// Description of absolute directions represented by basis axes (XYZ).
/// Also describes handedness: right-handed (rh) bases have counter-clockwise rotations.
/// </summary>
typedef enum AxisOrder_t
{
	/// <summary>
	/// Right-handed basis with XYZ = Forward, Left, Up; 
	/// forward axis defines zero rotation.
	/// </summary>
	axis_rh_FLU,
	/// <summary>
	/// Right-handed basis with XYZ = Right, Forward, Up; 
	/// also known as ENU (East, North, Up), a standard navigation basis.
	/// </summary>
	axis_rh_RFU_ENU,
	/// <summary>
	/// Right-handed basis with XYZ = Forward, Right, Down; 
	/// also known as NED (North, East, Down), the principal aircraft basis with traditional roll/pitch/yaw axes.
	/// </summary>
	axis_rh_FRD_NED,
} AxisOrder_t;

/// <summary>
/// Description of absolute rotation order of axes described in terms of yaw (normal or vertical axis), 
/// pitch (lateral or transverse axis) and roll (longitudinal or directional axis).
/// Positive yaw represents a rotation about the normal or vertical axis such that the forward axis rotates towards the right (CCW about down).
/// Positive pitch represents a rotation about the lateral or transverse axis such that the forward axis rotates upward (CCW about right).
/// Positive roll represents a rotation about the longitudinal or directional axis such that the left axis rotates upward (CCW about forward).
/// </summary>
typedef enum RotateOrder_t
{
	/// <summary>
	/// Rotation order is Yaw then Pitch then Roll (standard aircraft order).
	/// </summary>
	rot_YPR,
	/// <summary>
	/// Rotation order is Pitch then Roll then Yaw.
	/// </summary>
	rot_PRY,
	/// <summary>
	/// Rotation order is Roll then Yaw then Pitch.
	/// </summary>
	rot_RYP,
	/// <summary>
	/// Rotation order is Pitch then Yaw then Roll.
	/// </summary>
	rot_PYR,
	/// <summary>
	/// Rotation order is Yaw then Roll then Pitch.
	/// </summary>
	rot_YRP,
	/// <summary>
	/// Rotation order is Roll then Pitch then Yaw.
	/// </summary>
	rot_RPY,
} RotateOrder_t;


/// <summary>
/// Single-precision container for 2D rotation components.
/// </summary>
typedef struct rotate2f_t
{
	/// <summary>
	/// Rotation matrix representation.
	/// </summary>
	float2x2_t m;

	/// <summary>
	/// Angle representation (always about normal axis).
	/// </summary>
	float_t angle;
} rotate2f_t;

/// <summary>
/// Single-precision container for 2D translation components.
/// </summary>
typedef struct translate2f_t
{
	/// <summary>
	/// Translation vector.
	/// </summary>
	float2_t v;
} translate2f_t;

/// <summary>
/// Single-precision container for 2D scale components.
/// </summary>
typedef struct scale2f_t
{
	/// <summary>
	/// Uniform scale applied to all axes.
	/// </summary>
	float_t s;
} scale2f_t;

/// <summary>
/// Single-precision container for encoded 2D transform components.
/// </summary>
typedef struct transform2f_t
{
	/// <summary>
	/// Encoded transformation matrix with all components.
	/// </summary>
	float3x3_t T;

	/// <summary>
	/// Raw rotation descriptor.
	/// </summary>
	rotate2f_t rotate;

	/// <summary>
	/// Raw translation descriptor.
	/// </summary>
	translate2f_t translate;

	/// <summary>
	/// Raw scale descriptor.
	/// </summary>
	scale2f_t scale;
} transform2f_t;


/// <summary>
/// Double-precision container for 2D rotation components.
/// </summary>
typedef struct rotate2d_t
{
	/// <summary>
	/// Rotation matrix representation.
	/// </summary>
	double2x2_t m;

	/// <summary>
	/// Angle representation (always about normal axis).
	/// </summary>
	double_t angle;
} rotate2d_t;

/// <summary>
/// Double-precision container for 2D translation components.
/// </summary>
typedef struct translate2d_t
{
	/// <summary>
	/// Translation vector.
	/// </summary>
	double2_t v;
} translate2d_t;

/// <summary>
/// Double-precision container for 2D scale components.
/// </summary>
typedef struct scale2d_t
{
	/// <summary>
	/// Uniform scale applied to all axes.
	/// </summary>
	double_t s;
} scale2d_t;

/// <summary>
/// Double-precision container for encoded 2D transform components.
/// </summary>
typedef struct transform2d_t
{
	/// <summary>
	/// Encoded transformation matrix with all components.
	/// </summary>
	double3x3_t T;

	/// <summary>
	/// Raw rotation descriptor.
	/// </summary>
	rotate2d_t rotate;

	/// <summary>
	/// Raw translation descriptor.
	/// </summary>
	translate2d_t translate;

	/// <summary>
	/// Raw scale descriptor.
	/// </summary>
	scale2d_t scale;
} transform2d_t;


/// <summary>
/// Single-precision container for 3D rotation components.
/// </summary>
typedef struct rotate3f_t
{
	/// <summary>
	/// Rotation matrix representation.
	/// </summary>
	float3x3_t m;

	/// <summary>
	/// Euler angle representation.
	/// </summary>
	float3_t angles;
} rotate3f_t;

/// <summary>
/// Single-precision container for 3D translation components.
/// </summary>
typedef struct translate3f_t
{
	/// <summary>
	/// Translation vector.
	/// </summary>
	float3_t v;
} translate3f_t;

/// <summary>
/// Single-precision container for 3D scale components.
/// </summary>
typedef struct scale3f_t
{
	/// <summary>
	/// Uniform scale applied to all axes.
	/// </summary>
	float_t s;
} scale3f_t;

/// <summary>
/// Single-precision container for encoded 3D transform components.
/// </summary>
typedef struct transform3f_t
{
	/// <summary>
	/// Encoded transformation matrix with all components.
	/// </summary>
	float4x4_t T;

	/// <summary>
	/// Raw rotation descriptor.
	/// </summary>
	rotate3f_t rotate;

	/// <summary>
	/// Raw translation descriptor.
	/// </summary>
	translate3f_t translate;

	/// <summary>
	/// Raw scale descriptor.
	/// </summary>
	scale3f_t scale;
} transform3f_t;


/// <summary>
/// Double-precision container for 3D rotation components.
/// </summary>
typedef struct rotate3d_t
{
	/// <summary>
	/// Rotation matrix representation.
	/// </summary>
	double3x3_t m;

	/// <summary>
	/// Euler angle representation.
	/// </summary>
	double3_t angles;
} rotate3d_t;

/// <summary>
/// Double-precision container for 3D translation components.
/// </summary>
typedef struct translate3d_t
{
	/// <summary>
	/// Translation vector.
	/// </summary>
	double3_t v;
} translate3d_t;

/// <summary>
/// Double-precision container for 3D scale components.
/// </summary>
typedef struct scale3d_t
{
	/// <summary>
	/// Uniform scale applied to all axes.
	/// </summary>
	double_t s;
} scale3d_t;

/// <summary>
/// Double-precision container for encoded 3D transform components.
/// </summary>
typedef struct transform3d_t
{
	/// <summary>
	/// Encoded transformation matrix with all components.
	/// </summary>
	double4x4_t transform;

	/// <summary>
	/// Raw rotation descriptor.
	/// </summary>
	rotate3d_t rotate;

	/// <summary>
	/// Raw translation descriptor.
	/// </summary>
	translate3d_t translate;

	/// <summary>
	/// Raw scale descriptor.
	/// </summary>
	scale3d_t scale;
} transform3d_t;


/******************************************************************************
* Configuration.
******************************************************************************/

#if CDRAW_USING_SCALAR_PREF
#if CDRAW_USING_SCALAR_PREF_DOUBLE
typedef rotate2d_t		rotate2_t;
typedef translate2d_t	translate2_t;
typedef scale2d_t		scale2_t;
typedef transform2d_t	transform2_t;
typedef rotate3d_t		rotate3_t;
typedef translate3d_t	translate3_t;
typedef scale3d_t		scale3_t;
typedef transform3d_t	transform3_t;
#else // #if CDRAW_USING_SCALAR_PREF_DOUBLE
typedef rotate2f_t		rotate2_t;
typedef translate2f_t	translate2_t;
typedef scale2f_t		scale2_t;
typedef transform2f_t	transform2_t;
typedef rotate3f_t		rotate3_t;
typedef translate3f_t	translate3_t;
typedef scale3f_t		scale3_t;
typedef transform3f_t	transform3_t;
#endif // #else // #if CDRAW_USING_SCALAR_PREF_DOUBLE
#else // #if CDRAW_USING_SCALAR_PREF
#endif // #else // #if CDRAW_USING_SCALAR_PREF
#define transform_suffix	matrix_suffix
#define transform_other		matrix_other


/******************************************************************************
* Type-agnostic functions.
******************************************************************************/

/// <summary>
/// Encode reference frame given preferred absolute basis axis and rotation orders.
/// </summary>
/// <param name="axisOrder">Absolute axis order.</param>
/// <param name="rotateOrder">Absolute rotation order</param>
/// <returns>Encoded reference frame.</returns>
ReferenceFrame_t cdrawReferenceFrame(AxisOrder_t const axisOrder, RotateOrder_t const rotateOrder);

/// <summary>
/// Decode absolute basis axis order from encoded reference frame.
/// </summary>
/// <param name="refFrame">Encoded reference frame.</param>
/// <returns>Absolute basis axis order.</returns>
AxisOrder_t cdrawReferenceFrameAxisOrder(ReferenceFrame_t const refFrame);

/// <summary>
/// Decode absolute rotation order from encoded reference frame.
/// </summary>
/// <param name="refFrame">Encoded reference frame.</param>
/// <returns>Absolute rotation order.</returns>
RotateOrder_t cdrawReferenceFrameRotateOrder(ReferenceFrame_t const refFrame);


/******************************************************************************
* Functions.
******************************************************************************/

#include "cdrawTransform/cdrawTransform2f.h"
#include "cdrawTransform/cdrawTransform2d.h"
#include "cdrawTransform/cdrawTransform3f.h"
#include "cdrawTransform/cdrawTransform3d.h"

#if CDRAW_USING_SCALAR_PREF
#include "cdrawTransform/cdrawTransform2.h"
#include "cdrawTransform/cdrawTransform3.h"
#endif // #if CDRAW_USING_SCALAR_PREF


#if !CDRAW_USING_PRECOMPILE_LIBS
#include "_inl/cdrawTransform.inl"
#endif // #if !CDRAW_USING_PRECOMPILE_LIBS


// ****TO-DO: dual quaternion (dq type, q type, add dq to transform, q to rotate and q to translate)
// ****TO-DO: axis-angle (managed vector type, add to rotate)
// ****TO-DO: non-uniforms (add to scale)
// ****TO-DO: implement 2D functionality


#endif // #ifndef _CDRAW_TRANSFORM_H_