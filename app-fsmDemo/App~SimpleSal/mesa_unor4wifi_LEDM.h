/* -- SimpleSal: ssTEA, ssUI, and ssIO | Copyright (C) 2025, 2026 TruSoft Computing LLC |  All rights reserved. --
   This software is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
   This software is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
   ---------------------------------------------------------------------------------------------------
 *
 * mesa_unor4wifo_LEDM.h            only slightly helpful helpers
 *
 * 2026/10/10   "fsmDemo.ino" arduino App update release version of SimpleSal as "app-fsmDemo.ino".
 * 2025/08/06   special version appeared as the star of first videos published at SimpleSal substack.
 * 2024/05/27   created superset FSM Demo Application as basis for ssTEA and SimpleSal dev/test.
 **************************************************************************************************/
#ifndef __MESA_UNOR4WIFI_LEDM_H
#define __MESA_UNOR4WIFI_LEDM_H

// -------------------------------------------------------------------------------------------------
// EXCEPTION TO LICENSE AND COPYRIGHT ASSERTIONS:
// The license and copyright assertions within the official file released by Arduino are inherited.
// The assertions by TruSoft Computing LLC apply to this source referring to Arduino, not to Arduino.
// -------------------------------------------------------------------------------------------------
#include "Arduino_LED_Matrix.h"

// -------------------------------------------------------------------------------------------------
// A bunch of magical stuff that is assumed to be understood by everyone occurs here, apparently.
// -------------------------------------------------------------------------------------------------
ArduinoLEDMatrix    matrixBlob;

// -------------------------------------------------------------------------------------------------
// This function is the equivalent of the Bitmap version which simply assigns a value to a row/col.
// The Frame version requires a computation to set and clear one bit within a 96-bit data type.
// -------------------------------------------------------------------------------------------------
#ifdef  LEDM_ONEOF_CarrierIsFrame
void LEDM_FrameBitAtRowColGetsValue (int LEDM_RowNumber, int LEDM_ColNumber, int PinValue);
#endif // LEDM_ONEOF_CarrierIsFrame

// -------------------------------------------------------------------------------------------------
// This is a functional display of all the LEDs and also shows the quality of the random number set.
// -------------------------------------------------------------------------------------------------
// #define LEDM_OPTIN_STYLE_RANDOMALL

// -------------------------------------------------------------------------------------------------
// The Matrix software defines signal carriers to transfer a description from memory into a device.
// The device does not switch the geometry between 8 rows by 12 columns versus 12 rows by 8 columns,
// all signals sent to the device assume there are 8 rows, each with 12 columns.  Each LED value is
// stored as bits or bytes, the value 0 describes "LED off" and the value 1 describes "LED on".
// -------------------------------------------------------------------------------------------------
// For 1 row by 12 columns, the message is either: bytes <0,0,1,1,1,1,0,0,1,1,1,1> or bits <0x3CF>.
// For bytes: each LED is described by one byte whose value is either 0 or 1, for a total of 96.
// for bits:  each row of 12 columns maps into 3 hex "nibbles"; 8 groups of 3 nibbles, one per row.
// -------------------------------------------------------------------------------------------------
#define Tuple_Row0      (0)
#define Tuple_Row1      (1)
#define Tuple_Row2      (2)
#define Tuple_Row3      (3)
#define Tuple_Row4      (4)
#define Tuple_Row5      (5)
#define Tuple_Row6      (6)
#define Tuple_Row7      (7)
#define Tuple_Col0      (0)
#define Tuple_Col1      (1)
#define Tuple_Col2      (2)
#define Tuple_Col3      (3)
#define Tuple_Col4      (4)
#define Tuple_Col5      (5)
#define Tuple_Col6      (6)
#define Tuple_Col7      (7)

// -------------------------------------------------------------------------------------------------
// Bitmap signal carrier or medium transfers 96 bytes: each value is applied to a corresponding LED.
// The frame medium transfers 96 bits to do the same, at the cost of doing math to compute the bits.
// -------------------------------------------------------------------------------------------------
#ifdef  LEDM_ONEOF_CarrierIsBitMap
Bits8_t LEDM_AsBytes[LEDM_LEDs_Rows][LEDM_LEDs_Cols];
#endif  // LEDM_ONEOF_CarrierIsBitMap

#ifdef  LEDM_ONEOF_CarrierIsFrame
unsigned long int           LEDM_Frame[3];     // The chip would have to change for this 3 to change
#define LEDM_bits_max_i     (3-1)
#endif  // LEDM_ONEOF_CarrierIsFrame

