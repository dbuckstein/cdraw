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
* cdrawMatrix.h
* Definition of matrix types.
*/

#ifndef _CDRAW_MATRIX_H_
#define _CDRAW_MATRIX_H_


#include "cdrawVector.h"


/******************************************************************************
* Type definitions.
******************************************************************************/

/// <summary>
/// Array-based single-precision matrices, faster and more explicit than objects: 2D or pointer.
/// </summary>
typedef float2_t							float2x2_t[2], * floatNx2_t;

/// <summary>
/// Array-based single-precision matrices, faster and more explicit than objects: 3D or pointer.
/// </summary>
typedef float3_t							float3x3_t[3], * floatNx3_t;

/// <summary>
/// Array-based single-precision matrices, faster and more explicit than objects: 4D or pointer.
/// </summary>
typedef float4_t							float4x4_t[4], * floatNx4_t;

/// <summary>
/// Array-based double-precision matrices, faster and more explicit than objects: 2D or pointer.
/// </summary>
typedef double2_t							double2x2_t[2], * doubleNx2_t;

/// <summary>
/// Array-based double-precision matrices, faster and more explicit than objects: 3D or pointer.
/// </summary>
typedef double3_t							double3x3_t[3], * doubleNx3_t;

/// <summary>
/// Array-based double-precision matrices, faster and more explicit than objects: 4D or pointer.
/// </summary>
typedef double4_t							double4x4_t[4], * doubleNx4_t;


/// <summary>
/// Single-precision 2D column-major matrix container for array and named columns in matrix.
/// </summary>
typedef union mat2f_t
{
	/// <summary>
	/// Array-based data representing all columns.
	/// </summary>
	float2x2_t m;

	/// <summary>
	/// Array of column vectors.
	/// </summary>
	vec2f_t cv[2];

	struct
	{
		/// <summary>
		/// Named first column and elements.
		/// </summary>
		union { vec2f_t cx, cv0, m0; struct { vecf_t m00, m01; }; };

		/// <summary>
		/// Named second column and elements.
		/// </summary>
		union { vec2f_t cy, cv1, m1; struct { vecf_t m10, m11; }; };
	};
} mat2f_t;

/// <summary>
/// Single-precision 3D column-major matrix container for array and named columns in matrix.
/// </summary>
typedef union mat3f_t
{
	/// <summary>
	/// Array-based data representing all columns.
	/// </summary>
	float3x3_t m;

	/// <summary>
	/// Array of column vectors.
	/// </summary>
	vec3f_t cv[3];

	struct
	{
		/// <summary>
		/// Named first column and elements.
		/// </summary>
		union { vec3f_t cx, cv0, m0; struct { vecf_t m00, m01, m02; }; };

		union
		{
			struct
			{
				/// <summary>
				/// Named second column and elements.
				/// </summary>
				union { vec3f_t cy, cv1, m1; struct { vecf_t m10, m11, m12; }; };

				/// <summary>
				/// Named third column and elements.
				/// </summary>
				union { vec3f_t cz, cv2, m2; struct { vecf_t m20, m21, m22; }; };
			};
		};
	};
} mat3f_t;

/// <summary>
/// Single-precision 4D column-major matrix container for array and named columns in matrix.
/// </summary>
typedef union mat4f_t
{
	/// <summary>
	/// Array-based data representing all columns.
	/// </summary>
	float4x4_t m;

	/// <summary>
	/// Array of column vectors.
	/// </summary>
	vec4f_t cv[4];

	struct
	{
		/// <summary>
		/// Named first column and elements.
		/// </summary>
		union { vec4f_t cx, cv0, m0; struct { vecf_t m00, m01, m02, m03; }; };

		union
		{
			struct
			{
				/// <summary>
				/// Named second column and elements.
				/// </summary>
				union { vec4f_t cy, cv1, m1; struct { vecf_t m10, m11, m12, m13; }; };

				union
				{
					struct
					{
						/// <summary>
						/// Named third column and elements.
						/// </summary>
						union { vec4f_t cz, cv2, m2; struct { vecf_t m20, m21, m22, m23; }; };

						/// <summary>
						/// Named fourth column and elements.
						/// </summary>
						union { vec4f_t cw, cv3, m3; struct { vecf_t m30, m31, m32, m33; }; };
					};
				};
			};
		};
	};
} mat4f_t;


