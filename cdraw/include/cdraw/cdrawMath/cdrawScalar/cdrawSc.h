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
* cdrawSc.h
* Declarations of preferred floating point precision scalar functions.
*/

#if (!(defined _CDRAW_SC_H_) && (defined _CDRAW_SCALAR_H_))
#define _CDRAW_SC_H_


#ifdef __cplusplus
extern "C" {
#endif // #ifdef __cplusplus

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
	/// Test if value is approximately unity (+1).
	/// </summary>
	/// <param name="x">Value to test.</param>
	/// <returns>True if value is approximately unity.</returns>
#define scIsUnityApprox cdraw_scalar_base(scIsUnityApprox)

	/// <summary>
	/// Test if value is approximately non-unity (+1).
	/// </summary>
	/// <param name="x">Value to test.</param>
	/// <returns>True if value is approximately non-unity.</returns>
#define scIsNonUnityApprox cdraw_scalar_base(scIsNonUnityApprox)


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
	/// Safely calculate quotient and modulo of inputs.
	/// </summary>
	/// <param name="x_mod_out">Pointer to resulting modulo if denominator is non-zero, otherwise numerator.</param>
	/// <param name="x_num">Input numerator.</param>
	/// <param name="x_den">Input denominator.</param>
	/// <returns>Quotient of inputs if denominator is non-zero, otherwise zero.</returns>
#define scDivMod cdraw_scalar_base(scDivMod)

	/// <summary>
	/// Calculate multiply-add given origin and delta to scale by some parameter.
	/// </summary>
	/// <param name="u">Scaling parameter, in [0,1] for some result in [x_origin,x_origin+x_delta].</param>
	/// <param name="x_origin">Known value in range.</param>
	/// <param name="x_delta">Known change in range.</param>
	/// <returns>Sum of origin and scaled delta.</returns>
#define scMad cdraw_scalar_base(scMad)

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
	/// Calculate simultaneous sine and cosine of angle in radians.
	/// </summary>
	/// <param name="sin_out">Pointer to sine result storage.</param>
	/// <param name="cos_out">Pointer to cosine result storage.</param>
	/// <param name="x">Angle in radians.</param>
	/// <returns><paramref name="x"/></returns>
#define scSinCosr cdraw_scalar_base(scSinCosr)

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
	/// Calculate simultaneous sine and cosine of angle in degrees.
	/// </summary>
	/// <param name="sin_out">Pointer to sine result storage.</param>
	/// <param name="cos_out">Pointer to cosine result storage.</param>
	/// <param name="x">Angle in degrees.</param>
	/// <returns><paramref name="x"/></returns>
#define scSinCosd cdraw_scalar_base(scSinCosd)

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


#endif // #if (!(defined _CDRAW_SC_H_) && (defined _CDRAW_SCALAR_H_))