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
* cdrawScD.inl
* Definitions of double-precision scalar functions.
*/

#if (!(defined _CDRAW_SCD_INL_) && (defined _CDRAW_SCALAR_INL_))
#define _CDRAW_SCD_INL_


CDRAW_INL bool scIsZeroApproxD(fp64_t const x)
{
	return (x <= +scEpsD && x >= -scEpsD);
}

CDRAW_INL bool scIsNonZeroApproxD(fp64_t const x)
{
	return (x > +scEpsD || x < -scEpsD);
}

CDRAW_INL bool scIsPositiveApproxD(fp64_t const x)
{
	return (x > +scEpsD);
}

CDRAW_INL bool scIsNonPositiveApproxD(fp64_t const x)
{
	return (x <= +scEpsD);
}

CDRAW_INL bool scIsNegativeApproxD(fp64_t const x)
{
	return (x < -scEpsD);
}

CDRAW_INL bool scIsNonNegativeApproxD(fp64_t const x)
{
	return (x >= -scEpsD);
}


CDRAW_INL fp64_t scSgnD(fp64_t const x)
{
	return (scIsNonPositiveApproxD(x) ? scIsNegativeApproxD(x) ? -sc1D : sc0D : +sc1D);
}

CDRAW_INL fp64_t scAbsD(fp64_t const x)
{
	return (scIsNonPositiveApproxD(x) ? scIsNegativeApproxD(x) ? -x : sc0D : +x);
}

CDRAW_INL fp64_t scFracD(fp64_t const x)
{
	return (x - floor(x));
}

CDRAW_INL fp64_t scRecipD(fp64_t const x)
{
	failassertret(scIsNonZeroApproxD(x), sc0D);
	return (sc1D / x);
}

CDRAW_INL fp64_t scDivD(fp64_t const x_num, fp64_t x_den)
{
	failassertret(scIsNonZeroApproxD(x_den), sc0D);
	return (x_num / x_den);
}

CDRAW_INL fp64_t scModD(fp64_t const x_num, fp64_t const x_den)
{
	failassertret(scIsNonZeroApproxD(x_den), x_num);
	return (x_num - x_den * floor(x_num / x_den));
}

CDRAW_INL fp64_t scMadD(fp64_t const u, fp64_t const x_origin, fp64_t const x_delta)
{
	return (x_origin + x_delta * u);
}

CDRAW_INL fp64_t scLerpD(fp64_t const u, fp64_t const x_min, fp64_t const x_max)
{
	return (x_min + (x_max - x_min) * u);
}

CDRAW_INL fp64_t scLerpInvD(fp64_t const x, fp64_t const x_min, fp64_t const x_max)
{
	return scDivD(x - x_min, x_max - x_min);
}

CDRAW_INL fp64_t scClampD(fp64_t const x, fp64_t const x_min, fp64_t const x_max)
{
	return (x >= x_min ? x <= x_max ? x : x_max : x_min);
}

CDRAW_INL fp64_t scRepeatD(fp64_t const x, fp64_t const x_min, fp64_t const x_max)
{
	fp64_t const rng = x_max - x_min;
	failassertret(scIsNonZeroApproxD(rng), x);
	return (x - rng * floor((x - x_min) / rng));
}

CDRAW_INL fp64_t scRemapD(fp64_t const x, fp64_t const x_min_src, fp64_t const x_max_src, fp64_t const x_min_dst, fp64_t const x_max_dst)
{
	fp64_t const rng_src = x_max_src - x_min_src;
	failassertret(scIsNonZeroApproxD(rng_src), x_min_dst);
	return (x_min_dst + (x_max_dst - x_min_dst) * (x - x_min_src) / rng_src);
}

CDRAW_INL bool scIsInRangeD(fp64_t const x, fp64_t const x_min, fp64_t const x_max)
{
	return (x >= x_min && x <= x_max);
}

CDRAW_INL bool scIsInUnitRangeD(fp64_t const x)
{
	return (x >= sc0D && x <= sc1D);
}

CDRAW_INL bool scIsInSymmRangeD(fp64_t const x)
{
	return (x <= +sc1D && x >= -sc1D);
}