/// <summary>
/// Double-precision 2D column-major matrix container for array and named columns in matrix.
/// </summary>
typedef union mat2d_t
{
	/// <summary>
	/// Array-based data representing all columns.
	/// </summary>
	double2x2_t m;

	/// <summary>
	/// Array of column vectors.
	/// </summary>
	vec2d_t cv[2];

	struct
	{
		/// <summary>
		/// Named first column and elements.
		/// </summary>
		union { vec2d_t cx, cv0, m0; struct { vecd_t m00, m01; }; };

		/// <summary>
		/// Named second column and elements.
		/// </summary>
		union { vec2d_t cy, cv1, m1; struct { vecd_t m10, m11; }; };
	};
} mat2d_t;

/// <summary>
/// Double-precision 3D column-major matrix container for array and named columns in matrix.
/// </summary>
typedef union mat3d_t
{
	/// <summary>
	/// Array-based data representing all columns.
	/// </summary>
	double3x3_t m;

	/// <summary>
	/// Array of column vectors.
	/// </summary>
	vec3d_t cv[3];

	struct
	{
		/// <summary>
		/// Named first column and elements.
		/// </summary>
		union { vec3d_t cx, cv0, m0; struct { vecd_t m00, m01, m02; }; };

		union
		{
			struct
			{
				/// <summary>
				/// Named second column and elements.
				/// </summary>
				union { vec3d_t cy, cv1, m1; struct { vecd_t m10, m11, m12; }; };

				/// <summary>
				/// Named third column and elements.
				/// </summary>
				union { vec3d_t cz, cv2, m2; struct { vecd_t m20, m21, m22; }; };
			};
		};
	};
} mat3d_t;

/// <summary>
/// Double-precision 4D column-major matrix container for array and named columns in matrix.
/// </summary>
typedef union mat4d_t
{
	/// <summary>
	/// Array-based data representing all columns.
	/// </summary>
	double4x4_t m;

	/// <summary>
	/// Array of column vectors.
	/// </summary>
	vec4d_t cv[4];

	struct
	{
		/// <summary>
		/// Named first column and elements.
		/// </summary>
		union { vec4d_t cx, cv0, m0; struct { vecd_t m00, m01, m02, m03; }; };

		union
		{
			struct
			{
				/// <summary>
				/// Named second column and elements.
				/// </summary>
				union { vec4d_t cy, cv1, m1; struct { vecd_t m10, m11, m12, m13; }; };

				union
				{
					struct
					{
						/// <summary>
						/// Named third column and elements.
						/// </summary>
						union { vec4d_t cz, cv2, m2; struct { vecd_t m20, m21, m22, m23; }; };

						/// <summary>
						/// Named fourth column and elements.
						/// </summary>
						union { vec4d_t cw, cv3, m3; struct { vecd_t m30, m31, m32, m33; }; };
					};
				};
			};
		};
	};
} mat4d_t;


/******************************************************************************
* Configuration.
******************************************************************************/

#define cdraw_matrix_base(name)													tokencat(name, matrix_suffix)
#define cdraw_matrix_decl_sc(name,len,...)										\
		tokencat(mat, tokencat(len, f_t)) tokencat(name, f) = { __VA_ARGS__ };	\
		tokencat(mat, tokencat(len, d_t)) tokencat(name, d) = { __VA_ARGS__ }
