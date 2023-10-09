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
* cdrawTransform.inl
* Definitions of transform functions.
*/

#if (!(defined _CDRAW_TRANSFORM_INL_) && (defined _CDRAW_TRANSFORM_H_))
#define _CDRAW_TRANSFORM_INL_


/******************************************************************************
* Type-agnostic functions.
******************************************************************************/


CDRAW_INL ReferenceFrame_t cdrawReferenceFrame(AxisOrder_t const axisOrder, RotateOrder_t const rotateOrder)
{
	// remapping F/R/D signs and indices
	static const uint16_t axOrder[] = {
		0b000000110, // axis_rh_FRD_NED
		0b001010010, // axis_rh_RFU_ENU
		0b011000110, // axis_rh_FLU
	};

	// remapping R/P/Y axes
	static const uint8_t rotOrder[][6] = {
		{ rot_ZYX, rot_YXZ, rot_XZY, rot_YZX, rot_ZXY, rot_XYZ }, // axis_rh_FRD_NED
		{ rot_ZXY, rot_XYZ, rot_YZX, rot_XZY, rot_ZYX, rot_YXZ }, // axis_rh_RFU_ENU
		{ rot_ZYX, rot_YXZ, rot_XZY, rot_YZX, rot_ZXY, rot_XYZ }, // axis_rh_FLU
	};

	ReferenceFrame_t result = 0;
	uint8_t const axisOrder_encode = flagcheck(axisOrder, 0b111);
	uint8_t const rotateOrder_encode = flagcheck(rotateOrder, 0b111);
	result |= axisOrder_encode << 0;
	result |= rotateOrder_encode << 3;
	result |= flagcheck(axOrder[axisOrder_encode], 0b111111111) << 6;
	result |= flagcheck(rotOrder[axisOrder_encode][rotateOrder_encode], 0b111) << 16;
	return result;
}

CDRAW_INL AxisOrder_t cdrawReferenceFrameAxisOrder(ReferenceFrame_t const refFrame)
{
	return (AxisOrder_t)flagcheck(refFrame >> 0, 0b111);
}

CDRAW_INL RotateOrder_t cdrawReferenceFrameRotateOrder(ReferenceFrame_t const refFrame)
{
	return (RotateOrder_t)flagcheck(refFrame >> 3, 0b111);
}

CDRAW_INL RotateAxisOrder_t cdrawReferenceFrameRotateAxisOrder(ReferenceFrame_t const refFrame)
{
	return (RotateAxisOrder_t)flagcheck(refFrame >> 16, 0b111);
}


#define cdrawRefForwardSgn(refFrame)	flagcheckincl(refFrame >> 14, 0b1)
#define cdrawRefRightSgn(refFrame)		flagcheckincl(refFrame >> 13, 0b1)
#define cdrawRefDownSgn(refFrame)		flagcheckincl(refFrame >> 12, 0b1)
#define cdrawRefForwardIdx(refFrame)	flagcheck(refFrame >> 10, 0b11)
#define cdrawRefRightIdx(refFrame)		flagcheck(refFrame >>  8, 0b11)
#define cdrawRefDownIdx(refFrame)		flagcheck(refFrame >>  6, 0b11)


/******************************************************************************
* Functions.
******************************************************************************/

#include "cdrawTransform/cdrawTransform2f.inl"
#include "cdrawTransform/cdrawTransform2d.inl"
#include "cdrawTransform/cdrawTransform3f.inl"
#include "cdrawTransform/cdrawTransform3d.inl"


#endif // #if (!(defined _CDRAW_TRANSFORM_INL_) && (defined _CDRAW_TRANSFORM_H_))