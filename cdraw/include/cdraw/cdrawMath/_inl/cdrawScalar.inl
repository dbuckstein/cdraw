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
* cdrawScalar.inl
* Definitions of scalar methods.
*/

#ifndef _CDRAW_SCALAR_INL_
#define _CDRAW_SCALAR_INL_
#ifdef _CDRAW_SCALAR_H_


CDRAW_INL bool scIsZeroApproxF(fp32_t const x)
{
	return (x <= +FLT_EPSILON && x >= -FLT_EPSILON);
}

CDRAW_INL bool scIsNonZeroApproxF(fp32_t const x)
{
	return (x > +FLT_EPSILON || x < -FLT_EPSILON);
}

CDRAW_INL bool scIsPositiveApproxF(fp32_t const x)
{
	return (x > +FLT_EPSILON);
}

CDRAW_INL bool scIsNonPositiveApproxF(fp32_t const x)
{
	return (x <= +FLT_EPSILON);
}

CDRAW_INL bool scIsNegativeApproxF(fp32_t const x)
{
	return (x < -FLT_EPSILON);
}

CDRAW_INL bool scIsNonNegativeApproxF(fp32_t const x)
{
	return (x >= -FLT_EPSILON);
}

CDRAW_INL bool scIsInRangeF(fp32_t const x, fp32_t const x_min, fp32_t const x_max)
{
	return (x >= x_min && x <= x_max);
}

CDRAW_INL bool scIsInUnitRangeF(fp32_t const x)
{
	return (x >= sc0F && x <= sc1F);
}

CDRAW_INL bool scIsInSymmRangeF(fp32_t const x)
{
	return (x <= +sc1F && x >= -sc1F);
}


CDRAW_INL fp32_t scSgnF(fp32_t const x)
{
	return (scIsNonPositiveApproxF(x) ? scIsNegativeApproxF(x) ? -sc1F : sc0F : +sc1F);
}

CDRAW_INL fp32_t scAbsF(fp32_t const x)
{
	return (scIsNonPositiveApproxF(x) ? scIsNegativeApproxF(x) ? -x : sc0F : +x);
}

CDRAW_INL fp32_t scFracF(fp32_t const x)
{
	return (x - floorf(x));
}

CDRAW_INL fp32_t scRecipF(fp32_t const x)
{
	failassertret(scIsNonZeroApproxF(x), sc0F);
	return (sc1F / x);
}

CDRAW_INL fp32_t scDivF(fp32_t const x_num, fp32_t x_den)
{
	failassertret(scIsNonZeroApproxF(x_den), sc0F);
	return (x_num / x_den);
}

CDRAW_INL fp32_t scModF(fp32_t const x_num, fp32_t const x_den)
{
	failassertret(scIsNonZeroApproxF(x_den), x_num);
	return (x_num - x_den * floorf(x_num / x_den));
}

CDRAW_INL fp32_t scLerpF(fp32_t const u, fp32_t const x_min, fp32_t const x_max)
{
	return (x_min + (x_max - x_min) * u);
}

CDRAW_INL fp32_t scLerpInvF(fp32_t const x, fp32_t const x_min, fp32_t const x_max)
{
	return scDivF(x - x_min, x_max - x_min);
}

CDRAW_INL fp32_t scClampF(fp32_t const x, fp32_t const x_min, fp32_t const x_max)
{
	return (x >= x_min ? x <= x_max ? x : x_max : x_min);
}

CDRAW_INL fp32_t scRepeatF(fp32_t const x, fp32_t const x_min, fp32_t const x_max)
{
	fp32_t const rng = x_max - x_min;
	failassertret(scIsNonZeroApproxF(rng), x);
	return (x - rng * floorf((x - x_min) / rng));
}

CDRAW_INL fp32_t scRemapF(fp32_t const x, fp32_t const x_min_src, fp32_t const x_max_src, fp32_t const x_min_dst, fp32_t const x_max_dst)
{
	fp32_t const rng_src = x_max_src - x_min_src;
	failassertret(scIsNonZeroApproxF(rng_src), x_min_dst);
	return (x_min_dst + (x_max_dst - x_min_dst) * (x - x_min_src) / rng_src);
}


