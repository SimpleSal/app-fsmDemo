/* -- SimpleSal: ssTEA, ssUI, and ssIO | (C) 2025, 2026 TruSoft Computing LLC |   All rights reserved. --
   This software is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
   This software is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
   ---------------------------------------------------------------------------------------------------
 *
 * app_mrb_ino.h        data and software definitions: application MrBlinky
 *
 * 2026/10/10   "fsmDemo.ino" Arduino software updated with released ssTEA/ssUI into "app-fsmDemo.ino".
 * 2025/08/06   special version appeared as the star of first videos published at SimpleSal substack.
 * 2024/05/27   created superset FSM Demo Application as basis for ssTEA and SimpleSal dev/test.
 **************************************************************************************************/
#ifndef _APP_MRB_INO_H
#define _APP_MRB_INO_H

// =================================================================================================
// -------------------------------------------------------------------------------------------------
// MrB App          A SimpleSal App instantiated within the app-fsmDemo infrastucture.
// -------------------------------------------------------------------------------------------------

// =================================================================================================
// -------------------------------------------------------------------------------------------------
// The MRB options are effective on the MrB Cycler, Chaser, and Driver build options.  MRB0 and MRB1
// options are effective on one of two instances of a MrB.  Most of the options are "one of" choices
// related to the use of app-fsmDemo as a test and demo platform for a variety of ssTEA interactions.
// -------------------------------------------------------------------------------------------------
// stop Time choice is between using a Time variable edit via ssUI or App software setting a value.
// stop Model choice is whether an Event is triggered that signals it is time to stop, or,
//      Events stop themselves when the value of a Time variable matches the current Time.
// -------------------------------------------------------------------------------------------------
#define APP_MRB_ONEOF_STOP_TIME_FROM_EVAPI
// #define APP_MRB_ONEOF_STOP_TIME_FROM_GLOBAL

// #define APP_MRB_ONEOF_STOP_MODEL_SIGNAL
#define APP_MRB_ONEOF_STOP_MODEL_SELFSTOP

#define APP_BPM_ONEOF_STOP_TIME_FROM_EVAPI
// #define APP_BPM_ONEOF_STOP_TIME_FROM_GLOBAL

// -------------------------------------------------------------------------------------------------
// These are related to the sequence of values used to drive the outputs while MrB is MrB'ing.
// -------------------------------------------------------------------------------------------------
// #define APP_MRB0_ONEOF_CYCLE_STYLE_DUTY
// #define APP_MRB0_ONEOF_CYCLE_STYLE_ROTATE_LEFT
#define APP_MRB0_ONEOF_CYCLE_STYLE_ROTATE_RIGHT
// #define APP_MRB0_ONEOF_CYCLE_STYLE_RANDOM

// #define APP_MRB1_ONEOF_CYCLE_STYLE_DUTY
// #define APP_MRB1_ONEOF_CYCLE_STYLE_ROTATE_LEFT
#define APP_MRB1_ONEOF_CYCLE_STYLE_ROTATE_RIGHT
// #define APP_MRB1_ONEOF_CYCLE_STYLE_RANDOM

// -------------------------------------------------------------------------------------------------
// The assumption is that every pattern is nonzero; the value being rotated is 0 means Init State.
// Each of the Cycle patterns is an FSM: the init state occurs when the value in the variable is 0.
// -------------------------------------------------------------------------------------------------
#ifdef APP_MRB0_ONEOF_CYCLE_STYLE_DUTY
#define     MRB0_PATTERN_INIT_VALUE              (0xCC)
#define     MRB0_PATTERN_NEXT_VALUE(thisValue)   ((thisValue!=0)? ~thisValue : MRB0_PATTERN_INIT_VALUE)
#endif  // APP_MRB0_ONEOF_CYCLE_STYLE_DUTY

#ifdef APP_MRB0_ONEOF_CYCLE_STYLE_ROTATE_LEFT
#define     MRB0_PATTERN_INIT_VALUE              (Bit0)
#define     MRB0_PATTERN_NEXT_VALUE(thisValue)   ((thisValue!=0)? Bits8Op_RotateLeft (thisValue) : MRB0_PATTERN_INIT_VALUE)
#endif  // APP_MRB0_ONEOF_CYCLE_STYLE_ROTATE_LEFT

#ifdef APP_MRB0_ONEOF_CYCLE_STYLE_ROTATE_RIGHT
#define     MRB0_PATTERN_INIT_VALUE              (Bit7)
#define     MRB0_PATTERN_NEXT_VALUE(thisValue)   ((thisValue!=0)? Bits8Op_RotateRight (thisValue) : MRB0_PATTERN_INIT_VALUE)
#endif  // APP_MRB0_ONEOF_CYCLE_STYLE_ROTATE_RIGHT

#ifdef APP_MRB1_ONEOF_CYCLE_STYLE_DUTY
#define     MRB1_PATTERN_INIT_VALUE              (0xCC)
#define     MRB1_PATTERN_NEXT_VALUE(thisValue)   ((thisValue!=0)? ~thisValue : MRB1_PATTERN_INIT_VALUE)
#endif  // APP_MRB1_ONEOF_CYCLE_STYLE_DUTY

#ifdef APP_MRB1_ONEOF_CYCLE_STYLE_ROTATE_LEFT
#define     MRB1_PATTERN_INIT_VALUE              (Bit0)
#define     MRB1_PATTERN_NEXT_VALUE(thisValue)   ((thisValue!=0)? Bits8Op_RotateLeft (thisValue) : MRB1_PATTERN_INIT_VALUE)
#endif  // APP_MRB1_ONEOF_CYCLE_STYLE_ROTATE_LEFT

#ifdef APP_MRB1_ONEOF_CYCLE_STYLE_ROTATE_RIGHT
#define     MRB1_PATTERN_INIT_VALUE              (Bit7)
#define     MRB1_PATTERN_NEXT_VALUE(thisValue)   ((thisValue!=0)? Bits8Op_RotateRight (thisValue) : MRB1_PATTERN_INIT_VALUE)
#endif  // APP_MRB1_ONEOF_CYCLE_STYLE_ROTATE_RIGHT

#ifdef APP_MRB0_ONEOF_CYCLE_STYLE_RANDOM
#define     MRB0_PATTERN_INIT_VALUE              mesa_Random_NextFrom256 ()
#define     MRB0_PATTERN_NEXT_VALUE(thisValue)   mesa_Random_NextFrom256 ()
#endif  // APP_MRB0_ONEOF_CYCLE_STYLE_RANDOM

#ifdef APP_MRB1_ONEOF_CYCLE_STYLE_RANDOM
#define     MRB1_PATTERN_INIT_VALUE              mesa_Random_NextFrom256 ()
#define     MRB1_PATTERN_NEXT_VALUE(thisValue)   mesa_Random_NextFrom256 ()
#endif  // APP_MRB1_ONEOF_CYCLE_STYLE_RANDOM

// -------------------------------------------------------------------------------------------------
// Both Time Events, Cycler and Chaser, signal when they are granted Agency.  When the two events
// occur at the same Time, the occur in some order, and they both may signal changes to their MrB.
// The App Event Data for the Driver has to remember both; a simple bit map works to collect all
// the Time Event signals, and then process the accumulated signal in the single instance of Driver.
// -------------------------------------------------------------------------------------------------
#define Cycler_MrB_Delta    (Bit7)
#define Chaser_MrB_Delta    (Bit6)
#define Nobody_MrB_Delta    (0)

// -------------------------------------------------------------------------------------------------
// The MrB algorithm needs to make the Driver act on every PIN (regardless of state) to force all
// all of the channels off or on at the beginning or the end of a sequence as a feature FOR the App.
// A bit ON in the "changed" signal only means the value changed; also signaled is a new bit value.
// -------------------------------------------------------------------------------------------------
#define     PatternsMrB_ChannelsTurnAllOff  (0x00)      // shorthand compresses all pins set to 0
#define     PatternsMrB_ChannelsTurnAllOn   (0xFF)      // shorthand compresses all pins set to 1
#define     PatternsMrB_ChannelsNoneChanged (0x00)
#define     PatternsMrB_ChannelsAllChanged  (0xFF)

// -------------------------------------------------------------------------------------------------
// when the map of channels shows a bit changed, the value it was changed to must be one of these.
// -------------------------------------------------------------------------------------------------
#define     mesaPin_ValueChannelOff         mesaPin_VALUE_LOW
#define     mesaPin_ValueChannelOn          mesaPin_VALUE_HIGH

// -------------------------------------------------------------------------------------------------
// Example of a Time variable with a constant value
// -------------------------------------------------------------------------------------------------
ReadOnly  ssT_Time_t  App_Time_cycle_Length =
{
//  valid positive zero    d   h   m   s  ms  us  ns  p    note: order MUST match ssTEA_data_dcl
    true, true,    false,  0,  0,  0, 10,  0, 0,
#ifdef SST_OPTIN_TIME_NSECS_TU
/* not blank, don't delete */                      0,
#endif  // SST_OPTIN_TIME_NSECS_TU
/* not blank, don't delete */                         0, 0, 0       // all TUs, momentU/L
};
ReadOnly  ssT_pTime_t  App_pTime_cycle_Length = &App_Time_cycle_Length;
// -------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------
// This array points to allocated MAC data, one for each pin that is considered a "channel".
// In order to virtualize a MAC, there must be a descriptor for the MAC in the "pMACs" array.
// -------------------------------------------------------------------------------------------------
ssIO_pMAC_t         MrB_pMACs[MrB_Ct_ALLOC][MrB_Channels_PerMrB_Alloc];

