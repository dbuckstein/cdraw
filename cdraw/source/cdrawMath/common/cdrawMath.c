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
#define cdraw_testScDF_rB1S(testname,funcD,funcF,arg0_fp64)								\
		cdraw_testSc(cdraw_testname(testname,_d), 0, 0, funcD, (fp64_t)(arg0_fp64));	\
		cdraw_testSc(cdraw_testname(testname,_f), 0, 0, funcF, (fp32_t)(arg0_fp64))
#define cdraw_testScDF_rB3S(testname,funcD,funcF,arg0_fp64,arg1_fp64,arg2_fp64)													\
		cdraw_testSc(cdraw_testname(testname,_d), 0, 0, funcD, (fp64_t)(arg0_fp64), (fp64_t)(arg1_fp64), (fp64_t)(arg2_fp64));	\
		cdraw_testSc(cdraw_testname(testname,_f), 0, 0, funcF, (fp32_t)(arg0_fp64), (fp32_t)(arg1_fp64), (fp32_t)(arg2_fp64))
#define cdraw_testScDF_1S(testname,expect_fp64,funcD,funcF,arg0_fp64)												\
		cdraw_testSc(cdraw_testname(testname,_d), (scBigEpsD), (fp64_t)(expect_fp64), funcD, (fp64_t)(arg0_fp64));	\
		cdraw_testSc(cdraw_testname(testname,_f), (scBigEpsF), (fp32_t)(expect_fp64), funcF, (fp32_t)(arg0_fp64))
#define cdraw_testScDF_2S(testname,expect_fp64,funcD,funcF,arg0_fp64,arg1_fp64)															\
		cdraw_testSc(cdraw_testname(testname,_d), (scBigEpsD), (fp64_t)(expect_fp64), funcD, (fp64_t)(arg0_fp64), (fp64_t)(arg1_fp64));	\
		cdraw_testSc(cdraw_testname(testname,_f), (scBigEpsF), (fp32_t)(expect_fp64), funcF, (fp32_t)(arg0_fp64), (fp32_t)(arg1_fp64))
#define cdraw_testScDF_3S(testname,expect_fp64,funcD,funcF,arg0_fp64,arg1_fp64,arg2_fp64)																		\
		cdraw_testSc(cdraw_testname(testname,_d), (scBigEpsD), (fp64_t)(expect_fp64), funcD, (fp64_t)(arg0_fp64), (fp64_t)(arg1_fp64), (fp64_t)(arg2_fp64));	\
		cdraw_testSc(cdraw_testname(testname,_f), (scBigEpsF), (fp32_t)(expect_fp64), funcF, (fp32_t)(arg0_fp64), (fp32_t)(arg1_fp64), (fp32_t)(arg2_fp64))
#define cdraw_testScDF_5S(testname,expect_fp64,funcD,funcF,arg0_fp64,arg1_fp64,arg2_fp64,arg3_fp64,arg4_fp64)																							\
		cdraw_testSc(cdraw_testname(testname,_d), (scBigEpsD), (fp64_t)(expect_fp64), funcD, (fp64_t)(arg0_fp64), (fp64_t)(arg1_fp64), (fp64_t)(arg2_fp64), (fp64_t)(arg3_fp64), (fp64_t)(arg4_fp64));	\
		cdraw_testSc(cdraw_testname(testname,_f), (scBigEpsF), (fp32_t)(expect_fp64), funcF, (fp32_t)(arg0_fp64), (fp32_t)(arg1_fp64), (fp32_t)(arg2_fp64), (fp32_t)(arg3_fp64), (fp32_t)(arg4_fp64))


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
		cdraw_testVec3_rSV(cdraw_testname(testname, _u), cdraw_testname(testname_x, _u), cdraw_testname(testname_y, _u), cdraw_testname(testname_z, _u), 0, expect, vecCopy4i4u(tokencat(expectname, u).v, tokencat(expectname, i).v), tokencat(funcname, u), tokencat(resultname, i).v, vecCopy4i4u(tokencat(arg0name, u).v, tokencat(arg0name, i).v), vecCopy4i4u(tokencat(arg1name, u).v, tokencat(arg1name, i).v))
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
		cdraw_testVec2(cdraw_testname(testname_x, _u), cdraw_testname(testname_y, _u), 0, vecCopy4i4u(tokencat(expectname, u).v, tokencat(expectname, i).v), tokencat(funcname, u), vecCopy4i4u(tokencat(resultname, u).v, tokencat(resultname, i).v), vecCopy4i4u(tokencat(arg0name, u).v, tokencat(arg0name, i).v))
