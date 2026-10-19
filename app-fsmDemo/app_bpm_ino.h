/* -- SimpleSal: ssTEA, ssUI, and ssIO | (C) 2025, 2026 TruSoft Computing LLC |   All rights reserved. --
   This software is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
   This software is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
   ---------------------------------------------------------------------------------------------------
 *
 * app_bpm_ino.h        configure and activate the Beats Per Minute FSM
 *
 * 2026/10/10   "fsmDemo.ino" Arduino software updated with released ssTEA/ssUI into "app-fsmDemo.ino".
 * 2025/08/06   special version appeared as the star of first videos published at SimpleSal substack.
 * 2024/05/27   created superset FSM Demo Application as basis for ssTEA and SimpleSal dev/test.
 **************************************************************************************************/
#ifndef __APP_BPM_INO_H
#define __APP_BPM_INO_H

// =================================================================================================
// -------------------------------------------------------------------------------------------------
// What's the BPM? App          A SimpleSal App instantiated within the app-fsmDemo SimpleSal App.
// -------------------------------------------------------------------------------------------------
// This App places the algorithm to decide the rate that events should occur into the User's AI.
// The outcome desired from use of the App is see the relationship between Time and a Beat, such as
// increasing the time between beats (longer beat period) for a slower beat and decreasing the time
// (shorter beat period) for a faster beat. The User adjusts a Chaser BPM to match the Cycler's BPM.
// -------------------------------------------------------------------------------------------------
// An FSM, human/computer, asks/answers "should fewer or more events occur over the next minute?".
// -------------------------------------------------------------------------------------------------
// The Events that are occuring are a result of Time measured in milliseconds reaching a set Time.
// The Event signals that it occured by causing the LED Matrix display to update a "cloctal".
// -------------------------------------------------------------------------------------------------
// The Time between beats N and N+1 is the beat period BP: ((60*1000)/BPM) == BP time units per beat.
// When the Time while N beats are counted is one minute, the value is BPM, "beats per minute".
// -------------------------------------------------------------------------------------------------
// Each 1-second period within each 60-second period contains 1000 1-millisecond periods.  Really.
// "10 milliseconds" describes how long it takes 10 Time Units to occur, when each Time Unit is
// 1/1000 of a second long.  Also "10ms", "10 millis".  Other Time Units are seconds and plancks.
// Milliseconds is commonly used by devices that interact with humans because humans are very slow.
// -------------------------------------------------------------------------------------------------
// The interesting math is: both rates use the (60*1000) number representing all the millis in 60.
// Dividing the total millis count by any beat count (BPM) gives a result for millis per beat (BP).
// Dividing the same millis count by any millis count (BP) gives a result for the beat count (BPM).
// -------------------------------------------------------------------------------------------------

// =================================================================================================
// -------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------
ReadOnly pAsciiA_t  pcReady     = S("ready");
ReadOnly pAsciiA_t  pcSet       = S("set");
ReadOnly pAsciiA_t  pcGo        = S("go");
ReadOnly pAsciiA_t  pcShow      = S("show");

ReadOnly pAsciiA_t  pcMore      = S("more");
ReadOnly pAsciiA_t  pcLess      = S("less");

ReadOnly pAsciiA_t  pcDelta     = S("delta");
ReadOnly pAsciiA_t  pcSmaller   = S("smaller");
ReadOnly pAsciiA_t  pcBigger    = S("bigger");

ReadOnly pAsciiA_t  pcUReady    = S("r");
ReadOnly pAsciiA_t  pcUSet      = S("set");
ReadOnly pAsciiA_t  pcUGo       = S("g");
ReadOnly pAsciiA_t  pcUShow     = S("sh");

ReadOnly pAsciiA_t  pcUMore     = S("m");
ReadOnly pAsciiA_t  pcULess     = S("l");

ReadOnly pAsciiA_t  pcUDelta    = S("d");
ReadOnly pAsciiA_t  pcUSmaller  = S("s");
ReadOnly pAsciiA_t  pcUBigger   = S("b");

int     gCycler_BPM;
int     gChaser_BPM;