void                MrB_Driver_SetOutputMrB_ChannelsAll (void);
void                MrB_Driver_MAC_Config ();
void                MrB_Driver_MAC_DeConfig ();

PinNumber_t         MrB_Give_Pin_This_MrB_Channel (int MrB_i, int MrB_Channel_i);
void                MrB_Write_Pin_This_MrB_Channel (int MrB_i, int MrB_Channel_i, LogicalPinValue_t PinValue);
LogicalPinValue_t   MrB_Read_Pin_This_MrB_Channel  (int MrB_i, int MrB_Channel_i);

void                MrB_Set_Pin_Output_This_MrB_Channel (int MAC_ID, int MrB_Channel_i);


// =================================================================================================
// -------------------------------------------------------------------------------------------------
// App: control power 110AC power through 8-channel digital switch
//
// The mesa provides 8 digital I/O pins; each pin is connected (which is to say wired) to the
// daughterboard, which has 8 AC switches.  The daughterboard uses the mother's ground plane and 5V.
// The entire hardware contraption is in a box with the numbers 1 through 8 from the USER point of
// view, each number above 1 of 8 AC power cords that go through the contraption to be AC switched.
//
// The mesa's daughterboard-based 110AC power switching device is configured to enable or disable the
// power for a channel when the digital input it receives is HIGH (mesa voltage) or LOW (probably 0).
// This device is typically used in conjunction with flow-detection devices that shut down faults
// by controlling the channel in the power switch.  The transmitting signaler asserts LOW or HIGH
// to indicate it wants the sensor to shut OFF or ON power; each level is independently designed.
// -------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------
// First, a fundamental concept in Event-based software: When does "the data" exist and who sees it?
// Variables on the stack only exist while within the scope of the function they are defined within.
// static variables within a function are only visible within the scope of the same function.
// FSM algorithms are dependent on knowledge that exists and is accessible across function calls.
// When Events share information or communicate through signal carriers, static variables are used.
// -------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------
// An example of an application coexisting with SimpleSal, ssUI, and ssTEA.  Values entered into
// variables named by ssUI can be accessed: provide a name-to-variable mapping like time variables.
// -------------------------------------------------------------------------------------------------
// =================================================================================================
#define MrB_param_StartTimeFromNow      ssUI_gpTimeA
#define MrB_param_CycleLength           ssUI_gpTimeB
#define MrB_param_CyclerPeriod          ssUI_gpTimeC
#define MrB_param_ChaserPeriod          ssUI_gpTimeD

#define MrB_default_StartTimeFromNow    ssT_gpTime_10sc
// Example of a Time variable with a constant value
#define MrB_default_cycle_Length        App_pTime_cycle_Length
#define MrB_default_CyclerPeriod        ssT_gpTime_1sc
#define MrB_default_ChaserPeriod        ssT_gpTime_1sc

#define MrB_computed_StartTime          ssUI_gpTimeE
#define MrB_computed_StopTime           ssUI_gpTimeF

// =================================================================================================
// -------------------------------------------------------------------------------------------------
void App_MrB_Show_Choices (void)
{
    // ---------------------------------------------------------------------------------------------
    ss_uiOp_emit_qBanner (lfY, "How to pass values from ssUI's command line to 'mrb' event software:", lfY);
    ss_uiOp_emit_qBanner (lfY, "  assign nonzero values to ssUI variables using 'math TimeVar = value'", lfN);
    ss_uiOp_emit_qBanner (lfY, "   timea : Time from launch until the Cycler FSM starts agencying ", lfN);
    ss_uiOp_emit_qBanner (lfY, "   timeb : Time to keep the Cycler FSM in the agencying state     ", lfN);
    ss_uiOp_emit_qBanner (lfY, "   timec : Period for each Cycle while the Cycler FSM is agencying", lfN);
    ss_uiOp_emit_qBanner (lfY, "   timed : Period for each Cycle while the Chaser FSM is agencying", lfY);
}   // App_MrB_Show_Choices
// -------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------
ssE_EvResult_t App_MrB_useParamsOrDefaults (void)
{
    if (ssT_timeif_P1equalsZero (MrB_param_StartTimeFromNow))
    {
        ss_uiOp_emit_qBanner (lfN, "      using default Start Time From Now", lfY);
        ssT_mathOp_P1getsP2 (MrB_param_StartTimeFromNow, MrB_default_StartTimeFromNow);
    }
    else
    {
        ss_uiOp_emit_qBanner (lfN, "      using ssUI's <timea> value for Cycler's Start Time From Now", lfY);
    }

    if (ssT_timeif_P1equalsZero (MrB_param_CycleLength))
    {
        ss_uiOp_emit_qBanner (lfN, "      using default Cycler Active Time", lfY);
        ssT_mathOp_P1getsP2 (MrB_param_CycleLength, MrB_default_cycle_Length);
    }
    else
    {
        ss_uiOp_emit_qBanner (lfN, "      using ssUI's <timeb> value for Cycler's Active Time", lfY);
    }

    if (ssT_timeif_P1equalsZero (MrB_param_CyclerPeriod))
    {
        ss_uiOp_emit_qBanner (lfN, "      using default Cycler Period", lfY);
        ssT_mathOp_P1getsP2 (MrB_param_CyclerPeriod, MrB_default_CyclerPeriod);
    }
    else
    {
        ss_uiOp_emit_qBanner (lfN, "      using ssUI's <timec> value for Cycler period", lfY);
    }
    if (ssT_timeif_P1equalsZero (MrB_param_ChaserPeriod))
    {
        ss_uiOp_emit_qBanner (lfN, "      using default Chaser Period", lfY);
        ssT_mathOp_P1getsP2 (MrB_param_ChaserPeriod, MrB_default_CyclerPeriod);
    }
    else
    {
        ss_uiOp_emit_qBanner (lfN, "      using ssUI's <timed> value for Chaser initial period", lfY);
    }
    return (ssE_EvResult_OK_go);
}   //  App_MrB_useParamsOrDefaults
// -------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------
ssE_EvResult_t App_MrB_getParams (void)
{
    if (
        (ssT_timeif_P1equalsZero (MrB_param_StartTimeFromNow)) &&
        (ssT_timeif_P1equalsZero (MrB_param_CycleLength)) &&
        (ssT_timeif_P1equalsZero (MrB_param_CyclerPeriod)) &&
        (ssT_timeif_P1equalsZero (MrB_param_ChaserPeriod))
       )
    {   // if all params left at default, may be cry for help
        App_MrB_Show_Choices ();
    }

    if (App_MrB_useParamsOrDefaults () == ssE_EvResult_notOK_data)
    {
        return (ssE_EvResult_notOK_data);
    }

#ifdef APP_MRB_OPTIN_ANNOUNCE
    ss_uiOp_emit_qBanner (lfY, "ssUI Time variables compute and signal Time:", lfY);
    ss_uiOp_emit_qBanner (lfY, "    Current Time      (timecurr) ", lfN);
    ss_uiOp_Show_Time (ssT_gpTime_Current, lfN);
    ss_uiOp_emit_qBanner (lfY, "    Time until Start     (timea) ", lfN);
    ss_uiOp_Show_Time (MrB_param_StartTimeFromNow, lfN);
    ss_uiOp_emit_qBanner (lfY, "    Total Time Active    (timeb) ", lfN);
    ss_uiOp_Show_Time (MrB_param_CycleLength, lfN);
    ss_uiOp_emit_qBanner (lfY, "    Period for Cycler    (timec) ", lfN);
    ss_uiOp_Show_Time (MrB_param_CyclerPeriod, lfN);
    ss_uiOp_emit_qBanner (lfY, "    Period for Chaser    (timed) ", lfN);
    ss_uiOp_Show_Time (MrB_param_ChaserPeriod, lfY);
    ss_uiOp_emit_qBanner (lfY, "    Start Time math is (timecurr + timea)", lfN);
    ss_uiOp_emit_qBanner (lfY, "    Stop Time math is  (Start Time + timeb)", lfN);
#endif  // APP_MRB_OPTIN_ANNOUNCE

    // ---------------------------------------------------------------------------------------------
    ssT_mathOp_P1getsP2plusP3 (MrB_computed_StartTime, ssT_gpTime_Current, MrB_param_StartTimeFromNow);
    ssT_mathOp_P1getsP2plusP3 (MrB_computed_StopTime, MrB_computed_StartTime, MrB_param_CycleLength);

#ifdef APP_MRB_OPTIN_ANNOUNCE
    ss_uiOp_emit_qBanner (lfY, "      Computed Start Time         ", lfN);
    ss_uiOp_Show_Time (MrB_computed_StartTime, lfN);
    ss_uiOp_emit_qBanner (lfY, "      Computed Stop Time (timef)  ", lfN);
    ss_uiOp_Show_Time (MrB_computed_StopTime, lfN);
    mesa_uiOp_emit_newline ();
#endif  // APP_MRB_OPTIN_ANNOUNCE

    return (ssE_EvResult_OK_go);
}   // App_MrB_getParams
// -------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------
// Per-Event allocation of Event data by the App_EventData type definition and operative.
// An alternative to App_EventData method: defining public static ssHL variables in the sketch.
// -------------------------------------------------------------------------------------------------
#ifdef APP_MRB_ONEOF_STOP_TIME_FROM_GLOBAL
ssT_Time_t      gMrB_TimeToStop;
#endif // APP_MRB_ONEOF_STOP_TIME_FROM_GLOBAL