#define cdraw_matrix_decl2DF(name,xx_value_fp64,xy_value_fp64,yx_value_fp64,yy_value_fp64)			\
		mat2d_t tokencat(name, 2D) = {	xx_value_fp64, xy_value_fp64,								\
										yx_value_fp64, yy_value_fp64	};							\
		mat2f_t tokencat(name, 2F) = {	tokencat(xx_value_fp64, f), tokencat(xy_value_fp64, f),		\
										tokencat(yx_value_fp64, f), tokencat(yy_value_fp64, f)	}
#define cdraw_matrix_decl3DF(name,xx_value_fp64,xy_value_fp64,xz_value_fp64,yx_value_fp64,yy_value_fp64,yz_value_fp64,zx_value_fp64,zy_value_fp64,zz_value_fp64)	\
		mat3d_t tokencat(name, 3D) = {	xx_value_fp64, xy_value_fp64, xz_value_fp64,																				\
										yx_value_fp64, yy_value_fp64, yz_value_fp64,																				\
										zx_value_fp64, zy_value_fp64, zz_value_fp64	};																				\
		mat3f_t tokencat(name, 3F) = {	tokencat(xx_value_fp64, f), tokencat(xy_value_fp64, f), tokencat(xz_value_fp64, f),											\
										tokencat(yx_value_fp64, f), tokencat(yy_value_fp64, f), tokencat(yz_value_fp64, f),											\
										tokencat(zx_value_fp64, f), tokencat(zy_value_fp64, f), tokencat(zz_value_fp64, f)	}
#define cdraw_matrix_decl4DF(name,xx_value_fp64,xy_value_fp64,xz_value_fp64,xw_value_fp64,yx_value_fp64,yy_value_fp64,yz_value_fp64,yw_value_fp64,zx_value_fp64,zy_value_fp64,zz_value_fp64,zxw_value_fp64,wx_value_fp64,wy_value_fp64,wz_value_fp64,ww_value_fp64)	\
		mat4d_t tokencat(name, 4D) = {	xx_value_fp64, xy_value_fp64, xz_value_fp64, xw_value_fp64,																																									\
										yx_value_fp64, yy_value_fp64, yz_value_fp64, yw_value_fp64,																																									\
										zx_value_fp64, zy_value_fp64, zz_value_fp64, zw_value_fp64,																																									\
										wx_value_fp64, wy_value_fp64, wz_value_fp64, ww_value_fp64	};																																								\
		mat4f_t tokencat(name, 4F) = {	tokencat(xx_value_fp64, f), tokencat(xy_value_fp64, f), tokencat(xz_value_fp64, f), tokencat(xw_value_fp64, f),																												\
										tokencat(yx_value_fp64, f), tokencat(yy_value_fp64, f), tokencat(yz_value_fp64, f), tokencat(yw_value_fp64, f),																												\
										tokencat(zx_value_fp64, f), tokencat(zy_value_fp64, f), tokencat(zz_value_fp64, f), tokencat(zw_value_fp64, f),																												\
										tokencat(wx_value_fp64, f), tokencat(wy_value_fp64, f), tokencat(wz_value_fp64, f), tokencat(ww_value_fp64, f)	}

#define mxx(m) ((m)[0][0])	// Access first component of first column of matrix for read/write ('cx.x').
#define mxy(m) ((m)[0][1])	// Access second component of first column of matrix for read/write ('cx.y').
#define mxz(m) ((m)[0][2])	// Access third component of first column of matrix for read/write ('cx.z').
#define mxw(m) ((m)[0][3])	// Access fourth component of first column of matrix for read/write ('cx.w').
#define myx(m) ((m)[1][0])	// Access first component of second column of matrix for read/write ('cy.x').
#define myy(m) ((m)[1][1])	// Access second component of second column of matrix for read/write ('cy.y').
#define myz(m) ((m)[1][2])	// Access third component of second column of matrix for read/write ('cy.z').
#define myw(m) ((m)[1][3])	// Access fourth component of second column of matrix for read/write ('cy.w').
#define mzx(m) ((m)[2][0])	// Access first component of third column of matrix for read/write ('cz.x').
#define mzy(m) ((m)[2][1])	// Access second component of third column of matrix for read/write ('cz.y').
#define mzz(m) ((m)[2][2])	// Access third component of third column of matrix for read/write ('cz.z').
#define mzw(m) ((m)[2][3])	// Access fourth component of third column of matrix for read/write ('cz.w').
#define mwx(m) ((m)[3][0])	// Access first component of fourth column of matrix for read/write ('cw.x').
#define mwy(m) ((m)[3][1])	// Access second component of fourth column of matrix for read/write ('cw.y').
#define mwz(m) ((m)[3][2])	// Access third component of fourth column of matrix for read/write ('cw.z').
#define mww(m) ((m)[3][3])	// Access fourth component of fourth column of matrix for read/write ('cw.w').


