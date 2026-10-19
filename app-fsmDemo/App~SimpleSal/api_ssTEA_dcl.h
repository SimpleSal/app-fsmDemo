/* -- SimpleSal: ssTEA, ssUI, and ssIO | (C) 2025, 2026 TruSoft Computing LLC |   All rights reserved. --
   This software is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
   This software is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
   ---------------------------------------------------------------------------------------------------
 *
 * api_ssTEA_dcl.h      data and software declarations: application API (software interface) to ssTEA
 *
 * 2026/10/10   "fsmDemo.ino" Arduino software updated with released ssTEA/ssUI into "app-fsmDemo.ino".
 * 2025/08/06   special version appeared as the star of first videos published at SimpleSal substack.
 * 2024/05/27   created superset FSM Demo Application as basis for ssTEA and SimpleSal dev/test.
 **************************************************************************************************/
#ifndef __API_SSTEA_DCL_H
#define __API_SSTEA_DCL_H

// =================================================================================================
// -------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------
ssE_EvResult_t      App_Launch_in_ssTEA (ssUI_db_pEvApi_t pEvApi);

// -------------------------------------------------------------------------------------------------
// [.\SimpleSal\ssDocs\ssTEA\Event Declarations.note]        ssE declares what the interface will be
// [.\SimpleSal\ssDocs\ssTEA\Event Databases.note]          ssE or the App instantiates the database
// -------------------------------------------------------------------------------------------------
// NOT declared by the App          void    AboutEvDB_dbOp_Init_DB (void);

// -------------------------------------------------------------------------------------------------
// [.\SimpleSal\ssDocs\swDev\Software Developer Tricks.note]    macros make function calls disappear
// -------------------------------------------------------------------------------------------------
#ifdef BLD_DEBUG_APIDATA_ACCESS
void                Show_EvApiData_Access (pAscii_t pMessage, ssUI_db_pEvApi_t pEvApi);

#define Bld_ApiData_Debug_Access_func(xpEvApi)   Show_EvApiData_Access ((pAscii_t) __func__, xpEvApi)
#define Bld_ApiData_Debug_Action(xpEvApi,pAA)    Show_EvApiData_Access ((pAscii_t) pAA, xpEvApi)
#else   // not // BLD_DEBUG_APIDATA_ACCESS
#define Bld_ApiData_Debug_Access_func(xpEvApi)
#define Bld_ApiData_Debug_Action(xpEvApi,pAA)
#endif  // BLD_DEBUG_APIDATA_ACCESS

#endif // __API_SSTEA_DCL_H