// -------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------
// The LED Shape software represents Channels ranging from 0-7, each channel controls an output pin.
// This is the basis of the design of MrBlinky (Due) and ports directly to the LED Matrix UnoR4.
// The LED matrix doesn't know about what the LED represents; it is just a grid of rows and columns.
// MrBlinky's design is a one dimensional array (the row is one 8-port AC switcher) and 8 columns.
// -------------------------------------------------------------------------------------------------
// The definition of a "shape" is a series of LEDs, each LED individually on or off.  The location
// of the the upper left corner of the matrix in portrait or landscape mode is called LED[0,0].
// A Shape has 4 pixels, each pixel is named using a single [row,col] coordinate on the matrix grid.
// The Shape selection for a line of 8 LEDs maps to the state of each channel in MrBlinky's design.
// The Shape selection for a Cloctal dial "needle" maps in the same way to the MrBlinky channel.
// -------------------------------------------------------------------------------------------------
#define PIXEL_NOT_THERE     (0x080C)                // and out-of-range row/column in 8x12 or 12x8
#define PN                  PIXEL_NOT_THERE

// Each 5x5 Cloctal space is 5 rows by 5 columns, but the Shape definition only needs to name 4 pixels.
// In the ShapeSet data here, each row is a Shape, each col is 1 out of 4 within a 5/5 square, the
// x*?*x column demonstrates the "hand" of the clock octal or cloctal is long on one side, short on
// the other side, and note that all of the hands of octal shared the same pivot point in the middle.
Bits16_t ShapeSet[][4] =
{ //  [0,2]   [1,2]  [2,2]  [3,2]     // any [row, col] in portrait/landscape
    { 0x002, 0x102, 0x202, 0x302   /**/ },  // [dial Needle 0]  vertical from top
    { 0x004, 0x103, 0x202, 0x301   /**/ },  // [dial Needle 1]  slope up to upper right
    { /**/   0x201, 0x202, 0x203, 0x204 },  // [dial Needle 2]  horizontal to right
    { 0x404, 0x303, 0x202, 0x101   /**/ },  // [dial Needle 3]  slope down to lower right
    { /**/   0x102, 0x202, 0x302, 0x402 },  // [dial Needle 4]  vertical at bottom
    { 0x400, 0x301, 0x202, 0x103   /**/ },  // [dial Needle 5]  slope down to lower left
    { 0x200, 0x201, 0x202, 0x203   /**/ },  // [dial Needle 6]  horizontal to left
    { 0x000, 0x101, 0x202, 0x303   /**/ }   // [dial Needle 7]  slope up to upper left
};

#define Unpack_Row(Pixel)   (((Pixel) >> 8) & 0xFF)
#define Unpack_Col(Pixel)   ((Pixel)        & 0xFF)

#ifdef  LEDM_ONEOF_CarrierIsBitMap
#define LEDM_SetLEDValue(xwork,xnewvalue)   \
        LEDM_AsBytes[ (Unpack_Row (xwork)) ][ (Unpack_Col (xwork)) ] = xnewvalue;
#endif  // LEDM_ONEOF_CarrierIsBitMap

#ifdef  LEDM_ONEOF_CarrierIsFrame
#define LEDM_SetLEDValue(xwork,xnewvalue)   \
        LEDM_FrameBitAtRowColGetsValue ((Unpack_Row (xwork)), Unpack_Col (xwork), xnewvalue)
