/* -- SimpleSal: ssTEA, ssUI, and ssIO | Copyright (C) 2025, 2026 TruSoft Computing LLC |  All rights reserved. --
   This software is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
   This software is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
   ---------------------------------------------------------------------------------------------------
 *
 * app-fsmDemo.ino     Arduino main program: demonstrate FSMs, Apps, ssTEA, Time, Events, Agency.
 *
 * 2026/10/16   "fsmDemo.ino" Arduino software updated with released ssTEA/ssUI into "app-fsmDemo.ino".
 * 2025/08/06   special version appeared as the star of first videos published at SimpleSal substack.
 * 2024/05/27   created superset FSM Demo Application as basis for ssTEA and SimpleSal dev/test.
 ---------------------------------------------------------------------------------------------------- */
// =================================================================================================
// -------------------------------------------------------------------------------------------------
// This is a SimpleSal App developed and published by TSC LLC to demonstrate FSMs and ssTEA features.
// The main purpose, then, is to provide a structure for prototyping and experimenting with Events.
// -------------------------------------------------------------------------------------------------
// The content is all a "demo", it shows a mix of ssTEA FSM implementation methods, and, showcases
// ssUI informational capabilities that will prove helpful when watching App FSMs operate over Time.
// The unusual complexity gives the reader permission to simply ignore something confusing and go on.
// -------------------------------------------------------------------------------------------------
// Until your App is completed, this is the most complex SimpleSal App for developing/testing ssTEA.
// app-fsmDemo.ino is complex because it serves multiple purposes; the ssTEA interactions are basic.
// -------------------------------------------------------------------------------------------------
// Partitioning software content into related material (files) in an "app-fsmDemo" directory tree:
//
//  .\SimpleSal\        ssTEA and ssUI source code software included by app-fsmDemo.ino
//  .\App~SimpleSal\    provides ssTEA Api helpers and the local definition of the platform
//
//  .\app-fsmDemo.ino   Arduino App (in reality a structure for Events and Apps to run within)
//  .\app_fifos_ino.h   Events, activated creates 2 FIFOs using data Events for Serial data.
//  .\app_mrb_ino.h     Events, activated creates 2 Time Events writing to 1 Data Event.
//  .\app_bpm_ino.h     With app_mrb_ino functioning, edit the periods at which Events occur.
//  .\app_cmds_ssUI.h   UI commands, packaged for Event Management such as activating Events.
//
// The partitioning of content into ".h" files allows a single Arduino Sketch INO file to be "main".
// The structure of the directories (from the partitioning) are visible by searching for "/* using".
// =================================================================================================
// -------------------------------------------------------------------------------------------------
// The FSM ideas implemented as demos are described in detail: "simplesal.substack.com", free to you.
// The order of the description is from not-abstract physical world, up to the abstract and invented.
// -------------------------------------------------------------------------------------------------
// "mesa"           the electronic circuits sharing a sense of Time and Power, and a processor.
// "devices"        hardware connected to each other "on the mesa" using a variety of mediums.
// -------------------------------------------------------------------------------------------------
// "host"           the software context providing access to the circuits, devices, and mediums.
// "host OS"        the software the User's App software runs within, on the host mesa.
// -------------------------------------------------------------------------------------------------
// "App"            the User's software being run by the Host OS on the host mesa.
// "SimpleSal App"  the User's software being run by the Host OS on the host mesa, with ssTEA/ssUI.
// "ssTEA"          Events implement the App's features, interacting with host OS, host, and mesa.
// "ssUI"           provides access and visibility into ssTEA and App Event activity in Real Time.
// -------------------------------------------------------------------------------------------------
// "app-fsmDemo"    runs Apps (MrB, cycler/chaser, fifos, BPM) alone or together by using ssTEA/ssUI.
// "FSM"            software-based Finite State Machines are demonstrated by features of app-fsmDemo.
// -------------------------------------------------------------------------------------------------
// "high-level language"  an App builder comprehends K&R C language software at build- and run-time.
// -------------------------------------------------------------------------------------------------
// =================================================================================================
// if the reader gets lost following the software language order requirements, they can search for
// "app_ss_launch".  That function is like a good rug; it really brings the room together, Dude.
// =================================================================================================
// -------------------------------------------------------------------------------------------------
// SimpleSal App "app-time" minimally describes these few settings; "app-events" briefly describes.
// -------------------------------------------------------------------------------------------------
// configuration section: in the App's context, define the mesa (Host, Host OS, Language) for ssTEA
// -------------------------------------------------------------------------------------------------
// This is the Root of concepts of the content of app-fsmDemo, an example educational SimpleSal App.
// Choices are simpler once the integrated nature of the individual FSM demonstration Apps is known.
// -------------------------------------------------------------------------------------------------
// In the SimpleSal model, everything is declared and defined, no auto-generation dependencies here.
// A quick read by developers will show most of those references are already satisfied for them.
// Extensions and modifications are related mostly to differences between common Arduino platforms;
// a set of portability content drivers relate to C++, Python, and Linux C device driver languages.
// -------------------------------------------------------------------------------------------------
// FSMs share access to the LED, Ascii, and LED Matrix.  Init States for FSMs need to be defined.
// It is useful to limit changes by isolating facts in a configuration file instead of "app.ino".
// -------------------------------------------------------------------------------------------------
// This SimpleSal App runs on UnoR4Wifo or Due, the specific implementation for this build is picked:
// e.g., the configuration for Due does not include the code/data space used by the UnoR4 LED Matrix.
// -------------------------------------------------------------------------------------------------
#include "App~SimpleSal\mesa_SelectThisBuild.h"       /* using App file implementing App features  */

#include "App~SimpleSal\ss_RO_configuration.h"        /* using App file based on ssTEA source file */

#include "App~SimpleSal\mesa_fsmDemo_dcl.h"          /* using App file extending ssTEA source file */

// =================================================================================================
// -------------------------------------------------------------------------------------------------
// configuration section: ssTEA and ssUI; within the context of the Mesa as defined by the App
// -------------------------------------------------------------------------------------------------
// Defining INCLUDE_SSTEA is required for the ssHL compiler to get into the configuration of ssTEA.
// -------------------------------------------------------------------------------------------------
#define SS_OPTIN_INCLUDE_SSTEA

#define SS_OPTIN_SHOW_BUILD_OPTIONS

#ifdef SS_OPTIN_INCLUDE_SSTEA

// -------------------------------------------------------------------------------------------------
// The ssTEA "simplest" configuration was decided by "someone", not you, based on a common context.
// The ssTEA "complex" configuration was decided by "someone", and it is more likely someone is you.
// -------------------------------------------------------------------------------------------------
// [.\SimpleSal\ssDocs\configuration\Managing App+ssTEA Configurations.note]    top-level view
// -------------------------------------------------------------------------------------------------
// The "APP_" naming and use is purely an app-fsmDemo abstraction, outside the definition of ssTEA.
// For app-fsmDemo as a test-bed for SimpleSal, the ability to switch between App models is helpful.
// -------------------------------------------------------------------------------------------------
// #define APP_ONEOF_USE_SIMPLEST_CONFIG
#define APP_ONEOF_USE_COMPLEX_CONFIG

// -------------------------------------------------------------------------------------------------
// The most fundamental aspect of ssTEA is the control of the method and pace of understanding Time.
// The App loop that app-fsmDemo pretends to be based on (pre-SimpleSal) only runs the App each 1ms.
// -------------------------------------------------------------------------------------------------
#define     App_Default_Agency_Pace         ssTEA_Agency_pace_period
#define     App_Default_Agency_Pace_period  (5)

// -------------------------------------------------------------------------------------------------
// The ssTEA simplest configuration may be modified here only for the number of Events requested;
// the section defining "simplest" simply asserts all choices EXCEPT it will accept a defined count.
// -------------------------------------------------------------------------------------------------
#ifdef APP_ONEOF_USE_SIMPLEST_CONFIG

#define SSE_ACTIVE_EVENTS_CT_REQUESTED (10)

#define SS_ACCEPT_SIMPLEST_CONFIG

#endif  // APP_ONEOF_USE_SIMPLEST_CONFIG

// -------------------------------------------------------------------------------------------------
// [.\SimpleSal\ssDocs\ssUI\UI Considerations for ssUI.note]   ssUI input buffer size considerations
// -------------------------------------------------------------------------------------------------
#ifdef APP_ONEOF_USE_COMPLEX_CONFIG

#define SS_OPTIN_INCLUDE_SSUI

// -------------------------------------------------------------------------------------------------
// ssUI's menus consist of a prompt, an input of Ascii values, a parser, and a token pattern matcher.
// The parser finds a "command" and its optional [parameters] as an array of Ascii values with a NUL.
// "commands" are collections or arrays of command arrays which are considered as input as if typed.
// -------------------------------------------------------------------------------------------------
#define SSUI_OPTIN_INCLUDE_CMDS_IN_SSUI_SSUI
#define SSUI_OPTIN_INCLUDE_CMDS_IN_SSUI_APP

// -------------------------------------------------------------------------------------------------
// [.\SimpleSal\ssDocs\ssUI\UI Considerations for ssUI.note]   ssUI input buffer size considerations
// -------------------------------------------------------------------------------------------------
#define SSUI_UIBUFFER_ALLOC     (60+2+1)   // <0-60 Ascii values><0-2 Ascii input terminators><1 nul>
#define SSUI_UIBUFFER_MAX_I     (SSUI_UIBUFFER_ALLOC-1)

#define SSUI_ONEOF_CONTROL_INIT_RUNTIME
#define SSUI_ONEOF_SERIAL_RECEIVES_LINES
// #define SSUI_ONEOF_SERIAL_RECEIVES_BYTES

// data and names in the UI are limited to events 0-9; modifications to ssUI required to increase.
#define SSE_ACTIVE_EVENTS_CT_REQUESTED (10)

// These choices are interesting only to those who want to rewrite or simply modify ssTEA sources.
#define SST_ONEOF_TIME_IS_AN_ARROW

#define SSE_OPTIN_ABOUTEVDB_TRUSTED_APP
#define SSE_ONEOF_ABOUTEVDB_ALLOC_BY_APP

