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

typedef bool(*cdraw_test_b)(ptrk_t const, bool const expect, bool const result);
typedef bool(*cdraw_test_i)(ptrk_t const, int32_t const expect, int32_t const result);
typedef bool(*cdraw_test_u)(ptrk_t const, uint32_t const expect, uint32_t const result);
typedef bool(*cdraw_test_f)(fp32_t tolerance, fp32_t const expect, fp32_t const result);
typedef bool(*cdraw_test_d)(fp64_t tolerance, fp64_t const expect, fp64_t const result);

static bool cdraw_istrue_b(ptrk_t const tolerance_unused, bool const expect_unused, bool const result) { return cdraw_istrue(0, 0, result); }
static bool cdraw_istrue_i(ptrk_t const tolerance_unused, int32_t const expect_unused, int32_t const result) { return cdraw_istrue(0, 0, result); }
static bool cdraw_istrue_u(ptrk_t const tolerance_unused, uint32_t const expect_unused, uint32_t const result) { return cdraw_istrue(0, 0, result); }
static bool cdraw_istrue_f(fp32_t tolerance_unused, fp32_t const expect_unused, fp64_t const result) { return cdraw_istrue(0, 0, result); }
static bool cdraw_istrue_d(fp64_t tolerance_unused, fp64_t const expect_unused, fp64_t const result) { return cdraw_istrue(0, 0, result); }
static bool cdraw_isfalse_b(ptrk_t const tolerance_unused, bool const expect_unused, bool const result) { return cdraw_isfalse(0, 0, result); }
static bool cdraw_isfalse_i(ptrk_t const tolerance_unused, int32_t const expect_unused, int32_t const result) { return cdraw_isfalse(0, 0, result); }
static bool cdraw_isfalse_u(ptrk_t const tolerance_unused, uint32_t const expect_unused, uint32_t const result) { return cdraw_isfalse(0, 0, result); }
static bool cdraw_isfalse_f(fp32_t tolerance_unused, fp32_t const expect_unused, fp64_t const result) { return cdraw_isfalse(0, 0, result); }
static bool cdraw_isfalse_d(fp64_t tolerance_unused, fp64_t const expect_unused, fp64_t const result) { return cdraw_isfalse(0, 0, result); }
static bool cdraw_isexact_b(ptrk_t const tolerance_unused, bool const expect, bool const result) { return cdraw_isexact(0, expect, result); }
static bool cdraw_isexact_i(ptrk_t const tolerance_unused, int32_t const expect, int32_t const result) { return cdraw_isexact(0, expect, result); }
static bool cdraw_isexact_u(ptrk_t const tolerance_unused, uint32_t const expect, uint32_t const result) { return cdraw_isexact(0, expect, result); }
static bool cdraw_isexact_f(fp32_t tolerance_unused, fp32_t const expect, fp64_t const result) { return cdraw_isexact(0, expect, result); }
static bool cdraw_isexact_d(fp64_t tolerance_unused, fp64_t const expect, fp64_t const result) { return cdraw_isexact(0, expect, result); }
static bool cdraw_isapprox_f(fp32_t tolerance, fp32_t const expect, fp32_t const result) { tolerance = (tolerance >= -tolerance ? tolerance : -tolerance); return cdraw_isapprox(tolerance, expect, result); }
static bool cdraw_isapprox_d(fp64_t tolerance, fp64_t const expect, fp64_t const result) { tolerance = (tolerance >= -tolerance ? tolerance : -tolerance); return cdraw_isapprox(tolerance, expect, result); }

#if (defined CDRAW_ASSERT_TEST)
static bool cdraw_isunsafe_i(ptrk_t const tolerance_unused, int32_t const expect, int32_t const result) { return cdraw_isunsafe(0, expect, result); }
static bool cdraw_isunsafe_u(ptrk_t const tolerance_unused, uint32_t const expect, uint32_t const result) { return cdraw_isunsafe(0, expect, result); }
static bool cdraw_isunsafe_f(fp32_t tolerance_unused, fp32_t const expect, fp32_t const result) { return cdraw_isunsafe(0, expect, result); }
static bool cdraw_isunsafe_d(fp64_t tolerance_unused, fp64_t const expect, fp64_t const result) { return cdraw_isunsafe(0, expect, result); }
#endif // #if (defined CDRAW_ASSERT_TEST)


#define cdraw_testSc(test,tolerance,expect,func,...) failassertreset();					\
		asserterr(test(tolerance, expect, func(__VA_ARGS__)), errcode_CUSTOM_BEGIN)
#define cdraw_testSc_rB(test,func,...) failassertreset();								\
		asserterr(test(0, 0, func(__VA_ARGS__)), errcode_CUSTOM_BEGIN)
#define cdraw_testScDF_rB1S(testname,funcname,arg0_fp64)												\
		cdraw_testSc(cdraw_testname(testname,_d), 0, 0, tokencat(funcname, D), (fp64_t)(arg0_fp64));	\
		cdraw_testSc(cdraw_testname(testname,_f), 0, 0, tokencat(funcname, F), (fp32_t)(arg0_fp64))
#define cdraw_testScDF_rB3S(testname,funcname,arg0_fp64,arg1_fp64,arg2_fp64)																	\
		cdraw_testSc(cdraw_testname(testname,_d), 0, 0, tokencat(funcname, D), (fp64_t)(arg0_fp64), (fp64_t)(arg1_fp64), (fp64_t)(arg2_fp64));	\
		cdraw_testSc(cdraw_testname(testname,_f), 0, 0, tokencat(funcname, F), (fp32_t)(arg0_fp64), (fp32_t)(arg1_fp64), (fp32_t)(arg2_fp64))
#define cdraw_testScDF_1S(testname,expect_fp64,funcname,arg0_fp64)																	\
		cdraw_testSc(cdraw_testname(testname,_d), (scBigEpsD), (fp64_t)(expect_fp64), tokencat(funcname, D), (fp64_t)(arg0_fp64));	\
		cdraw_testSc(cdraw_testname(testname,_f), (scBigEpsF), (fp32_t)(expect_fp64), tokencat(funcname, F), (fp32_t)(arg0_fp64))
#define cdraw_testScDF_2S(testname,expect_fp64,funcname,arg0_fp64,arg1_fp64)																			\
		cdraw_testSc(cdraw_testname(testname,_d), (scBigEpsD), (fp64_t)(expect_fp64), tokencat(funcname, D), (fp64_t)(arg0_fp64), (fp64_t)(arg1_fp64));	\
		cdraw_testSc(cdraw_testname(testname,_f), (scBigEpsF), (fp32_t)(expect_fp64), tokencat(funcname, F), (fp32_t)(arg0_fp64), (fp32_t)(arg1_fp64))
#define cdraw_testScDF_3S(testname,expect_fp64,funcname,arg0_fp64,arg1_fp64,arg2_fp64)																							\
		cdraw_testSc(cdraw_testname(testname,_d), (scBigEpsD), (fp64_t)(expect_fp64), tokencat(funcname, D), (fp64_t)(arg0_fp64), (fp64_t)(arg1_fp64), (fp64_t)(arg2_fp64));	\
		cdraw_testSc(cdraw_testname(testname,_f), (scBigEpsF), (fp32_t)(expect_fp64), tokencat(funcname, F), (fp32_t)(arg0_fp64), (fp32_t)(arg1_fp64), (fp32_t)(arg2_fp64))
#define cdraw_testScDF_5S(testname,expect_fp64,funcname,arg0_fp64,arg1_fp64,arg2_fp64,arg3_fp64,arg4_fp64)																												\
		cdraw_testSc(cdraw_testname(testname,_d), (scBigEpsD), (fp64_t)(expect_fp64), tokencat(funcname, D), (fp64_t)(arg0_fp64), (fp64_t)(arg1_fp64), (fp64_t)(arg2_fp64), (fp64_t)(arg3_fp64), (fp64_t)(arg4_fp64));	\
		cdraw_testSc(cdraw_testname(testname,_f), (scBigEpsF), (fp32_t)(expect_fp64), tokencat(funcname, F), (fp32_t)(arg0_fp64), (fp32_t)(arg1_fp64), (fp32_t)(arg2_fp64), (fp32_t)(arg3_fp64), (fp32_t)(arg4_fp64))


#define cdraw_testVec_rB(test,func,...)																												\
		cdraw_testSc_rB(test, func, __VA_ARGS__)
#define cdraw_testVec_rS(test,tolerance,expect,func,...)																							\
		cdraw_testSc(test, tolerance, expect, func, __VA_ARGS__)
#define cdraw_testVec2_rvB(test_x,test_y,func,result,...) failassertreset(); func(result, __VA_ARGS__);												\
		asserterr(test_x(0, 0, result[0]), errcode_CUSTOM_BEGIN);																					\
		asserterr(test_y(0, 0, result[1]), errcode_CUSTOM_BEGIN)	
#define cdraw_testVec3_rvB(test_x,test_y,test_z,func,result,...)																					\
		cdraw_testVec2_rvB(test_x,test_y, func, result, __VA_ARGS__);																				\
		asserterr(test_z(0, 0, result[2]), errcode_CUSTOM_BEGIN)
#define cdraw_testVec4_rvB(test_x,test_y,test_z,test_w,func,result,...)																				\
		cdraw_testVec3_rvB(test_x,test_y,test_z, func, result, __VA_ARGS__);																		\
		asserterr(test_w(0, 0, result[3]), errcode_CUSTOM_BEGIN)
#define cdraw_testVec2_rSV(test_s,test_x,test_y,tolerance,expect_s,expect,func,result,...) failassertreset();										\
		asserterr(test_s(tolerance, expect_s, func(result, __VA_ARGS__)), errcode_CUSTOM_BEGIN);													\
		asserterr(test_x(tolerance, expect[0], result[0]), errcode_CUSTOM_BEGIN);																	\
		asserterr(test_y(tolerance, expect[1], result[1]), errcode_CUSTOM_BEGIN)
#define cdraw_testVec3_rSV(test_s,test_x,test_y,test_z,tolerance,expect_s,expect,func,result,...)													\
		cdraw_testVec2_rSV(test_s,test_x,test_y, tolerance, expect_s, expect, func, result, __VA_ARGS__);											\
		asserterr(test_z(tolerance, expect[2], result[2]), errcode_CUSTOM_BEGIN)
#define cdraw_testVec4_rSV(test_s,test_x,test_y,test_z,test_w,tolerance,expect_s,expect,func,result,...)											\
		cdraw_testVec3_rSV(test_s,test_x,test_y,test_z, tolerance, expect_s, expect, func, result, __VA_ARGS__);									\
		asserterr(test_w(tolerance, expect[3], result[3]), errcode_CUSTOM_BEGIN)
#define cdraw_testVec2(test_x,test_y,tolerance,expect,func,result,...) failassertreset(); func(result, __VA_ARGS__);								\
		asserterr(test_x(tolerance, expect[0], result[0]), errcode_CUSTOM_BEGIN);																	\
		asserterr(test_y(tolerance, expect[1], result[1]), errcode_CUSTOM_BEGIN)
#define cdraw_testVec3(test_x,test_y,test_z,tolerance,expect,func,result,...)																		\
		cdraw_testVec2(test_x,test_y, tolerance, expect, func, result, __VA_ARGS__);																\
		asserterr(test_z(tolerance, expect[2], result[2]), errcode_CUSTOM_BEGIN)
#define cdraw_testVec4(test_x,test_y,test_z,test_w,tolerance,expect,func,result,...)																\
		cdraw_testVec3(test_x,test_y,test_z, tolerance, expect, func, result, __VA_ARGS__);															\
		asserterr(test_w(tolerance, expect[3], result[3]), errcode_CUSTOM_BEGIN)
#define cdraw_testVec2_rr(test_x,test_y,tolerance,expect0,expect1,func,result0,result1,...) failassertreset(); func(result0, result1, __VA_ARGS__);	\
		asserterr(test_x(tolerance, expect0[0], result0[0]), errcode_CUSTOM_BEGIN);																	\
		asserterr(test_x(tolerance, expect1[0], result1[0]), errcode_CUSTOM_BEGIN);																	\
		asserterr(test_x(tolerance, expect0[1], result0[1]), errcode_CUSTOM_BEGIN);																	\
		asserterr(test_y(tolerance, expect1[1], result1[1]), errcode_CUSTOM_BEGIN)
#define cdraw_testVec3_rr(test_x,test_y,test_z,tolerance,expect0,expect1,func,result0,result1,...)													\
		cdraw_testVec2_rr(test_x,test_y, tolerance, expect0, expect1, func, result0, result1, __VA_ARGS__);											\
		asserterr(test_z(tolerance, expect0[2], result0[2]), errcode_CUSTOM_BEGIN);																	\
		asserterr(test_z(tolerance, expect1[2], result1[2]), errcode_CUSTOM_BEGIN)
#define cdraw_testVec4_rr(test_x,test_y,test_z,test_w,tolerance,expect0,expect1,func,result0,result1,...)											\
		cdraw_testVec3_rr(test_x,test_y,test_z, tolerance, expect0, expect1, func, result0, result1, __VA_ARGS__);									\
		asserterr(test_w(tolerance, expect0[3], result0[3]), errcode_CUSTOM_BEGIN);																	\
		asserterr(test_w(tolerance, expect1[3], result1[3]), errcode_CUSTOM_BEGIN)

#define cdraw_testVec2B_1V(testname_x,testname_y,func,result,arg0)																\
		cdraw_testVec2(cdraw_testname(testname_x, _b), cdraw_testname(testname_y, _b), 0, vecID2b.v, func, result, arg0)
#define cdraw_testVec2B_2V(testname_x,testname_y,func,result,arg0,arg1)															\
		cdraw_testVec2(cdraw_testname(testname_x, _b), cdraw_testname(testname_y, _b), 0, vecID2b.v, func, result, arg0, arg1)
#define cdraw_testVec3B_1V(testname_x,testname_y,testname_z,func,result,arg0)																					\
		cdraw_testVec3(cdraw_testname(testname_x, _b), cdraw_testname(testname_y, _b), cdraw_testname(testname_z, _b), 0, vecID3b.v, func, result, arg0)
#define cdraw_testVec3B_2V(testname_x,testname_y,testname_z,func,result,arg0,arg1)																				\
		cdraw_testVec3(cdraw_testname(testname_x, _b), cdraw_testname(testname_y, _b), cdraw_testname(testname_z, _b), 0, vecID3b.v, func, result, arg0, arg1)
#define cdraw_testVec4B_1V(testname_x,testname_y,testname_z,testname_w,func,result,arg0)																										\
		cdraw_testVec4(cdraw_testname(testname_x, _b), cdraw_testname(testname_y, _b), cdraw_testname(testname_z, _b), cdraw_testname(testname_w, _b), 0, vecID4b.v, func, result, arg0)
#define cdraw_testVec4B_2V(testname_x,testname_y,testname_z,testname_w,func,result,arg0,arg1)																									\
		cdraw_testVec4(cdraw_testname(testname_x, _b), cdraw_testname(testname_y, _b), cdraw_testname(testname_z, _b), cdraw_testname(testname_w, _b), 0, vecID4b.v, func, result, arg0, arg1)

#define cdraw_testVecIU_rB1V(testname,funcname,arg0name)												\
		cdraw_testVec_rB(cdraw_testname(testname, _i), tokencat(funcname, i), tokencat(arg0name, i).v);	\
		cdraw_testVec_rB(cdraw_testname(testname, _u), tokencat(funcname, u), vecCopy4i4u(tokencat(arg0name, u).v, tokencat(arg0name, i).v))
#define cdraw_testVecIU_rS1V(testname,expect,funcname,arg0name)														\
		cdraw_testVec_rS(cdraw_testname(testname, _i), 0, expect, tokencat(funcname, i), tokencat(arg0name, i).v);	\
		cdraw_testVec_rS(cdraw_testname(testname, _u), 0, tokencat(expect, u), tokencat(funcname, u), vecCopy4i4u(tokencat(arg0name, u).v, tokencat(arg0name, i).v))
#define cdraw_testVecIU_rS2V(testname,expect,funcname,arg0name,arg1name)																	\
		cdraw_testVec_rS(cdraw_testname(testname, _i), 0, expect, tokencat(funcname, i), tokencat(arg0name, i).v, tokencat(arg1name, i).v);	\
		cdraw_testVec_rS(cdraw_testname(testname, _u), 0, tokencat(expect, u), tokencat(funcname, u), vecCopy4i4u(tokencat(arg0name, u).v, tokencat(arg0name, i).v), vecCopy4i4u(tokencat(arg1name, u).v, tokencat(arg1name, i).v))
#define cdraw_testVec2IU_rSV2V(testname,testname_x,testname_y,expect,expectname,funcname,resultname,arg0name,arg1name)																																				\
		cdraw_testVec2_rSV(cdraw_testname(testname, _i), cdraw_testname(testname_x, _i), cdraw_testname(testname_y, _i), 0, expect, tokencat(expectname, i).v, tokencat(funcname, i), tokencat(resultname, i).v, tokencat(arg0name, i).v, tokencat(arg1name, i).v);	\
		cdraw_testVec2_rSV(cdraw_testname(testname, _u), cdraw_testname(testname_x, _u), cdraw_testname(testname_y, _u), 0, expect, vecCopy4i4u(tokencat(expectname, u).v, tokencat(expectname, i).v), tokencat(funcname, u), tokencat(resultname, u).v, vecCopy4i4u(tokencat(arg0name, u).v, tokencat(arg0name, i).v), vecCopy4i4u(tokencat(arg1name, u).v, tokencat(arg1name, i).v))
#define cdraw_testVec3IU_rSV2V(testname,testname_x,testname_y,testname_z,expect,expectname,funcname,resultname,arg0name,arg1name)																																									\
		cdraw_testVec3_rSV(cdraw_testname(testname, _i), cdraw_testname(testname_x, _i), cdraw_testname(testname_y, _i), cdraw_testname(testname_z, _i), 0, expect, tokencat(expectname, i).v, tokencat(funcname, i), tokencat(resultname, i).v, tokencat(arg0name, i).v, tokencat(arg1name, i).v);	\
		cdraw_testVec3_rSV(cdraw_testname(testname, _u), cdraw_testname(testname_x, _u), cdraw_testname(testname_y, _u), cdraw_testname(testname_z, _u), 0, expect, vecCopy4i4u(tokencat(expectname, u).v, tokencat(expectname, i).v), tokencat(funcname, u), tokencat(resultname, u).v, vecCopy4i4u(tokencat(arg0name, u).v, tokencat(arg0name, i).v), vecCopy4i4u(tokencat(arg1name, u).v, tokencat(arg1name, i).v))
#define cdraw_testVec4IU_rSV2V(testname,testname_x,testname_y,testname_z,testname_w,expect,expectname,funcname,resultname,arg0name,arg1name)																																														\
		cdraw_testVec4_rSV(cdraw_testname(testname, _i), cdraw_testname(testname_x, _i), cdraw_testname(testname_y, _i), cdraw_testname(testname_z, _i), cdraw_testname(testname_w, _i), 0, expect, tokencat(expectname, i).v, tokencat(funcname, i), tokencat(resultname, i).v, tokencat(arg0name, i).v, tokencat(arg1name, i).v);	\
		cdraw_testVec4_rSV(cdraw_testname(testname, _u), cdraw_testname(testname_x, _u), cdraw_testname(testname_y, _u), cdraw_testname(testname_z, _u), cdraw_testname(testname_w, _u), 0, expect, vecCopy4i4u(tokencat(expectname, u).v, tokencat(expectname, i).v), tokencat(funcname, u), tokencat(resultname, u).v, vecCopy4i4u(tokencat(arg0name, u).v, tokencat(arg0name, i).v), vecCopy4i4u(tokencat(arg1name, u).v, tokencat(arg1name, i).v))
#define cdraw_testVec2IU_rvB2V(testname_x,testname_y,funcname,result,arg0name,arg1name)																							\
		cdraw_testVec2_rvB(cdraw_testname(testname_x, _i), cdraw_testname(testname_y, _i), tokencat(funcname, i), result, tokencat(arg0name, i).v, tokencat(arg1name, i).v);	\
		cdraw_testVec2_rvB(cdraw_testname(testname_x, _u), cdraw_testname(testname_y, _u), tokencat(funcname, u), result, vecCopy4i4u(tokencat(arg0name, u).v, tokencat(arg0name, i).v), vecCopy4i4u(tokencat(arg1name, u).v, tokencat(arg1name, i).v))
#define cdraw_testVec3IU_rvB2V(testname_x,testname_y,testname_z,funcname,result,arg0name,arg1name)																												\
		cdraw_testVec3_rvB(cdraw_testname(testname_x, _i), cdraw_testname(testname_y, _i), cdraw_testname(testname_z, _i), tokencat(funcname, i), result, tokencat(arg0name, i).v, tokencat(arg1name, i).v);	\
		cdraw_testVec3_rvB(cdraw_testname(testname_x, _u), cdraw_testname(testname_y, _u), cdraw_testname(testname_z, _u), tokencat(funcname, u), result, vecCopy4i4u(tokencat(arg0name, u).v, tokencat(arg0name, i).v), vecCopy4i4u(tokencat(arg1name, u).v, tokencat(arg1name, i).v))
#define cdraw_testVec4IU_rvB2V(testname_x,testname_y,testname_z,testname_w,funcname,result,arg0name,arg1name)																																	\
		cdraw_testVec4_rvB(cdraw_testname(testname_x, _i), cdraw_testname(testname_y, _i), cdraw_testname(testname_z, _i), cdraw_testname(testname_w, _i), tokencat(funcname, i), result, tokencat(arg0name, i).v, tokencat(arg1name, i).v);	\
		cdraw_testVec4_rvB(cdraw_testname(testname_x, _u), cdraw_testname(testname_y, _u), cdraw_testname(testname_z, _u), cdraw_testname(testname_w, _u), tokencat(funcname, u), result, vecCopy4i4u(tokencat(arg0name, u).v, tokencat(arg0name, i).v), vecCopy4i4u(tokencat(arg1name, u).v, tokencat(arg1name, i).v))
