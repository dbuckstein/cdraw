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
* cdrawVector.h
* Definition of vector types.
*/

#ifndef _CDRAW_VECTOR_H_
#define _CDRAW_VECTOR_H_


#include "cdrawScalar.h"


/******************************************************************************
* Type definitions.
******************************************************************************/

/// <summary>
/// Array-based integer vectors, faster and more explicit than objects: 2D, 3D, 4D or pointer.
/// </summary>
typedef int32_t								veci_t, int2_t[2], int3_t[3], int4_t[4], * intN_t;

/// <summary>
/// Array-based unsigned vectors, faster and more explicit than objects: 2D, 3D, 4D or pointer.
/// </summary>
typedef uint32_t							vecu_t, uint2_t[2], uint3_t[3], uint4_t[4], * uintN_t;

/// <summary>
/// Array-based boolean vectors, faster and more explicit than objects: 2D, 3D, 4D or pointer.
/// </summary>
typedef bool								vecb_t, bool2_t[2], bool3_t[3], bool4_t[4], * boolN_t;

/// <summary>
/// Array-based single-precision vectors, faster and more explicit than objects: 2D, 3D, 4D or pointer.
/// </summary>
typedef fp32_t								vecf_t, float2_t[2], float3_t[3], float4_t[4], * floatN_t;

/// <summary>
/// Array-based double-precision vectors, faster and more explicit than objects: 2D, 3D, 4D or pointer.
/// </summary>
typedef fp64_t								vecd_t, double2_t[2], double3_t[3], double4_t[4], * doubleN_t;


/// <summary>
/// Integer 2D vector container for array and named components in vector, color and parametric spaces.
/// </summary>
typedef union vec2i_t
{
	/// <summary>
	/// Array-based data representing all components.
	/// </summary>
	int2_t v;

	struct
	{
		/// <summary>
		/// Named first component.
		/// </summary>
		union { veci_t x, r, s; };

		/// <summary>
		/// Named second component.
		/// </summary>
		union { veci_t y, g, t; };
	};
} vec2i_t;

/// <summary>
/// Integer 3D vector container for array and named components in vector, color and parametric spaces.
/// </summary>
typedef union vec3i_t
{
	/// <summary>
	/// Array-based data representing all components.
	/// </summary>
	int3_t v;

	/// <summary>
	/// Partial swizzle of first and second components.
	/// </summary>
	vec2i_t xy, rg, st;

	struct
	{
		/// <summary>
		/// Named first component.
		/// </summary>
		union { veci_t x, r, s; };

		union
		{
			/// <summary>
			/// Partial swizzle of second and third components.
			/// </summary>
			vec2i_t yz, gb, tp;

			struct
			{
				/// <summary>
				/// Named second component.
				/// </summary>
				union { veci_t y, g, t; };

				/// <summary>
				/// Named third component.
				/// </summary>
				union { veci_t z, b, p; };
			};
		};
	};
} vec3i_t;

/// <summary>
/// Integer 4D vector container for array and named components in vector, color and parametric spaces.
/// </summary>
typedef union vec4i_t
{
	/// <summary>
	/// Array-based data representing all components.
	/// </summary>
	int4_t v;

	/// <summary>
	/// Partial swizzle of first, second and third components.
	/// </summary>
	vec3i_t xyz, rgb, stp;

	/// <summary>
	/// Partial swizzle of first and second components.
	/// </summary>
	vec2i_t xy, rg, st;

	struct
	{
		/// <summary>
		/// Named first component.
		/// </summary>
		union { veci_t x, r, s; };

		union
		{
			/// <summary>
			/// Partial swizzle of second, third and fourth components.
			/// </summary>
			vec3i_t yzw, gba, tpq;

			/// <summary>
			/// Partial swizzle of second and third components.
			/// </summary>
			vec2i_t yz, gb, tp;

			struct
			{
				/// <summary>
				/// Named second component.
				/// </summary>
				union { veci_t y, g, t; };

				union
				{
					/// <summary>
					/// Partial swizzle of third and fourth components.
					/// </summary>
					vec2i_t zw, ba, pq;

					struct
					{
						/// <summary>
						/// Named third component.
						/// </summary>
						union { veci_t z, b, p; };

						/// <summary>
						/// Named fourth component.
						/// </summary>
						union { veci_t w, a, q; };
					};
				};
			};
		};
	};
} vec4i_t;