#define cdraw_testVec3IU_1V(testname_x,testname_y,testname_z,expectname,funcname,resultname,arg0name)																																\
		cdraw_testVec3(cdraw_testname(testname_x, _i), cdraw_testname(testname_y, _i), cdraw_testname(testname_z, _i), 0, tokencat(expectname, i).v, tokencat(funcname, i), tokencat(resultname, i).v, tokencat(arg0name, i).v);	\
		cdraw_testVec3(cdraw_testname(testname_x, _u), cdraw_testname(testname_y, _u), cdraw_testname(testname_z, _u), 0, vecCopy4i4u(tokencat(expectname, u).v, tokencat(expectname, i).v), tokencat(funcname, u), vecCopy4i4u(tokencat(resultname, u).v, tokencat(resultname, i).v), vecCopy4i4u(tokencat(arg0name, u).v, tokencat(arg0name, i).v))
#define cdraw_testVec4IU_1V(testname_x,testname_y,testname_z,testname_w,expectname,funcname,resultname,arg0name)																																					\
		cdraw_testVec4(cdraw_testname(testname_x, _i), cdraw_testname(testname_y, _i), cdraw_testname(testname_z, _i), cdraw_testname(testname_w, _i), 0, tokencat(expectname, i).v, tokencat(funcname, i), tokencat(resultname, i).v, tokencat(arg0name, i).v);	\
		cdraw_testVec4(cdraw_testname(testname_x, _u), cdraw_testname(testname_y, _u), cdraw_testname(testname_z, _u), cdraw_testname(testname_w, _u), 0, vecCopy4i4u(tokencat(expectname, u).v, tokencat(expectname, i).v), tokencat(funcname, u), vecCopy4i4u(tokencat(resultname, u).v, tokencat(resultname, i).v), vecCopy4i4u(tokencat(arg0name, u).v, tokencat(arg0name, i).v))
#define cdraw_testVec2IU_1V1S(testname_x,testname_y,expectname,funcname,resultname,arg0name,arg1)																										\
		cdraw_testVec2(cdraw_testname(testname_x, _i), cdraw_testname(testname_y, _i), 0, tokencat(expectname, i).v, tokencat(funcname, i), tokencat(resultname, i).v, tokencat(arg0name, i).v, arg1);	\
		cdraw_testVec2(cdraw_testname(testname_x, _u), cdraw_testname(testname_y, _u), 0, vecCopy4i4u(tokencat(expectname, u).v, tokencat(expectname, i).v), tokencat(funcname, u), vecCopy4i4u(tokencat(resultname, u).v, tokencat(resultname, i).v), vecCopy4i4u(tokencat(arg0name, u).v, tokencat(arg0name, i).v), tokencat(arg1, u))
#define cdraw_testVec3IU_1V1S(testname_x,testname_y,testname_z,expectname,funcname,resultname,arg0name,arg1)																															\
		cdraw_testVec3(cdraw_testname(testname_x, _i), cdraw_testname(testname_y, _i), cdraw_testname(testname_z, _i), 0, tokencat(expectname, i).v, tokencat(funcname, i), tokencat(resultname, i).v, tokencat(arg0name, i).v, arg1);	\
		cdraw_testVec3(cdraw_testname(testname_x, _u), cdraw_testname(testname_y, _u), cdraw_testname(testname_z, _u), 0, vecCopy4i4u(tokencat(expectname, u).v, tokencat(expectname, i).v), tokencat(funcname, u), vecCopy4i4u(tokencat(resultname, u).v, tokencat(resultname, i).v), vecCopy4i4u(tokencat(arg0name, u).v, tokencat(arg0name, i).v), tokencat(arg1, u))
#define cdraw_testVec4IU_1V1S(testname_x,testname_y,testname_z,testname_w,expectname,funcname,resultname,arg0name,arg1)																																					\
		cdraw_testVec4(cdraw_testname(testname_x, _i), cdraw_testname(testname_y, _i), cdraw_testname(testname_z, _i), cdraw_testname(testname_w, _i), 0, tokencat(expectname, i).v, tokencat(funcname, i), tokencat(resultname, i).v, tokencat(arg0name, i).v, arg1);	\
		cdraw_testVec4(cdraw_testname(testname_x, _u), cdraw_testname(testname_y, _u), cdraw_testname(testname_z, _u), cdraw_testname(testname_w, _u), 0, vecCopy4i4u(tokencat(expectname, u).v, tokencat(expectname, i).v), tokencat(funcname, u), vecCopy4i4u(tokencat(resultname, u).v, tokencat(resultname, i).v), vecCopy4i4u(tokencat(arg0name, u).v, tokencat(arg0name, i).v), tokencat(arg1, u))
#define cdraw_testVec2IU_2V(testname_x,testname_y,expectname,funcname,resultname,arg0name,arg1name)																															\
		cdraw_testVec2(cdraw_testname(testname_x, _i), cdraw_testname(testname_y, _i), 0, tokencat(expectname, i).v, tokencat(funcname, i), tokencat(resultname, i).v, tokencat(arg0name, i).v, tokencat(arg1name, i).v);	\
		cdraw_testVec2(cdraw_testname(testname_x, _u), cdraw_testname(testname_y, _u), 0, vecCopy4i4u(tokencat(expectname, u).v, tokencat(expectname, i).v), tokencat(funcname, u), vecCopy4i4u(tokencat(resultname, u).v, tokencat(resultname, i).v), vecCopy4i4u(tokencat(arg0name, u).v, tokencat(arg0name, i).v), vecCopy4i4u(tokencat(arg1name, u).v, tokencat(arg1name, i).v))