#define SSE_ONEOF_ABOUTEVDB_ALLOC_CT    (SSE_ACTIVE_EVENTS_CT_REQUESTED * 1)
#define SSE_ONEOF_ALLOC_ABOUTEV_MAX_I   (SSE_ONEOF_ABOUTEVDB_ALLOC_CT-1)

// The first reference to a variable by software may be a write instruction or a read instruction.
// All of the magic tricks of building and running an embedded application influence which happens.
#define SSTEA_ONEOF_CONTROL_INIT_RUNTIME

// the SimpleSal context, educational and experimental, calls for talkative UIs and data collection.
#define SSTEA_OPTIN_ANNOUNCE

#define SSA_OPTIN_STATS_FSM
#define SSA_OPTIN_STATS_RUNFSM
#define SSA_OPTIN_STATS_APIFSM

#endif  // APP_ONEOF_USE_COMPLEX_CONFIG

// -------------------------------------------------------------------------------------------------
// Completed the configuration section, everybody has had a shot at it, now include ssTEA's view.
// Note that the ssTEA configuration is not included in the build if ssTEA itself is not included.
// If there is ever a trick added to enforce a license, it will a create null configuration here.
// -------------------------------------------------------------------------------------------------
#include "App~SimpleSal\ss_configuration.h"      /* App requests config; using ssTEA source files */

#endif // SS_OPTIN_INCLUDE_SSTEA

// =================================================================================================
// -------------------------------------------------------------------------------------------------
// The reason that the ssTEA data type declarations are included HERE, unlike all the other includes
// in "ss_include_*.h", is to allow App definition of types to reference ssTEA data types, AND, allow
// App data types to be referenced by ssTEA.  At their edges, the two co-exist and share a language.
// -------------------------------------------------------------------------------------------------
// =================================================================================================
// -------------------------------------------------------------------------------------------------
// configuration section: type definitions create the shared language.
// -------------------------------------------------------------------------------------------------
// <App> data types are dependent on the <software language> data types such as "int" within C++.
//
// <ss configuration> defines the number of data instances of <ssTEA> and <App> data types (1..N).
//
// <ssTEA> data types are dependent on the <ss configuration> + <App> + <software language> types.
//    configuration wants to manage N Events, ssTEA Event management instantiates "<type> Array[N]".
//
// <App> data types are dependent on <ssTEA> data types that refer to <ssTEA> and <App> data types.
//    this is a declaration loop problem; it exists at a liminal boundary (hole between two spaces).
// -------------------------------------------------------------------------------------------------
#include "SimpleSal\ssTEA\ssTEA_data_dcl.h"                           /* using ssTEA source files */

// =================================================================================================
// -------------------------------------------------------------------------------------------------
// configuration section: Categories on the board today for our fans at home:
//      pins, registers, fifos, DMAs, signals, sensors, switches (between mediums), interrupts, ...
// -------------------------------------------------------------------------------------------------
// Within the SimpleSal context, ssIO is using a different high-level language file inclusion model:
//      If the SSIO_OPTIN_XXX has not been #defined, the corresponding XXX file appears to be empty.
// -------------------------------------------------------------------------------------------------
// The ssIO facility is independent of both ssTEA and the ssUI application; the App references ssIO
// because the App Event Data uses Fifo control data structures to manage the flow of data on mediums.
// Mediums interconnecting mesas, such as RS-232, are moving the data at the same agreed-upon rate.
// NOTE: Within a mesa, the medium rate of operation is not the same as the mesa rate of operation.
// -------------------------------------------------------------------------------------------------
// App Event Data refers to a fifo being used as a signal carrier. Ignore or agressively scan this
// section today, it is laying the groundwork for The Ultimate SimpleSal App.  That App will be
// comprised of what appears to be two Apps on different mesas, both using a shared signal carrier.
// Each mesa will be defined by a Time Domain and Virtual Interrupt Controller.  It will appear so.
// -------------------------------------------------------------------------------------------------
// There is a dependency on the Mesa because ssIO emulates MACs (Medium Access Controller) and, the
// patented/trademarked Virtual Interrupt Controller is based on what the Mesa Reality instantiates.
// In order to consider Real and/or Virtual MACs as a set of MACs, data is required for set members.
// -------------------------------------------------------------------------------------------------
#define SS_OPTIN_INCLUDE_SSIO

#ifdef SS_OPTIN_INCLUDE_SSIO

#define SSIO_OPTIN_ANY_MAC

#define SSIO_OPTIN_PINS
#define PINS_MAC_CT    (16)

#define SSIO_OPTIN_REGS
#define REGS_MAC_CT    (2)

#define SSIO_OPTIN_DMAS
#define DMAS_MAC_CT    (2)

#define SSIO_OPTIN_FIFOS
#define FIFOS_MAC_CT   (4)

#define SSIO_OPTIN_FIFOS_STATS

// #define SSIO_OPTIN_SIGNALS
// #define SIGNALS_MAC_CT (0)
// #define SSIO_OPTIN_SENSORS
// #define SENSORS_MAC_CT (0)

#define SSIO_OPTIN_DEBUG

#include "SimpleSal\ssIO\ssIO_macs_dcl.h"                              /* using ssIO source files */

#endif  // SS_OPTIN_INCLUDE_SSIO

// =================================================================================================
// -------------------------------------------------------------------------------------------------
// configuration section: App Event Data pointed to by ssTEA Event, referenced by ssUI
// -------------------------------------------------------------------------------------------------
// The App data structure is defined and allocated for FSM use by the App for the Event.  When an
// Event is granted Agency, it is given a pointer to App data; a pointer given to ssTEA by the App.
// -------------------------------------------------------------------------------------------------
// ssTEA does not refer to App Data besides the Event function pointer and the App Data pointer.
//      The ssTEA data type referencing App data is "a pointer to some stuff I don't know about".
// ssUI, co-existing with the App, allocates App Data defined by App for each Event. The allocation
//      is in the Event API data structures instantiated per Event, using this App Data data type.
// -------------------------------------------------------------------------------------------------
// While this frees ssTEA from any knowledge of App types, the burden falls to the App to convert
// an ssTEA pointer to an App pointer or convert an App pointer to an ssTEA pointer.  The App must
// place the pointer it wants when granted Agency into a variable of the ssTEA type "a pointer to
// some stuff I don't know about", and then turn it back into an App Event Data type pointer when
// the Event software is granted Agency.  No processor instructions occur; its only compiler's view.
// But the design forces ssTEA information into the App, violating the "information hiding" rules.
// -------------------------------------------------------------------------------------------------
// The demonstration prt of app-fsmDemo results in unrelated groups of fields; all software can see
// the fields in the data structure allocated to maintain memory over Time for the FSMs in the demo.
// A more specific model may use a set of specific variables for the App's purpose (find "option #").
// -------------------------------------------------------------------------------------------------
// The SuperSet SimpleSal App that appears to be twos mesa talking across a signal carrier will have
// App Data that indicates which of the two mesas the App Data describes.  The App uses one mesa's
// MAC described by the App Data to talk to the other mesa's MAC, as described by the App Data.  The
// carrier of the physical signal between MACs probably requires the receiver copy the bytes out.
// -------------------------------------------------------------------------------------------------
typedef struct App_EventDataDesc_s
{
    Bits8_t             Bits8_A;            // App Event data option #1: generic name; use #defines
    Bits8_t             Bits8_B;
    Bits8_t             Bits8_C;
    Bits8_t             Bits8_D;
    boolean             a_boolean;

    ssT_pTime_t         a_pTimeVar;         // ssTEA types available to create applications variables

    pVoid_t             Sneak_pEvApiData;   // App Event data option #2: this Event's Api Data

#ifdef SSIO_OPTIN_FIFOS
    ssIO_pFifo_t        pFifo_In;           // App Event data option #4: App's types referenced
    ssIO_pFifo_t        pFifo_Out;
#endif  // SSIO_OPTIN_FIFOS

    Bits8_t             MrB_DeltaBitMap;    // definition of carrier, message, meaning: Driver owns

}   App_EventDataDesc_t,  *App_pEventDataDesc_t;

typedef App_EventDataDesc_t      App_EventData_t;       // referenced by ssTEA data type directly
typedef App_pEventDataDesc_t     App_pEventData_t;
#define App_pEventDataNull       ((App_pEventData_t) NULL)

// -------------------------------------------------------------------------------------------------
// App Event data option #1: generic name; use #defines to map the generic field to a useful name.
// The fields above (Bits8_A, a_pTimeVar) are generic.  A specific name in software can be invented.
// -------------------------------------------------------------------------------------------------
#define     Cycler_DeltaChannels        Bits8_A         // values valid when ID_0 even if no LEDM
#define     Cycler_DeltaValues          Bits8_B
#define     Chaser_DeltaChannels        Bits8_C         // values valid when ID_1 only if LEDM
#define     Chaser_DeltaValues          Bits8_D

#define     StopYourSelf                a_boolean
#define     pStopTime                   a_pTimeVar

// =================================================================================================
// -------------------------------------------------------------------------------------------------
// Inclusion of the source files implementing the declaration of the language of ssTEA.
// -------------------------------------------------------------------------------------------------
// ssUI co-exists with the App so the ssUI Event Api type declarations refer to the App's App Data,
// in contrast to ssTEA's view of App data in the "About Event" data used to manage Event Agency.
// Said another way: an App/ssUI view cares about the layout of the memory locations; ssTEA doesn't.
// -------------------------------------------------------------------------------------------------
#include "SimpleSal\ss_include_dcl.h"                                 /* using ssTEA source files */

// =================================================================================================
// configuration section: App use of resources on the mesa
// =================================================================================================
// configuration section: Configure the application(s) to be run within app-fsmDemo.
// =================================================================================================
// -------------------------------------------------------------------------------------------------
// #define APP_MRB_OPTIN_DEBUG_BITS_SEQUENCE