/// <summary>
/// Unsigned 2D vector container for array and named components in vector, color and parametric spaces.
/// </summary>
typedef union vec2u_t
{
	/// <summary>
	/// Array-based data representing all components.
	/// </summary>
	uint2_t v;

	struct
	{
		/// <summary>
		/// Named first component.
		/// </summary>
		union { vecu_t x, r, s; };

		/// <summary>
		/// Named second component.
		/// </summary>
		union { vecu_t y, g, t; };
	};
} vec2u_t;

/// <summary>
/// Unsigned 3D vector container for array and named components in vector, color and parametric spaces.
/// </summary>
typedef union vec3u_t
{
	/// <summary>
	/// Array-based data representing all components.
	/// </summary>
	uint3_t v;

	/// <summary>
	/// Partial swizzle of first and second components.
	/// </summary>
	vec2u_t xy, rg, st;

	struct
	{
		/// <summary>
		/// Named first component.
		/// </summary>
		union { vecu_t x, r, s; };

		union
		{
			/// <summary>
			/// Partial swizzle of second and third components.
			/// </summary>
			vec2u_t yz, gb, tp;

			struct
			{
				/// <summary>
				/// Named second component.
				/// </summary>
				union { vecu_t y, g, t; };

				/// <summary>
				/// Named third component.
				/// </summary>
				union { vecu_t z, b, p; };
			};
		};
	};
} vec3u_t;

/// <summary>
/// Unsigned 4D vector container for array and named components in vector, color and parametric spaces.
/// </summary>
typedef union vec4u_t
{
	/// <summary>
	/// Array-based data representing all components.
	/// </summary>
	uint4_t v;

	/// <summary>
	/// Partial swizzle of first, second and third components.
	/// </summary>
	vec3u_t xyz, rgb, stp;

	/// <summary>
	/// Partial swizzle of first and second components.
	/// </summary>
	vec2u_t xy, rg, st;

	struct
	{
		/// <summary>
		/// Named first component.
		/// </summary>
		union { vecu_t x, r, s; };

		union
		{
			/// <summary>
			/// Partial swizzle of second, third and fourth components.
			/// </summary>
			vec3u_t yzw, gba, tpq;

			/// <summary>
			/// Partial swizzle of second and third components.
			/// </summary>
			vec2u_t yz, gb, tp;

			struct
			{
				/// <summary>
				/// Named second component.
				/// </summary>
				union { vecu_t y, g, t; };

				union
				{
					/// <summary>
					/// Partial swizzle of third and fourth components.
					/// </summary>
					vec2u_t zw, ba, pq;

					struct
					{
						/// <summary>
						/// Named third component.
						/// </summary>
						union { vecu_t z, b, p; };

						/// <summary>
						/// Named fourth component.
						/// </summary>
						union { vecu_t w, a, q; };
					};
				};
			};
		};
	};
} vec4u_t;


/// <summary>
/// Boolean 2D vector container for array and named components in vector, color and parametric spaces.
/// </summary>
typedef union vec2b_t
{
	/// <summary>
	/// Array-based data representing all components.
	/// </summary>
	bool2_t v;

	struct
	{
		/// <summary>
		/// Named first component.
		/// </summary>
		union { vecb_t x, r, s; };

		/// <summary>
		/// Named second component.
		/// </summary>
		union { vecb_t y, g, t; };
	};
} vec2b_t;

/// <summary>
/// Boolean 3D vector container for array and named components in vector, color and parametric spaces.
/// </summary>
typedef union vec3b_t
{
	/// <summary>
	/// Array-based data representing all components.
	/// </summary>
	bool3_t v;

	/// <summary>
	/// Partial swizzle of first and second components.
	/// </summary>
	vec2b_t xy, rg, st;

	struct
	{
		/// <summary>
		/// Named first component.
		/// </summary>
		union { vecb_t x, r, s; };

		union
		{
			/// <summary>
			/// Partial swizzle of second and third components.
			/// </summary>
			vec2b_t yz, gb, tp;

			struct
			{
				/// <summary>
				/// Named second component.
				/// </summary>
				union { vecb_t y, g, t; };

				/// <summary>
				/// Named third component.
				/// </summary>
				union { vecb_t z, b, p; };
			};
		};
	};
} vec3b_t;

