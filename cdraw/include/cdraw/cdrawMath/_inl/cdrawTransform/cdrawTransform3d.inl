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
* cdrawTransform3d.inl
* Definitions of double-precision 3D transform functions.
*/

#if (!(defined _CDRAW_TRANSFORM3D_INL_) && (defined _CDRAW_TRANSFORM_INL_))
#define _CDRAW_TRANSFORM3D_INL_


CDRAW_INL doubleNx3_t RmatID3d(Rmat3d_t R_out)
{
	failassert(R_out, NULL);
	return matIdentity3d(R_out);
}

CDRAW_INL doubleNx3_t RmatInv3d(Rmat3d_t R_out, Rmat3d_t const R)
{
	failassert(R_out && R, NULL);
	return matTranspose3d(R_out, R);
}

CDRAW_INL doubleNx3_t RmatFromAnglesXYZ3d(Rmat3d_t R_out, double3_t anglesXYZ_actual_out_opt, double3_t const anglesXYZ)
{
	failassert(R_out && anglesXYZ, NULL);
	vecd_t angle_actual_y = scNormalizeAngle180D(vy(anglesXYZ)), angle_actual_z, sz, cz;
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
		angle_actual_z = scNormalizeAngle180D(vz(anglesXYZ) - vx(anglesXYZ));
		scSinCosdD(&sz, &cz, angle_actual_z);
		vecInit3d(vx(R_out), sc0D, sc0D, -sc1D);
		vecInit3d(vy(R_out), -sz, +cz, sc0D);
		vecInit3d(vz(R_out), +cz, +sz, sc0D);
		if (anglesXYZ_actual_out_opt)
		{
			vx(anglesXYZ_actual_out_opt) = sc0D;
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
		angle_actual_z = scNormalizeAngle180D(vz(anglesXYZ) + vx(anglesXYZ));
		scSinCosdD(&sz, &cz, angle_actual_z);
		vecInit3d(vx(R_out), sc0D, sc0D, +sc1D);
		vecInit3d(vy(R_out), -sz, +cz, sc0D);
		vecInit3d(vz(R_out), -cz, -sz, sc0D);
		if (anglesXYZ_actual_out_opt)
		{
			vx(anglesXYZ_actual_out_opt) = sc0D;
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
		vecd_t sy, cy, sx, cx, sysx, sycx;
		scSinCosdD(&sx, &cx, vx(anglesXYZ));
		scSinCosdD(&sy, &cy, vy(anglesXYZ));
		scSinCosdD(&sz, &cz, vz(anglesXYZ));
		sysx = sy * sx;
		sycx = sy * cx;
		vecInit3d(vx(R_out), +cz * cy, +sz * cy, -sy);
		vecInit3d(vy(R_out), (cz * sysx - sz * cx), (sz * sysx + cz * cx), +cy * sx);
		vecInit3d(vz(R_out), (cz * sycx + sz * sx), (sz * sycx - cz * sx), +cy * cx);
		if (anglesXYZ_actual_out_opt)
		{
			vx(anglesXYZ_actual_out_opt) = scAtan2dD(+myz(R_out), +mzz(R_out));
			vy(anglesXYZ_actual_out_opt) = scAsindD(sy);
			vz(anglesXYZ_actual_out_opt) = scAtan2dD(+mxy(R_out), +mxx(R_out));
		}
	}
	return R_out;
}

CDRAW_INL doubleN_t RmatToAnglesXYZ3d(double3_t anglesXYZ_actual_out, Rmat3d_t const R)
{
	failassert(anglesXYZ_actual_out && R, NULL);
	vecd_t const sy = -mxz(R);
	if (sy >= +scBigEpsL1F)
	{
		vx(anglesXYZ_actual_out) = sc0D;
		vy(anglesXYZ_actual_out) = +sc90F;
		vz(anglesXYZ_actual_out) = scAtan2dD(+mzy(R), +mzx(R));
	}
	else if (sy <= -scBigEpsL1F)
	{
		vx(anglesXYZ_actual_out) = sc0D;
		vy(anglesXYZ_actual_out) = -sc90F;
		vz(anglesXYZ_actual_out) = scAtan2dD(-myx(R), +myy(R));
	}
	else
	{
		vx(anglesXYZ_actual_out) = scAtan2dD(+myz(R), +mzz(R));
		vy(anglesXYZ_actual_out) = scAsindD(sy);
		vz(anglesXYZ_actual_out) = scAtan2dD(+mxy(R), +mxx(R));
	}
	return anglesXYZ_actual_out;
}

CDRAW_INL doubleNx3_t RmatFromAnglesYZX3d(Rmat3d_t R_out, double3_t anglesXYZ_actual_out_opt, double3_t const anglesXYZ)
{
	failassert(R_out && anglesXYZ, NULL);
	vecd_t angle_actual_z = scNormalizeAngle180D(vz(anglesXYZ)), angle_actual_x, sx, cx;
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
		angle_actual_x = scNormalizeAngle180D(vx(anglesXYZ) - vy(anglesXYZ));
		scSinCosdD(&sx, &cx, angle_actual_x);
		vecInit3d(vx(R_out), sc0D, +cx, +sx);
		vecInit3d(vy(R_out), -sc1D, sc0D, sc0D);
		vecInit3d(vz(R_out), sc0D, -sx, +cx);
		if (anglesXYZ_actual_out_opt)
		{
			vx(anglesXYZ_actual_out_opt) = angle_actual_x;
			vy(anglesXYZ_actual_out_opt) = sc0D;
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
		angle_actual_x = scNormalizeAngle180D(vx(anglesXYZ) + vy(anglesXYZ));
		scSinCosdD(&sx, &cx, angle_actual_x);
		vecInit3d(vx(R_out), sc0D, -cx, -sx);
		vecInit3d(vy(R_out), +sc1D, sc0D, sc0D);
		vecInit3d(vz(R_out), sc0D, -sx, +cx);
		if (anglesXYZ_actual_out_opt)
		{
			vx(anglesXYZ_actual_out_opt) = angle_actual_x;
			vy(anglesXYZ_actual_out_opt) = sc0D;
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
		vecd_t sz, cz, sy, cy, szsy, szcy;
		scSinCosdD(&sx, &cx, vx(anglesXYZ));
		scSinCosdD(&sy, &cy, vy(anglesXYZ));
		scSinCosdD(&sz, &cz, vz(anglesXYZ));
		szsy = sz * sy;
		szcy = sz * cy;
		vecInit3d(vx(R_out), +cz * cy, (cx * szcy + sx * sy), (sx * szcy - cx * sy));
		vecInit3d(vy(R_out), -sz, +cx * cz, +sx * cz);
		vecInit3d(vz(R_out), +cz * sy, (cx * szsy - sx * cy), (sx * szsy + cx * cy));
		if (anglesXYZ_actual_out_opt)
		{
			vx(anglesXYZ_actual_out_opt) = scAtan2dD(+myz(R_out), +myy(R_out));
			vy(anglesXYZ_actual_out_opt) = scAtan2dD(+mzx(R_out), +mxx(R_out));
			vz(anglesXYZ_actual_out_opt) = scAsindD(sz);
		}
	}
	return R_out;
}

CDRAW_INL doubleN_t RmatToAnglesYZX3d(double3_t anglesXYZ_actual_out, Rmat3d_t const R)
{
	failassert(anglesXYZ_actual_out && R, NULL);
	vecd_t const sz = -myx(R);
	if (sz >= +scBigEpsL1F)
	{
		vx(anglesXYZ_actual_out) = scAtan2dD(+mxz(R), +mxy(R));
		vy(anglesXYZ_actual_out) = sc0D;
		vz(anglesXYZ_actual_out) = +sc90F;
	}
	else if (sz <= -scBigEpsL1F)
	{
		vx(anglesXYZ_actual_out) = scAtan2dD(-mzy(R), +mzz(R));
		vy(anglesXYZ_actual_out) = sc0D;
		vz(anglesXYZ_actual_out) = -sc90F;
	}
	else
	{
		vx(anglesXYZ_actual_out) = scAtan2dD(+myz(R), +myy(R));
		vy(anglesXYZ_actual_out) = scAtan2dD(+mzx(R), +mxx(R));
		vz(anglesXYZ_actual_out) = scAsindD(sz);
	}
	return anglesXYZ_actual_out;
}

CDRAW_INL doubleNx3_t RmatFromAnglesZXY3d(Rmat3d_t R_out, double3_t anglesXYZ_actual_out_opt, double3_t const anglesXYZ)
{
	failassert(R_out && anglesXYZ, NULL);
	vecd_t angle_actual_x = scNormalizeAngle180D(vx(anglesXYZ)), angle_actual_y, sy, cy;
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
		angle_actual_y = scNormalizeAngle180D(vy(anglesXYZ) - vz(anglesXYZ));
		scSinCosdD(&sy, &cy, angle_actual_y);
		vecInit3d(vx(R_out), +cy, sc0D, -sy);
		vecInit3d(vy(R_out), +sy, sc0D, +cy);
		vecInit3d(vz(R_out), sc0D, -sc1D, sc0D);
		if (anglesXYZ_actual_out_opt)
		{
			vx(anglesXYZ_actual_out_opt) = +sc90F;
			vy(anglesXYZ_actual_out_opt) = angle_actual_y;
			vz(anglesXYZ_actual_out_opt) = sc0D;
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
		angle_actual_y = scNormalizeAngle180D(vy(anglesXYZ) + vz(anglesXYZ));
		scSinCosdD(&sy, &cy, angle_actual_y);
		vecInit3d(vx(R_out), +cy, sc0D, -sy);
		vecInit3d(vy(R_out), -sy, sc0D, -cy);
		vecInit3d(vz(R_out), sc0D, +sc1D, sc0D);
		if (anglesXYZ_actual_out_opt)
		{
			vx(anglesXYZ_actual_out_opt) = -sc90F;
			vy(anglesXYZ_actual_out_opt) = angle_actual_y;
			vz(anglesXYZ_actual_out_opt) = sc0D;
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
		vecd_t sx, cx, sz, cz, sxsz, sxcz;
		scSinCosdD(&sx, &cx, vx(anglesXYZ));
		scSinCosdD(&sy, &cy, vy(anglesXYZ));
		scSinCosdD(&sz, &cz, vz(anglesXYZ));
		sxsz = sx * sz;
		sxcz = sx * cz;
		vecInit3d(vx(R_out), (sy * sxsz + cy * cz), +cx * sz, (cy * sxsz - sy * cz));
		vecInit3d(vy(R_out), (sy * sxcz - cy * sz), +cx * cz, (cy * sxcz + sy * sz));
		vecInit3d(vz(R_out), +sy * cx, -sx, +cy * cx);
		if (anglesXYZ_actual_out_opt)
		{
			vx(anglesXYZ_actual_out_opt) = scAsindD(sx);
			vy(anglesXYZ_actual_out_opt) = scAtan2dD(+mzx(R_out), +mzz(R_out));
			vz(anglesXYZ_actual_out_opt) = scAtan2dD(+mxy(R_out), +myy(R_out));
		}
	}
	return R_out;
}

CDRAW_INL doubleN_t RmatToAnglesZXY3d(double3_t anglesXYZ_actual_out, Rmat3d_t const R)
{
	failassert(anglesXYZ_actual_out && R, NULL);
	vecd_t const sx = -mzy(R);
	if (sx >= +scBigEpsL1F)
	{
		vx(anglesXYZ_actual_out) = +sc90F;
		vy(anglesXYZ_actual_out) = scAtan2dD(+myx(R), +myz(R));
		vz(anglesXYZ_actual_out) = sc0D;
	}
	else if (sx <= -scBigEpsL1F)
	{
		vx(anglesXYZ_actual_out) = -sc90F;
		vy(anglesXYZ_actual_out) = scAtan2dD(-mxz(R), +mxx(R));
		vz(anglesXYZ_actual_out) = sc0D;
	}
	else
	{
		vx(anglesXYZ_actual_out) = scAsindD(sx);
		vy(anglesXYZ_actual_out) = scAtan2dD(+mzx(R), +mzz(R));
		vz(anglesXYZ_actual_out) = scAtan2dD(+mxy(R), +myy(R));
	}
	return anglesXYZ_actual_out;
}

CDRAW_INL doubleNx3_t RmatFromAnglesYXZ3d(Rmat3d_t R_out, double3_t anglesXYZ_actual_out_opt, double3_t const anglesXYZ)
{
	failassert(R_out && anglesXYZ, NULL);
	vecd_t angle_actual_x = scNormalizeAngle180D(vx(anglesXYZ)), angle_actual_z, sz, cz;
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
		angle_actual_z = scNormalizeAngle180D(vz(anglesXYZ) + vy(anglesXYZ));
		scSinCosdD(&sz, &cz, angle_actual_z);
		vecInit3d(vx(R_out), +cz, +sz, sc0D);
		vecInit3d(vy(R_out), sc0D, sc0D, +sc1D);
		vecInit3d(vz(R_out), +sz, -cz, sc0D);
		if (anglesXYZ_actual_out_opt)
		{
			vx(anglesXYZ_actual_out_opt) = +sc90F;
			vy(anglesXYZ_actual_out_opt) = sc0D;
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
		angle_actual_z = scNormalizeAngle180D(vz(anglesXYZ) - vy(anglesXYZ));
		scSinCosdD(&sz, &cz, angle_actual_z);
		vecInit3d(vx(R_out), +cz, +sz, sc0D);
		vecInit3d(vy(R_out), sc0D, sc0D, -sc1D);
		vecInit3d(vz(R_out), -sz, +cz, sc0D);
		if (anglesXYZ_actual_out_opt)
		{
			vx(anglesXYZ_actual_out_opt) = -sc90F;
			vy(anglesXYZ_actual_out_opt) = sc0D;
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
		vecd_t sx, cx, sy, cy, sxsy, sxcy;
		scSinCosdD(&sx, &cx, vx(anglesXYZ));
		scSinCosdD(&sy, &cy, vy(anglesXYZ));
		scSinCosdD(&sz, &cz, vz(anglesXYZ));
		sxsy = sx * sy;
		sxcy = sx * cy;
		vecInit3d(vx(R_out), (cz * cy - sz * sxsy), (sz * cy + cz * sxsy), -cx * sy);
		vecInit3d(vy(R_out), -sz * cx, +cz * cx, +sx);
		vecInit3d(vz(R_out), (cz * sy + sz * sxcy), (sz * sy - cz * sxcy), +cx * cy);
		if (anglesXYZ_actual_out_opt)
		{
			vx(anglesXYZ_actual_out_opt) = scAsindD(sx);
			vy(anglesXYZ_actual_out_opt) = scAtan2dD(-mxz(R_out), +mzz(R_out));
			vz(anglesXYZ_actual_out_opt) = scAtan2dD(-myx(R_out), +myy(R_out));
		}
	}
	return R_out;
}

CDRAW_INL doubleN_t RmatToAnglesYXZ3d(double3_t anglesXYZ_actual_out, Rmat3d_t const R)
{
	failassert(anglesXYZ_actual_out && R, NULL);
	vecd_t const sx = +myz(R);
	if (sx >= +scBigEpsL1F)
	{
		vx(anglesXYZ_actual_out) = +sc90F;
		vy(anglesXYZ_actual_out) = sc0D;
		vz(anglesXYZ_actual_out) = scAtan2dD(+mxy(R), +mxx(R));
	}
	else if (sx <= -scBigEpsL1F)
	{
		vx(anglesXYZ_actual_out) = -sc90F;
		vy(anglesXYZ_actual_out) = sc0D;
		vz(anglesXYZ_actual_out) = scAtan2dD(+mxy(R), +mxx(R));
	}
	else
	{
		vx(anglesXYZ_actual_out) = scAsindD(sx);
		vy(anglesXYZ_actual_out) = scAtan2dD(-mxz(R), +mzz(R));
		vz(anglesXYZ_actual_out) = scAtan2dD(-myx(R), +myy(R));
	}
	return anglesXYZ_actual_out;
}

CDRAW_INL doubleNx3_t RmatFromAnglesXZY3d(Rmat3d_t R_out, double3_t anglesXYZ_actual_out_opt, double3_t const anglesXYZ)
{
	failassert(R_out && anglesXYZ, NULL);
	vecd_t angle_actual_z = scNormalizeAngle180D(vz(anglesXYZ)), angle_actual_y, sy, cy;
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
		angle_actual_y = scNormalizeAngle180D(vy(anglesXYZ) + vx(anglesXYZ));
		scSinCosdD(&sy, &cy, angle_actual_y);
		vecInit3d(vx(R_out), sc0D, +sc1D, sc0D);
		vecInit3d(vy(R_out), -cy, sc0D, +sy);
		vecInit3d(vz(R_out), +sy, sc0D, +cy);
		if (anglesXYZ_actual_out_opt)
		{
			vx(anglesXYZ_actual_out_opt) = sc0D;
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
		angle_actual_y = scNormalizeAngle180D(vy(anglesXYZ) - vx(anglesXYZ));
		scSinCosdD(&sy, &cy, angle_actual_y);
		vecInit3d(vx(R_out), sc0D, -sc1D, sc0D);
		vecInit3d(vy(R_out), +cy, sc0D, -sy);
		vecInit3d(vz(R_out), +sy, sc0D, +cy);
		if (anglesXYZ_actual_out_opt)
		{
			vx(anglesXYZ_actual_out_opt) = sc0D;
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
		vecd_t sz, cz, sx, cx, szsx, szcx;
		scSinCosdD(&sx, &cx, vx(anglesXYZ));
		scSinCosdD(&sy, &cy, vy(anglesXYZ));
		scSinCosdD(&sz, &cz, vz(anglesXYZ));
		szsx = sz * sx;
		szcx = sz * cx;
		vecInit3d(vx(R_out), +cy * cz, +sz, -sy * cz);
		vecInit3d(vy(R_out), (sy * sx - cy * szcx), +cz * cx, (cy * sx + sy * szcx));
		vecInit3d(vz(R_out), (sy * cx + cy * szsx), -cz * sx, (cy * cx - sy * szsx));
		if (anglesXYZ_actual_out_opt)
		{
			vx(anglesXYZ_actual_out_opt) = scAtan2dD(-mzy(R_out), +myy(R_out));
			vy(anglesXYZ_actual_out_opt) = scAtan2dD(-mxz(R_out), +mxx(R_out));
			vz(anglesXYZ_actual_out_opt) = scAsindD(sz);
		}
	}
	return R_out;
}

CDRAW_INL doubleN_t RmatToAnglesXZY3d(double3_t anglesXYZ_actual_out, Rmat3d_t const R)
{
	failassert(anglesXYZ_actual_out && R, NULL);
	vecd_t const sz = +mxy(R);
	if (sz >= +scBigEpsL1F)
	{
		vx(anglesXYZ_actual_out) = sc0D;
		vy(anglesXYZ_actual_out) = scAtan2dD(+mzx(R), +mzz(R));
		vz(anglesXYZ_actual_out) = +sc90F;
	}
	else if (sz <= -scBigEpsL1F)
	{
		vx(anglesXYZ_actual_out) = sc0D;
		vy(anglesXYZ_actual_out) = scAtan2dD(+mzx(R), +mzz(R));
		vz(anglesXYZ_actual_out) = -sc90F;
	}
	else
	{
		vx(anglesXYZ_actual_out) = scAtan2dD(-mzy(R), +myy(R));
		vy(anglesXYZ_actual_out) = scAtan2dD(-mxz(R), +mxx(R));
		vz(anglesXYZ_actual_out) = scAsindD(sz);
	}
	return anglesXYZ_actual_out;
}

CDRAW_INL doubleNx3_t RmatFromAnglesZYX3d(Rmat3d_t R_out, double3_t anglesXYZ_actual_out_opt, double3_t const anglesXYZ)
{
	failassert(R_out && anglesXYZ, NULL);
	vecd_t angle_actual_y = scNormalizeAngle180D(vy(anglesXYZ)), angle_actual_x, sx, cx;
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
		angle_actual_x = scNormalizeAngle180D(vx(anglesXYZ) + vz(anglesXYZ));
		scSinCosdD(&sx, &cx, angle_actual_x);
		vecInit3d(vx(R_out), sc0D, +sx, -cx);
		vecInit3d(vy(R_out), sc0D, +cx, +sx);
		vecInit3d(vz(R_out), +sc1D, sc0D, sc0D);
		if (anglesXYZ_actual_out_opt)
		{
			vx(anglesXYZ_actual_out_opt) = angle_actual_x;
			vy(anglesXYZ_actual_out_opt) = +sc90F;
			vz(anglesXYZ_actual_out_opt) = sc0D;
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
		angle_actual_x = scNormalizeAngle180D(vx(anglesXYZ) - vz(anglesXYZ));
		scSinCosdD(&sx, &cx, angle_actual_x);
		vecInit3d(vx(R_out), sc0D, -sx, +cx);
		vecInit3d(vy(R_out), sc0D, +cx, +sx);
		vecInit3d(vz(R_out), -sc1D, sc0D, sc0D);
		if (anglesXYZ_actual_out_opt)
		{
			vx(anglesXYZ_actual_out_opt) = angle_actual_x;
			vy(anglesXYZ_actual_out_opt) = -sc90F;
			vz(anglesXYZ_actual_out_opt) = sc0D;
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
		vecd_t sy, cy, sz, cz, sysz, sycz;
		scSinCosdD(&sx, &cx, vx(anglesXYZ));
		scSinCosdD(&sy, &cy, vy(anglesXYZ));
		scSinCosdD(&sz, &cz, vz(anglesXYZ));
		sysz = sy * sz;
		sycz = sy * cz;
		vecInit3d(vx(R_out), +cy * cz, (cx * sz + sx * sycz), (sx * sz - cx * sycz));
		vecInit3d(vy(R_out), -cy * sz, (cx * cz - sx * sysz), (sx * cz + cx * sysz));
		vecInit3d(vz(R_out), +sy, -sx * cy, +cx * cy);
		if (anglesXYZ_actual_out_opt)
		{
			vx(anglesXYZ_actual_out_opt) = scAtan2dD(-mzy(R_out), +mzz(R_out));
			vy(anglesXYZ_actual_out_opt) = scAsindD(sy);
			vz(anglesXYZ_actual_out_opt) = scAtan2dD(-myx(R_out), +mxx(R_out));
		}
	}
	return R_out;
}

CDRAW_INL doubleN_t RmatToAnglesZYX3d(double3_t anglesXYZ_actual_out, Rmat3d_t const R)
{
	failassert(anglesXYZ_actual_out && R, NULL);
	vecd_t const sy = +mzx(R);
	if (sy >= +scBigEpsL1F)
	{
		vx(anglesXYZ_actual_out) = scAtan2dD(+myz(R), +myy(R));
		vy(anglesXYZ_actual_out) = +sc90F;
		vz(anglesXYZ_actual_out) = sc0D;
	}
	else if (sy <= -scBigEpsL1F)
	{
		vx(anglesXYZ_actual_out) = scAtan2dD(+myz(R), +myy(R));
		vy(anglesXYZ_actual_out) = -sc90F;
		vz(anglesXYZ_actual_out) = sc0D;
	}
	else
	{
		vx(anglesXYZ_actual_out) = scAtan2dD(-mzy(R), +mzz(R));
		vy(anglesXYZ_actual_out) = scAsindD(sy);
		vz(anglesXYZ_actual_out) = scAtan2dD(-myx(R), +mxx(R));
	}
	return anglesXYZ_actual_out;
}

CDRAW_INL doubleNx3_t RmatFromAngles3d(Rmat3d_t R_out, double3_t anglesXYZ_actual_out_opt, double3_t const anglesXYZ, RotateAxisOrder_t const encodeOrder)
{
	/*
	* General format of rotations: product of first, second, third, right to left.
	* If the middle rotation angle would result in a sine of +/-1 and cosine of 0,
	* there is a gimbal lock case that must be handled separately.
	*/
	failassert(R_out && anglesXYZ && (encodeOrder < 6), NULL);
	switch (encodeOrder)
	{
	case rot_XYZ: return RmatFromAnglesXYZ3d(R_out, anglesXYZ_actual_out_opt, anglesXYZ);
	case rot_YZX: return RmatFromAnglesYZX3d(R_out, anglesXYZ_actual_out_opt, anglesXYZ);
	case rot_ZXY: return RmatFromAnglesZXY3d(R_out, anglesXYZ_actual_out_opt, anglesXYZ);
	case rot_YXZ: return RmatFromAnglesYXZ3d(R_out, anglesXYZ_actual_out_opt, anglesXYZ);
	case rot_XZY: return RmatFromAnglesXZY3d(R_out, anglesXYZ_actual_out_opt, anglesXYZ);
	case rot_ZYX: return RmatFromAnglesZYX3d(R_out, anglesXYZ_actual_out_opt, anglesXYZ);
	}
	return matIdentity3d(R_out);
}

CDRAW_INL doubleN_t RmatToAngles3d(double3_t anglesXYZ_actual_out, Rmat3d_t const R, RotateAxisOrder_t const encodeOrder)
{
	failassert(anglesXYZ_actual_out && R && (encodeOrder < 6), NULL);
	switch (encodeOrder)
	{
	case rot_XYZ: return RmatToAnglesXYZ3d(anglesXYZ_actual_out, R);
	case rot_YZX: return RmatToAnglesYZX3d(anglesXYZ_actual_out, R);
	case rot_ZXY: return RmatToAnglesZXY3d(anglesXYZ_actual_out, R);
	case rot_YXZ: return RmatToAnglesYXZ3d(anglesXYZ_actual_out, R);
	case rot_XZY: return RmatToAnglesXZY3d(anglesXYZ_actual_out, R);
	case rot_ZYX: return RmatToAnglesZYX3d(anglesXYZ_actual_out, R);
	}
	return vecZero3d(anglesXYZ_actual_out);
}

CDRAW_INL doubleN_t RmatForward3d(double3_t v_out, Rmat3d_t const R, ReferenceFrame_t const ref)
{
	failassert(v_out && R && cdrawRefValid(ref), NULL);
	if (cdrawRefSgnR(ref))
		return vecNegate3d(v_out, R[cdrawRefIdxR(ref)]);
	return vecCopy3d(v_out, R[cdrawRefIdxR(ref)]);
}

CDRAW_INL doubleN_t RmatBackward3d(double3_t v_out, Rmat3d_t const R, ReferenceFrame_t const ref)
{
	failassert(v_out && R && cdrawRefValid(ref), NULL);
	if (cdrawRefSgnR(ref))
		return vecCopy3d(v_out, R[cdrawRefIdxR(ref)]);
	return vecNegate3d(v_out, R[cdrawRefIdxR(ref)]);
}

CDRAW_INL doubleN_t RmatRight3d(double3_t v_out, Rmat3d_t const R, ReferenceFrame_t const ref)
{
	failassert(v_out && R && cdrawRefValid(ref), NULL);
	if (cdrawRefSgnP(ref))
		return vecNegate3d(v_out, R[cdrawRefIdxP(ref)]);
	return vecCopy3d(v_out, R[cdrawRefIdxP(ref)]);
}

CDRAW_INL doubleN_t RmatLeft3d(double3_t v_out, Rmat3d_t const R, ReferenceFrame_t const ref)
{
	failassert(v_out && R && cdrawRefValid(ref), NULL);
	if (cdrawRefSgnP(ref))
		return vecCopy3d(v_out, R[cdrawRefIdxP(ref)]);
	return vecNegate3d(v_out, R[cdrawRefIdxP(ref)]);
}

CDRAW_INL doubleN_t RmatDown3d(double3_t v_out, Rmat3d_t const R, ReferenceFrame_t const ref)
{
	failassert(v_out && R && cdrawRefValid(ref), NULL);
	if (cdrawRefSgnY(ref))
		return vecNegate3d(v_out, R[cdrawRefIdxY(ref)]);
	return vecCopy3d(v_out, R[cdrawRefIdxY(ref)]);
}

CDRAW_INL doubleN_t RmatUp3d(double3_t v_out, Rmat3d_t const R, ReferenceFrame_t const ref)
{
	failassert(v_out && R && cdrawRefValid(ref), NULL);
	if (cdrawRefSgnY(ref))
		return vecCopy3d(v_out, R[cdrawRefIdxY(ref)]);
	return vecNegate3d(v_out, R[cdrawRefIdxY(ref)]);
}

CDRAW_INL doubleN_t RmatRefForward3d(double3_t v_out, ReferenceFrame_t const ref)
{
	failassert(v_out && cdrawRefValid(ref), NULL);
	return RmatForward3d(v_out, matID3d.m, ref);
}

CDRAW_INL doubleN_t RmatRefBackward3d(double3_t v_out, ReferenceFrame_t const ref)
{
	failassert(v_out && cdrawRefValid(ref), NULL);
	return RmatBackward3d(v_out, matID3d.m, ref);
}

CDRAW_INL doubleN_t RmatRefRight3d(double3_t v_out, ReferenceFrame_t const ref)
{
	failassert(v_out && cdrawRefValid(ref), NULL);
	return RmatRight3d(v_out, matID3d.m, ref);
}

CDRAW_INL doubleN_t RmatRefLeft3d(double3_t v_out, ReferenceFrame_t const ref)
{
	failassert(v_out && cdrawRefValid(ref), NULL);
	return RmatLeft3d(v_out, matID3d.m, ref);
}

CDRAW_INL doubleN_t RmatRefUp3d(double3_t v_out, ReferenceFrame_t const ref)
{
	failassert(v_out && cdrawRefValid(ref), NULL);
	return RmatUp3d(v_out, matID3d.m, ref);
}

CDRAW_INL doubleN_t RmatRefDown3d(double3_t v_out, ReferenceFrame_t const ref)
{
	failassert(v_out && cdrawRefValid(ref), NULL);
	return RmatDown3d(v_out, matID3d.m, ref);
}

CDRAW_INL doubleNx3_t RmatLookAt3d(Rmat3d_t R_out, double3_t const v_center, double3_t const v_target, ReferenceFrame_t const ref)
{
	failassert(R_out && v_center && v_target && cdrawRefValid(ref), NULL);
	double3_t fw, rt, dn;
	vecd_t d, lenSq = vecDispDistSq3d(fw, v_target, v_center);
	if (scIsNonPositiveApproxD(lenSq))
		return RmatID3d(R_out);
	vecMulS3d(fw, fw, gSafeSqrtInvD(lenSq));
	RmatDown3d(dn, matID3d.m, ref);
	d = vecDot3d(fw, dn);
	if (d >= +scBigEpsL1F)
	{
		vecCopy3d(fw, dn);
		RmatBackward3d(dn, matID3d.m, ref);
		RmatRight3d(rt, matID3d.m, ref);
	}
	else if (d <= -scBigEpsL1F)
	{
		vecNegate3d(fw, dn);
		RmatForward3d(dn, matID3d.m, ref);
		RmatRight3d(rt, matID3d.m, ref);
	}
	else
	{
		vecCross3d(rt, dn, fw);
		lenSq = vecLenSq3d(rt);
		vecMulS3d(rt, rt, gSafeSqrtInvD(lenSq));
		vecCross3d(dn, fw, rt);
	}
	if (cdrawRefSgnR(ref))
		vecNegate3d(R_out[cdrawRefIdxR(ref)], fw);
	else
		vecCopy3d(R_out[cdrawRefIdxR(ref)], fw);
	if (cdrawRefSgnP(ref))
		vecNegate3d(R_out[cdrawRefIdxP(ref)], rt);
	else
		vecCopy3d(R_out[cdrawRefIdxP(ref)], rt);
	if (cdrawRefSgnY(ref))
		vecNegate3d(R_out[cdrawRefIdxY(ref)], dn);
	else
		vecCopy3d(R_out[cdrawRefIdxY(ref)], dn);
	return R_out;
}

CDRAW_INL doubleN_t vecAbsToRel3d(double3_t v_rel_out, double3_t const v_abs, ReferenceFrame_t const ref)
{
	failassert(v_rel_out && v_abs && cdrawRefValid(ref), NULL);
	v_rel_out[cdrawRefIdxR(ref)] = cdrawRefSgnR(ref) ? -vx(v_abs) : +vx(v_abs);
	v_rel_out[cdrawRefIdxP(ref)] = cdrawRefSgnP(ref) ? -vy(v_abs) : +vy(v_abs);
	v_rel_out[cdrawRefIdxY(ref)] = cdrawRefSgnY(ref) ? -vz(v_abs) : +vz(v_abs);
	return v_rel_out;
}

CDRAW_INL doubleN_t vecRelToAbs3d(double3_t v_abs_out, double3_t const v_rel, ReferenceFrame_t const ref)
{
	failassert(v_abs_out && v_rel && cdrawRefValid(ref), NULL);
	vx(v_abs_out) = cdrawRefSgnR(ref) ? -v_rel[cdrawRefIdxR(ref)] : +v_rel[cdrawRefIdxR(ref)];
	vy(v_abs_out) = cdrawRefSgnP(ref) ? -v_rel[cdrawRefIdxP(ref)] : +v_rel[cdrawRefIdxP(ref)];
	vz(v_abs_out) = cdrawRefSgnY(ref) ? -v_rel[cdrawRefIdxY(ref)] : +v_rel[cdrawRefIdxY(ref)];
	return v_abs_out;
}

CDRAW_INL vecd_t vecAbsRoll3d(double3_t const v_rel, ReferenceFrame_t const ref)
{
	failassert(v_rel && cdrawRefValid(ref), sc0D);
	return (cdrawRefSgnR(ref) ? -v_rel[cdrawRefIdxR(ref)] : +v_rel[cdrawRefIdxR(ref)]);
}

CDRAW_INL vecd_t vecAbsPitch3d(double3_t const v_rel, ReferenceFrame_t const ref)
{
	failassert(v_rel && cdrawRefValid(ref), sc0D);
	return (cdrawRefSgnP(ref) ? -v_rel[cdrawRefIdxP(ref)] : +v_rel[cdrawRefIdxP(ref)]);
}

CDRAW_INL vecd_t vecAbsYaw3d(double3_t const v_rel, ReferenceFrame_t const ref)
{
	failassert(v_rel && cdrawRefValid(ref), sc0D);
	return (cdrawRefSgnY(ref) ? -v_rel[cdrawRefIdxY(ref)] : +v_rel[cdrawRefIdxY(ref)]);
}

CDRAW_INL doubleN_t vecRelSetAbs3d(double3_t v_rel_out, vecd_t const x_roll, vecd_t const x_pitch, vecd_t const x_yaw, ReferenceFrame_t const ref)
{
	failassert(v_rel_out && cdrawRefValid(ref), NULL);
	v_rel_out[cdrawRefIdxR(ref)] = cdrawRefSgnR(ref) ? -x_roll : +x_roll;
	v_rel_out[cdrawRefIdxP(ref)] = cdrawRefSgnP(ref) ? -x_pitch : +x_pitch;
	v_rel_out[cdrawRefIdxY(ref)] = cdrawRefSgnY(ref) ? -x_yaw : +x_yaw;
	return v_rel_out;
}

CDRAW_INL doubleN_t vecRelAddAbs3d(double3_t v_rel_out, double3_t const v_rel, vecd_t const d_roll, vecd_t const d_pitch, vecd_t const d_yaw, ReferenceFrame_t const ref)
{
	failassert(v_rel_out && v_rel && cdrawRefValid(ref), NULL);
	v_rel_out[cdrawRefIdxR(ref)] = v_rel[cdrawRefIdxR(ref)] + (cdrawRefSgnR(ref) ? -d_roll : +d_roll);
	v_rel_out[cdrawRefIdxP(ref)] = v_rel[cdrawRefIdxP(ref)] + (cdrawRefSgnP(ref) ? -d_pitch : +d_pitch);
	v_rel_out[cdrawRefIdxY(ref)] = v_rel[cdrawRefIdxY(ref)] + (cdrawRefSgnY(ref) ? -d_yaw : +d_yaw);
	return v_rel_out;
}

CDRAW_INL doubleN_t vecRelScaleAbs3d(double3_t v_rel_out, double3_t const v_rel, vecd_t const s_roll, vecd_t const s_pitch, vecd_t const s_yaw, ReferenceFrame_t const ref)
{
	failassert(v_rel_out && v_rel && cdrawRefValid(ref), NULL);
	v_rel_out[cdrawRefIdxR(ref)] = v_rel[cdrawRefIdxR(ref)] * (cdrawRefSgnR(ref) ? -s_roll : +s_roll);
	v_rel_out[cdrawRefIdxP(ref)] = v_rel[cdrawRefIdxP(ref)] * (cdrawRefSgnP(ref) ? -s_pitch : +s_pitch);
	v_rel_out[cdrawRefIdxY(ref)] = v_rel[cdrawRefIdxY(ref)] * (cdrawRefSgnY(ref) ? -s_yaw : +s_yaw);
	return v_rel_out;
}

CDRAW_INL doubleN_t TmatMulVec3d(double3_t v_out, Tmat3d_t const T_lh, double3_t const v_rh)
{
	failassert(v_out && T_lh && v_rh, NULL);
	failassert((v_out != v_rh) && (v_out != vx(T_lh)) && (v_out != vy(T_lh)) && (v_out != vz(T_lh)), NULL);
	return matMulVec3d(v_out, T_lh, v_rh);
}

CDRAW_INL doubleN_t TmatMulPoint3d(double3_t p_out, Tmat3d_t const T_lh, double3_t const p_rh)
{
	failassert(p_out && T_lh && p_rh, NULL);
	failassert((p_out != p_rh) && (p_out != vx(T_lh)) && (p_out != vy(T_lh)) && (p_out != vz(T_lh)) && (p_out != vw(T_lh)), NULL);
	return vecAdd3d(p_out, matMulVec3d(p_out, T_lh, p_rh), vw(T_lh));
}

CDRAW_INL doubleNx3_t TmatMul3d(Tmat3d_t T_out, Tmat3d_t const T_lh, Tmat3d_t const T_rh)
{
	failassert(T_out && T_lh && T_rh, NULL);
	// v' = t' + M'(t + Mv) = t' + M't + M'Mv
	vecAdd3d(vw(T_out), matMulVec3d(vw(T_out), T_lh, vw(T_rh)), vw(T_lh));
	return matMul3d(T_out, T_lh, T_rh);
}

CDRAW_INL rotate3d_t* rotateReset3d(rotate3d_t* rotate_out)
{
	failassert(rotate_out, NULL);
	RmatID3d(rotate_out->R);
	vecZero3d(rotate_out->angles);
	return rotate_out;
}

CDRAW_INL rotate3d_t* rotateUpdate3d(rotate3d_t* rotate_out, ReferenceFrame_t const ref)
{
	failassert(rotate_out && cdrawRefValid(ref), NULL);
	RmatFromAngles3d(rotate_out->R, rotate_out->angles, rotate_out->angles, cdrawRefRotateAxis(ref));
	return rotate_out;
}

CDRAW_INL rotate3d_t* rotateInvert3d(rotate3d_t* rotate_out, rotate3d_t const* rotate, ReferenceFrame_t const ref)
{
	failassert(rotate_out && rotate && cdrawRefValid(ref), NULL);
	RmatInv3d(rotate_out->R, rotate->R);
	RmatToAngles3d(rotate_out->angles, rotate_out->R, cdrawRefRotateAxis(ref));
	return rotate_out;
}

CDRAW_INL rotate3d_t* rotateConcat3d(rotate3d_t* rotate_out, rotate3d_t const* rotate_lh, rotate3d_t const* rotate_rh, ReferenceFrame_t const ref)
{
	failassert(rotate_out && rotate_lh && rotate_rh && cdrawRefValid(ref), NULL);
	matMul3d(rotate_out->R, rotate_lh->R, rotate_rh->R);
	RmatToAngles3d(rotate_out->angles, rotate_out->R, cdrawRefRotateAxis(ref));
	return rotate_out;
}

CDRAW_INL rotate3d_t* rotateUpdateAngles3d(rotate3d_t* rotate_out, ReferenceFrame_t const ref)
{
	failassert(rotate_out && cdrawRefValid(ref), NULL);
	RmatToAngles3d(rotate_out->angles, rotate_out->R, cdrawRefRotateAxis(ref));
	return rotate_out;
}

CDRAW_INL rotate3d_t* rotateSetAngles3d(rotate3d_t* rotate_out, angle3d_t const* angles, ReferenceFrame_t const ref)
{
	failassert(rotate_out && angles && cdrawRefValid(ref), NULL);
	vecAbsToRel3d(rotate_out->angles, angles->v, ref);
	RmatFromAngles3d(rotate_out->R, rotate_out->angles, rotate_out->angles, cdrawRefRotateAxis(ref));
	return rotate_out;
}

CDRAW_INL angle3d_t* rotateGetAngles3d(angle3d_t* angles_out, rotate3d_t const* rotate, ReferenceFrame_t const ref)
{
	failassert(angles_out && rotate && cdrawRefValid(ref), NULL);
	vecRelToAbs3d(angles_out->v, rotate->angles, ref);
	return angles_out;
}

CDRAW_INL rotate3d_t* rotateInvertAngles3d(rotate3d_t* rotate_out, rotate3d_t const* rotate, ReferenceFrame_t const ref)
{
	failassert(rotate_out && rotate && cdrawRefValid(ref), NULL);
	vecNegate3d(rotate_out->angles, rotate->angles);
	RmatFromAngles3d(rotate_out->R, 0, rotate_out->angles, cdrawRefRotateAxis(ref));
	return rotate_out;
}

CDRAW_INL rotate3d_t* rotateConcatAngles3d(rotate3d_t* rotate_out, rotate3d_t const* rotate_lh, rotate3d_t const* rotate_rh, ReferenceFrame_t const ref)
{
	failassert(rotate_out && rotate_lh && rotate_rh && cdrawRefValid(ref), NULL);
	vecAdd3d(rotate_out->angles, rotate_lh->angles, rotate_rh->angles);
	RmatFromAngles3d(rotate_out->R, rotate_out->angles, rotate_out->angles, cdrawRefRotateAxis(ref));
	return rotate_out;
}

CDRAW_INL translate3d_t* translateReset3d(translate3d_t* translate_out)
{
	failassert(translate_out, NULL);
	vecZero3d(translate_out->t);
	return translate_out;
}

CDRAW_INL translate3d_t* translateSetAxis3d(translate3d_t* translate_out, axis3d_t const* axis, ReferenceFrame_t const ref)
{
	failassert(translate_out && axis && cdrawRefValid(ref), NULL);
	vecAbsToRel3d(translate_out->t, axis->v, ref);
	return translate_out;
}

CDRAW_INL axis3d_t* translateGetAxis3d(axis3d_t* axis_out, translate3d_t const* translate, ReferenceFrame_t const ref)
{
	failassert(axis_out && translate && cdrawRefValid(ref), NULL);
	vecRelToAbs3d(axis_out->v, translate->t, ref);
	return axis_out;
}

CDRAW_INL translate3d_t* translateInvertAxis3d(translate3d_t* translate_out, translate3d_t const* translate)
{
	failassert(translate_out && translate, NULL);
	vecNegate3d(translate_out->t, translate->t);
	return translate_out;
}

CDRAW_INL translate3d_t* translateConcatAxis3d(translate3d_t* translate_out, translate3d_t const* translate_lh, translate3d_t const* translate_rh)
{
	failassert(translate_out && translate_lh && translate_rh, NULL);
	vecAdd3d(translate_out->t, translate_lh->t, translate_rh->t);
	return translate_out;
}

CDRAW_INL scale3d_t* scaleReset3d(scale3d_t* scale_out)
{
	failassert(scale_out, NULL);
	scale_out->s = sc1D;
	return scale_out;
}

CDRAW_INL scale3d_t* scaleInvertAxis3d(scale3d_t* scale_out, scale3d_t const* scale)
{
	failassert(scale_out && scale, NULL);
	scale_out->s = scIsNonZeroApproxD(scale->s) ? (sc1D / scale->s) : sc0D;
	return scale_out;
}

CDRAW_INL scale3d_t* scaleConcatAxis3d(scale3d_t* scale_out, scale3d_t const* scale_lh, scale3d_t const* scale_rh)
{
	failassert(scale_out && scale_lh && scale_rh, NULL);
	scale_out->s = scale_lh->s * scale_rh->s;
	return scale_out;
}

CDRAW_INL transform3d_t* transformReset3d(transform3d_t* transform_out)
{
	failassert(transform_out, NULL);
	matIdentity3d(transform_out->rs.m);
	vecZero3d(transform_out->t.v);
	rotateReset3d(&transform_out->rotate);
	translateReset3d(&transform_out->translate);
	scaleReset3d(&transform_out->scale);
	return transform_out;
}

CDRAW_INL transform3d_t* transformUpdate3d(transform3d_t* transform_out)
{
	failassert(transform_out, NULL);
	vecCopy3d(transform_out->t.v, transform_out->translate.t);
	matMulS3d(transform_out->rs.m, transform_out->rotate.R, transform_out->scale.s);
	return transform_out;
}

CDRAW_INL transform3d_t* transformInvert3d(transform3d_t* transform_out, transform3d_t const* transform, ReferenceFrame_t const ref)
{
	failassert(transform_out && transform && cdrawRefValid(ref), NULL);
	vecd_t const scaleInv = (sc1D / transform->scale.s);
	matTranspose3d(transform_out->rs.m, transform->rs.m);
	matMulS3d(transform_out->rotate.R, transform_out->rs.m, scaleInv);
	rotateUpdateAngles3d(&transform_out->rotate, ref);
	matMulS3d(transform_out->rs.m, transform_out->rotate.R, scaleInv);
	vecNegate3d(transform_out->t.v, transform->t.v);
	matMulVec3d(transform_out->t.v, transform_out->rs.m, transform_out->t.v);
	vecCopy3d(transform_out->translate.t, transform_out->t.v);
	transform_out->scale.s = scaleInv;
	return transform_out;
}

CDRAW_INL transform3d_t* transformConcat3d(transform3d_t* transform_out, transform3d_t const* transform_lh, transform3d_t const* transform_rh, ReferenceFrame_t const ref)
{
	failassert(transform_out && transform_lh && transform_rh && cdrawRefValid(ref), NULL);
	vecd_t const scale = (transform_lh->scale.s * transform_lh->scale.s), scaleInv = (sc1D / scale);
	TmatMul3d(transform_out->T, transform_lh->T, transform_rh->T);
	matMulS3d(transform_out->rotate.R, transform_out->T, scaleInv);
	rotateUpdateAngles3d(&transform_out->rotate, ref);
	vecCopy3d(transform_out->translate.t, transform_out->t.v);
	transform_out->scale.s = scale;
	return transform_out;
}

CDRAW_INL transform3d_t* transformUpdateComponents3d(transform3d_t* transform_out, ReferenceFrame_t const ref)
{
	failassert(transform_out && cdrawRefValid(ref), NULL);
	vecd_t const scale = vecLen3d(vx(transform_out->T)), scaleInv = (sc1D / scale);
	matMulS3d(transform_out->rotate.R, transform_out->T, scaleInv);
	rotateUpdateAngles3d(&transform_out->rotate, ref);
	vecCopy3d(transform_out->translate.t, transform_out->t.v);
	transform_out->scale.s = scale;
	return transform_out;
}

CDRAW_INL transform3d_t* transformInvertComponents3d(transform3d_t* transform_out, transform3d_t const* transform, ReferenceFrame_t const ref)
{
	failassert(transform_out && transform && cdrawRefValid(ref), NULL);
	rotateInvertAngles3d(&transform_out->rotate, &transform->rotate, ref);
	translateInvertAxis3d(&transform_out->translate, &transform->translate);
	scaleInvertAxis3d(&transform_out->scale, &transform->scale);
	return transformUpdate3d(transform_out);
}

CDRAW_INL transform3d_t* transformConcatComponents3d(transform3d_t* transform_out, transform3d_t const* transform_lh, transform3d_t const* transform_rh, ReferenceFrame_t const ref)
{
	failassert(transform_out && transform_lh && transform_rh && cdrawRefValid(ref), NULL);
	rotateConcatAngles3d(&transform_out->rotate, &transform_lh->rotate, &transform_rh->rotate, ref);
	translateConcatAxis3d(&transform_out->translate, &transform_lh->translate, &transform_rh->translate);
	scaleConcatAxis3d(&transform_out->scale, &transform_lh->scale, &transform_rh->scale);
	return transformUpdate3d(transform_out);
}


#endif // #if (!(defined _CDRAW_TRANSFORM3D_INL_) && (defined _CDRAW_TRANSFORM_INL_))