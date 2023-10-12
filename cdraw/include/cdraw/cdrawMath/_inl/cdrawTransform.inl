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
	// remapping FRD/RPY signs and indices
	static const uint16_t axOrder[] = {
		0b000000110, // axis_rh_FRD_NED
		0b001010010, // axis_rh_RFU_ENU
		0b011000110, // axis_rh_FLU
	};

	// remapping RPY axes
	static const uint8_t rotOrder[][6] = {
		{ rot_ZYX, rot_YXZ, rot_XZY, rot_YZX, rot_ZXY, rot_XYZ }, // axis_rh_FRD_NED
		{ rot_ZXY, rot_XYZ, rot_YZX, rot_XZY, rot_ZYX, rot_YXZ }, // axis_rh_RFU_ENU
		{ rot_ZYX, rot_YXZ, rot_XZY, rot_YZX, rot_ZXY, rot_XYZ }, // axis_rh_FLU
	};

	ReferenceFrame_t result = 0;
	result |= flagcheck(axisOrder, 0b111) << 0;
	result |= flagcheck(rotateOrder, 0b111) << 3;
	result |= flagcheck(axOrder[axisOrder], 0b111111111) << 6;
	result |= flagcheck(rotOrder[axisOrder][rotateOrder], 0b111) << 16;
	return result;
}

CDRAW_INL AxisOrder_t cdrawReferenceFrameAxisOrder(ReferenceFrame_t const ref)
{
#define cdrawRefAxis(ref) ((AxisOrder_t)flagcheck(ref >> 0, 0b111))
	return cdrawRefAxis(ref);
}

CDRAW_INL RotateOrder_t cdrawReferenceFrameRotateOrder(ReferenceFrame_t const ref)
{
#define cdrawRefRotate(ref) ((RotateOrder_t)flagcheck(ref >> 3, 0b111))
	return cdrawRefRotate(ref);
}

CDRAW_INL RotateAxisOrder_t cdrawReferenceFrameRotateAxisOrder(ReferenceFrame_t const ref)
{
#define cdrawRefRotateAxis(ref) ((RotateAxisOrder_t)flagcheck(ref >> 16, 0b111))
	return cdrawRefRotateAxis(ref);
}


#define cdrawRefSgnR(ref)	flagcheckincl(ref >> 14, 0b1)
#define cdrawRefSgnP(ref)	flagcheckincl(ref >> 13, 0b1)
#define cdrawRefSgnY(ref)	flagcheckincl(ref >> 12, 0b1)
#define cdrawRefIdxR(ref)	flagcheck(ref >> 10, 0b11)
#define cdrawRefIdxP(ref)	flagcheck(ref >>  8, 0b11)
#define cdrawRefIdxY(ref)	flagcheck(ref >>  6, 0b11)
#define cdrawRefValid(ref)	(ref == cdrawReferenceFrame(cdrawRefAxis(ref), cdrawRefRotate(ref)))


/******************************************************************************
* Functions.
******************************************************************************/

#include "cdrawTransform/cdrawTransform2f.inl"
#include "cdrawTransform/cdrawTransform2d.inl"
#include "cdrawTransform/cdrawTransform3f.inl"
#include "cdrawTransform/cdrawTransform3d.inl"


#endif // #if (!(defined _CDRAW_TRANSFORM_INL_) && (defined _CDRAW_TRANSFORM_H_))