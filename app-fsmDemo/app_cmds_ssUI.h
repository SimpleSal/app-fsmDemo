/* -- SimpleSal: ssTEA, ssUI, and ssIO | (C) 2025, 2026 TruSoft Computing LLC |   All rights reserved. --
   This software is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
   This software is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
   ---------------------------------------------------------------------------------------------------
 *
 * app_cmds_ssui.h      data and software definitions: application fifos demonstration
 *
 * 2026/10/10   "fsmDemo.ino" Arduino software updated with released ssTEA/ssUI into "app-fsmDemo.ino".
 * 2025/08/06   special version appeared as the star of first videos published at SimpleSal substack.
 * 2024/05/27   created superset FSM Demo Application as basis for ssTEA and SimpleSal dev/test.
 **************************************************************************************************/
#ifndef _APP_CMDS_SSUI_H
#define _APP_CMDS_SSUI_H

// =================================================================================================
// -------------------------------------------------------------------------------------------------
// The purposes of this file with only "commands" arrays from ssUI include:
//   1) remove this complication from the App source and make it portable from App to App
//   2) create a single file that can be edited frequently without editing any other files
//   3) isolate a large chunk of optional content that requires conformance to a specific definition
// -------------------------------------------------------------------------------------------------

// =================================================================================================
// -------------------------------------------------------------------------------------------------
// Any collection of characters within "" quotes can be changed to another collection of characters;
// quoted arrays of Ascii values may be added or removed as long as each one but the last is followed by a comma;
// NoBananas is educational, a demonstration of entropy and decay that occurs in software over Time.
// -------------------------------------------------------------------------------------------------
ssUI_Token_t  cmdsTkn_mrb_launch = { S("mrb_launch"),       9, NoBananas };
pCmd_t pcCmds_mrb_launch[] =
{
    "echo pcCmds_mrb_launch",
    "echo ==========================",
    "evapi * access off",
    "evapi evapi1 access on",
    "evapi evapi2 access on",
    "evapi evapi3 access on",
    "evapi evapi0 access on",
    "evapi evapi0 apifsm on",
    "evapi evapi0 occursat timecurr",
    "evapi evapi0 apifsm validate",
    "evapi evapi0 apifsm off",
    "evapi evapi0 access off",
    "echo ==========================",
    // this may be used here (or anywhere) as the terminating condition for the "cmds" FSM
    "cmds off"
};
// -------------------------------------------------------------------------------------------------
ssUI_Token_t  cmdsTkn_mrb_10sc_fast = { S("mrb_10sc_fast"),      13, NoBananas };
pCmd_t pcCmds_mrb_10sc_fast[] =
{
    "math",
    "timea=1sc",
    "timeb=10sc",
    "timec=100ms",
    "timed=125ms",
    "timef=timecurr",
    "up",
    // this may be used here (or anywhere) as the terminating condition for the "cmds" FSM
    "cmds off"
};
ssUI_Token_t  cmdsTkn_mrb_2mn_slow = { S("mrb_2mn_slow"),      13, NoBananas };
pCmd_t pcCmds_mrb_2mn_slow[] =
{
    "math",
    "timea=10sc",
    "timeb=2mn",
    "timec=1.0sc",
    "timed=1.25sc",
    "timef=timecurr",
    "up",
    // this may be used here (or anywhere) as the terminating condition for the "cmds" FSM
    "cmds off"
};
// -------------------------------------------------------------------------------------------------
ssUI_Token_t  cmdsTkn_mrb_again = { S("mrb_again"),       9, NoBananas };
pCmd_t pcCmds_mrb_again[] =
{
    "echo pcCmds_mrb_again",
    "echo space",
    "evapi cycler access off",
    "evapi chaser access off",
    "evapi driver access off",
    // this may be used here (or anywhere) as the terminating condition for the "cmds" FSM
    "cmds off"
};
// -------------------------------------------------------------------------------------------------
ssUI_Token_t  cmdsTkn_fifos        = { S("fifos"),       5, NoBananas };
pCmd_t pcCmds_fifos[] =
{
    "echo pcCmds_fifos",
    "up;up;up;up;up",
    "echo space",
    "evapi evapi4 apifsm on",           // allocate a signal carrier
    "evapi evapi4 occurs timecurr",     // set event agency parameter
    "evapi evapi4 apifsm validate",     //
    "evapi evapi4 apifsm off",
    "evapi evapi4 access off",
    "evapi evapi5",
    "evapi evapi6",
    // this may be used here (or anywhere) as the terminating condition for the "cmds" FSM
    "cmds off"
};
// -------------------------------------------------------------------------------------------------
// "whip" is a randomly chosen name to differentiate from test-driven or purpose-driven name choices.
// The purpose: contain the setup sequence useful for a specific set of events in an application.
// Another purpose: provide an example definition of some commands, unrelated to the App controls.
// -------------------------------------------------------------------------------------------------
ssUI_Token_t  cmdsTkn_whip       = { S("whip"),      4, NoBananas };

pCmd_t pcCmdsWHIP[] =
{
    "echo pcCmdsWHIP",
    "up", "up", "up", "up",
    "cmds off"
};

// -------------------------------------------------------------------------------------------------
// [.\SimpleSal\ssDocs\ssUI\UI Considerations for ssUI.note]      stored input sequences: CMD arrays
// -------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------
// the form is VERY SPECIFIC: each field in the data structure MUST HAVE an initializer, AND, more
// important, EACH line of field values, EXCEPT the last line, MUST END with a '\' slash character.
// -------------------------------------------------------------------------------------------------
// On the other hand, this feature lets software contain lists of activities runnable through the UI.
// A moment taken to conquer the random complexity reveals the deeper value hidden by implementation.
// -------------------------------------------------------------------------------------------------
#define SSUI_PLEASE_INCLUDE_THESE_APP_CMDS  \
    { &cmdsTkn_whip,            &pcCmdsWHIP[0],             Compute_pCharNArray_MAX_I(pcCmdsWHIP)        } ,\
    { &cmdsTkn_mrb_launch,      &pcCmds_mrb_launch[0],      Compute_pCharNArray_MAX_I(pcCmds_mrb_launch) } ,\
    { &cmdsTkn_mrb_again,       &pcCmds_mrb_again[0],       Compute_pCharNArray_MAX_I(pcCmds_mrb_again)  } ,\
    { &cmdsTkn_mrb_2mn_slow,    &pcCmds_mrb_2mn_slow[0],    Compute_pCharNArray_MAX_I(pcCmds_mrb_2mn_slow)  } ,\
    { &cmdsTkn_mrb_10sc_fast,   &pcCmds_mrb_10sc_fast[0],   Compute_pCharNArray_MAX_I(pcCmds_mrb_10sc_fast)  } ,\
    { &cmdsTkn_fifos,      &pcCmds_fifos[0],      Compute_pCharNArray_MAX_I(pcCmds_fifos)      } ,
// -------------------------------------------------------------------------------------------------

#endif // _APP_CMDS_SSUI_H
