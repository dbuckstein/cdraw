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
* cdrawTransform3f.inl
* Definitions of single-precision 3D transform functions.
*/

#if (!(defined _CDRAW_TRANSFORM3F_INL_) && (defined _CDRAW_TRANSFORM_INL_))
#define _CDRAW_TRANSFORM3F_INL_


CDRAW_INL floatNx3_t RmatID3f(Rmat3f_t R_out)
{
	failassert(R_out, NULL);
	return matIdentity3f(R_out);
}

CDRAW_INL floatNx3_t RmatInv3f(Rmat3f_t R_out, Rmat3f_t const R)
{
	failassert(R_out && R, NULL);
	return matTranspose3f(R_out, R);
}

CDRAW_INL floatNx3_t RmatFromAnglesXYZ3f(Rmat3f_t R_out, float3_t anglesXYZ_actual_out_opt, float3_t const anglesXYZ)
{
	failassert(R_out && anglesXYZ, NULL);
	vecf_t angle_actual_y = scNormalizeAngle180F(vy(anglesXYZ)), angle_actual_z, sz, cz;
	if (angle_actual_y >= +transform_gimbalangle_minF && angle_actual_y <= +transform_gimbalangle_maxF)
	{
		/*
		* If y=+90 then sy=+1 and cy=0: 
		*					|  0   czsx-szcx  czcx+szsx|
		*		RzR+90Rx=	|  0   szsx+czcx  szcx-czsx|
		*					| -sy     0          0     |
		* 
		*					|  0 -s(z-x)  c(z-x)|
		*				=	|  0  c(z-x)  s(z-x)|
		*					| -1    0       0   |
		*/
		angle_actual_z = scNormalizeAngle180F(vz(anglesXYZ) - vx(anglesXYZ));
		scSinCosdF(&sz, &cz, angle_actual_z);
		vecInit3f(vx(R_out), sc0F, sc0F, -sc1F);
		vecInit3f(vy(R_out), -sz, +cz, sc0F);
		vecInit3f(vz(R_out), +cz, +sz, sc0F);
		if (anglesXYZ_actual_out_opt)
		{
			vx(anglesXYZ_actual_out_opt) = sc0F;
			vy(anglesXYZ_actual_out_opt) = +sc90F;
			vz(anglesXYZ_actual_out_opt) = angle_actual_z;
		}
	}
	else if (angle_actual_y >= -transform_gimbalangle_maxF && angle_actual_y <= -transform_gimbalangle_minF)
	{
		/*
		* If y=-90 then sy=-1 and cy=0: 
		*					|  0  -czsx-szcx -czcx+szsx|
		*		RzR-90Rx=	|  0  -szsx+czcx -szcx-czsx|
		*					| -sy     0          0     |
		* 
		*					|  0 -s(z+x) -c(z+x)|
		*				=	|  0  c(z+x) -s(z+x)|
		*					| +1    0       0   |
		*/
		angle_actual_z = scNormalizeAngle180F(vz(anglesXYZ) + vx(anglesXYZ));
		scSinCosdF(&sz, &cz, angle_actual_z);
		vecInit3f(vx(R_out), sc0F, sc0F, +sc1F);
		vecInit3f(vy(R_out), -sz, +cz, sc0F);
		vecInit3f(vz(R_out), -cz, -sz, sc0F);
		if (anglesXYZ_actual_out_opt)
		{
			vx(anglesXYZ_actual_out_opt) = sc0F;
			vy(anglesXYZ_actual_out_opt) = -sc90F;
			vz(anglesXYZ_actual_out_opt) = angle_actual_z;
		}
	}
	else
	{
		/*
		*					| cz -sz  0 || cy  0   sy|| 1   0   0 |
		*	R =	RzRyRx	=	| sz  cz  0 || 0   1   0 || 0   cx -sx|
		*					| 0   0   1 ||-sy  0   cy|| 0   sx  cx|
		*
		*					| cz -sz  0 || cy  sysx  sycx|
		*				=	| sz  cz  0 || 0     cx   -sx|
		*					| 0   0   1 ||-sy  cysx  cycx|
		*
		*					| czcy  czsysx-szcx  czsycx+szsx|
		*				=	| szcy  szsysx+czcx  szsycx-czsx|
		*					|  -sy    cysx         cycx     |
		*/
		vecf_t sy, cy, sx, cx, sysx, sycx;
		scSinCosdF(&sx, &cx, vx(anglesXYZ));
		scSinCosdF(&sy, &cy, vy(anglesXYZ));
		scSinCosdF(&sz, &cz, vz(anglesXYZ));
		sysx = sy * sx;
		sycx = sy * cx;
		vecInit3f(vx(R_out), +cz * cy, +sz * cy, -sy);
		vecInit3f(vy(R_out), (cz * sysx - sz * cx), (sz * sysx + cz * cx), +cy * sx);
		vecInit3f(vz(R_out), (cz * sycx + sz * sx), (sz * sycx - cz * sx), +cy * cx);
		if (anglesXYZ_actual_out_opt)
		{
			vx(anglesXYZ_actual_out_opt) = scAtan2dF(+myz(R_out), +mzz(R_out));
			vy(anglesXYZ_actual_out_opt) = scAsindF(sy);
			vz(anglesXYZ_actual_out_opt) = scAtan2dF(+mxy(R_out), +mxx(R_out));
		}
	}
	return R_out;
}

CDRAW_INL floatN_t RmatToAnglesXYZ3f(float3_t anglesXYZ_actual_out, Rmat3f_t const R)
{
	failassert(anglesXYZ_actual_out && R, NULL);
	vecf_t const sy = -mxz(R);
	if (sy >= +scBigEpsL1F)
	{
		vx(anglesXYZ_actual_out) = sc0F;
		vy(anglesXYZ_actual_out) = +sc90F;
		vz(anglesXYZ_actual_out) = scAtan2dF(+mzy(R), +mzx(R));
	}
	else if (sy <= -scBigEpsL1F)
	{
		vx(anglesXYZ_actual_out) = sc0F;
		vy(anglesXYZ_actual_out) = -sc90F;
		vz(anglesXYZ_actual_out) = scAtan2dF(-myx(R), +myy(R));
	}
	else
	{
		vx(anglesXYZ_actual_out) = scAtan2dF(+myz(R), +mzz(R));
		vy(anglesXYZ_actual_out) = scAsindF(sy);
		vz(anglesXYZ_actual_out) = scAtan2dF(+mxy(R), +mxx(R));
	}
	return anglesXYZ_actual_out;
}

