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
* cdrawVec2.h
* Declaration of preferred floating point precision 2D vector functions.
*/

#if (!(defined _CDRAW_VEC2_H_) && (defined _CDRAW_VECTOR_H_))
#define _CDRAW_VEC2_H_


#ifdef __cplusplus
extern "C" {
#endif // #ifdef __cplusplus



	/// <summary>
	/// Initialize vector by copying the leading components of another.
	/// </summary>
	/// <param name="v_out">Result vector.</param>
	/// <param name="v">Vector to copy.</param>
	/// <returns><paramref name="v_out"/></returns>
#define vecCopy22b tokencat(vecCopy2, tokencat(vector_suffix, 2b))


#ifdef __cplusplus
}
#endif // #ifdef __cplusplus


#endif // #if (!(defined _CDRAW_VEC2_H_) && (defined _CDRAW_VECTOR_H_))