#define cdraw_testVec3IU_2V(testname_x,testname_y,testname_z,expectname,funcname,resultname,arg0name,arg1name)																																				\
		cdraw_testVec3(cdraw_testname(testname_x, _i), cdraw_testname(testname_y, _i), cdraw_testname(testname_z, _i), 0, tokencat(expectname, i).v, tokencat(funcname, i), tokencat(resultname, i).v, tokencat(arg0name, i).v, tokencat(arg1name, i).v);	\
		cdraw_testVec3(cdraw_testname(testname_x, _u), cdraw_testname(testname_y, _u), cdraw_testname(testname_z, _u), 0, vecCopy4i4u(tokencat(expectname, u).v, tokencat(expectname, i).v), tokencat(funcname, u), vecCopy4i4u(tokencat(resultname, u).v, tokencat(resultname, i).v), vecCopy4i4u(tokencat(arg0name, u).v, tokencat(arg0name, i).v), vecCopy4i4u(tokencat(arg1name, u).v, tokencat(arg1name, i).v))
#define cdraw_testVec4IU_2V(testname_x,testname_y,testname_z,testname_w,expectname,funcname,resultname,arg0name,arg1name)																																									\
		cdraw_testVec4(cdraw_testname(testname_x, _i), cdraw_testname(testname_y, _i), cdraw_testname(testname_z, _i), cdraw_testname(testname_w, _i), 0, tokencat(expectname, i).v, tokencat(funcname, i), tokencat(resultname, i).v, tokencat(arg0name, i).v, tokencat(arg1name, i).v);	\
		cdraw_testVec4(cdraw_testname(testname_x, _u), cdraw_testname(testname_y, _u), cdraw_testname(testname_z, _u), cdraw_testname(testname_w, _u), 0, vecCopy4i4u(tokencat(expectname, u).v, tokencat(expectname, i).v), tokencat(funcname, u), vecCopy4i4u(tokencat(resultname, u).v, tokencat(resultname, i).v), vecCopy4i4u(tokencat(arg0name, u).v, tokencat(arg0name, i).v), vecCopy4i4u(tokencat(arg1name, u).v, tokencat(arg1name, i).v))
#define cdraw_testVec2IU_1S2V(testname_x,testname_y,expectname,funcname,resultname,arg0,arg1name,arg2name)																														\
		cdraw_testVec2(cdraw_testname(testname_x, _i), cdraw_testname(testname_y, _i), 0, tokencat(expectname, i).v, tokencat(funcname, i), tokencat(resultname, i).v, arg0, tokencat(arg1name, i).v, tokencat(arg2name, i).v);	\
		cdraw_testVec2(cdraw_testname(testname_x, _u), cdraw_testname(testname_y, _u), 0, vecCopy4i4u(tokencat(expectname, u).v, tokencat(expectname, i).v), tokencat(funcname, u), vecCopy4i4u(tokencat(resultname, u).v, tokencat(resultname, i).v), tokencat(arg0, u), vecCopy4i4u(tokencat(arg1name, u).v, tokencat(arg1name, i).v), vecCopy4i4u(tokencat(arg2name, u).v, tokencat(arg2name, i).v))
#define cdraw_testVec3IU_1S2V(testname_x,testname_y,testname_z,expectname,funcname,resultname,arg0,arg1name,arg2name)																																			\
		cdraw_testVec3(cdraw_testname(testname_x, _i), cdraw_testname(testname_y, _i), cdraw_testname(testname_z, _i), 0, tokencat(expectname, i).v, tokencat(funcname, i), tokencat(resultname, i).v, arg0, tokencat(arg1name, i).v, tokencat(arg2name, i).v);	\
		cdraw_testVec3(cdraw_testname(testname_x, _u), cdraw_testname(testname_y, _u), cdraw_testname(testname_z, _u), 0, vecCopy4i4u(tokencat(expectname, u).v, tokencat(expectname, i).v), tokencat(funcname, u), vecCopy4i4u(tokencat(resultname, u).v, tokencat(resultname, i).v), tokencat(arg0, u), vecCopy4i4u(tokencat(arg1name, u).v, tokencat(arg1name, i).v), vecCopy4i4u(tokencat(arg2name, u).v, tokencat(arg2name, i).v))