CDRAW_INL floatNx3_t RmatFromAnglesYZX3f(Rmat3f_t R_out, float3_t anglesXYZ_actual_out_opt, float3_t const anglesXYZ)
{
	failassert(R_out && anglesXYZ, NULL);
	vecf_t angle_actual_z = scNormalizeAngle180F(vz(anglesXYZ)), angle_actual_x, sx, cx;
	if (angle_actual_z >= +transform_gimbalangle_minF && angle_actual_z <= +transform_gimbalangle_maxF)
	{
		/*
		* If z=+90 then sz=+1 and cz=0:
		*					|    0      -sz    0     |
		*		RxR+90Ry=	| cxcy+sxsy  0  cxsy-sxcy|
		*					| sxcy-cxsy  0  sxsy+cxcy|
		*
		*					|   0    -1    0   |
		*				=	| c(x-y)  0 -s(x-y)|
		*					| s(x-y)  0  c(x-y)|
		*/
		angle_actual_x = scNormalizeAngle180F(vx(anglesXYZ) - vy(anglesXYZ));
		scSinCosdF(&sx, &cx, angle_actual_x);
		vecInit3f(vx(R_out), sc0F, +cx, +sx);
		vecInit3f(vy(R_out), -sc1F, sc0F, sc0F);
		vecInit3f(vz(R_out), sc0F, -sx, +cx);
		if (anglesXYZ_actual_out_opt)
		{
			vx(anglesXYZ_actual_out_opt) = angle_actual_x;
			vy(anglesXYZ_actual_out_opt) = sc0F;
			vz(anglesXYZ_actual_out_opt) = +sc90F;
		}
	}
	else if (angle_actual_z >= -transform_gimbalangle_maxF && angle_actual_z <= -transform_gimbalangle_minF)
	{
		/*
		* If z=-90 then sz=-1 and cz=0:
		*					|    0      -sz     0     |
		*		RxR-90Ry=	|-cxcy+sxsy  0  -cxsy-sxcy|
		*					|-sxcy-cxsy  0  -sxsy+cxcy|
		*
		*					|   0    +1     0   |
		*				=	|-c(x+y)  0  -s(x+y)|
		*					|-s(x+y)  0   c(x+y)|
		*/
		angle_actual_x = scNormalizeAngle180F(vx(anglesXYZ) + vy(anglesXYZ));
		scSinCosdF(&sx, &cx, angle_actual_x);
		vecInit3f(vx(R_out), sc0F, -cx, -sx);
		vecInit3f(vy(R_out), +sc1F, sc0F, sc0F);
		vecInit3f(vz(R_out), sc0F, -sx, +cx);
		if (anglesXYZ_actual_out_opt)
		{
			vx(anglesXYZ_actual_out_opt) = angle_actual_x;
			vy(anglesXYZ_actual_out_opt) = sc0F;
			vz(anglesXYZ_actual_out_opt) = -sc90F;
		}
	}
	else
	{
		/*
		*					| 1   0   0 || cz -sz  0 || cy  0   sy|
		*	R =	RxRzRy	=	| 0   cx -sx|| sz  cz  0 || 0   1   0 |
		*					| 0   sx  cx|| 0   0   1 ||-sy  0   cy|
		*
		*					| 1   0   0 || czcy -sz  czsy|
		*				=	| 0   cx -sx|| szcy  cz  szsy|
		*					| 0   sx  cx||  -sy  0     cy|
		*
		*					|   czcy        -sz    czsy     |
		*				=	| cxszcy+sxsy  cxcz  cxszsy-sxcy|
		*					| sxszcy-cxsy  sxcz  sxszsy+cxcy|
		*/
		vecf_t sz, cz, sy, cy, szsy, szcy;
		scSinCosdF(&sx, &cx, vx(anglesXYZ));
		scSinCosdF(&sy, &cy, vy(anglesXYZ));
		scSinCosdF(&sz, &cz, vz(anglesXYZ));
		szsy = sz * sy;
		szcy = sz * cy;
		vecInit3f(vx(R_out), +cz * cy, (cx * szcy + sx * sy), (sx * szcy - cx * sy));
		vecInit3f(vy(R_out), -sz, +cx * cz, +sx * cz);
		vecInit3f(vz(R_out), +cz * sy, (cx * szsy - sx * cy), (sx * szsy + cx * cy));
		if (anglesXYZ_actual_out_opt)
		{
			vx(anglesXYZ_actual_out_opt) = scAtan2dF(+myz(R_out), +myy(R_out));
			vy(anglesXYZ_actual_out_opt) = scAtan2dF(+mzx(R_out), +mxx(R_out));
			vz(anglesXYZ_actual_out_opt) = scAsindF(sz);
		}
	}
	return R_out;
}

CDRAW_INL floatN_t RmatToAnglesYZX3f(float3_t anglesXYZ_actual_out, Rmat3f_t const R)
{
	failassert(anglesXYZ_actual_out && R, NULL);
	vecf_t const sz = -myx(R);
	if (sz >= +scBigEpsL1F)
	{
		vx(anglesXYZ_actual_out) = scAtan2dF(+mxz(R), +mxy(R));
		vy(anglesXYZ_actual_out) = sc0F;
		vz(anglesXYZ_actual_out) = +sc90F;
	}
	else if (sz <= -scBigEpsL1F)
	{
		vx(anglesXYZ_actual_out) = scAtan2dF(-mzy(R), +mzz(R));
		vy(anglesXYZ_actual_out) = sc0F;
		vz(anglesXYZ_actual_out) = -sc90F;
	}
	else
	{
		vx(anglesXYZ_actual_out) = scAtan2dF(+myz(R), +myy(R));
		vy(anglesXYZ_actual_out) = scAtan2dF(+mzx(R), +mxx(R));
		vz(anglesXYZ_actual_out) = scAsindF(sz);
	}
	return anglesXYZ_actual_out;
}

