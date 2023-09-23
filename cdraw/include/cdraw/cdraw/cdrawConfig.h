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
* cdrawConfig.h
* Global platform configuration, settings, core types and utilities.
*/

#ifndef _CDRAW_CONFIG_H_
#define _CDRAW_CONFIG_H_


#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <assert.h>


/******************************************************************************
* Platform configuration.
******************************************************************************/

#define CDRAW_USING_WINDOWS					(((defined _WIN32) && (_WIN32 == 1)) || (defined __WIN32__) || (defined __NT__))
#define CDRAW_USING_MACOSX					((defined __APPLE__) || (defined __MACH__))
#define CDRAW_USING_LINUX					(defined __linux__)
#if CDRAW_USING_MACOSX
#include <TargetConditionals.h>
#endif // #if CDRAW_USING_MACOSX
#define CDRAW_TARGET_WINDOWS				(CDRAW_USING_WINDOWS || (CDRAW_USING_MACOSX && (defined TARGET_OS_WIN32) && TARGET_OS_WIN32))
#define CDRAW_TARGET_WIN32					(CDRAW_TARGET_WINDOWS && !(defined _WIN64))
#define CDRAW_TARGET_WIN64					(CDRAW_TARGET_WINDOWS && (defined _WIN64) && (_WIN64 == 1))
#define CDRAW_TARGET_MACOSX					(CDRAW_USING_MACOSX && (defined TARGET_OS_OSX) && TARGET_OS_OSX)
#define CDRAW_TARGET_IOS					(CDRAW_USING_MACOSX && (defined TARGET_OS_IOS) && TARGET_OS_IOS)
#define CDRAW_TARGET_LINUX					(CDRAW_USING_LINUX)
#define CDRAW_TARGET_ANDROID				(defined __ANDROID__)


#define CDRAW_DEBUG							((defined _DEBUG) || ((defined DEBUG) && (DEBUG == 1)))
#define CDRAW_RELEASE						(!CDRAW_DEBUG)


#define CDRAW_64BIT							(UINTPTR_MAX > 0xFFFFFFFF)
#if CDRAW_64BIT
#define CDRAW_BITWIDTH						64
#else // #if CDRAW_64BIT
#define CDRAW_BITWIDTH						32
#endif // #else // #if CDRAW_64BIT


/******************************************************************************
* Global tags and utilities.
******************************************************************************/

#define CDRAW_INL							static inline
#if CDRAW_USING_WINDOWS
#define CDRAW_INL_NEVER						__declspec(noinline)
#define CDRAW_INL_ALWAYS					static __forceinline
#else // #if CDRAW_USING_WINDOWS
#define CDRAW_INL_NEVER						__attribute__((noinline))
#define CDRAW_INL_ALWAYS					static inline __attribute__((always_inline))
#endif // #else // #if CDRAW_USING_WINDOWS


#define CDRAW_ENABLE						&&
#define CDRAW_DISABLE						&&!

// Preprocessor check for feature tagged as enabled.
//	-x: Name of preprocessor macro to check.
#define cdraw_enabled(x)					((defined x## ) && (1 x 1))


// Describes whether framework distribution has its libraries pre-built.
//	-CDRAW_ENABLE if libraries are pre-built and source is not available.
//	-CDRAW_DISABLE if source is available and libraries are not pre-built.
#define CDRAW_PRECOMPILE_LIBS				CDRAW_DISABLE
#define CDRAW_USING_PRECOMPILE_LIBS			cdraw_enabled(CDRAW_PRECOMPILE_LIBS)


#define __cdraw_tokenstr_internal__(x)		#x
#define __cdraw_tokencat_internal__(x,y)	x##y


// Stringify token.
//	-x: Token to represent as c-string.
#define tokenstr(x)							__cdraw_tokenstr_internal__(x)

// Concatenate tokens.
//	-x: First token (left).
//	-y: Second token (right).
#define tokencat(x,y)						__cdraw_tokencat_internal__(x,y)


/******************************************************************************
* Global type definitions and utilities.
******************************************************************************/

typedef unsigned int						uint;		// Unsigned integer (analogous to int)
typedef void*								ptr_t;		// Generic pointer.
typedef void const*							ptrk_t;		// Generic constant pointer.
typedef char*								cstr_t;		// Generic c-style string.
typedef char const*							cstrk_t;	// Generic c-style constant string.
typedef float								fp32_t;		// Single-precision (32-bit) float.
typedef double								fp64_t;		// Double-precision (64-bit) float.
typedef uint8_t								byte_t;		// Byte type (1 byte).
typedef uint16_t							word_t;		// Word type (2 bytes).
typedef uint32_t							dword_t;	// Double-word type (4 bytes).
typedef uint64_t							qword_t;	// Quad-word type (8 bytes).
#define int8_invalid						INT8_MIN	// Global value representing invalid 8-bit signed integer.
#define int16_invalid						INT16_MIN	// Global value representing invalid 16-bit signed integer.
#define int32_invalid						INT32_MIN	// Global value representing invalid 32-bit signed integer.
#define int64_invalid						INT64_MIN	// Global value representing invalid 64-bit signed integer.
#define uint8_invalid						UINT8_MAX	// Global value representing invalid 8-bit unsigned integer.
#define uint16_invalid						UINT16_MAX	// Global value representing invalid 16-bit unsigned integer.
#define uint32_invalid						UINT32_MAX	// Global value representing invalid 32-bit unsigned integer.
#define uint64_invalid						UINT64_MAX	// Global value representing invalid 64-bit unsigned integer.