#define cdraw_testVec2IU_1V(testname_x,testname_y,expectname,funcname,resultname,arg0name)																											\
		cdraw_testVec2(cdraw_testname(testname_x, _i), cdraw_testname(testname_y, _i), 0, tokencat(expectname, i).v, tokencat(funcname, i), tokencat(resultname, i).v, tokencat(arg0name, i).v);	\
		cdraw_testVec2(cdraw_testname(testname_x, _u), cdraw_testname(testname_y, _u), 0, vecCopy4i4u(tokencat(expectname, u).v, tokencat(expectname, i).v), tokencat(funcname, u), tokencat(resultname, u).v, vecCopy4i4u(tokencat(arg0name, u).v, tokencat(arg0name, i).v))
#define cdraw_testVec3IU_1V(testname_x,testname_y,testname_z,expectname,funcname,resultname,arg0name)																																\
		cdraw_testVec3(cdraw_testname(testname_x, _i), cdraw_testname(testname_y, _i), cdraw_testname(testname_z, _i), 0, tokencat(expectname, i).v, tokencat(funcname, i), tokencat(resultname, i).v, tokencat(arg0name, i).v);	\
		cdraw_testVec3(cdraw_testname(testname_x, _u), cdraw_testname(testname_y, _u), cdraw_testname(testname_z, _u), 0, vecCopy4i4u(tokencat(expectname, u).v, tokencat(expectname, i).v), tokencat(funcname, u), tokencat(resultname, u).v, vecCopy4i4u(tokencat(arg0name, u).v, tokencat(arg0name, i).v))
#define cdraw_testVec4IU_1V(testname_x,testname_y,testname_z,testname_w,expectname,funcname,resultname,arg0name)																																					\
		cdraw_testVec4(cdraw_testname(testname_x, _i), cdraw_testname(testname_y, _i), cdraw_testname(testname_z, _i), cdraw_testname(testname_w, _i), 0, tokencat(expectname, i).v, tokencat(funcname, i), tokencat(resultname, i).v, tokencat(arg0name, i).v);	\
		cdraw_testVec4(cdraw_testname(testname_x, _u), cdraw_testname(testname_y, _u), cdraw_testname(testname_z, _u), cdraw_testname(testname_w, _u), 0, vecCopy4i4u(tokencat(expectname, u).v, tokencat(expectname, i).v), tokencat(funcname, u), tokencat(resultname, u).v, vecCopy4i4u(tokencat(arg0name, u).v, tokencat(arg0name, i).v))
#define cdraw_testVec2IU_1V1S(testname_x,testname_y,expectname,funcname,resultname,arg0name,arg1)																										\
		cdraw_testVec2(cdraw_testname(testname_x, _i), cdraw_testname(testname_y, _i), 0, tokencat(expectname, i).v, tokencat(funcname, i), tokencat(resultname, i).v, tokencat(arg0name, i).v, arg1);	\
		cdraw_testVec2(cdraw_testname(testname_x, _u), cdraw_testname(testname_y, _u), 0, vecCopy4i4u(tokencat(expectname, u).v, tokencat(expectname, i).v), tokencat(funcname, u), tokencat(resultname, u).v, vecCopy4i4u(tokencat(arg0name, u).v, tokencat(arg0name, i).v), tokencat(arg1, u))
#define cdraw_testVec3IU_1V1S(testname_x,testname_y,testname_z,expectname,funcname,resultname,arg0name,arg1)																															\
		cdraw_testVec3(cdraw_testname(testname_x, _i), cdraw_testname(testname_y, _i), cdraw_testname(testname_z, _i), 0, tokencat(expectname, i).v, tokencat(funcname, i), tokencat(resultname, i).v, tokencat(arg0name, i).v, arg1);	\
		cdraw_testVec3(cdraw_testname(testname_x, _u), cdraw_testname(testname_y, _u), cdraw_testname(testname_z, _u), 0, vecCopy4i4u(tokencat(expectname, u).v, tokencat(expectname, i).v), tokencat(funcname, u), tokencat(resultname, u).v, vecCopy4i4u(tokencat(arg0name, u).v, tokencat(arg0name, i).v), tokencat(arg1, u))
#define cdraw_testVec4IU_1V1S(testname_x,testname_y,testname_z,testname_w,expectname,funcname,resultname,arg0name,arg1)																																					\
		cdraw_testVec4(cdraw_testname(testname_x, _i), cdraw_testname(testname_y, _i), cdraw_testname(testname_z, _i), cdraw_testname(testname_w, _i), 0, tokencat(expectname, i).v, tokencat(funcname, i), tokencat(resultname, i).v, tokencat(arg0name, i).v, arg1);	\
		cdraw_testVec4(cdraw_testname(testname_x, _u), cdraw_testname(testname_y, _u), cdraw_testname(testname_z, _u), cdraw_testname(testname_w, _u), 0, vecCopy4i4u(tokencat(expectname, u).v, tokencat(expectname, i).v), tokencat(funcname, u), tokencat(resultname, u).v, vecCopy4i4u(tokencat(arg0name, u).v, tokencat(arg0name, i).v), tokencat(arg1, u))
#define cdraw_testVec2IU_2V(testname_x,testname_y,expectname,funcname,resultname,arg0name,arg1name)																															\
		cdraw_testVec2(cdraw_testname(testname_x, _i), cdraw_testname(testname_y, _i), 0, tokencat(expectname, i).v, tokencat(funcname, i), tokencat(resultname, i).v, tokencat(arg0name, i).v, tokencat(arg1name, i).v);	\
		cdraw_testVec2(cdraw_testname(testname_x, _u), cdraw_testname(testname_y, _u), 0, vecCopy4i4u(tokencat(expectname, u).v, tokencat(expectname, i).v), tokencat(funcname, u), tokencat(resultname, u).v, vecCopy4i4u(tokencat(arg0name, u).v, tokencat(arg0name, i).v), vecCopy4i4u(tokencat(arg1name, u).v, tokencat(arg1name, i).v))
#define cdraw_testVec3IU_2V(testname_x,testname_y,testname_z,expectname,funcname,resultname,arg0name,arg1name)																																				\
		cdraw_testVec3(cdraw_testname(testname_x, _i), cdraw_testname(testname_y, _i), cdraw_testname(testname_z, _i), 0, tokencat(expectname, i).v, tokencat(funcname, i), tokencat(resultname, i).v, tokencat(arg0name, i).v, tokencat(arg1name, i).v);	\
		cdraw_testVec3(cdraw_testname(testname_x, _u), cdraw_testname(testname_y, _u), cdraw_testname(testname_z, _u), 0, vecCopy4i4u(tokencat(expectname, u).v, tokencat(expectname, i).v), tokencat(funcname, u), tokencat(resultname, u).v, vecCopy4i4u(tokencat(arg0name, u).v, tokencat(arg0name, i).v), vecCopy4i4u(tokencat(arg1name, u).v, tokencat(arg1name, i).v))
#define cdraw_testVec4IU_2V(testname_x,testname_y,testname_z,testname_w,expectname,funcname,resultname,arg0name,arg1name)																																									\
		cdraw_testVec4(cdraw_testname(testname_x, _i), cdraw_testname(testname_y, _i), cdraw_testname(testname_z, _i), cdraw_testname(testname_w, _i), 0, tokencat(expectname, i).v, tokencat(funcname, i), tokencat(resultname, i).v, tokencat(arg0name, i).v, tokencat(arg1name, i).v);	\
		cdraw_testVec4(cdraw_testname(testname_x, _u), cdraw_testname(testname_y, _u), cdraw_testname(testname_z, _u), cdraw_testname(testname_w, _u), 0, vecCopy4i4u(tokencat(expectname, u).v, tokencat(expectname, i).v), tokencat(funcname, u), tokencat(resultname, u).v, vecCopy4i4u(tokencat(arg0name, u).v, tokencat(arg0name, i).v), vecCopy4i4u(tokencat(arg1name, u).v, tokencat(arg1name, i).v))
#define cdraw_testVec2IU_1S2V(testname_x,testname_y,expectname,funcname,resultname,arg0,arg1name,arg2name)																														\
		cdraw_testVec2(cdraw_testname(testname_x, _i), cdraw_testname(testname_y, _i), 0, tokencat(expectname, i).v, tokencat(funcname, i), tokencat(resultname, i).v, arg0, tokencat(arg1name, i).v, tokencat(arg2name, i).v);	\
		cdraw_testVec2(cdraw_testname(testname_x, _u), cdraw_testname(testname_y, _u), 0, vecCopy4i4u(tokencat(expectname, u).v, tokencat(expectname, i).v), tokencat(funcname, u), tokencat(resultname, u).v, tokencat(arg0, u), vecCopy4i4u(tokencat(arg1name, u).v, tokencat(arg1name, i).v), vecCopy4i4u(tokencat(arg2name, u).v, tokencat(arg2name, i).v))
#define cdraw_testVec3IU_1S2V(testname_x,testname_y,testname_z,expectname,funcname,resultname,arg0,arg1name,arg2name)																																			\
		cdraw_testVec3(cdraw_testname(testname_x, _i), cdraw_testname(testname_y, _i), cdraw_testname(testname_z, _i), 0, tokencat(expectname, i).v, tokencat(funcname, i), tokencat(resultname, i).v, arg0, tokencat(arg1name, i).v, tokencat(arg2name, i).v);	\
		cdraw_testVec3(cdraw_testname(testname_x, _u), cdraw_testname(testname_y, _u), cdraw_testname(testname_z, _u), 0, vecCopy4i4u(tokencat(expectname, u).v, tokencat(expectname, i).v), tokencat(funcname, u), tokencat(resultname, u).v, tokencat(arg0, u), vecCopy4i4u(tokencat(arg1name, u).v, tokencat(arg1name, i).v), vecCopy4i4u(tokencat(arg2name, u).v, tokencat(arg2name, i).v))
#define cdraw_testVec4IU_1S2V(testname_x,testname_y,testname_z,testname_w,expectname,funcname,resultname,arg0,arg1name,arg2name)																																								\
		cdraw_testVec4(cdraw_testname(testname_x, _i), cdraw_testname(testname_y, _i), cdraw_testname(testname_z, _i), cdraw_testname(testname_w, _i), 0, tokencat(expectname, i).v, tokencat(funcname, i), tokencat(resultname, i).v, arg0, tokencat(arg1name, i).v, tokencat(arg2name, i).v);	\
		cdraw_testVec4(cdraw_testname(testname_x, _u), cdraw_testname(testname_y, _u), cdraw_testname(testname_z, _u), cdraw_testname(testname_w, _u), 0, vecCopy4i4u(tokencat(expectname, u).v, tokencat(expectname, i).v), tokencat(funcname, u), tokencat(resultname, u).v, tokencat(arg0, u), vecCopy4i4u(tokencat(arg1name, u).v, tokencat(arg1name, i).v), vecCopy4i4u(tokencat(arg2name, u).v, tokencat(arg2name, i).v))
#define cdraw_testVec2IU_rr2V(testname_x,testname_y,expect0name,expect1name,funcname,result0name,result1name,arg0name,arg1name)																																							\
		cdraw_testVec2_rr(cdraw_testname(testname_x, _i), cdraw_testname(testname_y, _i), 0, tokencat(expect0name, i).v, tokencat(expect1name, i).v, tokencat(funcname, i), tokencat(result0name, i).v, tokencat(result1name, i).v, tokencat(arg0name, i).v, tokencat(arg1name, i).v);	\
		cdraw_testVec2_rr(cdraw_testname(testname_x, _u), cdraw_testname(testname_y, _u), 0, vecCopy4i4u(tokencat(expect0name, u).v, tokencat(expect0name, i).v), vecCopy4i4u(tokencat(expect1name, u).v, tokencat(expect1name, i).v), tokencat(funcname, u), tokencat(result0name, u).v, tokencat(result1name, u).v, vecCopy4i4u(tokencat(arg0name, u).v, tokencat(arg0name, i).v), vecCopy4i4u(tokencat(arg1name, u).v, tokencat(arg1name, i).v))
#define cdraw_testVec3IU_rr2V(testname_x,testname_y,testname_z,expect0name,expect1name,funcname,result0name,result1name,arg0name,arg1name)																																												\
		cdraw_testVec3_rr(cdraw_testname(testname_x, _i), cdraw_testname(testname_y, _i), cdraw_testname(testname_z, _i), 0, tokencat(expect0name, i).v, tokencat(expect1name, i).v, tokencat(funcname, i), tokencat(result0name, i).v, tokencat(result1name, i).v, tokencat(arg0name, i).v, tokencat(arg1name, i).v);	\
		cdraw_testVec3_rr(cdraw_testname(testname_x, _u), cdraw_testname(testname_y, _u), cdraw_testname(testname_z, _u), 0, vecCopy4i4u(tokencat(expect0name, u).v, tokencat(expect0name, i).v), vecCopy4i4u(tokencat(expect1name, u).v, tokencat(expect1name, i).v), tokencat(funcname, u), tokencat(result0name, u).v, tokencat(result1name, u).v, vecCopy4i4u(tokencat(arg0name, u).v, tokencat(arg0name, i).v), vecCopy4i4u(tokencat(arg1name, u).v, tokencat(arg1name, i).v))
#define cdraw_testVec4IU_rr2V(testname_x,testname_y,testname_z,testname_w,expect0name,expect1name,funcname,result0name,result1name,arg0name,arg1name)																																																	\
		cdraw_testVec4_rr(cdraw_testname(testname_x, _i), cdraw_testname(testname_y, _i), cdraw_testname(testname_z, _i), cdraw_testname(testname_w, _i), 0, tokencat(expect0name, i).v, tokencat(expect1name, i).v, tokencat(funcname, i), tokencat(result0name, i).v, tokencat(result1name, i).v, tokencat(arg0name, i).v, tokencat(arg1name, i).v);	\
		cdraw_testVec4_rr(cdraw_testname(testname_x, _u), cdraw_testname(testname_y, _u), cdraw_testname(testname_z, _u), cdraw_testname(testname_w, _u), 0, vecCopy4i4u(tokencat(expect0name, u).v, tokencat(expect0name, i).v), vecCopy4i4u(tokencat(expect1name, u).v, tokencat(expect1name, i).v), tokencat(funcname, u), tokencat(result0name, u).v, tokencat(result1name, u).v, vecCopy4i4u(tokencat(arg0name, u).v, tokencat(arg0name, i).v), vecCopy4i4u(tokencat(arg1name, u).v, tokencat(arg1name, i).v))
#define cdraw_testVec2IU_rr1V1S(testname_x,testname_y,expect0name,expect1name,funcname,result0name,result1name,arg0name,arg1)																																		\
		cdraw_testVec2_rr(cdraw_testname(testname_x, _i), cdraw_testname(testname_y, _i), 0, tokencat(expect0name, i).v, tokencat(expect1name, i).v, tokencat(funcname, i), tokencat(result0name, i).v, tokencat(result1name, i).v, tokencat(arg0name, i).v, arg1);	\
		cdraw_testVec2_rr(cdraw_testname(testname_x, _u), cdraw_testname(testname_y, _u), 0, vecCopy4i4u(tokencat(expect0name, u).v, tokencat(expect0name, i).v), vecCopy4i4u(tokencat(expect1name, u).v, tokencat(expect1name, i).v), tokencat(funcname, u), tokencat(result0name, u).v, tokencat(result1name, u).v, vecCopy4i4u(tokencat(arg0name, u).v, tokencat(arg0name, i).v), tokencat(arg1, u))
#define cdraw_testVec3IU_rr1V1S(testname_x,testname_y,testname_z,expect0name,expect1name,funcname,result0name,result1name,arg0name,arg1)																																							\
		cdraw_testVec3_rr(cdraw_testname(testname_x, _i), cdraw_testname(testname_y, _i), cdraw_testname(testname_z, _i), 0, tokencat(expect0name, i).v, tokencat(expect1name, i).v, tokencat(funcname, i), tokencat(result0name, i).v, tokencat(result1name, i).v, tokencat(arg0name, i).v, arg1);	\
		cdraw_testVec3_rr(cdraw_testname(testname_x, _u), cdraw_testname(testname_y, _u), cdraw_testname(testname_z, _u), 0, vecCopy4i4u(tokencat(expect0name, u).v, tokencat(expect0name, i).v), vecCopy4i4u(tokencat(expect1name, u).v, tokencat(expect1name, i).v), tokencat(funcname, u), tokencat(result0name, u).v, tokencat(result1name, u).v, vecCopy4i4u(tokencat(arg0name, u).v, tokencat(arg0name, i).v), tokencat(arg1, u))
#define cdraw_testVec4IU_rr1V1S(testname_x,testname_y,testname_z,testname_w,expect0name,expect1name,funcname,result0name,result1name,arg0name,arg1)																																													\
		cdraw_testVec4_rr(cdraw_testname(testname_x, _i), cdraw_testname(testname_y, _i), cdraw_testname(testname_z, _i), cdraw_testname(testname_w, _i), 0, tokencat(expect0name, i).v, tokencat(expect1name, i).v, tokencat(funcname, i), tokencat(result0name, i).v, tokencat(result1name, i).v, tokencat(arg0name, i).v, arg1);	\
		cdraw_testVec4_rr(cdraw_testname(testname_x, _u), cdraw_testname(testname_y, _u), cdraw_testname(testname_z, _u), cdraw_testname(testname_w, _u), 0, vecCopy4i4u(tokencat(expect0name, u).v, tokencat(expect0name, i).v), vecCopy4i4u(tokencat(expect1name, u).v, tokencat(expect1name, i).v), tokencat(funcname, u), tokencat(result0name, u).v, tokencat(result1name, u).v, vecCopy4i4u(tokencat(arg0name, u).v, tokencat(arg0name, i).v), tokencat(arg1, u))

#define cdraw_testVecDF_rB1V(testname,funcname,arg0name)												\
		cdraw_testVec_rB(cdraw_testname(testname, _d), tokencat(funcname, d), tokencat(arg0name, d).v);	\
		cdraw_testVec_rB(cdraw_testname(testname, _f), tokencat(funcname, f), vecCopy4d4f(tokencat(arg0name, f).v, tokencat(arg0name, d).v))
#define cdraw_testVecDF_rS1V(testname,expect,funcname,arg0name)																	\
		cdraw_testVec_rS(cdraw_testname(testname, _d), (scBigEpsD), expect, tokencat(funcname, d), tokencat(arg0name, d).v);	\
		cdraw_testVec_rS(cdraw_testname(testname, _f), (scBigEpsF), tokencat(expect, f), tokencat(funcname, f), vecCopy4d4f(tokencat(arg0name, f).v, tokencat(arg0name, d).v))
#define cdraw_testVecDF_rS2V(testname,expect,funcname,arg0name,arg1name)																				\
		cdraw_testVec_rS(cdraw_testname(testname, _d), (scBigEpsD), expect, tokencat(funcname, d), tokencat(arg0name, d).v, tokencat(arg1name, d).v);	\
		cdraw_testVec_rS(cdraw_testname(testname, _f), (scBigEpsF), tokencat(expect, f), tokencat(funcname, f), vecCopy4d4f(tokencat(arg0name, f).v, tokencat(arg0name, d).v), vecCopy4d4f(tokencat(arg1name, f).v, tokencat(arg1name, d).v))
#define cdraw_testVecDF_rS3V(testname,expect,funcname,arg0name,arg1name,arg2name)																								\
		cdraw_testVec_rS(cdraw_testname(testname, _d), (scBigEpsD), expect, tokencat(funcname, d), tokencat(arg0name, d).v, tokencat(arg1name, d).v, tokencat(arg2name, d).v);	\
		cdraw_testVec_rS(cdraw_testname(testname, _f), (scBigEpsF), tokencat(expect, f), tokencat(funcname, f), vecCopy4d4f(tokencat(arg0name, f).v, tokencat(arg0name, d).v), vecCopy4d4f(tokencat(arg1name, f).v, tokencat(arg1name, d).v), vecCopy4d4f(tokencat(arg2name, f).v, tokencat(arg2name, d).v))
#define cdraw_testVec2DF_rSV2V(testname,testname_x,testname_y,expect,expectname,funcname,resultname,arg0name,arg1name)																																							\
		cdraw_testVec2_rSV(cdraw_testname(testname, _d), cdraw_testname(testname_x, _d), cdraw_testname(testname_y, _d), (scBigEpsD), expect, tokencat(expectname, d).v, tokencat(funcname, d), tokencat(resultname, d).v, tokencat(arg0name, d).v, tokencat(arg1name, d).v);	\
		cdraw_testVec2_rSV(cdraw_testname(testname, _f), cdraw_testname(testname_x, _f), cdraw_testname(testname_y, _f), (scBigEpsF), expect, vecCopy4d4f(tokencat(expectname, f).v, tokencat(expectname, d).v), tokencat(funcname, f), tokencat(resultname, f).v, vecCopy4d4f(tokencat(arg0name, f).v, tokencat(arg0name, d).v), vecCopy4d4f(tokencat(arg1name, f).v, tokencat(arg1name, d).v))
#define cdraw_testVec3DF_rSV2V(testname,testname_x,testname_y,testname_z,expect,expectname,funcname,resultname,arg0name,arg1name)																																												\
		cdraw_testVec3_rSV(cdraw_testname(testname, _d), cdraw_testname(testname_x, _d), cdraw_testname(testname_y, _d), cdraw_testname(testname_z, _d), (scBigEpsD), expect, tokencat(expectname, d).v, tokencat(funcname, d), tokencat(resultname, d).v, tokencat(arg0name, d).v, tokencat(arg1name, d).v);	\
		cdraw_testVec3_rSV(cdraw_testname(testname, _f), cdraw_testname(testname_x, _f), cdraw_testname(testname_y, _f), cdraw_testname(testname_z, _f), (scBigEpsF), expect, vecCopy4d4f(tokencat(expectname, f).v, tokencat(expectname, d).v), tokencat(funcname, f), tokencat(resultname, f).v, vecCopy4d4f(tokencat(arg0name, f).v, tokencat(arg0name, d).v), vecCopy4d4f(tokencat(arg1name, f).v, tokencat(arg1name, d).v))