CDRAW_INL floatNx3_t RmatFromAnglesZXY3f(Rmat3f_t R_out, float3_t anglesXYZ_actual_out_opt, float3_t const anglesXYZ)
{
	failassert(R_out && anglesXYZ, NULL);
	vecf_t angle_actual_x = scNormalizeAngle180F(vx(anglesXYZ)), angle_actual_y, sy, cy;
	if (angle_actual_x >= +transform_gimbalangle_minF && angle_actual_x <= +transform_gimbalangle_maxF)
	{
		/*
		* If x=+90 then sx=+1 and cx=0:
		*					| cycz+sysz -cysz+sycz  0 |
		*		RyR+90Rz=	|    0         0       -sx|
		*					|-sycz+cysz  sysz+cycz  0 |
		*
		*					| c(y-z)  s(y-z)  0 |
		*				=	|   0       0    -1 |
		*					|-s(y-z)  c(y-z)  0 |
		*/
		angle_actual_y = scNormalizeAngle180F(vy(anglesXYZ) - vz(anglesXYZ));
		scSinCosdF(&sy, &cy, angle_actual_y);
		vecInit3f(vx(R_out), +cy, sc0F, -sy);
		vecInit3f(vy(R_out), +sy, sc0F, +cy);
		vecInit3f(vz(R_out), sc0F, -sc1F, sc0F);
		if (anglesXYZ_actual_out_opt)
		{
			vx(anglesXYZ_actual_out_opt) = +sc90F;
			vy(anglesXYZ_actual_out_opt) = angle_actual_y;
			vz(anglesXYZ_actual_out_opt) = sc0F;
		}
	}
	else if (angle_actual_x >= -transform_gimbalangle_maxF && angle_actual_x <= -transform_gimbalangle_minF)
	{
		/*
		* If x=-90 then sx=-1 and cx=0:
		*					| cycz-sysz -cysz-sycz  0 |
		*		RyR-90Rz=	|    0          0      +sx|
		*					|-sycz-cysz  sysz-cycz  0 |
		*
		*					| c(y+z) -s(y+z)  0 |
		*				=	|   0       0    +1 |
		*					|-s(y+z) -c(y+z)  0 |
		*/
		angle_actual_y = scNormalizeAngle180F(vy(anglesXYZ) + vz(anglesXYZ));
		scSinCosdF(&sy, &cy, angle_actual_y);
		vecInit3f(vx(R_out), +cy, sc0F, -sy);
		vecInit3f(vy(R_out), -sy, sc0F, -cy);
		vecInit3f(vz(R_out), sc0F, +sc1F, sc0F);
		if (anglesXYZ_actual_out_opt)
		{
			vx(anglesXYZ_actual_out_opt) = -sc90F;
			vy(anglesXYZ_actual_out_opt) = angle_actual_y;
			vz(anglesXYZ_actual_out_opt) = sc0F;
		}
	}
	else
	{
		/*
		*					| cy  0   sy|| 1   0   0 || cz -sz  0 |
		*	R =	RyRxRz	=	| 0   1   0 || 0   cx -sx|| sz  cz  0 |
		*					|-sy  0   cy|| 0   sx  cx|| 0   0   1 |
		*
		*					| cy  0   sy||   cz   -sz  0 |
		*				=	| 0   1   0 || cxsz  cxcz -sx|
		*					|-sy  0   cy|| sxsz  sxcz  cx|
		*
		*					| cycz+sysxsz -cysz+sysxcz  sycx|
		*				=	|        cxsz         cxcz   -sx|
		*					|-sycz+cysxsz  sysz+cysxcz  cycx|
		*/
		vecf_t sx, cx, sz, cz, sxsz, sxcz;
		scSinCosdF(&sx, &cx, vx(anglesXYZ));
		scSinCosdF(&sy, &cy, vy(anglesXYZ));
		scSinCosdF(&sz, &cz, vz(anglesXYZ));
		sxsz = sx * sz;
		sxcz = sx * cz;
		vecInit3f(vx(R_out), (sy * sxsz + cy * cz), +cx * sz, (cy * sxsz - sy * cz));
		vecInit3f(vy(R_out), (sy * sxcz - cy * sz), +cx * cz, (cy * sxcz + sy * sz));
		vecInit3f(vz(R_out), +sy * cx, -sx, +cy * cx);
		if (anglesXYZ_actual_out_opt)
		{
			vx(anglesXYZ_actual_out_opt) = scAsindF(sx);
			vy(anglesXYZ_actual_out_opt) = scAtan2dF(+mzx(R_out), +mzz(R_out));
			vz(anglesXYZ_actual_out_opt) = scAtan2dF(+mxy(R_out), +myy(R_out));
		}
	}
	return R_out;
}

CDRAW_INL floatN_t RmatToAnglesZXY3f(float3_t anglesXYZ_actual_out, Rmat3f_t const R)
{
	failassert(anglesXYZ_actual_out && R, NULL);
	vecf_t const sx = -mzy(R);
	if (sx >= +scBigEpsL1F)
	{
		vx(anglesXYZ_actual_out) = +sc90F;
		vy(anglesXYZ_actual_out) = scAtan2dF(+myx(R), +myz(R));
		vz(anglesXYZ_actual_out) = sc0F;
	}
	else if (sx <= -scBigEpsL1F)
	{
		vx(anglesXYZ_actual_out) = -sc90F;
		vy(anglesXYZ_actual_out) = scAtan2dF(-mxz(R), +mxx(R));
		vz(anglesXYZ_actual_out) = sc0F;
	}
	else
	{
		vx(anglesXYZ_actual_out) = scAsindF(sx);
		vy(anglesXYZ_actual_out) = scAtan2dF(+mzx(R), +mzz(R));
		vz(anglesXYZ_actual_out) = scAtan2dF(+mxy(R), +myy(R));
	}
	return anglesXYZ_actual_out;
}