#define cdraw_testVec4IU_1S2V(testname_x,testname_y,testname_z,testname_w,expectname,funcname,resultname,arg0,arg1name,arg2name)																																								\
		cdraw_testVec4(cdraw_testname(testname_x, _i), cdraw_testname(testname_y, _i), cdraw_testname(testname_z, _i), cdraw_testname(testname_w, _i), 0, tokencat(expectname, i).v, tokencat(funcname, i), tokencat(resultname, i).v, arg0, tokencat(arg1name, i).v, tokencat(arg2name, i).v);	\
		cdraw_testVec4(cdraw_testname(testname_x, _u), cdraw_testname(testname_y, _u), cdraw_testname(testname_z, _u), cdraw_testname(testname_w, _u), 0, vecCopy4i4u(tokencat(expectname, u).v, tokencat(expectname, i).v), tokencat(funcname, u), vecCopy4i4u(tokencat(resultname, u).v, tokencat(resultname, i).v), tokencat(arg0, u), vecCopy4i4u(tokencat(arg1name, u).v, tokencat(arg1name, i).v), vecCopy4i4u(tokencat(arg2name, u).v, tokencat(arg2name, i).v))
#define cdraw_testVec2IU_rr2V(testname_x,testname_y,expect0name,expect1name,funcname,result0name,result1name,arg0name,arg1name)																																							\
		cdraw_testVec2_rr(cdraw_testname(testname_x, _i), cdraw_testname(testname_y, _i), 0, tokencat(expect0name, i).v, tokencat(expect1name, i).v, tokencat(funcname, i), tokencat(result0name, i).v, tokencat(result1name, i).v, tokencat(arg0name, i).v, tokencat(arg1name, i).v);	\
		cdraw_testVec2_rr(cdraw_testname(testname_x, _u), cdraw_testname(testname_y, _u), 0, vecCopy4i4u(tokencat(expect0name, u).v, tokencat(expect0name, i).v), vecCopy4i4u(tokencat(expect1name, u).v, tokencat(expect1name, i).v), tokencat(funcname, u), vecCopy4i4u(tokencat(result0name, u).v, tokencat(result0name, i).v), vecCopy4i4u(tokencat(result1name, u).v, tokencat(result1name, i).v), vecCopy4i4u(tokencat(arg0name, u).v, tokencat(arg0name, i).v), vecCopy4i4u(tokencat(arg1name, u).v, tokencat(arg1name, i).v))
#define cdraw_testVec3IU_rr2V(testname_x,testname_y,testname_z,expect0name,expect1name,funcname,result0name,result1name,arg0name,arg1name)																																												\
		cdraw_testVec3_rr(cdraw_testname(testname_x, _i), cdraw_testname(testname_y, _i), cdraw_testname(testname_z, _i), 0, tokencat(expect0name, i).v, tokencat(expect1name, i).v, tokencat(funcname, i), tokencat(result0name, i).v, tokencat(result1name, i).v, tokencat(arg0name, i).v, tokencat(arg1name, i).v);	\
		cdraw_testVec3_rr(cdraw_testname(testname_x, _u), cdraw_testname(testname_y, _u), cdraw_testname(testname_z, _u), 0, vecCopy4i4u(tokencat(expect0name, u).v, tokencat(expect0name, i).v), vecCopy4i4u(tokencat(expect1name, u).v, tokencat(expect1name, i).v), tokencat(funcname, u), vecCopy4i4u(tokencat(result0name, u).v, tokencat(result0name, i).v), vecCopy4i4u(tokencat(result1name, u).v, tokencat(result1name, i).v), vecCopy4i4u(tokencat(arg0name, u).v, tokencat(arg0name, i).v), vecCopy4i4u(tokencat(arg1name, u).v, tokencat(arg1name, i).v))
#define cdraw_testVec4IU_rr2V(testname_x,testname_y,testname_z,testname_w,expect0name,expect1name,funcname,result0name,result1name,arg0name,arg1name)																																																	\
		cdraw_testVec4_rr(cdraw_testname(testname_x, _i), cdraw_testname(testname_y, _i), cdraw_testname(testname_z, _i), cdraw_testname(testname_w, _i), 0, tokencat(expect0name, i).v, tokencat(expect1name, i).v, tokencat(funcname, i), tokencat(result0name, i).v, tokencat(result1name, i).v, tokencat(arg0name, i).v, tokencat(arg1name, i).v);	\
		cdraw_testVec4_rr(cdraw_testname(testname_x, _u), cdraw_testname(testname_y, _u), cdraw_testname(testname_z, _u), cdraw_testname(testname_w, _u), 0, vecCopy4i4u(tokencat(expect0name, u).v, tokencat(expect0name, i).v), vecCopy4i4u(tokencat(expect1name, u).v, tokencat(expect1name, i).v), tokencat(funcname, u), vecCopy4i4u(tokencat(result0name, u).v, tokencat(result0name, i).v), vecCopy4i4u(tokencat(result1name, u).v, tokencat(result1name, i).v), vecCopy4i4u(tokencat(arg0name, u).v, tokencat(arg0name, i).v), vecCopy4i4u(tokencat(arg1name, u).v, tokencat(arg1name, i).v))