#define cdraw_testVec4DF_rSV2V(testname,testname_x,testname_y,testname_z,testname_w,expect,expectname,funcname,resultname,arg0name,arg1name)																																																	\
		cdraw_testVec4_rSV(cdraw_testname(testname, _d), cdraw_testname(testname_x, _d), cdraw_testname(testname_y, _d), cdraw_testname(testname_z, _d), cdraw_testname(testname_w, _d), (scBigEpsD), expect, tokencat(expectname, d).v, tokencat(funcname, d), tokencat(resultname, d).v, tokencat(arg0name, d).v, tokencat(arg1name, d).v);	\
		cdraw_testVec4_rSV(cdraw_testname(testname, _f), cdraw_testname(testname_x, _f), cdraw_testname(testname_y, _f), cdraw_testname(testname_z, _f), cdraw_testname(testname_w, _f), (scBigEpsF), expect, vecCopy4d4f(tokencat(expectname, f).v, tokencat(expectname, d).v), tokencat(funcname, f), tokencat(resultname, f).v, vecCopy4d4f(tokencat(arg0name, f).v, tokencat(arg0name, d).v), vecCopy4d4f(tokencat(arg1name, f).v, tokencat(arg1name, d).v))
#define cdraw_testVec2DF_rvB2V(testname_x,testname_y,funcname,result,arg0name,arg1name)																							\
		cdraw_testVec2_rvB(cdraw_testname(testname_x, _d), cdraw_testname(testname_y, _d), tokencat(funcname, d), result, tokencat(arg0name, d).v, tokencat(arg1name, d).v);	\
		cdraw_testVec2_rvB(cdraw_testname(testname_x, _f), cdraw_testname(testname_y, _f), tokencat(funcname, f), result, vecCopy4d4f(tokencat(arg0name, f).v, tokencat(arg0name, d).v), vecCopy4d4f(tokencat(arg1name, f).v, tokencat(arg1name, d).v))
#define cdraw_testVec3DF_rvB2V(testname_x,testname_y,testname_z,funcname,result,arg0name,arg1name)																												\
		cdraw_testVec3_rvB(cdraw_testname(testname_x, _d), cdraw_testname(testname_y, _d), cdraw_testname(testname_z, _d), tokencat(funcname, d), result, tokencat(arg0name, d).v, tokencat(arg1name, d).v);	\
		cdraw_testVec3_rvB(cdraw_testname(testname_x, _f), cdraw_testname(testname_y, _f), cdraw_testname(testname_z, _f), tokencat(funcname, f), result, vecCopy4d4f(tokencat(arg0name, f).v, tokencat(arg0name, d).v), vecCopy4d4f(tokencat(arg1name, f).v, tokencat(arg1name, d).v))
#define cdraw_testVec4DF_rvB2V(testname_x,testname_y,testname_z,testname_w,funcname,result,arg0name,arg1name)																																	\
		cdraw_testVec4_rvB(cdraw_testname(testname_x, _d), cdraw_testname(testname_y, _d), cdraw_testname(testname_z, _d), cdraw_testname(testname_w, _d), tokencat(funcname, d), result, tokencat(arg0name, d).v, tokencat(arg1name, d).v);	\
		cdraw_testVec4_rvB(cdraw_testname(testname_x, _f), cdraw_testname(testname_y, _f), cdraw_testname(testname_z, _f), cdraw_testname(testname_w, _f), tokencat(funcname, f), result, vecCopy4d4f(tokencat(arg0name, f).v, tokencat(arg0name, d).v), vecCopy4d4f(tokencat(arg1name, f).v, tokencat(arg1name, d).v))
#define cdraw_testVec2DF_1V(testname_x,testname_y,expectname,funcname,resultname,arg0name)																													\
		cdraw_testVec2(cdraw_testname(testname_x, _d), cdraw_testname(testname_y, _d), (scBigEpsD), tokencat(expectname, d).v, tokencat(funcname, d), tokencat(resultname, d).v, tokencat(arg0name, d).v);	\
		cdraw_testVec2(cdraw_testname(testname_x, _f), cdraw_testname(testname_y, _f), (scBigEpsF), vecCopy4d4f(tokencat(expectname, f).v, tokencat(expectname, d).v), tokencat(funcname, f), tokencat(resultname, f).v, vecCopy4d4f(tokencat(arg0name, f).v, tokencat(arg0name, d).v))
#define cdraw_testVec3DF_1V(testname_x,testname_y,testname_z,expectname,funcname,resultname,arg0name)																																		\
		cdraw_testVec3(cdraw_testname(testname_x, _d), cdraw_testname(testname_y, _d), cdraw_testname(testname_z, _d), (scBigEpsD), tokencat(expectname, d).v, tokencat(funcname, d), tokencat(resultname, d).v, tokencat(arg0name, d).v);	\
		cdraw_testVec3(cdraw_testname(testname_x, _f), cdraw_testname(testname_y, _f), cdraw_testname(testname_z, _f), (scBigEpsF), vecCopy4d4f(tokencat(expectname, f).v, tokencat(expectname, d).v), tokencat(funcname, f), tokencat(resultname, f).v, vecCopy4d4f(tokencat(arg0name, f).v, tokencat(arg0name, d).v))
#define cdraw_testVec4DF_1V(testname_x,testname_y,testname_z,testname_w,expectname,funcname,resultname,arg0name)																																							\
		cdraw_testVec4(cdraw_testname(testname_x, _d), cdraw_testname(testname_y, _d), cdraw_testname(testname_z, _d), cdraw_testname(testname_w, _d), (scBigEpsD), tokencat(expectname, d).v, tokencat(funcname, d), tokencat(resultname, d).v, tokencat(arg0name, d).v);	\
		cdraw_testVec4(cdraw_testname(testname_x, _f), cdraw_testname(testname_y, _f), cdraw_testname(testname_z, _f), cdraw_testname(testname_w, _f), (scBigEpsF), vecCopy4d4f(tokencat(expectname, f).v, tokencat(expectname, d).v), tokencat(funcname, f), tokencat(resultname, f).v, vecCopy4d4f(tokencat(arg0name, f).v, tokencat(arg0name, d).v))
#define cdraw_testVec2DF_1V1S(testname_x,testname_y,expectname,funcname,resultname,arg0name,arg1)																													\
		cdraw_testVec2(cdraw_testname(testname_x, _d), cdraw_testname(testname_y, _d), (scBigEpsD), tokencat(expectname, d).v, tokencat(funcname, d), tokencat(resultname, d).v, tokencat(arg0name, d).v, arg1);	\
		cdraw_testVec2(cdraw_testname(testname_x, _f), cdraw_testname(testname_y, _f), (scBigEpsF), vecCopy4d4f(tokencat(expectname, f).v, tokencat(expectname, d).v), tokencat(funcname, f), tokencat(resultname, f).v, vecCopy4d4f(tokencat(arg0name, f).v, tokencat(arg0name, d).v), tokencat(arg1, f))
#define cdraw_testVec3DF_1V1S(testname_x,testname_y,testname_z,expectname,funcname,resultname,arg0name,arg1)																																		\
		cdraw_testVec3(cdraw_testname(testname_x, _d), cdraw_testname(testname_y, _d), cdraw_testname(testname_z, _d), (scBigEpsD), tokencat(expectname, d).v, tokencat(funcname, d), tokencat(resultname, d).v, tokencat(arg0name, d).v, arg1);	\
		cdraw_testVec3(cdraw_testname(testname_x, _f), cdraw_testname(testname_y, _f), cdraw_testname(testname_z, _f), (scBigEpsF), vecCopy4d4f(tokencat(expectname, f).v, tokencat(expectname, d).v), tokencat(funcname, f), tokencat(resultname, f).v, vecCopy4d4f(tokencat(arg0name, f).v, tokencat(arg0name, d).v), tokencat(arg1, f))
#define cdraw_testVec4DF_1V1S(testname_x,testname_y,testname_z,testname_w,expectname,funcname,resultname,arg0name,arg1)																																								\
		cdraw_testVec4(cdraw_testname(testname_x, _d), cdraw_testname(testname_y, _d), cdraw_testname(testname_z, _d), cdraw_testname(testname_w, _d), (scBigEpsD), tokencat(expectname, d).v, tokencat(funcname, d), tokencat(resultname, d).v, tokencat(arg0name, d).v, arg1);	\
		cdraw_testVec4(cdraw_testname(testname_x, _f), cdraw_testname(testname_y, _f), cdraw_testname(testname_z, _f), cdraw_testname(testname_w, _f), (scBigEpsF), vecCopy4d4f(tokencat(expectname, f).v, tokencat(expectname, d).v), tokencat(funcname, f), tokencat(resultname, f).v, vecCopy4d4f(tokencat(arg0name, f).v, tokencat(arg0name, d).v), tokencat(arg1, f))
#define cdraw_testVec2DF_2V(testname_x,testname_y,expectname,funcname,resultname,arg0name,arg1name)																																	\
		cdraw_testVec2(cdraw_testname(testname_x, _d), cdraw_testname(testname_y, _d), (scBigEpsD), tokencat(expectname, d).v, tokencat(funcname, d), tokencat(resultname, d).v, tokencat(arg0name, d).v, tokencat(arg1name, d).v);	\
		cdraw_testVec2(cdraw_testname(testname_x, _f), cdraw_testname(testname_y, _f), (scBigEpsF), vecCopy4d4f(tokencat(expectname, f).v, tokencat(expectname, d).v), tokencat(funcname, f), tokencat(resultname, f).v, vecCopy4d4f(tokencat(arg0name, f).v, tokencat(arg0name, d).v), vecCopy4d4f(tokencat(arg1name, f).v, tokencat(arg1name, d).v))
#define cdraw_testVec3DF_2V(testname_x,testname_y,testname_z,expectname,funcname,resultname,arg0name,arg1name)																																						\
		cdraw_testVec3(cdraw_testname(testname_x, _d), cdraw_testname(testname_y, _d), cdraw_testname(testname_z, _d), (scBigEpsD), tokencat(expectname, d).v, tokencat(funcname, d), tokencat(resultname, d).v, tokencat(arg0name, d).v, tokencat(arg1name, d).v);	\
		cdraw_testVec3(cdraw_testname(testname_x, _f), cdraw_testname(testname_y, _f), cdraw_testname(testname_z, _f), (scBigEpsF), vecCopy4d4f(tokencat(expectname, f).v, tokencat(expectname, d).v), tokencat(funcname, f), tokencat(resultname, f).v, vecCopy4d4f(tokencat(arg0name, f).v, tokencat(arg0name, d).v), vecCopy4d4f(tokencat(arg1name, f).v, tokencat(arg1name, d).v))
#define cdraw_testVec4DF_2V(testname_x,testname_y,testname_z,testname_w,expectname,funcname,resultname,arg0name,arg1name)																																											\
		cdraw_testVec4(cdraw_testname(testname_x, _d), cdraw_testname(testname_y, _d), cdraw_testname(testname_z, _d), cdraw_testname(testname_w, _d), (scBigEpsD), tokencat(expectname, d).v, tokencat(funcname, d), tokencat(resultname, d).v, tokencat(arg0name, d).v, tokencat(arg1name, d).v);	\
		cdraw_testVec4(cdraw_testname(testname_x, _f), cdraw_testname(testname_y, _f), cdraw_testname(testname_z, _f), cdraw_testname(testname_w, _f), (scBigEpsF), vecCopy4d4f(tokencat(expectname, f).v, tokencat(expectname, d).v), tokencat(funcname, f), tokencat(resultname, f).v, vecCopy4d4f(tokencat(arg0name, f).v, tokencat(arg0name, d).v), vecCopy4d4f(tokencat(arg1name, f).v, tokencat(arg1name, d).v))
#define cdraw_testVec2DF_1S2V(testname_x,testname_y,expectname,funcname,resultname,arg0,arg1name,arg2name)																																	\
		cdraw_testVec2(cdraw_testname(testname_x, _d), cdraw_testname(testname_y, _d), (scBigEpsD), tokencat(expectname, d).v, tokencat(funcname, d), tokencat(resultname, d).v, arg0, tokencat(arg1name, d).v, tokencat(arg2name, d).v);	\
		cdraw_testVec2(cdraw_testname(testname_x, _f), cdraw_testname(testname_y, _f), (scBigEpsF), vecCopy4d4f(tokencat(expectname, f).v, tokencat(expectname, d).v), tokencat(funcname, f), tokencat(resultname, f).v, tokencat(arg0, f), vecCopy4d4f(tokencat(arg1name, f).v, tokencat(arg1name, d).v), vecCopy4d4f(tokencat(arg2name, f).v, tokencat(arg2name, d).v))
#define cdraw_testVec3DF_1S2V(testname_x,testname_y,testname_z,expectname,funcname,resultname,arg0,arg1name,arg2name)																																						\
		cdraw_testVec3(cdraw_testname(testname_x, _d), cdraw_testname(testname_y, _d), cdraw_testname(testname_z, _d), (scBigEpsD), tokencat(expectname, d).v, tokencat(funcname, d), tokencat(resultname, d).v, arg0, tokencat(arg1name, d).v, tokencat(arg2name, d).v);	\
		cdraw_testVec3(cdraw_testname(testname_x, _f), cdraw_testname(testname_y, _f), cdraw_testname(testname_z, _f), (scBigEpsF), vecCopy4d4f(tokencat(expectname, f).v, tokencat(expectname, d).v), tokencat(funcname, f), tokencat(resultname, f).v, tokencat(arg0, f), vecCopy4d4f(tokencat(arg1name, f).v, tokencat(arg1name, d).v), vecCopy4d4f(tokencat(arg2name, f).v, tokencat(arg2name, d).v))
#define cdraw_testVec4DF_1S2V(testname_x,testname_y,testname_z,testname_w,expectname,funcname,resultname,arg0,arg1name,arg2name)																																											\
		cdraw_testVec4(cdraw_testname(testname_x, _d), cdraw_testname(testname_y, _d), cdraw_testname(testname_z, _d), cdraw_testname(testname_w, _d), (scBigEpsD), tokencat(expectname, d).v, tokencat(funcname, d), tokencat(resultname, d).v, arg0, tokencat(arg1name, d).v, tokencat(arg2name, d).v);	\
		cdraw_testVec4(cdraw_testname(testname_x, _f), cdraw_testname(testname_y, _f), cdraw_testname(testname_z, _f), cdraw_testname(testname_w, _f), (scBigEpsF), vecCopy4d4f(tokencat(expectname, f).v, tokencat(expectname, d).v), tokencat(funcname, f), tokencat(resultname, f).v, tokencat(arg0, f), vecCopy4d4f(tokencat(arg1name, f).v, tokencat(arg1name, d).v), vecCopy4d4f(tokencat(arg2name, f).v, tokencat(arg2name, d).v))
#define cdraw_testVec2DF_rr2V(testname_x,testname_y,expect0name,expect1name,funcname,result0name,result1name,arg0name,arg1name)																																										\
		cdraw_testVec2_rr(cdraw_testname(testname_x, _d), cdraw_testname(testname_y, _d), (scBigEpsD), tokencat(expect0name, d).v, tokencat(expect1name, d).v, tokencat(funcname, d), tokencat(result0name, d).v, tokencat(result1name, d).v, tokencat(arg0name, d).v, tokencat(arg1name, d).v);	\
		cdraw_testVec2_rr(cdraw_testname(testname_x, _f), cdraw_testname(testname_y, _f), (scBigEpsF), vecCopy4d4f(tokencat(expect0name, f).v, tokencat(expect0name, d).v), vecCopy4d4f(tokencat(expect1name, f).v, tokencat(expect1name, d).v), tokencat(funcname, f), tokencat(result0name, f).v, tokencat(result1name, f).v, vecCopy4d4f(tokencat(arg0name, f).v, tokencat(arg0name, d).v), vecCopy4d4f(tokencat(arg1name, f).v, tokencat(arg1name, d).v))
#define cdraw_testVec3DF_rr2V(testname_x,testname_y,testname_z,expect0name,expect1name,funcname,result0name,result1name,arg0name,arg1name)																																															\
		cdraw_testVec3_rr(cdraw_testname(testname_x, _d), cdraw_testname(testname_y, _d), cdraw_testname(testname_z, _d), (scBigEpsD), tokencat(expect0name, d).v, tokencat(expect1name, d).v, tokencat(funcname, d), tokencat(result0name, d).v, tokencat(result1name, d).v, tokencat(arg0name, d).v, tokencat(arg1name, d).v);	\
		cdraw_testVec3_rr(cdraw_testname(testname_x, _f), cdraw_testname(testname_y, _f), cdraw_testname(testname_z, _f), (scBigEpsF), vecCopy4d4f(tokencat(expect0name, f).v, tokencat(expect0name, d).v), vecCopy4d4f(tokencat(expect1name, f).v, tokencat(expect1name, d).v), tokencat(funcname, f), tokencat(result0name, f).v, tokencat(result1name, f).v, vecCopy4d4f(tokencat(arg0name, f).v, tokencat(arg0name, d).v), vecCopy4d4f(tokencat(arg1name, f).v, tokencat(arg1name, d).v))
#define cdraw_testVec4DF_rr2V(testname_x,testname_y,testname_z,testname_w,expect0name,expect1name,funcname,result0name,result1name,arg0name,arg1name)																																																				\
		cdraw_testVec4_rr(cdraw_testname(testname_x, _d), cdraw_testname(testname_y, _d), cdraw_testname(testname_z, _d), cdraw_testname(testname_w, _d), (scBigEpsD), tokencat(expect0name, d).v, tokencat(expect1name, d).v, tokencat(funcname, d), tokencat(result0name, d).v, tokencat(result1name, d).v, tokencat(arg0name, d).v, tokencat(arg1name, d).v);	\
		cdraw_testVec4_rr(cdraw_testname(testname_x, _f), cdraw_testname(testname_y, _f), cdraw_testname(testname_z, _f), cdraw_testname(testname_w, _f), (scBigEpsF), vecCopy4d4f(tokencat(expect0name, f).v, tokencat(expect0name, d).v), vecCopy4d4f(tokencat(expect1name, f).v, tokencat(expect1name, d).v), tokencat(funcname, f), tokencat(result0name, f).v, tokencat(result1name, f).v, vecCopy4d4f(tokencat(arg0name, f).v, tokencat(arg0name, d).v), vecCopy4d4f(tokencat(arg1name, f).v, tokencat(arg1name, d).v))
#define cdraw_testVec2DF_rr1V1S(testname_x,testname_y,expect0name,expect1name,funcname,result0name,result1name,arg0name,arg1)																																					\
		cdraw_testVec2_rr(cdraw_testname(testname_x, _d), cdraw_testname(testname_y, _d), (scBigEpsD), tokencat(expect0name, d).v, tokencat(expect1name, d).v, tokencat(funcname, d), tokencat(result0name, d).v, tokencat(result1name, d).v, tokencat(arg0name, d).v, arg1);	\
		cdraw_testVec2_rr(cdraw_testname(testname_x, _f), cdraw_testname(testname_y, _f), (scBigEpsF), vecCopy4d4f(tokencat(expect0name, f).v, tokencat(expect0name, d).v), vecCopy4d4f(tokencat(expect1name, f).v, tokencat(expect1name, d).v), tokencat(funcname, f), tokencat(result0name, f).v, tokencat(result1name, f).v, vecCopy4d4f(tokencat(arg0name, f).v, tokencat(arg0name, d).v), tokencat(arg1, f))
#define cdraw_testVec3DF_rr1V1S(testname_x,testname_y,testname_z,expect0name,expect1name,funcname,result0name,result1name,arg0name,arg1)																																										\
		cdraw_testVec3_rr(cdraw_testname(testname_x, _d), cdraw_testname(testname_y, _d), cdraw_testname(testname_z, _d), (scBigEpsD), tokencat(expect0name, d).v, tokencat(expect1name, d).v, tokencat(funcname, d), tokencat(result0name, d).v, tokencat(result1name, d).v, tokencat(arg0name, d).v, arg1);	\
		cdraw_testVec3_rr(cdraw_testname(testname_x, _f), cdraw_testname(testname_y, _f), cdraw_testname(testname_z, _f), (scBigEpsF), vecCopy4d4f(tokencat(expect0name, f).v, tokencat(expect0name, d).v), vecCopy4d4f(tokencat(expect1name, f).v, tokencat(expect1name, d).v), tokencat(funcname, f), tokencat(result0name, f).v, tokencat(result1name, f).v, vecCopy4d4f(tokencat(arg0name, f).v, tokencat(arg0name, d).v), tokencat(arg1, f))
#define cdraw_testVec4DF_rr1V1S(testname_x,testname_y,testname_z,testname_w,expect0name,expect1name,funcname,result0name,result1name,arg0name,arg1)																																																\
		cdraw_testVec4_rr(cdraw_testname(testname_x, _d), cdraw_testname(testname_y, _d), cdraw_testname(testname_z, _d), cdraw_testname(testname_w, _d), (scBigEpsD), tokencat(expect0name, d).v, tokencat(expect1name, d).v, tokencat(funcname, d), tokencat(result0name, d).v, tokencat(result1name, d).v, tokencat(arg0name, d).v, arg1);	\
		cdraw_testVec4_rr(cdraw_testname(testname_x, _f), cdraw_testname(testname_y, _f), cdraw_testname(testname_z, _f), cdraw_testname(testname_w, _f), (scBigEpsF), vecCopy4d4f(tokencat(expect0name, f).v, tokencat(expect0name, d).v), vecCopy4d4f(tokencat(expect1name, f).v, tokencat(expect1name, d).v), tokencat(funcname, f), tokencat(result0name, f).v, tokencat(result1name, f).v, vecCopy4d4f(tokencat(arg0name, f).v, tokencat(arg0name, d).v), tokencat(arg1, f))
#define cdraw_testVec2DF_rSV1V(testname,testname_x,testname_y,expect,expectname,funcname,resultname,arg0name)																																			\
		cdraw_testVec2_rSV(cdraw_testname(testname, _d), cdraw_testname(testname_x, _d), cdraw_testname(testname_y, _d), (scBigEpsD), expect, tokencat(expectname, d).v, tokencat(funcname, d), tokencat(resultname, d).v, tokencat(arg0name, d).v);	\
		cdraw_testVec2_rSV(cdraw_testname(testname, _f), cdraw_testname(testname_x, _f), cdraw_testname(testname_y, _f), (scBigEpsF), expect, vecCopy4d4f(tokencat(expectname, f).v, tokencat(expectname, d).v), tokencat(funcname, f), tokencat(resultname, f).v, vecCopy4d4f(tokencat(arg0name, f).v, tokencat(arg0name, d).v))