CDRAW_INL floatNx3_t RmatFromAnglesYXZ3f(Rmat3f_t R_out, float3_t anglesXYZ_actual_out_opt, float3_t const anglesXYZ)
{
	failassert(R_out && anglesXYZ, NULL);
	vecf_t angle_actual_x = scNormalizeAngle180F(vx(anglesXYZ)), angle_actual_z, sz, cz;
	if (angle_actual_x >= +transform_gimbalangle_minF && angle_actual_x <= +transform_gimbalangle_maxF)
	{
		/*
		* If x=+90 then sx=+1 and cx=0:
		*					| czcy-szsy  0   czsy+szcy|
		*		RzR+90Ry=	| szcy+czsy  0   szsy-czcy|
		*					|    0       sx     0     |
		*
		*					| c(z+y)  0  s(z+y)|
		*				=	| s(z+y)  0 -c(z+y)|
		*					|   0    +1    0   |
		*/
		angle_actual_z = scNormalizeAngle180F(vz(anglesXYZ) + vy(anglesXYZ));
		scSinCosdF(&sz, &cz, angle_actual_z);
		vecInit3f(vx(R_out), +cz, +sz, sc0F);
		vecInit3f(vy(R_out), sc0F, sc0F, +sc1F);
		vecInit3f(vz(R_out), +sz, -cz, sc0F);
		if (anglesXYZ_actual_out_opt)
		{
			vx(anglesXYZ_actual_out_opt) = +sc90F;
			vy(anglesXYZ_actual_out_opt) = sc0F;
			vz(anglesXYZ_actual_out_opt) = angle_actual_z;
		}
	}
	else if (angle_actual_x >= -transform_gimbalangle_maxF && angle_actual_x <= -transform_gimbalangle_minF)
	{
		/*
		* If x=-90 then sx=-1 and cx=0:
		*					| czcy+szsy  0   czsy-szcy|
		*		RzR-90Ry=	| szcy-czsy  0   szsy+czcy|
		*					|    0       sx     0     |
		*
		*					| c(z-y)  0 -s(z-y)|
		*				=	| s(z-y)  0  c(z-y)|
		*					|   0    -1    0   |
		*/
		angle_actual_z = scNormalizeAngle180F(vz(anglesXYZ) - vy(anglesXYZ));
		scSinCosdF(&sz, &cz, angle_actual_z);
		vecInit3f(vx(R_out), +cz, +sz, sc0F);
		vecInit3f(vy(R_out), sc0F, sc0F, -sc1F);
		vecInit3f(vz(R_out), -sz, +cz, sc0F);
		if (anglesXYZ_actual_out_opt)
		{
			vx(anglesXYZ_actual_out_opt) = -sc90F;
			vy(anglesXYZ_actual_out_opt) = sc0F;
			vz(anglesXYZ_actual_out_opt) = angle_actual_z;
		}
	}
	else
	{
		/*
		*					| cz -sz  0 || 1   0   0 || cy  0   sy|
		*	R =	RzRxRy	=	| sz  cz  0 || 0   cx -sx|| 0   1   0 |
		*					| 0   0   1 || 0   sx  cx||-sy  0   cy|
		*
		*					| cz -sz  0 ||   cy  0     sy|
		*				=	| sz  cz  0 || sxsy  cx -sxcy|
		*					| 0   0   1 ||-cxsy  sx  cxcy|
		*
		*					| czcy-szsxsy -szcx  czsy+szsxcy|
		*				=	| szcy+czsxsy  czcx  szsy-czsxcy|
		*					|       -cxsy    sx         cxcy|
		*/
		vecf_t sx, cx, sy, cy, sxsy, sxcy;
		scSinCosdF(&sx, &cx, vx(anglesXYZ));
		scSinCosdF(&sy, &cy, vy(anglesXYZ));
		scSinCosdF(&sz, &cz, vz(anglesXYZ));
		sxsy = sx * sy;
		sxcy = sx * cy;
		vecInit3f(vx(R_out), (cz * cy - sz * sxsy), (sz * cy + cz * sxsy), -cx * sy);
		vecInit3f(vy(R_out), -sz * cx, +cz * cx, +sx);
		vecInit3f(vz(R_out), (cz * sy + sz * sxcy), (sz * sy - cz * sxcy), +cx * cy);
		if (anglesXYZ_actual_out_opt)
		{
			vx(anglesXYZ_actual_out_opt) = scAsindF(sx);
			vy(anglesXYZ_actual_out_opt) = scAtan2dF(-mxz(R_out), +mzz(R_out));
			vz(anglesXYZ_actual_out_opt) = scAtan2dF(-myx(R_out), +myy(R_out));
		}
	}
	return R_out;
}

CDRAW_INL floatN_t RmatToAnglesYXZ3f(float3_t anglesXYZ_actual_out, Rmat3f_t const R)
{
	failassert(anglesXYZ_actual_out && R, NULL);
	vecf_t const sx = +myz(R);
	if (sx >= +scBigEpsL1F)
	{
		vx(anglesXYZ_actual_out) = +sc90F;
		vy(anglesXYZ_actual_out) = sc0F;
		vz(anglesXYZ_actual_out) = scAtan2dF(+mxy(R), +mxx(R));
	}
	else if (sx <= -scBigEpsL1F)
	{
		vx(anglesXYZ_actual_out) = -sc90F;
		vy(anglesXYZ_actual_out) = sc0F;
		vz(anglesXYZ_actual_out) = scAtan2dF(+mxy(R), +mxx(R));
	}
	else
	{
		vx(anglesXYZ_actual_out) = scAsindF(sx);
		vy(anglesXYZ_actual_out) = scAtan2dF(-mxz(R), +mzz(R));
		vz(anglesXYZ_actual_out) = scAtan2dF(-myx(R), +myy(R));
	}
	return anglesXYZ_actual_out;
}

CDRAW_INL floatNx3_t RmatFromAnglesXZY3f(Rmat3f_t R_out, float3_t anglesXYZ_actual_out_opt, float3_t const anglesXYZ)
{
	failassert(R_out && anglesXYZ, NULL);
	vecf_t angle_actual_z = scNormalizeAngle180F(vz(anglesXYZ)), angle_actual_y, sy, cy;
	if (angle_actual_z >= +transform_gimbalangle_minF && angle_actual_z <= +transform_gimbalangle_maxF)
	{
		/*
		* If z=+90 then sz=+1 and cz=0:
		*					| 0  -cycx+sysx  cysx+sycx|
		*		RyR+90Rx=	| sz      0          0    |
		*					| 0   sycx+cysx -sysx+cycx|
		*
		*					| 0  -c(y+x)  s(y+x)|
		*				=	|+1     0       0   |
		*					| 0   s(y+x)  c(y+x)|
		*/
		angle_actual_y = scNormalizeAngle180F(vy(anglesXYZ) + vx(anglesXYZ));
		scSinCosdF(&sy, &cy, angle_actual_y);
		vecInit3f(vx(R_out), sc0F, +sc1F, sc0F);
		vecInit3f(vy(R_out), -cy, sc0F, +sy);
		vecInit3f(vz(R_out), +sy, sc0F, +cy);
		if (anglesXYZ_actual_out_opt)
		{
			vx(anglesXYZ_actual_out_opt) = sc0F;
			vy(anglesXYZ_actual_out_opt) = angle_actual_y;
			vz(anglesXYZ_actual_out_opt) = +sc90F;
		}
	}
	else if (angle_actual_z >= -transform_gimbalangle_maxF && angle_actual_z <= -transform_gimbalangle_minF)
	{
		/*
		* If z=-90 then sz=-1 and cz=0:
		*					|  0   cycx+sysx -cysx+sycx|
		*		RyR-90Rx=	|  sz     0          0     |
		*					|  0  -sycx+cysx  sysx+cycx|
		*
		*					|  0   c(y-x)  s(y-x)|
		*				=	| -1     0       0   |
		*					|  0  -s(y-x)  c(y-x)|
		*/
		angle_actual_y = scNormalizeAngle180F(vy(anglesXYZ) - vx(anglesXYZ));
		scSinCosdF(&sy, &cy, angle_actual_y);
		vecInit3f(vx(R_out), sc0F, -sc1F, sc0F);
		vecInit3f(vy(R_out), +cy, sc0F, -sy);
		vecInit3f(vz(R_out), +sy, sc0F, +cy);
		if (anglesXYZ_actual_out_opt)
		{
			vx(anglesXYZ_actual_out_opt) = sc0F;
			vy(anglesXYZ_actual_out_opt) = angle_actual_y;
			vz(anglesXYZ_actual_out_opt) = -sc90F;
		}
	}
	else
	{
		/*
		*					| cy  0   sy|| cz -sz  0 || 1   0   0 |
		*	R =	RyRzRx	=	| 0   1   0 || sz  cz  0 || 0   cx -sx|
		*					|-sy  0   cy|| 0   0   1 || 0   sx  cx|
		*
		*					| cy  0   sy|| cz -szcx  szsx|
		*				=	| 0   1   0 || sz  czcx -czsx|
		*					|-sy  0   cy|| 0     sx    cx|
		*
		*					| cycz -cyszcx+sysx  cyszsx+sycx|
		*				=	|   sz    czcx        -czsx     |
		*					|-sycz  syszcx+cysx -syszsx+cycx|
		*/
		vecf_t sz, cz, sx, cx, szsx, szcx;
		scSinCosdF(&sx, &cx, vx(anglesXYZ));
		scSinCosdF(&sy, &cy, vy(anglesXYZ));
		scSinCosdF(&sz, &cz, vz(anglesXYZ));
		szsx = sz * sx;
		szcx = sz * cx;
		vecInit3f(vx(R_out), +cy * cz, +sz, -sy * cz);
		vecInit3f(vy(R_out), (sy * sx - cy * szcx), +cz * cx, (cy * sx + sy * szcx));
		vecInit3f(vz(R_out), (sy * cx + cy * szsx), -cz * sx, (cy * cx - sy * szsx));
		if (anglesXYZ_actual_out_opt)
		{
			vx(anglesXYZ_actual_out_opt) = scAtan2dF(-mzy(R_out), +myy(R_out));
			vy(anglesXYZ_actual_out_opt) = scAtan2dF(-mxz(R_out), +mxx(R_out));
			vz(anglesXYZ_actual_out_opt) = scAsindF(sz);
		}
	}
	return R_out;
}

