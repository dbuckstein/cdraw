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
* cdrawTransform2d.inl
* Definitions of double-precision 2D transform functions.
*/

#if (!(defined _CDRAW_TRANSFORM2D_INL_) && (defined _CDRAW_TRANSFORM_INL_))
#define _CDRAW_TRANSFORM2D_INL_


CDRAW_INL doubleNx2_t RmatID2d(Rmat2d_t R_out)
{
	failassert(R_out, NULL);
	return matIdentity2d(R_out);
}

CDRAW_INL doubleNx2_t RmatFromAngle2d(Rmat2d_t R_out, angle2d_t* angle_actual_out_opt, angle2d_t const angle)
{
	failassert(R_out, NULL);
	angle2d_t const angle_actual = scNormalizeAngle180D(angle);
	vecd_t s, c;
	scSinCosdD(&s, &c, angle_actual);
	mxx(R_out) = c;		myx(R_out) = -s;
	mxy(R_out) = s;		myy(R_out) = c;
	if (angle_actual_out_opt) *angle_actual_out_opt = angle_actual;
	return R_out;
}

CDRAW_INL angle2d_t RmatToAngle2d(Rmat2d_t const R)
{
	failassert(R, sc0D);
	return scAtan2dD(mxy(R), mxx(R));
}


#endif // #if (!(defined _CDRAW_TRANSFORM2D_INL_) && (defined _CDRAW_TRANSFORM_INL_))