/// <summary>
/// Boolean 4D vector container for array and named components in vector, color and parametric spaces.
/// </summary>
typedef union vec4b_t
{
	/// <summary>
	/// Array-based data representing all components.
	/// </summary>
	bool4_t v;

	/// <summary>
	/// Partial swizzle of first, second and third components.
	/// </summary>
	vec3b_t xyz, rgb, stp;

	/// <summary>
	/// Partial swizzle of first and second components.
	/// </summary>
	vec2b_t xy, rg, st;

	struct
	{
		/// <summary>
		/// Named first component.
		/// </summary>
		union { vecb_t x, r, s; };

		union
		{
			/// <summary>
			/// Partial swizzle of second, third and fourth components.
			/// </summary>
			vec3b_t yzw, gba, tpq;

			/// <summary>
			/// Partial swizzle of second and third components.
			/// </summary>
			vec2b_t yz, gb, tp;

			struct
			{
				/// <summary>
				/// Named second component.
				/// </summary>
				union { vecb_t y, g, t; };

				union
				{
					/// <summary>
					/// Partial swizzle of third and fourth components.
					/// </summary>
					vec2b_t zw, ba, pq;

					struct
					{
						/// <summary>
						/// Named third component.
						/// </summary>
						union { vecb_t z, b, p; };

						/// <summary>
						/// Named fourth component.
						/// </summary>
						union { vecb_t w, a, q; };
					};
				};
			};
		};
	};
} vec4b_t;


/// <summary>
/// Single-precision 2D vector container for array and named components in vector, color and parametric spaces.
/// </summary>
typedef union vec2f_t
{
	/// <summary>
	/// Array-based data representing all components.
	/// </summary>
	float2_t v;

	struct
	{
		/// <summary>
		/// Named first component.
		/// </summary>
		union { vecf_t x, r, s; };

		/// <summary>
		/// Named second component.
		/// </summary>
		union { vecf_t y, g, t; };
	};
} vec2f_t;

/// <summary>
/// Single-precision 3D vector container for array and named components in vector, color and parametric spaces.
/// </summary>
typedef union vec3f_t
{
	/// <summary>
	/// Array-based data representing all components.
	/// </summary>
	float3_t v;

	/// <summary>
	/// Partial swizzle of first and second components.
	/// </summary>
	vec2f_t xy, rg, st;

	struct
	{
		/// <summary>
		/// Named first component.
		/// </summary>
		union { vecf_t x, r, s; };

		union
		{
			/// <summary>
			/// Partial swizzle of second and third components.
			/// </summary>
			vec2f_t yz, gb, tp;

			struct
			{
				/// <summary>
				/// Named second component.
				/// </summary>
				union { vecf_t y, g, t; };

				/// <summary>
				/// Named third component.
				/// </summary>
				union { vecf_t z, b, p; };
			};
		};
	};
} vec3f_t;

/// <summary>
/// Single-precision 4D vector container for array and named components in vector, color and parametric spaces.
/// </summary>
typedef union vec4f_t
{
	/// <summary>
	/// Array-based data representing all components.
	/// </summary>
	float4_t v;

	/// <summary>
	/// Partial swizzle of first, second and third components.
	/// </summary>
	vec3f_t xyz, rgb, stp;

	/// <summary>
	/// Partial swizzle of first and second components.
	/// </summary>
	vec2f_t xy, rg, st;

	struct
	{
		/// <summary>
		/// Named first component.
		/// </summary>
		union { vecf_t x, r, s; };

		union
		{
			/// <summary>
			/// Partial swizzle of second, third and fourth components.
			/// </summary>
			vec3f_t yzw, gba, tpq;

			/// <summary>
			/// Partial swizzle of second and third components.
			/// </summary>
			vec2f_t yz, gb, tp;

			struct
			{
				/// <summary>
				/// Named second component.
				/// </summary>
				union { vecf_t y, g, t; };

				union
				{
					/// <summary>
					/// Partial swizzle of third and fourth components.
					/// </summary>
					vec2f_t zw, ba, pq;

					struct
					{
						/// <summary>
						/// Named third component.
						/// </summary>
						union { vecf_t z, b, p; };

						/// <summary>
						/// Named fourth component.
						/// </summary>
						union { vecf_t w, a, q; };
					};
				};
			};
		};
	};
} vec4f_t;


