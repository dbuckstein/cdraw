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
* cdrawTransform2f.inl
* Definitions of single-precision 2D transform functions.
*/

#if (!(defined _CDRAW_TRANSFORM2F_INL_) && (defined _CDRAW_TRANSFORM_INL_))
#define _CDRAW_TRANSFORM2F_INL_


CDRAW_INL floatNx2_t RmatID2f(Rmat2f_t R_out)
{
	failassert(R_out, NULL);
	return matIdentity2f(R_out);
}

CDRAW_INL floatNx2_t RmatInv2f(Rmat2f_t R_out, Rmat2f_t const R)
{
	failassert(R_out && R, NULL);
	return matTranspose2f(R_out, R);
}

CDRAW_INL floatNx2_t RmatFromAngle2f(Rmat2f_t R_out, angle2f_t* angle_actual_out_opt, angle2f_t const angle)
{
	failassert(R_out, NULL);
	angle2f_t const angle_actual = scNormalizeAngle180F(angle);
	vecf_t s, c;
	scSinCosdF(&s, &c, angle_actual);
	mxx(R_out) = c;		myx(R_out) = -s;
	mxy(R_out) = s;		myy(R_out) = c;
	if (angle_actual_out_opt) *angle_actual_out_opt = angle_actual;
	return R_out;
}

CDRAW_INL angle2f_t RmatToAngle2f(Rmat2f_t const R)
{
	failassert(R, sc0F);
	return scAtan2dF(mxy(R), mxx(R));
}


#endif // #if (!(defined _CDRAW_TRANSFORM2F_INL_) && (defined _CDRAW_TRANSFORM_INL_))