#define cdraw_testVec3DF_rSV1V(testname,testname_x,testname_y,testname_z,expect,expectname,funcname,resultname,arg0name)																																								\
		cdraw_testVec3_rSV(cdraw_testname(testname, _d), cdraw_testname(testname_x, _d), cdraw_testname(testname_y, _d), cdraw_testname(testname_z, _d), (scBigEpsD), expect, tokencat(expectname, d).v, tokencat(funcname, d), tokencat(resultname, d).v, tokencat(arg0name, d).v);	\
		cdraw_testVec3_rSV(cdraw_testname(testname, _f), cdraw_testname(testname_x, _f), cdraw_testname(testname_y, _f), cdraw_testname(testname_z, _f), (scBigEpsF), expect, vecCopy4d4f(tokencat(expectname, f).v, tokencat(expectname, d).v), tokencat(funcname, f), tokencat(resultname, f).v, vecCopy4d4f(tokencat(arg0name, f).v, tokencat(arg0name, d).v))
#define cdraw_testVec4DF_rSV1V(testname,testname_x,testname_y,testname_z,testname_w,expect,expectname,funcname,resultname,arg0name)																																														\
		cdraw_testVec4_rSV(cdraw_testname(testname, _d), cdraw_testname(testname_x, _d), cdraw_testname(testname_y, _d), cdraw_testname(testname_z, _d), cdraw_testname(testname_w, _d), (scBigEpsD), expect, tokencat(expectname, d).v, tokencat(funcname, d), tokencat(resultname, d).v, tokencat(arg0name, d).v);	\
		cdraw_testVec4_rSV(cdraw_testname(testname, _f), cdraw_testname(testname_x, _f), cdraw_testname(testname_y, _f), cdraw_testname(testname_z, _f), cdraw_testname(testname_w, _f), (scBigEpsF), expect, vecCopy4d4f(tokencat(expectname, f).v, tokencat(expectname, d).v), tokencat(funcname, f), tokencat(resultname, f).v, vecCopy4d4f(tokencat(arg0name, f).v, tokencat(arg0name, d).v))
#define cdraw_testVec2DF_rSV1V1S(testname,testname_x,testname_y,expect,expectname,funcname,resultname,arg0name,arg1name)																																			\
		cdraw_testVec2_rSV(cdraw_testname(testname, _d), cdraw_testname(testname_x, _d), cdraw_testname(testname_y, _d), (scBigEpsD), expect, tokencat(expectname, d).v, tokencat(funcname, d), tokencat(resultname, d).v, tokencat(arg0name, d).v, arg1name);		\
		cdraw_testVec2_rSV(cdraw_testname(testname, _f), cdraw_testname(testname_x, _f), cdraw_testname(testname_y, _f), (scBigEpsF), expect, vecCopy4d4f(tokencat(expectname, f).v, tokencat(expectname, d).v), tokencat(funcname, f), tokencat(resultname, f).v, vecCopy4d4f(tokencat(arg0name, f).v, tokencat(arg0name, d).v), tokencat(arg1name, f))
#define cdraw_testVec3DF_rSV1V1S(testname,testname_x,testname_y,testname_z,expect,expectname,funcname,resultname,arg0name,arg1name)																																								\
		cdraw_testVec3_rSV(cdraw_testname(testname, _d), cdraw_testname(testname_x, _d), cdraw_testname(testname_y, _d), cdraw_testname(testname_z, _d), (scBigEpsD), expect, tokencat(expectname, d).v, tokencat(funcname, d), tokencat(resultname, d).v, tokencat(arg0name, d).v, arg1name);	\
		cdraw_testVec3_rSV(cdraw_testname(testname, _f), cdraw_testname(testname_x, _f), cdraw_testname(testname_y, _f), cdraw_testname(testname_z, _f), (scBigEpsF), expect, vecCopy4d4f(tokencat(expectname, f).v, tokencat(expectname, d).v), tokencat(funcname, f), tokencat(resultname, f).v, vecCopy4d4f(tokencat(arg0name, f).v, tokencat(arg0name, d).v), tokencat(arg1name, f))
#define cdraw_testVec4DF_rSV1V1S(testname,testname_x,testname_y,testname_z,testname_w,expect,expectname,funcname,resultname,arg0name,arg1name)																																													\
		cdraw_testVec4_rSV(cdraw_testname(testname, _d), cdraw_testname(testname_x, _d), cdraw_testname(testname_y, _d), cdraw_testname(testname_z, _d), cdraw_testname(testname_w, _d), (scBigEpsD), expect, tokencat(expectname, d).v, tokencat(funcname, d), tokencat(resultname, d).v, tokencat(arg0name, d).v, arg1name);	\
		cdraw_testVec4_rSV(cdraw_testname(testname, _f), cdraw_testname(testname_x, _f), cdraw_testname(testname_y, _f), cdraw_testname(testname_z, _f), cdraw_testname(testname_w, _f), (scBigEpsF), expect, vecCopy4d4f(tokencat(expectname, f).v, tokencat(expectname, d).v), tokencat(funcname, f), tokencat(resultname, f).v, vecCopy4d4f(tokencat(arg0name, f).v, tokencat(arg0name, d).v), tokencat(arg1name, f))


/******************************************************************************
* Tests.
******************************************************************************/

result_t cdrawScalarTest()
{
	result_init();

	{
		fp64_t const x_B = 3.14159, x_C = -180.0;

		cdraw_testSc_rB(cdraw_istrue_d, scIsZeroApproxD, scEpsD);
		cdraw_testSc_rB(cdraw_istrue_f, scIsZeroApproxF, scEpsF);
		cdraw_testScDF_rB1S(isfalse, scIsZeroApprox, x_B);
		cdraw_testScDF_rB1S(isfalse, scIsZeroApprox, x_C);

		cdraw_testSc_rB(cdraw_isfalse_d, scIsNonZeroApproxD, scEpsD);
		cdraw_testSc_rB(cdraw_isfalse_f, scIsNonZeroApproxF, scEpsF);
		cdraw_testScDF_rB1S(istrue, scIsNonZeroApprox, x_B);
		cdraw_testScDF_rB1S(istrue, scIsNonZeroApprox, x_C);

		cdraw_testSc_rB(cdraw_isfalse_d, scIsPositiveApproxD, scEpsD);
		cdraw_testSc_rB(cdraw_isfalse_f, scIsPositiveApproxF, scEpsF);
		cdraw_testScDF_rB1S(istrue, scIsPositiveApprox, x_B);
		cdraw_testScDF_rB1S(isfalse, scIsPositiveApprox, x_C);

		cdraw_testSc_rB(cdraw_istrue_d, scIsNonPositiveApproxD, scEpsD);
		cdraw_testSc_rB(cdraw_istrue_f, scIsNonPositiveApproxF, scEpsF);
		cdraw_testScDF_rB1S(isfalse, scIsNonPositiveApprox, x_B);
		cdraw_testScDF_rB1S(istrue, scIsNonPositiveApprox, x_C);

		cdraw_testSc_rB(cdraw_isfalse_d, scIsNegativeApproxD, scEpsD);
		cdraw_testSc_rB(cdraw_isfalse_f, scIsNegativeApproxF, scEpsF);
		cdraw_testScDF_rB1S(isfalse, scIsNegativeApprox, x_B);
		cdraw_testScDF_rB1S(istrue, scIsNegativeApprox, x_C);

		cdraw_testSc_rB(cdraw_istrue_d, scIsNonNegativeApproxD, scEpsD);
		cdraw_testSc_rB(cdraw_istrue_f, scIsNonNegativeApproxF, scEpsF);
		cdraw_testScDF_rB1S(istrue, scIsNonNegativeApprox, x_B);
		cdraw_testScDF_rB1S(isfalse, scIsNonNegativeApprox, x_C);

		cdraw_testScDF_rB1S(istrue, scIsUnityApprox, sc1D);
		cdraw_testScDF_rB1S(isfalse, scIsUnityApprox, sc2D);

		cdraw_testScDF_rB1S(isfalse, scIsNonUnityApprox, sc1D);
		cdraw_testScDF_rB1S(istrue, scIsNonUnityApprox, sc2D);
	}
	
	{
		fp64_t const x_A = +2.5, x_B = -1.25, x_C = 0.0, x_num = 4.0;

		cdraw_testScDF_1S(isexact, +1.0, scSgn, x_A);
		cdraw_testScDF_1S(isexact, -1.0, scSgn, x_B);
		cdraw_testScDF_1S(isexact, 0.00, scSgn, x_C);

		cdraw_testScDF_1S(isexact, +x_A, scAbs, x_A);
		cdraw_testScDF_1S(isexact, -x_B, scAbs, x_B);
		cdraw_testScDF_1S(isexact, 0.00, scAbs, x_C);

		cdraw_testScDF_1S(isapprox, 0.50, scFrac, x_A);
		cdraw_testScDF_1S(isapprox, 0.75, scFrac, x_B);
		cdraw_testScDF_1S(isapprox, 0.00, scFrac, x_C);

		cdraw_testScDF_1S(isapprox, +0.4, scRecip, x_A);
		cdraw_testScDF_1S(isapprox, -0.8, scRecip, x_B);
		cdraw_testScDF_1S(isunsafe, 0.00, scRecip, x_C);

		cdraw_testScDF_2S(isapprox, +1.6, scDiv, x_num, x_A);
		cdraw_testScDF_2S(isapprox, -3.2, scDiv, x_num, x_B);
		cdraw_testScDF_2S(isunsafe, 0.00, scDiv, x_num, x_C);

		cdraw_testScDF_2S(isapprox, +1.5, scMod, x_num, x_A);
		cdraw_testScDF_2S(isapprox, -1.0, scMod, x_num, x_B);
		cdraw_testScDF_2S(isunsafe, x_num, scMod, x_num, x_C);
	}

	{
		fp64_t const x_A = +0.75, x_B = +1.25, x_C = -0.25, x_min = -2.0, x_max = +6.0, x_min_dst = +50.0, x_max_dst = +100.0;

		cdraw_testScDF_3S(isapprox, +2.5, scMad, x_A, x_min, x_max);
		cdraw_testScDF_3S(isapprox, +5.5, scMad, x_B, x_min, x_max);
		cdraw_testScDF_3S(isapprox, -3.5, scMad, x_C, x_min, x_max);

		cdraw_testScDF_3S(isapprox, +4.00, scLerp, x_A, x_min, x_max);
		cdraw_testScDF_3S(isapprox, +8.00, scLerp, x_B, x_min, x_max);
		cdraw_testScDF_3S(isapprox, x_min, scLerp, x_C, x_min, x_min);

		cdraw_testScDF_3S(isapprox, x_A, scLerpInv, 4.0, x_min, x_max);
		cdraw_testScDF_3S(isapprox, x_B, scLerpInv, 8.0, x_min, x_max);
		cdraw_testScDF_3S(isunsafe, 0.0, scLerpInv, x_C, x_min, x_min);

		cdraw_testScDF_3S(isexact, x_A, scClamp, x_A, x_min, x_max);
		cdraw_testScDF_3S(isexact, x_max, scClamp, +x_max_dst, x_min, x_max);
		cdraw_testScDF_3S(isexact, x_min, scClamp, -x_max_dst, x_min, x_max);

		cdraw_testScDF_3S(isapprox, x_A, scRepeat, x_A, x_min, x_max);
		cdraw_testScDF_3S(isapprox, 4.0, scRepeat, x_max_dst, x_min, x_max);
		cdraw_testScDF_3S(isunsafe, x_C, scRepeat, x_C, x_min, x_min);

		cdraw_testScDF_5S(isapprox, 87.5, scRemap, +4.0, x_min, x_max, x_min_dst, x_max_dst);
		cdraw_testScDF_5S(isapprox, x_min_dst, scRemap, +8.0, x_min, x_max, x_min_dst, x_min_dst);
		cdraw_testScDF_5S(isunsafe, x_min_dst, scRemap, x_C, x_min, x_min, x_min_dst, x_max_dst);

		cdraw_testScDF_rB3S(istrue, scIsInRange, x_A, x_min, x_max);
		cdraw_testScDF_rB3S(isfalse, scIsInRange, x_B, x_min_dst, x_max_dst);
		cdraw_testScDF_rB3S(istrue, scIsInRange, x_C, x_C, x_C);

		cdraw_testScDF_rB1S(istrue, scIsInUnitRange, x_A);
		cdraw_testScDF_rB1S(isfalse, scIsInUnitRange, x_B);
		cdraw_testScDF_rB1S(isfalse, scIsInUnitRange, x_C);

		cdraw_testScDF_rB1S(istrue, scIsInSymmRange, x_A);
		cdraw_testScDF_rB1S(isfalse, scIsInSymmRange, x_B);
		cdraw_testScDF_rB1S(istrue, scIsInSymmRange, x_C);
	}

	{
		fp64_t const x_A = +64.0, x_B = -64.0, x_C = 0.0;

		cdraw_testScDF_1S(isapprox, 8.0, scSqrt, x_A);
		cdraw_testScDF_1S(isunsafe, 0.0, scSqrt, x_B);
		cdraw_testScDF_1S(isexact, 0.00, scSqrt, x_C);

		cdraw_testScDF_1S(isapprox, 0.125, scSqrtInv, x_A);
		cdraw_testScDF_1S(isunsafe, 0.000, scSqrtInv, x_B);
		cdraw_testScDF_1S(isunsafe, 0.000, scSqrtInv, x_C);

		cdraw_testScDF_1S(isapprox, +4.0, scCbrt, x_A);
		cdraw_testScDF_1S(isapprox, -4.0, scCbrt, x_B);
		cdraw_testScDF_1S(isexact, 0.000, scCbrt, x_C);

		cdraw_testScDF_1S(isapprox, +0.25, scCbrtInv, x_A);
		cdraw_testScDF_1S(isapprox, -0.25, scCbrtInv, x_B);
		cdraw_testScDF_1S(isunsafe, 0.000, scCbrtInv, x_C);
	}

	{
		fp64_t const x_r = sc3Pi_2D, x_d = sc270D, x_rr = -scPi_4D, x_dd = -sc45D;

		cdraw_testScDF_1S(isapprox, x_r, scDeg2Rad, x_d);
		cdraw_testScDF_1S(isapprox, x_rr, scDeg2Rad, x_dd);
		
		cdraw_testScDF_1S(isapprox, x_d, scRad2Deg, x_r);
		cdraw_testScDF_1S(isapprox, x_dd, scRad2Deg, x_rr);
		
		cdraw_testScDF_1S(isapprox, -sc90D, scNormalizeAngle180, x_d);
		cdraw_testScDF_1S(isapprox, x_dd, scNormalizeAngle180, x_dd);
		
		cdraw_testScDF_1S(isapprox, x_d, scNormalizeAngle360, x_d);
		cdraw_testScDF_1S(isapprox, 315.0, scNormalizeAngle360, x_dd);
		
		cdraw_testScDF_1S(isapprox, -scPi_2D, scNormalizeAnglePi, x_r);
		cdraw_testScDF_1S(isapprox, x_rr, scNormalizeAnglePi, x_rr);
		
		cdraw_testScDF_1S(isapprox, x_r, scNormalizeAngle2Pi, x_r);
		cdraw_testScDF_1S(isapprox, (scPi_4D * 7.0), scNormalizeAngle2Pi, x_rr);

		cdraw_testScDF_1S(isapprox, -1.0, scSinr, x_r);
		cdraw_testScDF_1S(isapprox, -scSqrt1_2D, scSinr, x_rr);

		cdraw_testScDF_1S(isapprox, x_r - sc2PiD, scAsinr, -1.0);
		cdraw_testScDF_1S(isapprox, x_rr, scAsinr, -scSqrt1_2D);
		cdraw_testScDF_1S(isunsafe, 0.0, scAsinr, -2.0);

		cdraw_testScDF_1S(isapprox, 0.0, scCosr, x_r);
		cdraw_testScDF_1S(isapprox, +scSqrt1_2D, scCosr, x_rr);

		cdraw_testScDF_1S(isapprox, x_r - scPi_2D, scAcosr, -1.0);
		cdraw_testScDF_1S(isapprox, x_rr + scPiD, scAcosr, -scSqrt1_2D);
		cdraw_testScDF_1S(isunsafe, 1.0, scAcosr, -2.0);

		cdraw_testScDF_1S(isunsafe, 0.0, scTanr, x_r);
		cdraw_testScDF_1S(isapprox, -1.0, scTanr, x_rr);

		cdraw_testScDF_1S(isapprox, 0.0, scAtanr, 0.0);
		cdraw_testScDF_1S(isapprox, x_rr, scAtanr, -1.0);

		cdraw_testScDF_2S(isapprox, (x_r - sc2PiD), scAtan2r, -1.0, 0.0);
		cdraw_testScDF_2S(isapprox, x_rr, scAtan2r, -1.0, +1.0);
		cdraw_testScDF_2S(isunsafe, 0.0, scAtan2r, 0.0, 0.0);

		cdraw_testScDF_1S(isapprox, -1.0, scSind, x_d);
		cdraw_testScDF_1S(isapprox, -scSqrt1_2D, scSind, x_dd);

		cdraw_testScDF_1S(isapprox, x_d - sc360D, scAsind, -1.0);
		cdraw_testScDF_1S(isapprox, x_dd, scAsind, -scSqrt1_2D);
		cdraw_testScDF_1S(isunsafe, 0.0, scAsind, -2.0);

		cdraw_testScDF_1S(isapprox, 0.0, scCosd, x_d);
		cdraw_testScDF_1S(isapprox, +scSqrt1_2D, scCosd, x_dd);

		cdraw_testScDF_1S(isapprox, x_d - sc90D, scAcosd, -1.0);
		cdraw_testScDF_1S(isapprox, x_dd + sc180D, scAcosd, -scSqrt1_2D);
		cdraw_testScDF_1S(isunsafe, 1.0, scAcosd, -2.0);

		cdraw_testScDF_1S(isunsafe, 0.0, scTand, x_d);
		cdraw_testScDF_1S(isapprox, -1.0, scTand, x_dd);

		cdraw_testScDF_1S(isapprox, 0.0, scAtand, 0.0);
		cdraw_testScDF_1S(isapprox, x_dd, scAtand, -1.0);

		cdraw_testScDF_2S(isapprox, (x_d - sc360D), scAtan2d, -1.0, 0.0);
		cdraw_testScDF_2S(isapprox, x_dd, scAtan2d, -1.0, +1.0);
		cdraw_testScDF_2S(isunsafe, 0.0, scAtan2d, 0.0, 0.0);

		{
			fp64_t const x_num = 3.6, x_den = 2.4;
			fp64_t r_modD;
			fp32_t r_modF;
			cdraw_testSc(cdraw_isapprox_d, scBigEpsD, 1.5, scDivModD, &r_modD, x_num, x_den);
			cdraw_testSc_rB(cdraw_istrue_b, cdraw_isapprox, scBigEpsD, 1.2, r_modD);
			cdraw_testSc(cdraw_isunsafe_d, sc0D, sc0D, scDivModD, &r_modD, x_num, sc0D);
			cdraw_testSc_rB(cdraw_istrue_b, cdraw_isexact, sc0D, x_num, r_modD);
			cdraw_testSc(cdraw_isapprox_f, scBigEpsF, 1.5f, scDivModF, &r_modF, (fp32_t)x_num, (fp32_t)x_den);
			cdraw_testSc_rB(cdraw_istrue_b, cdraw_isapprox, scBigEpsF, 1.2f, r_modF);
			cdraw_testSc(cdraw_isunsafe_f, sc0F, sc0F, scDivModF, &r_modF, (fp32_t)x_num, sc0F);
			cdraw_testSc_rB(cdraw_istrue_b, cdraw_isexact, sc0F, (fp32_t)x_num, r_modF);
		}

		{
			fp32_t rF, sF, cF;
			fp64_t rD, sD, cD;
			rD = scSinCosrD(&sD, &cD, x_r);
			cdraw_testSc_rB(cdraw_istrue_b, cdraw_isapprox, scBigEpsD, rD, +x_r);
			cdraw_testSc_rB(cdraw_istrue_b, cdraw_isapprox, scBigEpsD, sD, -sc1D);
			cdraw_testSc_rB(cdraw_istrue_b, cdraw_isapprox, scBigEpsD, cD, -sc0D);
			rD = scSinCosrD(&sD, &cD, x_rr);
			cdraw_testSc_rB(cdraw_istrue_b, cdraw_isapprox, scBigEpsD, rD, +x_rr);
			cdraw_testSc_rB(cdraw_istrue_b, cdraw_isapprox, scBigEpsD, sD, -scSqrt1_2D);
			cdraw_testSc_rB(cdraw_istrue_b, cdraw_isapprox, scBigEpsD, cD, +scSqrt1_2D);
			rF = scSinCosrF(&sF, &cF, (fp32_t)x_r);
			cdraw_testSc_rB(cdraw_istrue_b, cdraw_isapprox, scBigEpsF, rF, +(fp32_t)x_r);
			cdraw_testSc_rB(cdraw_istrue_b, cdraw_isapprox, scBigEpsF, sF, -sc1F);
			cdraw_testSc_rB(cdraw_istrue_b, cdraw_isapprox, scBigEpsF, cF, -sc0F);
			rF = scSinCosrF(&sF, &cF, (fp32_t)x_rr);
			cdraw_testSc_rB(cdraw_istrue_b, cdraw_isapprox, scBigEpsF, rF, +(fp32_t)x_rr);
			cdraw_testSc_rB(cdraw_istrue_b, cdraw_isapprox, scBigEpsF, sF, -scSqrt1_2F);
			cdraw_testSc_rB(cdraw_istrue_b, cdraw_isapprox, scBigEpsF, cF, +scSqrt1_2F);
		}
	}

	{
		fp64_t rand64;
		fp32_t rand32;

		rand64 = scRandD();
		rand32 = scRandF();
		cdraw_testSc_rB(cdraw_istrue_d, scIsInRangeD, rand64, sc0D, (fp64_t)RAND_MAX);
		cdraw_testSc_rB(cdraw_istrue_f, scIsInRangeF, rand32, sc0F, (fp32_t)RAND_MAX);

		rand64 = scRandUnitD();
		rand32 = scRandUnitF();
		cdraw_testSc_rB(cdraw_istrue_d, scIsInUnitRangeD, rand64);
		cdraw_testSc_rB(cdraw_istrue_f, scIsInUnitRangeF, rand32);

		rand64 = scRandSymmD();
		rand32 = scRandSymmF();
		cdraw_testSc_rB(cdraw_istrue_d, scIsInSymmRangeD, rand64);
		cdraw_testSc_rB(cdraw_istrue_f, scIsInSymmRangeF, rand32);
	}

	result_return();
}