// -------------------------------------------------------------------------------------------------
// the 8 events used for bit toggling are allocated using the Channel number as an identifier.
// The application and the USER want to be able to think of them as channels 1-8.  To software and
// coder, channels are an abstraction within the software for interfacing to the physical mesa.
// The root abstraction is a "pattern", which is a sequence of 8-bit values driving the switches.
// USER channel 3 == software channel 3 == bit 3 in one value AND channel[3] in the database.
// -------------------------------------------------------------------------------------------------
// The Bit number is used to know
//   the digital Pin number on the mesa we're on, as currently assembled (through a lookup table);
//   the bit within each 8-bit value in the pattern that is the value for the specific digital Pin;
//   the index into the Event Api array for ssTEA data structures (data is @ [BitNumber]);
// -------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------
// The ssUI evApi interface will describe the Event information using this user-specified value.
// -------------------------------------------------------------------------------------------------
pAsciiA_t       pcCycler           = S("cycler")      ;
ssUI_Token_t    myCyclerTkn        = { pcCycler       , 6, NoBananas };
pAsciiA_t       pcChaser           = S("chaser")      ;
ssUI_Token_t    myChaserTkn        = { pcChaser       , 6, NoBananas };
//
// -------------------------------------------------------------------------------------------------
// This is an "example of launching an Event" using the Init State of the launching Event and ssUI.
//    The Init State occurs when the Event is granted Agency during ssE Event Agency data validation,
//          which can be triggered by a ssUI command: "evapi <name> apifsm validate"
// Within the activities of launching the Cycler are included a programmatic launch of the Driver
// and the Chaser.  When an error occurs during any of the launching, everything is left as it is.
// -------------------------------------------------------------------------------------------------
ssE_EvResult_t App_MrB_Launcher (ssE_pAboutEvVoid_t  param1, ssT_pTime_t pCurrentTime)
{
    ssUI_db_pEvApi_t    Cycler_pEvApi;
    ssUI_db_pEvApi_t    Chaser_pEvApi;
    ssUI_db_pEvApi_t    Driver_pEvApi;
    ssE_EvResult_t      EvResult;

    if (param1 != ssE_pAboutEvVoid)
    {
        // if not Null, error.  Cycler_Launcher only uses an Event Api to be granted Agency in Init.
        App_Announce_State ("App_MrB_Launcher in init state");
        App_Announce_State ("Flaw: a 'launcher' Event does not achieve Agency Api FSM <On_Running> state");
        return (ssE_EvResult_notOK_state);
    }
    // pointer to void signals this is the implementation of the "init" state for the event, which
    // in this implementation is an event that will not be fully launched, as it is the "launcher".

#ifdef APP_MRB_OPTIN_ANNOUNCE
    ss_uiOp_emit_Dash (10);
    ss_uiOp_emit_newline ();
#endif  // APP_MRB_OPTIN_ANNOUNCE
    App_Announce_State ("App_MrB_Launcher in init state");

    // ------------------------------------------------------------------------------------------
    // ------------------------------------------------------------------------------------------
    EvResult = App_MrB_getParams ();
    if (EvResult != ssE_EvResult_OK_go)
    {
        App_Announce_App ("Some problem with the parameters prevented Launch.");
        return (EvResult);
    }

    // ------------------------------------------------------------------------------------------
    // CHANGE Cycler_pEvApi to refer to specific EvApiData database entry for the Cycler.
    // Cycler and Chaser and driver are similar in configuration; only Cycler is commented.
    // ------------------------------------------------------------------------------------------
    Cycler_pEvApi = ssUI_dbOp_Lookup_UsingEvApi_i (MRB_CYCLER_EVAPI);
    Bld_ApiData_Debug_Access_func (Cycler_pEvApi);
    EvResult = App_MrB_Cycler_Launcher (Cycler_pEvApi);
    if (EvResult == ssE_EvResult_OK_go)
    {
        Chaser_pEvApi = ssUI_dbOp_Lookup_UsingEvApi_i (MRB_CHASER_EVAPI);
        Bld_ApiData_Debug_Access_func (Chaser_pEvApi);
        EvResult = App_MrB_Chaser_Launcher (Chaser_pEvApi);
    }
    if (EvResult != ssE_EvResult_OK_go)
    {
        // one or the other failed, return from the init state Agency Grant to MrB_Launcher
        return (EvResult);
    }   // Cycler or Chaser prep error

    // ------------------------------------------------------------------------------------------
    // launch the Cycler event, the Chaser Event, and the Driver Event, depending on each other.
    // ------------------------------------------------------------------------------------------
    EvResult = App_Launch_in_ssTEA (Cycler_pEvApi);
    if (EvResult == ssE_EvResult_OK_go)
    {
        EvResult = App_Launch_in_ssTEA (Chaser_pEvApi);
        if (EvResult == ssE_EvResult_OK_go)
        {
            // ------------------------------------------------------------------------------------------
            // Launch the output pin driver, which will change output state when Cycler or Chaser needs.
            // The driver receives signals (data type: agency granted) as a subordinate to Cycler/Chaser.
            // ------------------------------------------------------------------------------------------
            // -----------------------------------------------------------------------------------------
            // CHANGE Driver_pEvApi to refer to a specific EvApiData database entry allocated to this Event.
            // -----------------------------------------------------------------------------------------
            Driver_pEvApi = ssUI_dbOp_Lookup_UsingEvApi_i (MRB_DRIVER_EVAPI);
            Bld_ApiData_Debug_Access_func (Driver_pEvApi);

            EvResult = App_MrB_Driver_Launcher (Driver_pEvApi);
            if (EvResult == ssE_EvResult_OK_go)
            {
                // -----------------------------------------------------------------------------------------
                // launch the Driver event, it will be running when complete, within state of Time/Agency.
                // -----------------------------------------------------------------------------------------
                App_Announce_App ("Driver Launch in ssTEA: ");
                EvResult = App_Launch_in_ssTEA (Driver_pEvApi);
                if (EvResult == ssE_EvResult_OK_go)
                {
                    // The initial Run FSM state of an ssTEA "launched" event is Stopped.  Each time the FSM
                    // Event occurs, the Event assumes it occurred before, and left itself in the Paused state.
                    // An initial state condition of the FSM is the Event in the state after normal agencying.
                    App_Announce_App ("Driver Pause in ssTEA: ");
                    if (!ssUI_apiSignalOp_Emit (Driver_pEvApi, ss_ApiSigMsgValue_AgRunfsm_Pause))
                    {
                        App_Announce_ssTEA ("attempt to signal Driver Pause post-launch failed");
                        return (ssE_EvResult_notOK_API);
                    }   // Paused after Launch

                    App_Announce_App ("Driver Launcher returned OK; MrB Launcher completing");
                    return(ssE_EvResult_OK_go);
                }   // launch Driver succeeded
                else
                {
                    App_Announce_App (" MrB Launcher returned error; Driver Launcher incomplete");
                }   // launch Driver succeeded followed by launch driver failed
            }   // Ok_go on Driver LaunchPrep
        }   // Chaser Launch OK
    }   // Cycler Launch OK
    else
    {   // launch Cycler failed
        App_Announce_App (" MrB Launcher returned error; Cycler Launcher incomplete");
    }
    return (EvResult);
}   // App_MrB_Launcher  (redefined as EvFunc0 for function pointers in EvApi and ApiSig)
// -----------------------------------------------------------------------------------------
void App_MrB_Cycler_Init_EventData (ssUI_db_pEvApi_t Cycler_pEvApi)
{
    App_pEventData_t App_pEventData = &Cycler_pEvApi->App_EventData;

    App_pEventData->Cycler_DeltaChannels = 0;
    App_pEventData->Cycler_DeltaValues   = 0;
    App_pEventData->MrB_DeltaBitMap &= ~Cycler_MrB_Delta;

    App_pEventData->StopYourSelf = false;
    // App_pEventData->pStopTime is referenced only after the Cycler is agencying, after App sets

    // find: App_EventData_Style_Any "ssE_AboutEv's App_EventData carries a signal to an Event"
    // find: App_EventData_Style_Tricky "App_EventData signal carried points to anything in the App"
    App_pEventData->Sneak_pEvApiData = Cycler_pEvApi;
}   // App_MrB_Cycler_Init_EventData
// -------------------------------------------------------------------------------------------------
// plotter App  int Plot_Clock = 0;