#endif  // LEDM_ONEOF_CarrierIsFrame
// -------------------------------------------------------------------------------------------------
// Make changes to the matrix in the configured form (bytes or bits) but do not emit the changes.
// -------------------------------------------------------------------------------------------------
void  LEDM_matrixModify (int LEDM_RowNumber, int LEDM_ColNumber, LogicalLEDValue_t LEDValue)
{
#ifdef LEDM_OPTIN_debugLEDM
    mesa_uiOp_emit_qAsciiA ("led Matrix update for LED row<");
    mesa_uiOp_emit_Int_999 ((Bits8_t) LEDM_RowNumber);
    mesa_uiOp_emit_qAsciiA ("> column <");
    mesa_uiOp_emit_Int_999 ((Bits8_t) LEDM_ColNumber);
    mesa_uiOp_emit_qAsciiA (">  value<");
    mesa_uiOp_emit_Int_999 ((Bits8_t) LEDValue);
    mesa_uiOp_emit_qAsciiA (">");
    mesa_uiOp_emit_newline ();
#endif  // LEDM_OPTIN_debugLEDM

#ifdef LEDM_ONEOF_CarrierIsBitMap
    LEDM_AsBytes[LEDM_RowNumber][LEDM_ColNumber] = (Bits8_t) LEDValue;
#endif  // LEDM_ONEOF_CarrierIsBitMap

#ifdef LEDM_ONEOF_CarrierIsFrame
    LEDM_FrameBitAtRowColGetsValue (LEDM_RowNumber, LEDM_ColNumber, LEDValue);
#endif  // LEDM_ONEOF_CarrierIsFrame
}   // LEDM_matrixModify
// -------------------------------------------------------------------------------------------------
void LEDM_shapeOp_DecodeAndApply (Bits16_t Base, Bits16_t *pPixel, LogicalPinValue_t LEDValue)
{
    Bits16_t    work;

    work = *pPixel;
    if (work != PIXEL_NOT_THERE)
    {
        work += Base;
        LEDM_matrixModify ((Unpack_Row (work)), (Unpack_Col (work)), LEDValue);
    }   // leave LED untouched
    pPixel++;
    work = *pPixel;
    if (work != PIXEL_NOT_THERE)
    {
        work += Base;
        LEDM_matrixModify ((Unpack_Row (work)), (Unpack_Col (work)), LEDValue);
    }   // leave LED untouched
    pPixel++;
    work = *pPixel;
    if (work != PIXEL_NOT_THERE)
    {
        work += Base;
        LEDM_matrixModify ((Unpack_Row (work)), (Unpack_Col (work)), LEDValue);
    }   // leave LED untouched
    pPixel++;
    work = *pPixel;
    if (work != PIXEL_NOT_THERE)
    {
        work += Base;
        LEDM_matrixModify ((Unpack_Row (work)), (Unpack_Col (work)), LEDValue);
    }   // leave LED untouched
//  pPixel++;       // unnecessary, done
    LEDM_matrixRefresh ();
}   // LEDM_shapeOp_DecodeAndApply
// -------------------------------------------------------------------------------------------------
void LEDM_shapeOp_ClearAll (int MrB_ID, int fromShape_i, int toShape_i)
{
    Bits16_t    Shape_Base;
    int         shape_i;

    switch (MrB_ID)
    {
        case MAC_ID_0   : Shape_Base = MAC_ID_0_Shape_Base; break;
        case MAC_ID_1   : Shape_Base = MAC_ID_1_Shape_Base; break;
        default         : Shape_Base = MAC_ID_1_Shape_Base; break;
    }

    for (shape_i = fromShape_i; shape_i <= toShape_i; shape_i++)
    {
        LEDM_shapeOp_DecodeAndApply (Shape_Base, &ShapeSet[shape_i][0], 0);
    }   // for
}   // LEDM_shapeOp_ClearAll
// -------------------------------------------------------------------------------------------------
void LEDM_shapeOp_LightAll (int MrB_ID, int fromShape_i, int toShape_i)
{
    Bits16_t    Shape_Base;
    int         shape_i;

    switch (MrB_ID)
    {
        case MAC_ID_0   : Shape_Base = MAC_ID_0_Shape_Base; break;
        case MAC_ID_1   : Shape_Base = MAC_ID_1_Shape_Base; break;
        default         : Shape_Base = MAC_ID_1_Shape_Base; break;
    }

    for (shape_i = fromShape_i; shape_i <= toShape_i; shape_i++)
    {
        LEDM_shapeOp_DecodeAndApply (Shape_Base, &ShapeSet[shape_i][0], 1);
    }   // for
}   // LEDM_shapeOp_LightAll
// -------------------------------------------------------------------------------------------------
void LEDM_shapeOp_Clear (int MrB_ID, int ShapeSelect)
{
    Bits16_t    Shape_Base;

    switch (MrB_ID)
    {
        case MAC_ID_0 :
        {
            Shape_Base = MAC_ID_0_Shape_Base;
            LEDM_shapeOp_DecodeAndApply (Shape_Base, &ShapeSet[ShapeSelect][0], 0);
            break;
        }   // case MAC_ID_0
        case MAC_ID_1 :
        {
            Shape_Base = MAC_ID_1_Shape_Base;
            LEDM_shapeOp_DecodeAndApply (Shape_Base, &ShapeSet[ShapeSelect][0], 0);
            break;
        }   // case MAC_ID_1
    }   // switch MrB_ID
}   //  LEDM_shapeOp_Clear
// -------------------------------------------------------------------------------------------------
void LEDM_shapeOp_Light (int MrB_ID, int ShapeSelect)
{
    Bits16_t    Shape_Base;

    switch (MrB_ID)
    {
        case MAC_ID_0 :
        {
            Shape_Base = MAC_ID_0_Shape_Base;
            LEDM_shapeOp_DecodeAndApply (Shape_Base, &ShapeSet[ShapeSelect][0], 1);
            break;
        }   // case MAC_ID_0
        case MAC_ID_1 :
        {
            Shape_Base = MAC_ID_1_Shape_Base;
            LEDM_shapeOp_DecodeAndApply (Shape_Base, &ShapeSet[ShapeSelect][0], 1);
            break;
        }   // case MAC_ID_1
    }   // switch MrB_ID
}   //  LEDM_shapeOp_Light
// -------------------------------------------------------------------------------------------------
void  LEDM_matrixSetup (void)
{
    matrixBlob.begin();
    LEDM_blastOut_HelloWorld ();
}   // LEDM_matrixSetup
// -------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------
void  LEDM_matrixRefresh (void)
{
#ifdef LEDM_ONEOF_CarrierIsBitMap
    matrixBlob.renderBitmap (LEDM_AsBytes, LEDM_LEDs_Rows, LEDM_LEDs_Cols);
#endif  // LEDM_ONEOF_CarrierIsBitMap
#ifdef LEDM_ONEOF_CarrierIsFrame
    matrixBlob.loadFrame (LEDM_Frame);
#endif  // LEDM_ONEOF_CarrierIsFrame
}   // LEDM_matrixRefresh
// -------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------
void LEDM_matrixClearAll (void)
{
#ifdef LEDM_ONEOF_CarrierIsBitMap
    int row_i, col_i;

    for (row_i=0; row_i < LEDM_LEDs_Rows; row_i++)
    {
        for (col_i=0; col_i < LEDM_LEDs_Cols; col_i++)
        {
            LEDM_AsBytes[row_i][col_i] = 0;
        }   // for col_i
    }   // for row_i
#endif  // LEDM_ONEOF_CarrierIsBitMap
#ifdef LEDM_ONEOF_CarrierIsFrame
    LEDM_Frame[0] = LEDM_Frame[1] = LEDM_Frame[2] = 0;
#endif  // LEDM_ONEOF_CarrierIsFrame
    LEDM_matrixRefresh ();
}   // LEDM_matrixClearAll
// -------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------
//#define LEDM_DEBUG_SHOW_SHAPE_CHOSEN

