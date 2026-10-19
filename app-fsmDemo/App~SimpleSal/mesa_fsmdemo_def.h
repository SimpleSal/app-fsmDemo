/* -- SimpleSal: ssTEA, ssUI, and ssIO | (C) 2025, 2026 TruSoft Computing LLC |  All rights reserved. --
   This software is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
   This software is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
   ---------------------------------------------------------------------------------------------------
 *
 * mesa_fsmDemo_def.h       data and software definitions: mesa-specific dependencies of SimpleSal
 *
 * 2026/10/10   "fsmDemo.ino" Arduino software updated with released ssTEA/ssUI into "app-fsmDemo.ino".
 * 2025/08/06   special version appeared as the star of first videos published at SimpleSal substack.
 * 2024/05/27   created superset FSM Demo Application as basis for ssTEA and SimpleSal dev/test.
 **************************************************************************************************/
#ifndef __MESA_FSMDEMO_DEF_H
#define __MESA_FSMDEMO_DEF_H

// =================================================================================================
// SimpleSal's suggestion: name EVERY mesa declare/define pair with specific reference to the mesa.
// Otherwise, the "mesa_*.h" you have may or may not match the mesa you are targeting in the build.
// The "mesa_min_*.start" pair resides in the SimpleSal directory; copy and rename to your choice.
// =================================================================================================

// =================================================================================================
// -------------------------------------------------------------------------------------------------
// To override the default definition in "mesas_min_def.h", #define the value before including file.
// -------------------------------------------------------------------------------------------------
#define     THIS_BUILDS_BAUD_RATE  (38400)

// #define MESA_ONEOF_CONTROL_INIT_BUILDTIME
#define MESA_ONEOF_CONTROL_INIT_RUNTIME

#include "mesas_min_def.h"

// =================================================================================================
#ifdef MESA_MIN_OPTOUT_ASCII           // opted out of mesa_min implementation
// -------------------------------------------------------------------------------------------------
// Every serial port emission of an Ascii byte is done using this function.  The ownership state is
// either the App or the SimpleSal.  The substate indicates WHEN the FSM will be given agency.
// -------------------------------------------------------------------------------------------------
// When the SimpleSal does not own the Ascii interface, everything sent to the Ascii interface
// goes straight to "Serial".  A different solution exists based on a change from sending all Ascii
// interface data through Serial, to sending all Ascii interface data through some other interface.
// The RO FSM manages turn-taking between multiple users of a resource; it does require that the
// Resources being managed (the Ascii interfaces) be the same- besides supporting Ascii data.
// -------------------------------------------------------------------------------------------------
// When the SimpleSal (ssTEA, ssUI, the App) own the Ascii interface, and the substate is:
// 1) delay: in loop, all bytes go to Serial, which may delay loop if the FIFO in Serial is full.
// 2) classic: all bytes go to FIFO, at N millisecond intervals bytes from mesa FIFO go to Serial.
// 2) pace: all bytes go to FIFO, at Agency Pace period intervals bytes from mesa FIFO go to Serial.
// 3) Agency: all bytes go to FIFO, at Time Event intervals bytes from mesa FIFO go to Serial.
// -------------------------------------------------------------------------------------------------
// In SimpleSal substates the mesa_FSM_AsciiEmit_FSM must be given Agency by ssT or ssA in order to
// make the FSM operate.  This creates an interdependency between the fsmDemo SimpleSal and ssTEA.
// -------------------------------------------------------------------------------------------------
void  mesa_uiOp_emit_1 (Bits8_t thisByte)
{
    if ((Ascii_ROs == RO_owner_ssUI) || (Ascii_ROs == RO_owner_Anybody))
    {
        if (Ascii_RO_active & RO_active_delay)
        {
            Serial.write (thisByte);
        }
        else
        {   // classic, pace, agency
            mesa_FSM_AsciiEmit_ThisByte (thisByte);
        }
    }
    else
    {   // App
        Serial.write (thisByte);
    }
}   // mesa_uiOp_emit_1+
// -------------------------------------------------------------------------------------------------
// The declaration for this function is in ssTEA declarations, but opt-out means must define it here.
// "Ascii Init Not Complete" must be hidden by whatever resolves to a boolean true or false.
// -------------------------------------------------------------------------------------------------
boolean mesa_uiOp_deviceInitialize (void)
{
    int try_ct = 0;

    mesa_Ascii_InitState (THIS_BUILDS_BAUD_RATE);
    while (mesa_Ascii_Init_NotComplete)
    {
        delay (100);        // Ascii Device Initialize takes some Time
        if (++try_ct > 10)
        {
            return (false);
        }
    }   // while

    return (true);
}   // mesa_uiOp_deviceInitialize
// -------------------------------------------------------------------------------------------------
// This is a signal carrier between a memory location and a device that will emit an Ascii value.
// The definition by the emitting device is so complex and mesa-specific that SimpleSal ignores it.
// Because this is a signal carrier, the values given to the device are leaving the building now.
// -------------------------------------------------------------------------------------------------
// The complex data type, just like the simple data types int or char, has operands and operations:
//  a) pAsciiA points to a contiguous array in any memory of values the emitting device accepts,
//  b) each location has a value that may be interpreted as an Ascii data or control value,
//  c) any location with the value of 0 (Ascii NUL) indicates that the end of data has been reached,
//  d) any pointer expression with a value of 0 (zero) indicates there is no array to reference.
// -------------------------------------------------------------------------------------------------
void mesa_uiOp_emit_pAsciiA (pAsciiA_t pAsciiA)
{
    if (pAsciiA != pAsciiNull)              // is the value of the pointer null?
    {
        while (*pAsciiA != Ascii_NUL)  // is the value the pointer points to NOT an Ascii NUL value?
        {
            mesa_uiOp_emit_1 (*pAsciiA);    // both are true. transfer from memory to device.
            pAsciiA++;                      // move the pointer along to the next value in memory
        }   // while non-nul characters
    }   // if pointer
}   // mesa_uiOp_emit_pAsciiA
// -------------------------------------------------------------------------------------------------
#define ASCII_BUFFER_ALLOC  (2000)
#define ASCII_BUFFER_MAX_I  (ASCII_BUFFER_ALLOC-1)