CDRAW_INL floatN_t RmatToAnglesXZY3f(float3_t anglesXYZ_actual_out, Rmat3f_t const R)
{
	failassert(anglesXYZ_actual_out && R, NULL);
	vecf_t const sz = +mxy(R);
	if (sz >= +scBigEpsL1F)
	{
		vx(anglesXYZ_actual_out) = sc0F;
		vy(anglesXYZ_actual_out) = scAtan2dF(+mzx(R), +mzz(R));
		vz(anglesXYZ_actual_out) = +sc90F;
	}
	else if (sz <= -scBigEpsL1F)
	{
		vx(anglesXYZ_actual_out) = sc0F;
		vy(anglesXYZ_actual_out) = scAtan2dF(+mzx(R), +mzz(R));
		vz(anglesXYZ_actual_out) = -sc90F;
	}
	else
	{
		vx(anglesXYZ_actual_out) = scAtan2dF(-mzy(R), +myy(R));
		vy(anglesXYZ_actual_out) = scAtan2dF(-mxz(R), +mxx(R));
		vz(anglesXYZ_actual_out) = scAsindF(sz);
	}
	return anglesXYZ_actual_out;
}

CDRAW_INL floatNx3_t RmatFromAnglesZYX3f(Rmat3f_t R_out, float3_t anglesXYZ_actual_out_opt, float3_t const anglesXYZ)
{
	failassert(R_out && anglesXYZ, NULL);
	vecf_t angle_actual_y = scNormalizeAngle180F(vy(anglesXYZ)), angle_actual_x, sx, cx;
	if (angle_actual_y >= +transform_gimbalangle_minF && angle_actual_y <= +transform_gimbalangle_maxF)
	{
		/*
		* If y=+90 then sy=+1 and cy=0:
		*					|    0          0       sy|
		*		RxR+90Rz=	| cxsz+sxcz  cxcz-sxsz  0 |
		*					| sxsz-cxcz  sxcz+cxsz  0 |
		*
		*					|   0       0    +1 |
		*				=	| s(x+z)  c(x+z)  0 |
		*					|-c(x+z)  s(x+z)  0 |
		*/
		angle_actual_x = scNormalizeAngle180F(vx(anglesXYZ) + vz(anglesXYZ));
		scSinCosdF(&sx, &cx, angle_actual_x);
		vecInit3f(vx(R_out), sc0F, +sx, -cx);
		vecInit3f(vy(R_out), sc0F, +cx, +sx);
		vecInit3f(vz(R_out), +sc1F, sc0F, sc0F);
		if (anglesXYZ_actual_out_opt)
		{
			vx(anglesXYZ_actual_out_opt) = angle_actual_x;
			vy(anglesXYZ_actual_out_opt) = +sc90F;
			vz(anglesXYZ_actual_out_opt) = sc0F;
		}
	}
	else if (angle_actual_y >= -transform_gimbalangle_maxF && angle_actual_y <= -transform_gimbalangle_minF)
	{
		/*
		* If y=-90 then sy=-1 and cy=0:
		*					|    0          0       sy|
		*		RxR-90Rz=	| cxsz-sxcz  cxcz+sxsz  0 |
		*					| sxsz+cxcz  sxcz-cxsz  0 |
		*
		*					|   0       0    -1 |
		*				=	|-s(x-z)  c(x-z)  0 |
		*					| c(x-z)  s(x-z)  0 |
		*/
		angle_actual_x = scNormalizeAngle180F(vx(anglesXYZ) - vz(anglesXYZ));
		scSinCosdF(&sx, &cx, angle_actual_x);
		vecInit3f(vx(R_out), sc0F, -sx, +cx);
		vecInit3f(vy(R_out), sc0F, +cx, +sx);
		vecInit3f(vz(R_out), -sc1F, sc0F, sc0F);
		if (anglesXYZ_actual_out_opt)
		{
			vx(anglesXYZ_actual_out_opt) = angle_actual_x;
			vy(anglesXYZ_actual_out_opt) = -sc90F;
			vz(anglesXYZ_actual_out_opt) = sc0F;
		}
	}
	else
	{
		/*
		*					| 1   0   0 || cy  0   sy|| cz -sz  0 |
		*	R =	RxRyRz	=	| 0   cx -sx|| 0   1   0 || sz  cz  0 |
		*					| 0   sx  cx||-sy  0   cy|| 0   0   1 |
		*
		*					| 1   0   0 || cycz -cysz  sy|
		*				=	| 0   cx -sx||   sz    cz  0 |
		*					| 0   sx  cx||-sycz  sysz  cy|
		*
		*					|        cycz        -cysz    sy|
		*				=	| cxsz+sxsycz  cxcz-sxsysz -sxcy|
		*					| sxsz-cxsycz  sxcz+cxsysz  cxcy|
		*/
		vecf_t sy, cy, sz, cz, sysz, sycz;
		scSinCosdF(&sx, &cx, vx(anglesXYZ));
		scSinCosdF(&sy, &cy, vy(anglesXYZ));
		scSinCosdF(&sz, &cz, vz(anglesXYZ));
		sysz = sy * sz;
		sycz = sy * cz;
		vecInit3f(vx(R_out), +cy * cz, (cx * sz + sx * sycz), (sx * sz - cx * sycz));
		vecInit3f(vy(R_out), -cy * sz, (cx * cz - sx * sysz), (sx * cz + cx * sysz));
		vecInit3f(vz(R_out), +sy, -sx * cy, +cx * cy);
		if (anglesXYZ_actual_out_opt)
		{
			vx(anglesXYZ_actual_out_opt) = scAtan2dF(-mzy(R_out), +mzz(R_out));
			vy(anglesXYZ_actual_out_opt) = scAsindF(sy);
			vz(anglesXYZ_actual_out_opt) = scAtan2dF(-myx(R_out), +mxx(R_out));
		}
	}
	return R_out;
}