// -------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------
// #define LEDM_OPTIN_MrB0_group_VERT
// #define LEDM_OPTIN_MrB0_group_FLAT
// #define LEDM_OPTIN_MrB1_group_VERT
// #define LEDM_OPTIN_MrB1_group_FLAT

// #define LEDM_OPTIN_MrB0_Shape_CHEVRON
// #define LEDM_OPTIN_MrB0_shape_CLOCTALS

// #define LEDM_OPTIN_MrB1_Shape_CHEVRON
// #define LEDM_OPTIN_MrB1_shape_CLOCTALS

#ifdef LEDM_DEBUG_SHOW_SHAPE_CHOSEN
#define LEDM_IsShowingShape(x)    {mesa_uiOp_emit_qAsciiA (S(x));  mesa_uiOp_emit_newline (); }
#else   // not LEDM_DEBUG_SHOW_SHAPE_CHOSEN
#define LEDM_IsShowingShape(x)
#endif  // not LEDM_DEBUG_SHOW_SHAPE_CHOSEN

// -------------------------------------------------------------------------------------------------
// The Shape PinNumber selects the shape from 8 shapes.
// The Group PinNumber selects the pin in the group of 8 pins called a "Mr Blinky".
// -------------------------------------------------------------------------------------------------
void    LEDM_DigitalWrite_MrB (int MrB_ID, Bits8_t MrB_IDs_PinNumber, int LEDvalue)
{
    int ShapeNumber = MrB_IDs_PinNumber;        // same value, different meaning
    int GroupPinNumber = MrB_IDs_PinNumber;     // same value, different meaning
    boolean MatrixChanged = false;              // must set to true to get refresh at end

#ifdef LEDM_OPTIN_MrB0_shape_CLOCTALS
static  int LastShapeNumber_ID_0 = 7;       // true the initial value depends on the FSM Init state
#endif // LEDM_OPTIN_MrB0_shape_CLOCTALS
#ifdef LEDM_OPTIN_MrB1_shape_CLOCTALS
static  int LastShapeNumber_ID_1 = 7;       // true the initial value depends on the FSM Init state
#endif // LEDM_OPTIN_MrB1_shape_CLOCTALS

#ifdef LEDM_OPTIN_MrBshapes_CHEVRON
    int   SelectedForTuple;
#endif // LEDM_OPTIN_MrBshapes_CHEVRON

#ifdef LEDM_OPTIN_STYLE_RANDOMALL
    int   this_Random_NextFrom100;
#endif // LEDM_OPTIN_STYLE_RANDOMALL

// -------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------
#ifdef LEDM_OPTIN_MrB0_group_VERT
    if (
        (MrB_ID == MAC_ID_0) && (RO_IS_ONEOF_Matrix_ROs (RO_owner_ssUI, RO_active_group0))
       )
    {
        MatrixChanged = true;
        LEDM_IsShowingShape ("MrB0g0V");
#ifdef LEDM_ONEOF_OrientationIsPortrait
        LEDM_matrixModify (LEDM_LEDs_Row7, GroupPinNumber, LEDvalue);
#endif  // LEDM_ONEOF_OrientationIsPortrait
#ifdef LEDM_ONEOF_OrientationIsLandscape
        LEDM_matrixModify (GroupPinNumber, LEDM_LEDs_Col0, LEDvalue);
#endif  // LEDM_ONEOF_OrientationIsLandscape
    }  // MrB_ID is 0
#endif  // LEDM_OPTIN_MrB0_group_VERT
// -------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------
#ifdef LEDM_OPTIN_MrB1_group_VERT
    if (
        (MrB_ID == MAC_ID_1) && (RO_IS_ONEOF_Matrix_ROs (RO_owner_ssUI, RO_active_group1))
       )
    {
        MatrixChanged = true;
        LEDM_IsShowingShape ("MrB1g1V");
#ifdef LEDM_ONEOF_OrientationIsPortrait
        LEDM_matrixModify (LEDM_LEDs_Row7, GroupPinNumber, LEDvalue);
#endif  // LEDM_ONEOF_OrientationIsPortrait
#ifdef LEDM_ONEOF_OrientationIsLandscape
        LEDM_matrixModify (GroupPinNumber, LEDM_LEDs_Col11,  LEDvalue);
#endif  // LEDM_ONEOF_OrientationIsLandscape
    }  // MrB_ID is MAC_ID_1
#endif  // LEDM_OPTIN_MrB1_group_VERT

// -------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------
#ifdef LEDM_OPTIN_MrB0_group_FLAT
    if (
         (MrB_ID == MAC_ID_0) && (RO_IS_ONEOF_Matrix_ROs (RO_owner_ssUI, RO_active_group0))
       )
    {
        MatrixChanged = true;
        LEDM_IsShowingShape ("MrB0g0F");
#ifdef LEDM_ONEOF_OrientationIsPortrait
        LEDM_matrixModify (GroupPinNumber, LEDM_LEDs_Col0, LEDvalue);
#endif  // LEDM_ONEOF_OrientationIsPortrait
#ifdef LEDM_ONEOF_OrientationIsLandscape
        LEDM_matrixModify (LEDM_LEDs_Row0, GroupPinNumber+2, LEDvalue);
#endif  // LEDM_ONEOF_OrientationIsLandscape
    }  // MrB_ID is MAC_ID_0
#endif  // LEDM_OPTIN_MrB0_group_FLAT
// -------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------
#ifdef LEDM_OPTIN_MrB1_group_FLAT
    if (
         (MrB_ID == MAC_ID_1) && (RO_IS_ONEOF_Matrix_ROs (RO_owner_ssUI, RO_active_group1))
       )
    {
        MatrixChanged = true;
        LEDM_IsShowingShape ("MrB1g1F");
#ifdef LEDM_ONEOF_OrientationIsPortrait
        LEDM_matrixModify (GroupPinNumber, LEDM_LEDs_Col11, LEDvalue);
#endif  // LEDM_ONEOF_OrientationIsPortrait
#ifdef LEDM_ONEOF_OrientationIsLandscape
        LEDM_matrixModify (LEDM_LEDs_Row1, GroupPinNumber+2, LEDvalue);
#endif  // LEDM_ONEOF_OrientationIsLandscape
    }  // MrB_ID is MAC_ID_1
#endif  // LEDM_OPTIN_MrB1_group_FLAT
// -------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------
#ifdef LEDM_OPTIN_MrB0_shape_CLOCTALS
    if (
         (MrB_ID == MAC_ID_0) && (RO_IS_ONEOF_Matrix_ROs (RO_owner_ssUI, RO_active_shape0))
       )
    {
        MatrixChanged = true;
        LEDM_IsShowingShape ("MrB0s0");
        LEDM_shapeOp_Clear (MAC_ID_0, LastShapeNumber_ID_0);
        LEDM_shapeOp_Light (MAC_ID_0, ShapeNumber);
        LastShapeNumber_ID_0 = ShapeNumber;
    }   // MrB_ID is MAC_ID_0
#endif // LEDM_OPTIN_MrB0_shape_CLOCTALS
// -------------------------------------------------------------------------------------------------
#ifdef LEDM_OPTIN_MrB1_shape_CLOCTALS
    if (
         (MrB_ID == MAC_ID_1) && (RO_IS_ONEOF_Matrix_ROs (RO_owner_ssUI, RO_active_shape1))
       )
    {
        MatrixChanged = true;
        LEDM_IsShowingShape ("MrB1s1");
        LEDM_shapeOp_Clear (MAC_ID_1, LastShapeNumber_ID_1);
        LEDM_shapeOp_Light (MAC_ID_1, ShapeNumber);
        LastShapeNumber_ID_1 = ShapeNumber;
    }   // MrB_ID is MAC_ID_1
#endif // LEDM_OPTIN_MrB1_shape_CLOCTALS
// -------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------
#ifdef LEDM_OPTIN_STYLE_RANDOMALL
    MatrixChanged = true;
    LEDM_IsShowingShape ("randomall");
#ifdef LEDM_ONEOF_OrientationIsPortrait
    this_Random_NextFrom100 = mesa_Random_NextFrom100 ();
    LEDM_matrixModify ((this_Random_NextFrom100 % LEDM_LEDs_Rows),
                       (this_Random_NextFrom100 % LEDM_LEDs_Cols), LEDvalue);
#endif  // LEDM_ONEOF_OrientationIsPortrait
#ifdef LEDM_ONEOF_OrientationIsLandscape
    this_Random_NextFrom100 = mesa_Random_NextFrom100 ();
    LEDM_matrixModify ((this_Random_NextFrom100 % LEDM_LEDs_Rows),
                       (this_Random_NextFrom100 % LEDM_LEDs_Cols), LEDvalue);
#endif  // LEDM_ONEOF_OrientationIsLandscape
#endif  // LEDM_OPTIN_STYLE_RANDOMALL
#ifdef LEDM_OPTIN_MrBshapes_CHEVRON
    LEDM_IsShowingShape ("chevron");
#ifdef LEDM_ONEOF_OrientationIsPortrait
    switch (PinNumber)
    {
        case 0  : SelectedForTuple = Tuple_Row0;   break;
        case 1  : SelectedForTuple = Tuple_Row1;   break;
        case 2  : SelectedForTuple = Tuple_Row2;   break;
        case 3  : SelectedForTuple = Tuple_Row3;   break;
        case 4  : SelectedForTuple = Tuple_Row3;   break;
        case 5  : SelectedForTuple = Tuple_Row2;   break;
        case 6  : SelectedForTuple = Tuple_Row1;   break;
        case 7  : SelectedForTuple = Tuple_Row0;   break;
        default : SelectedForTuple = Tuple_Row6;   break;
    }   // switch
    LEDM_matrixModify (SelectedForTuple, GroupPinNumber, LEDvalue);
#endif  // LEDM_ONEOF_OrientationIsPortrait

#ifdef LEDM_ONEOF_OrientationIsLandscape
    switch (GroupPinNumber)
    {
        case 0  : SelectedForTuple = Tuple_Col0;   break;
        case 1  : SelectedForTuple = Tuple_Col1;   break;
        case 2  : SelectedForTuple = Tuple_Col2;   break;
        case 3  : SelectedForTuple = Tuple_Col3;   break;
        case 4  : SelectedForTuple = Tuple_Col3;   break;
        case 5  : SelectedForTuple = Tuple_Col2;   break;
        case 6  : SelectedForTuple = Tuple_Col1;   break;
        case 7  : SelectedForTuple = Tuple_Col0;   break;
        default : SelectedForTuple = Tuple_Col6;   break;
    }   // switch
    LEDM_matrixModify (SelectedForTuple, GroupPinNumber, LEDvalue);
#endif  // LEDM_ONEOF_OrientationIsLandscape
#endif  // LEDM_OPTIN_MrBshapes_CHEVRON

    if (MatrixChanged)
    {
        LEDM_matrixRefresh ();
    }

}   // LEDM_DigitalWrite_MrB