/// <summary>
/// Double-precision 2D vector container for array and named components in vector, color and parametric spaces.
/// </summary>
typedef union vec2d_t
{
	/// <summary>
	/// Array-based data representing all components.
	/// </summary>
	double2_t v;

	struct
	{
		/// <summary>
		/// Named first component.
		/// </summary>
		union { vecd_t x, r, s; };

		/// <summary>
		/// Named second component.
		/// </summary>
		union { vecd_t y, g, t; };
	};
} vec2d_t;

/// <summary>
/// Double-precision 3D vector container for array and named components in vector, color and parametric spaces.
/// </summary>
typedef union vec3d_t
{
	/// <summary>
	/// Array-based data representing all components.
	/// </summary>
	double3_t v;

	/// <summary>
	/// Partial swizzle of first and second components.
	/// </summary>
	vec2d_t xy, rg, st;

	struct
	{
		/// <summary>
		/// Named first component.
		/// </summary>
		union { vecd_t x, r, s; };

		union
		{
			/// <summary>
			/// Partial swizzle of second and third components.
			/// </summary>
			vec2d_t yz, gb, tp;

			struct
			{
				/// <summary>
				/// Named second component.
				/// </summary>
				union { vecd_t y, g, t; };

				/// <summary>
				/// Named third component.
				/// </summary>
				union { vecd_t z, b, p; };
			};
		};
	};
} vec3d_t;

/// <summary>
/// Double-precision 4D vector container for array and named components in vector, color and parametric spaces.
/// </summary>
typedef union vec4d_t
{
	/// <summary>
	/// Array-based data representing all components.
	/// </summary>
	double4_t v;

	/// <summary>
	/// Partial swizzle of first, second and third components.
	/// </summary>
	vec3d_t xyz, rgb, stp;

	/// <summary>
	/// Partial swizzle of first and second components.
	/// </summary>
	vec2d_t xy, rg, st;

	struct
	{
		/// <summary>
		/// Named first component.
		/// </summary>
		union { vecd_t x, r, s; };

		union
		{
			/// <summary>
			/// Partial swizzle of second, third and fourth components.
			/// </summary>
			vec3d_t yzw, gba, tpq;

			/// <summary>
			/// Partial swizzle of second and third components.
			/// </summary>
			vec2d_t yz, gb, tp;

			struct
			{
				/// <summary>
				/// Named second component.
				/// </summary>
				union { vecd_t y, g, t; };

				union
				{
					/// <summary>
					/// Partial swizzle of third and fourth components.
					/// </summary>
					vec2d_t zw, ba, pq;

					struct
					{
						/// <summary>
						/// Named third component.
						/// </summary>
						union { vecd_t z, b, p; };

						/// <summary>
						/// Named fourth component.
						/// </summary>
						union { vecd_t w, a, q; };
					};
				};
			};
		};
	};
} vec4d_t;


/******************************************************************************
* Configuration.
******************************************************************************/

#define cdraw_vector_base(name)													tokencat(name, vector_suffix)
#define cdraw_vector_decl_sc(name,len,...)										\
		tokencat(vec, tokencat(len, b_t)) tokencat(name, b) = { __VA_ARGS__ };	\
		tokencat(vec, tokencat(len, i_t)) tokencat(name, i) = { __VA_ARGS__ };	\
		tokencat(vec, tokencat(len, u_t)) tokencat(name, u) = { __VA_ARGS__ };	\
		tokencat(vec, tokencat(len, f_t)) tokencat(name, f) = { __VA_ARGS__ };	\
		tokencat(vec, tokencat(len, d_t)) tokencat(name, d) = { __VA_ARGS__ }
#define cdraw_vector_decl2B(name,x_value,y_value)																									\
		vec2b_t tokencat(name, 2B) = { x_value, y_value }
#define cdraw_vector_decl3B(name,x_value,y_value,z_value)																							\
		vec3b_t tokencat(name, 3B) = { x_value, y_value, z_value }