CDRAW_INL floatN_t RmatToAnglesZYX3f(float3_t anglesXYZ_actual_out, Rmat3f_t const R)
{
	failassert(anglesXYZ_actual_out && R, NULL);
	vecf_t const sy = +mzx(R);
	if (sy >= +scBigEpsL1F)
	{
		vx(anglesXYZ_actual_out) = scAtan2dF(+myz(R), +myy(R));
		vy(anglesXYZ_actual_out) = +sc90F;
		vz(anglesXYZ_actual_out) = sc0F;
	}
	else if (sy <= -scBigEpsL1F)
	{
		vx(anglesXYZ_actual_out) = scAtan2dF(+myz(R), +myy(R));
		vy(anglesXYZ_actual_out) = -sc90F;
		vz(anglesXYZ_actual_out) = sc0F;
	}
	else
	{
		vx(anglesXYZ_actual_out) = scAtan2dF(-mzy(R), +mzz(R));
		vy(anglesXYZ_actual_out) = scAsindF(sy);
		vz(anglesXYZ_actual_out) = scAtan2dF(-myx(R), +mxx(R));
	}
	return anglesXYZ_actual_out;
}

CDRAW_INL floatNx3_t RmatFromAngles3f(Rmat3f_t R_out, float3_t anglesXYZ_actual_out_opt, float3_t const anglesXYZ, RotateAxisOrder_t const encodeOrder)
{
	/*
	* General format of rotations: product of first, second, third, right to left.
	* If the middle rotation angle would result in a sine of +/-1 and cosine of 0,
	* there is a gimbal lock case that must be handled separately.
	*/
	failassert(R_out && anglesXYZ && (encodeOrder < 6), NULL);
	switch (encodeOrder)
	{
	case rot_XYZ: return RmatFromAnglesXYZ3f(R_out, anglesXYZ_actual_out_opt, anglesXYZ);
	case rot_YZX: return RmatFromAnglesYZX3f(R_out, anglesXYZ_actual_out_opt, anglesXYZ);
	case rot_ZXY: return RmatFromAnglesZXY3f(R_out, anglesXYZ_actual_out_opt, anglesXYZ);
	case rot_YXZ: return RmatFromAnglesYXZ3f(R_out, anglesXYZ_actual_out_opt, anglesXYZ);
	case rot_XZY: return RmatFromAnglesXZY3f(R_out, anglesXYZ_actual_out_opt, anglesXYZ);
	case rot_ZYX: return RmatFromAnglesZYX3f(R_out, anglesXYZ_actual_out_opt, anglesXYZ);
	}
	return matIdentity3f(R_out);
}

CDRAW_INL floatN_t RmatToAngles3f(float3_t anglesXYZ_actual_out, Rmat3f_t const R, RotateAxisOrder_t const encodeOrder)
{
	failassert(anglesXYZ_actual_out && R && (encodeOrder < 6), NULL);
	switch (encodeOrder)
	{
	case rot_XYZ: return RmatToAnglesXYZ3f(anglesXYZ_actual_out, R);
	case rot_YZX: return RmatToAnglesYZX3f(anglesXYZ_actual_out, R);
	case rot_ZXY: return RmatToAnglesZXY3f(anglesXYZ_actual_out, R);
	case rot_YXZ: return RmatToAnglesYXZ3f(anglesXYZ_actual_out, R);
	case rot_XZY: return RmatToAnglesXZY3f(anglesXYZ_actual_out, R);
	case rot_ZYX: return RmatToAnglesZYX3f(anglesXYZ_actual_out, R);
	}
	return vecZero3f(anglesXYZ_actual_out);
}

CDRAW_INL floatN_t RmatForward3f(float3_t v_out, Rmat3f_t const R, ReferenceFrame_t const ref)
{
	failassert(v_out && R && cdrawRefValid(ref), NULL);
	if (cdrawRefSgnR(ref))
		return vecNegate3f(v_out, R[cdrawRefIdxR(ref)]);
	return vecCopy3f(v_out, R[cdrawRefIdxR(ref)]);
}

CDRAW_INL floatN_t RmatBackward3f(float3_t v_out, Rmat3f_t const R, ReferenceFrame_t const ref)
{
	failassert(v_out && R && cdrawRefValid(ref), NULL);
	if (cdrawRefSgnR(ref))
		return vecCopy3f(v_out, R[cdrawRefIdxR(ref)]);
	return vecNegate3f(v_out, R[cdrawRefIdxR(ref)]);
}

CDRAW_INL floatN_t RmatRight3f(float3_t v_out, Rmat3f_t const R, ReferenceFrame_t const ref)
{
	failassert(v_out && R && cdrawRefValid(ref), NULL);
	if (cdrawRefSgnP(ref))
		return vecNegate3f(v_out, R[cdrawRefIdxP(ref)]);
	return vecCopy3f(v_out, R[cdrawRefIdxP(ref)]);
}

CDRAW_INL floatN_t RmatLeft3f(float3_t v_out, Rmat3f_t const R, ReferenceFrame_t const ref)
{
	failassert(v_out && R && cdrawRefValid(ref), NULL);
	if (cdrawRefSgnP(ref))
		return vecCopy3f(v_out, R[cdrawRefIdxP(ref)]);
	return vecNegate3f(v_out, R[cdrawRefIdxP(ref)]);
}

CDRAW_INL floatN_t RmatDown3f(float3_t v_out, Rmat3f_t const R, ReferenceFrame_t const ref)
{
	failassert(v_out && R && cdrawRefValid(ref), NULL);
	if (cdrawRefSgnY(ref))
		return vecNegate3f(v_out, R[cdrawRefIdxY(ref)]);
	return vecCopy3f(v_out, R[cdrawRefIdxY(ref)]);
}

CDRAW_INL floatN_t RmatUp3f(float3_t v_out, Rmat3f_t const R, ReferenceFrame_t const ref)
{
	failassert(v_out && R && cdrawRefValid(ref), NULL);
	if (cdrawRefSgnY(ref))
		return vecCopy3f(v_out, R[cdrawRefIdxY(ref)]);
	return vecNegate3f(v_out, R[cdrawRefIdxY(ref)]);
}

CDRAW_INL floatN_t RmatRefForward3f(float3_t v_out, ReferenceFrame_t const ref)
{
	failassert(v_out && cdrawRefValid(ref), NULL);
	return RmatForward3f(v_out, matID3f.m, ref);
}