#ifdef LEDM_ONEOF_CarrierIsFrame
// -------------------------------------------------------------------------------------------------
// 24 bits, 0-based: bit 20 is the column 11 bit for row 0; bit 8 is the column 11 bit for row 1.
// The position in the array of 12-bit values selects the row; each bit within 12 bits select columns.
// 31  30  29  28  27  26  25  24  23  22  21  20  19  18  17  16  15  14  13  12  11  10   9   8   7   6   5   4   3   2   1   0
//
// c0: c1: c2: c3: c4: c5: c6: c7: c8: c9:c10:c11| c0: c1: c2: c3: c4: c5: c6: c7: c8: c9:c10:c11| c0: c1: c2: c3: c4: c5: c6: c7
// c8: c9:c10:c11| c0: c1: c2: c3: c4: c5: c6: c7: c8: c9:c10:c11| c0: c1: c2: c3| c4: c5: c6: c7: c8: c9:c10:c11| c0: c1: c2: c3
// c4: c5: c6: c7: c8: c9:c10:c11| c0: c1: c2: c3: c4: c5: c6: c7: c8: c9:c10:c11| c0: c1: c2: c3: c4: c5: c6: c7: c8: c9:c10:c11
// -------------------------------------------------------------------------------------------------
#define  LEDM_Row0 (0)
#define  LEDM_Row1 (1)
#define  LEDM_Row2 (2)
#define  LEDM_Row3 (3)
#define  LEDM_Row4 (4)
#define  LEDM_Row5 (5)
#define  LEDM_Row6 (6)
#define  LEDM_Row7 (7)