ssE_EvResult_t App_MrB_Cycler (ssE_pAboutEvVoid_t param1, ssT_pTime_t pCurrentTime)
{
    ssUI_db_pEvApi_t    Cycler_pEvApi;
    ssUI_db_pEvApi_t    Driver_pEvApi;
    ssE_pAboutEv_t      ssE_pAboutEv;

    // this static variable exists across calls to App_MrB_Cycler but is also local to same.
    static Bits8_t      BitsLastEdge;
    // these stack variables only exist while this function executes
    Bits8_t             BitsThisEdge = 0;
    Bits8_t             BitsChanged = 0;
    App_pEventData_t    App_pEventData;         // all variables managing the state of this FSM

    // ---------------------------------------------------------------------------------------------
    // This is the init state; must consider the init state the Driver leaves the LEDs in: off.
    // The pattern FSMs must independently handle the INIT value and the NEXT value on first use.
    // For example, the rotate bit pattern assume a start value of zero means "start with default".
    // A random bit pattern generator usually has a "seed" or starting value, value 0 is possible.
    // ---------------------------------------------------------------------------------------------
    if (param1 == ssE_pAboutEvVoid)
    {
        App_Announce_State ("App_MrB_Cycler in init state");
        BitsLastEdge = MRB0_PATTERN_INIT_VALUE;
#ifdef APP_MRB_OPTIN_DEBUG_BITS_SEQUENCE
        ss_uiOp_emit_lbld_hex8 ("  Init: Bits Last Edge for first clock edge", BitsLastEdge);
        mesa_uiOp_emit_newline ();
#endif  // APP_MRB_OPTIN_DEBUG_BITS_SEQUENCE
        return (ssE_EvResult_OK_go);
    }   // init state for cycle on, the cycle is starting, the first instance is everybody OFF

    App_Announce_State ("App_MrB_Cycler in agency state");
// plotter App    switch (Plot_Clock & 7)
// plotter App    {
// plotter App        case 0: mesa_uiOp_emit_qAsciiA ("Clock:0"); break;
// plotter App        case 1: mesa_uiOp_emit_qAsciiA ("Clock:0"); break;
// plotter App        case 2: mesa_uiOp_emit_qAsciiA ("Clock:0"); break;
// plotter App        case 3: mesa_uiOp_emit_qAsciiA ("Clock:0"); break;
// plotter App        case 4: mesa_uiOp_emit_qAsciiA ("Clock:1"); break;
// plotter App        case 5: mesa_uiOp_emit_qAsciiA ("Clock:1"); break;
// plotter App        case 6: mesa_uiOp_emit_qAsciiA ("Clock:1"); break;
// plotter App        case 7: mesa_uiOp_emit_qAsciiA ("Clock:1"); break;
// plotter App    }   // switch
// plotter App    mesa_uiOp_emit_newline ();
// plotter App    Plot_Clock++;

    // ---------------------------------------------------------------------------------------------
    // This is NOT the init state.
    // ---------------------------------------------------------------------------------------------
    ssE_pAboutEv = (ssE_pAboutEv_t) param1;
    // find: App_EventData_Style_Any "ssE_AboutEv's App_EventData carries a signal to an Event"
    // find: App_EventData_Style_Tricky "App_EventData signal carried points to anything in the App"
    App_pEventData = (App_pEventData_t) ssE_pAboutEv->App_pEventData;
    Cycler_pEvApi = (ssUI_db_pEvApi_t) App_pEventData->Sneak_pEvApiData;
    Bld_ApiData_Debug_Access_func (Cycler_pEvApi);

    // ------------------------------------------------------------------------------------------
    // This is an example of run-time Event data communicated through "App_EventData".
    // ---------------------------------------------------------------------------------------------
    // When it is Time to Stop, the Cycler shuts off all pins; otherwise, a pin pattern is followed.
    // ---------------------------------------------------------------------------------------------
    Cycler_pEvApi->App_EventData.StopYourSelf =
                 App_isItTimeToStop (pCurrentTime, Cycler_pEvApi->App_EventData.pStopTime);

    if (Cycler_pEvApi->App_EventData.StopYourSelf)
    {
        // FSM reached termination condition, as decided elsewhere by someone who knows when to stop.
        // BitsChanged is set to non-zero to trigger a signal even when previous pattern was All Off.
        BitsThisEdge = PatternsMrB_ChannelsTurnAllOff;
        BitsChanged = PatternsMrB_ChannelsAllChanged;
#ifdef APP_MRB_OPTIN_DEBUG_BITS_SEQUENCE
        App_Announce_App ("  signal to Stop received, set all bits off  ");
#endif  // APP_MRB_OPTIN_DEBUG_BITS_SEQUENCE
    }   // the current Time has reached the Time to stop
    else
    {   // let's keep going
        // calculate/create valid values for the bit values and for the indicator the bit changed
        BitsThisEdge = MRB0_PATTERN_NEXT_VALUE (BitsLastEdge);
        BitsChanged = BitsLastEdge ^ BitsThisEdge;
    }   // let's keep going

// -------------------------------------------------------------------------------------------------
#ifdef APP_MRB_OPTIN_DEBUG_BITS_SEQUENCE
    ss_uiOp_emit_lbld_hex8 ("  this pattern this clock falling edge", BitsThisEdge);
    mesa_uiOp_emit_newline ();
#endif  // APP_MRB_OPTIN_DEBUG_BITS_SEQUENCE
    // the value in BitsLastEdge is no longer referenced, prepare for the next cycle
    BitsLastEdge = BitsThisEdge;

    // ---------------------------------------------------------------------------------------------
    // Cycler periodically sends a pattern to drive the bits to output by resuming the Driver Event.
    // ---------------------------------------------------------------------------------------------
    // this is implementation of the User FSM. The context ssE_pAboutEv no longer matters until return.
    // ---------------------------------------------------------------------------------------------
    // when it is TimeToStop, BitsChanged is forced to "all bits changed"; Stop signal is separate.
    // ---------------------------------------------------------------------------------------------
    if (BitsChanged != 0)
    {
#ifdef APP_MRB_OPTIN_DEBUG_BITS_SEQUENCE
        App_Announce_App ("  Cycler changed bit(s) triggers signal->Driver  ");
#endif  // APP_MRB_OPTIN_DEBUG_BITS_SEQUENCE
        Driver_pEvApi = ssUI_dbOp_Lookup_UsingEvApi_i (MRB_DRIVER_EVAPI);
        Bld_ApiData_Debug_Access_func (Driver_pEvApi);

        // ------------------------------------------------------------------------------------------
        // a tuple of calculated/forced values is sent to the driver: bit values and bits to write.
        // It doesn't matter that BitsThisEdge may contain bit values for bits that did not change.
        // The combination is necessary to implement the driver feature: write only changing bits.
        // Each state is one of four: no change or change, and, set bit that has changed to 0 or 1.
        // ------------------------------------------------------------------------------------------
        Driver_pEvApi->App_EventData.MrB_DeltaBitMap       |= Cycler_MrB_Delta;
        Driver_pEvApi->App_EventData.Cycler_DeltaValues     = BitsThisEdge;
        Driver_pEvApi->App_EventData.Cycler_DeltaChannels   = BitsChanged;

        Driver_pEvApi->App_EventData.StopYourSelf = Cycler_pEvApi->App_EventData.StopYourSelf;
        if (!ssUI_apiSignalOp_Emit (Driver_pEvApi, ss_ApiSigMsgValue_AgRunfsm_Resume))
        {
            App_Announce_ssTEA ("attempt to Signal Driver resume by Cycler failed");
            return (ssE_EvResult_notOK_state);
        }
    }   // at least one of the bits changed and/or it is time to stop

    if (Cycler_pEvApi->App_EventData.StopYourSelf)
    {
        App_MrB_Cycler_Init_EventData (Cycler_pEvApi);

    // -----------------------------------------------------------------------------------------
#ifdef APP_MRB_ONEOF_STOP_MODEL_SIGNAL
        App_Announce_ssTEA ("  Cycler Runfsm completes, signal Runfsm Stop to Self");
        if (!ssUI_apiSignalOp_Emit (Cycler_pEvApi, ss_ApiSigMsgValue_AgRunfsm_Stop))
        {
            App_Announce_ssTEA ("  Cycler attempt to signal Runfsm Stop to Self failed");
            return (ssE_EvResult_notOK_API);
        }

        // signal to continue normal processing, state has been changd in the database itself
        return (ssE_EvResult_OK_go);
#endif  // APP_MRB_ONEOF_STOP_MODEL_SIGNAL
    // -----------------------------------------------------------------------------------------
#ifdef  APP_MRB_ONEOF_STOP_MODEL_SELFSTOP
        App_Announce_ssTEA ("  Cycler Runfsm completes, signal Self Stop to ssA");
        return (ssE_EvResult_OK_stopMe);
#endif  // APP_MRB_ONEOF_STOP_MODEL_SELFSTOP
    }   // it's time to stop
    // -----------------------------------------------------------------------------------------

//  App_Announce_ssTEA ("  App's Cycler says good to go: to ssA");
    return(ssE_EvResult_OK_go);
}   // App_MrB_Cycler
// -----------------------------------------------------------------------------------------
void App_MrB_Chaser_Init_EventData (ssUI_db_pEvApi_t Chaser_pEvApi)
{
    App_pEventData_t App_pEventData = &Chaser_pEvApi->App_EventData;

    App_pEventData->Chaser_DeltaChannels = 0;
    App_pEventData->Chaser_DeltaValues   = 0;

    App_pEventData->MrB_DeltaBitMap &= ~Chaser_MrB_Delta;

    // find: App_EventData_Style_Any "ssE_AboutEv's App_EventData carries a signal to an Event"
    // find: App_EventData_Style_Tricky "App_EventData signal carried points to anything in the App"
    App_pEventData->Sneak_pEvApiData = Chaser_pEvApi;
}   // App_MrB_Chaser_Init_EventData
// -------------------------------------------------------------------------------------------------
ssE_EvResult_t App_MrB_Chaser (ssE_pAboutEvVoid_t param1, ssT_pTime_t pCurrentTime)
{
#ifdef APP_MRB_ONEOF_STOP_MODEL_SIGNAL
    ssUI_db_pEvApi_t    Chaser_pEvApi;
    ssE_pAboutEv_t      ssE_pAboutEv;
    App_pEventData_t    App_pEventData;
#endif // APP_MRB_ONEOF_STOP_MODEL_SIGNAL
    ssUI_db_pEvApi_t    Driver_pEvApi;
    ssTEA_pApiSig_t     pApiSig;

    // this static variable exists across calls to App_MrB_Chaser but is also local to same.
    static Bits8_t      BitsLastEdge;
    // these stack variables only exist while this function executes, the definition of local
    Bits8_t             BitsThisEdge = 0;
    Bits8_t             BitsChanged = 0;

    boolean             DriverNotThere = false;

    // ---------------------------------------------------------------------------------------------
    // This is the init state; must consider the init state the Driver leaves the LEDs in: off.
    // The pattern FSMs must independently handle the INIT value and the NEXT value on first use.
    // For example, the rotate bit pattern assume a start value of zero means "start with default".
    // A random bit pattern generator usually has a "seed" or starting value, value 0 is possible.
    // ---------------------------------------------------------------------------------------------
    if (param1 == ssE_pAboutEvVoid)
    {
        App_Announce_State ("App_MrB_Chaser in init state");
        BitsLastEdge = MRB1_PATTERN_INIT_VALUE;
#ifdef APP_MRB_OPTIN_DEBUG_BITS_SEQUENCE
        ss_uiOp_emit_lbld_hex8 ("  Init: Bits Last Edge for first clock edge", BitsLastEdge);
        ss_uiOp_emit_newline ();
#endif  // APP_MRB_OPTIN_DEBUG_BITS_SEQUENCE
        return (ssE_EvResult_OK_go);
    }   // init state for cycle on, the cycle is starting, the first instance is everybody OFF

    App_Announce_State ("App_MrB_Chaser in agency state");

    // calculate/create valid values for the bit values and for the indicator the bit changed
    BitsThisEdge = MRB1_PATTERN_NEXT_VALUE (BitsLastEdge);
    BitsChanged = BitsLastEdge ^ BitsThisEdge;

// -------------------------------------------------------------------------------------------------
#ifdef APP_MRB_OPTIN_DEBUG_BITS_SEQUENCE
    ss_uiOp_emit_lbld_hex8 ("  this pattern this clock falling edge", BitsThisEdge);
    mesa_uiOp_emit_newline ();
#endif  // APP_MRB_OPTIN_DEBUG_BITS_SEQUENCE
    // the value in BitsLastEdge is no longer referenced, prepare for the next cycle
    BitsLastEdge = BitsThisEdge;

    // ---------------------------------------------------------------------------------------------
    // Chaser periodically sends a pattern to drive the bits to output by resuming the Driver Event.
    // ---------------------------------------------------------------------------------------------
    // when it is TimeToStop, BitsChanged is forced to "all bits changed"; signal to Stop is separate.
    // ---------------------------------------------------------------------------------------------
    if (BitsChanged != 0)
    {
#ifdef APP_MRB_OPTIN_DEBUG_BITS_SEQUENCE
        App_Announce_App ("  Chaser changed bit(s) triggers signal->Driver  ");
#endif  // APP_MRB_OPTIN_DEBUG_BITS_SEQUENCE
        Driver_pEvApi = ssUI_dbOp_Lookup_UsingEvApi_i (MRB_DRIVER_EVAPI);
        Bld_ApiData_Debug_Access_func (Driver_pEvApi);
        pApiSig = Driver_pEvApi->ssTEA_pApiSig;     // want to look directly at that Event's State

        // ------------------------------------------------------------------------------------------
        // a tuple of calculated/forced values is sent to the driver: bit values and bits to write.
        // It doesn't matter that BitsThisEdge may contain bit values for bits that did not change.
        // The combination is necessary to implement the driver feature: write only changing bits.
        // Each state is one of four: no change or change, and, set bit that has changed to 0 or 1.
        // ------------------------------------------------------------------------------------------
        if (pApiSig->ssE_pAboutEv->Runfsm_state != ssA_Runfsm_State_stopped)
        {
            Driver_pEvApi->App_EventData.MrB_DeltaBitMap       |= Chaser_MrB_Delta;
            Driver_pEvApi->App_EventData.Chaser_DeltaValues     = BitsThisEdge;
            Driver_pEvApi->App_EventData.Chaser_DeltaChannels   = BitsChanged;
            if (pApiSig->ssE_pAboutEv->Runfsm_state == ssA_Runfsm_State_paused)
            {
                if (!ssUI_apiSignalOp_Emit (Driver_pEvApi, ss_ApiSigMsgValue_AgRunfsm_Resume))
                {
                    App_Announce_ssTEA ("attempt to signal Driver resume by Chaser failed");
                    return (ssE_EvResult_notOK_API);
                }
                DriverNotThere = false;         // do not change the App Event Data StopYourself
            }   // paused
        }
        else
        {
            DriverNotThere = true;
        }
    }   // at least one of the bits changed

    if (DriverNotThere)
    {
        // -----------------------------------------------------------------------------------------
#ifdef  APP_MRB_ONEOF_STOP_MODEL_SELFSTOP
        App_Announce_ssTEA ("  Chaser Runfsm completes, signal Self Stop to ssA");

        return (ssE_EvResult_OK_stopMe);
#endif  // APP_MRB_ONEOF_STOP_MODEL_SELFSTOP

#ifdef APP_MRB_ONEOF_STOP_MODEL_SIGNAL
        // -----------------------------------------------------------------------------------------
        // The Driver Event example refers to the pre-allocated Driver EvApi per the design, the
        // Chaser Event example sneaks the EvApi pointer into the Event Data to use while Agencying.
        // -----------------------------------------------------------------------------------------
        ssE_pAboutEv = (ssE_pAboutEv_t) param1;
        App_pEventData = (App_pEventData_t) ssE_pAboutEv->App_pEventData;
        Chaser_pEvApi = (ssUI_db_pEvApi_t) App_pEventData->Sneak_pEvApiData;
        Bld_ApiData_Debug_Access_func (Chaser_pEvApi);

        App_MrB_Chaser_Init_EventData (Chaser_pEvApi);

        App_Announce_App ("Finding Driver stopped, Chaser stops Self");

        App_Announce_ssTEA ("  Chaser Runfsm completes, signal Runfsm Stop to Self");
        if (!ssUI_apiSignalOp_Emit (Chaser_pEvApi, ss_ApiSigMsgValue_AgRunfsm_Stop))
        {
            App_Announce_ssTEA ("Chaser attempt to signal Runfsm Stop to Self failed");
            return (ssE_EvResult_notOK_API);
        }

        // signal to continue normal processing, state has been changd in the database itself
        return (ssE_EvResult_OK_go);
#endif  // APP_MRB_ONEOF_STOP_MODEL_SIGNAL
    }   // it's time to stop

    // -----------------------------------------------------------------------------------------
//  App_Announce_ssTEA ("  App's Chaser says good to go: to ssA");
    return(ssE_EvResult_OK_go);
}   // App_MrB_Chaser
// -----------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------
ssE_EvResult_t App_MrB_Cycler_Launcher (ssUI_db_pEvApi_t Cycler_pEvApi)
{
    ssTEA_pApiSig_t         pApiSig;                // create a local expression for an object within
    ssE_pAboutEv_t          ssE_pAboutEv;           // same

    ssE_EvResult_t          EvResult = ssE_EvResult_OK_go;

    Cycler_pEvApi->pEvApiNameTkn_App = &myCyclerTkn;

    // ----------------------------------------------------------------------------------------------
    // When the total Time to cycle is added to the initial OccursAt the math gives a Time result:
    // The Time at which to stop performing Cycles.  That value must be stored in memory that can be
    // referenced during initialization and that also can be referenced while agencying, or, "static".
    // app-fsmDemo contains example methods referring to static (always accessible) data by Events.
    // ----------------------------------------------------------------------------------------------
    // This is two things: a method for implementing a terminating condition for a Time-based FSM;
    //                          infinite choices: the name "FSM" does include "finite", it's not ISM.
    //    AND              a demonstration of an FSM accessing variables over Time while agencying.
    //                          two choices: use the EvApi's allocated time variable, or, use Global.
    // ----------------------------------------------------------------------------------------------
#ifdef APP_MRB_ONEOF_STOP_TIME_FROM_EVAPI
    // just point to data: in EvApi data, there is a pointer to a Time Variable dedicated/allocated
    Cycler_pEvApi->App_EventData.pStopTime = Cycler_pEvApi->pTimeVar;
#endif // APP_MRB_ONEOF_STOP_TIME_FROM_EVAPI
#ifdef APP_MRB_ONEOF_STOP_TIME_FROM_GLOBAL
    // just point to data: a global variable
    Cycler_pEvApi->App_EventData.pStopTime = &gMrB_TimeToStop;
#endif // APP_MRB_ONEOF_STOP_TIME_FROM_GLOBAL

    // ----------------------------------------------------------------------------------------------
    // Restarting the MrB FSMs from the ssUI requires setting the stop time as Cycler Launch does.
    // ----------------------------------------------------------------------------------------------
    ssT_mathOp_P1getsP2 (Cycler_pEvApi->App_EventData.pStopTime, MrB_computed_StopTime);

    // ------------------------------------------------------------------------------------------
    // change signal focus to the signal carrier that we just used to open the Api FSM.
    // The signal carrier pointed to by the ApiData must be allocated and its pointer stored in ApiData.
    // The ssE Event Agency data pointed to by the signal carrier is not referenced by the App
    // until AFTER the Api FSM On completes.
    // ------------------------------------------------------------------------------------------
    pApiSig = Cycler_pEvApi->ssTEA_pApiSig;
    pApiSig->ssE_pAboutEv = ssE_pAboutEvNull;

    // find: "ssE Event Agency Allocation Rules"   : allocation pending Api Fsm On
#ifdef SSE_OPTIN_ABOUTEVDB_TRUSTED_APP
#ifdef SSE_OPTIN_ABOUTEV_DEBUG
    ssE_EvDB_Show_ssE_pAboutEv (S("ApiSig->ssE_pAboutEv will be allocated by ssE *during* Api FSM On"), Cycler_pEvApi->ssTEA_pApiSig->ssE_pAboutEv);
#endif  // SSE_OPTIN_ABOUTEV_DEBUG
// -- trusted
#else   // not SSE_OPTIN_ABOUTEVDB_TRUSTED_APP
// -- not trusted
#ifdef SSE_OPTIN_ABOUTEV_DEBUG
    ssE_EvDB_Show_ssE_pAboutEv (S("ApiSig->ssE_pAboutEv will be allocated by App *after* Api FSM On"), Cycler_pEvApi->ssTEA_pApiSig->ssE_pAboutEv);
#endif  // SSE_OPTIN_ABOUTEV_DEBUG
#endif  // SSE_OPTIN_ABOUTEVDB_TRUSTED_APP

    // ------------------------------------------------------------------------------------------
    // ------------------------------------------------------------------------------------------
    App_Announce_App ("Cycler Launcher Api FSM on Request : ");

    if (!ssUI_apiSignalOp_Emit (Cycler_pEvApi, ss_ApiSigMsgValue_AgApifsm_On))
    {
        App_Announce_ssTEA ("Request to set Cycler Api FSM state to On failed.");
        return (ssE_EvResult_notOK_state);
    }

    // find: App_EventData_Style_Any "ssE_AboutEv's App_EventData carries a signal to an Event"
    // find: App_EventData_Style_Tricky "App_EventData signal carried points to anything in the App"
    Cycler_pEvApi->App_EventData.Sneak_pEvApiData = Cycler_pEvApi;

    // find: "ssE Event Agency Allocation Rules"     : when App not trusted, allocation after Api FSM On
#ifdef SSE_OPTIN_ABOUTEVDB_TRUSTED_APP       // ssE allocated, pointed to an ssE Event Agency data structure for this Event
#ifdef SSE_OPTIN_ABOUTEV_DEBUG
    ssE_EvDB_Show_ssE_pAboutEv (S("ApiSig->ssE_pAboutEv as assigned by ssE : "), pApiSig->ssE_pAboutEv);
#endif  // SSE_OPTIN_ABOUTEV_DEBUG
// -- trusted
#else   // not SSE_OPTIN_ABOUTEVDB_TRUSTED_APP
// -- not trusted
    pApiSig->ssE_pAboutEv = AboutEvDB_dbOp_Get ();      // pointer to our copy
#ifdef SSE_OPTIN_ABOUTEV_DEBUG
    ssE_EvDB_Show_ssE_pAboutEv (S("ApiSig->ssE_pAboutEv allocated by App : "), pApiSig->ssE_pAboutEv);
#endif  // SSE_OPTIN_ABOUTEV_DEBUG
#endif  // SSE_OPTIN_ABOUTEVDB_TRUSTED_APP

    // change focus to the Agency information about the event.
    ssE_pAboutEv = pApiSig->ssE_pAboutEv;           // trusted application uses what ssE allocated
    ssE_pAboutEv->Runfsm_EvType = ssE_EvType_Time;
    ssT_mathOp_P1getsP2 (&ssE_pAboutEv->OccursAt, MrB_computed_StartTime);
    ssE_pAboutEv->recurs = true;
    ssE_pAboutEv->recursFromTrue = true;
    ssT_mathOp_P1getsP2 (&ssE_pAboutEv->Period, MrB_param_CyclerPeriod);

    // find: App_EventData_Style_Any "ssE_AboutEv's App_EventData carries a signal to an Event"
    // find: App_EventData_Style_Basic  "App_EventData pointer in ssE_AboutEv -> EvApi App_EventData"
    // find: ssE_event_dcl.h to see why the pointer to the App Event Data is a pointer to void
    ssE_pAboutEv->App_pEventData = (EvEx_App_pEventData_t) &Cycler_pEvApi->App_EventData;

    // Set the function pointer in the signal for the API before trying to validate.
    // ssTEA note: this trick solves the problem of the timing of the About_Ev allocation based on
    // a trusted App for the Event DB, versus the timing of the About_Ev information being assigned.
    // This is a result of the design of ssUI supporting N Events inherently for the App to use.
    ssE_pAboutEv->pEvFunc = &App_MrB_Cycler;

    App_Announce_App ("Cycler Prepped");
    return (EvResult);
}  // App_MrB_Cycler_Launcher
// -----------------------------------------------------------------------------------------
ssE_EvResult_t App_MrB_Chaser_Launcher (ssUI_db_pEvApi_t Chaser_pEvApi)
{
    ssTEA_pApiSig_t         pApiSig;                // create a local expression for an object within
    ssE_pAboutEv_t          ssE_pAboutEv;           // same
    ssE_EvResult_t          EvResult = ssE_EvResult_OK_go;

    // ----------------------------------------------------------------------------------------------
    // The Chaser Event does not have loadtime parameters; it uses changes Period's value in real time.
    // Cycler and Chaser are similar in configuration, Chaser comments are trimmed back to a minimum.
    // ----------------------------------------------------------------------------------------------
    // Set values in App_EventData. for this App_Event if have any
    // ----------------------------------------------------------------------------------------------
    Chaser_pEvApi->pEvApiNameTkn_App = &myChaserTkn;

    pApiSig = Chaser_pEvApi->ssTEA_pApiSig;
    pApiSig->ssE_pAboutEv = ssE_pAboutEvNull;       // trusted configuration, ssE will allocate here

    App_Announce_ssTEA ("Chaser Launcher Api FSM on Request : ");
    if (!ssUI_apiSignalOp_Emit (Chaser_pEvApi, ss_ApiSigMsgValue_AgApifsm_On))
    {
        App_Announce_ssTEA ("Request to set Chaser Api FSM state to On failed.");
        return (ssE_EvResult_notOK_state);
    }
    Chaser_pEvApi->App_EventData.Sneak_pEvApiData = Chaser_pEvApi;

    ssE_pAboutEv = pApiSig->ssE_pAboutEv;       // trusted application uses ssE what allocated
    ssE_pAboutEv->Runfsm_EvType = ssE_EvType_Time;
    ssT_mathOp_P1getsP2plusP3 (&ssE_pAboutEv->OccursAt, MrB_computed_StartTime, ssT_gpTime_1sc);
    ssE_pAboutEv->recurs = true;
    ssE_pAboutEv->recursFromTrue = true;
    // find: ssE_event_dcl.h to see why the pointer to the App Event Data is a pointer to void
    ssE_pAboutEv->App_pEventData = (EvEx_App_pEventData_t) &Chaser_pEvApi->App_EventData;
    ssE_pAboutEv->pEvFunc = &App_MrB_Chaser;
    ssT_mathOp_P1getsP2 (&ssE_pAboutEv->Period, MrB_param_ChaserPeriod);

    App_Announce_App ("Chaser Prepped");
    return (EvResult);
}  // App_MrB_Chaser_Launcher
// =================================================================================================
pAsciiA_t       pcmyOutputDriver        = S("driver")      ;
ssUI_Token_t    myOutputDriverTkn       = { pcmyOutputDriver,     6, NoBananas };