#define cdraw_testVec2IU_rr1V1S(testname_x,testname_y,expect0name,expect1name,funcname,result0name,result1name,arg0name,arg1)																																		\
		cdraw_testVec2_rr(cdraw_testname(testname_x, _i), cdraw_testname(testname_y, _i), 0, tokencat(expect0name, i).v, tokencat(expect1name, i).v, tokencat(funcname, i), tokencat(result0name, i).v, tokencat(result1name, i).v, tokencat(arg0name, i).v, arg1);	\
		cdraw_testVec2_rr(cdraw_testname(testname_x, _u), cdraw_testname(testname_y, _u), 0, vecCopy4i4u(tokencat(expect0name, u).v, tokencat(expect0name, i).v), vecCopy4i4u(tokencat(expect1name, u).v, tokencat(expect1name, i).v), tokencat(funcname, u), vecCopy4i4u(tokencat(result0name, u).v, tokencat(result0name, i).v), vecCopy4i4u(tokencat(result1name, u).v, tokencat(result1name, i).v), vecCopy4i4u(tokencat(arg0name, u).v, tokencat(arg0name, i).v), tokencat(arg1, u))
#define cdraw_testVec3IU_rr1V1S(testname_x,testname_y,testname_z,expect0name,expect1name,funcname,result0name,result1name,arg0name,arg1)																																							\
		cdraw_testVec3_rr(cdraw_testname(testname_x, _i), cdraw_testname(testname_y, _i), cdraw_testname(testname_z, _i), 0, tokencat(expect0name, i).v, tokencat(expect1name, i).v, tokencat(funcname, i), tokencat(result0name, i).v, tokencat(result1name, i).v, tokencat(arg0name, i).v, arg1);	\
		cdraw_testVec3_rr(cdraw_testname(testname_x, _u), cdraw_testname(testname_y, _u), cdraw_testname(testname_z, _u), 0, vecCopy4i4u(tokencat(expect0name, u).v, tokencat(expect0name, i).v), vecCopy4i4u(tokencat(expect1name, u).v, tokencat(expect1name, i).v), tokencat(funcname, u), vecCopy4i4u(tokencat(result0name, u).v, tokencat(result0name, i).v), vecCopy4i4u(tokencat(result1name, u).v, tokencat(result1name, i).v), vecCopy4i4u(tokencat(arg0name, u).v, tokencat(arg0name, i).v), tokencat(arg1, u))
#define cdraw_testVec4IU_rr1V1S(testname_x,testname_y,testname_z,testname_w,expect0name,expect1name,funcname,result0name,result1name,arg0name,arg1)																																													\
		cdraw_testVec4_rr(cdraw_testname(testname_x, _i), cdraw_testname(testname_y, _i), cdraw_testname(testname_z, _i), cdraw_testname(testname_w, _i), 0, tokencat(expect0name, i).v, tokencat(expect1name, i).v, tokencat(funcname, i), tokencat(result0name, i).v, tokencat(result1name, i).v, tokencat(arg0name, i).v, arg1);	\
		cdraw_testVec4_rr(cdraw_testname(testname_x, _u), cdraw_testname(testname_y, _u), cdraw_testname(testname_z, _u), cdraw_testname(testname_w, _u), 0, vecCopy4i4u(tokencat(expect0name, u).v, tokencat(expect0name, i).v), vecCopy4i4u(tokencat(expect1name, u).v, tokencat(expect1name, i).v), tokencat(funcname, u), vecCopy4i4u(tokencat(result0name, u).v, tokencat(result0name, i).v), vecCopy4i4u(tokencat(result1name, u).v, tokencat(result1name, i).v), vecCopy4i4u(tokencat(arg0name, u).v, tokencat(arg0name, i).v), tokencat(arg1, u))