#define  APP_MRB_OPTIN_ANNOUNCE
#define  APP_MRB_OPTIN_ANNOUNCE_APP
#define  APP_MRB_OPTIN_ANNOUNCE_SSTEA
// #define  APP_MRB_OPTIN_ANNOUNCE_STATE

// #define  APP_MRB_OPTIN_ANNOUNCE_PATTERN
// #define  APP_MRB_OPTIN_ANNOUNCE_TIMETOSTOP

#ifdef APP_MRB_OPTIN_ANNOUNCE_SSTEA
#define App_Announce_ssTEA(xpThis)  { if (OK_to_Blast_Ascii_Live()) { mesa_uiOp_emit_qAsciiA (xpThis); mesa_uiOp_emit_newline (); } }
#else  // not APP_MRB_OPTIN_ANNOUNCE_SSTEA
#define App_Announce_ssTEA(xthis)
#endif // APP_MRB_OPTIN_ANNOUNCE_SSTEA

#ifdef APP_MRB_OPTIN_ANNOUNCE_APP
#define App_Announce_App(xpThis)    { if (OK_to_Blast_Ascii_Live()) { mesa_uiOp_emit_qAsciiA (xpThis); mesa_uiOp_emit_newline (); } }
#else  // not APP_MRB_OPTIN_ANNOUNCE_APP
#define App_Announce_App(xthis)
#endif // APP_MRB_OPTIN_ANNOUNCE_APP

#ifdef APP_MRB_OPTIN_ANNOUNCE_STATE
#define App_Announce_State(xpThis)  { if (OK_to_Blast_Ascii_Live()) { mesa_uiOp_emit_qAsciiA ("&&&"); mesa_uiOp_emit_qAsciiA (xpThis); mesa_uiOp_emit_newline (); } }
#else  // not APP_MRB_OPTIN_ANNOUNCE_APP
#define App_Announce_State(xpThis)
#endif // APP_MRB_OPTIN_ANNOUNCE_APP
// -------------------------------------------------------------------------------------------------
// The App's choice to use Random numbers or Ascii input as the fifo source is independent of Mesa.
// -------------------------------------------------------------------------------------------------
#define     APP_FIFOS_ONEOF_SOURCE_DEV_RANDOM
// #define     APP_FIFOS_ONEOF_SOURCE_DEV_ASCII

// #define  APP_FIFOS_OPTIN_ANNOUNCE

// =================================================================================================
// -------------------------------------------------------------------------------------------------
// now that the mesa, App, and the ssTEA context has been declared, declare the App Events.
// Anything that follows can refer to data and code in the mesa, ssTEA, and App Event name space.
// -------------------------------------------------------------------------------------------------
#include "App~SimpleSal\api_ssTEA_dcl.h"                                /* using App source files */

// =================================================================================================
#ifdef  MESA_OPTIN_LED_Matrix_Support
// -------------------------------------------------------------------------------------------------
// The Matrix features a variety of signal carriers (frame/bitmap) and 96 LEDs to group into shapes.
// The implementation of the Matrix in the UnoR4 mesa will map any 8-bit group or shape and display.
// -------------------------------------------------------------------------------------------------
//#define LEDM_ONEOF_CarrierIsFrame
#define LEDM_ONEOF_CarrierIsBitMap

#define LEDM_OPTIN_MrB0_group_FLAT
#define LEDM_OPTIN_MrB1_group_FLAT

// #define LEDM_OPTIN_MrB0_group_VERT
// #define LEDM_OPTIN_MrB1_group_VERT

#define LEDM_OPTIN_MrB0_shape_CLOCTALS
#define LEDM_OPTIN_MrB1_shape_CLOCTALS

// #define LEDM_OPTIN_debugLEDM
#endif  // MESA_OPTIN_LED_Matrix_Support

// =================================================================================================
// -------------------------------------------------------------------------------------------------
// start at the hardware platform mesa again, define everything that has been previously declared.
// -------------------------------------------------------------------------------------------------
// Actual implementations of functions supporting use of THIS hardware environment are defined here.
// Resources are the serial input/output streams, the BUILTIN LED, and the LED matrix for UnoR4.
// -------------------------------------------------------------------------------------------------
#include "App~SimpleSal\mesa_fsmDemo_def.h"         /* using App file extending ssTEA source file */

// -------------------------------------------------------------------------------------------------
// Virtual MACs, similar or dissimilar to the physical MACs defined for this mesa, are defined here.
// -------------------------------------------------------------------------------------------------
#ifdef SS_OPTIN_INCLUDE_SSIO
#include "SimpleSal\ssIO\ssIO_macs_def.h"                              /* using ssIO source files */
#endif // SS_OPTIN_INCLUDE_SSIO

// =================================================================================================
// -------------------------------------------------------------------------------------------------
// database allocation: memory described by a C data structure must be allocated for each Event
// -------------------------------------------------------------------------------------------------
// The "database" model for this application's API data, required for using ssTEA, is a statically
// allocated array of structures.  Each event API data structure is used by, assigned to, an Event.
// app-fsmDemo takes advantage of this allocation of ssUI and Event ("EvApi") variables by ssUI.
// -------------------------------------------------------------------------------------------------
// This makes the ssE EvAgency and EvApi data for the Event available through the ssUI interface.
// -------------------------------------------------------------------------------------------------
// The integration into EvApi is done by "taking over" one of the Event APIs built-in to ssUI.
// This is a form of allocation and sharing: each Event has an API instance, each App has N Events.
// -------------------------------------------------------------------------------------------------
// From a software view, necessary because there has to be a proper Init State definition; ssUI uses
// valid nonfunctional default values for pointers so data structures can be populated at build-time.
// "valid nonfunctional default values for pointers" refers to the function pointer for each Event.
// Another term for "invalid values" is "hey, they've added a nice parser to the hard fault data".
// -------------------------------------------------------------------------------------------------
// HOWEVER: Only a desire to launch Events from the ssUI command line requires the need to replace a
// Event function Ascii name, the ssUI command line knows only about DB entries, not App Events.
// -------------------------------------------------------------------------------------------------
// The form of the database is actually optimized for ARM software, the "lookup" function makes the
// only reference to the address of the allocation.  Given an array instance identifier. This is a
// choice affected by execution memory type, code is always loading pointers to use. Data access to
// executable memory for a copy of a 32-bit pointer competes with processor fething processes. Boom.
// -------------------------------------------------------------------------------------------------
// Building an App with ssTEA does not require use of the EvApi data structures allocated by ssUI.
// Of course, in that case, the App needs to allocate the API Signal and App data memory.
// That model of integrating ssTEA with the App is described elsewhere, outside the ssUI context.
// It doesn't say it, so we'll say it here: the extensive use of pointers requires your attention,
// simply adopting the ssUI data structures and software managing them uses well-proven software.
// -------------------------------------------------------------------------------------------------

// =================================================================================================
// -------------------------------------------------------------------------------------------------
// App Launching ssTEA Events
// -------------------------------------------------------------------------------------------------
// The "_Launcher" model: the User uses ssUI to enter and initiate the validation of the Event's
// ssE Event Agency data, and then abandon the "_Launcher" Event after Init; the Event continues.
// -------------------------------------------------------------------------------------------------
// A "Launcher" Event is used to launch events that consume EVAPI data structures to implement an FSM.
// In a proper application, the initialization state (Root) the FSM and the EVAPI data structures
// must be filled in with proper information at build- and/or run-time.  ssUI allows run-time edits,
// shown here by the Launcher Event being assigned by ssUI at build-time, the others are at run-time.
// -------------------------------------------------------------------------------------------------
// It would be fairly basic, but only useful if EvApis are scarce, to have one _Launcher for all.
// -------------------------------------------------------------------------------------------------
// The "define funcN_name App_name" satisfies the ssUI build-time init reference to a word that
// looks like "EvFunc0_Name" but is changed by the pre-compile processor into "App_MrB_Launcher".
// -------------------------------------------------------------------------------------------------
#define MRB_LAUNCHER_EVAPI      (0)
#undef EvFunc0_Name
#define EvFunc0_Name            App_MrB_Launcher

#define MRB_CYCLER_EVAPI        (1)
#define MRB_DRIVER_EVAPI        (2)
#define MRB_CHASER_EVAPI        (3)

#define FIFOS_LAUNCHER_EVAPI    (4)
#undef EvFunc4_Name
#define EvFunc4_Name            App_fifos_Launcher
#define FIFOS_PRODUCER_EVAPI    (5)
#define FIFOS_CONSUMER_EVAPI    (6)

// =================================================================================================
// App Instantiation: definition, implementation, allocation.
// -------------------------------------------------------------------------------------------------
// There are two methods for launching application Events into an active state:
//      where "launch" means "transition an application Event from inactive to active within ssTEA",
//      where "Event" means "software defined within the Agency Run Api FSM and the Agency Run FSM",
//      where "active" means "interacts with ssTEA within the current Time and Agency context".
//
// "fsmDemo_MrB_Launcher" and "fsmDemo_fifos_Launcher" are examples of these methods in ssTEA Apps.
// "fsmDemo_MrB_Launcher" configures and launches the Cycler AND the Driver events in series.
// "fsmDemo_fifos_Launcher" configures and launches the Producer and the Consumer fifo example.
//
// the ssUI method:
//    The compiled software of the App and its Events may be triggered by an event that can be
//    controlled through a menu command in ssUI.  The connection through ssUI means that the Init
//    State of the FSM can be triggered or Launched by invoking API calls to Validate the Event.
//
// the App method:
//    ssUI-independent method for executing the *_Launcher functions
//    call the App_MrB_Launcher function directly to start FSMs from the "setup" function,
//        after all ssTEA initialization has occurred, including the start of Time and Agency FSMs.
// ------------------------------------------------------------------------------------------
// To satisfy all references to the functions of the application FSM(s), declare their type here.
// -------------------------------------------------------------------------------------------------
// MrBlinky App in app-fsmDemo ---------------------------------------------------------------------
// These process parameters and launch the Events, Cycler/Chaser/Driver are the Events.
ssE_EvResult_t      App_MrB_getParams (void);
ssE_EvResult_t      App_MrB_Launcher (ssE_pAboutEvVoid_t  param1, ssT_pTime_t pCurrentTime);
ssE_EvResult_t      App_MrB_Cycler_Launcher (ssUI_db_pEvApi_t pCycler_EvApi);
ssE_EvResult_t      App_MrB_Chaser_Launcher (ssUI_db_pEvApi_t pChaser_EvApi);
ssE_EvResult_t      App_MrB_Driver_Launcher (ssUI_db_pEvApi_t Driver_pEvApi);