result_t cdrawVectorTest()
{
	result_init();
	
	vec4b_t resultb = vecID4b;

	{
		cdraw_testVec2(cdraw_isexact_b, cdraw_isexact_b, 0, vecID2b.v, vecZero2b, resultb.v);
		cdraw_testVec3(cdraw_isexact_b, cdraw_isexact_b, cdraw_isexact_b, 0, vecID3b.v, vecZero3b, resultb.v);
		cdraw_testVec4(cdraw_isexact_b, cdraw_isexact_b, cdraw_isexact_b, cdraw_isexact_b, 0, vecID4b.v, vecZero4b, resultb.v);

		cdraw_testVec2(cdraw_isexact_b, cdraw_isexact_b, 0, vecXY2b.v, vecInit2b, resultb.v, true, true);
		cdraw_testVec3(cdraw_isexact_b, cdraw_isexact_b, cdraw_isexact_b, 0, vecZ3b.v, vecInit3b, resultb.v, false, false, true);
		cdraw_testVec4(cdraw_isexact_b, cdraw_isexact_b, cdraw_isexact_b, cdraw_isexact_b, 0, vecXYW4b.v, vecInit4b, resultb.v, true, true, false, true);

		cdraw_testVec2(cdraw_isexact_b, cdraw_isexact_b, 0, vecID2b.v, vecInitS2b, resultb.v, false);
		cdraw_testVec3(cdraw_isexact_b, cdraw_isexact_b, cdraw_isexact_b, 0, vecXYZ3b.v, vecInitS3b, resultb.v, true);
		cdraw_testVec4(cdraw_isexact_b, cdraw_isexact_b, cdraw_isexact_b, cdraw_isexact_b, 0, vecID4b.v, vecInitS4b, resultb.v, false);

		cdraw_testVec2(cdraw_isexact_b, cdraw_isexact_b, 0, vecX2b.v, vecCopy2b, resultb.v, vecX2b.v);
		cdraw_testVec3(cdraw_isexact_b, cdraw_isexact_b, cdraw_isexact_b, 0, vecYZ3b.v, vecCopy3b, resultb.v, vecYZ3b.v);
		cdraw_testVec4(cdraw_isexact_b, cdraw_isexact_b, cdraw_isexact_b, cdraw_isexact_b, 0, vecXW4b.v, vecCopy4b, resultb.v, vecXW4b.v);
		cdraw_testVec3(cdraw_isexact_b, cdraw_isexact_b, cdraw_isexact_b, 0, vecXZ3b.v, vecCopy2z3b, resultb.v, vecX2b.v, true);
		cdraw_testVec4(cdraw_isexact_b, cdraw_isexact_b, cdraw_isexact_b, cdraw_isexact_b, 0, vecYZ4b.v, vecCopy2zw4b, resultb.v, vecY2b.v, true, false);
		cdraw_testVec4(cdraw_isexact_b, cdraw_isexact_b, cdraw_isexact_b, cdraw_isexact_b, 0, vecXW4b.v, vecCopy3w4b, resultb.v, vecX3b.v, true);

		cdraw_testVec2(cdraw_isexact_b, cdraw_isexact_b, 0, vecY2b.v, vecCopy2i2b, resultb.v, vecY2i.v);
		cdraw_testVec3(cdraw_isexact_b, cdraw_isexact_b, cdraw_isexact_b, 0, vecXZ3b.v, vecCopy3i3b, resultb.v, vecXZ3i.v);
		cdraw_testVec4(cdraw_isexact_b, cdraw_isexact_b, cdraw_isexact_b, cdraw_isexact_b, 0, vecYW4b.v, vecCopy4i4b, resultb.v, vecYW4i.v);

		cdraw_testVec2(cdraw_isexact_b, cdraw_isexact_b, 0, vecX2b.v, vecCopy2u2b, resultb.v, vecX2u.v);
		cdraw_testVec3(cdraw_isexact_b, cdraw_isexact_b, cdraw_isexact_b, 0, vecY3b.v, vecCopy3u3b, resultb.v, vecY3u.v);
		cdraw_testVec4(cdraw_isexact_b, cdraw_isexact_b, cdraw_isexact_b, cdraw_isexact_b, 0, vecZ4b.v, vecCopy4u4b, resultb.v, vecZ4u.v);

		cdraw_testVec2(cdraw_isexact_b, cdraw_isexact_b, 0, vecY2b.v, vecCopy2f2b, resultb.v, vecY2f.v);
		cdraw_testVec3(cdraw_isexact_b, cdraw_isexact_b, cdraw_isexact_b, 0, vecXZ3b.v, vecCopy3f3b, resultb.v, vecXZ3f.v);
		cdraw_testVec4(cdraw_isexact_b, cdraw_isexact_b, cdraw_isexact_b, cdraw_isexact_b, 0, vecYW4b.v, vecCopy4f4b, resultb.v, vecYW4f.v);

		cdraw_testVec2(cdraw_isexact_b, cdraw_isexact_b, 0, vecX2b.v, vecCopy2d2b, resultb.v, vecX2d.v);
		cdraw_testVec3(cdraw_isexact_b, cdraw_isexact_b, cdraw_isexact_b, 0, vecY3b.v, vecCopy3d3b, resultb.v, vecY3d.v);
		cdraw_testVec4(cdraw_isexact_b, cdraw_isexact_b, cdraw_isexact_b, cdraw_isexact_b, 0, vecZ4b.v, vecCopy4d4b, resultb.v, vecZ4d.v);

		{
			cdraw_testVec2B_1V(istrue, isfalse, vecNot2b, resultb.v, vecY2b.v);
			cdraw_testVec2B_2V(isfalse, istrue, vecAnd2b, resultb.v, vecY2b.v, vecXY2b.v);
			cdraw_testVec2B_2V(istrue, isfalse, vecNand2b, resultb.v, vecY2b.v, vecXY2b.v);
			cdraw_testVec2B_2V(istrue, istrue, vecOr2b, resultb.v, vecY2b.v, vecXY2b.v);
			cdraw_testVec2B_2V(isfalse, isfalse, vecNor2b, resultb.v, vecY2b.v, vecXY2b.v);
			cdraw_testVec2B_2V(istrue, isfalse, vecXor2b, resultb.v, vecY2b.v, vecXY2b.v);
			cdraw_testVec2B_2V(isfalse, istrue, vecXnor2b, resultb.v, vecY2b.v, vecXY2b.v);
			cdraw_testVec2B_2V(isfalse, istrue, vecEq2b, resultb.v, vecY2b.v, vecXY2b.v);
			cdraw_testVec2B_2V(istrue, isfalse, vecNeq2b, resultb.v, vecY2b.v, vecXY2b.v);

			cdraw_testVec3B_1V(istrue, isfalse, isfalse, vecNot3b, resultb.v, vecYZ3b.v);
			cdraw_testVec3B_2V(isfalse, istrue, isfalse, vecAnd3b, resultb.v, vecYZ3b.v, vecXY3b.v);
			cdraw_testVec3B_2V(istrue, isfalse, istrue, vecNand3b, resultb.v, vecYZ3b.v, vecXY3b.v);
			cdraw_testVec3B_2V(istrue, istrue, istrue, vecOr3b, resultb.v, vecYZ3b.v, vecXY3b.v);
			cdraw_testVec3B_2V(isfalse, isfalse, isfalse, vecNor3b, resultb.v, vecYZ3b.v, vecXY3b.v);
			cdraw_testVec3B_2V(istrue, isfalse, istrue, vecXor3b, resultb.v, vecYZ3b.v, vecXY3b.v);
			cdraw_testVec3B_2V(isfalse, istrue, isfalse, vecXnor3b, resultb.v, vecYZ3b.v, vecXY3b.v);
			cdraw_testVec3B_2V(isfalse, istrue, isfalse, vecEq3b, resultb.v, vecYZ3b.v, vecXY3b.v);
			cdraw_testVec3B_2V(istrue, isfalse, istrue, vecNeq3b, resultb.v, vecYZ3b.v, vecXY3b.v);

			cdraw_testVec4B_1V(istrue, isfalse, isfalse, istrue, vecNot4b, resultb.v, vecYZ4b.v);
			cdraw_testVec4B_2V(isfalse, istrue, isfalse, isfalse, vecAnd4b, resultb.v, vecYZ4b.v, vecXYW4b.v);
			cdraw_testVec4B_2V(istrue, isfalse, istrue, istrue, vecNand4b, resultb.v, vecYZ4b.v, vecXYW4b.v);
			cdraw_testVec4B_2V(istrue, istrue, istrue, istrue, vecOr4b, resultb.v, vecYZ4b.v, vecXYW4b.v);
			cdraw_testVec4B_2V(isfalse, isfalse, isfalse, isfalse, vecNor4b, resultb.v, vecYZ4b.v, vecXYW4b.v);
			cdraw_testVec4B_2V(istrue, isfalse, istrue, istrue, vecXor4b, resultb.v, vecYZ4b.v, vecXYW4b.v);
			cdraw_testVec4B_2V(isfalse, istrue, isfalse, isfalse, vecXnor4b, resultb.v, vecYZ4b.v, vecXYW4b.v);
			cdraw_testVec4B_2V(isfalse, istrue, isfalse, isfalse, vecEq4b, resultb.v, vecYZ4b.v, vecXYW4b.v);
			cdraw_testVec4B_2V(istrue, isfalse, istrue, istrue, vecNeq4b, resultb.v, vecYZ4b.v, vecXYW4b.v);
		}
	}

	{
		vec4i_t resulti = vecID4i;
		vec4u_t resultu = vecID4u;

		cdraw_testVec2(cdraw_isexact_i, cdraw_isexact_i, 0, vecID2i.v, vecZero2i, resulti.v);
		cdraw_testVec3(cdraw_isexact_i, cdraw_isexact_i, cdraw_isexact_i, 0, vecID3i.v, vecZero3i, resulti.v);
		cdraw_testVec4(cdraw_isexact_i, cdraw_isexact_i, cdraw_isexact_i, cdraw_isexact_i, 0, vecID4i.v, vecZero4i, resulti.v);

		cdraw_testVec2(cdraw_isexact_i, cdraw_isexact_i, 0, vecXY2i.v, vecInit2i, resulti.v, 1, 1);
		cdraw_testVec3(cdraw_isexact_i, cdraw_isexact_i, cdraw_isexact_i, 0, vecZ3i.v, vecInit3i, resulti.v, 0, 0, 1);
		cdraw_testVec4(cdraw_isexact_i, cdraw_isexact_i, cdraw_isexact_i, cdraw_isexact_i, 0, vecXYW4i.v, vecInit4i, resulti.v, 1, 1, 0, 1);

		cdraw_testVec2(cdraw_isexact_i, cdraw_isexact_i, 0, vecID2i.v, vecInitS2i, resulti.v, 0);
		cdraw_testVec3(cdraw_isexact_i, cdraw_isexact_i, cdraw_isexact_i, 0, vecXYZ3i.v, vecInitS3i, resulti.v, 1);
		cdraw_testVec4(cdraw_isexact_i, cdraw_isexact_i, cdraw_isexact_i, cdraw_isexact_i, 0, vecID4i.v, vecInitS4i, resulti.v, 0);

		cdraw_testVec2(cdraw_isexact_i, cdraw_isexact_i, 0, vecX2i.v, vecCopy2i, resulti.v, vecX2i.v);
		cdraw_testVec3(cdraw_isexact_i, cdraw_isexact_i, cdraw_isexact_i, 0, vecYZ3i.v, vecCopy3i, resulti.v, vecYZ3i.v);
		cdraw_testVec4(cdraw_isexact_i, cdraw_isexact_i, cdraw_isexact_i, cdraw_isexact_i, 0, vecXW4i.v, vecCopy4i, resulti.v, vecXW4i.v);
		cdraw_testVec3(cdraw_isexact_i, cdraw_isexact_i, cdraw_isexact_i, 0, vecXZ3i.v, vecCopy2z3i, resulti.v, vecX2i.v, 1);
		cdraw_testVec4(cdraw_isexact_i, cdraw_isexact_i, cdraw_isexact_i, cdraw_isexact_i, 0, vecYZ4i.v, vecCopy2zw4i, resulti.v, vecY2i.v, 1, 0);
		cdraw_testVec4(cdraw_isexact_i, cdraw_isexact_i, cdraw_isexact_i, cdraw_isexact_i, 0, vecXW4i.v, vecCopy3w4i, resulti.v, vecX3i.v, 1);

		cdraw_testVec2(cdraw_isexact_i, cdraw_isexact_i, 0, vecY2i.v, vecCopy2b2i, resulti.v, vecY2b.v);
		cdraw_testVec3(cdraw_isexact_i, cdraw_isexact_i, cdraw_isexact_i, 0, vecXZ3i.v, vecCopy3b3i, resulti.v, vecXZ3b.v);
		cdraw_testVec4(cdraw_isexact_i, cdraw_isexact_i, cdraw_isexact_i, cdraw_isexact_i, 0, vecYW4i.v, vecCopy4b4i, resulti.v, vecYW4b.v);

		cdraw_testVec2(cdraw_isexact_i, cdraw_isexact_i, 0, vecX2i.v, vecCopy2u2i, resulti.v, vecX2u.v);
		cdraw_testVec3(cdraw_isexact_i, cdraw_isexact_i, cdraw_isexact_i, 0, vecY3i.v, vecCopy3u3i, resulti.v, vecY3u.v);
		cdraw_testVec4(cdraw_isexact_i, cdraw_isexact_i, cdraw_isexact_i, cdraw_isexact_i, 0, vecZ4i.v, vecCopy4u4i, resulti.v, vecZ4u.v);

		cdraw_testVec2(cdraw_isexact_i, cdraw_isexact_i, 0, vecY2i.v, vecCopy2f2i, resulti.v, vecY2f.v);
		cdraw_testVec3(cdraw_isexact_i, cdraw_isexact_i, cdraw_isexact_i, 0, vecXZ3i.v, vecCopy3f3i, resulti.v, vecXZ3f.v);
		cdraw_testVec4(cdraw_isexact_i, cdraw_isexact_i, cdraw_isexact_i, cdraw_isexact_i, 0, vecYW4i.v, vecCopy4f4i, resulti.v, vecYW4f.v);

		cdraw_testVec2(cdraw_isexact_i, cdraw_isexact_i, 0, vecX2i.v, vecCopy2d2i, resulti.v, vecX2d.v);
		cdraw_testVec3(cdraw_isexact_i, cdraw_isexact_i, cdraw_isexact_i, 0, vecY3i.v, vecCopy3d3i, resulti.v, vecY3d.v);
		cdraw_testVec4(cdraw_isexact_i, cdraw_isexact_i, cdraw_isexact_i, cdraw_isexact_i, 0, vecZ4i.v, vecCopy4d4i, resulti.v, vecZ4d.v);

		cdraw_testVec2(cdraw_isexact_u, cdraw_isexact_u, 0, vecID2u.v, vecZero2u, resultu.v);
		cdraw_testVec3(cdraw_isexact_u, cdraw_isexact_u, cdraw_isexact_u, 0, vecID3u.v, vecZero3u, resultu.v);
		cdraw_testVec4(cdraw_isexact_u, cdraw_isexact_u, cdraw_isexact_u, cdraw_isexact_u, 0, vecID4u.v, vecZero4u, resultu.v);

		cdraw_testVec2(cdraw_isexact_u, cdraw_isexact_u, 0, vecXY2u.v, vecInit2u, resultu.v, 1u, 1u);
		cdraw_testVec3(cdraw_isexact_u, cdraw_isexact_u, cdraw_isexact_u, 0, vecZ3u.v, vecInit3u, resultu.v, 0u, 0u, 1u);
		cdraw_testVec4(cdraw_isexact_u, cdraw_isexact_u, cdraw_isexact_u, cdraw_isexact_u, 0, vecXYW4u.v, vecInit4u, resultu.v, 1u, 1u, 0u, 1u);

		cdraw_testVec2(cdraw_isexact_u, cdraw_isexact_u, 0, vecID2u.v, vecInitS2u, resultu.v, 0u);
		cdraw_testVec3(cdraw_isexact_u, cdraw_isexact_u, cdraw_isexact_u, 0, vecXYZ3u.v, vecInitS3u, resultu.v, 1u);
		cdraw_testVec4(cdraw_isexact_u, cdraw_isexact_u, cdraw_isexact_u, cdraw_isexact_u, 0, vecID4u.v, vecInitS4u, resultu.v, 0u);

		cdraw_testVec2(cdraw_isexact_u, cdraw_isexact_u, 0, vecX2u.v, vecCopy2u, resultu.v, vecX2u.v);
		cdraw_testVec3(cdraw_isexact_u, cdraw_isexact_u, cdraw_isexact_u, 0, vecYZ3u.v, vecCopy3u, resultu.v, vecYZ3u.v);
		cdraw_testVec4(cdraw_isexact_u, cdraw_isexact_u, cdraw_isexact_u, cdraw_isexact_u, 0, vecXW4u.v, vecCopy4u, resultu.v, vecXW4u.v);
		cdraw_testVec3(cdraw_isexact_u, cdraw_isexact_u, cdraw_isexact_u, 0, vecXZ3u.v, vecCopy2z3u, resultu.v, vecX2u.v, 1u);
		cdraw_testVec4(cdraw_isexact_u, cdraw_isexact_u, cdraw_isexact_u, cdraw_isexact_u, 0, vecYZ4u.v, vecCopy2zw4u, resultu.v, vecY2u.v, 1u, 0u);
		cdraw_testVec4(cdraw_isexact_u, cdraw_isexact_u, cdraw_isexact_u, cdraw_isexact_u, 0, vecXW4u.v, vecCopy3w4u, resultu.v, vecX3u.v, 1u);

		cdraw_testVec2(cdraw_isexact_u, cdraw_isexact_u, 0, vecY2u.v, vecCopy2i2u, resultu.v, vecY2i.v);
		cdraw_testVec3(cdraw_isexact_u, cdraw_isexact_u, cdraw_isexact_u, 0, vecXZ3u.v, vecCopy3i3u, resultu.v, vecXZ3i.v);
		cdraw_testVec4(cdraw_isexact_u, cdraw_isexact_u, cdraw_isexact_u, cdraw_isexact_u, 0, vecYW4u.v, vecCopy4i4u, resultu.v, vecYW4i.v);

		cdraw_testVec2(cdraw_isexact_u, cdraw_isexact_u, 0, vecX2u.v, vecCopy2b2u, resultu.v, vecX2b.v);
		cdraw_testVec3(cdraw_isexact_u, cdraw_isexact_u, cdraw_isexact_u, 0, vecY3u.v, vecCopy3b3u, resultu.v, vecY3b.v);
		cdraw_testVec4(cdraw_isexact_u, cdraw_isexact_u, cdraw_isexact_u, cdraw_isexact_u, 0, vecZ4u.v, vecCopy4b4u, resultu.v, vecZ4b.v);

		cdraw_testVec2(cdraw_isexact_u, cdraw_isexact_u, 0, vecY2u.v, vecCopy2f2u, resultu.v, vecY2f.v);
		cdraw_testVec3(cdraw_isexact_u, cdraw_isexact_u, cdraw_isexact_u, 0, vecXZ3u.v, vecCopy3f3u, resultu.v, vecXZ3f.v);
		cdraw_testVec4(cdraw_isexact_u, cdraw_isexact_u, cdraw_isexact_u, cdraw_isexact_u, 0, vecYW4u.v, vecCopy4f4u, resultu.v, vecYW4f.v);

		cdraw_testVec2(cdraw_isexact_u, cdraw_isexact_u, 0, vecX2u.v, vecCopy2d2u, resultu.v, vecX2d.v);
		cdraw_testVec3(cdraw_isexact_u, cdraw_isexact_u, cdraw_isexact_u, 0, vecY3u.v, vecCopy3d3u, resultu.v, vecY3d.v);
		cdraw_testVec4(cdraw_isexact_u, cdraw_isexact_u, cdraw_isexact_u, cdraw_isexact_u, 0, vecZ4u.v, vecCopy4d4u, resultu.v, vecZ4d.v);

		{
			vec4i_t expect0i, param0i, param1i, expect1i, resultAi;
			vec4u_t expect0u, param0u, param1u, expect1u, resultAu;

			vecInit4i(param0i.v, 131415, 161718, 1920, 1011);
			vecInit4i(param1i.v, 123, 456, 1920, 222324);

			vecInit4i(expect0i.v, ~131415, ~161718, ~1920, ~1011);
			cdraw_testVec2IU_1V(isexact, isexact, expect0, vecNot2, result, param0);
			cdraw_testVec3IU_1V(isexact, isexact, isexact, expect0, vecNot3, result, param0);
			cdraw_testVec4IU_1V(isexact, isexact, isexact, isexact, expect0, vecNot4, result, param0);

			vecInit4i(expect0i.v, (131415 & 123), (161718 & 456), (1920 & 1920), (1011 & 222324));
			cdraw_testVec2IU_2V(isexact, isexact, expect0, vecAnd2, result, param0, param1);
			cdraw_testVec3IU_2V(isexact, isexact, isexact, expect0, vecAnd3, result, param0, param1);
			cdraw_testVec4IU_2V(isexact, isexact, isexact, isexact, expect0, vecAnd4, result, param0, param1);

			vecInit4i(expect0i.v, ~(131415 & 123), ~(161718 & 456), ~(1920 & 1920), ~(1011 & 222324));
			cdraw_testVec2IU_2V(isexact, isexact, expect0, vecNand2, result, param0, param1);
			cdraw_testVec3IU_2V(isexact, isexact, isexact, expect0, vecNand3, result, param0, param1);
			cdraw_testVec4IU_2V(isexact, isexact, isexact, isexact, expect0, vecNand4, result, param0, param1);

			vecInit4i(expect0i.v, (131415 | 123), (161718 | 456), (1920 | 1920), (1011 | 222324));
			cdraw_testVec2IU_2V(isexact, isexact, expect0, vecOr2, result, param0, param1);
			cdraw_testVec3IU_2V(isexact, isexact, isexact, expect0, vecOr3, result, param0, param1);
			cdraw_testVec4IU_2V(isexact, isexact, isexact, isexact, expect0, vecOr4, result, param0, param1);

			vecInit4i(expect0i.v, ~(131415 | 123), ~(161718 | 456), ~(1920 | 1920), ~(1011 | 222324));
			cdraw_testVec2IU_2V(isexact, isexact, expect0, vecNor2, result, param0, param1);
			cdraw_testVec3IU_2V(isexact, isexact, isexact, expect0, vecNor3, result, param0, param1);
			cdraw_testVec4IU_2V(isexact, isexact, isexact, isexact, expect0, vecNor4, result, param0, param1);

			vecInit4i(expect0i.v, (131415 ^ 123), (161718 ^ 456), (1920 ^ 1920), (1011 ^ 222324));
			cdraw_testVec2IU_2V(isexact, isexact, expect0, vecXor2, result, param0, param1);
			cdraw_testVec3IU_2V(isexact, isexact, isexact, expect0, vecXor3, result, param0, param1);
			cdraw_testVec4IU_2V(isexact, isexact, isexact, isexact, expect0, vecXor4, result, param0, param1);

			vecInit4i(expect0i.v, ~(131415 ^ 123), ~(161718 ^ 456), ~(1920 ^ 1920), ~(1011 ^ 222324));
			cdraw_testVec2IU_2V(isexact, isexact, expect0, vecXnor2, result, param0, param1);
			cdraw_testVec3IU_2V(isexact, isexact, isexact, expect0, vecXnor3, result, param0, param1);
			cdraw_testVec4IU_2V(isexact, isexact, isexact, isexact, expect0, vecXnor4, result, param0, param1);

			cdraw_testVec2IU_rvB2V(isfalse, isfalse, vecEq2, resultb.v, param0, param1);
			cdraw_testVec3IU_rvB2V(isfalse, isfalse, istrue, vecEq3, resultb.v, param0, param1);
			cdraw_testVec4IU_rvB2V(isfalse, isfalse, istrue, isfalse, vecEq4, resultb.v, param0, param1);

			cdraw_testVec2IU_rvB2V(istrue, istrue, vecNeq2, resultb.v, param0, param1);
			cdraw_testVec3IU_rvB2V(istrue, istrue, isfalse, vecNeq3, resultb.v, param0, param1);
			cdraw_testVec4IU_rvB2V(istrue, istrue, isfalse, istrue, vecNeq4, resultb.v, param0, param1);

			cdraw_testVec2IU_rvB2V(istrue, istrue, vecGreater2, resultb.v, param0, param1);
			cdraw_testVec3IU_rvB2V(istrue, istrue, isfalse, vecGreater3, resultb.v, param0, param1);
			cdraw_testVec4IU_rvB2V(istrue, istrue, isfalse, isfalse, vecGreater4, resultb.v, param0, param1);

			cdraw_testVec2IU_rvB2V(istrue, istrue, vecGreaterEq2, resultb.v, param0, param1);
			cdraw_testVec3IU_rvB2V(istrue, istrue, istrue, vecGreaterEq3, resultb.v, param0, param1);
			cdraw_testVec4IU_rvB2V(istrue, istrue, istrue, isfalse, vecGreaterEq4, resultb.v, param0, param1);

			cdraw_testVec2IU_rvB2V(isfalse, isfalse, vecLess2, resultb.v, param0, param1);
			cdraw_testVec3IU_rvB2V(isfalse, isfalse, isfalse, vecLess3, resultb.v, param0, param1);
			cdraw_testVec4IU_rvB2V(isfalse, isfalse, isfalse, istrue, vecLess4, resultb.v, param0, param1);

			cdraw_testVec2IU_rvB2V(isfalse, isfalse, vecLessEq2, resultb.v, param0, param1);
			cdraw_testVec3IU_rvB2V(isfalse, isfalse, istrue, vecLessEq3, resultb.v, param0, param1);
			cdraw_testVec4IU_rvB2V(isfalse, isfalse, istrue, istrue, vecLessEq4, resultb.v, param0, param1);

			vecInit4i(expect0i.v, -131415, -161718, -1920, -1011);
			cdraw_testVec2IU_1V(isexact, isexact, expect0, vecNegate2, result, param0);
			cdraw_testVec3IU_1V(isexact, isexact, isexact, expect0, vecNegate3, result, param0);
			cdraw_testVec4IU_1V(isexact, isexact, isexact, isexact, expect0, vecNegate4, result, param0);

			vecInit4i(expect0i.v, (131415 + 123), (161718 + 456), (1920 + 1920), (1011 + 222324));
			cdraw_testVec2IU_2V(isexact, isexact, expect0, vecAdd2, result, param0, param1);
			cdraw_testVec3IU_2V(isexact, isexact, isexact, expect0, vecAdd3, result, param0, param1);
			cdraw_testVec4IU_2V(isexact, isexact, isexact, isexact, expect0, vecAdd4, result, param0, param1);

			vecInit4i(expect0i.v, (131415 - 123), (161718 - 456), (1920 - 1920), (1011 - 222324));
			cdraw_testVec2IU_2V(isexact, isexact, expect0, vecSub2, result, param0, param1);
			cdraw_testVec3IU_2V(isexact, isexact, isexact, expect0, vecSub3, result, param0, param1);
			cdraw_testVec4IU_2V(isexact, isexact, isexact, isexact, expect0, vecSub4, result, param0, param1);

			vecInit4i(expect0i.v, (131415 * 123), (161718 * 456), (1920 * 1920), (1011 * 222324));
			cdraw_testVec2IU_2V(isexact, isexact, expect0, vecMul2, result, param0, param1);
			cdraw_testVec3IU_2V(isexact, isexact, isexact, expect0, vecMul3, result, param0, param1);
			cdraw_testVec4IU_2V(isexact, isexact, isexact, isexact, expect0, vecMul4, result, param0, param1);

			vecInit4i(expect0i.v, (131415 / 123), (161718 / 456), (1920 / 1920), (1011 / 222324));
			cdraw_testVec2IU_2V(isexact, isexact, expect0, vecDiv2, result, param0, param1);
			cdraw_testVec3IU_2V(isexact, isexact, isexact, expect0, vecDiv3, result, param0, param1);
			cdraw_testVec4IU_2V(isexact, isexact, isexact, isexact, expect0, vecDiv4, result, param0, param1);

			vecInit4i(expect1i.v, (131415 % 123), (161718 % 456), (1920 % 1920), (1011 % 222324));
			cdraw_testVec2IU_2V(isexact, isexact, expect1, vecMod2, result, param0, param1);
			cdraw_testVec3IU_2V(isexact, isexact, isexact, expect1, vecMod3, result, param0, param1);
			cdraw_testVec4IU_2V(isexact, isexact, isexact, isexact, expect1, vecMod4, result, param0, param1);

			cdraw_testVec2IU_rr2V(isexact, isexact, expect0, expect1, vecDivMod2, result, resultA, param0, param1);
			cdraw_testVec3IU_rr2V(isexact, isexact, isexact, expect0, expect1, vecDivMod3, result, resultA, param0, param1);
			cdraw_testVec4IU_rr2V(isexact, isexact, isexact, isexact, expect0, expect1, vecDivMod4, result, resultA, param0, param1);

			expect0i = param1i = vecID4i;
			expect1i = param0i;
			cdraw_testVec2IU_2V(isunsafe, isunsafe, expect0, vecDiv2, result, param0, param1);
			cdraw_testVec3IU_2V(isunsafe, isunsafe, isunsafe, expect0, vecDiv3, result, param0, param1);
			cdraw_testVec4IU_2V(isunsafe, isunsafe, isunsafe, isunsafe, expect0, vecDiv4, result, param0, param1);
			cdraw_testVec2IU_2V(isunsafe, isunsafe, param0, vecMod2, result, param0, param1);
			cdraw_testVec3IU_2V(isunsafe, isunsafe, isunsafe, param0, vecMod3, result, param0, param1);
			cdraw_testVec4IU_2V(isunsafe, isunsafe, isunsafe, isunsafe, param0, vecMod4, result, param0, param1);
			cdraw_testVec2IU_rr2V(isunsafe, isunsafe, expect0, expect1, vecDivMod2, result, resultA, param0, param1);
			cdraw_testVec3IU_rr2V(isunsafe, isunsafe, isunsafe, expect0, expect1, vecDivMod3, result, resultA, param0, param1);
			cdraw_testVec4IU_rr2V(isunsafe, isunsafe, isunsafe, isunsafe, expect0, expect1, vecDivMod4, result, resultA, param0, param1);

			vecInit4i(expect0i.v, (131415 + 789), (161718 + 789), (1920 + 789), (1011 + 789));
			cdraw_testVec2IU_1V1S(isexact, isexact, expect0, vecAddS2, result, param0, 789);
			cdraw_testVec3IU_1V1S(isexact, isexact, isexact, expect0, vecAddS3, result, param0, 789);
			cdraw_testVec4IU_1V1S(isexact, isexact, isexact, isexact, expect0, vecAddS4, result, param0, 789);

			vecInit4i(expect0i.v, (131415 - 789), (161718 - 789), (1920 - 789), (1011 - 789));
			cdraw_testVec2IU_1V1S(isexact, isexact, expect0, vecSubS2, result, param0, 789);
			cdraw_testVec3IU_1V1S(isexact, isexact, isexact, expect0, vecSubS3, result, param0, 789);
			cdraw_testVec4IU_1V1S(isexact, isexact, isexact, isexact, expect0, vecSubS4, result, param0, 789);

			vecInit4i(expect0i.v, (131415 * 789), (161718 * 789), (1920 * 789), (1011 * 789));
			cdraw_testVec2IU_1V1S(isexact, isexact, expect0, vecMulS2, result, param0, 789);
			cdraw_testVec3IU_1V1S(isexact, isexact, isexact, expect0, vecMulS3, result, param0, 789);
			cdraw_testVec4IU_1V1S(isexact, isexact, isexact, isexact, expect0, vecMulS4, result, param0, 789);

			vecInit4i(expect0i.v, (131415 / 789), (161718 / 789), (1920 / 789), (1011 / 789));
			cdraw_testVec2IU_1V1S(isexact, isexact, expect0, vecDivS2, result, param0, 789);
			cdraw_testVec3IU_1V1S(isexact, isexact, isexact, expect0, vecDivS3, result, param0, 789);
			cdraw_testVec4IU_1V1S(isexact, isexact, isexact, isexact, expect0, vecDivS4, result, param0, 789);

			vecInit4i(expect1i.v, (131415 % 789), (161718 % 789), (1920 % 789), (1011 % 789));
			cdraw_testVec2IU_1V1S(isexact, isexact, expect1, vecModS2, result, param0, 789);
			cdraw_testVec3IU_1V1S(isexact, isexact, isexact, expect1, vecModS3, result, param0, 789);
			cdraw_testVec4IU_1V1S(isexact, isexact, isexact, isexact, expect1, vecModS4, result, param0, 789);

			cdraw_testVec2IU_rr1V1S(isexact, isexact, expect0, expect1, vecDivModS2, result, resultA, param0, 789);
			cdraw_testVec3IU_rr1V1S(isexact, isexact, isexact, expect0, expect1, vecDivModS3, result, resultA, param0, 789);
			cdraw_testVec4IU_rr1V1S(isexact, isexact, isexact, isexact, expect0, expect1, vecDivModS4, result, resultA, param0, 789);

			expect0i = vecID4i;
			expect1i = param0i;
			cdraw_testVec2IU_1V1S(isunsafe, isunsafe, expect0, vecDivS2, result, param0, 0);
			cdraw_testVec3IU_1V1S(isunsafe, isunsafe, isunsafe, expect0, vecDivS3, result, param0, 0);
			cdraw_testVec4IU_1V1S(isunsafe, isunsafe, isunsafe, isunsafe, expect0, vecDivS4, result, param0, 0);
			cdraw_testVec2IU_1V1S(isunsafe, isunsafe, expect1, vecModS2, result, param0, 0);
			cdraw_testVec3IU_1V1S(isunsafe, isunsafe, isunsafe, expect1, vecModS3, result, param0, 0);
			cdraw_testVec4IU_1V1S(isunsafe, isunsafe, isunsafe, isunsafe, expect1, vecModS4, result, param0, 0);
			cdraw_testVec2IU_rr1V1S(isunsafe, isunsafe, expect0, expect1, vecDivModS2, result, resultA, param0, 0);
			cdraw_testVec3IU_rr1V1S(isunsafe, isunsafe, isunsafe, expect0, expect1, vecDivModS3, result, resultA, param0, 0);
			cdraw_testVec4IU_rr1V1S(isunsafe, isunsafe, isunsafe, isunsafe, expect0, expect1, vecDivModS4, result, resultA, param0, 0);

			vecInit4i(param0i.v, 4, 3, 2, 1);
			vecInit4i(param1i.v, 5, 6, 7, 8);
			cdraw_testVecIU_rS2V(isexact, 38, vecDot2, param0, param1);	// 4*5 + 3*6 = 38
			cdraw_testVecIU_rS2V(isexact, 52, vecDot3, param0, param1);	//     + 2*7 = 52
			cdraw_testVecIU_rS2V(isexact, 60, vecDot4, param0, param1);	//     + 1*8 = 60

			vecInit4i(expect0i.v, 9, -18, 9, 0);	// ((3*7 - 2*6 = 9), (2*5 - 4*7 = -18), (4*6 - 3*5 = 9), 0)
			cdraw_testVecIU_rS2V(isexact, 9, vecCross2, param0, param1);	// z-only
			cdraw_testVec3IU_2V(isexact, isexact, isexact, expect0, vecCross3, result, param0, param1);
			cdraw_testVec4IU_2V(isexact, isexact, isexact, isexact, expect0, vecCross4, result, param0, param1);

			cdraw_testVecIU_rS1V(isexact, 25, vecLenSq2, param0);	// 4*4 + 3*3 = 25
			cdraw_testVecIU_rS1V(isexact, 29, vecLenSq3, param0);	//     + 2*2 = 29
			cdraw_testVecIU_rS1V(isexact, 30, vecLenSq4, param0);	//     + 1*1 = 30

			cdraw_testVecIU_rS2V(isexact, 10, vecDistSq2, param0, param1);	// sq(4-5) + sq(3-6) =  1 +  9 = 10
			cdraw_testVecIU_rS2V(isexact, 35, vecDistSq3, param0, param1);	//         + sq(2-7) = 10 + 25 = 35
			cdraw_testVecIU_rS2V(isexact, 84, vecDistSq4, param0, param1);	//         + sq(1-8) = 35 + 49 = 84

			vecInit4i(expect0i.v, -1, -3, -5, -7);
			cdraw_testVec2IU_rSV2V(isexact, isexact, isexact, 10, expect0, vecDispDistSq2, result, param0, param1);
			cdraw_testVec3IU_rSV2V(isexact, isexact, isexact, isexact, 35, expect0, vecDispDistSq3, result, param0, param1);
			cdraw_testVec4IU_rSV2V(isexact, isexact, isexact, isexact, isexact, 84, expect0, vecDispDistSq4, result, param0, param1);

			vecInit4i(expect0i.v, 24, 27, 30, 33); // ((4 + 4*5 = 24), (3 + 4*6 = 27), (2 + 4*7 = 30), (1 + 4*8 = 33))
			cdraw_testVec2IU_1S2V(isexact, isexact, expect0, vecMad2, result, 4, param0, param1);
			cdraw_testVec3IU_1S2V(isexact, isexact, isexact, expect0, vecMad3, result, 4, param0, param1);
			cdraw_testVec4IU_1S2V(isexact, isexact, isexact, isexact, expect0, vecMad4, result, 4, param0, param1);

			vecInit4i(expect0i.v, 7, 12, 17, 22); // ((4 + (5 - 4)*3 = 7), (3 + (6 - 3)*3 = 12), (2 + (7 - 2)*3 = 17), (1 + (8 - 1)*3 = 22))
			cdraw_testVec2IU_1S2V(isexact, isexact, expect0, vecLerp2, result, 3, param0, param1);
			cdraw_testVec3IU_1S2V(isexact, isexact, isexact, expect0, vecLerp3, result, 3, param0, param1);
			cdraw_testVec4IU_1S2V(isexact, isexact, isexact, isexact, expect0, vecLerp4, result, 3, param0, param1);

			param1i = vecID4i;
			cdraw_testVecIU_rB1V(isfalse, vecIsZero2, param0);
			cdraw_testVecIU_rB1V(istrue, vecIsZero2, param1);
			cdraw_testVecIU_rB1V(istrue, vecIsNonZero2, param0);
			cdraw_testVecIU_rB1V(isfalse, vecIsNonZero2, param1);
			cdraw_testVecIU_rB1V(isfalse, vecIsZero3, param0);
			cdraw_testVecIU_rB1V(istrue, vecIsZero3, param1);
			cdraw_testVecIU_rB1V(istrue, vecIsNonZero3, param0);
			cdraw_testVecIU_rB1V(isfalse, vecIsNonZero3, param1);
			cdraw_testVecIU_rB1V(isfalse, vecIsZero4, param0);
			cdraw_testVecIU_rB1V(istrue, vecIsZero4, param1);
			cdraw_testVecIU_rB1V(istrue, vecIsNonZero4, param0);
			cdraw_testVecIU_rB1V(isfalse, vecIsNonZero4, param1);
		}
	}

	{
		vec4d_t resultd = vecID4d;
		vec4f_t resultf = vecID4f;

		cdraw_testVec2(cdraw_isexact_d, cdraw_isexact_d, 0, vecID2d.v, vecZero2d, resultd.v);
		cdraw_testVec3(cdraw_isexact_d, cdraw_isexact_d, cdraw_isexact_d, 0, vecID3d.v, vecZero3d, resultd.v);
		cdraw_testVec4(cdraw_isexact_d, cdraw_isexact_d, cdraw_isexact_d, cdraw_isexact_d, 0, vecID4d.v, vecZero4d, resultd.v);

		cdraw_testVec2(cdraw_isexact_d, cdraw_isexact_d, 0, vecXY2d.v, vecInit2d, resultd.v, 1, 1);
		cdraw_testVec3(cdraw_isexact_d, cdraw_isexact_d, cdraw_isexact_d, 0, vecZ3d.v, vecInit3d, resultd.v, 0, 0, 1);
		cdraw_testVec4(cdraw_isexact_d, cdraw_isexact_d, cdraw_isexact_d, cdraw_isexact_d, 0, vecXYW4d.v, vecInit4d, resultd.v, 1, 1, 0, 1);

		cdraw_testVec2(cdraw_isexact_d, cdraw_isexact_d, 0, vecID2d.v, vecInitS2d, resultd.v, 0);
		cdraw_testVec3(cdraw_isexact_d, cdraw_isexact_d, cdraw_isexact_d, 0, vecXYZ3d.v, vecInitS3d, resultd.v, 1);
		cdraw_testVec4(cdraw_isexact_d, cdraw_isexact_d, cdraw_isexact_d, cdraw_isexact_d, 0, vecID4d.v, vecInitS4d, resultd.v, 0);

		cdraw_testVec2(cdraw_isexact_d, cdraw_isexact_d, 0, vecX2d.v, vecCopy2d, resultd.v, vecX2d.v);
		cdraw_testVec3(cdraw_isexact_d, cdraw_isexact_d, cdraw_isexact_d, 0, vecYZ3d.v, vecCopy3d, resultd.v, vecYZ3d.v);
		cdraw_testVec4(cdraw_isexact_d, cdraw_isexact_d, cdraw_isexact_d, cdraw_isexact_d, 0, vecXW4d.v, vecCopy4d, resultd.v, vecXW4d.v);
		cdraw_testVec3(cdraw_isexact_d, cdraw_isexact_d, cdraw_isexact_d, 0, vecXZ3d.v, vecCopy2z3d, resultd.v, vecX2d.v, 1);
		cdraw_testVec4(cdraw_isexact_d, cdraw_isexact_d, cdraw_isexact_d, cdraw_isexact_d, 0, vecYZ4d.v, vecCopy2zw4d, resultd.v, vecY2d.v, 1, 0);
		cdraw_testVec4(cdraw_isexact_d, cdraw_isexact_d, cdraw_isexact_d, cdraw_isexact_d, 0, vecXW4d.v, vecCopy3w4d, resultd.v, vecX3d.v, 1);

		cdraw_testVec2(cdraw_isexact_d, cdraw_isexact_d, 0, vecY2d.v, vecCopy2b2d, resultd.v, vecY2b.v);
		cdraw_testVec3(cdraw_isexact_d, cdraw_isexact_d, cdraw_isexact_d, 0, vecXZ3d.v, vecCopy3b3d, resultd.v, vecXZ3b.v);
		cdraw_testVec4(cdraw_isexact_d, cdraw_isexact_d, cdraw_isexact_d, cdraw_isexact_d, 0, vecYW4d.v, vecCopy4b4d, resultd.v, vecYW4b.v);

		cdraw_testVec2(cdraw_isexact_d, cdraw_isexact_d, 0, vecX2d.v, vecCopy2f2d, resultd.v, vecX2f.v);
		cdraw_testVec3(cdraw_isexact_d, cdraw_isexact_d, cdraw_isexact_d, 0, vecY3d.v, vecCopy3f3d, resultd.v, vecY3f.v);
		cdraw_testVec4(cdraw_isexact_d, cdraw_isexact_d, cdraw_isexact_d, cdraw_isexact_d, 0, vecZ4d.v, vecCopy4f4d, resultd.v, vecZ4f.v);

		cdraw_testVec2(cdraw_isexact_d, cdraw_isexact_d, 0, vecY2d.v, vecCopy2i2d, resultd.v, vecY2i.v);
		cdraw_testVec3(cdraw_isexact_d, cdraw_isexact_d, cdraw_isexact_d, 0, vecXZ3d.v, vecCopy3i3d, resultd.v, vecXZ3i.v);
		cdraw_testVec4(cdraw_isexact_d, cdraw_isexact_d, cdraw_isexact_d, cdraw_isexact_d, 0, vecYW4d.v, vecCopy4i4d, resultd.v, vecYW4i.v);

		cdraw_testVec2(cdraw_isexact_d, cdraw_isexact_d, 0, vecX2d.v, vecCopy2u2d, resultd.v, vecX2u.v);
		cdraw_testVec3(cdraw_isexact_d, cdraw_isexact_d, cdraw_isexact_d, 0, vecY3d.v, vecCopy3u3d, resultd.v, vecY3u.v);
		cdraw_testVec4(cdraw_isexact_d, cdraw_isexact_d, cdraw_isexact_d, cdraw_isexact_d, 0, vecZ4d.v, vecCopy4u4d, resultd.v, vecZ4u.v);

		cdraw_testVec2(cdraw_isexact_f, cdraw_isexact_f, 0, vecID2f.v, vecZero2f, resultf.v);
		cdraw_testVec3(cdraw_isexact_f, cdraw_isexact_f, cdraw_isexact_f, 0, vecID3f.v, vecZero3f, resultf.v);
		cdraw_testVec4(cdraw_isexact_f, cdraw_isexact_f, cdraw_isexact_f, cdraw_isexact_f, 0, vecID4f.v, vecZero4f, resultf.v);

		cdraw_testVec2(cdraw_isexact_f, cdraw_isexact_f, 0, vecXY2f.v, vecInit2f, resultf.v, 1u, 1u);
		cdraw_testVec3(cdraw_isexact_f, cdraw_isexact_f, cdraw_isexact_f, 0, vecZ3f.v, vecInit3f, resultf.v, 0u, 0u, 1u);
		cdraw_testVec4(cdraw_isexact_f, cdraw_isexact_f, cdraw_isexact_f, cdraw_isexact_f, 0, vecXYW4f.v, vecInit4f, resultf.v, 1u, 1u, 0u, 1u);

		cdraw_testVec2(cdraw_isexact_f, cdraw_isexact_f, 0, vecID2f.v, vecInitS2f, resultf.v, 0u);
		cdraw_testVec3(cdraw_isexact_f, cdraw_isexact_f, cdraw_isexact_f, 0, vecXYZ3f.v, vecInitS3f, resultf.v, 1u);
		cdraw_testVec4(cdraw_isexact_f, cdraw_isexact_f, cdraw_isexact_f, cdraw_isexact_f, 0, vecID4f.v, vecInitS4f, resultf.v, 0u);

		cdraw_testVec2(cdraw_isexact_f, cdraw_isexact_f, 0, vecX2f.v, vecCopy2f, resultf.v, vecX2f.v);
		cdraw_testVec3(cdraw_isexact_f, cdraw_isexact_f, cdraw_isexact_f, 0, vecYZ3f.v, vecCopy3f, resultf.v, vecYZ3f.v);
		cdraw_testVec4(cdraw_isexact_f, cdraw_isexact_f, cdraw_isexact_f, cdraw_isexact_f, 0, vecXW4f.v, vecCopy4f, resultf.v, vecXW4f.v);
		cdraw_testVec3(cdraw_isexact_f, cdraw_isexact_f, cdraw_isexact_f, 0, vecXZ3f.v, vecCopy2z3f, resultf.v, vecX2f.v, 1u);
		cdraw_testVec4(cdraw_isexact_f, cdraw_isexact_f, cdraw_isexact_f, cdraw_isexact_f, 0, vecYZ4f.v, vecCopy2zw4f, resultf.v, vecY2f.v, 1u, 0u);
		cdraw_testVec4(cdraw_isexact_f, cdraw_isexact_f, cdraw_isexact_f, cdraw_isexact_f, 0, vecXW4f.v, vecCopy3w4f, resultf.v, vecX3f.v, 1u);

		cdraw_testVec2(cdraw_isexact_f, cdraw_isexact_f, 0, vecY2f.v, vecCopy2d2f, resultf.v, vecY2d.v);
		cdraw_testVec3(cdraw_isexact_f, cdraw_isexact_f, cdraw_isexact_f, 0, vecXZ3f.v, vecCopy3d3f, resultf.v, vecXZ3d.v);
		cdraw_testVec4(cdraw_isexact_f, cdraw_isexact_f, cdraw_isexact_f, cdraw_isexact_f, 0, vecYW4f.v, vecCopy4d4f, resultf.v, vecYW4d.v);

		cdraw_testVec2(cdraw_isexact_f, cdraw_isexact_f, 0, vecX2f.v, vecCopy2b2f, resultf.v, vecX2b.v);
		cdraw_testVec3(cdraw_isexact_f, cdraw_isexact_f, cdraw_isexact_f, 0, vecY3f.v, vecCopy3b3f, resultf.v, vecY3b.v);
		cdraw_testVec4(cdraw_isexact_f, cdraw_isexact_f, cdraw_isexact_f, cdraw_isexact_f, 0, vecZ4f.v, vecCopy4b4f, resultf.v, vecZ4b.v);

		cdraw_testVec2(cdraw_isexact_f, cdraw_isexact_f, 0, vecY2f.v, vecCopy2i2f, resultf.v, vecY2i.v);
		cdraw_testVec3(cdraw_isexact_f, cdraw_isexact_f, cdraw_isexact_f, 0, vecXZ3f.v, vecCopy3i3f, resultf.v, vecXZ3i.v);
		cdraw_testVec4(cdraw_isexact_f, cdraw_isexact_f, cdraw_isexact_f, cdraw_isexact_f, 0, vecYW4f.v, vecCopy4i4f, resultf.v, vecYW4i.v);

		cdraw_testVec2(cdraw_isexact_f, cdraw_isexact_f, 0, vecX2f.v, vecCopy2u2f, resultf.v, vecX2u.v);
		cdraw_testVec3(cdraw_isexact_f, cdraw_isexact_f, cdraw_isexact_f, 0, vecY3f.v, vecCopy3u3f, resultf.v, vecY3u.v);
		cdraw_testVec4(cdraw_isexact_f, cdraw_isexact_f, cdraw_isexact_f, cdraw_isexact_f, 0, vecZ4f.v, vecCopy4u4f, resultf.v, vecZ4u.v);

		{
			vec4d_t expect0d, param0d, param1d, expect1d, resultAd;
			vec4f_t expect0f, param0f, param1f, expect1f, resultAf;

			vecInit4d(param0d.v, 131415, 161718, 1920, 1011);
			vecInit4d(param1d.v, 123, 456, 1920, 222324);

			cdraw_testVec2DF_rvB2V(isfalse, isfalse, vecEq2, resultb.v, param0, param1);
			cdraw_testVec3DF_rvB2V(isfalse, isfalse, istrue, vecEq3, resultb.v, param0, param1);
			cdraw_testVec4DF_rvB2V(isfalse, isfalse, istrue, isfalse, vecEq4, resultb.v, param0, param1);

			cdraw_testVec2DF_rvB2V(istrue, istrue, vecNeq2, resultb.v, param0, param1);
			cdraw_testVec3DF_rvB2V(istrue, istrue, isfalse, vecNeq3, resultb.v, param0, param1);
			cdraw_testVec4DF_rvB2V(istrue, istrue, isfalse, istrue, vecNeq4, resultb.v, param0, param1);

			cdraw_testVec2DF_rvB2V(istrue, istrue, vecGreater2, resultb.v, param0, param1);
			cdraw_testVec3DF_rvB2V(istrue, istrue, isfalse, vecGreater3, resultb.v, param0, param1);
			cdraw_testVec4DF_rvB2V(istrue, istrue, isfalse, isfalse, vecGreater4, resultb.v, param0, param1);

			cdraw_testVec2DF_rvB2V(istrue, istrue, vecGreaterEq2, resultb.v, param0, param1);
			cdraw_testVec3DF_rvB2V(istrue, istrue, istrue, vecGreaterEq3, resultb.v, param0, param1);
			cdraw_testVec4DF_rvB2V(istrue, istrue, istrue, isfalse, vecGreaterEq4, resultb.v, param0, param1);

			cdraw_testVec2DF_rvB2V(isfalse, isfalse, vecLess2, resultb.v, param0, param1);
			cdraw_testVec3DF_rvB2V(isfalse, isfalse, isfalse, vecLess3, resultb.v, param0, param1);
			cdraw_testVec4DF_rvB2V(isfalse, isfalse, isfalse, istrue, vecLess4, resultb.v, param0, param1);

			cdraw_testVec2DF_rvB2V(isfalse, isfalse, vecLessEq2, resultb.v, param0, param1);
			cdraw_testVec3DF_rvB2V(isfalse, isfalse, istrue, vecLessEq3, resultb.v, param0, param1);
			cdraw_testVec4DF_rvB2V(isfalse, isfalse, istrue, istrue, vecLessEq4, resultb.v, param0, param1);

			vecInit4d(expect0d.v, -131415, -161718, -1920, -1011);
			cdraw_testVec2DF_1V(isexact, isexact, expect0, vecNegate2, result, param0);
			cdraw_testVec3DF_1V(isexact, isexact, isexact, expect0, vecNegate3, result, param0);
			cdraw_testVec4DF_1V(isexact, isexact, isexact, isexact, expect0, vecNegate4, result, param0);

			vecInit4d(expect0d.v, (131415 + 123), (161718 + 456), (1920 + 1920), (1011 + 222324));
			cdraw_testVec2DF_2V(isexact, isexact, expect0, vecAdd2, result, param0, param1);
			cdraw_testVec3DF_2V(isexact, isexact, isexact, expect0, vecAdd3, result, param0, param1);
			cdraw_testVec4DF_2V(isexact, isexact, isexact, isexact, expect0, vecAdd4, result, param0, param1);

			vecInit4d(expect0d.v, (131415 - 123), (161718 - 456), (1920 - 1920), (1011 - 222324));
			cdraw_testVec2DF_2V(isexact, isexact, expect0, vecSub2, result, param0, param1);
			cdraw_testVec3DF_2V(isexact, isexact, isexact, expect0, vecSub3, result, param0, param1);
			cdraw_testVec4DF_2V(isexact, isexact, isexact, isexact, expect0, vecSub4, result, param0, param1);

			vecInit4d(expect0d.v, (131415 * 123), (161718 * 456), (1920 * 1920), (1011 * 222324));
			cdraw_testVec2DF_2V(isexact, isexact, expect0, vecMul2, result, param0, param1);
			cdraw_testVec3DF_2V(isexact, isexact, isexact, expect0, vecMul3, result, param0, param1);
			cdraw_testVec4DF_2V(isexact, isexact, isexact, isexact, expect0, vecMul4, result, param0, param1);

			vecInit4d(expect0d.v, (131415.0 / 123.0), (161718.0 / 456.0), (1920.0 / 1920.0), (1011.0 / 222324.0));
			cdraw_testVec2DF_2V(isapprox, isapprox, expect0, vecDiv2, result, param0, param1);
			cdraw_testVec3DF_2V(isapprox, isapprox, isapprox, expect0, vecDiv3, result, param0, param1);
			cdraw_testVec4DF_2V(isapprox, isapprox, isapprox, isapprox, expect0, vecDiv4, result, param0, param1);

			vecInit4d(expect1d.v, (131415 % 123), (161718 % 456), (1920 % 1920), (1011 % 222324));
			cdraw_testVec2DF_2V(isexact, isexact, expect1, vecMod2, result, param0, param1);
			cdraw_testVec3DF_2V(isexact, isexact, isexact, expect1, vecMod3, result, param0, param1);
			cdraw_testVec4DF_2V(isexact, isexact, isexact, isexact, expect1, vecMod4, result, param0, param1);

			cdraw_testVec2DF_rr2V(isapprox, isapprox, expect0, expect1, vecDivMod2, result, resultA, param0, param1);
			cdraw_testVec3DF_rr2V(isapprox, isapprox, isapprox, expect0, expect1, vecDivMod3, result, resultA, param0, param1);
			cdraw_testVec4DF_rr2V(isapprox, isapprox, isapprox, isapprox, expect0, expect1, vecDivMod4, result, resultA, param0, param1);

			expect0d = param1d = vecID4d;
			expect1d = param0d;
			cdraw_testVec2DF_2V(isunsafe, isunsafe, expect0, vecDiv2, result, param0, param1);
			cdraw_testVec3DF_2V(isunsafe, isunsafe, isunsafe, expect0, vecDiv3, result, param0, param1);
			cdraw_testVec4DF_2V(isunsafe, isunsafe, isunsafe, isunsafe, expect0, vecDiv4, result, param0, param1);
			cdraw_testVec2DF_2V(isunsafe, isunsafe, param0, vecMod2, result, param0, param1);
			cdraw_testVec3DF_2V(isunsafe, isunsafe, isunsafe, param0, vecMod3, result, param0, param1);
			cdraw_testVec4DF_2V(isunsafe, isunsafe, isunsafe, isunsafe, param0, vecMod4, result, param0, param1);
			cdraw_testVec2DF_rr2V(isunsafe, isunsafe, expect0, expect1, vecDivMod2, result, resultA, param0, param1);
			cdraw_testVec3DF_rr2V(isunsafe, isunsafe, isunsafe, expect0, expect1, vecDivMod3, result, resultA, param0, param1);
			cdraw_testVec4DF_rr2V(isunsafe, isunsafe, isunsafe, isunsafe, expect0, expect1, vecDivMod4, result, resultA, param0, param1);

			vecInit4d(expect0d.v, (131415 + 789), (161718 + 789), (1920 + 789), (1011 + 789));
			cdraw_testVec2DF_1V1S(isexact, isexact, expect0, vecAddS2, result, param0, 789.0);
			cdraw_testVec3DF_1V1S(isexact, isexact, isexact, expect0, vecAddS3, result, param0, 789.0);
			cdraw_testVec4DF_1V1S(isexact, isexact, isexact, isexact, expect0, vecAddS4, result, param0, 789.0);

			vecInit4d(expect0d.v, (131415 - 789), (161718 - 789), (1920 - 789), (1011 - 789));
			cdraw_testVec2DF_1V1S(isexact, isexact, expect0, vecSubS2, result, param0, 789.0);
			cdraw_testVec3DF_1V1S(isexact, isexact, isexact, expect0, vecSubS3, result, param0, 789.0);
			cdraw_testVec4DF_1V1S(isexact, isexact, isexact, isexact, expect0, vecSubS4, result, param0, 789.0);

			vecInit4d(expect0d.v, (131415 * 789), (161718 * 789), (1920 * 789), (1011 * 789));
			cdraw_testVec2DF_1V1S(isexact, isexact, expect0, vecMulS2, result, param0, 789.0);
			cdraw_testVec3DF_1V1S(isexact, isexact, isexact, expect0, vecMulS3, result, param0, 789.0);
			cdraw_testVec4DF_1V1S(isexact, isexact, isexact, isexact, expect0, vecMulS4, result, param0, 789.0);

			vecInit4d(expect0d.v, (131415.0 / 789.0), (161718.0 / 789.0), (1920.0 / 789.0), (1011.0 / 789.0));
			cdraw_testVec2DF_1V1S(isapprox, isapprox, expect0, vecDivS2, result, param0, 789.0);
			cdraw_testVec3DF_1V1S(isapprox, isapprox, isapprox, expect0, vecDivS3, result, param0, 789.0);
			cdraw_testVec4DF_1V1S(isapprox, isapprox, isapprox, isapprox, expect0, vecDivS4, result, param0, 789.0);

			vecInit4d(expect1d.v, (131415 % 789), (161718 % 789), (1920 % 789), (1011 % 789));
			cdraw_testVec2DF_1V1S(isexact, isexact, expect1, vecModS2, result, param0, 789.0);
			cdraw_testVec3DF_1V1S(isexact, isexact, isexact, expect1, vecModS3, result, param0, 789.0);
			cdraw_testVec4DF_1V1S(isexact, isexact, isexact, isexact, expect1, vecModS4, result, param0, 789.0);

			cdraw_testVec2DF_rr1V1S(isapprox, isapprox, expect0, expect1, vecDivModS2, result, resultA, param0, 789.0);
			cdraw_testVec3DF_rr1V1S(isapprox, isapprox, isapprox, expect0, expect1, vecDivModS3, result, resultA, param0, 789.0);
			cdraw_testVec4DF_rr1V1S(isapprox, isapprox, isapprox, isapprox, expect0, expect1, vecDivModS4, result, resultA, param0, 789.0);

			expect0d = vecID4d;
			expect1d = param0d;
			cdraw_testVec2DF_1V1S(isunsafe, isunsafe, expect0, vecDivS2, result, param0, 0.0);
			cdraw_testVec3DF_1V1S(isunsafe, isunsafe, isunsafe, expect0, vecDivS3, result, param0, 0.0);
			cdraw_testVec4DF_1V1S(isunsafe, isunsafe, isunsafe, isunsafe, expect0, vecDivS4, result, param0, 0.0);
			cdraw_testVec2DF_1V1S(isunsafe, isunsafe, expect1, vecModS2, result, param0, 0.0);
			cdraw_testVec3DF_1V1S(isunsafe, isunsafe, isunsafe, expect1, vecModS3, result, param0, 0.0);
			cdraw_testVec4DF_1V1S(isunsafe, isunsafe, isunsafe, isunsafe, expect1, vecModS4, result, param0, 0.0);
			cdraw_testVec2DF_rr1V1S(isunsafe, isunsafe, expect0, expect1, vecDivModS2, result, resultA, param0, 0.0);
			cdraw_testVec3DF_rr1V1S(isunsafe, isunsafe, isunsafe, expect0, expect1, vecDivModS3, result, resultA, param0, 0.0);
			cdraw_testVec4DF_rr1V1S(isunsafe, isunsafe, isunsafe, isunsafe, expect0, expect1, vecDivModS4, result, resultA, param0, 0.0);

			vecInit4d(param0d.v, 4, 3, 2, 1);
			vecInit4d(param1d.v, 5, 6, 7, 8);
			cdraw_testVecDF_rS2V(isexact, 38.0, vecDot2, param0, param1);	// 4*5 + 3*6 = 38
			cdraw_testVecDF_rS2V(isexact, 52.0, vecDot3, param0, param1);	//     + 2*7 = 52
			cdraw_testVecDF_rS2V(isexact, 60.0, vecDot4, param0, param1);	//     + 1*8 = 60

			vecInit4d(expect0d.v, 9, -18, 9, 0);	// ((3*7 - 2*6 = 9), (2*5 - 4*7 = -18), (4*6 - 3*5 = 9), 0)
			cdraw_testVecDF_rS2V(isexact, 9.0, vecCross2, param0, param1);	// z-only
			cdraw_testVec3DF_2V(isexact, isexact, isexact, expect0, vecCross3, result, param0, param1);
			cdraw_testVec4DF_2V(isexact, isexact, isexact, isexact, expect0, vecCross4, result, param0, param1);

			cdraw_testVecDF_rS1V(isexact, 25.0, vecLenSq2, param0);	// 4*4 + 3*3 = 25
			cdraw_testVecDF_rS1V(isexact, 29.0, vecLenSq3, param0);	//     + 2*2 = 29
			cdraw_testVecDF_rS1V(isexact, 30.0, vecLenSq4, param0);	//     + 1*1 = 30

			cdraw_testVecDF_rS2V(isexact, 10.0, vecDistSq2, param0, param1);	// sq(4-5) + sq(3-6) =  1 +  9 = 10
			cdraw_testVecDF_rS2V(isexact, 35.0, vecDistSq3, param0, param1);	//         + sq(2-7) = 10 + 25 = 35
			cdraw_testVecDF_rS2V(isexact, 84.0, vecDistSq4, param0, param1);	//         + sq(1-8) = 35 + 49 = 84

			vecInit4d(expect0d.v, -1, -3, -5, -7);
			cdraw_testVec2DF_rSV2V(isexact, isexact, isexact, 10.0, expect0, vecDispDistSq2, result, param0, param1);
			cdraw_testVec3DF_rSV2V(isexact, isexact, isexact, isexact, 35.0, expect0, vecDispDistSq3, result, param0, param1);
			cdraw_testVec4DF_rSV2V(isexact, isexact, isexact, isexact, isexact, 84.0, expect0, vecDispDistSq4, result, param0, param1);

			vecInit4d(expect0d.v, 24, 27, 30, 33); // ((4 + 4*5 = 24), (3 + 4*6 = 27), (2 + 4*7 = 30), (1 + 4*8 = 33))
			cdraw_testVec2DF_1S2V(isexact, isexact, expect0, vecMad2, result, 4.0, param0, param1);
			cdraw_testVec3DF_1S2V(isexact, isexact, isexact, expect0, vecMad3, result, 4.0, param0, param1);
			cdraw_testVec4DF_1S2V(isexact, isexact, isexact, isexact, expect0, vecMad4, result, 4.0, param0, param1);

			vecInit4d(expect0d.v, 7, 12, 17, 22); // ((4 + (5 - 4)*3 = 7), (3 + (6 - 3)*3 = 12), (2 + (7 - 2)*3 = 17), (1 + (8 - 1)*3 = 22))
			cdraw_testVec2DF_1S2V(isexact, isexact, expect0, vecLerp2, result, 3.0, param0, param1);
			cdraw_testVec3DF_1S2V(isexact, isexact, isexact, expect0, vecLerp3, result, 3.0, param0, param1);
			cdraw_testVec4DF_1S2V(isexact, isexact, isexact, isexact, expect0, vecLerp4, result, 3.0, param0, param1);

			param1d = vecID4d;
			cdraw_testVecDF_rB1V(isfalse, vecIsZero2, param0);
			cdraw_testVecDF_rB1V(istrue, vecIsZero2, param1);
			cdraw_testVecDF_rB1V(istrue, vecIsNonZero2, param0);
			cdraw_testVecDF_rB1V(isfalse, vecIsNonZero2, param1);
			cdraw_testVecDF_rB1V(isfalse, vecIsZero3, param0);
			cdraw_testVecDF_rB1V(istrue, vecIsZero3, param1);
			cdraw_testVecDF_rB1V(istrue, vecIsNonZero3, param0);
			cdraw_testVecDF_rB1V(isfalse, vecIsNonZero3, param1);
			cdraw_testVecDF_rB1V(isfalse, vecIsZero4, param0);
			cdraw_testVecDF_rB1V(istrue, vecIsZero4, param1);
			cdraw_testVecDF_rB1V(istrue, vecIsNonZero4, param0);
			cdraw_testVecDF_rB1V(isfalse, vecIsNonZero4, param1);

			vecInit4d(param0d.v, 27.0, 36.0, 60.0, 100.0);
			cdraw_testVecDF_rS1V(isapprox, 45.0, vecLen2, param0);
			cdraw_testVecDF_rS1V(isapprox, 75.0, vecLen3, param0);
			cdraw_testVecDF_rS1V(isapprox, 125.0, vecLen4, param0);

			cdraw_testVecDF_rS1V(isapprox, 0.02222222222222222222222222222222, vecLenInv2, param0);
			cdraw_testVecDF_rS1V(isapprox, 0.01333333333333333333333333333333, vecLenInv3, param0);
			cdraw_testVecDF_rS1V(isapprox, 0.008, vecLenInv4, param0);

			vecInit4d(param1d.v, 54.0, 72.0, 120.0, 200.0);
			cdraw_testVecDF_rS2V(isapprox, 45.0, vecDist2, param0, param1);
			cdraw_testVecDF_rS2V(isapprox, 75.0, vecDist3, param0, param1);
			cdraw_testVecDF_rS2V(isapprox, 125.0, vecDist4, param0, param1);

			cdraw_testVec2DF_rSV2V(isapprox, isapprox, isapprox, 45.0, param0, vecDispDist2, result, param1, param0);
			cdraw_testVec3DF_rSV2V(isapprox, isapprox, isapprox, isapprox, 75.0, param0, vecDispDist3, result, param1, param0);
			cdraw_testVec4DF_rSV2V(isapprox, isapprox, isapprox, isapprox, isapprox, 125.0, param0, vecDispDist4, result, param1, param0);

			vecInit4d(expect0d.v, (27.0 / 45.0), (36.0 / 45.0), (60.0 / 75.0), (100.0 / 125.0));
			cdraw_testVec2DF_rSV1V(isapprox, isapprox, isapprox, 45.0, expect0, vecNormalize2, result, param0);
			vecInit2d(expect0d.v, (27.0 / 75.0), (36.0 / 75.0));
			cdraw_testVec3DF_rSV1V(isapprox, isapprox, isapprox, isapprox, 75.0, expect0, vecNormalize3, result, param0);
			vecInit3d(expect0d.v, (27.0 / 125.0), (36.0 / 125.0), (60.0 / 125.0));
			cdraw_testVec4DF_rSV1V(isapprox, isapprox, isapprox, isapprox, isapprox, 125.0, expect0, vecNormalize4, result, param0);

			cdraw_testVec2DF_rSV1V1S(isapprox, isapprox, isapprox, 45.0, param1, vecResize2, result, param0, 90.0);
			cdraw_testVec3DF_rSV1V1S(isapprox, isapprox, isapprox, isapprox, 75.0, param1, vecResize3, result, param0, 150.0);
			cdraw_testVec4DF_rSV1V1S(isapprox, isapprox, isapprox, isapprox, isapprox, 125.0, param1, vecResize4, result, param0, 250.0);

			cdraw_testVecDF_rS2V(isapprox, 0.5, vecProjS2, param0, param1);
			cdraw_testVecDF_rS2V(isapprox, 0.5, vecProjS3, param0, param1);
			cdraw_testVecDF_rS2V(isapprox, 0.5, vecProjS4, param0, param1);

			cdraw_testVec2DF_rSV2V(isapprox, isapprox, isapprox, 0.5, param0, vecProj2, result, param0, param1);
			cdraw_testVec3DF_rSV2V(isapprox, isapprox, isapprox, isapprox, 0.5, param0, vecProj3, result, param0, param1);
			cdraw_testVec4DF_rSV2V(isapprox, isapprox, isapprox, isapprox, isapprox, 0.5, param0, vecProj4, result, param0, param1);

			vecInit4d(expect0d.v, 47.25, 63.00, 105.00, 175.00);
			cdraw_testVecDF_rS3V(isapprox, 0.75, vecLerpInv2, expect0, param0, param1);
			cdraw_testVecDF_rS3V(isapprox, 0.75, vecLerpInv3, expect0, param0, param1);
			cdraw_testVecDF_rS3V(isapprox, 0.75, vecLerpInv4, expect0, param0, param1);

			{

				vec4d_t param2d, expect2d, resultBd;
				vec4f_t param2f, expect2f, resultBf;
				vecd_t resultSc0d, resultSc1d;
				vecf_t resultSc0f, resultSc1f;

				vecCopy4d4f(param0f.v, vecInit4d(param0d.v, 2.0, 0.0, 0.0, 1.0));
				vecCopy4d4f(param1f.v, vecInit4d(param1d.v, 3.0, 3.0, 0.0, 1.0));
				vecCopy4d4f(param2f.v, vecInit4d(param2d.v, 6.0, 6.0, 6.0, 1.0));
				vecCopy4d4f(expect0f.v, vecInit4d(expect0d.v, 2.0, 0.0, 0.0, 0.0));
				vecCopy4d4f(expect1f.v, vecInit4d(expect1d.v, 0.0, 3.0, 0.0, 0.0));
				vecCopy4d4f(expect2f.v, vecInit4d(expect2d.v, 0.0, 0.0, 6.0, 0.0));
				cdraw_testSc(cdraw_isapprox_d, (scBigEpsD), 1.5, vecOrtho2d, resultd.v, param1d.v, param0d.v);
				cdraw_testSc(cdraw_isapprox_d, (scBigEpsD), 1.5, vecOrtho3d, resultd.v, param1d.v, param0d.v);
				cdraw_testSc(cdraw_isapprox_d, (scBigEpsD), 1.5, vecOrtho4d, resultd.v, param1d.v, param0d.v);
				cdraw_testSc(cdraw_isapprox_f, (scBigEpsF), 1.5f, vecOrtho2f, resultf.v, param1f.v, param0f.v);
				cdraw_testSc(cdraw_isapprox_f, (scBigEpsF), 1.5f, vecOrtho3f, resultf.v, param1f.v, param0f.v);
				cdraw_testSc(cdraw_isapprox_f, (scBigEpsF), 1.5f, vecOrtho4f, resultf.v, param1f.v, param0f.v);
				cdraw_testSc(cdraw_isapprox_d, (scBigEpsD), 2.0, vecOrthoBasis3d, resultd.v, resultAd.v, &resultSc1d, &resultSc0d, param2d.v, param1d.v, param0d.v);
				cdraw_testSc(cdraw_isapprox_d, (scBigEpsD), 2.0, vecOrthoBasis4d, resultd.v, resultAd.v, resultBd.v, &resultSc1d, &resultSc0d, param2d.v, param1d.v, param0d.v);
				cdraw_testSc(cdraw_isapprox_f, (scBigEpsF), 2.0f, vecOrthoBasis3f, resultf.v, resultAf.v, &resultSc1f, &resultSc0f, param2f.v, param1f.v, param0f.v);
				cdraw_testSc(cdraw_isapprox_f, (scBigEpsF), 2.0f, vecOrthoBasis4f, resultf.v, resultAf.v, resultBf.v, &resultSc1f, &resultSc0f, param2f.v, param1f.v, param0f.v);
				cdraw_testSc_rB(cdraw_istrue_b, cdraw_isapprox, scBigEpsD, 3.0, resultSc1d);
				cdraw_testSc_rB(cdraw_istrue_b, cdraw_isapprox, scBigEpsD, 1.5, resultSc0d);
				cdraw_testSc_rB(cdraw_istrue_b, cdraw_isapprox, scBigEpsF, 3.0f, resultSc1f);
				cdraw_testSc_rB(cdraw_istrue_b, cdraw_isapprox, scBigEpsF, 1.5f, resultSc0f);
				vecSub4d(resultd.v, resultd.v, expect2d.v);
				vecSub4d(resultAd.v, resultAd.v, expect1d.v);
				vecSub4d(resultBd.v, resultBd.v, expect0d.v);
				vecSub4f(resultf.v, resultf.v, expect2f.v);
				vecSub4f(resultAf.v, resultAf.v, expect1f.v);
				vecSub4f(resultBf.v, resultBf.v, expect0f.v);
				cdraw_testVecDF_rB1V(istrue, vecIsZero4, result);
				cdraw_testVecDF_rB1V(istrue, vecIsZero4, resultA);
				cdraw_testVecDF_rB1V(istrue, vecIsZero4, resultB);
			}

			{
				vecInit4d(param0d.v, scSqrt1_2D, scSqrt1_2D, scSqrt1_3D, 0.5);
				cdraw_testVecDF_rB1V(istrue, vecIsUnit2, param0);
				cdraw_testVecDF_rB1V(isfalse, vecIsNonUnit2, param0);
				vecInit2d(param0d.v, scSqrt1_3D, scSqrt1_3D);
				cdraw_testVecDF_rB1V(istrue, vecIsUnit3, param0);
				cdraw_testVecDF_rB1V(isfalse, vecIsNonUnit3, param0);
				vecInit3d(param0d.v, 0.5, 0.5, 0.5);
				cdraw_testVecDF_rB1V(istrue, vecIsUnit4, param0);
				cdraw_testVecDF_rB1V(isfalse, vecIsNonUnit4, param0);
			}
		}
	}

	result_return();
}