CDRAW_INL fp32_t scSqrtF(fp32_t const x)
{
	failassertret(x >= sc0F, sc0F);
	return (scIsPositiveApproxF(x) ? sqrtf(x) : sc0F);
}

CDRAW_INL fp32_t scSqrtInvF(fp32_t const x)
{
	failassertret(scIsPositiveApproxF(x), sc0F);
	return (sc1F / sqrtf(x));
}

CDRAW_INL fp32_t scCbrtF(fp32_t const x)
{
	return (scIsNonZeroApproxF(x) ? cbrtf(x) : sc0F);
}

CDRAW_INL fp32_t scCbrtInvF(fp32_t const x)
{
	failassertret(scIsNonZeroApproxF(x), sc0F);
	return (sc1F / cbrtf(x));
}

CDRAW_INL fp32_t scDeg2RadF(fp32_t const x)
{
	return (x * scPiF / sc180F);
}

CDRAW_INL fp32_t scRad2DegF(fp32_t const x)
{
	return (x * sc180F / scPiF);
}

CDRAW_INL fp32_t scSinrF(fp32_t const x)
{
	return sinf(x);
}

CDRAW_INL fp32_t scAsinrF(fp32_t const x)
{
	failassertret(scIsInSymmRangeF(x), sc0F);
	return asinf(x);
}

CDRAW_INL fp32_t scCosrF(fp32_t const x)
{
	return cosf(x);
}

CDRAW_INL fp32_t scAcosrF(fp32_t const x)
{
	failassertret(scIsInSymmRangeF(x), sc1F);
	return acosf(x);
}

CDRAW_INL fp32_t scTanrF(fp32_t const x)
{
	return tanf(x);
}

CDRAW_INL fp32_t scAtanrF(fp32_t const x)
{
	return atanf(x);
}

CDRAW_INL fp32_t scAtan2rF(fp32_t const y, fp32_t const x)
{
	return atan2f(y, x);
}

CDRAW_INL fp32_t scSindF(fp32_t const x)
{
	return sinf(scDeg2RadF(x));
}

CDRAW_INL fp32_t scAsindF(fp32_t const x)
{
	failassertret(scIsInSymmRangeF(x), sc0F);
	return scRad2DegF(asinf(x));
}

CDRAW_INL fp32_t scCosdF(fp32_t const x)
{
	return cosf(scDeg2RadF(x));
}

CDRAW_INL fp32_t scAcosdF(fp32_t const x)
{
	failassertret(scIsInSymmRangeF(x), sc1F);
	return scRad2DegF(acosf(x));
}

CDRAW_INL fp32_t scTandF(fp32_t const x)
{
	return tanf(scDeg2RadF(x));
}

CDRAW_INL fp32_t scAtandF(fp32_t const x)
{
	return scRad2DegF(atanf(x));
}

CDRAW_INL fp32_t scAtan2dF(fp32_t const y, fp32_t const x)
{
	return scRad2DegF(atan2f(y, x));
}


CDRAW_INL fp32_t scRandF()
{
	return (fp32_t)(rand());
}

CDRAW_INL fp32_t scRandUnitF()
{
	return (fp32_t)(rand()) * 0.000030517578125f;
}

CDRAW_INL fp32_t scRandSymmF()
{
	return (fp32_t)(rand() - 16384) * 0.00006103515625f;
}




CDRAW_INL fp64_t scRandD()
{
	return (fp64_t)(rand());
}

CDRAW_INL fp64_t scRandUnitD()
{
	return (fp64_t)(rand()) * 0.000030517578125;
}

CDRAW_INL fp64_t scRandSymmD()
{
	return (fp64_t)(rand() - 16384) * 0.00006103515625;
}


#endif // #ifdef _CDRAW_SCALAR_H_
#endif // #ifndef _CDRAW_SCALAR_INL_