ssIO_fifo_t     AsciiEmit_fifo;
AsciiA_t        AsciiEmit_Buffer[ASCII_BUFFER_ALLOC];

// -------------------------------------------------------------------------------------------------
// This a dependency: mesa_FSM_ refers to ssIO; ssIO function finally emitting Ascii needs &AsciiEmit.
// -------------------------------------------------------------------------------------------------
pVoid_t    mesa_FSM_AsciiEmit_pfifo (void)
{
    return ((pVoid_t) &AsciiEmit_fifo);
}   // mesa_FSM_AsciiEmit_pfifo
// -------------------------------------------------------------------------------------------------
void mesa_FSM_AsciiEmit_InitState (void)        // called at init time
{
    ssIO_fifoOp_Init (TYPECAST_pfifo_From_void, IS_CIRCULAR_fifo, AsciiEmit_Buffer, ASCII_BUFFER_MAX_I);
}   // mesa_FSM_AsciiEmit_InitState
// -------------------------------------------------------------------------------------------------
void  mesa_FSM_AsciiEmit_ThisByte (Bits8_t thisByte)
{
    ssIO_fifoOp_Put_LastIn (TYPECAST_pfifo_From_void, thisByte);
}   // mesa_FSM_AsciiEmit_ThisByte
// -------------------------------------------------------------------------------------------------
//  Future:  unsigned int    testfifo = 0;
//  Future:  //  testfifo++;
//  Future:      if (testfifo > 1000)
//  Future:      {
//  Future:          for (testfifo=0; testfifo < 10; testfifo++)
//  Future:          {
//  Future:              ssIO_fifoOp_Put_LastIn (TYPECAST_pfifo_From_void, (Ascii_a+testfifo));
//  Future:          }
//  Future:          ssIO_fifoOp_Put_LastIn (TYPECAST_pfifo_From_void, Ascii_CR);
//  Future:          ssIO_fifoOp_Put_LastIn (TYPECAST_pfifo_From_void, Ascii_LF);
//  Future:          testfifo = 0;
//  Future:
//  Future:  //      Serial.write ((ssIO_fifoOp_State (TYPECAST_pfifo_From_void) == ssIO_fifo_State_NoData) ? 'Z' : 'E');
//  Future:      }   // one second sent by
//  Future:
// -------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------
// This FSM is granted agency at 1ms boundaries of Time to service the FIFO being filled in reality.
// When the Ascii Resource State is ssTEA_App and the substate is not delay, the granting of Agency
// occurs within Timekeeping model: loop counts milliseconds; at ssTEA Agency Pace; as ssTEA Event.
// -------------------------------------------------------------------------------------------------
int  mesa_FSM_AsciiEmit_FSM (int  CurrPeriodCt)
{
    if (ssIO_fifoOp_State (TYPECAST_pfifo_From_void) != ssIO_fifo_State_NoData)
    {
        Serial.write (ssIO_fifoOp_Get_FirstIn (TYPECAST_pfifo_From_void));
    }   // if not empty
    CurrPeriodCt = 0;
    return (CurrPeriodCt);
}   // mesa_FSM_AsciiEmit_FSM
#endif // MESA_MIN_OPTOUT_ASCII         // opted out of mesa_min implementation
// =================================================================================================
#ifdef MESA_MIN_OPTOUT_LED              // opted out of mesa_min implementation
// -------------------------------------------------------------------------------------------------
// [.\SimpleSal\ssDocs\mesa\FSMs- Resource Ownership.note]   a simple FSM for co-existence
// -------------------------------------------------------------------------------------------------
// The LED here is the Arduino LED_BUILTIN, which may be a standard device across all Arduino Mesas.
// -------------------------------------------------------------------------------------------------
// digitalWrite, LED_BUILTIN, LOW and HIGH are defined by the Host OS built in by the application.
// -------------------------------------------------------------------------------------------------
void mesa_uiOp_LEDon (void)
{
    mesa_LED_currentState = ON_STATE;
    digitalWrite (LED_BUILTIN, mesaPin_VALUE_HIGH);
}   // mesa_uiOp_LEDon
// -------------------------------------------------------------------------------------------------
void mesa_uiOp_LEDoff (void)
{
    mesa_LED_currentState = OFF_STATE;
    digitalWrite (LED_BUILTIN, mesaPin_VALUE_LOW);
}   // mesa_uiOp_LEDoff
// -------------------------------------------------------------------------------------------------
// The mesa provides a mechanism to flash the LED_BUILT LED.  "flash" means "turn on, then off for
// a period of Time visible to a human".  The FSM supports a variety of flash management models, for
// calculating the period of Time between flashes, as well as for the period of Time the LED is on.
// -------------------------------------------------------------------------------------------------
#define FlashPending_MaxCount   (10)
int mesa_FlashPending_Count = 0;