void LEDM_FrameBitAtRowColGetsValue (int LEDM_RowNumber, int LEDM_ColNumber, int LEDvalue)
{
    unsigned long int   work = 0;
    int                 LEDM_bits_i = 0;

        switch (LEDM_RowNumber)
        {
            case LEDM_Row0 :
                work = Bit20;               // bit 0 of a 12-bit value for row 0 in 32-bit bits[0]
                work = work << (12 - (LEDM_ColNumber + 1));     // shift from 0 to 11 into 1 to 12
                LEDM_bits_i = 0;
                break;
            case LEDM_Row1 :
                work = Bit8;                // bit 0 of a 12-bit value for row 1 in 32 bits
                work = work << (12 - (LEDM_ColNumber + 1));     // shift from 0 to 11 bits left
                LEDM_bits_i = 0;
                break;
            case LEDM_Row2 :
                if (LEDM_ColNumber < 8)
                {
                    work = Bit0;            // bit 0 of a 12-bit value for row 2 in 32 bits
                    work = work << (8 - (LEDM_ColNumber + 1));     // shift from 0 to 11 bits left
                    LEDM_bits_i = 0;
                }
                else
                {
                    work = Bit28;           // bit 8 of a 12-bit value for row 2 in 32 bits
                    work = work << (12 - (LEDM_ColNumber + 1));     // shift from 0 to 11 bits left
                    LEDM_bits_i = 1;
                }
                break;
            case LEDM_Row3 :
                work = Bit16;               // bit 0 of a 12-bit value for row 3 in 32 bits
                work = work << (12 - (LEDM_ColNumber + 1));     // shift from 0 to 11 bits left
                LEDM_bits_i = 1;
                break;
            case LEDM_Row4 :
                work = Bit4;                // bit 0 of a 12-bit value for row 4 in 32 bits
                work = work << (12 - (LEDM_ColNumber + 1));     // shift from 0 to 11 bits left
                LEDM_bits_i = 1;
                break;
            case LEDM_Row5 :
                if (LEDM_ColNumber < 4)
                {
                    work = Bit0;            // bit 3 of a 12-bit value for row 2 in 32 bits
                    work = work << (4 - (LEDM_ColNumber + 1));     // shift from 0 to 11 bits left
                    LEDM_bits_i = 1;
                }
                else
                {
                    work = Bit24;           // bit 0 of a 12-bit value for row 2 in 32 bits
                    work = work << (12 - (LEDM_ColNumber + 1));     // shift from 0 to 11 bits left
                    LEDM_bits_i = 2;
                }
                break;
            case LEDM_Row6 :
                work = Bit12;               // bit 0 of a 12-bit value for row 4 in 32 bits
                work = work << (12 - (LEDM_ColNumber + 1));     // shift from 0 to 11 bits left
                LEDM_bits_i = 2;
                break;
            case LEDM_Row7 :
                work = Bit0;                // bit 0 of a 12-bit value for row 4 in 32 bits
                work = work << (12 - (LEDM_ColNumber + 1));     // shift from 0 to 11 bits left
                LEDM_bits_i = 2;
                break;
        }   //   switch

        if (LEDvalue == 0)
        {
            work = ~work;
            LEDM_Frame[LEDM_bits_i] &= work;
        }
        else
        {
            LEDM_Frame[LEDM_bits_i] |= work;
        }
}   // LEDM_FrameBitAtRowColGetsValue
#endif // LEDM_ONEOF_CarrierIsFrame
// -------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------
#ifdef  LEDM_ONEOF_CarrierIsFrame
void    LEDM_BlastFrame (unsigned long pattern)
{
    int bits_i;

    for (bits_i=0; bits_i <= LEDM_bits_max_i; bits_i++)
    {
        LEDM_Frame[bits_i] = 0;
    }   // for
    matrixBlob.loadFrame (LEDM_Frame);
    for (bits_i=0; bits_i <= LEDM_bits_max_i; bits_i++)
    {
        LEDM_Frame[bits_i] = pattern;
    }   // for
    matrixBlob.loadFrame (LEDM_Frame);
}   // LEDM_BlastFrame
#endif  // LEDM_ONEOF_CarrierIsFrame
// -------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------
#ifdef  LEDM_ONEOF_CarrierIsBitMap