typedef uint32_t							bitflag_t[8];												// Convenient storage type for 256 bits.
#define flagraise(x,bits)					((x) |= (bits))												// Activate specified bits.
#define flaglower(x,bits)					((x) &=~(bits))												// Deactivate specified bits.
#define flagcheck(x,bits)					((x) &  (bits))												// Non-zero if specified bits are turned on.
#define flagcheckincl(x,bits)				(flagcheck(x, bits) != 0)									// True if specified bits are turned on.
#define flagcheckexcl(x,bits)				(flagcheck(x, bits) == (bits))								// True if specified bits are exclusively turned on.
#define flagscheckneither(x0,x1,bits)		flagcheck(~(x0) & ~(x1), bits)								// Non-zero if specified bits are deactivated in both values.
#define flagscheckenable(x0,x1,bits)		flagcheck(~(x0) &  (x1), bits)								// Non-zero if specified bits are deactivated first and activated second.
#define flagscheckdisable(x0,x1,bits)		flagcheck( (x0) & ~(x1), bits)								// Non-zero if specified bits are activated first and deactivated second.
#define flagscheckboth(x0,x1,bits)			flagcheck( (x0) &  (x1), bits)								// Non-zero if specified bits are activated in both values.
#define bitflagraise(bf,index)				flagraise(bf[index/32], 1<<(index%32))						// Activate bit at specified index.
#define bitflaglower(bf,index)				flaglower(bf[index/32], 1<<(index%32))						// Deactivate bit at specified index.
#define bitflagcheck(bf,index)				flagcheck(bf[index/32], 1<<(index%32))						// Non-zero if bit at specified index is turned on.
#define bitflagscheckneither(bf0,bf1,index)	flagcheck(~bf0[index/32] & ~bf1[index/32], 1<<(index%32))	// Non-zero if bit at specified index is deactivated in both values.
#define bitflagscheckenable(bf0,bf1,index)	flagcheck(~bf0[index/32] &  bf1[index/32], 1<<(index%32))	// Non-zero if bit at specified index is deactivated first and activated second.
#define bitflagscheckdisable(bf0,bf1,index)	flagcheck( bf0[index/32] & ~bf1[index/32], 1<<(index%32))	// Non-zero if bit at specified index is activated first and deactivated second.
#define bitflagscheckboth(bf0,bf1,index)	flagcheck( bf0[index/32] &  bf1[index/32], 1<<(index%32))	// Non-zero if bit at specified index is activated in both values.


typedef uint32_t							result_t;	// Global type representing result of function.
typedef enum errcode_common_t
{
	errcode_invalidarg,		// Global error code for invalid argument passed as parameter.
	errcode_invalidmem,		// Global error code for invalid memory management.
	errcode_CUSTOM_BEGIN	// Global error code template for user functions to start declaring error codes.
} errcode_common_t;
#define result_defaultname					__cdraw_result_internal__
#define result_init()						result_t result_defaultname = 0							// Common result initializer.
#define result_raisewarn(warncode)			flagraise(result_defaultname, (1<<(warncode&0xF)))		// Common result append warning code (bit).
#define result_seterror(errcode)			flagraise(result_defaultname, ((errcode&0xFFFF)<<16))	// Common result set error code (index).
#define result_return()						return result_defaultname								// Common result return.
#define result_isclean(result)				(result == 0)											// True if result is free of warnings and errors.
#define result_haswarns(result)				flagcheckincl(result, 0x0000FFFF)						// True if result contains warnings.
#define result_haserror(result)				flagcheckincl(result, 0xFFFF0000)						// True if result contains error.
#define result_getwarns(result)				flagcheck(result)										// Get warning codes (bits) from result.
#define result_geterror(result)				(result >> 16)											// Get error code (index) from result.


#define buffer_valid(x)						((x)!=NULL&&*(x)!=0)									// True if pointer is initialized and value it points to is non-zero.
#define buffer_len(x)						(sizeof(x) / sizeof(*x))								// Calculate number of elements in buffer (note: does not work on raw pointers).
#define buffer_init1(dst,offset,type)		(((type*)(dst))[offset]=(type)0)						// Initialize single value in buffer to zero.
#define buffer_copy1(dst,src,offset,type)	(((type*)(dst))[offset]=((type const*)(src))[offset])	// Copy single value in buffer.
#define buffer_init4(dst,offset,type)		(((type*)(dst))[offset]=((type*)(dst))[offset+1]=((type*)(dst))[offset+2]=((type*)(dst))[offset+3]=(type)0)										// Initialize set of four values in buffer to zero.
#define buffer_copy4(dst,src,offset,type)	buffer_copy1(dst,src,offset,type);buffer_copy1(dst,src,offset+1,type);buffer_copy1(dst,src,offset+2,type);buffer_copy1(dst,src,offset+3,type)	// Copy set of four values in buffer.