CDRAW_INL floatN_t RmatRefBackward3f(float3_t v_out, ReferenceFrame_t const ref)
{
	failassert(v_out && cdrawRefValid(ref), NULL);
	return RmatBackward3f(v_out, matID3f.m, ref);
}

CDRAW_INL floatN_t RmatRefRight3f(float3_t v_out, ReferenceFrame_t const ref)
{
	failassert(v_out && cdrawRefValid(ref), NULL);
	return RmatRight3f(v_out, matID3f.m, ref);
}

CDRAW_INL floatN_t RmatRefLeft3f(float3_t v_out, ReferenceFrame_t const ref)
{
	failassert(v_out && cdrawRefValid(ref), NULL);
	return RmatLeft3f(v_out, matID3f.m, ref);
}

CDRAW_INL floatN_t RmatRefUp3f(float3_t v_out, ReferenceFrame_t const ref)
{
	failassert(v_out && cdrawRefValid(ref), NULL);
	return RmatUp3f(v_out, matID3f.m, ref);
}

CDRAW_INL floatN_t RmatRefDown3f(float3_t v_out, ReferenceFrame_t const ref)
{
	failassert(v_out && cdrawRefValid(ref), NULL);
	return RmatDown3f(v_out, matID3f.m, ref);
}

CDRAW_INL floatNx3_t RmatLookAt3f(Rmat3f_t R_out, float3_t const v_center, float3_t const v_target, ReferenceFrame_t const ref)
{
	failassert(R_out && v_center && v_target && cdrawRefValid(ref), NULL);
	float3_t fw, rt, dn;
	vecf_t d, lenSq = vecDispDistSq3f(fw, v_target, v_center);
	if (scIsNonPositiveApproxF(lenSq))
		return RmatID3f(R_out);
	vecMulS3f(fw, fw, gSafeSqrtInvF(lenSq));
	RmatDown3f(dn, matID3f.m, ref);
	d = vecDot3f(fw, dn);
	if (d >= +scBigEpsL1F)
	{
		vecCopy3f(fw, dn);
		RmatBackward3f(dn, matID3f.m, ref);
		RmatRight3f(rt, matID3f.m, ref);
	}
	else if (d <= -scBigEpsL1F)
	{
		vecNegate3f(fw, dn);
		RmatForward3f(dn, matID3f.m, ref);
		RmatRight3f(rt, matID3f.m, ref);
	}
	else
	{
		vecCross3f(rt, dn, fw);
		lenSq = vecLenSq3f(rt);
		vecMulS3f(rt, rt, gSafeSqrtInvF(lenSq));
		vecCross3f(dn, fw, rt);
	}
	if (cdrawRefSgnR(ref))
		vecNegate3f(R_out[cdrawRefIdxR(ref)], fw);
	else
		vecCopy3f(R_out[cdrawRefIdxR(ref)], fw);
	if (cdrawRefSgnP(ref))
		vecNegate3f(R_out[cdrawRefIdxP(ref)], rt);
	else
		vecCopy3f(R_out[cdrawRefIdxP(ref)], rt);
	if (cdrawRefSgnY(ref))
		vecNegate3f(R_out[cdrawRefIdxY(ref)], dn);
	else
		vecCopy3f(R_out[cdrawRefIdxY(ref)], dn);
	return R_out;
}

CDRAW_INL floatN_t vecAbsToRel3f(float3_t v_rel_out, float3_t const v_abs, ReferenceFrame_t const ref)
{
	failassert(v_rel_out && v_abs && cdrawRefValid(ref), NULL);
	v_rel_out[cdrawRefIdxR(ref)] = cdrawRefSgnR(ref) ? -vx(v_abs) : +vx(v_abs);
	v_rel_out[cdrawRefIdxP(ref)] = cdrawRefSgnP(ref) ? -vy(v_abs) : +vy(v_abs);
	v_rel_out[cdrawRefIdxY(ref)] = cdrawRefSgnY(ref) ? -vz(v_abs) : +vz(v_abs);
	return v_rel_out;
}

CDRAW_INL floatN_t vecRelToAbs3f(float3_t v_abs_out, float3_t const v_rel, ReferenceFrame_t const ref)
{
	failassert(v_abs_out && v_rel && cdrawRefValid(ref), NULL);
	vx(v_abs_out) = cdrawRefSgnR(ref) ? -v_rel[cdrawRefIdxR(ref)] : +v_rel[cdrawRefIdxR(ref)];
	vy(v_abs_out) = cdrawRefSgnP(ref) ? -v_rel[cdrawRefIdxP(ref)] : +v_rel[cdrawRefIdxP(ref)];
	vz(v_abs_out) = cdrawRefSgnY(ref) ? -v_rel[cdrawRefIdxY(ref)] : +v_rel[cdrawRefIdxY(ref)];
	return v_abs_out;
}

CDRAW_INL vecf_t vecAbsRoll3f(float3_t const v_rel, ReferenceFrame_t const ref)
{
	failassert(v_rel && cdrawRefValid(ref), sc0F);
	return (cdrawRefSgnR(ref) ? -v_rel[cdrawRefIdxR(ref)] : +v_rel[cdrawRefIdxR(ref)]);
}

CDRAW_INL vecf_t vecAbsPitch3f(float3_t const v_rel, ReferenceFrame_t const ref)
{
	failassert(v_rel && cdrawRefValid(ref), sc0F);
	return (cdrawRefSgnP(ref) ? -v_rel[cdrawRefIdxP(ref)] : +v_rel[cdrawRefIdxP(ref)]);
}

CDRAW_INL vecf_t vecAbsYaw3f(float3_t const v_rel, ReferenceFrame_t const ref)
{
	failassert(v_rel && cdrawRefValid(ref), sc0F);
	return (cdrawRefSgnY(ref) ? -v_rel[cdrawRefIdxY(ref)] : +v_rel[cdrawRefIdxY(ref)]);
}

CDRAW_INL floatN_t vecRelSetAbs3f(float3_t v_rel_out, vecf_t const x_roll, vecf_t const x_pitch, vecf_t const x_yaw, ReferenceFrame_t const ref)
{
	failassert(v_rel_out && cdrawRefValid(ref), NULL);
	v_rel_out[cdrawRefIdxR(ref)] = cdrawRefSgnR(ref) ? -x_roll : +x_roll;
	v_rel_out[cdrawRefIdxP(ref)] = cdrawRefSgnP(ref) ? -x_pitch : +x_pitch;
	v_rel_out[cdrawRefIdxY(ref)] = cdrawRefSgnY(ref) ? -x_yaw : +x_yaw;
	return v_rel_out;
}

