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
* cdrawScalar.h
* Definition of scalar (real number) type.
*/

#ifndef _CDRAW_SCALAR_H_
#define _CDRAW_SCALAR_H_


#include <stdlib.h>
#include <float.h>
#include <math.h>

#include "cdraw/cdraw/cdrawConfig.h"


/******************************************************************************
* Configuration.
******************************************************************************/

// Describes whether double-precision float is desired for base scalar type.
//	-CDRAW_ENABLE if requesting base scalar type use double-precision (double).
//	-CDRAW_DISABLE if requesting base scalar type use single-precision (float).
#define CDRAW_SCALAR_DOUBLE			CDRAW_DISABLE
#define CDRAW_USING_SCALAR_DOUBLE	cdraw_enabled(CDRAW_SCALAR_DOUBLE) && !CDRAW_USING_PRECOMPILE_LIBS
#if CDRAW_USING_SCALAR_DOUBLE
typedef fp64_t			scalar_t;	// Global scalar number representation (double-precision).
#define scalar_suffix	D
#define const_suffix	
#define SC_EPSILON		(DBL_EPSILON)
#else // #if CDRAW_USING_SCALAR_DOUBLE
typedef fp32_t			scalar_t;	// Global scalar number representation (single-precision).
#define scalar_suffix	F
#define const_suffix	f
#define SC_EPSILON		(FLT_EPSILON)
#endif // #else // #if CDRAW_USING_SCALAR_DOUBLE
#define cdraw_scalar_base(name)											tokencat(name, scalar_suffix)
#define cdraw_scalar_base_t(name)										tokencat(cdraw_scalar_base(name), _t)
#define cdraw_scalar_const(name,value_fp64)									\
		static fp64_t const tokencat(name, D) = value_fp64;					\
		static fp32_t const tokencat(name, F) = tokencat(value_fp64, f);	\
		static scalar_t const name = tokencat(value_fp64, const_suffix)
#define cdraw_scalar_declDF(name,value_fp64)				\
		fp64_t tokencat(name, D) = value_fp64;				\
		fp32_t tokencat(name, F) = tokencat(value_fp64, f)


/******************************************************************************
* Constant scalars, also defined for single and double precisions.
******************************************************************************/

cdraw_scalar_const(sc0, 0.0);
cdraw_scalar_const(sc1, 1.0);
cdraw_scalar_const(sc2, 2.0);
cdraw_scalar_const(sc3, 3.0);
cdraw_scalar_const(sc4, 4.0);
cdraw_scalar_const(sc5, 5.0);
cdraw_scalar_const(sc6, 6.0);
cdraw_scalar_const(sc1_2, 0.5);
cdraw_scalar_const(sc1_3, 0.33333333333333333333333333333333);
cdraw_scalar_const(sc1_4, 0.25);
cdraw_scalar_const(sc1_5, 0.2);
cdraw_scalar_const(sc1_6, 0.16666666666666666666666666666667);
cdraw_scalar_const(sc2_3, 0.66666666666666666666666666666667);
cdraw_scalar_const(sc2_5, 0.4);
cdraw_scalar_const(sc3_2, 1.5);
cdraw_scalar_const(sc3_4, 0.75);
cdraw_scalar_const(sc3_5, 0.6);
cdraw_scalar_const(sc4_3, 1.3333333333333333333333333333333);
cdraw_scalar_const(sc4_5, 0.8);
cdraw_scalar_const(sc5_2, 2.5);
cdraw_scalar_const(sc5_3, 1.6666666666666666666666666666667);
cdraw_scalar_const(sc5_4, 1.25);
cdraw_scalar_const(sc5_6, 0.83333333333333333333333333333333);
cdraw_scalar_const(sc6_5, 1.2);
cdraw_scalar_const(scPi, 3.1415926535897932384626433832795);
cdraw_scalar_const(sc2Pi, 6.283185307179586476925286766559);
cdraw_scalar_const(sc3Pi, 9.4247779607693797153879301498385);
cdraw_scalar_const(sc4Pi, 12.566370614359172953850573533118);
cdraw_scalar_const(sc6Pi, 18.849555921538759430775860299677);
cdraw_scalar_const(scPi_2, 1.5707963267948966192313216916398);
cdraw_scalar_const(scPi_3, 1.0471975511965977461542144610932);
cdraw_scalar_const(scPi_4, 0.78539816339744830961566084581988);
cdraw_scalar_const(scPi_6, 0.52359877559829887307710723054658);
cdraw_scalar_const(sc2Pi_3, 2.0943951023931954923084289221863);
cdraw_scalar_const(sc3Pi_2, 4.7123889803846898576939650749193);
cdraw_scalar_const(sc3Pi_4, 2.3561944901923449288469825374596);
cdraw_scalar_const(sc4Pi_3, 4.1887902047863909846168578443727);
cdraw_scalar_const(sc5Pi_6, 2.6179938779914943653855361527329);
cdraw_scalar_const(scSqrt2, 1.4142135623730950488016887242097);
cdraw_scalar_const(scSqrt3, 1.7320508075688772935274463415059);
cdraw_scalar_const(scSqrt1_2, 0.70710678118654752440084436210485);
cdraw_scalar_const(scSqrt1_3, 0.57735026918962576450914878050196);
cdraw_scalar_const(scSqrt2_3, 0.81649658092772603273242802490196);
cdraw_scalar_const(scSqrt3_2, 1.2247448713915890490986420373529);
cdraw_scalar_const(scSqrt3_4, 0.86602540378443864676372317075294);
cdraw_scalar_const(scSqrt4_3, 1.1547005383792515290182975610039);
cdraw_scalar_const(sc30, 30.0);
cdraw_scalar_const(sc45, 45.0);
cdraw_scalar_const(sc60, 60.0);
cdraw_scalar_const(sc90, 90.0);
cdraw_scalar_const(sc120, 120.0);
cdraw_scalar_const(sc180, 180.0);
cdraw_scalar_const(sc270, 270.0);
cdraw_scalar_const(sc360, 360.0);