CDRAW_INL fp64_t scSqrtD(fp64_t const x)
{
	failassertret(x >= sc0D, sc0D);
	return (scIsPositiveApproxD(x) ? sqrt(x) : sc0D);
}

CDRAW_INL fp64_t scSqrtInvD(fp64_t const x)
{
	failassertret(scIsPositiveApproxD(x), sc0D);
	return (sc1D / sqrt(x));
}

CDRAW_INL fp64_t scCbrtD(fp64_t const x)
{
	return (scIsNonZeroApproxD(x) ? cbrt(x) : sc0D);
}

CDRAW_INL fp64_t scCbrtInvD(fp64_t const x)
{
	failassertret(scIsNonZeroApproxD(x), sc0D);
	return (sc1D / cbrt(x));
}

CDRAW_INL fp64_t scDeg2RadD(fp64_t const x)
{
	return (x * scPiD / sc180D);
}

CDRAW_INL fp64_t scRad2DegD(fp64_t const x)
{
	return (x * sc180D / scPiD);
}

CDRAW_INL fp64_t scNormalizeAngle180D(fp64_t const x)
{
	return (x - sc360D * floor((x + sc180D) / sc360D));
}

CDRAW_INL fp64_t scNormalizeAngle360D(fp64_t const x)
{
	return (x - sc360D * floor(x / sc360D));
}

CDRAW_INL fp64_t scNormalizeAnglePiD(fp64_t const x)
{
	return scDeg2RadD(scNormalizeAngle180D(scRad2DegD(x)));
}

CDRAW_INL fp64_t scNormalizeAngle2PiD(fp64_t const x)
{
	return scDeg2RadD(scNormalizeAngle360D(scRad2DegD(x)));
}

CDRAW_INL fp64_t scSinrD(fp64_t const x)
{
	return sin(x);
}

CDRAW_INL fp64_t scAsinrD(fp64_t const x)
{
	failassertret(scIsInSymmRangeD(x), sc0D);
	return asin(x);
}

CDRAW_INL fp64_t scCosrD(fp64_t const x)
{
	return cos(x);
}

CDRAW_INL fp64_t scAcosrD(fp64_t const x)
{
	failassertret(scIsInSymmRangeD(x), sc1D);
	return acos(x);
}

CDRAW_INL fp64_t scTanrD(fp64_t const x)
{
	fp64_t const c = cos(x);
	failassertret(scIsNonZeroApproxD(c), sc0D);
	return (sin(x) / c);
}

CDRAW_INL fp64_t scAtanrD(fp64_t const x)
{
	return atan(x);
}

CDRAW_INL fp64_t scAtan2rD(fp64_t const y, fp64_t const x)
{
	failassertret(scIsNonZeroApproxD(y) || scIsNonZeroApproxD(x), sc0D);
	return atan2(y, x);
}

CDRAW_INL fp64_t scSindD(fp64_t const x)
{
	return sin(scDeg2RadD(x));
}

CDRAW_INL fp64_t scAsindD(fp64_t const x)
{
	failassertret(scIsInSymmRangeD(x), sc0D);
	return scRad2DegD(asin(x));
}

CDRAW_INL fp64_t scCosdD(fp64_t const x)
{
	return cos(scDeg2RadD(x));
}

CDRAW_INL fp64_t scAcosdD(fp64_t const x)
{
	failassertret(scIsInSymmRangeD(x), sc1D);
	return scRad2DegD(acos(x));
}

CDRAW_INL fp64_t scTandD(fp64_t const x)
{
	fp64_t const xr = scDeg2RadD(x), c = cos(xr);
	failassertret(scIsNonZeroApproxD(c), sc0D);
	return (sin(xr) / c);
}

CDRAW_INL fp64_t scAtandD(fp64_t const x)
{
	return scRad2DegD(atan(x));
}

CDRAW_INL fp64_t scAtan2dD(fp64_t const y, fp64_t const x)
{
	failassertret(scIsNonZeroApproxD(y) || scIsNonZeroApproxD(x), sc0F);
	return scRad2DegD(atan2(y, x));
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


#endif // #if (!(defined _CDRAW_SCD_INL_) && (defined _CDRAW_SCALAR_INL_))