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
* cdrawScF.h
* Declarations of single-precision scalar functions.
*/

#if (!(defined _CDRAW_SCF_H_) && (defined _CDRAW_SCALAR_H_))
#define _CDRAW_SCF_H_


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
	/// Safely calculate quotient and modulo of inputs.
	/// </summary>
	/// <param name="x_mod_out">Pointer to resulting modulo if denominator is non-zero, otherwise numerator.</param>
	/// <param name="x_num">Input numerator.</param>
	/// <param name="x_den">Input denominator.</param>
	/// <returns>Quotient of inputs if denominator is non-zero, otherwise zero.</returns>
	fp32_t scDivModF(fp32_t* x_mod_out, fp32_t const x_num, fp32_t const x_den);

	/// <summary>
	/// Calculate multiply-add given origin and delta to scale by some parameter.
	/// </summary>
	/// <param name="u">Scaling parameter, in [0,1] for some result in [x_origin,x_origin+x_delta].</param>
	/// <param name="x_origin">Known value in range.</param>
	/// <param name="x_delta">Known change in range.</param>
	/// <returns>Sum of origin and scaled delta.</returns>
	fp32_t scMadF(fp32_t const u, fp32_t const x_origin, fp32_t const x_delta);

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
	/// Calculate simultaneous sine and cosine of angle in radians.
	/// </summary>
	/// <param name="sin_out">Pointer to sine result storage.</param>
	/// <param name="cos_out">Pointer to cosine result storage.</param>
	/// <param name="x">Angle in radians.</param>
	/// <returns><paramref name="x"/></returns>
	fp32_t scSinCosrF(fp32_t* sin_out, fp32_t* cos_out, fp32_t const x);

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
	/// Calculate simultaneous sine and cosine of angle in degrees.
	/// </summary>
	/// <param name="sin_out">Pointer to sine result storage.</param>
	/// <param name="cos_out">Pointer to cosine result storage.</param>
	/// <param name="x">Angle in degrees.</param>
	/// <returns><paramref name="x"/></returns>
	fp32_t scSinCosdF(fp32_t* sin_out, fp32_t* cos_out, fp32_t const x);

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


#ifdef __cplusplus
}
#endif // #ifdef __cplusplus


#endif // #if (!(defined _CDRAW_SCF_H_) && (defined _CDRAW_SCALAR_H_))