// -------------------------------------------------------------------------------------------------
// While the FSM is operating, a flash of the LED is accomplished by calling this function.  Done.
// -------------------------------------------------------------------------------------------------
void mesa_FSM_LEDflash_Req (void)
{
    if (mesa_FlashPending_Count < FlashPending_MaxCount)
    {
        mesa_FlashPending_Count++;
    }
}   // mesa_FSM_LEDflash_Req
// -------------------------------------------------------------------------------------------------
// Changes to the state of the LED occur at period boundaries: ON to OFF, OFF to ON.
// -------------------------------------------------------------------------------------------------
#define PATTERN_NOT_ACTIVE  (0)
#define PATTERN_ACTIVE_ON   (1)
#define PATTERN_ACTIVE_OFF  (2)
int     pattern_state;

#define FlashPeriod_On      (1)
#define FlashPeriod_Off     (99)
// -------------------------------------------------------------------------------------------------
void mesa_FSM_LEDflash_InitState (void)        // called at init time
{
    pattern_state = PATTERN_NOT_ACTIVE;
    mesa_FlashPending_Count = 0;
}   // mesa_FSM_LEDflash_InitState
// -------------------------------------------------------------------------------------------------
// This FSM is granted agency by the "loop" function each time a Time period occurs or passes.  The
// FSM does not care about the length of the Time period, the signal to the FSM is "period occurred".
// Each occurence of a period causes the state machine to progress through the sequence of actions.
// -------------------------------------------------------------------------------------------------
int  mesa_FSM_LEDflash_FSM (int  CurrPeriodCt)
{
    if (mesa_FlashPending_Count > 0)
    {
        // the currently Active Flash counts as Pending until cycle completes and FSM is NOT_ACTIVE
        switch (pattern_state)
        {
            // NOT_ACTIVE with a Pending Flash? Activate the Flash!  Being OFF first makes the flash;
            // if immediately ON when the LED had just been turned OFF from ON, no gap detectable.
            case PATTERN_NOT_ACTIVE :
                mesa_uiOp_LEDoff ();
                pattern_state = PATTERN_ACTIVE_OFF;
                CurrPeriodCt = 0;
                break;
            case PATTERN_ACTIVE_OFF :
                // take no action until time expires
                if (CurrPeriodCt > FlashPeriod_Off)
                {   // time expired, take action
                    mesa_uiOp_LEDon ();
                    pattern_state = PATTERN_ACTIVE_ON;
                    CurrPeriodCt = 0;
                }
                // if within the pattern period, do nothing to the led
                break;
            case PATTERN_ACTIVE_ON :
                // take no action until time expires
                if (CurrPeriodCt > FlashPeriod_On)
                {   // time expired, take action
                    mesa_uiOp_LEDoff ();
                    // is there a bug somewhere where flashes goes negative?
                    mesa_FlashPending_Count = (mesa_FlashPending_Count > 0) ?
                                                    (mesa_FlashPending_Count - 1) : 0;
                    pattern_state = PATTERN_NOT_ACTIVE;
                    CurrPeriodCt = 0;       // not necessary, canonical is clear on state change
                }
                // if within the pattern period, do nothing to the led
                break;
        }   // switch
    }   // anything to worry about?

    return (CurrPeriodCt);
}   // mesa_FSM_LEDflash_FSM

