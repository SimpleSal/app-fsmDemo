/* -- SimpleSal: ssTEA, ssUI, and ssIO | (C) 2025, 2026 TruSoft Computing LLC |  All rights reserved. --
   This software is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
   This software is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
   ---------------------------------------------------------------------------------------------------
 *
 * mesa_fsmDemo_dcl.h    data type, prototype declarations: mesa-specific features.
 *
 * 2026/10/10   "fsmDemo.ino" Arduino software updated with released ssTEA/ssUI into "app-fsmDemo.ino".
 * 2025/08/06   special version appeared as the star of first videos published at SimpleSal substack.
 * 2024/05/27   created superset FSM Demo Application as basis for ssTEA and SimpleSal dev/test.
 *************************************************************************************************/
#ifndef __MESA_FSMDEMO_DCL_H
#define __MESA_FSMDEMO_DCL_H

// =================================================================================================
// -------------------------------------------------------------------------------------------------
// These declarations relate directly to the SimpleSal App app-fsmDemo, an absurdly abstract example
// App containing demonstrations of software, software organization, methods of building software.
// -------------------------------------------------------------------------------------------------
// Alongside the functionality of app-fsmDemo are ssTEA and ssUI features: Time, Events, Agency, UI.
// The incremental definition and implementation of a vocabulary for this specific mesa starts here.
// The vocabulary for a collection of projects based on the same Host OS and hardware is static.
//
// A SimpleSal App aware of multiple vocabularies? Yes, when one build instance creates a Linux user
// application that sends any file appearing in a directory, or, an Arduino gadget powers a GPIO pin
// when a motion is detected.  Both are based on the same idea: Time-based Events given Agency.
// -------------------------------------------------------------------------------------------------
#include "mesas_vocabulary.h"

// -------------------------------------------------------------------------------------------------
// This SimpleSal App runs on UnoR4Wifo or Due; the specific implementation for this build is picked.
// e.g., the configuration for Due does not include the code/data space used by the UnoR4 LED Matrix.
// It is possible to affect the build-visible content, the mesa's configuration, using a vocabulary.
// -------------------------------------------------------------------------------------------------
// Notice these are details of the App, being related to the shared resources on the mesa.
// -------------------------------------------------------------------------------------------------
#include "mesas_configuration.h"

// =================================================================================================
// -------------------------------------------------------------------------------------------------
// A configuration specification only has to include those OPTIN features that are used by the App.
// app-fsmDemo attempts to demonstrate every configurable feature of ssTEA and ssUI in one place.
// Letter-writers may complain that is should be "_OPTION_", but OPTIN is action so it works better.
// -------------------------------------------------------------------------------------------------
// The "mesa_min_*.h" files provide some minimal implementations of Random, Ascii and LED functions.
// Ascii and LED resources are used by ssTEA and ssUI but are not intended to be required by either.
// -------------------------------------------------------------------------------------------------
// Unlike Ascii and LED, random has two levels: random numbers Y/N; if Y, whose implementation?
// -------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------
// Separately from use of the LED and Ascii interfaces by the App, this mesa supports a Resource
// Ownership FSM for the LED and the Ascii interface.  The FSM for the LED indicates the passage of
// Time by blinking when a variety of signals occur.  The FSM for the Ascii interface supports a
// variety of buffering styles through a FIFO, in order to eliminate the App waiting for Serial.
// -------------------------------------------------------------------------------------------------
#ifdef MESA_MIN_OPTOUT_LED                  // opted out of mesa_min implementation
void        mesa_FSM_LEDflash_InitState ();
void        mesa_FSM_LEDflash_Req (void);
int         mesa_FSM_LEDflash_FSM (int  CurrPeriodCt);
#endif  //  is MESA_MIN_OPTOUT_LED          // opted out of mesa_min implementation

#ifdef MESA_MIN_OPTOUT_ASCII                // opted out of mesa_min implementation
void        mesa_FSM_AsciiEmit_InitState ();
void        mesa_FSM_AsciiEmit_ThisByte (Bits8_t thisByte);
int         mesa_FSM_AsciiEmit_FSM (int  CurrPeriodCt);
#endif // MESA_MIN_OPTOUT_ASCII            // opted out of mesa_min implementation

// =================================================================================================
// -------------------------------------------------------------------------------------------------
// [.\SimpleSal\ssDocs\mesa\Uno4wifi LED Matrix.note]      describes how it works and how it is used
// -------------------------------------------------------------------------------------------------
// support for the LED Matrix on a UnoR4wifi mesa is provided only to demonstrate ssTEA.  The entire
// app-fsmDemo's implementation of LED Matrix is jammed into this file and buried here because this
// is where the demos are that have an intricate, or not-abstract, view of the LED Matrix product.
// This is a minimalist implementation that can easily be supported by your well-written library.
// -------------------------------------------------------------------------------------------------
#ifdef MESA_ONEOF_mesa_IsUnoR4wifi
#define MESA_OPTIN_LED_Matrix_Support
#endif // MESA_ONEOF_mesa_IsUnoR4wifi

#ifdef MESA_OPTIN_LED_Matrix_Support

#define LEDM_LEDs_Rows  (8)
#define LEDM_LEDs_Cols  (12)

#define LEDM_LEDs_Row0  (0)
#define LEDM_LEDs_Row1  (1)
#define LEDM_LEDs_Row7  (7)

#define LEDM_LEDs_Col0  (0)
#define LEDM_LEDs_Col6  (6)         // the leftmost column where of the right group+cloctal starts
#define LEDM_LEDs_Col11 (11)

// #define LEDM_ONEOF_OrientationIsPortrait
#define LEDM_ONEOF_OrientationIsLandscape

#ifdef LEDM_ONEOF_OrientationIsPortrait
#define LED_RowCt       (LEDM_LEDs_Cols)
#define LED_ColCt       (LEDM_LEDs_Rows)
#endif  // LEDM_ONEOF_OrientationIsPortrait
//
#ifdef LEDM_ONEOF_OrientationIsLandscape
#define LED_RowCt       (LEDM_LEDs_Rows)
#define LED_ColCt       (LEDM_LEDs_Cols)
#endif  // LEDM_ONEOF_OrientationIsLandscape

#define LED_RowCt_max_i (LED_RowCt-1)
#define LED_ColCt_max_i (LED_ColCt-1)

// #define LEDM_OPTIN_debugLEDM
typedef unsigned int    octalNumber_t;

#define MAC_ID_0_Shape_Base (0x0301)
#define MAC_ID_1_Shape_Base (0x0306)

// Initialize the matrix with the appropriate data variables being given proper assumed values
void LEDM_matrixSetup           (void);

// modify the Matrix in a single location used to control one LED
void LEDM_matrixModify          (int LEDM_RowNumber, int LEDM_ColNumber, LogicalLEDValue_t LEDValue);

// modify the Matrix by having LEDM modify the Matrix in a complex set of locations
void LEDM_DigitalWrite_MrB      (int MrB_ID, Bits8_t MrB_IDs_PinNumber, LogicalPinValue_t PinValue);

// transfer the contents of the data about the Matrix LED states to the Matrix
void LEDM_matrixRefresh         (void);

// clear the local view of what the matrix contains and then emit that to the Matrix
void LEDM_matrixClearAll        (void);

// blastOut: demonstrate that each location for LEDs designed into the Matrix works AND display the
// contents in the form of shapes that people will remember AND announce arrivals and departures.
void LEDM_blastOut_HelloWorld   (void);
void LEDM_blastOut_GoodbyeWorld (void);

#endif // MESA_OPTIN_LED_Matrix_Support

#endif  // __MESA_FSMDEMO_DCL_H