result_t cdrawMatrixTest()
{
	result_init();

	mat4d_t test4 = {
		1.0, -1.0, 1.0, -1.0,
		1.0,  1.0, 1.0,  1.0,
		3.0, -3.0, 2.0, -2.0,
		3.0,  3.0, 2.0,  2.0,
		//1.0, 2.0,-0.125,-0.250,
		//8.0, 4.0,-1.000,-0.500,
		//256.0, -128.0, -0.03125000, 0.01562500,
		//32.00, -64.00, -0.00390625, 0.00781250,
	}, test4inv, test4Id, test4L = {
		1.0, 2.0, 3.0, 4.0,
		5.0, 6.0, 7.0, 8.0,
		9.0, 10.0, 11.0, 12.0,
		13.0, 14.0, 15.0, 16.0,
	}, test4R = {
		90.0, 100.0, 110.0, 120.0,
		202.0, 228.0, 254.0, 280.0,
		314.0, 356.0, 398.0, 440.0,
		426.0, 484.0, 542.0, 600.0,
	};

	mat3d_t test3, test3inv, test3Id, test3L = {
		1.0, 2.0, 3.0,
		4.0, 5.0, 6.0,
		7.0, 8.0, 9.0,
	}, test3R = {
		30.0, 36.0, 42.0,
		66.0, 81.0, 96.0,
		102.0, 126.0, 150.0,
	};

	mat2d_t test2, test2inv, test2Id, test2L = {
		1.0, 2.0,
		3.0, 4.0,
	}, test2R = {
		7.0, 10.0,
		15.0, 22.0,
	};

	mat4f_t test4f, test4invf, test4Idf, test4Lf, test4Rf;
	mat3f_t test3f, test3invf, test3Idf, test3Lf, test3Rf;
	mat2f_t test2f, test2invf, test2Idf, test2Lf, test2Rf;

	matCopy43d(test3.m, test4.m);
	matCopy32d(test2.m, test3.m);
	matCopy4d4f(test4f.m, test4.m);
	matCopy4d4f(test4Lf.m, test4L.m);
	matCopy4d4f(test4Rf.m, test4R.m);
	matCopy3d3f(test3f.m, test3.m);
	matCopy3d3f(test3Lf.m, test3L.m);
	matCopy3d3f(test3Rf.m, test3R.m);
	matCopy2d2f(test2f.m, test2.m);
	matCopy2d2f(test2Lf.m, test2L.m);
	matCopy2d2f(test2Rf.m, test2R.m);

	matMul4d(test4Id.m, test4L.m, test4L.m);
	asserterr((vecDistSq4d(test4Id.m[0], test4R.m[0]) == sc0D)
		&& (vecDistSq4d(test4Id.m[1], test4R.m[1]) == sc0D)
		&& (vecDistSq4d(test4Id.m[2], test4R.m[2]) == sc0D)
		&& (vecDistSq4d(test4Id.m[3], test4R.m[3]) == sc0D), errcode_CUSTOM_BEGIN);
	matMul3d(test3Id.m, test3L.m, test3L.m);
	asserterr((vecDistSq3d(test3Id.m[0], test3R.m[0]) == sc0D)
		&& (vecDistSq3d(test3Id.m[1], test3R.m[1]) == sc0D)
		&& (vecDistSq3d(test3Id.m[2], test3R.m[2]) == sc0D), errcode_CUSTOM_BEGIN);
	matMul2d(test2Id.m, test2L.m, test2L.m);
	asserterr((vecDistSq2d(test2Id.m[0], test2R.m[0]) == sc0D)
		&& (vecDistSq2d(test2Id.m[1], test2R.m[1]) == sc0D), errcode_CUSTOM_BEGIN);

	matMul4f(test4Idf.m, test4Lf.m, test4Lf.m);
	asserterr((vecDistSq4f(test4Idf.m[0], test4Rf.m[0]) == sc0F)
		&& (vecDistSq4f(test4Idf.m[1], test4Rf.m[1]) == sc0F)
		&& (vecDistSq4f(test4Idf.m[2], test4Rf.m[2]) == sc0F)
		&& (vecDistSq4f(test4Idf.m[3], test4Rf.m[3]) == sc0F), errcode_CUSTOM_BEGIN);
	matMul3f(test3Idf.m, test3Lf.m, test3Lf.m);
	asserterr((vecDistSq3f(test3Idf.m[0], test3Rf.m[0]) == sc0F)
		&& (vecDistSq3f(test3Idf.m[1], test3Rf.m[1]) == sc0F)
		&& (vecDistSq3f(test3Idf.m[2], test3Rf.m[2]) == sc0F), errcode_CUSTOM_BEGIN);
	matMul2f(test2Idf.m, test2Lf.m, test2Lf.m);
	asserterr((vecDistSq2f(test2Idf.m[0], test2Rf.m[0]) == sc0F)
		&& (vecDistSq2f(test2Idf.m[1], test2Rf.m[1]) == sc0F), errcode_CUSTOM_BEGIN);

	matInverse4d(test4inv.m, matID4d.m);
	asserterr(matIsIdentity4d(test4inv.m), errcode_CUSTOM_BEGIN);
	matInverse3d(test3inv.m, matID3d.m);
	asserterr(matIsIdentity3d(test3inv.m), errcode_CUSTOM_BEGIN);
	matInverse2d(test2inv.m, matID2d.m);
	asserterr(matIsIdentity2d(test2inv.m), errcode_CUSTOM_BEGIN);

	matInverse4f(test4invf.m, matID4f.m);
	asserterr(matIsIdentity4f(test4invf.m), errcode_CUSTOM_BEGIN);
	matInverse3f(test3invf.m, matID3f.m);
	asserterr(matIsIdentity3f(test3invf.m), errcode_CUSTOM_BEGIN);
	matInverse2f(test2invf.m, matID2f.m);
	asserterr(matIsIdentity2f(test2invf.m), errcode_CUSTOM_BEGIN);

	matInverse4d(test4inv.m, test4.m);
	matInverse3d(test3inv.m, test3.m);
	matInverse2d(test2inv.m, test2.m);
	matMul4d(test4Id.m, test4.m, test4inv.m);
	asserterr(matIsIdentity4d(test4Id.m), errcode_CUSTOM_BEGIN);
	matMul3d(test3Id.m, test3.m, test3inv.m);
	asserterr(matIsIdentity3d(test3Id.m), errcode_CUSTOM_BEGIN);
	matMul2d(test2Id.m, test2.m, test2inv.m);
	asserterr(matIsIdentity2d(test2Id.m), errcode_CUSTOM_BEGIN);
	matMul4d(test4Id.m, test4inv.m, test4.m);
	asserterr(matIsIdentity4d(test4Id.m), errcode_CUSTOM_BEGIN);
	matMul3d(test3Id.m, test3inv.m, test3.m);
	asserterr(matIsIdentity3d(test3Id.m), errcode_CUSTOM_BEGIN);
	matMul2d(test2Id.m, test2inv.m, test2.m);
	asserterr(matIsIdentity2d(test2Id.m), errcode_CUSTOM_BEGIN);

	matInverse4f(test4invf.m, test4f.m);
	matInverse3f(test3invf.m, test3f.m);
	matInverse2f(test2invf.m, test2f.m);
	matMul4f(test4Idf.m, test4f.m, test4invf.m);
	asserterr(matIsIdentity4f(test4Idf.m), errcode_CUSTOM_BEGIN);
	matMul3f(test3Idf.m, test3f.m, test3invf.m);
	asserterr(matIsIdentity3f(test3Idf.m), errcode_CUSTOM_BEGIN);
	matMul2f(test2Idf.m, test2f.m, test2invf.m);
	asserterr(matIsIdentity2f(test2Idf.m), errcode_CUSTOM_BEGIN);
	matMul4f(test4Idf.m, test4invf.m, test4f.m);
	asserterr(matIsIdentity4f(test4Idf.m), errcode_CUSTOM_BEGIN);
	matMul3f(test3Idf.m, test3invf.m, test3f.m);
	asserterr(matIsIdentity3f(test3Idf.m), errcode_CUSTOM_BEGIN);
	matMul2f(test2Idf.m, test2invf.m, test2f.m);
	asserterr(matIsIdentity2f(test2Idf.m), errcode_CUSTOM_BEGIN);

	result_return();
}