CDRAW_INL floatN_t vecRelAddAbs3f(float3_t v_rel_out, float3_t const v_rel, vecf_t const d_roll, vecf_t const d_pitch, vecf_t const d_yaw, ReferenceFrame_t const ref)
{
	failassert(v_rel_out && v_rel && cdrawRefValid(ref), NULL);
	v_rel_out[cdrawRefIdxR(ref)] = v_rel[cdrawRefIdxR(ref)] + (cdrawRefSgnR(ref) ? -d_roll : +d_roll);
	v_rel_out[cdrawRefIdxP(ref)] = v_rel[cdrawRefIdxP(ref)] + (cdrawRefSgnP(ref) ? -d_pitch : +d_pitch);
	v_rel_out[cdrawRefIdxY(ref)] = v_rel[cdrawRefIdxY(ref)] + (cdrawRefSgnY(ref) ? -d_yaw : +d_yaw);
	return v_rel_out;
}

CDRAW_INL floatN_t vecRelScaleAbs3f(float3_t v_rel_out, float3_t const v_rel, vecf_t const s_roll, vecf_t const s_pitch, vecf_t const s_yaw, ReferenceFrame_t const ref)
{
	failassert(v_rel_out && v_rel && cdrawRefValid(ref), NULL);
	v_rel_out[cdrawRefIdxR(ref)] = v_rel[cdrawRefIdxR(ref)] * (cdrawRefSgnR(ref) ? -s_roll : +s_roll);
	v_rel_out[cdrawRefIdxP(ref)] = v_rel[cdrawRefIdxP(ref)] * (cdrawRefSgnP(ref) ? -s_pitch : +s_pitch);
	v_rel_out[cdrawRefIdxY(ref)] = v_rel[cdrawRefIdxY(ref)] * (cdrawRefSgnY(ref) ? -s_yaw : +s_yaw);
	return v_rel_out;
}

CDRAW_INL rotate3f_t* rotateReset3f(rotate3f_t* rotate_out)
{
	failassert(rotate_out, NULL);

	return rotate_out;
}

CDRAW_INL rotate3f_t* rotateInvert3f(rotate3f_t* rotate_out, rotate3f_t const* rotate, ReferenceFrame_t const ref)
{
	failassert(rotate_out && rotate && cdrawRefValid(ref), NULL);

	return rotate_out;
}

CDRAW_INL rotate3f_t* rotateConcat3f(rotate3f_t* rotate_out, rotate3f_t const* rotate_lh, rotate3f_t const* rotate_rh, ReferenceFrame_t const ref)
{
	failassert(rotate_out && rotate_lh && rotate_rh && cdrawRefValid(ref), NULL);

	return rotate_out;
}

CDRAW_INL rotate3f_t* rotateSetAngles3f(rotate3f_t* rotate_out, angle3f_t const* angles, ReferenceFrame_t const ref)
{
	failassert(rotate_out && angles && cdrawRefValid(ref), NULL);

	return rotate_out;
}

CDRAW_INL angle3f_t* rotateGetAngles3f(angle3f_t* angles_out, rotate3f_t const* rotate, ReferenceFrame_t const ref)
{
	failassert(angles_out && rotate && cdrawRefValid(ref), NULL);

	return angles_out;
}

CDRAW_INL rotate3f_t* rotateInvertAngles3f(rotate3f_t* rotate_out, rotate3f_t const* rotate, ReferenceFrame_t const ref)
{
	failassert(rotate_out && rotate && cdrawRefValid(ref), NULL);

	return rotate_out;
}

CDRAW_INL rotate3f_t* rotateConcatAngles3f(rotate3f_t* rotate_out, rotate3f_t const* rotate_lh, rotate3f_t const* rotate_rh, ReferenceFrame_t const ref)
{
	failassert(rotate_out && rotate_lh && rotate_rh && cdrawRefValid(ref), NULL);

	return rotate_out;
}

CDRAW_INL translate3f_t* translateReset3f(translate3f_t* translate_out)
{
	failassert(translate_out, NULL);

	return translate_out;
}

CDRAW_INL translate3f_t* translateSetAxis3f(translate3f_t* translate_out, axis3f_t const* axis, ReferenceFrame_t const ref)
{
	failassert(translate_out && axis && cdrawRefValid(ref), NULL);

	return translate_out;
}

CDRAW_INL axis3f_t* translateGetAxis3f(axis3f_t* axis_out, translate3f_t const* translate, ReferenceFrame_t const ref)
{
	failassert(axis_out && translate && cdrawRefValid(ref), NULL);

	return axis_out;
}

CDRAW_INL translate3f_t* translateInvertAxis3f(translate3f_t* translate_out, translate3f_t const* translate)
{
	failassert(translate_out && translate, NULL);

	return translate_out;
}

CDRAW_INL translate3f_t* translateConcatAxis3f(translate3f_t* translate_out, translate3f_t const* translate_lh, translate3f_t const* translate_rh)
{
	failassert(translate_out && translate_lh && translate_rh, NULL);

	return translate_out;
}

CDRAW_INL transform3f_t* transformReset3f(transform3f_t* transform_out)
{
	failassert(transform_out, NULL);

	return transform_out;
}

CDRAW_INL transform3f_t* transformUpdate3f(transform3f_t* transform_out, ReferenceFrame_t const ref)
{
	failassert(transform_out && cdrawRefValid(ref), NULL);

	return transform_out;
}

CDRAW_INL transform3f_t* transformInvert3f(transform3f_t* transform_out, transform3f_t const* transform, ReferenceFrame_t const ref)
{
	failassert(transform_out && transform && cdrawRefValid(ref), NULL);

	return transform_out;
}

CDRAW_INL transform3f_t* transformConcat3f(transform3f_t* transform_out, transform3f_t const* transform_lh, transform3f_t const* transform_rh, ReferenceFrame_t const ref)
{
	failassert(transform_out && transform_lh && transform_rh && cdrawRefValid(ref), NULL);

	return transform_out;
}

CDRAW_INL transform3f_t* transformUpdateComponents3f(transform3f_t* transform_out, ReferenceFrame_t const ref)
{
	failassert(transform_out && cdrawRefValid(ref), NULL);

	return transform_out;
}

CDRAW_INL transform3f_t* transformInvertComponents3f(transform3f_t* transform_out, transform3f_t const* transform, ReferenceFrame_t const ref)
{
	failassert(transform_out && transform && cdrawRefValid(ref), NULL);

	return transform_out;
}

CDRAW_INL transform3f_t* transformConcatComponents3f(transform3f_t* transform_out, transform3f_t const* transform_lh, transform3f_t const* transform_rh, ReferenceFrame_t const ref)
{
	failassert(transform_out && transform_lh && transform_rh && cdrawRefValid(ref), NULL);

	return transform_out;
}


#endif // #if (!(defined _CDRAW_TRANSFORM3F_INL_) && (defined _CDRAW_TRANSFORM_INL_))