ssE_EvResult_t      App_MrB_Cycler (ssE_pAboutEvVoid_t param1, ssT_pTime_t pCurrentTime);
ssE_EvResult_t      App_MrB_Chaser (ssE_pAboutEvVoid_t param1, ssT_pTime_t pCurrentTime);
ssE_EvResult_t      App_MrB_Driver (ssE_pAboutEvVoid_t param1, ssT_pTime_t pCurrentTime);

// fifo(s) App in app-fsmDemo --------------------------------------------------------------------==//--
// These process parameters and launch the Events, Producer/Consumer are the Events.
ssE_EvResult_t      App_fifos_getParams (void);
ssE_EvResult_t      App_fifos_Launcher (ssE_pAboutEvVoid_t  param1, ssT_pTime_t pCurrentTime);
ssE_EvResult_t      App_fifos_Producer_Launcher (void);
ssE_EvResult_t      App_fifos_Consumer_Launcher (void);

ssE_EvResult_t      App_fifos_Producer (ssE_pAboutEvVoid_t param1, ssT_pTime_t pCurrentTime);
ssE_EvResult_t      App_fifos_Consumer (ssE_pAboutEvVoid_t param1, ssT_pTime_t pCurrentTime);

// -------------------------------------------------------------------------------------------------
// An announcement of the current time, and other info about what exactly is going on in here.
void App_IThink_Therefore_IAm (void);
// -------------------------------------------------------------------------------------------------
// ssUI refers to this function with received input when a particular command is entered in ssUI.
// The choice of method and implementation of switching between UIs is granted to the App.
// -------------------------------------------------------------------------------------------------
void App_Select_UI (pAsciiA_t  pReceivedInput);
// -------------------------------------------------------------------------------------------------
// The App's Time-based Events decide to stop, using a comparison between CurrentTime and TimeToStop.
boolean App_isItTimeToStop (ssT_pTime_t pCurrentTime, ssT_pTime_t pTimeToStop);
// -------------------------------------------------------------------------------------------------
// Instantiate the co-existence of the App and ssTEA as configured.  This creates the SimpleSal App.
void App_ss_launch (void);
// -------------------------------------------------------------------------------------------------
// The SimpleSal UI model allows either UI to issue sequences of commands of the other UI.
//    "app xxx" entered is sent from ssUI, asking the App to respond to "xxx" in some way.
//    "ssui ss ro" entered is sent from the app, asking ssUI to display the current ss RO status.
// -------------------------------------------------------------------------------------------------
// The App function App_HeyProcessThis, is declared by ssUI_input_dcl.h, defined in app_ssTEA_def.h.
// The only correlation between a user's input and the function for that input is established here.
// -------------------------------------------------------------------------------------------------
void App_bpm (pAsciiA_t pBytesReceived);

// =================================================================================================
// -------------------------------------------------------------------------------------------------
// App configuration: definition of arrays of ssUI commands executable from the ssUI command prompt.
// -------------------------------------------------------------------------------------------------
// [.\SimpleSal\ssDocs\ssUI\UI Considerations for ssUI.note]      stored input sequences: CMD arrays
// -------------------------------------------------------------------------------------------------
#ifdef SSUI_OPTIN_INCLUDE_CMDS_IN_SSUI_APP
#include "app_cmds_ssUI.h"                                              /* using App source files */
#endif  // SSUI_OPTIN_INCLUDE_CMDS_IN_SSUI_APP

// =================================================================================================
// -------------------------------------------------------------------------------------------------
// ssUI configuration: the build option to include/exclude message contents heavily affects size.
// ------------------------------------------------------------------------------------------------
// Define to select ssUI input processor performing extra actions during normal operational use.
// "Verbosity" means the processor will respond to all input, echo input, and offer you weak praise.
// The choice of verbosity occurs before all of ssUI is included because the effort to speak is real:
// there is the data "this is a message", the naming pThisIsAMessage, and the call to Ascii emitter.
// The use of 2/4 instead of true/false causes all ssUI references to directly compare for a value.
// -------------------------------------------------------------------------------------------------
#define YES_BE_VERBOSE  2
#define NO_BE_RETICENT  4

// #define ssUI_ONEOF_Default_Verbosity         YES_BE_VERBOSE
#define ssUI_ONEOF_Default_Verbosity         NO_BE_RETICENT

// =================================================================================================
// -------------------------------------------------------------------------------------------------
// Inclusion of the source files implementing the definition the grammar and language of ssTEA.
// -------------------------------------------------------------------------------------------------
// Notice how the files included refer to subdirs within "SimpleSal\" only, within App's directory.
// This is a direct result of using the Arduino IDE and conforming to that development context.
// Adding a path to a common and shared copy of ssTEA usually requires advanced IDE understanding.
// -------------------------------------------------------------------------------------------------
// The classic Timekeeping model software is referenced by ssTEA only to demonstrate old-time FSMs.
// -------------------------------------------------------------------------------------------------
#include "SimpleSal\ss_include_def.h"                                 /* using ssTEA source files */

// =================================================================================================
// -------------------------------------------------------------------------------------------------
// The mesa receives bytes, ssUI receives those bytes, the App receives those bytes.  Up to a limit.
// -------------------------------------------------------------------------------------------------
#define APP_BUFFER_ALLOC      SSUI_UIBUFFER_ALLOC
#define APP_BUFFER_MAX_I      (APP_BUFFER_ALLOC-1)

#include "App~SimpleSal\api_ssTEA_def.h"                                /* using App source files */
#include "App_fifos_ino.h"                                              /* using App source files */
#include "App_mrb_ino.h"                                                /* using App source files */
#include "App_bpm_ino.h"                                                /* using App source files */

// =================================================================================================
// -------------------------------------------------------------------------------------------------
// Original App using classic Timekeeping Models
// -------------------------------------------------------------------------------------------------
// The word "classic" is used to mean "a good solution at a time, before code space exceeded 8K".
// -------------------------------------------------------------------------------------------------
// This is a basic implementation of a counter-based Timekeeping model.  This model uses only the
// 32-bits of knowledge that an amount of Time has passed.  Knowledge, or perception, of the actual
// passage of time allows a Time-based Agency model based on "when": when 1ms has gone by, when 1
// second has elapsed, etc.  Layers are built at each subunit; variables to track counters blow up.
// -------------------------------------------------------------------------------------------------
// The "time-since" calculation is performed at the highest precision value available, e.g., the
// number of microseconds.  That number is added to the count of microseconds this milliseconds,
// taking into account the valid range of such a number.  The math carry operation extends the range
// of the Time that can be expressed: count the number of seconds in a day and then increment days.
// -------------------------------------------------------------------------------------------------
// The "invention" at the core of ssTEA is a data type and the math operations on data of that type.
// -------------------------------------------------------------------------------------------------
// The Classic Timekeeping implementation allows any software to use these Timekeeping variables.
// In the "loop" function, something knows how to deterime whether one millisecond has passed since
// the last time the loop did something.  The FSM event (e.g., the count of microseconds reaching a
// value that means a millisecond has occurred) marks the passage of time by incrementing a counter.
// -------------------------------------------------------------------------------------------------
TimeUnitsBig_t  CountOf_1usThis1ms;    // anytime: # of microsecond periods this 1 millisecond period
TimeUnitsBig_t  CountOf_1msThis100ms;  // anytime: # of millisecond periods this 100 millisecond period
TimeUnitsBig_t  CountOf_100msThis1sc;  // anytime: # of 100 millisecond periods this 1 second period

// =================================================================================================
// -------------------------------------------------------------------------------------------------
// Extension of Original App using classic Timekeeping Models to include asynchronous ssTEA events
// -------------------------------------------------------------------------------------------------
// LED flashing is done through an FSM that receives "the current count of milliseconds" and returns
// the new value for the current count.  This is the same as "CountOf_1" except that the "CountOf_1"
// algorithm places the detection of the boundary in loop.  The Flash FSM decides the period boundary.
// -------------------------------------------------------------------------------------------------
// Another algorithm might have the Flash FSM use the knowledge each call occurs at a millisecond
// boundary and maintain the logic and count of 1msThisXYXYX within private data to the XYXYX FSM.
// That is how the ssUI command line input algorithm avoids sitting-watching for slow RS232 data.
// -------------------------------------------------------------------------------------------------
TimeUnitsBig_t  mesa_gCtOf_1mThisFlash;     // # of millisecond periods this flash period
TimeUnitsBig_t  mesa_gCtOf_1mThisAscii;     // # of millisecond periods this Ascii Emit period

// =================================================================================================
// -------------------------------------------------------------------------------------------------
// declarations driving the compilation and content for the app-fsmDemo SuperApp: FSM example platform.
// -------------------------------------------------------------------------------------------------
#define  SIMPLESAL_APP_OPTIN_TIMECURR_FROM_TIMEBUILT
#define  SIMPLESAL_APP_OPTIN_AUTOSTART_SSTEA_FSMS

// =================================================================================================
#ifdef MESA_ONEOF_mesa_IsMrBlinky
pAsciiA_t  pBuildBanner = S("<mesa [MrBlinky] and App [app-fsmDemo] init...");
#endif  // MESA_ONEOF_mesa_IsMrBlinky
#ifdef MESA_ONEOF_mesa_IsUnoR4wifi
pAsciiA_t  pBuildBanner = S("<mesa [UnoR4wifi] and App [app-fsmDemo] init...");
#endif  // MESA_ONEOF_mesa_IsUnoR4wifi

