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

#define CDRAW_ASSERT_TEST cdraw_assert_test_flag

#include "cdraw/cdrawMath.h"


/******************************************************************************
* Testing interfaces.
******************************************************************************/

#define cdraw_testDF_errcode									errcode_CUSTOM_BEGIN
#define cdraw_testDF_defaultname								__cdraw_testDF__
#define cdraw_testDF_init()										cdraw_scalar_declDF(cdraw_testDF_defaultname, 0.0)

#define cdraw_testDF_true1(funcD,funcF,arg0_fp64)										\
		cdraw_test_true_asserterr(cdraw_testDF_errcode, funcD, (fp64_t)(arg0_fp64));	\
		cdraw_test_true_asserterr(cdraw_testDF_errcode, funcF, (fp32_t)(arg0_fp64))
#define cdraw_testDF_true3(funcD,funcF,arg0_fp64,arg1_fp64,arg2_fp64)															\
		cdraw_test_true_asserterr(cdraw_testDF_errcode, funcD, (fp64_t)(arg0_fp64), (fp64_t)(arg1_fp64), (fp64_t)(arg2_fp64));	\
		cdraw_test_true_asserterr(cdraw_testDF_errcode, funcF, (fp32_t)(arg0_fp64), (fp32_t)(arg1_fp64), (fp32_t)(arg2_fp64))

#define cdraw_testDF_false1(funcD,funcF,arg0_fp64)										\
		cdraw_test_false_asserterr(cdraw_testDF_errcode, funcD, (fp64_t)(arg0_fp64));	\
		cdraw_test_false_asserterr(cdraw_testDF_errcode, funcF, (fp32_t)(arg0_fp64))
#define cdraw_testDF_false3(funcD,funcF,arg0_fp64,arg1_fp64,arg2_fp64)															\
		cdraw_test_false_asserterr(cdraw_testDF_errcode, funcD, (fp64_t)(arg0_fp64), (fp64_t)(arg1_fp64), (fp64_t)(arg2_fp64));	\
		cdraw_test_false_asserterr(cdraw_testDF_errcode, funcF, (fp32_t)(arg0_fp64), (fp32_t)(arg1_fp64), (fp32_t)(arg2_fp64))

#define cdraw_testDF_exact1(expect_fp64,funcD,funcF,arg0_fp64)																						\
		cdraw_test_exact_asserterr(cdraw_testDF_errcode, (fp64_t)(expect_fp64), tokencat(cdraw_testDF_defaultname, D), funcD, (fp64_t)(arg0_fp64));	\
		cdraw_test_exact_asserterr(cdraw_testDF_errcode, (fp32_t)(expect_fp64), tokencat(cdraw_testDF_defaultname, F), funcF, (fp32_t)(arg0_fp64))
#define cdraw_testDF_exact2(expect_fp64,funcD,funcF,arg0_fp64,arg1_fp64)																									\
		cdraw_test_exact_asserterr(cdraw_testDF_errcode, (fp64_t)(expect_fp64), tokencat(cdraw_testDF_defaultname, D), funcD, (fp64_t)(arg0_fp64), (fp64_t)(arg1_fp64));	\
		cdraw_test_exact_asserterr(cdraw_testDF_errcode, (fp32_t)(expect_fp64), tokencat(cdraw_testDF_defaultname, F), funcF, (fp32_t)(arg0_fp64), (fp32_t)(arg1_fp64))
#define cdraw_testDF_exact3(expect_fp64,funcD,funcF,arg0_fp64,arg1_fp64,arg2_fp64)																												\
		cdraw_test_exact_asserterr(cdraw_testDF_errcode, (fp64_t)(expect_fp64), tokencat(cdraw_testDF_defaultname, D), funcD, (fp64_t)(arg0_fp64), (fp64_t)(arg1_fp64), (fp64_t)(arg2_fp64));	\
		cdraw_test_exact_asserterr(cdraw_testDF_errcode, (fp32_t)(expect_fp64), tokencat(cdraw_testDF_defaultname, F), funcF, (fp32_t)(arg0_fp64), (fp32_t)(arg1_fp64), (fp32_t)(arg2_fp64))