// all of the above for DF


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
		cdraw_testScDF_rB1S(isfalse, scIsZeroApproxD, scIsZeroApproxF, x_B);
		cdraw_testScDF_rB1S(isfalse, scIsZeroApproxD, scIsZeroApproxF, x_C);

		cdraw_testSc_rB(cdraw_isfalse_d, scIsNonZeroApproxD, scEpsD);
		cdraw_testSc_rB(cdraw_isfalse_f, scIsNonZeroApproxF, scEpsF);
		cdraw_testScDF_rB1S(istrue, scIsNonZeroApproxD, scIsNonZeroApproxF, x_B);
		cdraw_testScDF_rB1S(istrue, scIsNonZeroApproxD, scIsNonZeroApproxF, x_C);

		cdraw_testSc_rB(cdraw_isfalse_d, scIsPositiveApproxD, scEpsD);
		cdraw_testSc_rB(cdraw_isfalse_f, scIsPositiveApproxF, scEpsF);
		cdraw_testScDF_rB1S(istrue, scIsPositiveApproxD, scIsPositiveApproxF, x_B);
		cdraw_testScDF_rB1S(isfalse, scIsPositiveApproxD, scIsPositiveApproxF, x_C);

		cdraw_testSc_rB(cdraw_istrue_d, scIsNonPositiveApproxD, scEpsD);
		cdraw_testSc_rB(cdraw_istrue_f, scIsNonPositiveApproxF, scEpsF);
		cdraw_testScDF_rB1S(isfalse, scIsNonPositiveApproxD, scIsNonPositiveApproxF, x_B);
		cdraw_testScDF_rB1S(istrue, scIsNonPositiveApproxD, scIsNonPositiveApproxF, x_C);

		cdraw_testSc_rB(cdraw_isfalse_d, scIsNegativeApproxD, scEpsD);
		cdraw_testSc_rB(cdraw_isfalse_f, scIsNegativeApproxF, scEpsF);
		cdraw_testScDF_rB1S(isfalse, scIsNegativeApproxD, scIsNegativeApproxF, x_B);
		cdraw_testScDF_rB1S(istrue, scIsNegativeApproxD, scIsNegativeApproxF, x_C);

		cdraw_testSc_rB(cdraw_istrue_d, scIsNonNegativeApproxD, scEpsD);
		cdraw_testSc_rB(cdraw_istrue_f, scIsNonNegativeApproxF, scEpsF);
		cdraw_testScDF_rB1S(istrue, scIsNonNegativeApproxD, scIsNonNegativeApproxF, x_B);
		cdraw_testScDF_rB1S(isfalse, scIsNonNegativeApproxD, scIsNonNegativeApproxF, x_C);
	}
	
	{
		fp64_t const x_A = +2.5, x_B = -1.25, x_C = 0.0, x_num = 4.0;

		cdraw_testScDF_1S(isexact, +1.0, scSgnD, scSgnF, x_A);
		cdraw_testScDF_1S(isexact, -1.0, scSgnD, scSgnF, x_B);
		cdraw_testScDF_1S(isexact, 0.00, scSgnD, scSgnF, x_C);

		cdraw_testScDF_1S(isexact, +x_A, scAbsD, scAbsF, x_A);
		cdraw_testScDF_1S(isexact, -x_B, scAbsD, scAbsF, x_B);
		cdraw_testScDF_1S(isexact, 0.00, scAbsD, scAbsF, x_C);

		cdraw_testScDF_1S(isapprox, 0.50, scFracD, scFracF, x_A);
		cdraw_testScDF_1S(isapprox, 0.75, scFracD, scFracF, x_B);
		cdraw_testScDF_1S(isapprox, 0.00, scFracD, scFracF, x_C);

		cdraw_testScDF_1S(isapprox, +0.4, scRecipD, scRecipF, x_A);
		cdraw_testScDF_1S(isapprox, -0.8, scRecipD, scRecipF, x_B);
		cdraw_testScDF_1S(isunsafe, 0.00, scRecipD, scRecipF, x_C);

		cdraw_testScDF_2S(isapprox, +1.6, scDivD, scDivF, x_num, x_A);
		cdraw_testScDF_2S(isapprox, -3.2, scDivD, scDivF, x_num, x_B);
		cdraw_testScDF_2S(isunsafe, 0.00, scDivD, scDivF, x_num, x_C);

		cdraw_testScDF_2S(isapprox, +1.5, scModD, scModF, x_num, x_A);
		cdraw_testScDF_2S(isapprox, -1.0, scModD, scModF, x_num, x_B);
		cdraw_testScDF_2S(isunsafe, x_num, scModD, scModF, x_num, x_C);
	}

	{
		fp64_t const x_A = +0.75, x_B = +1.25, x_C = -0.25, x_min = -2.0, x_max = +6.0, x_min_dst = +50.0, x_max_dst = +100.0;

		cdraw_testScDF_3S(isapprox, +2.5, scMadD, scMadF, x_A, x_min, x_max);
		cdraw_testScDF_3S(isapprox, +5.5, scMadD, scMadF, x_B, x_min, x_max);
		cdraw_testScDF_3S(isapprox, -3.5, scMadD, scMadF, x_C, x_min, x_max);

		cdraw_testScDF_3S(isapprox, +4.00, scLerpD, scLerpF, x_A, x_min, x_max);
		cdraw_testScDF_3S(isapprox, +8.00, scLerpD, scLerpF, x_B, x_min, x_max);
		cdraw_testScDF_3S(isapprox, x_min, scLerpD, scLerpF, x_C, x_min, x_min);

		cdraw_testScDF_3S(isapprox, x_A, scLerpInvD, scLerpInvF, 4.0, x_min, x_max);
		cdraw_testScDF_3S(isapprox, x_B, scLerpInvD, scLerpInvF, 8.0, x_min, x_max);
		cdraw_testScDF_3S(isunsafe, 0.0, scLerpInvD, scLerpInvF, x_C, x_min, x_min);

		cdraw_testScDF_3S(isexact, x_A, scClampD, scClampF, x_A, x_min, x_max);
		cdraw_testScDF_3S(isexact, x_max, scClampD, scClampF, +x_max_dst, x_min, x_max);
		cdraw_testScDF_3S(isexact, x_min, scClampD, scClampF, -x_max_dst, x_min, x_max);

		cdraw_testScDF_3S(isapprox, x_A, scRepeatD, scRepeatF, x_A, x_min, x_max);
		cdraw_testScDF_3S(isapprox, 4.0, scRepeatD, scRepeatF, x_max_dst, x_min, x_max);
		cdraw_testScDF_3S(isunsafe, x_C, scRepeatD, scRepeatF, x_C, x_min, x_min);

		cdraw_testScDF_5S(isapprox, 87.5, scRemapD, scRemapF, +4.0, x_min, x_max, x_min_dst, x_max_dst);
		cdraw_testScDF_5S(isapprox, x_min_dst, scRemapD, scRemapF, +8.0, x_min, x_max, x_min_dst, x_min_dst);
		cdraw_testScDF_5S(isunsafe, x_min_dst, scRemapD, scRemapF, x_C, x_min, x_min, x_min_dst, x_max_dst);

		cdraw_testScDF_rB3S(istrue, scIsInRangeD, scIsInRangeF, x_A, x_min, x_max);
		cdraw_testScDF_rB3S(isfalse, scIsInRangeD, scIsInRangeF, x_B, x_min_dst, x_max_dst);
		cdraw_testScDF_rB3S(istrue, scIsInRangeD, scIsInRangeF, x_C, x_C, x_C);

		cdraw_testScDF_rB1S(istrue, scIsInUnitRangeD, scIsInUnitRangeF, x_A);
		cdraw_testScDF_rB1S(isfalse, scIsInUnitRangeD, scIsInUnitRangeF, x_B);
		cdraw_testScDF_rB1S(isfalse, scIsInUnitRangeD, scIsInUnitRangeF, x_C);

		cdraw_testScDF_rB1S(istrue, scIsInSymmRangeD, scIsInSymmRangeF, x_A);
		cdraw_testScDF_rB1S(isfalse, scIsInSymmRangeD, scIsInSymmRangeF, x_B);
		cdraw_testScDF_rB1S(istrue, scIsInSymmRangeD, scIsInSymmRangeF, x_C);
	}

	{
		fp64_t const x_A = +64.0, x_B = -64.0, x_C = 0.0;

		cdraw_testScDF_1S(isapprox, 8.0, scSqrtD, scSqrtF, x_A);
		cdraw_testScDF_1S(isunsafe, 0.0, scSqrtD, scSqrtF, x_B);
		cdraw_testScDF_1S(isexact, 0.00, scSqrtD, scSqrtF, x_C);

		cdraw_testScDF_1S(isapprox, 0.125, scSqrtInvD, scSqrtInvF, x_A);
		cdraw_testScDF_1S(isunsafe, 0.000, scSqrtInvD, scSqrtInvF, x_B);
		cdraw_testScDF_1S(isunsafe, 0.000, scSqrtInvD, scSqrtInvF, x_C);

		cdraw_testScDF_1S(isapprox, +4.0, scCbrtD, scCbrtF, x_A);
		cdraw_testScDF_1S(isapprox, -4.0, scCbrtD, scCbrtF, x_B);
		cdraw_testScDF_1S(isexact, 0.000, scCbrtD, scCbrtF, x_C);

		cdraw_testScDF_1S(isapprox, +0.25, scCbrtInvD, scCbrtInvF, x_A);
		cdraw_testScDF_1S(isapprox, -0.25, scCbrtInvD, scCbrtInvF, x_B);
		cdraw_testScDF_1S(isunsafe, 0.000, scCbrtInvD, scCbrtInvF, x_C);
	}

	{
		fp64_t const x_r = sc3Pi_2D, x_d = sc270D, x_rr = -scPi_4D, x_dd = -sc45D;

		cdraw_testScDF_1S(isapprox, x_r, scDeg2RadD, scDeg2RadF, x_d);
		cdraw_testScDF_1S(isapprox, x_rr, scDeg2RadD, scDeg2RadF, x_dd);
		
		cdraw_testScDF_1S(isapprox, x_d, scRad2DegD, scRad2DegF, x_r);
		cdraw_testScDF_1S(isapprox, x_dd, scRad2DegD, scRad2DegF, x_rr);
		
		cdraw_testScDF_1S(isapprox, -sc90D, scNormalizeAngle180D, scNormalizeAngle180F, x_d);
		cdraw_testScDF_1S(isapprox, x_dd, scNormalizeAngle180D, scNormalizeAngle180F, x_dd);
		
		cdraw_testScDF_1S(isapprox, x_d, scNormalizeAngle360D, scNormalizeAngle360F, x_d);
		cdraw_testScDF_1S(isapprox, 315.0, scNormalizeAngle360D, scNormalizeAngle360F, x_dd);
		
		cdraw_testScDF_1S(isapprox, -scPi_2D, scNormalizeAnglePiD, scNormalizeAnglePiF, x_r);
		cdraw_testScDF_1S(isapprox, x_rr, scNormalizeAnglePiD, scNormalizeAnglePiF, x_rr);
		
		cdraw_testScDF_1S(isapprox, x_r, scNormalizeAngle2PiD, scNormalizeAngle2PiF, x_r);
		cdraw_testScDF_1S(isapprox, (scPi_4D * 7.0), scNormalizeAngle2PiD, scNormalizeAngle2PiF, x_rr);

		cdraw_testScDF_1S(isapprox, -1.0, scSinrD, scSinrF, x_r);
		cdraw_testScDF_1S(isapprox, -scSqrt1_2D, scSinrD, scSinrF, x_rr);

		cdraw_testScDF_1S(isapprox, x_r - sc2PiD, scAsinrD, scAsinrF, -1.0);
		cdraw_testScDF_1S(isapprox, x_rr, scAsinrD, scAsinrF, -scSqrt1_2D);
		cdraw_testScDF_1S(isunsafe, 0.0, scAsinrD, scAsinrF, -2.0);

		cdraw_testScDF_1S(isapprox, 0.0, scCosrD, scCosrF, x_r);
		cdraw_testScDF_1S(isapprox, +scSqrt1_2D, scCosrD, scCosrF, x_rr);

		cdraw_testScDF_1S(isapprox, x_r - scPi_2D, scAcosrD, scAcosrF, -1.0);
		cdraw_testScDF_1S(isapprox, x_rr + scPiD, scAcosrD, scAcosrF, -scSqrt1_2D);
		cdraw_testScDF_1S(isunsafe, 1.0, scAcosrD, scAcosrF, -2.0);

		cdraw_testScDF_1S(isunsafe, 0.0, scTanrD, scTanrF, x_r);
		cdraw_testScDF_1S(isapprox, -1.0, scTanrD, scTanrF, x_rr);

		cdraw_testScDF_1S(isapprox, 0.0, scAtanrD, scAtanrF, 0.0);
		cdraw_testScDF_1S(isapprox, x_rr, scAtanrD, scAtanrF, -1.0);

		cdraw_testScDF_2S(isapprox, (x_r - sc2PiD), scAtan2rD, scAtan2rF, -1.0, 0.0);
		cdraw_testScDF_2S(isapprox, x_rr, scAtan2rD, scAtan2rF, -1.0, +1.0);
		cdraw_testScDF_2S(isunsafe, 0.0, scAtan2rD, scAtan2rF, 0.0, 0.0);

		cdraw_testScDF_1S(isapprox, -1.0, scSindD, scSindF, x_d);
		cdraw_testScDF_1S(isapprox, -scSqrt1_2D, scSindD, scSindF, x_dd);

		cdraw_testScDF_1S(isapprox, x_d - sc360D, scAsindD, scAsindF, -1.0);
		cdraw_testScDF_1S(isapprox, x_dd, scAsindD, scAsindF, -scSqrt1_2D);
		cdraw_testScDF_1S(isunsafe, 0.0, scAsindD, scAsindF, -2.0);

		cdraw_testScDF_1S(isapprox, 0.0, scCosdD, scCosdF, x_d);
		cdraw_testScDF_1S(isapprox, +scSqrt1_2D, scCosdD, scCosdF, x_dd);

		cdraw_testScDF_1S(isapprox, x_d - sc90D, scAcosdD, scAcosdF, -1.0);
		cdraw_testScDF_1S(isapprox, x_dd + sc180D, scAcosdD, scAcosdF, -scSqrt1_2D);
		cdraw_testScDF_1S(isunsafe, 1.0, scAcosdD, scAcosdF, -2.0);

		cdraw_testScDF_1S(isunsafe, 0.0, scTandD, scTandF, x_d);
		cdraw_testScDF_1S(isapprox, -1.0, scTandD, scTandF, x_dd);

		cdraw_testScDF_1S(isapprox, 0.0, scAtandD, scAtandF, 0.0);
		cdraw_testScDF_1S(isapprox, x_dd, scAtandD, scAtandF, -1.0);

		cdraw_testScDF_2S(isapprox, (x_d - sc360D), scAtan2dD, scAtan2dF, -1.0, 0.0);
		cdraw_testScDF_2S(isapprox, x_dd, scAtan2dD, scAtan2dF, -1.0, +1.0);
		cdraw_testScDF_2S(isunsafe, 0.0, scAtan2dD, scAtan2dF, 0.0, 0.0);
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

		// copy f/d

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

		// copy f/d

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

		// copy f/d

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
		//vec4f_t resultf = vecID4f;
		//vec4d_t resultd = vecID4d;

		{
			//vec4i_t expect0i, param0i, param1i, expect1i, resultAi;
			//vec4u_t expect0u, param0u, param1u, expect1u, resultAu;

		}
	}

	result_return();
}


result_t cdrawMathTest()
{
	result_init();
	cdrawScalarTest();
	cdrawVectorTest();
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