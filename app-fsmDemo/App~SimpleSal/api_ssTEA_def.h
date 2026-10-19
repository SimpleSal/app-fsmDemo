/* -- SimpleSal: ssTEA, ssUI, and ssIO | (C) 2025, 2026 TruSoft Computing LLC |   All rights reserved. --
   This software is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
   This software is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
   ---------------------------------------------------------------------------------------------------
 *
 * api_ssTEA_def.h      data and software definitions: application API (software interface) to ssTEA
 *
 * 2026/10/10   "fsmDemo.ino" Arduino software updated with released ssTEA/ssUI into "app-fsmDemo.ino".
 * 2025/08/06   special version appeared as the star of first videos published at SimpleSal substack.
 * 2024/05/27   created superset FSM Demo Application as basis for ssTEA and SimpleSal dev/test.
 **************************************************************************************************/
#ifndef __API_SSTEA_DEF_H
#define __API_SSTEA_DEF_H

// =================================================================================================
// -------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------
// ssUI gives when input is "app" or "app xxx": if "app": pointer to NUL; "app xxx": pointer to 'x'.
// The declaration imposed by ssUI is made in "ssUI_input_dcl.h"
// ssUI removes the "app " at the start of the input
// -------------------------------------------------------------------------------------------------
void  App_HeyProcessThis (pAsciiA_t pAscii_After_App)
{
    App_bpm (pAscii_After_App);             // there is only one App, and that App is "bpm"
}   // App_HeyProcessThis
// =================================================================================================
// -------------------------------------------------------------------------------------------------
// [.\SimpleSal\ssDocs\ssTEA\Event Declarations.note]        ssE declares what the interface will be
// [.\SimpleSal\ssDocs\ssTEA\Event Databases.note]          ssE or the App instantiates the database
// -------------------------------------------------------------------------------------------------
// App note: the allocation is optional UNLESS the configuration says the App will allocate data.
// App note: the method is arbitrarily different from the ssE implementation just to show it can be.
// -------------------------------------------------------------------------------------------------
#ifdef SSE_ONEOF_ABOUTEVDB_ALLOC_BY_APP

ssE_AboutEv_t   AboutEvDB_allocSpace[SSE_ONEOF_ABOUTEVDB_ALLOC_CT];
boolean         AboutEvDB_allocTrack[SSE_ONEOF_ABOUTEVDB_ALLOC_CT];

// -------------------------------------------------------------------------------------------------
// each time initialization is performed, the database is cleared and valid.
// -------------------------------------------------------------------------------------------------
void    AboutEvDB_dbOp_Init_DB (void)
{
    int                 DB_i;

    for (DB_i=0; DB_i <= SSE_ONEOF_ALLOC_ABOUTEV_MAX_I; DB_i++)
    {
        AboutEvDB_allocTrack[DB_i] = false;
#ifdef SSE_OPTIN_ABOUTEVDB_DEBUG
        ss_uiOp_emit_1 (Ascii_EncapL);
        ss_uiOp_emit_Hex_2 (DB_i);
        ss_uiOp_emit_1 (Ascii_EncapR);
        ssE_EvDB_Show_ssE_pAboutEv (S("Event Data : App Init"), &AboutEvDB_allocSpace[DB_i]);
#endif  // SSE_OPTIN_ABOUTEVDB_DEBUG
        ssE_dbOp_Init_One_AboutEv (&AboutEvDB_allocSpace[DB_i], DEEPINIT);
    }
}   // AboutEvDB_dbOp_Init_DB

// -------------------------------------------------------------------------------------------------
ssE_pAboutEv_t   AboutEvDB_dbOp_Get (void)
{
    ssE_pAboutEv_t      ssE_pAboutEv = ssE_pAboutEvNull;
    int                 DB_i;

    for (DB_i=0; DB_i <= SSE_ONEOF_ALLOC_ABOUTEV_MAX_I; DB_i++)
    {
        if (AboutEvDB_allocTrack[DB_i] == false)
        {
            AboutEvDB_allocTrack[DB_i] = true;
            ssE_pAboutEv = &AboutEvDB_allocSpace[DB_i];
            break;      // out of for
        }   // if not in use already
    }   // checking all allocated data
    return (ssE_pAboutEv);         // null if none found or the first one found
}   // AboutEvDB_dbOp_Get

// -------------------------------------------------------------------------------------------------
ssE_pAboutEv_t     AboutEvDB_dbOp_Put (ssE_pAboutEv_t pReleaseThis)
{
    ssE_pAboutEv_t   ssE_pAboutEv = ssE_pAboutEvNull;
    int                     DB_i;

    for (DB_i=0; DB_i <= SSE_ONEOF_ALLOC_ABOUTEV_MAX_I; DB_i++)
    {
        if (&AboutEvDB_allocSpace[DB_i] == pReleaseThis)  // only put good values
        {
            AboutEvDB_allocTrack[DB_i] = false;
            return (ssE_pAboutEvNull);
        }   // any match completes the task, whethere allocated or not
    }   // look in each slot

    // couldn't put it back, better to not just pretend we did put it back
    return (ssE_pAboutEv);
}   // AboutEvDB_dbOp_Put
#endif  // SSE_ONEOF_ABOUTEVDB_ALLOC_BY_APP
// =================================================================================================
// =================================================================================================
// =================================================================================================
// =================================================================================================
// =================================================================================================
// =================================================================================================
// -------------------------------------------------------------------------------------------------

// =================================================================================================

// =================================================================================================
// -------------------------------------------------------------------------------------------------
ssE_EvResult_t App_Launch_in_ssTEA (ssUI_db_pEvApi_t pEvApi)
{
    App_Announce_App ("Launching Event in ssTEA");
    App_Announce_App (pEvApi->pEvApiNameTkn_App->pAsciiA);

    if (ssUI_apiSignalOp_Emit (pEvApi, ss_ApiSigMsgValue_AgApifsm_Validate))
    {
        App_Announce_ssTEA ("Event Agency Descriptor <validate> succeeded");
        if (ssUI_apiSignalOp_Emit (pEvApi, ss_ApiSigMsgValue_AgApifsm_Run))
        {
            App_Announce_ssTEA ("Agency Api FSM <On Running> succeeded");
            if (ssUI_apiSignalOp_Emit (pEvApi, ss_ApiSigMsgValue_AgRunfsm_Start))
            {
                App_Announce_ssTEA ("Agency Run FSM <Start> succeeded");
                ss_uiOp_Show_Apifsm_state (pEvApi->ssTEA_pApiSig, pApiSigName (pEvApi));
                ss_uiOp_Show_Runfsm_state (pEvApi->ssTEA_pApiSig, pApiSigName (pEvApi));
                return (ssE_EvResult_OK_go);
            }   // Start succeeded
            else
            {   // Start failed
                App_Announce_ssTEA ("Error: Agency Run FSM failed to enter <Agencying>");
                return (ssE_EvResult_notOK_state);
            }   // Start failed
        }   // Run succeeded
        else
        {   // Run failed
            App_Announce_ssTEA ("Error: Agency Api FSM failed to enter <On Running>");
            return (ssE_EvResult_notOK_state);
        }   // Run failed
    }   // Validate succeeded
    else
    {   // Validate failed
        App_Announce_ssTEA ("Error: Event Agency Descriptor <validate> failed");
        return (ssE_EvResult_notOK_state);
    }   // Validate failed
}   // App_Launch_in_ssTEA

#endif // __API_SSTEA_DEF_H