#define cdraw_testDF_exact4(expect_fp64,funcD,funcF,arg0_fp64,arg1_fp64,arg2_fp64,arg3_fp64)																														\
		cdraw_test_exact_asserterr(cdraw_testDF_errcode, (fp64_t)(expect_fp64), tokencat(cdraw_testDF_defaultname, D), funcD, (fp64_t)(arg0_fp64), (fp64_t)(arg1_fp64), (fp64_t)(arg2_fp64), (fp64_t)(arg3_fp64));	\
		cdraw_test_exact_asserterr(cdraw_testDF_errcode, (fp32_t)(expect_fp64), tokencat(cdraw_testDF_defaultname, F), funcF, (fp32_t)(arg0_fp64), (fp32_t)(arg1_fp64), (fp32_t)(arg2_fp64), (fp32_t)(arg3_fp64))
#define cdraw_testDF_exact5(expect_fp64,funcD,funcF,arg0_fp64,arg1_fp64,arg2_fp64,arg3_fp64,arg4_fp64)																																	\
		cdraw_test_exact_asserterr(cdraw_testDF_errcode, (fp64_t)(expect_fp64), tokencat(cdraw_testDF_defaultname, D), funcD, (fp64_t)(arg0_fp64), (fp64_t)(arg1_fp64), (fp64_t)(arg2_fp64), (fp64_t)(arg3_fp64), (fp64_t)(arg4_fp64));	\
		cdraw_test_exact_asserterr(cdraw_testDF_errcode, (fp32_t)(expect_fp64), tokencat(cdraw_testDF_defaultname, F), funcF, (fp32_t)(arg0_fp64), (fp32_t)(arg1_fp64), (fp32_t)(arg2_fp64), (fp32_t)(arg3_fp64), (fp32_t)(arg4_fp64))

#define cdraw_testDF_approx1(expect_fp64,funcD,funcF,arg0_fp64)																										\
		cdraw_test_approx_asserterr(cdraw_testDF_errcode, (scAtrigEpsD), (fp64_t)(expect_fp64), tokencat(cdraw_testDF_defaultname, D), funcD, (fp64_t)(arg0_fp64));	\
		cdraw_test_approx_asserterr(cdraw_testDF_errcode, (scAtrigEpsF), (fp32_t)(expect_fp64), tokencat(cdraw_testDF_defaultname, F), funcF, (fp32_t)(arg0_fp64))
#define cdraw_testDF_approx2(expect_fp64,funcD,funcF,arg0_fp64,arg1_fp64)																													\
		cdraw_test_approx_asserterr(cdraw_testDF_errcode, (scAtrigEpsD), (fp64_t)(expect_fp64), tokencat(cdraw_testDF_defaultname, D), funcD, (fp64_t)(arg0_fp64), (fp64_t)(arg1_fp64));	\
		cdraw_test_approx_asserterr(cdraw_testDF_errcode, (scAtrigEpsF), (fp32_t)(expect_fp64), tokencat(cdraw_testDF_defaultname, F), funcF, (fp32_t)(arg0_fp64), (fp32_t)(arg1_fp64))
#define cdraw_testDF_approx3(expect_fp64,funcD,funcF,arg0_fp64,arg1_fp64,arg2_fp64)																																\
		cdraw_test_approx_asserterr(cdraw_testDF_errcode, (scAtrigEpsD), (fp64_t)(expect_fp64), tokencat(cdraw_testDF_defaultname, D), funcD, (fp64_t)(arg0_fp64), (fp64_t)(arg1_fp64), (fp64_t)(arg2_fp64));	\
		cdraw_test_approx_asserterr(cdraw_testDF_errcode, (scAtrigEpsF), (fp32_t)(expect_fp64), tokencat(cdraw_testDF_defaultname, F), funcF, (fp32_t)(arg0_fp64), (fp32_t)(arg1_fp64), (fp32_t)(arg2_fp64))