// =================================================================================================
// -------------------------------------------------------------------------------------------------
// The ssTEA_ functionality is configured and enabled during the setup function; always co-existing.
// -------------------------------------------------------------------------------------------------
// In the original app, Arduino would call this function (as "setup") during the initialization.
// The first step makes the compiler think the name of the original is "App_setup", after #define.
// The second step is to make the compiler think the ssTEA software "ssTEA_setup" is seen as "setup".
// Arduino invokes "ssTEA_setup" as "setup"; "ssTEA_setup" invokes the App original as "App_setup".
// -------------------------------------------------------------------------------------------------
// The same mechanism is used to cause the Arduino Host's invocation of "loop" to call ssTEA's loop.
// -------------------------------------------------------------------------------------------------
#define setup   App_setup
void setup (void)
{
    int i;

    //=====  Start of app-fsmDemo SimpleSal App stuff maybe not in original App "setup" function

    mesa_gCtOf_1msFreeRunning   = 0;

#ifdef MESA_ONEOF_CONTROL_INIT_RUNTIME
    LED_RO                  = mesa_ONEOF_Default_LED_RO_owner;
    LED_RO_active           = mesa_ONEOF_Default_LED_RO_active;
    Ascii_ROs               = mesa_ONEOF_Default_Ascii_RO_owner;
    Ascii_RO_active         = mesa_ONEOF_Default_Ascii_RO_active;
    LED_Matrix_ROs          = mesa_ONEOF_Default_LED_Matrix_RO_owner;
    LED_Matrix_RO_active    = mesa_ONEOF_Default_LED_Matrix_RO_active;
#endif  // MESA_ONEOF_CONTROL_INIT_RUNTIME

    if (RO_IS_ONEOF_Ascii_ROs (RO_owner_App, RO_active_OneOfUIs_App))
    {
        if (mesa_uiOp_deviceInitialize ())     // Ascii is initialized based on the RO settings above
        {
            for (i=0; i < 20; i++)  mesa_uiOp_emit_newline ();
            mesa_uiOp_emit_qAsciiA ("<Ascii device init...success>");
            mesa_uiOp_emit_newline ();
        }
        else
        {
            for (i=0; i < 2; i++)   mesa_uiOp_emit_newline ();
            mesa_uiOp_emit_qAsciiA ("<Ascii device init...failed, so you can't see this...>");
            for (i=0; i < 2; i++)   mesa_uiOp_emit_newline ();
        }
        mesa_uiOp_emit_newline ();
        mesa_uiOp_emit_qAsciiA ("App's original setup function running");
        mesa_uiOp_emit_newline ();

        mesa_uiOp_emit_qAsciiA ( "--- mesa resource ownership build-time ---");
        mesa_uiOp_emit_newline ();
        H_ss_RO (ssUI_tkn2nd_i, false);
        mesa_uiOp_emit_newline ();
    }   // is mesa RO?
    //=====  end of app-fsmDemo SimpleSal App stuff maybe not in original App "setup" function

    // ---------------------------------------------------------------------------------------------
    // Reality occurs within the context of a TimeKeeper: what Time is it, at what Time should I act?
    // This is an example of a basic method for running software on a time basis, called "legacy".
    // Legacy software is often structured based on simplicity and small time/code/data costs.
    // ---------------------------------------------------------------------------------------------
    // The legacy implementation here of timing software uses variables, adds micros and millis
    // values over Time (over successive calls to "loop"), then does some 32-bit math with them.
    // The accumulated value of the micros/millis typically increment without concern for overflow of
    // typically 32-bit variables.  On overflow, some action must be taken to maintain a continuous
    // and coherent Time value.  Actions vary: the effect of a wrap to 0 of the fastest-accumulating
    // value (us) and the slowest (ms) are effectively decided by how long the software will run.
    // ---------------------------------------------------------------------------------------------
    CountOf_1usThis1ms   = 0;
    CountOf_1msThis100ms = 0;
    CountOf_100msThis1sc = 0;

    // ---------------------------------------------------------------------------------------------
    // To demonstrate FSMs, the App adds time-stretching to the LED, and fifos to the Ascii stream.
    // There are no clues here about whether the FSM uses a legacy model or uses ssTEA to operate.
    // ---------------------------------------------------------------------------------------------
#ifdef MESA_MIN_OPTOUT_LED              // opted out of mesa_min implementation
    mesa_gCtOf_1mThisFlash = 0;
    mesa_FSM_LEDflash_InitState ();
#endif // MESA_MIN_OPTOUT_LED           // opted out of mesa_min implementation

#ifdef MESA_MIN_OPTOUT_ASCII            // opted out of mesa_min implementation
    mesa_gCtOf_1mThisAscii = 0;
    mesa_FSM_AsciiEmit_InitState ();
#endif // MESA_MIN_OPTOUT_ASCII         // opted out of mesa_min implementation

    //=====  Start of app-fsmDemo SimpleSal App stuff maybe not in original App "setup" function
    if (RO_IS_ONEOF_Ascii_ROs (RO_owner_App, RO_active_OneOfUIs_App))
    {
        mesa_uiOp_emit_pAsciiA (pBuildBanner);

        // ---------------------------------------------------------------------------------------------
        // Now that there is at least one basis for Time, actions can be attached to specific values.
        // ---------------------------------------------------------------------------------------------
        // low-level or service software setup has completed, if we made it, the user will see something
        // ---------------------------------------------------------------------------------------------
        mesa_uiOp_emit_qAsciiA ("success>");
        mesa_uiOp_emit_newline ();
    }   // mesa is at least partial owner
    //=====  end of app-fsmDemo SimpleSal App stuff maybe not in original App "setup" function
}   // setup            // the App's original setup function
#undef setup
// the ssHL compiler has seen and compiled "App_setup", and has not yet seen anything called "setup".

