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

#ifndef CDRAW_SCALAR_DOUBLE
// Describes whether double-precision float is desired for base scalar type.
// Should be defined by includer outside of this file as one of the following: 
//	-CDRAW_ENABLE if requesting base scalar type use double-precision (double).
//	-CDRAW_DISABLE if requesting base scalar type use single-precision (float).
#define CDRAW_SCALAR_DOUBLE			CDRAW_DISABLE
#endif // #ifndef CDRAW_SCALAR_DOUBLE

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
#define scBigEpsD	(DBL_EPSILON * 32.0)
#define scBigEpsF	(FLT_EPSILON * 32.0f)
#define scBigEps	cdraw_scalar_base(scBigEps)

#define gSq(x)							((x) * (x))																				// Compute general square of input.
#define gDivSafe(x_num,x_den)			((x_den) ? (x_num) / (x_den) : 0)														// Compute general safe quotient.
#define gModSafe(x_num,x_den)			((x_den) ? (x_num) % (x_den) : (x_num))													// Compute general safe modulo.
#define gDivModSafe(x_mod,x_num,x_den)	((x_den) ? ((x_mod) = (x_num) % (x_den), (x_num) / (x_den)) : ((x_mod) = (x_num), 0))	// Compute general safe quotient and modulo.
#define gMad(x0,dx,u)					((x0) + (dx) * (u))																		// Compute general multiply-add.
#define gLerp(x0,x1,u)					((x0) + ((x1) - (x0)) * u)																// Compute general linear interpolation.


/******************************************************************************
* Functions.
******************************************************************************/

#include "cdrawScalar/cdrawScF.h"
#include "cdrawScalar/cdrawScD.h"
#include "cdrawScalar/cdrawSc.h"


#if !CDRAW_USING_PRECOMPILE_LIBS
#include "_inl/cdrawScalar.inl"
#endif // #if !CDRAW_USING_PRECOMPILE_LIBS


#endif // #ifndef _CDRAW_SCALAR_H_