#endif  //  is MESA_MIN_OPTOUT_LED          // opted out of mesa_min implementation

// =================================================================================================
// -------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------
// MrBlinky's mesa has several specific layers of mapping to create the "channels" abstraction.
// The App is ignorant of the mapping; it refers to App channels 0 through 7.
// The transformation of a channel number to a pin number abstracts the pin mapping for the App.
// The mesa supports two independent sets of Channels: MAC_ID 0 and MAC_ID 1.  UnoR4 has two distinct
// definitions of a group on the LED Matrix; the DUE mesa has two OVERLAPPING groups on 8 pins.
// -------------------------------------------------------------------------------------------------
#define MrB_Channels_PerMrB_Alloc   (8)
#define MrB_Channels_Max_I          (MrB_Channels_PerMrB_Alloc-1)

#define MrB_Ct_ALLOC (2)
#define MrB_Ct_MAX_I (MrB_Ct_ALLOC-1)

#ifdef  MESA_ONEOF_mesa_IsMrBlinky
// -------------------------------------------------------------------------------------------------
// MrBlinky's DUE build uses the channels here to map a DUE true pin to an AC power supply switch.
// The entire platform contains an 8-channel switcher and 8 DUE board pins wired to the switcher.
// The original MrBlinky mesa supported one group of pins, direct and true. No sets of Shapes.
// -------------------------------------------------------------------------------------------------
// A "group of Pins" means "8 Pins, each controlling a switch via the Pin, or, an LED via the Pin".
// The LED Matrix available with the UnoR4 introduces the concept of multiple MrBlinkys as groups.
// In this App's use of this idea, there are two Mr Blinkys, each of them a group with 8 Pins.
// -------------------------------------------------------------------------------------------------
// This was a solution to poor soldering skills. The mapping is specific to a MrBlinky, still alive.
// If the use of true pin 41 becomes a problem, the User can move the wire to another true pin on
// the mesa. Then, in the position that says "41" now, change the value to the new true pin number.
// No other references in the App to the pin using this abstraction would need modification.
// -------------------------------------------------------------------------------------------------
// The mesa abstraction supporting multiple "MAC_ID" instances on the UnoR4 pushed changes back into
// the original MrBlinky implementation.  8 channels per MrBlinky instance map to 8 Pins or LEDs;
// the abstraction uses two MrBlinkys, is there any reason both MrBlinkys can't use the same truth?
// "PinsThisMrB" makes the connection between the true (direct or mapped) Pin and what MrB calls it.
// -------------------------------------------------------------------------------------------------
// find: defining UnoR4 LEDs as Pins for the map of a BitN to a Matrix column number within a group.
// find: Pin order versus Bit order resolved by mapping
// -------------------------------------------------------------------------------------------------
// This "PinsThisMrB" makes the connection between the true direct DUE Pin and a MAC_ID channel.
// -------------------------------------------------------------------------------------------------
PinNumber_t     PinsThisMrB[MrB_Ct_ALLOC][MrB_Channels_PerMrB_Alloc] =
    {
        { 43, 45, 39, 41, 33, 29, 35, 31 },
        { 43, 45, 39, 41, 33, 29, 35, 31 }
    };