#define cdraw_testDF_approx4(expect_fp64,funcD,funcF,arg0_fp64,arg1_fp64,arg2_fp64,arg3_fp64)																																		\
		cdraw_test_approx_asserterr(cdraw_testDF_errcode, (scAtrigEpsD), (fp64_t)(expect_fp64), tokencat(cdraw_testDF_defaultname, D), funcD, (fp64_t)(arg0_fp64), (fp64_t)(arg1_fp64), (fp64_t)(arg2_fp64), (fp64_t)(arg3_fp64));	\
		cdraw_test_approx_asserterr(cdraw_testDF_errcode, (scAtrigEpsF), (fp32_t)(expect_fp64), tokencat(cdraw_testDF_defaultname, F), funcF, (fp32_t)(arg0_fp64), (fp32_t)(arg1_fp64), (fp32_t)(arg2_fp64), (fp32_t)(arg3_fp64))
#define cdraw_testDF_approx5(expect_fp64,funcD,funcF,arg0_fp64,arg1_fp64,arg2_fp64,arg3_fp64,arg4_fp64)																																					\
		cdraw_test_approx_asserterr(cdraw_testDF_errcode, (scAtrigEpsD), (fp64_t)(expect_fp64), tokencat(cdraw_testDF_defaultname, D), funcD, (fp64_t)(arg0_fp64), (fp64_t)(arg1_fp64), (fp64_t)(arg2_fp64), (fp64_t)(arg3_fp64), (fp64_t)(arg4_fp64));	\
		cdraw_test_approx_asserterr(cdraw_testDF_errcode, (scAtrigEpsF), (fp32_t)(expect_fp64), tokencat(cdraw_testDF_defaultname, F), funcF, (fp32_t)(arg0_fp64), (fp32_t)(arg1_fp64), (fp32_t)(arg2_fp64), (fp32_t)(arg3_fp64), (fp32_t)(arg4_fp64))

#if (defined CDRAW_ASSERT_TEST)
#define cdraw_testDF_unsafe1(expect_fp64,funcD,funcF,arg0_fp64)																							\
		cdraw_test_unsafe_asserterr(cdraw_testDF_errcode, (fp64_t)(expect_fp64), tokencat(cdraw_testDF_defaultname, D), funcD, (fp64_t)(arg0_fp64));	\
		cdraw_test_unsafe_asserterr(cdraw_testDF_errcode, (fp32_t)(expect_fp64), tokencat(cdraw_testDF_defaultname, F), funcF, (fp32_t)(arg0_fp64))
#define cdraw_testDF_unsafe2(expect_fp64,funcD,funcF,arg0_fp64,arg1_fp64)																									\
		cdraw_test_unsafe_asserterr(cdraw_testDF_errcode, (fp64_t)(expect_fp64), tokencat(cdraw_testDF_defaultname, D), funcD, (fp64_t)(arg0_fp64), (fp64_t)(arg1_fp64));	\
		cdraw_test_unsafe_asserterr(cdraw_testDF_errcode, (fp32_t)(expect_fp64), tokencat(cdraw_testDF_defaultname, F), funcF, (fp32_t)(arg0_fp64), (fp32_t)(arg1_fp64))
#define cdraw_testDF_unsafe3(expect_fp64,funcD,funcF,arg0_fp64,arg1_fp64,arg2_fp64)																												\
		cdraw_test_unsafe_asserterr(cdraw_testDF_errcode, (fp64_t)(expect_fp64), tokencat(cdraw_testDF_defaultname, D), funcD, (fp64_t)(arg0_fp64), (fp64_t)(arg1_fp64), (fp64_t)(arg2_fp64));	\
		cdraw_test_unsafe_asserterr(cdraw_testDF_errcode, (fp32_t)(expect_fp64), tokencat(cdraw_testDF_defaultname, F), funcF, (fp32_t)(arg0_fp64), (fp32_t)(arg1_fp64), (fp32_t)(arg2_fp64))
#define cdraw_testDF_unsafe4(expect_fp64,funcD,funcF,arg0_fp64,arg1_fp64,arg2_fp64,arg3_fp64)																														\
		cdraw_test_unsafe_asserterr(cdraw_testDF_errcode, (fp64_t)(expect_fp64), tokencat(cdraw_testDF_defaultname, D), funcD, (fp64_t)(arg0_fp64), (fp64_t)(arg1_fp64), (fp64_t)(arg2_fp64), (fp64_t)(arg3_fp64));	\
		cdraw_test_unsafe_asserterr(cdraw_testDF_errcode, (fp32_t)(expect_fp64), tokencat(cdraw_testDF_defaultname, F), funcF, (fp32_t)(arg0_fp64), (fp32_t)(arg1_fp64), (fp32_t)(arg2_fp64), (fp32_t)(arg3_fp64))