// -------------------------------------------------------------------------------------------------
// signed integer math is easy when types such as "int" can be used for variables: "total millis".
// signed integer math of "S seconds and M millis" is difficult and why TIME_IS_AN_ARROW in reality.
// -------------------------------------------------------------------------------------------------
// #define APP_OPTIN_SHOW_YOUR_WORK

// -------------------------------------------------------------------------------------------------
// The developer who masters the human AI algorithm may come here and modify the search algorithm.
// Init State occurs to establish "deltas[deltas_i]"; values for the change rate moving up and down.
// -------------------------------------------------------------------------------------------------
#define APP_ONEOF_DELTA_MAP_FIBONNACI
// #define APP_ONEOF_DELTA_MAP_INCREMENTAL
// #define APP_ONEOF_DELTA_MAP_8BITS

#ifdef  APP_ONEOF_DELTA_MAP_FIBONNACI
int     deltas[] = { 1, 2, 3, 5,  8, 13, 21,  44,  63, 107 };
#define DeltaInitState  (1)
#define DeltaInit_i     (0)
#endif  // APP_ONEOF_DELTA_MAP_FIBONNACI
#ifdef  APP_ONEOF_DELTA_MAP_INCREMENTAL
int     deltas[] = { 1, 2, 3, 4,  5,  6,  7,   8,   9,  10 };
#define DeltaInitState  (1)
#define DeltaInit_i     (0)
#endif  // APP_ONEOF_DELTA_MAP_INCREMENTAL
#ifdef  APP_ONEOF_DELTA_MAP_8BITS
int     deltas[] = { 1, 2, 4, 8, 16, 32, 64, 128 };         // N does not need to match others
#define DeltaInitState  (16)
#define DeltaInit_i     (4)
#endif  // APP_ONEOF_DELTA_MAP_8BITS

int     deltas_i = DeltaInit_i;
#define ComputeDeltasNArray_Ct      ((int) (sizeof(deltas) / sizeof(int)))
#define deltas_i_max_i              (ComputeDeltasNArray_Ct-1)
#define DELTA_MULTIPLIER   (10)

// -------------------------------------------------------------------------------------------------
// ssTEA provides a data type which can be used to describe a Time: day:hour:minute:second:ms:us:ns.
// -------------------------------------------------------------------------------------------------
ssT_Time_t   Delta_as_Time =
{
//  valid positive zero    d   h   m   s  ms               us  ns  p
    true, true,    false,  0,  0,  0,  0, DeltaInitState,  0,
#ifdef SST_OPTIN_TIME_NSECS_TU
/* not blank, don't delete */                                  0,
#endif  // SST_OPTIN_TIME_NSECS_TU
/* not blank, don't delete */                                      0, 0, 0
};
ReadOnly    ssT_pTime_t pDelta_as_Time = &Delta_as_Time;

// -------------------------------------------------------------------------------------------------
// N BPM is N beats per minute or 60 seconds; each second has 1000 millis; beats are always nonzero.
// 60 seconds has 60,000 millis.  Each beat uses up an equal portion of the 60,000 milliseconds.
// When the BPM is 144, the BP (beat period) is 417 millis (60000/144); max BP is 60000ms (60000/1).
// ------------------------------------------------------------------------------------------------
#define MAX_MILLIS_PER_MINUTE               (SST_SECS_PER_MIN * SST_MSECS_PER_SEC)
#define MAX_BEATS_PER_MINUTE                (SST_SECS_PER_MIN * SST_MSECS_PER_SEC)

