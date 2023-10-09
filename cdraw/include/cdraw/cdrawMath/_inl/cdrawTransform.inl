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
	// remapping YPR
	static const uint16_t rotOrderBase[]  = {
		rot_RPY, // axis_rh_FLU -> XYZ=RPY
		rot_PRY, // axis_rh_RFU_ENU -> XYZ=PRY
		rot_RPY, // axis_rh_FRD_NED -> XYZ=RPY
	};

	// remapping F/R/D signs and indices
	static const uint16_t axOrder[] = {
		0b011000110, // axis_rh_FLU
		0b001010010, // axis_rh_RFU_ENU
		0b000000110, // axis_rh_FRD_NED
	};

	ReferenceFrame_t result = 0;
	result |= (axisOrder & 0b111) << 0;
	result |= (rotateOrder & 0b111) << 4;
	result |= (axOrder[axisOrder] & 0b111111111) << 16;
	result |= (((rotOrderBase[axisOrder] + rotateOrder) % 6) & 0b111) << 28;
	return result;
}

CDRAW_INL AxisOrder_t cdrawReferenceFrameAxisOrder(ReferenceFrame_t const refFrame)
{
	return (AxisOrder_t)((refFrame >> 0) & 3);
}

CDRAW_INL RotateOrder_t cdrawReferenceFrameRotateOrder(ReferenceFrame_t const refFrame)
{
	return (RotateOrder_t)((refFrame >> 4) & 3);
}


#define cdrawRefForwardIdx(refFrame)	((refFrame >> 20) & 3)
#define cdrawRefRightIdx(refFrame)		((refFrame >> 18) & 3)
#define cdrawRefDownIdx(refFrame)		((refFrame >> 16) & 3)
#define cdrawRefForwardSgn(refFrame)	(((refFrame >> 24) & 1) != 0)
#define cdrawRefRightSgn(refFrame)		(((refFrame >> 23) & 1) != 0)
#define cdrawRefDownSgn(refFrame)		(((refFrame >> 22) & 1) != 0)
#define cdrawRefAngleOrder(refFrame)	((refFrame >> 28) & 3)


/******************************************************************************
* Functions.
******************************************************************************/

#include "cdrawTransform/cdrawTransform2f.inl"
#include "cdrawTransform/cdrawTransform2d.inl"
#include "cdrawTransform/cdrawTransform3f.inl"
#include "cdrawTransform/cdrawTransform3d.inl"


#endif // #if (!(defined _CDRAW_TRANSFORM_INL_) && (defined _CDRAW_TRANSFORM_H_))