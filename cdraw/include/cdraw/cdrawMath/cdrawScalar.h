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

#ifndef CDRAW_SCALAR_PREF
// Describes whether using preferred base scalar type.
// Should be defined by includer outside of this file as one of the following: 
//	-CDRAW_ENABLE if requesting preferred scalar type.
//	-CDRAW_DISABLE if requesting no preferred scalar type.
#define CDRAW_SCALAR_PREF				CDRAW_DISABLE
#endif // #ifndef CDRAW_SCALAR_PREF
#define CDRAW_USING_SCALAR_PREF			cdraw_enabled(CDRAW_SCALAR_PREF) && !CDRAW_USING_PRECOMPILE_LIBS

#ifndef CDRAW_SCALAR_PREF_DOUBLE
// Describes whether double-precision float is desired for base scalar type.
// Should be defined by includer outside of this file as one of the following: 
//	-CDRAW_ENABLE if requesting base scalar type use double-precision (double).
//	-CDRAW_DISABLE if requesting base scalar type use single-precision (float).
#define CDRAW_SCALAR_PREF_DOUBLE		CDRAW_DISABLE
#endif // #ifndef CDRAW_SCALAR_PREF_DOUBLE
#define CDRAW_USING_SCALAR_PREF_DOUBLE	cdraw_enabled(CDRAW_SCALAR_PREF_DOUBLE) && CDRAW_USING_SCALAR_PREF


#define cdraw_scalar_base(name)								tokencat(name, scalar_suffix)
#define cdraw_scalar_declDF(name,value_fp64)				\
		fp64_t tokencat(name, D) = value_fp64;				\
		fp32_t tokencat(name, F) = tokencat(value_fp64, f)
#define cdraw_scalar_declIU(name,value_int)					\
		 int32_t tokencat(name, I) = value_int;				\
		uint32_t tokencat(name, U) = tokencat(value_int, u)

#define scEpsD		(DBL_EPSILON)				// Standard epsilon ("smallest value") for double-precision.
#define scBigEpsD	(DBL_EPSILON * 64.0)		// Bigger epsilon for double-precision (32x covers inverse trigonometry).
#define scEpsG1D	(1.0 + scEpsD)				// Smallest value greater than 1.
#define scEpsL1D	(1.0 - scEpsD)				// Smallest value less than 1.
#define scBigEpsG1D	(1.0 + scBigEpsD)			// Smallest bigger value greater than 1.
#define scBigEpsL1D	(1.0 - scBigEpsD)			// Smallest bigger value less than 1.
#define scEpsF		(FLT_EPSILON)				// Standard epsilon ("smallest value") for single-precision.
#define scBigEpsF	(FLT_EPSILON * 64.0f)		// Bigger epsilon for single-precision (32x covers inverse trigonometry).
#define scEpsG1F	(1.0f + scEpsF)				// Smallest value greater than 1.
#define scEpsL1F	(1.0f - scEpsF)				// Smallest value less than 1.
#define scBigEpsG1F	(1.0f + scBigEpsF)			// Smallest bigger value greater than 1.
#define scBigEpsL1F	(1.0f - scBigEpsF)			// Smallest bigger value less than 1.


#if CDRAW_USING_SCALAR_PREF
#if CDRAW_USING_SCALAR_PREF_DOUBLE
typedef fp64_t			scalar_t;	// Global scalar number representation (double-precision).
#define scalar_suffix	D
#define const_suffix	
#define SC_EPSILON		(DBL_EPSILON)
#else // #if CDRAW_USING_SCALAR_PREF_DOUBLE
typedef fp32_t			scalar_t;	// Global scalar number representation (single-precision).
#define scalar_suffix	F
#define const_suffix	f
#define SC_EPSILON		(FLT_EPSILON)
#endif // #else // #if CDRAW_USING_SCALAR_PREF_DOUBLE
#define cdraw_scalar_decl(name,value_fp64)					\
		cdraw_scalar_declDF(name, value_fp64);				\
		scalar_t name = tokencat(value_fp64, const_suffix)
#define scEps		cdraw_scalar_base(scEps)		// Standard epsilon ("smallest value") for preferred precision.
#define scBigEps	cdraw_scalar_base(scBigEps)		// Bigger epsilon for preferred precision (32x covers inverse trigonometry).
#define scEpsG1		cdraw_scalar_base(scEpsG1)		// Smallest value greater than 1.
#define scEpsL1		cdraw_scalar_base(scEpsL1)		// Smallest value less than 1.
#define scBigEpsG1	cdraw_scalar_base(scBigEpsG1)	// Smallest bigger value greater than 1.
#define scBigEpsL1	cdraw_scalar_base(scBigEpsL1)	// Smallest bigger value less than 1.
#else // #if CDRAW_USING_SCALAR_PREF
#define cdraw_scalar_decl(name,value_fp64)					\
		cdraw_scalar_declDF(name, value_fp64)