#define label_base_type						uint64_t															// Basis type of label type.
typedef byte_t								label_t[sizeof(label_base_type)*4];									// Convenient label type for predefined small strings or tags.
typedef byte_t const						labelk_t[sizeof(label_base_type)*4];								// Convenient constant label type for predefined small strings or tags.
#define label_term(label)					(label[sizeof(label_t)-1]=0)										// Terminate label string.
#define label_valid(label)					buffer_valid(label)													// True if label string has contents.
#define label_init(label)					buffer_init4(label,0,label_base_type)								// Initialize label string to empty.
#define label_copy(label_dst,label_src)		buffer_copy4(label_dst,label_src,0,label_base_type);label_term(dst)	// Copy and terminate label string.


#define cdraw_assert(condition)				assert(condition)											// Framework assert.
#define failret(condition,...)				if (!(condition)) return __VA_ARGS__						// Return variadic argument if condition fails.
#define failassertret(condition,...)		cdraw_assert(condition); failret(condition, __VA_ARGS__)	// Assert and/or return variadic argument if condition fails.
#define asserterr(condition,errcode)		failassertret(condition, result_seterror(errcode))			// Assert and/or return error result if condition fails.
#define asserterr_range(x,min,max,errcode)	asserterr((x)>=(min)&&(x)<=(max), errcode)					// Assert and/or return error result if value is not in range.
#define asserterr_ptr(ptr,errcode)			asserterr((ptr)!=NULL, errcode)								// Assert and/or return error result if pointer is not initialized.
#define asserterr_ptrval(ptr,errcode)		asserterr(buffer_valid(ptr), errcode)						// Assert and/or return error result if pointer or its value is not initialized.
#define asserterr_cstr(cstr,errorcode)		asserterr(label_valid(cstr), errcode)						// Assert and/or return error result if c-style string is not initialized.
#define asserterr_count(x,count,errcode)	asserterr_range(x, 0, count, errcode)						// Assert and/or return error result if value is negative or exceeds count.


#define swap2(x,y,tmp)						(tmp=x);(x=y);(y=tmp)				// Swap two values.
#define swap3(x,y,z,tmp)					(tmp=x);(x=y);(y=z);(z=tmp)			// Swap three values.
#define swap3r(x,y,z,tmp)					(tmp=x);(x=z);(z=y);(y=tmp)			// Swap three values (alt).
#define swap4(x,y,z,w,tmp)					(tmp=x);(x=y);(y=z);(z=w);(w=tmp)	// Swap four values.
#define swap4r(x,y,z,w,tmp)					(tmp=x);(x=w);(w=z);(z=y);(y=tmp)	// Swap four values (alt).


// ****TO-DO: float constants - move to real
#define f32Pi								3.1415926535897932384626433832795f
#define f64Pi								3.1415926535897932384626433832795


// ****TO-DO: float rands - move to real as functions
#define f32rand()							((fp32_t)(rand()))
#define f32randUnit()						((fp32_t)(rand())*0.000030517578125f)
#define f32randSymm()						((fp32_t)(rand()-16384)*0.00006103515625f)
#define f64rand()							((fp64_t)(rand()))
#define f64randUnit()						((fp64_t)(rand())*0.000030517578125)
#define f64randSymm()						((fp64_t)(rand()-16384)*0.00006103515625)


// ****TO-DO: loop in range - move to real as functions
#define flerp(x,min,max)					((min)+(x)*((max)-(min)))
#define flerpinv(x,min,max)					(((x)-(min))/((max)-(min)))
#define ffrac(x)							((x)-floor(x))
#define fmod(x,y)							((x)-(y)*floor((x)/(y)))
#define fmodrange(x,min,max)				((x)-((max)-(min))*floor(((x)-(min))/((max)-(min))))
#define fclamp(x,min,max)					((x)>=(min)?(x)<=(max)?(x):(max):(min))
#define fisclamped(x,min,max)				((x)>=(min)&&(x)<=(max))
// min,max,abs,sgn,etc


// ****TO-DO: vector/matrix base types - move to vector/matrix
typedef int32_t								int2_t[2], int3_t[3], int4_t[4], * intN_t;
typedef uint32_t							uint2_t[2], uint3_t[3], uint4_t[4], * uintN_t;
typedef bool								bool2_t[2], bool3_t[3], bool4_t[4], * boolN_t;
typedef fp32_t								float2_t[2], float3_t[3], float4_t[4], * floatN_t;
typedef fp64_t								double2_t[2], double3_t[3], double4_t[4], * doubleN_t;
typedef float2_t							float2x2_t[2], * floatNx2;
typedef float3_t							float3x3_t[3], * floatNx3;
typedef float4_t							float4x4_t[4], * floatNx4;
typedef double2_t							double2x2_t[2], * doubleNx2;
typedef double3_t							double3x3_t[3], * doubleNx3;
typedef double4_t							double4x4_t[4], * doubleNx4;


#endif // #ifndef _CDRAW_CONFIG_H_