#define cdraw_vector_decl4B(name,x_value,y_value,z_value,w_value)																					\
		vec4b_t tokencat(name, 4B) = { x_value, y_value, z_value, w_value }
#define cdraw_vector_decl2IU(name,x_value_int,y_value_int)																							\
		vec2i_t tokencat(name, 2I) = { x_value_int, y_value_int };																					\
		vec2u_t tokencat(name, 2U) = { tokencat(x_value_int, u), tokencat(y_value_int, u) }
#define cdraw_vector_decl3IU(name,x_value_int,y_value_int,z_value_int)																				\
		vec3i_t tokencat(name, 3I) = { x_value_int, y_value_int, z_value_int };																		\
		vec3u_t tokencat(name, 3U) = { tokencat(x_value_int, u), tokencat(y_value_int, u), tokencat(z_value_int, u) }
#define cdraw_vector_decl4IU(name,x_value_int,y_value_int,z_value_int,w_value_int)																	\
		vec4i_t tokencat(name, 4I) = { x_value_int, y_value_int, z_value_int, w_value_int };														\
		vec4u_t tokencat(name, 4U) = { tokencat(x_value_int, u), tokencat(y_value_int, u), tokencat(z_value_int, u), tokencat(w_value_int, u) }
#define cdraw_vector_decl2DF(name,x_value_fp64,y_value_fp64)																						\
		vec2d_t tokencat(name, 2D) = { x_value_fp64, y_value_fp64 };																				\
		vec2f_t tokencat(name, 2F) = { tokencat(x_value_fp64, f), tokencat(y_value_fp64, f) }
#define cdraw_vector_decl3DF(name,x_value_fp64,y_value_fp64,z_value_fp64)																			\
		vec3d_t tokencat(name, 3D) = { x_value_fp64, y_value_fp64, z_value_fp64 };																	\
		vec3f_t tokencat(name, 3F) = { tokencat(x_value_fp64, f), tokencat(y_value_fp64, f), tokencat(z_value_fp64, f) }
#define cdraw_vector_decl4DF(name,x_value_fp64,y_value_fp64,z_value_fp64,w_value_fp64)																\
		vec4d_t tokencat(name, 4D) = { x_value_fp64, y_value_fp64, z_value_fp64, w_value_fp64 };													\
		vec4f_t tokencat(name, 4F) = { tokencat(x_value_fp64, f), tokencat(y_value_fp64, f), tokencat(z_value_fp64, f), tokencat(w_value_fp64, f) }

#if CDRAW_USING_SCALAR_PREF
/// <summary>
/// Array-based preferred precision vectors, faster and more explicit than objects: 2D, 3D, 4D or pointer.
/// </summary>
typedef scalar_t							vec_t, scalar2_t[2], scalar3_t[3], scalar4_t[4], * scalarN_t;
#if CDRAW_USING_SCALAR_PREF_DOUBLE
typedef vec2d_t			vec2_t;
typedef vec3d_t			vec3_t;
typedef vec4d_t			vec4_t;
#define vector_suffix	d
#define vector_other	f
#else // #if CDRAW_USING_SCALAR_PREF_DOUBLE
typedef vec2f_t			vec2_t;
typedef vec3f_t			vec3_t;
typedef vec4f_t			vec4_t;
#define vector_suffix	f
#define vector_other	d
#endif // #else // #if CDRAW_USING_SCALAR_PREF_DOUBLE
#define cdraw_vector_decl(name,len,...)	\
		cdraw_vector_decl_sc(name, len, __VA_ARGS__);	\
		tokencat(vec, tokencat(len, _t)) name = { __VA_ARGS__ }
#else // #if CDRAW_USING_SCALAR_PREF
#define cdraw_vector_decl(name,len,...)	\
		cdraw_vector_decl_sc(name, len, __VA_ARGS__)
#endif // #else // #if CDRAW_USING_SCALAR_PREF
#define cdraw_vector_const(name,len,...)										cdraw_vector_decl(static const name, len, __VA_ARGS__)

#define vx(v) ((v)[0])	// Access first component of vector for read/write ('x').
#define vy(v) ((v)[1])	// Access second component of vector for read/write ('y').
#define vz(v) ((v)[2])	// Access third component of vector for read/write ('z').
#define vw(v) ((v)[3])	// Access fourth component of vector for read/write ('w').