#define cdraw_testDF_unsafe5(expect_fp64,funcD,funcF,arg0_fp64,arg1_fp64,arg2_fp64,arg3_fp64,arg4_fp64)																																		\
		cdraw_test_unsafe_asserterr(cdraw_testDF_errcode, (fp64_t)(expect_fp64), tokencat(cdraw_testDF_defaultname, D), funcD, (fp64_t)(arg0_fp64), (fp64_t)(arg1_fp64), (fp64_t)(arg2_fp64), (fp64_t)(arg3_fp64), (fp64_t)(arg4_fp64));	\
		cdraw_test_unsafe_asserterr(cdraw_testDF_errcode, (fp32_t)(expect_fp64), tokencat(cdraw_testDF_defaultname, F), funcF, (fp32_t)(arg0_fp64), (fp32_t)(arg1_fp64), (fp32_t)(arg2_fp64), (fp32_t)(arg3_fp64), (fp32_t)(arg4_fp64))
#endif // #if (defined CDRAW_ASSERT_TEST)


/******************************************************************************
* Tests.
******************************************************************************/

result_t cdrawScalarTest()
{
	result_init();
	cdraw_testDF_init();

	{
		fp64_t const x_B = 3.14159, x_C = -180.0;

		cdraw_test_true_asserterr(cdraw_testDF_errcode, scIsZeroApproxD, scEpsD);
		cdraw_test_true_asserterr(cdraw_testDF_errcode, scIsZeroApproxF, scEpsF);
		cdraw_testDF_false1(scIsZeroApproxD, scIsZeroApproxF, x_B);
		cdraw_testDF_false1(scIsZeroApproxD, scIsZeroApproxF, x_C);

		cdraw_test_false_asserterr(cdraw_testDF_errcode, scIsNonZeroApproxD, scEpsD);
		cdraw_test_false_asserterr(cdraw_testDF_errcode, scIsNonZeroApproxF, scEpsF);
		cdraw_testDF_true1(scIsNonZeroApproxD, scIsNonZeroApproxF, x_B);
		cdraw_testDF_true1(scIsNonZeroApproxD, scIsNonZeroApproxF, x_C);

		cdraw_test_false_asserterr(cdraw_testDF_errcode, scIsPositiveApproxD, scEpsD);
		cdraw_test_false_asserterr(cdraw_testDF_errcode, scIsPositiveApproxF, scEpsF);
		cdraw_testDF_true1(scIsPositiveApproxD, scIsPositiveApproxF, x_B);
		cdraw_testDF_false1(scIsPositiveApproxD, scIsPositiveApproxF, x_C);

		cdraw_test_true_asserterr(cdraw_testDF_errcode, scIsNonPositiveApproxD, scEpsD);
		cdraw_test_true_asserterr(cdraw_testDF_errcode, scIsNonPositiveApproxF, scEpsF);
		cdraw_testDF_false1(scIsNonPositiveApproxD, scIsNonPositiveApproxF, x_B);
		cdraw_testDF_true1(scIsNonPositiveApproxD, scIsNonPositiveApproxF, x_C);

		cdraw_test_false_asserterr(cdraw_testDF_errcode, scIsNegativeApproxD, scEpsD);
		cdraw_test_false_asserterr(cdraw_testDF_errcode, scIsNegativeApproxF, scEpsF);
		cdraw_testDF_false1(scIsNegativeApproxD, scIsNegativeApproxF, x_B);
		cdraw_testDF_true1(scIsNegativeApproxD, scIsNegativeApproxF, x_C);

		cdraw_test_true_asserterr(cdraw_testDF_errcode, scIsNonNegativeApproxD, scEpsD);
		cdraw_test_true_asserterr(cdraw_testDF_errcode, scIsNonNegativeApproxF, scEpsF);
		cdraw_testDF_true1(scIsNonNegativeApproxD, scIsNonNegativeApproxF, x_B);
		cdraw_testDF_false1(scIsNonNegativeApproxD, scIsNonNegativeApproxF, x_C);
	}
	
	{
		fp64_t const x_A = +2.5, x_B = -1.25, x_C = 0.0, x_num = 4.0;

		cdraw_testDF_exact1(+1.0, scSgnD, scSgnF, x_A);
		cdraw_testDF_exact1(-1.0, scSgnD, scSgnF, x_B);
		cdraw_testDF_exact1(0.00, scSgnD, scSgnF, x_C);

		cdraw_testDF_exact1(+x_A, scAbsD, scAbsF, x_A);
		cdraw_testDF_exact1(-x_B, scAbsD, scAbsF, x_B);
		cdraw_testDF_exact1(0.00, scAbsD, scAbsF, x_C);

		cdraw_testDF_approx1(0.50, scFracD, scFracF, x_A);
		cdraw_testDF_approx1(0.75, scFracD, scFracF, x_B);
		cdraw_testDF_approx1(0.00, scFracD, scFracF, x_C);

		cdraw_testDF_approx1(+0.4, scRecipD, scRecipF, x_A);
		cdraw_testDF_approx1(-0.8, scRecipD, scRecipF, x_B);
		cdraw_testDF_unsafe1(0.00, scRecipD, scRecipF, x_C);

		cdraw_testDF_approx2(+1.6, scDivD, scDivF, x_num, x_A);
		cdraw_testDF_approx2(-3.2, scDivD, scDivF, x_num, x_B);
		cdraw_testDF_unsafe2(0.00, scDivD, scDivF, x_num, x_C);

		cdraw_testDF_approx2(+1.5, scModD, scModF, x_num, x_A);
		cdraw_testDF_approx2(-1.0, scModD, scModF, x_num, x_B);
		cdraw_testDF_unsafe2(x_num, scModD, scModF, x_num, x_C);
	}

	{
		fp64_t const x_A = +0.75, x_B = +1.25, x_C = -0.25, x_min = -2.0, x_max = +6.0, x_min_dst = +50.0, x_max_dst = +100.0;

		cdraw_testDF_approx3(+4.0, scLerpD, scLerpF, x_A, x_min, x_max);
		cdraw_testDF_approx3(+8.0, scLerpD, scLerpF, x_B, x_min, x_max);
		cdraw_testDF_approx3(x_min, scLerpD, scLerpF, x_C, x_min, x_min);

		cdraw_testDF_approx3(x_A, scLerpInvD, scLerpInvF, +4.0, x_min, x_max);
		cdraw_testDF_approx3(x_B, scLerpInvD, scLerpInvF, +8.0, x_min, x_max);
		cdraw_testDF_unsafe3(0.00, scLerpInvD, scLerpInvF, x_C, x_min, x_min);

		cdraw_testDF_exact3(x_A, scClampD, scClampF, x_A, x_min, x_max);
		cdraw_testDF_exact3(x_max, scClampD, scClampF, +x_max_dst, x_min, x_max);
		cdraw_testDF_exact3(x_min, scClampD, scClampF, -x_max_dst, x_min, x_max);

		cdraw_testDF_approx3(x_A, scRepeatD, scRepeatF, x_A, x_min, x_max);
		cdraw_testDF_approx3(4.0, scRepeatD, scRepeatF, x_max_dst, x_min, x_max);
		cdraw_testDF_unsafe3(x_C, scRepeatD, scRepeatF, x_C, x_min, x_min);

		cdraw_testDF_approx5(87.5, scRemapD, scRemapF, +4.0, x_min, x_max, x_min_dst, x_max_dst);
		cdraw_testDF_approx5(x_min_dst, scRemapD, scRemapF, +8.0, x_min, x_max, x_min_dst, x_min_dst);
		cdraw_testDF_unsafe5(x_min_dst, scRemapD, scRemapF, x_C, x_min, x_min, x_min_dst, x_max_dst);

		cdraw_testDF_true3(scIsInRangeD, scIsInRangeF, x_A, x_min, x_max);
		cdraw_testDF_false3(scIsInRangeD, scIsInRangeF, x_B, x_min_dst, x_max_dst);
		cdraw_testDF_true3(scIsInRangeD, scIsInRangeF, x_C, x_C, x_C);

		cdraw_testDF_true1(scIsInUnitRangeD, scIsInUnitRangeF, x_A);
		cdraw_testDF_false1(scIsInUnitRangeD, scIsInUnitRangeF, x_B);
		cdraw_testDF_false1(scIsInUnitRangeD, scIsInUnitRangeF, x_C);

		cdraw_testDF_true1(scIsInSymmRangeD, scIsInSymmRangeF, x_A);
		cdraw_testDF_false1(scIsInSymmRangeD, scIsInSymmRangeF, x_B);
		cdraw_testDF_true1(scIsInSymmRangeD, scIsInSymmRangeF, x_C);
	}

	{
		fp64_t const x_A = +64.0, x_B = -64.0, x_C = 0.0;

		cdraw_testDF_approx1(8.0, scSqrtD, scSqrtF, x_A);
		cdraw_testDF_unsafe1(0.0, scSqrtD, scSqrtF, x_B);
		cdraw_testDF_exact1(0.00, scSqrtD, scSqrtF, x_C);

		cdraw_testDF_approx1(0.125, scSqrtInvD, scSqrtInvF, x_A);
		cdraw_testDF_unsafe1(0.000, scSqrtInvD, scSqrtInvF, x_B);
		cdraw_testDF_unsafe1(0.000, scSqrtInvD, scSqrtInvF, x_C);

		cdraw_testDF_approx1(+4.0, scCbrtD, scCbrtF, x_A);
		cdraw_testDF_approx1(-4.0, scCbrtD, scCbrtF, x_B);
		cdraw_testDF_exact1(0.000, scCbrtD, scCbrtF, x_C);

		cdraw_testDF_approx1(+0.25, scCbrtInvD, scCbrtInvF, x_A);
		cdraw_testDF_approx1(-0.25, scCbrtInvD, scCbrtInvF, x_B);
		cdraw_testDF_unsafe1(0.000, scCbrtInvD, scCbrtInvF, x_C);
	}

	{
		fp64_t const x_r = sc3Pi_2D, x_d = sc270D, x_rr = -scPi_4D, x_dd = -sc45D;

		cdraw_testDF_approx1(x_r, scDeg2RadD, scDeg2RadF, x_d);
		cdraw_testDF_approx1(x_rr, scDeg2RadD, scDeg2RadF, x_dd);
		
		cdraw_testDF_approx1(x_d, scRad2DegD, scRad2DegF, x_r);
		cdraw_testDF_approx1(x_dd, scRad2DegD, scRad2DegF, x_rr);
		
		cdraw_testDF_approx1(-sc90D, scNormalizeAngle180D, scNormalizeAngle180F, x_d);
		cdraw_testDF_approx1(x_dd, scNormalizeAngle180D, scNormalizeAngle180F, x_dd);
		
		cdraw_testDF_approx1(x_d, scNormalizeAngle360D, scNormalizeAngle360F, x_d);
		cdraw_testDF_approx1(315.0, scNormalizeAngle360D, scNormalizeAngle360F, x_dd);
		
		cdraw_testDF_approx1(-scPi_2D, scNormalizeAnglePiD, scNormalizeAnglePiF, x_r);
		cdraw_testDF_approx1(x_rr, scNormalizeAnglePiD, scNormalizeAnglePiF, x_rr);
		
		cdraw_testDF_approx1(x_r, scNormalizeAngle2PiD, scNormalizeAngle2PiF, x_r);
		cdraw_testDF_approx1((scPi_4D * 7.0), scNormalizeAngle2PiD, scNormalizeAngle2PiF, x_rr);

		cdraw_testDF_approx1(-1.0, scSinrD, scSinrF, x_r);
		cdraw_testDF_approx1(-scSqrt1_2D, scSinrD, scSinrF, x_rr);

		cdraw_testDF_approx1(x_r - sc2PiD, scAsinrD, scAsinrF, -1.0);
		cdraw_testDF_approx1(x_rr, scAsinrD, scAsinrF, -scSqrt1_2D);
		cdraw_testDF_unsafe1(0.0, scAsinrD, scAsinrF, -2.0);

		cdraw_testDF_approx1(0.0, scCosrD, scCosrF, x_r);
		cdraw_testDF_approx1(+scSqrt1_2D, scCosrD, scCosrF, x_rr);

		cdraw_testDF_approx1(x_r - scPi_2D, scAcosrD, scAcosrF, -1.0);
		cdraw_testDF_approx1(x_rr + scPiD, scAcosrD, scAcosrF, -scSqrt1_2D);
		cdraw_testDF_unsafe1(1.0, scAcosrD, scAcosrF, -2.0);

		cdraw_testDF_unsafe1(0.0, scTanrD, scTanrF, x_r);
		cdraw_testDF_approx1(-1.0, scTanrD, scTanrF, x_rr);

		cdraw_testDF_approx1(0.0, scAtanrD, scAtanrF, 0.0);
		cdraw_testDF_approx1(x_rr, scAtanrD, scAtanrF, -1.0);

		cdraw_testDF_approx2((x_r - sc2PiD), scAtan2rD, scAtan2rF, -1.0, 0.0);
		cdraw_testDF_approx2(x_rr, scAtan2rD, scAtan2rF, -1.0, +1.0);
		cdraw_testDF_unsafe2(0.0, scAtan2rD, scAtan2rF, 0.0, 0.0);

		cdraw_testDF_approx1(-1.0, scSindD, scSindF, x_d);
		cdraw_testDF_approx1(-scSqrt1_2D, scSindD, scSindF, x_dd);

		cdraw_testDF_approx1(x_d - sc360D, scAsindD, scAsindF, -1.0);
		cdraw_testDF_approx1(x_dd, scAsindD, scAsindF, -scSqrt1_2D);
		cdraw_testDF_unsafe1(0.0, scAsindD, scAsindF, -2.0);

		cdraw_testDF_approx1(0.0, scCosdD, scCosdF, x_d);
		cdraw_testDF_approx1(+scSqrt1_2D, scCosdD, scCosdF, x_dd);

		cdraw_testDF_approx1(x_d - sc90D, scAcosdD, scAcosdF, -1.0);
		cdraw_testDF_approx1(x_dd + sc180D, scAcosdD, scAcosdF, -scSqrt1_2D);
		cdraw_testDF_unsafe1(1.0, scAcosdD, scAcosdF, -2.0);

		cdraw_testDF_unsafe1(0.0, scTandD, scTandF, x_d);
		cdraw_testDF_approx1(-1.0, scTandD, scTandF, x_dd);

		cdraw_testDF_approx1(0.0, scAtandD, scAtandF, 0.0);
		cdraw_testDF_approx1(x_dd, scAtandD, scAtandF, -1.0);

		cdraw_testDF_approx2((x_d - sc360D), scAtan2dD, scAtan2dF, -1.0, 0.0);
		cdraw_testDF_approx2(x_dd, scAtan2dD, scAtan2dF, -1.0, +1.0);
		cdraw_testDF_unsafe2(0.0, scAtan2dD, scAtan2dF, 0.0, 0.0);
	}

	{
		fp64_t rand64;
		fp32_t rand32;

		rand64 = scRandD();
		rand32 = scRandF();
		asserterr(scIsInRangeD(rand64, sc0D, (fp64_t)RAND_MAX), cdraw_testDF_errcode);
		asserterr(scIsInRangeF(rand32, sc0F, (fp32_t)RAND_MAX), cdraw_testDF_errcode);

		rand64 = scRandUnitD();
		rand32 = scRandUnitF();
		asserterr(scIsInUnitRangeD(rand64), cdraw_testDF_errcode);
		asserterr(scIsInUnitRangeF(rand32), cdraw_testDF_errcode);

		rand64 = scRandSymmD();
		rand32 = scRandSymmF();
		asserterr(scIsInSymmRangeD(rand64), cdraw_testDF_errcode);
		asserterr(scIsInSymmRangeF(rand32), cdraw_testDF_errcode);
	}

	result_return();
}


result_t cdrawMathTest()
{
	result_init();
	cdrawScalarTest();
	result_return();
}


/******************************************************************************
* Precompile all inlines.
******************************************************************************/

#if CDRAW_USING_PRECOMPILE_LIBS
#undef CDRAW_INL
#define CDRAW_INL
#include "cdraw/cdrawMath/_inl/cdrawScalar.inl"
#endif // #if CDRAW_USING_PRECOMPILE_LIBS