#endif // #else // #if CDRAW_USING_SCALAR_PREF
#define cdraw_scalar_const(name,value_fp64)					cdraw_scalar_decl(static const name, value_fp64)


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


/******************************************************************************
* Functions.
******************************************************************************/

#include "cdrawScalar/cdrawScF.h"
#include "cdrawScalar/cdrawScD.h"

#if CDRAW_USING_SCALAR_PREF
#include "cdrawScalar/cdrawSc.h"
#endif // #if CDRAW_USING_SCALAR_PREF


#define gDiv(x_num,x_den)					((x_num) / (x_den))																			// Compute general quotient.
#define gMod(x_num,x_den)					((x_num) % (x_den))																			// Compute general modulo.
#define gModF(x_num,x_den)					((x_num) - (x_den) * floorf(gDiv(x_num, x_den)))											// Compute general modulo for single-precision float.
#define gModD(x_num,x_den)					((x_num) - (x_den) * floor(gDiv(x_num, x_den)))												// Compute general modulo for double-precision float.
#define gDivMod(x_mod,x_num,x_den)			(((x_mod) = gMod(x_num, x_den)), gDiv(x_num, x_den))										// Compute general quotient and modulo.
#define gDivModF(x_mod,x_num,x_den)			(((x_mod) = gModF(x_num, x_den)), gDiv(x_num, x_den))										// Compute general quotient and modulo for single-precision float.
#define gDivModD(x_mod,x_num,x_den)			(((x_mod) = gModD(x_num, x_den)), gDiv(x_num, x_den))										// Compute general quotient and modulo for double-precision float.
#define gDivSafe(x_num,x_den)				((x_den) ? gDiv(x_num, x_den) : 0)															// Compute general safe quotient.
#define gDivSafeF(x_num,x_den)				(scIsNonZeroApproxF(x_den) ? gDiv(x_num, x_den) : sc0F)										// Compute safe quotient for single-precision float.
#define gDivSafeD(x_num,x_den)				(scIsNonZeroApproxD(x_den) ? gDiv(x_num, x_den) : sc0D)										// Compute safe quotient for double-precision float.
#define gModSafe(x_num,x_den)				((x_den) ? gMod(x_num, x_den) : (x_num))													// Compute general safe modulo.
#define gModSafeF(x_num,x_den)				(scIsNonZeroApproxF(x_den) ? gModF(x_num, x_den) : (x_num))									// Compute safe modulo for single-precision float.
#define gModSafeD(x_num,x_den)				(scIsNonZeroApproxD(x_den) ? gModD(x_num, x_den) : (x_num))									// Compute safe modulo for double-precision float.
#define gDivModSafe(x_mod,x_num,x_den)		((x_den) ? gDivMod(x_mod, x_num, x_den) : (((x_mod) = (x_num)), 0))							// Compute general safe quotient and modulo.
#define gDivModSafeF(x_mod,x_num,x_den)		(scIsNonZeroApproxF(x_den) ? gDivModF(x_mod, x_num, x_den) : (((x_mod) = (x_num)), sc0F))	// Compute safe quotient and modulo for single-precision float.
#define gDivModSafeD(x_mod,x_num,x_den)		(scIsNonZeroApproxD(x_den) ? gDivModD(x_mod, x_num, x_den) : (((x_mod) = (x_num)), sc0D))	// Compute safe quotient and modulo for double-precision float.
#define gModQF(x_num,x_den,x_quo)			((x_num) - (x_den) * floorf(x_quo))															// Compute optimized modulo for single-precision float.
#define gModQD(x_num,x_den,x_quo)			((x_num) - (x_den) * floor(x_quo))															// Compute optimized modulo for double-precision float.
#define gSafeSqrtF(x)						(sqrtf(x))																					// Compute safe (non-negative input) square root for single-precision float.
#define gSafeSqrtD(x)						(sqrt(x))																					// Compute safe (non-negative input) square root for double-precision float.
#define gSafeCbrtF(x)						(cbrtf(x))																					// Compute safe (any input) cube root for single-precision float.
#define gSafeCbrtD(x)						(cbrt(x))																					// Compute safe (any input) cube root for double-precision float.
#define gSafeSqrtInvF(x)					(sc1F / gSafeSqrtF(x))																		// Compute safe (positive input) square root inverse for single-precision float.
#define gSafeSqrtInvD(x)					(sc1D / gSafeSqrtD(x))																		// Compute safe (positive input) square root inverse for double-precision float.
#define gSafeCbrtInvF(x)					(sc1F / gSafeCbrtF(x))																		// Compute safe (non-zero input) cube root inverse for single-precision float.
#define gSafeCbrtInvD(x)					(sc1D / gSafeCbrtD(x))																		// Compute safe (non-zero input) cube root inverse for double-precision float.


#if !CDRAW_USING_PRECOMPILE_LIBS
#include "_inl/cdrawScalar.inl"
#endif // #if !CDRAW_USING_PRECOMPILE_LIBS


#endif // #ifndef _CDRAW_SCALAR_H_