result_t cdrawTransformTest()
{
	result_init();

	{
		vecd_t const angle_desired_d = 225.0;
		vecf_t const angle_desired_f = (vecf_t)angle_desired_d;
		vecd_t angle_actual_d;
		vecf_t angle_actual_f;
		Rmat2d_t Rd;
		Rmat2f_t Rf;

		RmatFromAngle2d(Rd, &angle_actual_d, angle_desired_d);
		RmatFromAngle2f(Rf, &angle_actual_f, angle_desired_f);

		angle_actual_d = RmatToAngle2d(Rd);
		angle_actual_f = RmatToAngle2f(Rf);
	}

	{
		double3_t const angle0 = { 225.0, -135.0, 120.0 };
		double3_t const angle1 = { 225.0, +90.0, 120.0 };
		double3_t const angle2 = { 225.0, +270.0, 120.0 };

		double3_t angle0d, angle1d, angle2d;
		float3_t angle0f, angle1f, angle2f;

		//double3_t angle_actual_d, angle_actual_1d;
		float3_t angle_actual_f, angle_actual_1f;

		//Rmat3d_t Rd, Rd1;
		Rmat3f_t Rf, Rf1;

		//vecd_t distd;
		vecf_t distf;

		RotateAxisOrder_t encodeOrder;
		uint i;
		for (i = 0; i < 3; ++i)
		{
			vecCopy3d3f(angle0f, vecInit3d(angle0d, angle0[(i + 0) % 3], angle0[(i + 1) % 3], angle0[(i + 2) % 3]));
			vecCopy3d3f(angle1f, vecInit3d(angle1d, angle1[(i + 0) % 3], angle1[(i + 1) % 3], angle1[(i + 2) % 3]));
			vecCopy3d3f(angle2f, vecInit3d(angle2d, angle2[(i + 0) % 3], angle2[(i + 1) % 3], angle2[(i + 2) % 3]));

			for (encodeOrder = rot_XYZ; encodeOrder <= rot_ZYX; ++encodeOrder)
			{
				RmatFromAngles3f(Rf, angle_actual_f, angle0f, encodeOrder);
				RmatToAngles3f(angle_actual_1f, Rf, encodeOrder);
				RmatFromAngles3f(Rf1, 0, angle_actual_1f, encodeOrder);
				asserterr(scBigEpsF >= (distf = vecDistSq3f(angle_actual_1f, angle_actual_f)), errcode_CUSTOM_BEGIN);
				asserterr(scBigEpsF >= (distf = vecDistSq3f(vx(Rf), vx(Rf1))), errcode_CUSTOM_BEGIN);
				asserterr(scBigEpsF >= (distf = vecDistSq3f(vy(Rf), vy(Rf1))), errcode_CUSTOM_BEGIN);
				asserterr(scBigEpsF >= (distf = vecDistSq3f(vz(Rf), vz(Rf1))), errcode_CUSTOM_BEGIN);

				RmatFromAngles3f(Rf, angle_actual_f, angle1f, encodeOrder);
				RmatToAngles3f(angle_actual_1f, Rf, encodeOrder);
				RmatFromAngles3f(Rf1, 0, angle_actual_1f, encodeOrder);
				asserterr(scBigEpsF >= (distf = vecDistSq3f(angle_actual_1f, angle_actual_f)), errcode_CUSTOM_BEGIN);
				asserterr(scBigEpsF >= (distf = vecDistSq3f(vx(Rf), vx(Rf1))), errcode_CUSTOM_BEGIN);
				asserterr(scBigEpsF >= (distf = vecDistSq3f(vz(Rf), vz(Rf1))), errcode_CUSTOM_BEGIN);
				asserterr(scBigEpsF >= (distf = vecDistSq3f(vy(Rf), vy(Rf1))), errcode_CUSTOM_BEGIN);

				RmatFromAngles3f(Rf, angle_actual_f, angle2f, encodeOrder);
				RmatToAngles3f(angle_actual_1f, Rf, encodeOrder);
				RmatFromAngles3f(Rf1, 0, angle_actual_1f, encodeOrder);
				asserterr(scBigEpsF >= (distf = vecDistSq3f(angle_actual_1f, angle_actual_f)), errcode_CUSTOM_BEGIN);
				asserterr(scBigEpsF >= (distf = vecDistSq3f(vx(Rf), vx(Rf1))), errcode_CUSTOM_BEGIN);
				asserterr(scBigEpsF >= (distf = vecDistSq3f(vy(Rf), vy(Rf1))), errcode_CUSTOM_BEGIN);
				asserterr(scBigEpsF >= (distf = vecDistSq3f(vz(Rf), vz(Rf1))), errcode_CUSTOM_BEGIN);
			}
		}
	}

	result_return();
}


result_t cdrawMathTest()
{
	result_init();
	cdrawScalarTest();
	cdrawVectorTest();
	cdrawMatrixTest();
	cdrawTransformTest();
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