// -------------------------------------------------------------------------------------------------
// This is an "example of launching" an Event using App software directly signaling ssTEA's API.
// The other "example of launching" an Event uses the Init State of the launching Event and ssUI.
// -------------------------------------------------------------------------------------------------
// The current state of the data structure(s) is (are) ignored; all data values for the FSM are set.
// -------------------------------------------------------------------------------------------------
void App_MrB_Driver_Init_EventData (ssUI_db_pEvApi_t Driver_pEvApi)
{
    App_pEventData_t App_pEventData = &Driver_pEvApi->App_EventData;

    App_pEventData->StopYourSelf = false;
    App_pEventData->MrB_DeltaBitMap = 0;

    // find: App_EventData_Style_Any "ssE_AboutEv's App_EventData carries a signal to an Event"
    // find: App_EventData_Style_Tricky "App_EventData signal carried points to anything in the App"
    App_pEventData->Sneak_pEvApiData = Driver_pEvApi;
}   // App_MrB_Driver_Init_EventData
// -------------------------------------------------------------------------------------------------
ssE_EvResult_t App_MrB_Driver_Launcher (ssUI_db_pEvApi_t Driver_pEvApi)
{
    ssTEA_pApiSig_t         pApiSig;                // create a local expression for an object within
    ssE_pAboutEv_t          ssE_pAboutEv;           // same
    ssE_EvResult_t          EvResult = ssE_EvResult_OK_go;

    // ------------------------------------------------------------------------------------------
    Driver_pEvApi->pEvApiNameTkn_App = &myOutputDriverTkn;

    // ------------------------------------------------------------------------------------------
    App_MrB_Driver_Init_EventData (Driver_pEvApi);

    // ------------------------------------------------------------------------------------------
    // set signal carrier focus to request the Event EvApi change from Off
    // ------------------------------------------------------------------------------------------
    pApiSig = Driver_pEvApi->ssTEA_pApiSig;
    pApiSig->ssE_pAboutEv = ssE_pAboutEvNull;

    App_Announce_App ("Driver Launcher Api FSM on Request : ");
    if (!ssUI_apiSignalOp_Emit (Driver_pEvApi, ss_ApiSigMsgValue_AgApifsm_On))
    {
        App_Announce_ssTEA ("Request to set Driver Api FSM state to On failed.");
        return (ssE_EvResult_notOK_state);
    }

    // change focus to the Agency information about the event, allocated by ssTEA or by the App
    ssE_pAboutEv = pApiSig->ssE_pAboutEv;
    ssE_pAboutEv->Runfsm_EvType = ssE_EvType_Data;
    // find: ssE_event_dcl.h to see why the pointer to the App Event Data is a pointer to void
    ssE_pAboutEv->App_pEventData = (EvEx_App_pEventData_t) &Driver_pEvApi->App_EventData;
    ssE_pAboutEv->pEvFunc = &App_MrB_Driver;

    App_Announce_App ("Driver Prepped");

    return (EvResult);
}  // App_MrB_Driver_Launcher
// -------------------------------------------------------------------------------------------------
void MrB_Driver_SetPinValueMrB_ChannelsAll (LogicalPinValue_t PinValue)
{
    int     MrB_i;
    int     MrB_Channel_i;
    // control all pin values AFTER configuring any virtualization
    for (MrB_i=0; MrB_i <= MrB_Ct_MAX_I; MrB_i++)
    {
        for (MrB_Channel_i=0; MrB_Channel_i <= MrB_Channels_Max_I; MrB_Channel_i++)
        {
            MrB_Write_Pin_This_MrB_Channel (MrB_i, MrB_Channel_i, PinValue);
        }   // for each channel
    }   // for each MrB
}   // MrB_Driver_SetPinValueMrB_ChannelsAll
// -------------------------------------------------------------------------------------------------
void MrB_Driver_WriteGroupBitsChanged (MAC_ID_t MAC_ID, Bits8_t BitsChanged, Bits8_t BitsThisEdge)
{
    // these variables are used to interact with the MrBlinky App's data
    int                 MrB_Channel_i;
    Bits8_t             CheckBit;

#ifdef APP_MRB_OPTIN_ANNOUNCE_PATTERN
    ss_uiOp_emit_lbld_hex8 ("    bits changed this clock edge", BitsChanged);
    mesa_uiOp_emit_newline ();
    ss_uiOp_emit_lbld_hex8 ("    bits output  this clock edge", BitsThisEdge);
    mesa_uiOp_emit_newline ();
#endif  // APP_MRB_OPTIN_ANNOUNCE_PATTERN

    CheckBit = Bit0;
    for (MrB_Channel_i=0; MrB_Channel_i <= MrB_Channels_Max_I; MrB_Channel_i++)
    {
        // only write to the output if the value existing at the output needs to be changed
        if (BitsChanged & CheckBit)
        {
            MrB_Write_Pin_This_MrB_Channel (MAC_ID,
                                            MrB_Channel_i,
                                            (BitsThisEdge & CheckBit) ?
                                                mesaPin_ValueChannelOn : mesaPin_ValueChannelOff);
        }
        // move the signal bit over the left to check the next bit
        CheckBit <<= 1;
    }   // for
}   // MrB_Driver_WriteGroupBitsChanged
// -------------------------------------------------------------------------------------------------
// A "Driver" within a computer system is generally software with responsibility for a Device.
// Drivers have extensive permissions and abilities, especially compared to application software.
// -------------------------------------------------------------------------------------------------
// the RunFsm of an Event, this Event, is resumed when the true output needs to be modified.
// The event is a data event, and will run as soon as ssTEA notices it is agencying; not using Time.
// The digital bit GPIOs must be configured elsewhere; per bit, there is no state knowledge.
// -------------------------------------------------------------------------------------------------
ssE_EvResult_t App_MrB_Driver (ssE_pAboutEvVoid_t param1, ssT_pTime_t pCurrentTime)
{
    // these stack variables only exist while this function executes, by definition local.
    // these variables are used to interact with the ssTEA data permanently maintained by ssTEA
    ssUI_db_pEvApi_t    Driver_pEvApi;
    ssE_pAboutEv_t      ssE_pAboutEv;      // valid after param1 is qualified as non-NULL
    App_pEventData_t    App_pEventData;
    Bits8_t             BitsThisEdge = 0;
    Bits8_t             BitsChanged = 0;

    // -----------------------------------------------------------------------------------------
    // Cycler and Driver share a common understanding that when Cycler starts signaling, the
    // state of the outputs will be "all-bits-off".  A delta from "all-bits-off" in the first
    // non-all-bits-off pattern in Cycler will cause a delta to be signaled to Driver.  Note
    // this means that if Cycler only finds "all-bits-off" in the pattern, no signal occurs.
    // -----------------------------------------------------------------------------------------
    if (param1 == pVoidEvParam1Null)
    {
        App_Announce_State ("App_MrB_Driver in init state");

        // -----------------------------------------------------------------------------------------
        // This App's design choice for an important design decision: is the initialization phase
        // of an FSM that controls hardware allowed to mess with the hardware during initialization?
        // A Host OS has a more complex design, with potentially multiple drivers sharing hardware.
        // This App's design creates the possibility of an initialization running without the actual
        // agencying Event running afterwards, leaving hardware in a condition or state of error.
        // Note also that the Driver only runs when bits change; Cycler knows the initialization
        // state (all off); if Cycler never finds a pattern that is not all off, it won't signal.
        // The Cycler's signal to the Driver to stop agencying may also a pattern that is all off.
        // This leaves the hardware in the condition it was set to when the initialization occurred.
        // -----------------------------------------------------------------------------------------
        // A different App's Driver solves this by moving the initialization to agencying, and then
        // every time the agencying Event occurs it asks "is this the first time this has Occured?".
        // The cost: a static variable (signal first time) and a "now?" comparison each agencying.
        // -----------------------------------------------------------------------------------------
        // virtualized definition created here is in immediate effect, handling "digitalWrite".
        // MrB application must wait to deal with Ps/Rs/Fs/Ss/Ss until ssIO has been initialized.
        // -----------------------------------------------------------------------------------------
        MrB_Driver_MAC_Config ();
        MrB_Driver_SetOutputMrB_ChannelsAll ();
        App_Announce_App ("all outputs configured, setting to off");
        MrB_Driver_SetPinValueMrB_ChannelsAll (mesaPin_ValueChannelOff);
        return (ssE_EvResult_OK_go);
    }   // Null param1

    // -----------------------------------------------------------------------------------------
    // -----------------------------------------------------------------------------------------
    App_Announce_State ("App_MrB_Driver in agency state");

    // -----------------------------------------------------------------------------------------
    // set focus (ssE_pAboutEv) to ssTEA Event Agency data, as initialized by Init state validate.
    // set focus (Driver_pEvApi) to refer to a specific App Event API data set for this Event.
    // -----------------------------------------------------------------------------------------
    // The ssE_AboutEv view (a pointer) does not know the form of the data; the EvApi view (a
    // different type of pointer) does know the form of the data pointed to.  The Event shares
    // the EvApi view, the App's view.  The typecast of one pointer to the other does not change
    // any value, the point of the trickery is both pointers to App Data point to the same place.
    // Faithful readers will appreciate that the comment does not say "the pointery of the trick".
    // -----------------------------------------------------------------------------------------
    ssE_pAboutEv = (ssE_pAboutEv_t) param1;
    // find: App_EventData_Style_Any "ssE_AboutEv's App_EventData carries a signal to an Event"
    // find: App_EventData_Style_Tricky "App_EventData signal carried points to anything in the App"
    App_pEventData = (App_pEventData_t) ssE_pAboutEv->App_pEventData;
    Driver_pEvApi = (ssUI_db_pEvApi_t) App_pEventData->Sneak_pEvApiData;    // pointer from Sneak EvApi
    Bld_ApiData_Debug_Access_func (Driver_pEvApi);

    // -----------------------------------------------------------------------------------------
    // Example access methods, as part of Event's view when granted Agency or of the App's view.
    // The bit map is used to allow multiple LED requests that occur at the same Time. Another
    // choice would be to do some sort of math to create the end result and just send the result.
    // -----------------------------------------------------------------------------------------
    if (App_pEventData->MrB_DeltaBitMap & Cycler_MrB_Delta)
    {
        BitsChanged = App_pEventData->Cycler_DeltaChannels;     // from ssE_AboutEv App Data
        BitsThisEdge = App_pEventData->Cycler_DeltaValues;      // from ssE_AboutEv App Data
        MrB_Driver_WriteGroupBitsChanged (MAC_ID_0, BitsChanged, BitsThisEdge);
    }
    if (App_pEventData->MrB_DeltaBitMap & Chaser_MrB_Delta)
    {
        BitsChanged = App_pEventData->Chaser_DeltaChannels;     // from ssE_AboutEv App Data
        BitsThisEdge = App_pEventData->Chaser_DeltaValues;      // from ssE_AboutEv App Data
        MrB_Driver_WriteGroupBitsChanged (MAC_ID_1, BitsChanged, BitsThisEdge);
    }
    App_pEventData->MrB_DeltaBitMap = Nobody_MrB_Delta;

    // -----------------------------------------------------------------------------------------
    if (App_pEventData->StopYourSelf)
    {
        App_Announce_App ("Driver asked to stop by Cycler");
        App_MrB_Driver_Init_EventData (Driver_pEvApi);
        App_Announce_App ("all outputs set to off");
        MrB_Driver_SetPinValueMrB_ChannelsAll (mesaPin_ValueChannelOff);

#ifdef MESA_OPTIN_LED_Matrix_Support
        LEDM_blastOut_GoodbyeWorld ();
        LEDM_matrixRefresh ();
#endif  // MESA_OPTIN_LED_Matrix_Support
        App_Announce_ssTEA ("Driver Runfsm completes, signal Self Stop to ssA");
        return (ssE_EvResult_OK_stopMe);
    }   // Cycler sent Signal to Stop

    return (ssE_EvResult_OK_go);
}   // App_MrB_Driver
// =================================================================================================
// -------------------------------------------------------------------------------------------------
// The path between a processor's pin and an AC switcher pin is a signal carrier with managed access.
// The manager of access to a signal carrier is the "Medium Access Controller", always "the MAC".
// In addition, introducing the concept of a MAC allows for a Pin medium that is actually Ethernet.
// And finally, the MAC abstraction extends to the definition of a MAC for a Virtual Pin RAM medium.
// -------------------------------------------------------------------------------------------------
void MrB_Driver_MAC_Config (void)
{
    int             MrB_i;
    int             MrB_Channel_i;
    ssIO_pMAC_t     pMAC;

    // for each AC switcher channel associated with a pin: allocate a descriptor used to contain
    // the definition; define the attributes this App wants (8 true output pins, direct or mapped).

    for (MrB_i=0; MrB_i <= MrB_Ct_MAX_I; MrB_i++)
    {
        for (MrB_Channel_i=0; MrB_Channel_i <= MrB_Channels_Max_I; MrB_Channel_i++)
        {
            MrB_pMACs[MrB_i][MrB_Channel_i] = ssIO_MAC_dbOp_Get ();
            if (MrB_pMACs[MrB_i][MrB_Channel_i] == ssIO_pMACNull)
            {
                App_Announce_App ("Pin pMAC allocation failed");
                continue;
            }

            pMAC = MrB_pMACs[MrB_i][MrB_Channel_i];            // changes no code, easier?

#ifdef SSIO_OPTIN_DEBUG_PINS_DEF
            ss_uiOp_emit_qAsciiA ("pMAC->Pin definition <");
#endif  //  SSIO_OPTIN_DEBUG_PINS_DEF
            ssIO_MAC_Pin_Default (pMAC, SSIO_MAC_IO_Ability_Output);    // good values for everyone
            pMAC->MAC_ID = ssIO_Build_MAC_ID (MACtype_Pin, MrB_i, MrB_Channel_i);
#ifdef SSIO_OPTIN_DEBUG_PINS_DEF
            mesa_uiOp_emit_Hex_32bits (pMAC->MAC_ID);
            ss_uiOp_emit_qAsciiA (">");
            ss_uiOp_emit_newline ();
#endif  //  SSIO_OPTIN_DEBUG_PINS_DEF
#ifdef MESA_ONEOF_mesa_IsMrBlinky
            pMAC->MappedState     = ssIO_Direct;      // using Host access methods
#endif  // MESA_ONEOF_mesa_IsMrBlinky
#ifdef MESA_ONEOF_mesa_IsUnoR4wifi
            pMAC->MappedState     = ssIO_Mapped;      // use emulated or mapped methods
#endif  // MESA_ONEOF_mesa_IsUnoR4wifi
        }   // for each MrB_Channel_i
    }   // for each MrB
//     ssIO_MAC_dbOp_dumpAll ();
}   // MrB_Driver_MAC_Config
// -------------------------------------------------------------------------------------------------
void MrB_Driver_MAC_DeConfig ()
{
}   // MrB_Driver_MAC_DeConfig
// -------------------------------------------------------------------------------------------------
void MrB_Driver_SetOutputMrB_ChannelsAll (void)
{
    int   MrB_Channel_i;
    // control all input/output AFTER configuring any virtualization
    for (MrB_Channel_i=0; MrB_Channel_i <= MrB_Channels_Max_I; MrB_Channel_i++)
    {
        MrB_Set_Pin_Output_This_MrB_Channel (MAC_ID_0, MrB_Channel_i);
        MrB_Set_Pin_Output_This_MrB_Channel (MAC_ID_1, MrB_Channel_i);
    }   // for
}   // MrB_Driver_SetOutputMrB_ChannelsAll
// -------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------
PinNumber_t MrB_Give_Pin_This_MrB_Channel (int MAC_ID, int MrB_Channel_i)
{
    if ((MrB_Channel_i >= 0) && (MrB_Channel_i <= MrB_Channels_Max_I))
    {
        return (PinsThisMrB[MAC_ID][MrB_Channel_i]);
    }
    else
    {
        return (7); // bug should be a pin number
    }
}   // MrB_Give_Pin_This_MrB_Channel
// -------------------------------------------------------------------------------------------------
void MrB_Write_Pin_This_MrB_Channel (int MAC_ID, int MrB_Channel_i, LogicalPinValue_t Value)
{
    if ((MrB_Channel_i >= 0) && (MrB_Channel_i <= MrB_Channels_Max_I))
    {
#ifdef SSIO_OPTIN_PINS
        ssIO_DigitalWrite (ssIO_Build_MAC_ID (MACtype_Pin, MAC_ID, MrB_Channel_i), Value);
#else   // not SSIO_OPTIN_PINS
        // This is a direct Arduino service call, specific to Arduino, trust that Pin is valid, true
        digitalWrite (MrB_Give_Pin_This_MrB_Channel (MAC_ID, MrB_Channel_i), Value);
#endif  // SSIO_OPTIN_PINS
    }
}   // MrB_Write_Pin_This_MrB_Channel
// -------------------------------------------------------------------------------------------------
void MrB_Set_Pin_Output_This_MrB_Channel (int MAC_ID, int MrB_Channel_i)
{
    if ((MrB_Channel_i >= 0) && (MrB_Channel_i <= MrB_Channels_Max_I))
    {
#ifdef SSIO_OPTIN_PINS
        ssIO_PinMode (ssIO_Build_MAC_ID (MACtype_Pin, MAC_ID, MrB_Channel_i), mesaPinMode_OUTPUT);
#else   // not SSIO_OPTIN_PINS
        // This is a direct Arduino service call, specific to Arduino, trust that Pin is valid, true
        pinMode (MrB_Give_Pin_This_MrB_Channel (MAC_ID, MrB_Channel_i), mesaPinMode_OUTPUT);
#endif  // SSIO_OPTIN_PINS
    }
}   // MrB_Set_Pin_Output_This_MrB_Channel
// -------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------
void MrB_Set_Pin_Input_This_MrB_Channel (int MAC_ID, int MrB_Channel_i)
{
    if ((MrB_Channel_i >= 0) && (MrB_Channel_i <= MrB_Channels_Max_I))
    {
#ifdef SSIO_OPTIN_PINS
    // build mac id
        ssIO_PinMode (ssIO_Build_MAC_ID (MACtype_Pin, MAC_ID, MrB_Channel_i), mesaPinMode_INPUT_NOPULLUP);
#else   // not SSIO_OPTIN_PINS
        // This is a direct Arduino service call, specific to Arduino.
        pinMode (MrB_Give_Pin_This_MrB_Channel (MAC_ID, MrB_Channel_i), mesaPinMode_INPUT_NOPULLUP);
#endif  // SSIO_OPTIN_PINS
    }
}   // MrB_Set_Pin_Input_This_MrB_Channel

#endif // _APP_MRB_INO_H