#endif  //  not MESA_ONEOF_mesa_IsMrBlinky

#ifdef  MESA_ONEOF_mesa_IsUnoR4wifi
// =================================================================================================
// -------------------------------------------------------------------------------------------------
// The process of adopting the MrBlinky software to the UnoR4 platform was constrained by the choice
// that MrBlinky should not change from the original job. MrBlinky controls 8 channels; On or Off.
// -------------------------------------------------------------------------------------------------
// Meaningful grouping of LEDs on the Matrix are useful, e.g., all 8 channels of MrBlinky's pins,
// mapped through ssIO pins onto 8 LEDs on the Matrix, in a line: an UnoR4 has two MrBlinkys.
// -------------------------------------------------------------------------------------------------
// Why not use the LED matrix software completely for bit and image manipulation?  1) SimpleSal is
// focused on the concepts of the Data Link Layer, the Matrix software is the Presentation Layer;
// 2) a simple Presentation Layer is required, within SimpleSal's philosophy of "you can see it"
// the best solution is a few direct actions rather than creating a dependency on a technology.
// -------------------------------------------------------------------------------------------------
#ifdef  MESA_OPTIN_LED_Matrix_Support
#include "mesa_unor4wifi_LEDM.h"
#endif  // MESA_OPTIN_LED_Matrix_Support
// =================================================================================================
// -------------------------------------------------------------------------------------------------
// MrBlinky's UnoR4 build uses "mapped" pin numbers to map a Pin number to a set of 8 LED bits as a
// group.  Each bit in the group represents an AC power supply channel switcher in the original MrB.
// Because the LED Matrix supports many potential groups, the Mr Blinky App has added a second Mrb.
// The MAC_ID selection is encoded in the MAC identifier so the same Pin Numbers for both is managed.
// -------------------------------------------------------------------------------------------------
// This requirement creates a requirement that BitN be mapped as a Channel to Pin/LED at [row,7-N].
// This mapping occurs through the Mesa Pin number arrays: for Mr Blinky to match physical wires;
// for UnoR4 to match the different numbering order described above.  Each Pin/LED number in the
// 8-element Pin/LED array maps to a BitN number in an 8-bit value.  P/L [0] describes the P/L
// for Bit7, P/L [1] is the P/L for Bit6, P/L [2] for Bit5, ..., P/L [7] for Bit0.  This same
// comment can be found in the Mesa where the mapping arrays are created specific to each mesa.
// -------------------------------------------------------------------------------------------------
// This "PinsThisMrB" makes the connection between the true mapped Pin and a MAC_ID channel.
// find: Pin order versus Bit order resolved by mapping
// -------------------------------------------------------------------------------------------------
PinNumber_t     PinsThisMrB[MrB_Ct_ALLOC][MrB_Channels_PerMrB_Alloc] =
    {   // bit0 is at LED7, bit1 is at LED6, bit2 is at LED5, etc.
        { 0x00,  0x01,  0x02,  0x03,  0x04,  0x05,  0x06,  0x07 },
        { 0x07,  0x06,  0x05,  0x04,  0x03,  0x02,  0x01,  0x00 }
    };


#endif  //  MESA_ONEOF_mesa_IsUnoR4wifi
// =================================================================================================

#endif  // __MESA_FSMDEMO_DEF_H