// ---------------------------------------------------------------------------------------------
// The Ascii and LED resources are used in "delay" mode to demonstrate consumed processor time.
// ---------------------------------------------------------------------------------------------
// LED delay() is used to hold ON the LED for 1ms alternating with holding it OFF for 99ms.
// This simulates an App consuming processor instruction execution time, doing what it does.
// The time consumed is not attributed to Events being granted Agency, this is the App loop.
// ---------------------------------------------------------------------------------------------
// The result is that for every 2 calls (a pair) to "loop" function, 100ms of time is consumed.
// A longer LED blink is obtained by increasing the ON period and decreasing the OFF period.
// A slower LED blink is obtained by increasing the OFF period; processor time lost to delay.
// ---------------------------------------------------------------------------------------------
// If all of the time consumed by Events totaled 1ms, every second would see 10 pairs execute,
// each pair consuming 100ms and everything else consuming 1ms.  This is no way to keep Time,
// but it does create a visual demonstration of the effect of processor load using the MrB App.
// ---------------------------------------------------------------------------------------------
#define loop    App_loop
void loop (void)        // (the App's original loop function, unchanged, ssTEA calls as App_loop)
{
    AsciiA_t    App_Buff[APP_BUFFER_ALLOC];

    boolean     doTick1ms;          // within loop, once time every 1 millisecond, true.

    static int  LED_counter = 0;    // if drop initializer and use RAM as is, same FSM result?
    static int  Ascii_counter;      // let's see what happens...

    // ---------------------------------------------------------------------------------------------
    // The original App did not co-exist so it always owned all resources by itself.  Now, check it.
    // ---------------------------------------------------------------------------------------------
    // As a demonstration, LED delay inserts 99ms delay or 1ms delay after changing LED state.  The
    // software always exist here, it only is executed during a demonstration of the effect of delay.
    // ---------------------------------------------------------------------------------------------
    if (RO_IS_ONEOF_LED_ROs (RO_owner_App, RO_active_delay))
    {
        LED_counter++;
        if (LED_counter & 1)
        {
            mesa_uiOp_LEDon ();
            delay (1);
        }
        else
        {
            mesa_uiOp_LEDoff ();
            delay (99);
        }
    }   // LED owned by app-fsmDemo Delay

    // ---------------------------------------------------------------------------------------------
    // As a demonstration, Ascii delay inserts 100ms delay or writes value 0 to Serial.  The delay
    // software always exists here, it only is executed during a demo of the effect of delay.
    // ---------------------------------------------------------------------------------------------
    if (RO_IS_ONEOF_Ascii_ROs (RO_owner_App, RO_active_delay))
    {
        Ascii_counter++;
        if (Ascii_counter & 1)
        {
            Serial.write ((uint8_t) 0);
        }
        else
        {
            delay (100);
        }
    }   // ascii owned by app-fsmDemo

    // ---------------------------------------------------------------------------------------------
    // The number of microseconds from one loop to the next, tracked by ssTEA, is borrowed here. The
    // number of microseconds was computed by ssTEA_loop (called as "loop") before calling "App_loop".
    // This is a traditional Arduino loop function: ignoring ssTEA Time features, using us/ms counts
    // for math to measure the passage of Time.  The Loop1toLoop2 value was computed by ssTEA.
    // ---------------------------------------------------------------------------------------------
    CountOf_1usThis1ms += ssT_us_gLoop1toLoop2;     // ssTEA dependency for simple task you could do
    doTick1ms = false;
    if (CountOf_1usThis1ms > SST_USECS_PER_MSEC)
    {
        doTick1ms = true;
        CountOf_1usThis1ms = CountOf_1usThis1ms - SST_USECS_PER_MSEC;   // accumulate with slew

        mesa_gCtOf_1msFreeRunning++;
    }

    // ---------------------------------------------------------------------------------------------
    // Nothing happens in the App faster than 1 millisecond, purely a choice of app-fsmDemo itself.
    // ---------------------------------------------------------------------------------------------
    if (!doTick1ms)
    {
        return;   // completed this instance of loop
    }

    // ===============  start of "once every 1 millisecond accumulated across loops" section
    // -----------------------------------------------------------------------------------------
    // this application processes commands with parameters that originate from an input source.
    // Anything involving human interaction is indecipherable at 1ms, and too slow at 1 second.
    // This is why the LED and Ascii FSMs are based on 1ms intervals; humans can't perceive it.
    // -----------------------------------------------------------------------------------------
    // The Flash FSM goes through a series of off/on/off states to make the LED flash. Agency
    // is granted at every 1ms boundary detected by the Classic Timekeeping method used here.
    // The period and sequence of the flashing is determined by the FSM; loop only keeps Time,
    // using variables for keeping Classic Time; BUT perhaps the counter should be in the FSM?
    // -----------------------------------------------------------------------------------------
#ifdef MESA_MIN_OPTOUT_LED              // opted out of mesa_min implementation
    mesa_gCtOf_1mThisFlash++;                                                   // information hiding
    mesa_gCtOf_1mThisFlash = mesa_FSM_LEDflash_FSM (mesa_gCtOf_1mThisFlash);    // design for change

    // ---------------------------------------------------------------------------------------------
    // this is an example of modifying the core of ssTEA for the benefit of the App.  Do it.
    // ---------------------------------------------------------------------------------------------
    if (RO_IS_ONEOF_LED_ROs (RO_owner_App, RO_active_classic))
    {
        mesa_FSM_LEDflash_Req ();
    }
#endif // MESA_MIN_OPTOUT_LED           // opted out of mesa_min implementation

#ifdef MESA_MIN_OPTOUT_ASCII            // opted out of mesa_min implementation
    mesa_gCtOf_1mThisAscii++;                                                   // design for change
    mesa_gCtOf_1mThisAscii = mesa_FSM_AsciiEmit_FSM (mesa_gCtOf_1mThisAscii);   // information hiding
#endif // MESA_MIN_OPTOUT_ASCII         // opted out of mesa_min implementation

    // app-fsmDemo is comprised of the Apps and the ssUI interface to manipulate the Apps and ssTEA
    if (RO_IS_ONEOF_Ascii_ROs (RO_owner_App, RO_active_OneOfUIs_App))
    {
        // -----------------------------------------------------------------------------------------
        // Two things seemingly opposite may both be true at the same time:
        //
        //   It is not necessary to use the Ascii signal/sensor mesa software (or, the MAC), HOWEVER
        //
        //   SimpleSal (ssTEA, ssUI, ssIO, the App) inherently depends on a MAC for Ascii values.
        //
        // -----------------------------------------------------------------------------------------
        // Different terminal programs act differently about forwarding typed characters pre-CR.
        // The Ascii FSM is build-time adaptable to the styles. The task of collecting 1-N input
        // characters in the foreground or background as they arrive is up to the current UI Owner.
        // -----------------------------------------------------------------------------------------
        // If no Ascii characters are available, the buffer is not modified, user must initialize.
        // The length returned is the number of Ascii characters in the buffer available before NUL.
        // Throughout SimpleSal software, MAX_I for any buffer is also the size in bytes of memory.
        // -----------------------------------------------------------------------------------------
        // if App UI is 1) at least a partial owner and 2) currently active as a UI, consume input.
        // When the RO_owner is "anybody" and both UIs are active, a race condition is created.
        // This comment, with App uI and ssUI reversed, should be placed with other inOp_Line_From calls.
        // -----------------------------------------------------------------------------------------
        App_Buff[0] = Ascii_NUL;
        if (ssUI_inOp_Line_FromSerial (App_Buff, APP_BUFFER_MAX_I))
        {
            //=====  Start of app-fsmDemo SimpleSal App stuff maybe not in original App "loop" function

            mesa_uiOp_emit_pAsciiA (App_Buff);
            mesa_uiOp_emit_newline ();                      // simple App UI echoes all input

            // -------------------------------------------------------------------------------------
            // ssUI and App UI watch for the name of the other UI, while they are the UI owner.
            // If, at the start of the input, there is the name of the "other UI", the arbitration
            // algorithm in "App_Select_UI" is the FSM that decides who owns and changes ownership.
            // If, after the name of the "other UI", there are additional values, "App_Select_UI"
            // forwards the array of input values, without modification, to the ssUI HeyProcessThis.
            // -------------------------------------------------------------------------------------
            if (ssUI_BufferStartsWith_ssui (App_Buff))
            {

                App_Select_UI (App_Buff);       // based on owner now and active for that owner

            }   // input is "ssuixxxx"
            else
            {
                if (RO_IS_ONEOF_Ascii_ROs (RO_owner_App, RO_active_OneOfUIs_App))
                {

                    App_bpm (App_Buff);             // There is Only One App, and that App is "bpm".

                }   // App is owner
            }   // input is anything BUT "ssuixxx"

            // App BPM may have caused change in ownership
            if (RO_IS_ONEOF_Ascii_ROs (RO_owner_App, RO_active_OneOfUIs_App))
            {
                // see comment above about ssUI bracketing the prompt and output from ssUI
                mesa_uiOp_emit_1 (Ascii_squiggleLBracket);
                mesa_uiOp_emit_qAsciiA ("bpm");    // There is Only One App, and that App is "bpm".
                mesa_uiOp_emit_1 (Ascii_squiggleRBracket);
                mesa_uiOp_emit_1 (Ascii_Colon);
            }   // App is owner

            //=====  end of app-fsmDemo SimpleSal App stuff maybe not in original App "loop" function
        }   // received input
#ifdef MESA_MIN_OPTOUT_LED
        else
        {
    //       Serial.write ((uint8_t) '?');
        }
#endif // MESA_MIN_OPTOUT_LED           // opted out of mesa_min implementation
    }   // I_AM_OWNER

    // ===============  end of "once every 1 millisecond accumulated across loops" section
}   // loop             // the App's original loop function, known as "App_loop"
#undef  loop
// the ssHL compiler has seen and compiled "App_loop", and has not yet seen anything called "loop".


// =================================================================================================
// -------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------
#define ssTEA_setup  setup
void ssTEA_setup (void)         // the one called by Arduino Host OS Api using the name "setup"
{
    //=====  Start of application-specific stuff as part of original App "setup" function
    // ---------------------------------------------------------------------------------------------
    // Call the original App's "setup", which was renamed to "App_setup" when the compiler found it.
    // ---------------------------------------------------------------------------------------------
    App_setup ();
    //=====  end of application-specific stuff as part of original App "setup" function

#ifdef MESA_OPTIN_RANDOM_NUMBERS
    // ---------------------------------------------------------------------------------------------
    // ---------------------------------------------------------------------------------------------
   mesa_Random_InitState ();
#endif  // MESA_OPTIN_RANDOM_NUMBERS

    // ---------------------------------------------------------------------------------------------
    // This encapsulates all the App/ssTEA run-time configuration implementing the Init State.
    // For example, configure events and start Time and Agency in support of the App's use of ssTEA.
    // The starting position is determined by values in ssTEA_control, as the App set during setup.
    // This is the software that will be added to an arduino project to incorporate ssTEA and ssUI.
    // ---------------------------------------------------------------------------------------------
    App_ss_launch ();

    // ---------------------------------------------------------------------------------------------
    // The App is now up and running, ssTEA is configured and running as configured: "hello, world".
    // ---------------------------------------------------------------------------------------------
    App_IThink_Therefore_IAm ();

} // ssTEA_setup              // the one called by Arduino Host OS Api using the name "setup"
#undef ssTEA_setup
// the ssHL compiler has seen and compiled "App_setup" and "setup"

// =================================================================================================
// "App_loop" is the original Arduino App's "loop" function, which will now be granted agency
// by ssTEA's "loop".  This was done by a #define trick here that renames the original App "loop"
// to "App_loop" and "ssTEA_loop" to "loop".  The result is the linker sees two functions:
// "App_loop" and "loop".  Instructions may be found elsewhere describing how to modify Original App.
// =================================================================================================
#undef loop
#define ssTEA_loop loop

void ssTEA_loop (void)
{
    // ---------------------------------------------------------------------------------------------
    // Time is measured at this point in each loop iteration; Time1 is then, Time2 is now.  The
    // amount of time elapsed between Time2 (now) and Time1 (then) is the basis of Time accounting.
    // ---------------------------------------------------------------------------------------------
    // the difference between being here at this point in Time (Time2) and being here at the last
    // point in Time (Time1) is the total Time consumed by software: Host, app Loop, and ssTEA.
    // Obviously that accounts for almost all Time, but what we are interested in is a delta value.
    // ssTEA statistics will break the total time into execution times for Host, ssTEA, and Loop.
    // Remember that interrupts consume Time by stealing it from everyone; background processing
    // (interrupts, serial input) consumes Time while in the Host (the App gives up Agency for this).
    // ---------------------------------------------------------------------------------------------
    // Additional math captures and calculations occur at the end of "loop", after the application.
    // ---------------------------------------------------------------------------------------------
    ssTEA_Time_MathEntry ();

    // ---------------------------------------------------------------------------------------------
    // Agency Root is always called as often as loop is called, it manages the Time and Agency FSMs
    // using the value of "Time1, the Time of the last call" and "Time2, the Time of this call".
    // The information that Agency Root uses (T1toT2) was established by ssTEA_Time_MathEntry ().
    // The information is supplied to Agency Root in anticipation of virtual Time manipulating data.
    // In addition, Agency Root does not depend upon the way Time values are arrived at or computed.
    // ---------------------------------------------------------------------------------------------
    ssTEA_Agency_Root (ssT_us_gLoop1toLoop2, ssT_ms_gLoop1toLoop2);

    // ---------------------------------------------------------------------------------------------
    // ssT is done with Time, except: as near to return as possible, capture Time for accounting.
    // ---------------------------------------------------------------------------------------------

    // ---------------------------------------------------------------------------------------------
    // It is possible to move the App's loop processing to precede ssTEA's Agency Root logic,
    // but then the results and information generated by Events would not be available to the App
    // until the next time that App_loop is called by ssTEA loop.  This is the user's choice.
    // The gLoop1toLoop2 values are calculated by LoopMath_Entry/Exit, App_loop must be within them.
    // ---------------------------------------------------------------------------------------------
    App_loop ();

    // ---------------------------------------------------------------------------------------------
    // While the App_loop's check of the UI may be at 1 millisecond intervals, the ssUI Main is not.
    // ---------------------------------------------------------------------------------------------
    // The ssUI menu FSM decides whether to pay attention or take action based on rules it made up.
    // The FSM must be given Agency at some rate, not because of the UI's dependency on ownership
    // of the Ascii resource, but because ssUI has a feature that executes an array of commands that
    // have been built into ssTEA+ssUI+App.  ssUI may execute only one command per Agency grant, or,
    // ssUI may execute the entire array in one Agency grant.  The side effect of executing all of
    // the commands in the array in one Agency grant is that they all will occur at the same Time.
    // ---------------------------------------------------------------------------------------------
    ssUI_Main ();

    // ---------------------------------------------------------------------------------------------
    // No Time should be wasted here explaining or apologizing; do the math and find the way home.
    // ---------------------------------------------------------------------------------------------
    ssTEA_Time_MathExit ();

} // ssTEA_loop
#undef ssTEA_loop
// the ssHL compiler has seen and compiled "App_loop" and "loop"