/******************************************************************************
* Constant vectors.
******************************************************************************/

cdraw_vector_const(vecID2, 2, 0, 0);
cdraw_vector_const(vecX2, 2, 1, 0);
cdraw_vector_const(vecY2, 2, 0, 1);
cdraw_vector_const(vecXY2, 2, 1, 1);
cdraw_vector_const(vecID3, 3, 0, 0, 0);
cdraw_vector_const(vecX3, 3, 1, 0, 0);
cdraw_vector_const(vecY3, 3, 0, 1, 0);
cdraw_vector_const(vecZ3, 3, 0, 0, 1);
cdraw_vector_const(vecXY3, 3, 1, 1, 0);
cdraw_vector_const(vecXZ3, 3, 1, 0, 1);
cdraw_vector_const(vecYZ3, 3, 0, 1, 1);
cdraw_vector_const(vecXYZ3, 3, 1, 1, 1);
cdraw_vector_const(vecID4, 4, 0, 0, 0, 0);
cdraw_vector_const(vecX4, 4, 1, 0, 0, 0);
cdraw_vector_const(vecY4, 4, 0, 1, 0, 0);
cdraw_vector_const(vecZ4, 4, 0, 0, 1, 0);
cdraw_vector_const(vecW4, 4, 0, 0, 0, 1);
cdraw_vector_const(vecXY4, 4, 1, 1, 0, 0);
cdraw_vector_const(vecXZ4, 4, 1, 0, 1, 0);
cdraw_vector_const(vecXW4, 4, 1, 0, 0, 1);
cdraw_vector_const(vecYZ4, 4, 0, 1, 1, 0);
cdraw_vector_const(vecYW4, 4, 0, 1, 0, 1);
cdraw_vector_const(vecZW4, 4, 0, 0, 1, 1);
cdraw_vector_const(vecXYZ4, 4, 1, 1, 1, 0);
cdraw_vector_const(vecXYW4, 4, 1, 1, 0, 1);
cdraw_vector_const(vecXZW4, 4, 1, 0, 1, 1);
cdraw_vector_const(vecYZW4, 4, 0, 1, 1, 1);
cdraw_vector_const(vecXYZW4, 4, 1, 1, 1, 1);


/******************************************************************************
* Functions.
******************************************************************************/

#include "cdrawVector/cdrawVec2b.h"
#include "cdrawVector/cdrawVec2i.h"
#include "cdrawVector/cdrawVec2u.h"
#include "cdrawVector/cdrawVec2f.h"
#include "cdrawVector/cdrawVec2d.h"

#include "cdrawVector/cdrawVec3b.h"
#include "cdrawVector/cdrawVec3i.h"
#include "cdrawVector/cdrawVec3u.h"
#include "cdrawVector/cdrawVec3f.h"
#include "cdrawVector/cdrawVec3d.h"

#include "cdrawVector/cdrawVec4b.h"
#include "cdrawVector/cdrawVec4i.h"
#include "cdrawVector/cdrawVec4u.h"
#include "cdrawVector/cdrawVec4f.h"
#include "cdrawVector/cdrawVec4d.h"

#if CDRAW_USING_SCALAR_PREF
#include "cdrawVector/cdrawVec2.h"
#include "cdrawVector/cdrawVec3.h"
#include "cdrawVector/cdrawVec4.h"
#endif // #if CDRAW_USING_SCALAR_PREF


// ****TO-DO: matrix base types - move to matrix
typedef float2_t							float2x2_t[2], * floatNx2;
typedef float3_t							float3x3_t[3], * floatNx3;
typedef float4_t							float4x4_t[4], * floatNx4;
typedef double2_t							double2x2_t[2], * doubleNx2;
typedef double3_t							double3x3_t[3], * doubleNx3;
typedef double4_t							double4x4_t[4], * doubleNx4;


#if !CDRAW_USING_PRECOMPILE_LIBS
#include "_inl/cdrawVector.inl"
#endif // #if !CDRAW_USING_PRECOMPILE_LIBS


#endif // #ifndef _CDRAW_VECTOR_H_