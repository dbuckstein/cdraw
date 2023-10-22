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
* cdrawUtility.h
* Interface for convenient utilities.
*/

#ifndef _CDRAW_UTILITY_H_
#define _CDRAW_UTILITY_H_


#include "cdraw/cdraw/cdrawConfig.h"


#ifdef __cplusplus
extern "C" {
#endif // #ifdef __cplusplus

	/// <summary>
	/// Count the pointers in a null-terminated pointer array.
	/// </summary>
	/// <param name="arr">Input array</param>
	/// <param name="baseLen">Base length of input array (maximum search).</param>
	/// <returns>Number of elements before null.</returns>
	uint32_t cdrawUtilityPtrCount(ptrk_t const arr[], uint32_t const baseLen);

	/// <summary>
	/// Find index of string in null-terminated array of strings.
	/// </summary>
	/// <param name="key">Valid search string.</param>
	/// <param name="arr">Input array.</param>
	/// <param name="baseLen">Base length of input array (maximum search).</param>
	/// <returns>Index of key string if found; -1 if not found.</returns>
	int32_t cdrawUtilityStrFind(cstrk_t const key, cstrk_t const arr[], uint32_t const baseLen);

	/// <summary>
	/// Reduce numerator and denominator for quotient by computing greatest common denominator (GCD).
	/// </summary>
	/// <param name="numerator">Original numerator.</param>
	/// <param name="denominator">Original denominator.</param>
	/// <param name="numerator_out">Pointer to storage of resulting reduced numerator.</param>
	/// <param name="denominator_out">Pointer to storage of resulting reduced denominator.</param>
	/// <param name="gcd_out_opt">Optional pointer to storage of GCD.</param>
	/// <returns>Integer component of quotient.</returns>
	int64_t cdrawUtilityReduceRationalGCD(int64_t numerator, int64_t denominator, int64_t* const numerator_out, int64_t* const denominator_out, int64_t* const gcd_out_opt);


#ifdef __cplusplus
}
#endif // #ifdef __cplusplus


#endif // #ifndef _CDRAW_UTILITY_H_