// =================================================================================================
// -------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------
void   App_BuildOp_Show_Build (void);

// -------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------
void App_ss_launch (void)
{
    // ---------------------------------------------------------------------------------------------
    // ssTEA and ssUI are educational tools, they can blather on and on while performing the task.
    // ---------------------------------------------------------------------------------------------
    ssTEA_control.Show_Path = false;
    ssTEA_control.Show_Cause = false;
    ssTEA_control.Show_Signals = false;

    // ---------------------------------------------------------------------------------------------
    // ssIO creates virtual and physical MAC management data structures before App and ssTEA exist.
    // ---------------------------------------------------------------------------------------------
    mesa_uiOp_emit_qAsciiA ("<ssIO mesa init...");
    ssIO_MAC_InitState ();
    mesa_uiOp_emit_qAsciiA ("success>");
    mesa_uiOp_emit_newline ();

    // ---------------------------------------------------------------------------------------------
    // Unless ssTEA is the current software executing right now, any changes to control values are
    // instantaneous, and accepted once executing, with no checking of value(s) before using them.
    // It is possible to create a configuration that is confused by its own incoherent definition.
    // ---------------------------------------------------------------------------------------------
    ssTEA_control.Time_state = ssTEA_state_stopped;
    ssTEA_control.Time_base = ssTEA_Time_base_reality;
    ssTEA_control.Agency_state = ssTEA_state_stopped;
    ssTEA_control.Agency_pace = App_Default_Agency_Pace;
    ssTEA_control.Agency_period = App_Default_Agency_Pace_period;

    // if a hard fault occurs in ssTEA, the serial output may not get a chance, can wait for done
    mesa_uiOp_emit_qAsciiA ("<ssTEA init...");
    ssTEA_InitState ();
    mesa_uiOp_emit_qAsciiA ("success>");
    mesa_uiOp_emit_newline ();

    mesa_uiOp_emit_qAsciiA ("<ssUI init...");
    ssUI_InitState ();
    mesa_uiOp_emit_qAsciiA ("success>");
    mesa_uiOp_emit_newline ();

#ifdef SIMPLESAL_APP_OPTIN_AUTOSTART_SSTEA_FSMS
    ssTEA_control.Time_state = ssTEA_state_running;
    ssTEA_control.Agency_state = ssTEA_state_running;
#else
    ssTEA_control.Time_state = ssTEA_state_stopped;
    ssTEA_control.Agency_state = ssTEA_state_stopped;
#endif  // SIMPLESAL_APP_OPTIN_AUTOSTART_SSTEA_FSMS

    if (ssUI_control.BeVerbose == YES_BE_VERBOSE)
    {
        // ---------------------------------------------------------------------------------------------
        // the menu handler FSM for the "ss ro" command is told "1st token matched 'ss'; 2nd 'ro'; there
        // are no more tokens".  There is no data to support this; the parameters describe an FSM state.
        // This function has to be called AFTER the Init state (at build or run time) to be accurate.
        // The result of the call to the function 'as if a user typed "ss ro"' is state data displayed.
        // ---------------------------------------------------------------------------------------------
        mesa_uiOp_emit_qAsciiA ( "--- mesa resource ownership run-time ---");
        ss_uiOp_emit_newline ();
        H_ss_RO (ssUI_tkn2nd_i, false);
        ss_uiOp_emit_newline ();
    }   // be verbose

    // ---------------------------------------------------------------------------------------------
    // This helpful software displays the settings the compiler saw when this software was built.
    // ---------------------------------------------------------------------------------------------
    App_BuildOp_Show_Build ();

} // App_ss_launch
// =================================================================================================
// -------------------------------------------------------------------------------------------------
// An algorithm seems a little stupid in that it doesn't know whether it is ssUI changing to App UI
// or the App UI changing to ssUI.  The algorithm only changes to a UI that is Owned and Active.
// Only the word "app" or "ssui", typed into the App UI or the ssUI, changes the focus of the UI.
// -------------------------------------------------------------------------------------------------
// The canonical "if owner and active" macros are not used here, to require specific references.
// -------------------------------------------------------------------------------------------------
void App_Select_UI (pAsciiA_t  pReceivedInput)
{
    if (ssUI_BufferStartsWith_app (pReceivedInput))
    {
        if (ssUI_BufferOnlyHas_app (pReceivedInput))
        {
            // "app" alone means "change the focus of the UI from ssUI to the App:
            switch (Ascii_ROs)
            {
                case RO_owner_ssUI :
                    if (Ascii_RO_active & RO_active_OneOfUIs_App)
                    {
                        ss_uiOp_emit_qBanner (lfN, "--  from ssUI focus, changing focus to App. Enter 'app' again to generate a prompt.", lfY);
                        Ascii_ROs = RO_owner_App;
                    }
                    else
                    {
                        ss_uiOp_emit_qBanner (lfN, "app is not UI focus", lfY);
                    }
                    break;
                case RO_owner_App :
                    ss_uiOp_emit_qBanner (lfN, "app is UI focus", lfY);
                    break;
                case RO_owner_Anybody :
                    ss_uiOp_emit_qBanner (lfN, "anybody is UI focus!", lfY);
                    break;
                case RO_owner_Nobody  :
                    ss_uiOp_emit_qBanner (lfN, "nobody is UI focus?", lfY);
                    break;
            }   // switch Ascii_ROs
        }   // "app" only
        else
        {   // "app xxx" means change the focus only by temporarily granting agency and permission
            // "appxxx" is not handled by "app"
            if (*(pReceivedInput+3) == Ascii_Space)         // first pointer math, then dereference
            {
                ss_uiOp_emit_qBanner (lfN, "giving app HeyProcessThis [", lfN);
                mesa_uiOp_emit_pAsciiA (pReceivedInput+4);
                ss_uiOp_emit_qBanner (lfN, "]", lfY);
                App_HeyProcessThis (pReceivedInput+4);
            }
        }   // "app" or "app xxx" or "appexxx" all handled
        return;                         // each match returns when done
    }   // starts with app

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

    // "ssui" alone means "change the focus of the UI from the App to ssUI"
    if (ssUI_BufferStartsWith_ssui (pReceivedInput))
    {
        if (ssUI_BufferOnlyHas_ssui (pReceivedInput))
        {
            // "ssui" only
            switch (Ascii_ROs)
            {
                case RO_owner_ssUI :
                    ss_uiOp_emit_qBanner (lfN, "ssui is UI focus", lfY);
                    break;
                case RO_owner_App :
                    if (Ascii_RO_active & RO_active_OneOfUIs_ss)
                    {
                        ss_uiOp_emit_qBanner (lfY, "--  from App focus, changing focus to ssui. Enter 'ssui' again to generate a prompt.", lfY);
                        Ascii_ROs = RO_owner_ssUI;
                    }
                    else
                    {
                        ss_uiOp_emit_qBanner (lfY, "ssui is not active", lfY);
                    }
                    break;
                case RO_owner_Anybody :
                    ss_uiOp_emit_qBanner (lfY, "anybody is UI focus!", lfY);
                    break;
                case RO_owner_Nobody  :
                    ss_uiOp_emit_qBanner (lfY, "nobody is UI focus?", lfY);
                    break;
            }   // switch Ascii_ROs
        }   // "ssui" only
        else
        {   // "ssui xxx" means change the focus only by temporarily granting agency and permission
            // "ssuixxx" is not handled by "ssui"
            if (*(pReceivedInput+4) == Ascii_Space)         // first pointer math, then dereference
            {
                ss_uiOp_emit_qBanner (lfN, "giving ssui HeyProcessThis [", lfN);
                mesa_uiOp_emit_pAsciiA (pReceivedInput+5);
                ss_uiOp_emit_qBanner (lfN, "]", lfY);
                ssUI_HeyProcessThis (pReceivedInput+5);
            }
        }
        return;                         // each match returns when done
    }   // does not start with "ssui"

} // App_Select_UI
// =================================================================================================
// -------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------
void App_IThink_Therefore_IAm (void)
{
#ifdef SIMPLESAL_APP_OPTIN_TIMECURR_FROM_TIMEBUILT
    AsciiA_t    localBuff[APP_BUFFER_ALLOC];
#endif  // SIMPLESAL_APP_OPTIN_TIMECURR_FROM_TIMEBUILT

    // ---------------------------------------------------------------------------------------------
    // ssTEA puts the build time into a static Time variable called TimeOfBuild, calls it a version:
    // this build has this version number. The Time uniquely identifies a collection of features,
    // the collection has features enabled/disabled and configurations defined in a formal way.
    // Finally, the formal way everything is defined is unique to the source files at Time of Build.
    // ---------------------------------------------------------------------------------------------
#ifdef SIMPLESAL_APP_OPTIN_TIMECURR_FROM_TIMEBUILT
    if (ssT_stampOp_TimeOfBuild_AsciiToTime (localBuff))
    {
        ssT_mathOp_P1getsP2 (ssT_gpTime_Current, ssT_gpTime_Built);
        mesa_uiOp_emit_qAsciiA ("Time built:");
        ss_uiOp_Show_Time (ssT_gpTime_Built, lfY);
    }
#endif  // SIMPLESAL_APP_OPTIN_TIMECURR_FROM_TIMEBUILT

    // ---------------------------------------------------------------------------------------------
    // What Time is it now, after initialization?  What is the value of ssT_gTimeCurrent?
    // ---------------------------------------------------------------------------------------------
    ss_uiOp_Show_ssTime_state ();
    ss_uiOp_Show_ssAgency_state ();
}   // App_IThink_Therefore_IAm
// =================================================================================================
// -------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------
boolean App_isItTimeToStop (ssT_pTime_t pCurrentTime, ssT_pTime_t pTimeToStop)
{
    boolean isTimeToStop;

#ifdef APP_MRB_OPTIN_ANNOUNCE_TIMETOSTOP
    mesa_uiOp_emit_qAsciiA ("--   Is <the current Time> equal to or after <the Time to stop>?");
    mesa_uiOp_emit_newline ();
    mesa_uiOp_emit_qAsciiA ("--   (the current Time)");
    ss_uiOp_Show_Time (pCurrentTime, lfY);
    mesa_uiOp_emit_newline ();
    mesa_uiOp_emit_qAsciiA ("--   (the Time to stop)");
    ss_uiOp_Show_Time (pTimeToStop, lfY);
#endif  // APP_MRB_OPTIN_ANNOUNCE_TIMETOSTOP

    // ---------------------------------------------------------------------------------------------
    //
    // ---------------------------------------------------------------------------------------------
    if (ssT_timeIf_P1equalsORafterP2 (pCurrentTime, pTimeToStop))
    {
        isTimeToStop = true;
#ifdef APP_MRB_OPTIN_ANNOUNCE_TIMETOSTOP
        mesa_uiOp_emit_qAsciiA ("<YES> the current Time is equal to or after the Time to stop");
#endif  // APP_MRB_OPTIN_ANNOUNCE_TIMETOSTOP
        // FSM reached termination condition, matching the stop time requested to the current time
    }   // the current Time has reached the Time to stop
    else
    {   // let's keep going
        isTimeToStop = false;
        // calculate/create valid values for the bit values and for the indicator the bit changed
#ifdef APP_MRB_OPTIN_ANNOUNCE_TIMETOSTOP
        mesa_uiOp_emit_qAsciiA ("<NO> the current time is before the Time to stop");
#endif  // APP_MRB_OPTIN_ANNOUNCE_TIMETOSTOP
    }   // let's keep going

    return (isTimeToStop);
}   // App_isItTimeToStop
// =================================================================================================
// -------------------------------------------------------------------------------------------------
// This is a shortcut that requires maintenance over time: display the actual build options selected.
// This model of ifdef-to-select-code makes same function available to every build, only varies here.
// All build-selecting choices must have been made by this time; a change later makes this wrong.
// -------------------------------------------------------------------------------------------------
// A difference from most instances of ifdef/endif in SimpleSal: became hard to read, very busy.
// -------------------------------------------------------------------------------------------------
void    App_BuildOp_Show_Build (void)
{
#ifdef SS_OPTIN_SHOW_BUILD_OPTIONS

#ifdef MESA_OPTIN_RANDOM_NUMBERS
    {   ss_uiOp_emit_qAsciiA ("MESA_OPTIN_RANDOM_NUMBERS");  ss_uiOp_emit_newline (); }
#endif
#ifdef MESA_MIN_OPTOUT_RANDOM_NUMBERS
    {   ss_uiOp_emit_qAsciiA ("MESA_MIN_OPTOUT_RANDOM_NUMBERS");  ss_uiOp_emit_newline (); }
#endif
#ifdef MESA_MIN_OPTOUT_ASCII
    {   ss_uiOp_emit_qAsciiA ("MESA_MIN_OPTOUT_ASCII");  ss_uiOp_emit_newline (); }
#endif
#ifdef MESA_MIN_OPTOUT_LED
    {   ss_uiOp_emit_qAsciiA ("MESA_MIN_OPTOUT_LED");    ss_uiOp_emit_newline (); }
#endif

#ifdef SS_ACCEPT_SIMPLEST_CONFIG
    {   ss_uiOp_emit_qAsciiA ("SS_ACCEPT_SIMPLEST_CONFIG");  ss_uiOp_emit_newline (); }
#endif
#ifdef  SST_ONEOF_TIME_IS_AN_ARROW
    {   ss_uiOp_emit_qAsciiA ("SST_ONEOF_TIME_IS_AN_ARROW");  ss_uiOp_emit_newline (); }
#endif
#ifdef  SST_ONEOF_TIME_IS_AN_FSM
    {   ss_uiOp_emit_qAsciiA ("SST_ONEOF_TIME_IS_AN_FSM");  ss_uiOp_emit_newline (); }
#endif
#ifdef  SSE_OPTIN_ABOUTEVDB_TRUSTED_APP
    {   ss_uiOp_emit_qAsciiA ("SSE_OPTIN_ABOUTEVDB_TRUSTED_APP");  ss_uiOp_emit_newline (); }
#endif
#ifdef  SSE_ONEOF_ABOUTEVDB_ALLOC_CT
    {
        ss_uiOp_emit_qAsciiA ("SSE_ONEOF_ABOUTEVDB_ALLOC_CT ");
        ss_uiOp_emit_Int_99 (SSE_ONEOF_ABOUTEVDB_ALLOC_CT);
        ss_uiOp_emit_newline ();
    }
#endif
#ifdef  SSE_ONEOF_ABOUTEVDB_ALLOC_BY_APP
    {   ss_uiOp_emit_qAsciiA ("SSE_ONEOF_ABOUTEVDB_ALLOC_BY_APP");  ss_uiOp_emit_newline (); }
#endif
#ifdef  SSE_ONEOF_ABOUTEVDB_ALLOC_BY_SSE
    {   ss_uiOp_emit_qAsciiA ("SSE_ONEOF_ABOUTEVDB_ALLOC_BY_SSE");  ss_uiOp_emit_newline (); }
#endif
#ifdef  SSA_OPTIN_STATS_FSM
    {   ss_uiOp_emit_qAsciiA ("SSA_OPTIN_STATS_FSM");  ss_uiOp_emit_newline (); }
#endif
#ifdef  SSA_OPTIN_STATS_APIFSM
    {   ss_uiOp_emit_qAsciiA ("SSA_OPTIN_STATS_APIFSM");  ss_uiOp_emit_newline (); }
#endif
#ifdef  SSA_OPTIN_STATS_RUNFSM
    {   ss_uiOp_emit_qAsciiA ("SSA_OPTIN_STATS_RUNFSM");  ss_uiOp_emit_newline (); }
#endif
#ifdef  SST_OPTIN_TIME_COMPRESSED_TUS
    {   ss_uiOp_emit_qAsciiA ("SST_OPTIN_TIME_COMPRESSED_TUS");  ss_uiOp_emit_newline (); }
#endif
#ifdef  SST_OPTIN_MATH_IS_TRUSTED
    {   ss_uiOp_emit_qAsciiA ("SST_OPTIN_MATH_IS_TRUSTED");  ss_uiOp_emit_newline (); }
#endif
#ifdef  SST_OPTIN_TIME_NSECS_TU
    {   ss_uiOp_emit_qAsciiA ("SST_OPTIN_TIME_NSECS_TU");  ss_uiOp_emit_newline (); }
#endif
#ifdef  SST_ONEOF_TIME_NSECS_RANDOM
    {   ss_uiOp_emit_qAsciiA ("SST_ONEOF_TIME_NSECS_RANDOM");  ss_uiOp_emit_newline (); }
#endif
#ifdef  SST_ONEOF_TIME_NSECS_ZERO
    {   ss_uiOp_emit_qAsciiA ("SST_ONEOF_TIME_NSECS_ZERO");  ss_uiOp_emit_newline (); }
#endif
#ifdef  SST_ONEOF_TIME_NSECS_INCR
    {   ss_uiOp_emit_qAsciiA ("SST_ONEOF_TIME_NSECS_INCR");  ss_uiOp_emit_newline (); }
#endif
#ifdef  SST_ONEOF_TIME_PLANCKS_RANDOM
    {   ss_uiOp_emit_qAsciiA ("SST_ONEOF_TIME_PLANCKS_RANDOM");  ss_uiOp_emit_newline (); }
#endif
#ifdef  SST_ONEOF_TIME_PLANCKS_ZERO
    {   ss_uiOp_emit_qAsciiA ("SST_ONEOF_TIME_PLANCKS_ZERO");  ss_uiOp_emit_newline (); }
#endif
#ifdef  SST_ONEOF_TIME_PLANCKS_INCR
    {   ss_uiOp_emit_qAsciiA ("SST_ONEOF_TIME_PLANCKS_INCR");  ss_uiOp_emit_newline (); }
#endif
#ifdef  SSA_OPTIN_AGENCY_DEBUG
    {   ss_uiOp_emit_qAsciiA ("SSA_OPTIN_AGENCY_DEBUG"); ss_uiOp_emit_newline (); }
#endif
#ifdef  SSUI_OPTIN_INCLUDE_CMDS_IN_SSUI_APP
    {   ss_uiOp_emit_qAsciiA ("SSUI_OPTIN_INCLUDE_CMDS_IN_SSUI_APP");  ss_uiOp_emit_newline (); }
#endif
#ifdef  SSUI_OPTIN_INCLUDE_CMDS_IN_SSUI_SSUI
    {   ss_uiOp_emit_qAsciiA ("SSUI_OPTIN_INCLUDE_CMDS_IN_SSUI_SSUI");  ss_uiOp_emit_newline (); }
#endif
#ifdef  SSUI_ONEOF_SERIAL_RECEIVES_LINES
    {   ss_uiOp_emit_qAsciiA ("SSUI_ONEOF_SERIAL_RECEIVES_LINES");  ss_uiOp_emit_newline (); }
#endif
#ifdef  SSUI_ONEOF_SERIAL_RECEIVES_BYTES
    {   ss_uiOp_emit_qAsciiA ("SSUI_ONEOF_SERIAL_RECEIVES_BYTES");  ss_uiOp_emit_newline (); }
#endif

#endif // SS_OPTIN_SHOW_BUILD_OPTIONS

}   // App_BuildOp_Show_Build

// end of file: app-fsmDemo.ino