#define scEpsD		(DBL_EPSILON)
#define scEpsF		(FLT_EPSILON)
#define scEps		cdraw_scalar_base(scEps)
#define scAtrigEpsD	(DBL_EPSILON * 32.0)
#define scAtrigEpsF	(FLT_EPSILON * 32.0f)
#define scAtrigEps	cdraw_scalar_base(scAtrigEps)


/******************************************************************************
* Functions.
******************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif // #ifdef __cplusplus

	/// <summary>
	/// Test if value is approximately zero.
	/// </summary>
	/// <param name="x">Value to test.</param>
	/// <returns>True if value is approximately zero.</returns>
	bool scIsZeroApproxF(fp32_t const x);
	
	/// <summary>
	/// Test if value is approximately non-zero.
	/// </summary>
	/// <param name="x">Value to test.</param>
	/// <returns>True if value is approximately non-zero.</returns>
	bool scIsNonZeroApproxF(fp32_t const x);
	
	/// <summary>
	/// Test if value is approximately positive.
	/// </summary>
	/// <param name="x">Value to test.</param>
	/// <returns>True if value is approximately positive.</returns>
	bool scIsPositiveApproxF(fp32_t const x);
	
	/// <summary>
	/// Test if value is approximately non-positive.
	/// </summary>
	/// <param name="x">Value to test.</param>
	/// <returns>True if value is approximately non-positive.</returns>
	bool scIsNonPositiveApproxF(fp32_t const x);
	
	/// <summary>
	/// Test if value is approximately negative.
	/// </summary>
	/// <param name="x">Value to test.</param>
	/// <returns>True if value is approximately negative.</returns>
	bool scIsNegativeApproxF(fp32_t const x);
	
	/// <summary>
	/// Test if value is approximately non-negative.
	/// </summary>
	/// <param name="x">Value to test.</param>
	/// <returns>True if value is approximately non-negative.</returns>
	bool scIsNonNegativeApproxF(fp32_t const x);


	/// <summary>
	/// Determine sign of input.
	/// </summary>
	/// <param name="x">Input value.</param>
	/// <returns>Sign of input: sgn(x).</returns>
	fp32_t scSgnF(fp32_t const x);
	
	/// <summary>
	/// Determine absolute value of input.
	/// </summary>
	/// <param name="x">Input value.</param>
	/// <returns>Absolute value of input: abs(x).</returns>
	fp32_t scAbsF(fp32_t const x);
	
	/// <summary>
	/// Determine fractional part of input.
	/// </summary>
	/// <param name="x">Input value.</param>
	/// <returns>Fraction of input: frac(x).</returns>
	fp32_t scFracF(fp32_t const x);
	
	/// <summary>
	/// Safely calculate reciprocal of input.
	/// </summary>
	/// <param name="x">Input value.</param>
	/// <returns>Reciprocal of input if non-zero, otherwise zero.</returns>
	fp32_t scRecipF(fp32_t const x);
	
	/// <summary>
	/// Safely calculate quotient of inputs.
	/// </summary>
	/// <param name="x_num">Input numerator.</param>
	/// <param name="x_den">Input denominator.</param>
	/// <returns>Quotient of inputs if denominator is non-zero, otherwise zero.</returns>
	fp32_t scDivF(fp32_t const x_num, fp32_t x_den);
	
	/// <summary>
	/// Safely calculate modulo, or division remainder of inputs.
	/// </summary>
	/// <param name="x_num">Input numerator.</param>
	/// <param name="x_den">Input denominator.</param>
	/// <returns>Remainder of inputs if denominator is non-zero, otherwise numerator.</returns>
	fp32_t scModF(fp32_t const x_num, fp32_t const x_den);
	
	/// <summary>
	/// Calculate linear interpolation value given parameter and range.
	/// </summary>
	/// <param name="u">Interpolation parameter, in [0,1] for interpolation, otherwise extrapolation.</param>
	/// <param name="x_min">Minimum value or lower bound of range.</param>
	/// <param name="x_max">Maximum value or upper bound of range.</param>
	/// <returns>Relative value in range.</returns>
	fp32_t scLerpF(fp32_t const u, fp32_t const x_min, fp32_t const x_max);
	
	/// <summary>
	/// Calculate linear interpolation parameter given value and range.
	/// </summary>
	/// <param name="x">Value assumed to be in range.</param>
	/// <param name="x_min">Minimum value or lower bound of range.</param>
	/// <param name="x_max">Maximum value or upper bound of range.</param>
	/// <returns>Parameter that would give value if used to interpolate.</returns>
	fp32_t scLerpInvF(fp32_t const x, fp32_t const x_min, fp32_t const x_max);
	
	/// <summary>
	/// Clamp value into given range.
	/// </summary>
	/// <param name="x">Value to clamp.</param>
	/// <param name="x_min">Minimum value or lower bound of range.</param>
	/// <param name="x_max">Maximum value or upper bound of range.</param>
	/// <returns>Value clamped within range.</returns>
	fp32_t scClampF(fp32_t const x, fp32_t const x_min, fp32_t const x_max);
	
	/// <summary>
	/// Calculate repeated value in range if it is outside the value.
	/// </summary>
	/// <param name="x">Value to place in range.</param>
	/// <param name="x_min">Minimum value or lower bound of range.</param>
	/// <param name="x_max">Maximum value or upper bound of range.</param>
	/// <returns>Value repeated into range.</returns>
	fp32_t scRepeatF(fp32_t const x, fp32_t const x_min, fp32_t const x_max);

	/// <summary>
	/// Remap value relative to one range into another range.
	/// </summary>
	/// <param name="x">Value to remap in source range.</param>
	/// <param name="x_min_src">Minimum value or lower bound of source range.</param>
	/// <param name="x_max_src">Maximum value or upper bound of source range.</param>
	/// <param name="x_min_dst">Minimum value or lower bound of destination range.</param>
	/// <param name="x_max_dst">Maximum value or upper bound of destination range.</param>
	/// <returns>Value remapped to destination range.</returns>
	fp32_t scRemapF(fp32_t const x, fp32_t const x_min_src, fp32_t const x_max_src, fp32_t const x_min_dst, fp32_t const x_max_dst);

	/// <summary>
	/// Test if value is in specified range.
	/// </summary>
	/// <param name="x">Value to test.</param>
	/// <param name="x_min">Minimum value or lower bound of range.</param>
	/// <param name="x_max">Maximum value or upper bound of range.</param>
	/// <returns>True if value is within range.</returns>
	bool scIsInRangeF(fp32_t const x, fp32_t const x_min, fp32_t const x_max);

	/// <summary>
	/// Test if value is in unit range, inclusive: [0,1].
	/// </summary>
	/// <param name="x">Value to test.</param>
	/// <returns>True if value is within range.</returns>
	bool scIsInUnitRangeF(fp32_t const x);

	/// <summary>
	/// Test if value is in symmetric range, inclusive: [-1,+1].
	/// </summary>
	/// <param name="x">Value to test.</param>
	/// <returns>True if value is within range.</returns>
	bool scIsInSymmRangeF(fp32_t const x);


	/// <summary>
	/// Calculate square root of non-negative value.
	/// </summary>
	/// <param name="x">Non-negative input value.</param>
	/// <returns>Square root of input.</returns>
	fp32_t scSqrtF(fp32_t const x);
	
	/// <summary>
	/// Calculate inverse square root of positive value.
	/// </summary>
	/// <param name="x">Positive input value.</param>
	/// <returns>Inverse square root of input.</returns>
	fp32_t scSqrtInvF(fp32_t const x);
	
	/// <summary>
	/// Calculate cube root of value.
	/// </summary>
	/// <param name="x">Input value.</param>
	/// <returns>Cube root of input.</returns>
	fp32_t scCbrtF(fp32_t const x);
	
	/// <summary>
	/// Calculate inverse cube root of non-zero value.
	/// </summary>
	/// <param name="x">Non-zero input value.</param>
	/// <returns>Inverse cube root of input.</returns>
	fp32_t scCbrtInvF(fp32_t const x);
	
	/// <summary>
	/// Calculate degree angle in radians.
	/// </summary>
	/// <param name="x">Angle in degrees.</param>
	/// <returns>Angle in radians.</returns>
	fp32_t scDeg2RadF(fp32_t const x);

	/// <summary>
	/// Calculate radian angle in degrees.
	/// </summary>
	/// <param name="x">Angle in radians.</param>
	/// <returns>Angle in degrees.</returns>
	fp32_t scRad2DegF(fp32_t const x);

	/// <summary>
	/// Calculate equivalent angle in range [-180,+180) degrees.
	/// </summary>
	/// <param name="x">Angle in degrees.</param>
	/// <returns>Angle in degrees in range [-180,+180).</returns>
	fp32_t scNormalizeAngle180F(fp32_t const x);

	/// <summary>
	/// Calculate equivalent angle in range [0,360) degrees.
	/// </summary>
	/// <param name="x">Angle in degrees.</param>
	/// <returns>Angle in degrees in range [0,360).</returns>
	fp32_t scNormalizeAngle360F(fp32_t const x);

	/// <summary>
	/// Calculate equivalent angle in range [-pi,+pi) radians.
	/// </summary>
	/// <param name="x">Angle in radians.</param>
	/// <returns>Angle in radians in range [-pi,+pi).</returns>
	fp32_t scNormalizeAnglePiF(fp32_t const x);

	/// <summary>
	/// Calculate equivalent angle in range [0,2pi) radians.
	/// </summary>
	/// <param name="x">Angle in radians.</param>
	/// <returns>Angle in radians in range [0,2pi).</returns>
	fp32_t scNormalizeAngle2PiF(fp32_t const x);

	/// <summary>
	/// Calculate sine of angle in radians.
	/// </summary>
	/// <param name="x">Angle in radians.</param>
	/// <returns>Sine of angle.</returns>
	fp32_t scSinrF(fp32_t const x);
	
	/// <summary>
	/// Calculate arcsine angle in radians.
	/// </summary>
	/// <param name="x">Sine of angle.</param>
	/// <returns>Angle in radians.</returns>
	fp32_t scAsinrF(fp32_t const x);
	
	/// <summary>
	/// Calculate cosine of angle in radians.
	/// </summary>
	/// <param name="x">Angle in radians.</param>
	/// <returns>Cosine of angle.</returns>
	fp32_t scCosrF(fp32_t const x);
	
	/// <summary>
	/// Calculate arccosine angle in radians.
	/// </summary>
	/// <param name="x">Cosine of angle.</param>
	/// <returns>Angle in radians.</returns>
	fp32_t scAcosrF(fp32_t const x);
	
	/// <summary>
	/// Calculate tangent of angle in radians.
	/// </summary>
	/// <param name="x">Angle in radians.</param>
	/// <returns>Tangent of angle.</returns>
	fp32_t scTanrF(fp32_t const x);
	
	/// <summary>
	/// Calculate arctangent angle in radians.
	/// </summary>
	/// <param name="x">Tangent of angle.</param>
	/// <returns>Angle in radians.</returns>
	fp32_t scAtanrF(fp32_t const x);
	
	/// <summary>
	/// Calculate two-component arctangent given sine and cosine parts: tan(a) = y/x = sin(a) / cos(a).
	/// </summary>
	/// <param name="y">Numerator of tangent quotient representing sine.</param>
	/// <param name="x">Denominator of tangent quotient representing cosine.</param>
	/// <returns>Angle in radians.</returns>
	fp32_t scAtan2rF(fp32_t const y, fp32_t const x);

	/// <summary>
	/// Calculate sine of angle in degrees.
	/// </summary>
	/// <param name="x">Angle in degrees.</param>
	/// <returns>Sine of angle.</returns>
	fp32_t scSindF(fp32_t const x);

	/// <summary>
	/// Calculate arcsine angle in degrees.
	/// </summary>
	/// <param name="x">Sine of angle.</param>
	/// <returns>Angle in degrees.</returns>
	fp32_t scAsindF(fp32_t const x);

	/// <summary>
	/// Calculate cosine of angle in degrees.
	/// </summary>
	/// <param name="x">Angle in degrees.</param>
	/// <returns>Cosine of angle.</returns>
	fp32_t scCosdF(fp32_t const x);

	/// <summary>
	/// Calculate arccosine angle in degrees.
	/// </summary>
	/// <param name="x">Cosine of angle.</param>
	/// <returns>Angle in degrees.</returns>
	fp32_t scAcosdF(fp32_t const x);

	/// <summary>
	/// Calculate tangent of angle in degrees.
	/// </summary>
	/// <param name="x">Angle in degrees.</param>
	/// <returns>Tangent of angle.</returns>
	fp32_t scTandF(fp32_t const x);

	/// <summary>
	/// Calculate arctangent angle in degrees.
	/// </summary>
	/// <param name="x">Tangent of angle.</param>
	/// <returns>Angle in degrees.</returns>
	fp32_t scAtandF(fp32_t const x);

	/// <summary>
	/// Calculate two-component arctangent given sine and cosine parts: tan(a) = y/x = sin(a) / cos(a).
	/// </summary>
	/// <param name="y">Numerator of tangent quotient representing sine.</param>
	/// <param name="x">Denominator of tangent quotient representing cosine.</param>
	/// <returns>Angle in degrees.</returns>
	fp32_t scAtan2dF(fp32_t const y, fp32_t const x);


	/// <summary>
	/// Random number generator.
	/// </summary>
	/// <returns>Random number in [0,RAND_MAX].</returns>
	fp32_t scRandF();

	/// <summary>
	/// Random unit number generator.
	/// </summary>
	/// <returns>Random number in [0,1).</returns>
	fp32_t scRandUnitF();

	/// <summary>
	/// Random symmetric number generator.
	/// </summary>
	/// <returns>Random number in [-1,+1).</returns>
	fp32_t scRandSymmF();


	/// <summary>
	/// Test if value is approximately zero.
	/// </summary>
	/// <param name="x">Value to test.</param>
	/// <returns>True if value is approximately zero.</returns>
	bool scIsZeroApproxD(fp64_t const x);

	/// <summary>
	/// Test if value is approximately non-zero.
	/// </summary>
	/// <param name="x">Value to test.</param>
	/// <returns>True if value is approximately non-zero.</returns>
	bool scIsNonZeroApproxD(fp64_t const x);

	/// <summary>
	/// Test if value is approximately positive.
	/// </summary>
	/// <param name="x">Value to test.</param>
	/// <returns>True if value is approximately positive.</returns>
	bool scIsPositiveApproxD(fp64_t const x);

	/// <summary>
	/// Test if value is approximately non-positive.
	/// </summary>
	/// <param name="x">Value to test.</param>
	/// <returns>True if value is approximately non-positive.</returns>
	bool scIsNonPositiveApproxD(fp64_t const x);

	/// <summary>
	/// Test if value is approximately negative.
	/// </summary>
	/// <param name="x">Value to test.</param>
	/// <returns>True if value is approximately negative.</returns>
	bool scIsNegativeApproxD(fp64_t const x);

	/// <summary>
	/// Test if value is approximately non-negative.
	/// </summary>
	/// <param name="x">Value to test.</param>
	/// <returns>True if value is approximately non-negative.</returns>
	bool scIsNonNegativeApproxD(fp64_t const x);


	/// <summary>
	/// Determine sign of input.
	/// </summary>
	/// <param name="x">Input value.</param>
	/// <returns>Sign of input: sgn(x).</returns>
	fp64_t scSgnD(fp64_t const x);

	/// <summary>
	/// Determine absolute value of input.
	/// </summary>
	/// <param name="x">Input value.</param>
	/// <returns>Absolute value of input: abs(x).</returns>
	fp64_t scAbsD(fp64_t const x);

	/// <summary>
	/// Determine fractional part of input.
	/// </summary>
	/// <param name="x">Input value.</param>
	/// <returns>Fraction of input: frac(x).</returns>
	fp64_t scFracD(fp64_t const x);

	/// <summary>
	/// Safely calculate reciprocal of input.
	/// </summary>
	/// <param name="x">Input value.</param>
	/// <returns>Reciprocal of input if non-zero, otherwise zero.</returns>
	fp64_t scRecipD(fp64_t const x);

	/// <summary>
	/// Safely calculate quotient of inputs.
	/// </summary>
	/// <param name="x_num">Input numerator.</param>
	/// <param name="x_den">Input denominator.</param>
	/// <returns>Quotient of inputs if denominator is non-zero, otherwise zero.</returns>
	fp64_t scDivD(fp64_t const x_num, fp64_t x_den);

	/// <summary>
	/// Safely calculate modulo, or division remainder of inputs.
	/// </summary>
	/// <param name="x_num">Input numerator.</param>
	/// <param name="x_den">Input denominator.</param>
	/// <returns>Remainder of inputs if denominator is non-zero, otherwise numerator.</returns>
	fp64_t scModD(fp64_t const x_num, fp64_t const x_den);

	/// <summary>
	/// Calculate linear interpolation value given parameter and range.
	/// </summary>
	/// <param name="u">Interpolation parameter, in [0,1] for interpolation, otherwise extrapolation.</param>
	/// <param name="x_min">Minimum value or lower bound of range.</param>
	/// <param name="x_max">Maximum value or upper bound of range.</param>
	/// <returns>Relative value in range.</returns>
	fp64_t scLerpD(fp64_t const u, fp64_t const x_min, fp64_t const x_max);

	/// <summary>
	/// Calculate linear interpolation parameter given value and range.
	/// </summary>
	/// <param name="x">Value assumed to be in range.</param>
	/// <param name="x_min">Minimum value or lower bound of range.</param>
	/// <param name="x_max">Maximum value or upper bound of range.</param>
	/// <returns>Parameter that would give value if used to interpolate.</returns>
	fp64_t scLerpInvD(fp64_t const x, fp64_t const x_min, fp64_t const x_max);

	/// <summary>
	/// Clamp value into given range.
	/// </summary>
	/// <param name="x">Value to clamp.</param>
	/// <param name="x_min">Minimum value or lower bound of range.</param>
	/// <param name="x_max">Maximum value or upper bound of range.</param>
	/// <returns>Value clamped within range.</returns>
	fp64_t scClampD(fp64_t const x, fp64_t const x_min, fp64_t const x_max);

	/// <summary>
	/// Calculate repeated value in range if it is outside the value.
	/// </summary>
	/// <param name="x">Value to place in range.</param>
	/// <param name="x_min">Minimum value or lower bound of range.</param>
	/// <param name="x_max">Maximum value or upper bound of range.</param>
	/// <returns>Value repeated into range.</returns>
	fp64_t scRepeatD(fp64_t const x, fp64_t const x_min, fp64_t const x_max);

	/// <summary>
	/// Remap value relative to one range into another range.
	/// </summary>
	/// <param name="x">Value to remap in source range.</param>
	/// <param name="x_min_src">Minimum value or lower bound of source range.</param>
	/// <param name="x_max_src">Maximum value or upper bound of source range.</param>
	/// <param name="x_min_dst">Minimum value or lower bound of destination range.</param>
	/// <param name="x_max_dst">Maximum value or upper bound of destination range.</param>
	/// <returns>Value remapped to destination range.</returns>
	fp64_t scRemapD(fp64_t const x, fp64_t const x_min_src, fp64_t const x_max_src, fp64_t const x_min_dst, fp64_t const x_max_dst);

	/// <summary>
	/// Test if value is in specified range.
	/// </summary>
	/// <param name="x">Value to test.</param>
	/// <param name="x_min">Minimum value or lower bound of range.</param>
	/// <param name="x_max">Maximum value or upper bound of range.</param>
	/// <returns>True if value is within range.</returns>
	bool scIsInRangeD(fp64_t const x, fp64_t const x_min, fp64_t const x_max);

	/// <summary>
	/// Test if value is in unit range, inclusive: [0,1].
	/// </summary>
	/// <param name="x">Value to test.</param>
	/// <returns>True if value is within range.</returns>
	bool scIsInUnitRangeD(fp64_t const x);

	/// <summary>
	/// Test if value is in symmetric range, inclusive: [-1,+1].
	/// </summary>
	/// <param name="x">Value to test.</param>
	/// <returns>True if value is within range.</returns>
	bool scIsInSymmRangeD(fp64_t const x);


	/// <summary>
	/// Calculate square root of non-negative value.
	/// </summary>
	/// <param name="x">Non-negative input value.</param>
	/// <returns>Square root of input.</returns>
	fp64_t scSqrtD(fp64_t const x);

	/// <summary>
	/// Calculate inverse square root of positive value.
	/// </summary>
	/// <param name="x">Positive input value.</param>
	/// <returns>Inverse square root of input.</returns>
	fp64_t scSqrtInvD(fp64_t const x);

	/// <summary>
	/// Calculate cube root of value.
	/// </summary>
	/// <param name="x">Input value.</param>
	/// <returns>Cube root of input.</returns>
	fp64_t scCbrtD(fp64_t const x);

	/// <summary>
	/// Calculate inverse cube root of non-zero value.
	/// </summary>
	/// <param name="x">Non-zero input value.</param>
	/// <returns>Inverse cube root of input.</returns>
	fp64_t scCbrtInvD(fp64_t const x);

	/// <summary>
	/// Calculate degree angle in radians.
	/// </summary>
	/// <param name="x">Angle in degrees.</param>
	/// <returns>Angle in radians.</returns>
	fp64_t scDeg2RadD(fp64_t const x);

	/// <summary>
	/// Calculate radian angle in degrees.
	/// </summary>
	/// <param name="x">Angle in radians.</param>
	/// <returns>Angle in degrees.</returns>
	fp64_t scRad2DegD(fp64_t const x);

	/// <summary>
	/// Calculate equivalent angle in range [-180,+180) degrees.
	/// </summary>
	/// <param name="x">Angle in degrees.</param>
	/// <returns>Angle in degrees in range [-180,+180).</returns>
	fp64_t scNormalizeAngle180D(fp64_t const x);

	/// <summary>
	/// Calculate equivalent angle in range [0,360) degrees.
	/// </summary>
	/// <param name="x">Angle in degrees.</param>
	/// <returns>Angle in degrees in range [0,360).</returns>
	fp64_t scNormalizeAngle360D(fp64_t const x);

	/// <summary>
	/// Calculate equivalent angle in range [-pi,+pi) radians.
	/// </summary>
	/// <param name="x">Angle in radians.</param>
	/// <returns>Angle in radians in range [-pi,+pi).</returns>
	fp64_t scNormalizeAnglePiD(fp64_t const x);

	/// <summary>
	/// Calculate equivalent angle in range [0,2pi) radians.
	/// </summary>
	/// <param name="x">Angle in radians.</param>
	/// <returns>Angle in radians in range [0,2pi).</returns>
	fp64_t scNormalizeAngle2PiD(fp64_t const x);

	/// <summary>
	/// Calculate sine of angle in radians.
	/// </summary>
	/// <param name="x">Angle in radians.</param>
	/// <returns>Sine of angle.</returns>
	fp64_t scSinrD(fp64_t const x);

	/// <summary>
	/// Calculate arcsine angle in radians.
	/// </summary>
	/// <param name="x">Sine of angle.</param>
	/// <returns>Angle in radians.</returns>
	fp64_t scAsinrD(fp64_t const x);

	/// <summary>
	/// Calculate cosine of angle in radians.
	/// </summary>
	/// <param name="x">Angle in radians.</param>
	/// <returns>Cosine of angle.</returns>
	fp64_t scCosrD(fp64_t const x);

	/// <summary>
	/// Calculate arccosine angle in radians.
	/// </summary>
	/// <param name="x">Cosine of angle.</param>
	/// <returns>Angle in radians.</returns>
	fp64_t scAcosrD(fp64_t const x);

	/// <summary>
	/// Calculate tangent of angle in radians.
	/// </summary>
	/// <param name="x">Angle in radians.</param>
	/// <returns>Tangent of angle.</returns>
	fp64_t scTanrD(fp64_t const x);

	/// <summary>
	/// Calculate arctangent angle in radians.
	/// </summary>
	/// <param name="x">Tangent of angle.</param>
	/// <returns>Angle in radians.</returns>
	fp64_t scAtanrD(fp64_t const x);

	/// <summary>
	/// Calculate two-component arctangent given sine and cosine parts: tan(a) = y/x = sin(a) / cos(a).
	/// </summary>
	/// <param name="y">Numerator of tangent quotient representing sine.</param>
	/// <param name="x">Denominator of tangent quotient representing cosine.</param>
	/// <returns>Angle in radians.</returns>
	fp64_t scAtan2rD(fp64_t const y, fp64_t const x);

	/// <summary>
	/// Calculate sine of angle in degrees.
	/// </summary>
	/// <param name="x">Angle in degrees.</param>
	/// <returns>Sine of angle.</returns>
	fp64_t scSindD(fp64_t const x);

	/// <summary>
	/// Calculate arcsine angle in degrees.
	/// </summary>
	/// <param name="x">Sine of angle.</param>
	/// <returns>Angle in degrees.</returns>
	fp64_t scAsindD(fp64_t const x);

	/// <summary>
	/// Calculate cosine of angle in degrees.
	/// </summary>
	/// <param name="x">Angle in degrees.</param>
	/// <returns>Cosine of angle.</returns>
	fp64_t scCosdD(fp64_t const x);

	/// <summary>
	/// Calculate arccosine angle in degrees.
	/// </summary>
	/// <param name="x">Cosine of angle.</param>
	/// <returns>Angle in degrees.</returns>
	fp64_t scAcosdD(fp64_t const x);

	/// <summary>
	/// Calculate tangent of angle in degrees.
	/// </summary>
	/// <param name="x">Angle in degrees.</param>
	/// <returns>Tangent of angle.</returns>
	fp64_t scTandD(fp64_t const x);

	/// <summary>
	/// Calculate arctangent angle in degrees.
	/// </summary>
	/// <param name="x">Tangent of angle.</param>
	/// <returns>Angle in degrees.</returns>
	fp64_t scAtandD(fp64_t const x);

	/// <summary>
	/// Calculate two-component arctangent given sine and cosine parts: tan(a) = y/x = sin(a) / cos(a).
	/// </summary>
	/// <param name="y">Numerator of tangent quotient representing sine.</param>
	/// <param name="x">Denominator of tangent quotient representing cosine.</param>
	/// <returns>Angle in degrees.</returns>
	fp64_t scAtan2dD(fp64_t const y, fp64_t const x);


	/// <summary>
	/// Random number generator.
	/// </summary>
	/// <returns>Random number in [0,RAND_MAX].</returns>
	fp64_t scRandD();

	/// <summary>
	/// Random unit number generator.
	/// </summary>
	/// <returns>Random number in [0,1).</returns>
	fp64_t scRandUnitD();

	/// <summary>
	/// Random symmetric number generator.
	/// </summary>
	/// <returns>Random number in [-1,+1).</returns>
	fp64_t scRandSymmD();


	/// <summary>
	/// Test if value is approximately zero.
	/// </summary>
	/// <param name="x">Value to test.</param>
	/// <returns>True if value is approximately zero.</returns>
#define scIsZeroApprox cdraw_scalar_base(scIsZeroApprox)

	/// <summary>
	/// Test if value is approximately non-zero.
	/// </summary>
	/// <param name="x">Value to test.</param>
	/// <returns>True if value is approximately non-zero.</returns>
#define scIsNonZeroApprox cdraw_scalar_base(scIsNonZeroApprox)

	/// <summary>
	/// Test if value is approximately positive.
	/// </summary>
	/// <param name="x">Value to test.</param>
	/// <returns>True if value is approximately positive.</returns>
#define scIsPositiveApprox cdraw_scalar_base(scIsPositiveApprox)

	/// <summary>
	/// Test if value is approximately non-positive.
	/// </summary>
	/// <param name="x">Value to test.</param>
	/// <returns>True if value is approximately non-positive.</returns>
#define scIsNonPositiveApprox cdraw_scalar_base(scIsNonPositiveApprox)

	/// <summary>
	/// Test if value is approximately negative.
	/// </summary>
	/// <param name="x">Value to test.</param>
	/// <returns>True if value is approximately negative.</returns>
#define scIsNegativeApprox cdraw_scalar_base(scIsNegativeApprox)

	/// <summary>
	/// Test if value is approximately non-negative.
	/// </summary>
	/// <param name="x">Value to test.</param>
	/// <returns>True if value is approximately non-negative.</returns>
#define scIsNonNegativeApprox cdraw_scalar_base(scIsNonNegativeApprox)


	/// <summary>
	/// Determine sign of input.
	/// </summary>
	/// <param name="x">Input value.</param>
	/// <returns>Sign of input: sgn(x).</returns>
#define scSgn cdraw_scalar_base(scSgn)

	/// <summary>
	/// Determine absolute value of input.
	/// </summary>
	/// <param name="x">Input value.</param>
	/// <returns>Absolute value of input: abs(x).</returns>
#define scAbs cdraw_scalar_base(scAbs)

	/// <summary>
	/// Determine fractional part of input.
	/// </summary>
	/// <param name="x">Input value.</param>
	/// <returns>Fraction of input: frac(x).</returns>
#define scFrac cdraw_scalar_base(scFrac)

	/// <summary>
	/// Safely calculate reciprocal of input.
	/// </summary>
	/// <param name="x">Input value.</param>
	/// <returns>Reciprocal of input if non-zero, otherwise zero.</returns>
#define scRecip cdraw_scalar_base(scRecip)

	/// <summary>
	/// Safely calculate quotient of inputs.
	/// </summary>
	/// <param name="x_num">Input numerator.</param>
	/// <param name="x_den">Input denominator.</param>
	/// <returns>Quotient of inputs if denominator is non-zero, otherwise zero.</returns>
#define scDiv cdraw_scalar_base(scDiv)

	/// <summary>
	/// Safely calculate modulo, or division remainder of inputs.
	/// </summary>
	/// <param name="x_num">Input numerator.</param>
	/// <param name="x_den">Input denominator.</param>
	/// <returns>Remainder of inputs if denominator is non-zero, otherwise numerator.</returns>
#define scMod cdraw_scalar_base(scMod)

	/// <summary>
	/// Calculate linear interpolation value given parameter and range.
	/// </summary>
	/// <param name="u">Interpolation parameter, in [0,1] for interpolation, otherwise extrapolation.</param>
	/// <param name="x_min">Minimum value or lower bound of range.</param>
	/// <param name="x_max">Maximum value or upper bound of range.</param>
	/// <returns>Relative value in range.</returns>
#define scLerp cdraw_scalar_base(scLerp)

	/// <summary>
	/// Calculate linear interpolation parameter given value and range.
	/// </summary>
	/// <param name="x">Value assumed to be in range.</param>
	/// <param name="x_min">Minimum value or lower bound of range.</param>
	/// <param name="x_max">Maximum value or upper bound of range.</param>
	/// <returns>Parameter that would give value if used to interpolate.</returns>
#define scLerpInv cdraw_scalar_base(scLerpInv)

	/// <summary>
	/// Clamp value into given range.
	/// </summary>
	/// <param name="x">Value to clamp.</param>
	/// <param name="x_min">Minimum value or lower bound of range.</param>
	/// <param name="x_max">Maximum value or upper bound of range.</param>
	/// <returns>Value clamped within range.</returns>
#define scClamp cdraw_scalar_base(scClamp)

	/// <summary>
	/// Calculate repeated value in range if it is outside the value.
	/// </summary>
	/// <param name="x">Value to place in range.</param>
	/// <param name="x_min">Minimum value or lower bound of range.</param>
	/// <param name="x_max">Maximum value or upper bound of range.</param>
	/// <returns>Value repeated into range.</returns>
#define scRepeat cdraw_scalar_base(scRepeat)

	/// <summary>
	/// Remap value relative to one range into another range.
	/// </summary>
	/// <param name="x">Value to remap in source range.</param>
	/// <param name="x_min_src">Minimum value or lower bound of source range.</param>
	/// <param name="x_max_src">Maximum value or upper bound of source range.</param>
	/// <param name="x_min_dst">Minimum value or lower bound of destination range.</param>
	/// <param name="x_max_dst">Maximum value or upper bound of destination range.</param>
	/// <returns>Value remapped to destination range.</returns>
#define scRemap cdraw_scalar_base(scRemap)

	/// <summary>
	/// Test if value is in specified range.
	/// </summary>
	/// <param name="x">Value to test.</param>
	/// <param name="x_min">Minimum value or lower bound of range.</param>
	/// <param name="x_max">Maximum value or upper bound of range.</param>
	/// <returns>True if value is within range.</returns>
#define scIsInRange cdraw_scalar_base(scIsInRange)

	/// <summary>
	/// Test if value is in unit range, inclusive: [0,1].
	/// </summary>
	/// <param name="x">Value to test.</param>
	/// <returns>True if value is within range.</returns>
#define scIsInUnitRange cdraw_scalar_base(scIsInUnitRange)

	/// <summary>
	/// Test if value is in symmetric range, inclusive: [-1,+1].
	/// </summary>
	/// <param name="x">Value to test.</param>
	/// <returns>True if value is within range.</returns>
#define scIsInSymmRange cdraw_scalar_base(scIsInSymmRange)


	/// <summary>
	/// Calculate square root of non-negative value.
	/// </summary>
	/// <param name="x">Non-negative input value.</param>
	/// <returns>Square root of input.</returns>
#define scSqrt cdraw_scalar_base(scSqrt)

	/// <summary>
	/// Calculate inverse square root of positive value.
	/// </summary>
	/// <param name="x">Positive input value.</param>
	/// <returns>Inverse square root of input.</returns>
#define scSqrtInv cdraw_scalar_base(scSqrtInv)

	/// <summary>
	/// Calculate cube root of value.
	/// </summary>
	/// <param name="x">Input value.</param>
	/// <returns>Cube root of input.</returns>
#define scCbrt cdraw_scalar_base(scCbrt)

	/// <summary>
	/// Calculate inverse cube root of non-zero value.
	/// </summary>
	/// <param name="x">Non-zero input value.</param>
	/// <returns>Inverse cube root of input.</returns>
#define scCbrtInv cdraw_scalar_base(scCbrtInv)

	/// <summary>
	/// Calculate degree angle in radians.
	/// </summary>
	/// <param name="x">Angle in degrees.</param>
	/// <returns>Angle in radians.</returns>
#define scDeg2Rad cdraw_scalar_base(scDeg2Rad)

	/// <summary>
	/// Calculate radian angle in degrees.
	/// </summary>
	/// <param name="x">Angle in radians.</param>
	/// <returns>Angle in degrees.</returns>
#define scRad2Deg cdraw_scalar_base(scRad2Deg)

	/// <summary>
	/// Calculate equivalent angle in range [-180,+180) degrees.
	/// </summary>
	/// <param name="x">Angle in degrees.</param>
	/// <returns>Angle in degrees in range [-180,+180).</returns>
#define scNormalizeAngle180 cdraw_scalar_base(scNormalizeAngle180)

	/// <summary>
	/// Calculate equivalent angle in range [0,360) degrees.
	/// </summary>
	/// <param name="x">Angle in degrees.</param>
	/// <returns>Angle in degrees in range [0,360).</returns>
#define scNormalizeAngle360 cdraw_scalar_base(scNormalizeAngle360)

	/// <summary>
	/// Calculate equivalent angle in range [-pi,+pi) radians.
	/// </summary>
	/// <param name="x">Angle in radians.</param>
	/// <returns>Angle in radians in range [-pi,+pi).</returns>
#define scNormalizeAnglePi cdraw_scalar_base(scNormalizeAnglePi)

	/// <summary>
	/// Calculate equivalent angle in range [0,2pi) radians.
	/// </summary>
	/// <param name="x">Angle in radians.</param>
	/// <returns>Angle in radians in range [0,2pi).</returns>
#define scNormalizeAngle2Pi cdraw_scalar_base(scNormalizeAngle2Pi)

	/// <summary>
	/// Calculate sine of angle in radians.
	/// </summary>
	/// <param name="x">Angle in radians.</param>
	/// <returns>Sine of angle.</returns>
#define scSinr cdraw_scalar_base(scSinr)

	/// <summary>
	/// Calculate arcsine angle in radians.
	/// </summary>
	/// <param name="x">Sine of angle.</param>
	/// <returns>Angle in radians.</returns>
#define scAsinr cdraw_scalar_base(scAsinr)

	/// <summary>
	/// Calculate cosine of angle in radians.
	/// </summary>
	/// <param name="x">Angle in radians.</param>
	/// <returns>Cosine of angle.</returns>
#define scCosr cdraw_scalar_base(scCosr)

	/// <summary>
	/// Calculate arccosine angle in radians.
	/// </summary>
	/// <param name="x">Cosine of angle.</param>
	/// <returns>Angle in radians.</returns>
#define scAcosr cdraw_scalar_base(scAcosr)

	/// <summary>
	/// Calculate tangent of angle in radians.
	/// </summary>
	/// <param name="x">Angle in radians.</param>
	/// <returns>Tangent of angle.</returns>
#define scTanr cdraw_scalar_base(scTanr)

	/// <summary>
	/// Calculate arctangent angle in radians.
	/// </summary>
	/// <param name="x">Tangent of angle.</param>
	/// <returns>Angle in radians.</returns>
#define scAtanr cdraw_scalar_base(scAtanr)

	/// <summary>
	/// Calculate two-component arctangent given sine and cosine parts: tan(a) = y/x = sin(a) / cos(a).
	/// </summary>
	/// <param name="y">Numerator of tangent quotient representing sine.</param>
	/// <param name="x">Denominator of tangent quotient representing cosine.</param>
	/// <returns>Angle in radians.</returns>
#define scAtan2r cdraw_scalar_base(scAtan2r)

	/// <summary>
	/// Calculate sine of angle in degrees.
	/// </summary>
	/// <param name="x">Angle in degrees.</param>
	/// <returns>Sine of angle.</returns>
#define scSind cdraw_scalar_base(scSind)

	/// <summary>
	/// Calculate arcsine angle in degrees.
	/// </summary>
	/// <param name="x">Sine of angle.</param>
	/// <returns>Angle in degrees.</returns>
#define scAsind cdraw_scalar_base(scAsind)

	/// <summary>
	/// Calculate cosine of angle in degrees.
	/// </summary>
	/// <param name="x">Angle in degrees.</param>
	/// <returns>Cosine of angle.</returns>
#define scCosd cdraw_scalar_base(scCosd)

	/// <summary>
	/// Calculate arccosine angle in degrees.
	/// </summary>
	/// <param name="x">Cosine of angle.</param>
	/// <returns>Angle in degrees.</returns>
#define scAcosd cdraw_scalar_base(scAcosd)

	/// <summary>
	/// Calculate tangent of angle in degrees.
	/// </summary>
	/// <param name="x">Angle in degrees.</param>
	/// <returns>Tangent of angle.</returns>
#define scTand cdraw_scalar_base(scTand)

	/// <summary>
	/// Calculate arctangent angle in degrees.
	/// </summary>
	/// <param name="x">Tangent of angle.</param>
	/// <returns>Angle in degrees.</returns>
#define scAtand cdraw_scalar_base(scAtand)

	/// <summary>
	/// Calculate two-component arctangent given sine and cosine parts: tan(a) = y/x = sin(a) / cos(a).
	/// </summary>
	/// <param name="y">Numerator of tangent quotient representing sine.</param>
	/// <param name="x">Denominator of tangent quotient representing cosine.</param>
	/// <returns>Angle in degrees.</returns>
#define scAtan2d cdraw_scalar_base(scAtan2d)


	/// <summary>
	/// Random number generator.
	/// </summary>
	/// <returns>Random number in [0,RAND_MAX].</returns>
#define scRand cdraw_scalar_base(scRand)

	/// <summary>
	/// Random unit number generator.
	/// </summary>
	/// <returns>Random number in [0,1).</returns>
#define scRandUnit cdraw_scalar_base(scRandUnit)

	/// <summary>
	/// Random symmetric number generator.
	/// </summary>
	/// <returns>Random number in [-1,+1).</returns>
#define scRandSymm cdraw_scalar_base(scRandSymm)


#ifdef __cplusplus
}
#endif // #ifdef __cplusplus


// ****TO-DO: vector/matrix base types - move to vector/matrix
typedef int32_t								int2_t[2], int3_t[3], int4_t[4], * intN_t;
typedef uint32_t							uint2_t[2], uint3_t[3], uint4_t[4], * uintN_t;
typedef bool								bool2_t[2], bool3_t[3], bool4_t[4], * boolN_t;
typedef fp32_t								float2_t[2], float3_t[3], float4_t[4], * floatN_t;
typedef fp64_t								double2_t[2], double3_t[3], double4_t[4], * doubleN_t;
typedef float2_t							float2x2_t[2], * floatNx2;
typedef float3_t							float3x3_t[3], * floatNx3;
typedef float4_t							float4x4_t[4], * floatNx4;
typedef double2_t							double2x2_t[2], * doubleNx2;
typedef double3_t							double3x3_t[3], * doubleNx3;
typedef double4_t							double4x4_t[4], * doubleNx4;


#if !CDRAW_USING_PRECOMPILE_LIBS
#include "_inl/cdrawScalar.inl"
#endif // #if !CDRAW_USING_PRECOMPILE_LIBS


#endif // #ifndef _CDRAW_SCALAR_H_