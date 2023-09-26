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
* Functions.
******************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif // #ifdef __cplusplus



#ifdef __cplusplus
}
#endif // #ifdef __cplusplus


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