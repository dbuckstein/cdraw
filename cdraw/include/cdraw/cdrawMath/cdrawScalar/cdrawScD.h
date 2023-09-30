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
* cdrawScD.h
* Declarations of double-precision scalar functions.
*/

#if (!(defined _CDRAW_SCD_H_) && (defined _CDRAW_SCALAR_H_))
#define _CDRAW_SCD_H_


#ifdef __cplusplus
extern "C" {
#endif // #ifdef __cplusplus

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
	/// Calculate multiply-add given origin and delta to scale by some parameter.
	/// </summary>
	/// <param name="u">Scaling parameter, in [0,1] for some result in [x_origin,x_origin+x_delta].</param>
	/// <param name="x_origin">Known value in range.</param>
	/// <param name="x_delta">Known change in range.</param>
	/// <returns>Sum of origin and scaled delta.</returns>
	fp64_t scMadD(fp64_t const u, fp64_t const x_origin, fp64_t const x_delta);

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


#ifdef __cplusplus
}
#endif // #ifdef __cplusplus


#endif // #if (!(defined _CDRAW_SCD_H_) && (defined _CDRAW_SCALAR_H_))