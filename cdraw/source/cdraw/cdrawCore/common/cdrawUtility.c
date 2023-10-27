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
* cdrawUtility.c
* Utility implementations.
*/

#include "cdraw/cdrawCore/cdrawUtility.h"

#include <string.h>


uint32_t cdrawUtilityPtrCount(ptrk_t const arr[], uint32_t const baseLen)
{
	uint32_t n = 0;
	while (*(arr++) && (++n < baseLen));
	return n;
}

int32_t cdrawUtilityStrFind(cstrk_t const key, cstrk_t const arr[], uint32_t const baseLen)
{
	uint32_t n = 0;
	while (*(arr) && (n < baseLen))
	{
		if (strcmp(key, *arr) == 0)
			return n;
		++arr;
		++n;
	}
	return -1;
}

int64_t cdrawUtilityReduceRationalGCD(int64_t numerator, int64_t denominator, int64_t* const numerator_out, int64_t* const denominator_out, int64_t* const gcd_out_opt)
{
	failassertret(numerator_out && denominator_out, 0);

	int64_t i = 0, r = denominator, n = numerator, d = 0;

	// iterate to find GCD
	while (r)
	{
		d = r;		// previous remainder becomes denominator
		r = n % d;	// calculate new remainder
		n = d;		// previous denominator becomes numerator
	}

	// d now holds the GCD
	if (d)
	{
		// reduce fraction
		numerator /= d;
		denominator /= d;

		// calculate integer and remainder
		i = numerator / denominator;
		numerator %= denominator;
	}

	// store final numerator and denominator, return integer
	*numerator_out = numerator;
	*denominator_out = denominator;
	if (gcd_out_opt)
		*gcd_out_opt = d;
	return i;
}

int8_t cdrawUtilityLowestBit32(uint32_t x)
{
	int8_t i = 0;
	if (x == 0)
		return -1;
	if ((x & 0xFFFF) == 0) { i += 16; x >>= 16; }
	if ((x & 0x00FF) == 0) { i +=  8; x >>=  8; }
	if ((x & 0x000F) == 0) { i +=  4; x >>=  4; }
	if ((x & 0x0003) == 0) { i +=  2; x >>=  2; }
	return ((x & 1) == 0 ? i + 1 : i );
}