// -------------------------------------------------------------------------------------------------
int millis_from_BPM_rounded (int bpm)
{
    int millis_div = (MAX_MILLIS_PER_MINUTE / bpm);     // how many complete beats occur in 1 minute
    int millis_mod = (MAX_MILLIS_PER_MINUTE % bpm);     //

#ifdef APP_OPTIN_SHOW_YOUR_WORK
    ss_uiOp_emit_1 ('M');
    ss_uiOp_emit_1 ('/');
    ss_uiOp_emit_Int_999 (bpm);
    ss_uiOp_emit_1 ('=');
    ss_uiOp_emit_Int_999 (millis_div);
    ss_uiOp_emit_1 ('%');
    ss_uiOp_emit_Int_999 (bpm);
    ss_uiOp_emit_1 ('=');
    ss_uiOp_emit_Int_999 (millis_mod);
#endif  // APP_OPTIN_SHOW_YOUR_WORK
    // 60000ms/100beats = 600ms per beat, remainder zero, BP has no remainder
    // 60000ms/111beats = 540.540 (ms.us) per beat, remainder 540, add 1ms for the partial beat
    if (millis_mod > 499)
    {
        millis_div++;
        ss_uiOp_emit_1 ('+');
        ss_uiOp_emit_Int_999 (millis_div);
    }
    return (millis_div);
}   // millis_from_BPM_rounded
// -------------------------------------------------------------------------------------------------
int BPM_from_millis_rounded (int millis)
{
    int BPM_div = (MAX_MILLIS_PER_MINUTE / millis);  // how many complete beats occur in 1 minute
    int BPM_mod = (MAX_MILLIS_PER_MINUTE % millis);  //

#ifdef APP_OPTIN_SHOW_YOUR_WORK
    ss_uiOp_emit_1 ('M');
    ss_uiOp_emit_1 ('/');
    ss_uiOp_emit_Int_999 (millis);
    ss_uiOp_emit_1 ('=');
    ss_uiOp_emit_Int_999 (BPM_div);
    ss_uiOp_emit_1 ('%');
    ss_uiOp_emit_Int_999 (millis);
    ss_uiOp_emit_1 ('=');
    ss_uiOp_emit_Int_999 (BPM_mod);
#endif  // APP_OPTIN_SHOW_YOUR_WORK

    // 60000ms/600ms = 100 BPM
    // 60000ms/613ms = 97.87    should be reported as 98 BPM, certainly closeer than 97 BPM.
    if (BPM_mod > 499)
    {
        BPM_div++;
    }
    return (BPM_div);
}   // BPM_from_millis_rounded
// -------------------------------------------------------------------------------------------------
// commands such as "go" and parameters such as "0xhhhh" are limited to this length.
// -------------------------------------------------------------------------------------------------
#define APPTOKEN_ALLOC (10)
#define APPTOKEN_MAX_I (APPTOKEN_ALLOC-1)

#define Is_Command(pbuf,plongcommand,pshortcommand)     \
        (ssUI_AaIf_pP1_eq_pP2 (pbuf, plongcommand) || ssUI_AaIf_pP1_eq_pP2 (pbuf, pshortcommand) )

// -------------------------------------------------------------------------------------------------
// nerdy details to ignore:
// A parsing FSM remembers (through some method) where parsing left off when the PREVIOUS token was
// copied out into the "ptoken" location of the caller.  Here, the return value is the memory method.
// The responsibility and cost of remembering the return value is pushed up to the user of the FSM.
// The return value is the address of the byte AFTER the bytes recognized and copied out as a token;
// so the parsing for the next token begins when that address is provided on the NEXT parsing call.
// -------------------------------------------------------------------------------------------------
pAsciiA_t   App_extract_token (pAsciiA_t pBytesReceived, pAsciiA_t ptoken);

// -------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------
void    App_show_gCycler_BPM (void);
void    App_show_gChaser_BPM (void);
void    App_show_delta (void);

// -------------------------------------------------------------------------------------------------
// this shorthand for a repeating item makes it easier to see all the help messages using "find all"
// -------------------------------------------------------------------------------------------------
#define bpmHelp(s)  ss_uiOp_emit_qBanner (lfN, S(s), lfY)
#define showVar(s)  { ss_uiOp_emit_qBanner (lfY, "---", lfN); ss_uiOp_emit_qBanner (lfY, S(s), lfN); }