#if CDRAW_USING_SCALAR_PREF
/// <summary>
/// Array-based preferred precision matrices, faster and more explicit than objects: 2D or pointer.
/// </summary>
typedef scalar2_t							scalar2x2_t[2], * scalarNx2_t;
/// <summary>
/// Array-based preferred precision matrices, faster and more explicit than objects: 3D or pointer.
/// </summary>
typedef scalar3_t							scalar3x3_t[3], * scalarNx3_t;
/// <summary>
/// Array-based preferred precision matrices, faster and more explicit than objects: 4D or pointer.
/// </summary>
typedef scalar4_t							scalar4x4_t[4], * scalarNx4_t;

#if CDRAW_USING_SCALAR_PREF_DOUBLE
typedef mat2d_t			mat2_t;
typedef mat3d_t			mat3_t;
typedef mat4d_t			mat4_t;
#else // #if CDRAW_USING_SCALAR_PREF_DOUBLE
typedef mat2f_t			mat2_t;
typedef mat3f_t			mat3_t;
typedef mat4f_t			mat4_t;
#endif // #else // #if CDRAW_USING_SCALAR_PREF_DOUBLE
#define cdraw_matrix_decl(name,len,...)	\
		cdraw_matrix_decl_sc(name, len, __VA_ARGS__);	\
		tokencat(mat, tokencat(len, _t)) name = { __VA_ARGS__ }
#else // #if CDRAW_USING_SCALAR_PREF
#define cdraw_matrix_decl(name,len,...)	\
		cdraw_matrix_decl_sc(name, len, __VA_ARGS__)
#endif // #else // #if CDRAW_USING_SCALAR_PREF
#define matrix_suffix	vector_suffix
#define matrix_other	vector_other
#define cdraw_matrix_const(name,len,...)										cdraw_matrix_decl(static const name, len, __VA_ARGS__)


/******************************************************************************
* Constant matrices.
******************************************************************************/

cdraw_matrix_const(matID2, 2, 1, 0, 0, 1);
cdraw_matrix_const(matID3, 3, 1, 0, 0, 0, 1, 0, 0, 0, 1);
cdraw_matrix_const(matID4, 4, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1);


/******************************************************************************
* Functions.
******************************************************************************/

#include "cdrawMatrix/cdrawMat2f.h"
#include "cdrawMatrix/cdrawMat2d.h"

#include "cdrawMatrix/cdrawMat3f.h"
#include "cdrawMatrix/cdrawMat3d.h"

#include "cdrawMatrix/cdrawMat4f.h"
#include "cdrawMatrix/cdrawMat4d.h"

#if CDRAW_USING_SCALAR_PREF
#include "cdrawMatrix/cdrawMat2.h"
#include "cdrawMatrix/cdrawMat3.h"
#include "cdrawMatrix/cdrawMat4.h"
#endif // #if CDRAW_USING_SCALAR_PREF


#if !CDRAW_USING_PRECOMPILE_LIBS
#include "_inl/cdrawMatrix.inl"
#endif // #if !CDRAW_USING_PRECOMPILE_LIBS


#endif // #ifndef _CDRAW_MATRIX_H_