#define col_none    0x77

void    LEDM_AllOnThisColumn (int column)
{
    int row_i, col_i;

    for (row_i=0; row_i < 8; row_i++)
    {
        for (col_i=0; col_i < 12; col_i++)
        {
            if (column != col_none)
            {
                LEDM_AsBytes[row_i][col_i] = (column == col_i) ? 1 : 0;
            }
            else
            {
                LEDM_AsBytes[row_i][col_i] = 0;
            }
        }   // for col_i
    }   // for row_i

    LEDM_matrixRefresh ();
}   // LEDM_AllOnThisColumn
#endif  // LEDM_ONEOF_CarrierIsBitMap
// -------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------
void  LEDM_blastOut_HelloWorld (void)
{
#ifdef  LEDM_ONEOF_CarrierIsFrame
    LEDM_BlastFrame (0x00000000);
    delay(50);
    LEDM_BlastFrame (0x88888888);
    delay(50);
    LEDM_BlastFrame (0x44444444);
    delay(50);
    LEDM_BlastFrame (0x22222222);
    delay(50);
    LEDM_BlastFrame (0x11111111);
    delay(50);
    LEDM_BlastFrame (0x00000000);
#endif  // LEDM_ONEOF_CarrierIsFrame

#ifdef  LEDM_ONEOF_CarrierIsBitMap
    int col_i;

    LEDM_AllOnThisColumn (col_none);
    for (col_i=0; col_i < 12; col_i++)
    {
        LEDM_AllOnThisColumn (col_i);
        delay(50);
    }   // for
    LEDM_AllOnThisColumn (col_none);
    for (col_i=11; col_i > 0; col_i--)
    {
        LEDM_AllOnThisColumn (col_i);
        delay(50);
    }   // for
    LEDM_AllOnThisColumn (col_none);
#endif  // LEDM_ONEOF_CarrierIsBitMap
}   // LEDM_blastOut_HelloWorld
// -------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------
void  LEDM_blastOut_GoodbyeWorld (void)
{
#ifdef  LEDM_ONEOF_CarrierIsFrame
    LEDM_BlastFrame (0x00000000);
    delay(50);
    LEDM_BlastFrame (0x11111111);
    delay(50);
    LEDM_BlastFrame (0x22222222);
    delay(50);
    LEDM_BlastFrame (0x44444444);
    delay(50);
    LEDM_BlastFrame (0x88888888);
    delay(50);
    LEDM_BlastFrame (0x00000000);
#endif  // LEDM_ONEOF_CarrierIsFrame

#ifdef  LEDM_ONEOF_CarrierIsBitMap
    int col_i;

    LEDM_AllOnThisColumn (col_none);
    for (col_i=11; col_i >= 0; col_i--)
    {
        LEDM_AllOnThisColumn (col_i);
        delay(50);
    }   // for
    LEDM_AllOnThisColumn (col_none);
#endif  // LEDM_ONEOF_CarrierIsBitMap
}   // LEDM_blastOut_GoodbyeWorld

#endif  // __MESA_UNOR4WIFI_LEDM_H