// -------------------------------------------------------------------------------------------------
// the App has a UI based on user input of ascii character sequences while the App UI is bpm's UI.
// -------------------------------------------------------------------------------------------------
void App_bpm (pAsciiA_t pBytesReceived)
{
    boolean             ValidHexNumber;
    boolean             IsHelpRequest = false;
    int                 period_msecs;                   // for math purposes
    int                 delta_msecs;                    // for math purposes, with period_msecs
    int                 bpm_value;

    ssUI_db_pEvApi_t    Cycler_pEvApi;                  // used to modify the Cycler's AboutEv.Period
    ssE_pAboutEv_t      Cycler_pAboutEv;

    ssUI_db_pEvApi_t    Chaser_pEvApi;                  // used to modify the Chaser's AboutEv.Period
    ssE_pAboutEv_t      Chaser_pAboutEv;

    AsciiA_t            command_buf[APPTOKEN_ALLOC];
    AsciiA_t            param_buf[APPTOKEN_ALLOC];

    // ---------------------------------------------------------------------------------------------
    pBytesReceived = App_extract_token (pBytesReceived, command_buf);

    // ---------------------------------------------------------------------------------------------
    // processing for each command has this format:
    //  if (help request)
    //      display the command and what it does, continue through all commands so all helps show
    //  else
    //      do what the command does, exit
    // ---------------------------------------------------------------------------------------------
    if (Is_Command (command_buf, pcHelp, pcHelp))     // no shorthand for "help"
    {
        IsHelpRequest = true;
    }   // help

    // ---------------------------------------------------------------------------------------------
    // reference to cycler or chaser varies by command; all references are through these pointers.
    // ---------------------------------------------------------------------------------------------
    Cycler_pEvApi = ssUI_dbOp_Lookup_UsingEvApi_i (MRB_CYCLER_EVAPI);
    Cycler_pAboutEv = Cycler_pEvApi->ssTEA_pApiSig->ssE_pAboutEv;

    Chaser_pEvApi = ssUI_dbOp_Lookup_UsingEvApi_i (MRB_CHASER_EVAPI);
    Chaser_pAboutEv = Chaser_pEvApi->ssTEA_pApiSig->ssE_pAboutEv;

    // ---------------------------------------------------------------------------------------------
    if (IsHelpRequest)
    {
        bpmHelp ("----  configure  ----");
        bpmHelp ("ready     assign Time values referenced by ssUI commands launching Events");
    }
    else
    {
        if (Is_Command (command_buf, pcReady, pcUReady))
        {
            showVar ("'mrb_launch' uses <timea>: for Cycler's Start Time From Now");
            // gives chance to issue 'go' to set secret BPM
            ssUI_HeyProcessThis (S("math timea=5.0sc"));

            showVar ("'mrb_launch' uses <timeb>: for Cycler's computed Stop Time");
            // sets the time limit on solving BPM
            ssUI_HeyProcessThis (S("math timeb=20mn"));

            showVar ("'mrb_launch' uses <timec>: for Cycler's starting period of recurrence");
            // A Time of Occurence is required, App modifies
            ssUI_HeyProcessThis (S("math timec=500ms"));

            showVar ("'mrb_launch' uses <timed>: for Chaser's starting period of recurrence");
            // A Time of Occurence is required, App modifies the AboutEvent data, not the TimeVar
            ssUI_HeyProcessThis (S("math timed=500ms"));

            ss_uiOp_emit_qBanner (lfY, "---", lfY);
            return;                             // each command completes the App response and returns
        }   // ready
    }   // not help

    // ---------------------------------------------------------------------------------------------
    if (IsHelpRequest)
    {
        bpmHelp ("set       issue ssUI commands launching Cycler and Chaser Events");
    }
    else
    {
        if (Is_Command (command_buf, pcSet, pcUSet))
        {
            ss_uiOp_emit_qBanner (lfY, "enter 'ssui' to grant ssUI agency to run mrb_launch commands,", lfY);
            ss_uiOp_emit_qBanner (lfY, "then enter 'app' to return to the UI for What's The BMP?", lfY);

            ssUI_HeyProcessThis (S("cmds run mrb_launch"));

            return;                             // each command completes the App response and returns
        }   // set
    }   // not help

    // ---------------------------------------------------------------------------------------------
    if (IsHelpRequest)
    {
        bpmHelp ("go [0xhhhh] set the secret Cycler BPM (if no parameter, assign random 1-1000)");
    }
    else
    {
        if (Is_Command (command_buf, pcGo, pcUGo))
        {
            pBytesReceived = App_extract_token (pBytesReceived, param_buf);

            if (param_buf[0] != Ascii_NUL)
            {
                bpm_value = ssUI_Ascii_chg_hexDigitsToInt (param_buf, &ValidHexNumber);
                if (ValidHexNumber)
                {
                    if ((bpm_value == 0) || (bpm_value > MAX_BEATS_PER_MINUTE))
                    {
                        ss_uiOp_emit_qBanner (lfY, "BPM is range-limited: 1-60,000 (0xEA60)", lfY);
                        return;
                    }
                    gCycler_BPM = bpm_value;
                    ss_uiOp_emit_qBanner (lfY, "secret Cycler BPM set from input", lfY);
                }
                else
                {
                    ss_uiOp_emit_qBanner (lfY, "secret Cycler BPM must be in the form: 0xHHHH", lfY);
                    return;
                }
            }
            else
            {
                gCycler_BPM = mesa_Random_From1000_Seq () + 1;      // change 0-999 into 1-1000
                ss_uiOp_emit_qBanner (lfY, "App has selected a random value 1-1000 for Cycler's BPM", lfY);
            }

            // The App sets the Cycler period from the BPM specified
            period_msecs = BPM_from_millis_rounded (gCycler_BPM);
            ssT_timeOp_TimeFromMillis (period_msecs, &Cycler_pAboutEv->Period);
            ss_uiOp_Show_Time (&Cycler_pAboutEv->Period, lfY);

            return;                             // each command completes the App response and returns
        }   // go [value]
    }   // not help

    // ---------------------------------------------------------------------------------------------
    if (IsHelpRequest)
    {
        bpmHelp ("show      compute, and display in clear text, the BPMs from period values");
    }
    else
    {
        if (Is_Command (command_buf, pcShow, pcUShow))
        {
            period_msecs = ssT_timeOp_MillisFromTime (&Cycler_pAboutEv->Period);
            gCycler_BPM = millis_from_BPM_rounded (period_msecs);

            period_msecs = ssT_timeOp_MillisFromTime (&Chaser_pAboutEv->Period);
            gChaser_BPM = millis_from_BPM_rounded (period_msecs);

            ss_uiOp_emit_newline ();

            App_show_gCycler_BPM ();

            App_show_gChaser_BPM ();

            ss_uiOp_emit_newline ();

            App_show_delta ();

            return;                             // each command completes the App response and returns
        }   // show
    }   // not help

    // ---------------------------------------------------------------------------------------------
    if (IsHelpRequest)
    {
        bpmHelp ("----   operate   ----");
        bpmHelp ("delta     display the Time value added to or subtracted from Chaser's period");
    }
    else
    {
        if (Is_Command (command_buf, pcDelta, pcUDelta))
        {
            App_show_delta ();
            return;                             // each command completes the App response and returns
        }   // Delta
    }   // not help

    // ---------------------------------------------------------------------------------------------
    // A few programming things to point out about "more" and "less" from an ssTEA perspective:
    //  1) two models for pointer use are demonstrated, both result in same processor instructions.
    //  2) assuming TIME_IS_AN_ARROW, subtracting a Time from another Time is not supported by ssT.
    //     Addition and subtraction operations use the total [millis] in [minutes:seconds:millis].
    // ---------------------------------------------------------------------------------------------
    // A few App concepts to point out about "more" and "less" from an app-fsmDemo perspective:
    //  1) The App allows the user to change the BPM of the Chaser in order to align with the Cycler.
    //  2) The App communicates using BPM values; user entering "more" increases BPM, "less" decreases.
    //     The App implements the change by directly modifying the value in the Chaser AboutEv data.
    //  3) ssUI implements the edit/change with a command line option to set the Chaser AboutEv data
    //     period of occurence directly. The difference between two is: the App converts the BPM to
    //     and from an ssT Time, does math, then sets the period; ssUI is an editor for the user.
    // ---------------------------------------------------------------------------------------------
    if (IsHelpRequest)
    {
        bpmHelp ("more      beats; increase the Chaser BPM by subtracting the delta value ");
    }
    else
    {
        if (Is_Command (command_buf, pcMore, pcUMore))
        {
            period_msecs = ssT_timeOp_MillisFromTime (&Chaser_pAboutEv->Period);
            delta_msecs = ssT_timeOp_MillisFromTime (pDelta_as_Time) * DELTA_MULTIPLIER;

#ifdef APP_OPTIN_SHOW_YOUR_WORK
            ss_uiOp_emit_Int_999 (period_msecs);
            ss_uiOp_emit_1 ('-');
            ss_uiOp_emit_Int_999 (delta_msecs);
            ss_uiOp_emit_1 ('=');
#endif  // APP_OPTIN_SHOW_YOUR_WORK

            period_msecs -= delta_msecs;

#ifdef APP_OPTIN_SHOW_YOUR_WORK
            if (period_msecs < 0)
            {
                ss_uiOp_emit_1 ('-');
            }
            ss_uiOp_emit_Int_999 (period_msecs);
            ss_uiOp_emit_newline ();
#endif  // APP_OPTIN_SHOW_YOUR_WORK

            if (period_msecs < 1)
            {
                ss_uiOp_emit_qBanner (lfY, "Chaser BP cannot be less than 1 millisecond, set to 1ms", lfY);
                period_msecs = 1;
            }   // msecs still nonzero positive

            ssT_timeOp_TimeFromMillis (period_msecs, &Chaser_pAboutEv->Period);

            gChaser_BPM = millis_from_BPM_rounded (period_msecs);
            App_show_gChaser_BPM ();

            return;                             // each command completes the App response and returns
        }   // more
    }   // not help

    // ---------------------------------------------------------------------------------------------
    if (IsHelpRequest)
    {
        bpmHelp ("less      beats; decrease the Chaser BPM by adding the delta value");
    }
    else
    {
        if (Is_Command (command_buf, pcLess, pcULess))
        {
            period_msecs = ssT_timeOp_MillisFromTime (&Chaser_pAboutEv->Period);
            delta_msecs = ssT_timeOp_MillisFromTime (pDelta_as_Time) * DELTA_MULTIPLIER;

#ifdef APP_OPTIN_SHOW_YOUR_WORK
            ss_uiOp_emit_Int_999 (period_msecs);
            ss_uiOp_emit_1 ('+');
            ss_uiOp_emit_Int_999 (delta_msecs);
            ss_uiOp_emit_1 ('=');
#endif  // APP_OPTIN_SHOW_YOUR_WORK

            period_msecs += delta_msecs;

#ifdef APP_OPTIN_SHOW_YOUR_WORK
            ss_uiOp_emit_Int_999 (period_msecs);
            ss_uiOp_emit_newline ();
#endif  // APP_OPTIN_SHOW_YOUR_WORK

            if (period_msecs > MAX_MILLIS_PER_MINUTE)
            {
                ss_uiOp_emit_qBanner (lfY, "Chaser BP cannot be more than 1 minute, or,", lfY);
                ss_uiOp_emit_qBanner (lfN, "   BPM cannot be less than 1 beat", lfY);
                period_msecs = MAX_MILLIS_PER_MINUTE;
            }   // msecs still nonzero positive

            ssT_timeOp_TimeFromMillis (period_msecs, &Chaser_pAboutEv->Period);

            gChaser_BPM = millis_from_BPM_rounded (period_msecs);

            ss_uiOp_Show_Time (&Chaser_pAboutEv->Period, lfY);
            App_show_gChaser_BPM ();

            return;                             // each command completes the App response and returns
        }   // fewer
    }   // not help

    // ---------------------------------------------------------------------------------------------
    if (IsHelpRequest)
    {
        bpmHelp ("smaller   changes; decrease the delta (using build-time table of deltas)");
    }
    else
    {
        if (Is_Command (command_buf, pcSmaller, pcUSmaller))
        {
            // if at zero, stick at zero; if not at zero, room to decrement
            if (deltas_i > 0)
            {
                deltas_i--;
            }
            ssT_timeOp_TimeFromMillis (deltas[deltas_i], pDelta_as_Time);
            ss_uiOp_Show_Time (pDelta_as_Time, lfY);

            App_show_delta ();

            return;                             // each command completes the App response and returns
        }   // smaller
    }   // not help

    // ---------------------------------------------------------------------------------------------
    if (IsHelpRequest)
    {
        bpmHelp ("bigger    changes; increase the delta (using build-time table of deltas)");
    }
    else
    {
        if (Is_Command (command_buf, pcBigger, pcUBigger))
        {
            // if at zero, stick at zero; if not at zero, room to decrement the index
            if (deltas_i < deltas_i_max_i)
            {
                deltas_i++;
            }
            ssT_timeOp_TimeFromMillis (deltas[deltas_i], pDelta_as_Time);
            ss_uiOp_Show_Time (pDelta_as_Time, lfY);

            App_show_delta ();

            return;                             // each command completes the App response and returns
        }   // Bigger
    }   // not help

    if (!IsHelpRequest)
    {
        ss_uiOp_emit_qBanner (lfY, "input not recognized as an 'app bpm command'", lfY);
    }   // not help and no match for input
}   // App_bpm
// -------------------------------------------------------------------------------------------------
void App_show_gCycler_BPM (void)
{
    ss_uiOp_emit_qAsciiA ("current computed BPM from Cycler period: ");
    ss_uiOp_emit_1 (Ascii_squareLBracket);
    ss_uiOp_emit_Int_999 (gCycler_BPM);
    ss_uiOp_emit_1 (Ascii_squareRBracket);
    ss_uiOp_emit_newline ();
}   // App_show_gCycler_BPM
// -------------------------------------------------------------------------------------------------
void App_show_gChaser_BPM (void)
{
    ss_uiOp_emit_qAsciiA ("current computed BPM from Chaser period: ");
    ss_uiOp_emit_1 (Ascii_squareLBracket);
    ss_uiOp_emit_Int_999 (gChaser_BPM);
    ss_uiOp_emit_1 (Ascii_squareRBracket);
    ss_uiOp_emit_newline ();
}   // App_show_gChaser_BPM
// -------------------------------------------------------------------------------------------------
void App_show_delta (void)
{
    ss_uiOp_emit_qAsciiA ("current delta in ms: ");
    ss_uiOp_emit_1 (Ascii_squareLBracket);
    ss_uiOp_emit_Int_999 (deltas[deltas_i]);
    ss_uiOp_emit_1 (Ascii_squareRBracket);
    ss_uiOp_emit_newline ();
}   // App_show_delta
// -------------------------------------------------------------------------------------------------
pAsciiA_t   App_extract_token (pAsciiA_t pBytesReceived, pAsciiA_t ptoken)
{
    int token_i = 0;

    // while loop exits when a non-Space is found: this includes NUL byte.
    while (*pBytesReceived == Ascii_Space)
    {
        pBytesReceived++;
    }

    // copy from input all bytes up to NUL or Space while not overflowing the destination buffer;
    // leave pointer to bytes received pointing at the space or the NUL following the last byte;
    // leave token NUL-terminated whether valid length copied up to NUL/Space or invalid length input.
    ptoken[token_i] = Ascii_NUL;
    while (*pBytesReceived != Ascii_NUL)
    {
        if (*pBytesReceived != Ascii_Space)             // byte received is not NUL and not space
        {
            ptoken[token_i] = *pBytesReceived;          // copy every byte except space
            pBytesReceived++;                               // future parsing starts after byte copied
        }
        else
        {   // *pBytesRecieved == Space
            // pBytesRecieved still points at Space, future parsing starts there, copy complete.
            // a secret: this "if" handles a space after the last token in the command line
            break;
        }

        // prevent stack-hit related faults by preventing user input from exceeding allocation
        if (token_i < APPTOKEN_MAX_I)                   // need one byte for NUL byte (the next byte)
        {
            token_i++;                                  // there is one byte at least available
        }
        else
        {
            if (
                (*pBytesReceived != Ascii_NUL) &&       // time for NUL, is there more input?
                (*pBytesReceived != Ascii_Space)        // only space(s) seperates tokens
               )
            {
                ss_uiOp_emit_qBanner (lfY, "command/param too long to parse, truncated", lfY);
            }
            token_i = APPTOKEN_MAX_I;                   // place NUL after last byte stored
            break;
        }
    }   // while not Space

    ptoken[token_i] = Ascii_NUL;                        // copied space we don't want
    return (pBytesReceived);
}   // App_extract_token
#endif // __APP_BPM_INO_H

