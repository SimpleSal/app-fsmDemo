/* -- SimpleSal: ssTEA, ssUI, and ssIO | (C) 2025, 2026 TruSoft Computing LLC |   All rights reserved. --
   This software is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
   This software is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
   ---------------------------------------------------------------------------------------------------
 *
 * app_fifos_ino.h      data and software definitions: application fifos demonstration
 *
 * 2026/10/10   "fsmDemo.ino" arduino App udpate release version of SimpleSal as "app-fsmDemo.ino".
 * 2025/08/06   special version appeared as the star of first videos published at SimpleSal substack.
 * 2024/05/27   created superset FSM Demo Application as basis for ssTEA and SimpleSal dev/test.
 **************************************************************************************************/
#ifndef _APP_FIFOS_INO_H
#define _APP_FIFOS_INO_H

// =================================================================================================
// -------------------------------------------------------------------------------------------------
// fifos       Not an App; instead a separation of slow-Device interactions from the full-speed App.
//             A complex demonstration of SimpleSal App co-existence using Finite State Machines.
// -------------------------------------------------------------------------------------------------

// =================================================================================================
// -------------------------------------------------------------------------------------------------
// The calculation of the period for the Timed Event that catches incoming data involves
//      the Production Rate (how many bytes will be produced in one time period, such as second?)
//      the Agency Pace Period (how often will an Event agencying based on Time get a chance to run?)
//      the Recurrence Period (how often does the Event want to run, when ssE thinks about it)
//      and the amount of physical memory the App is willing to commit to the Buffer itself.
// Within fsm_demo, devices producing a flow of bineary data (unjudged by value) follow this rule:
//     when the device indicates that there is data ready to be retrieved by the Event,
//          all of the data in the device must be retrieved from the device during that Event.
// -------------------------------------------------------------------------------------------------
// "incoming" Data will overflow the device's receive resources if data produced is read too slowly.
// Fifos move the burden of managing data overflows into fifo's domain, away from the device domain.
// This is another way of saying that a cardinal sin is to lose data in the device itself. Bad.
// This is represented in the data about the signal data: LostDataAtSomePointSinceYouLastChecked.
// -------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------
#define   CONSUMER_BUFFER_ALLOC   (256)
#define   CONSUMER_BUFFER_MAX_I   (CONSUMER_BUFFER_ALLOC-1)
#define   PRODUCER_BUFFER_ALLOC   (256)
#define   PRODUCER_BUFFER_MAX_I   (PRODUCER_BUFFER_ALLOC-1)

// "burstCt" reference use the Time Variable's msecs value as a parameter in range 0..999.
#define fifos_param_producer_burstCt            ssUI_gpTimeA
#define fifos_param_consumer_burstCt            ssUI_gpTimeB

// "period" and "cycle" references use the Time Variable as a Time value.
#define fifos_param_producer_service_period     ssUI_gpTimeC
#define fifos_param_fifo_service_period         ssUI_gpTimeD
#define fifos_param_cycle_startsIn              ssUI_gpTimeE
#define fifos_param_cycle_Length                ssUI_gpTimeF

ssT_Time_t  fifos_Computed_StartTime;
ssT_pTime_t fifos_pComputedStartTime = &fifos_Computed_StartTime;
ssT_Time_t  fifos_Computed_StopTime;
ssT_pTime_t fifos_pComputed_StopTime = &fifos_Computed_StopTime;

// -------------------------------------------------------------------------------------------------
#define fifos_default_producer_service_period         S("10ms")
#define fifos_default_fifo_service_period             S("10ms")
#define fifos_default_producer_burstCt                S("10ms")
#define fifos_default_consumer_burstCt                S("20ms")
#define fifos_default_cycle_startsIn                  S("10sc")
#define fifos_default_cycle_Length                    S("30sc")

void App_fifos_Show_Choices (void)
{
    ss_uiOp_emit_qBanner (lfY, "How to pass values from ssUI's command line to 'fifos' event software:",  lfN);
    ss_uiOp_emit_qBanner (lfY, "  Assign nonzero values to Time variables; zero value selects the [default]  ", lfN);
    ss_uiOp_emit_qBanner (lfY, "   timea : producer recurrence period (read device data into 8-bit) ",    lfN);
    mesa_uiOp_emit_1 (Ascii_squareLBracket);
    ss_uiOp_emit_pBanner (lfN, fifos_default_producer_service_period, lfN);
    mesa_uiOp_emit_1 (Ascii_squareRBracket);
    ss_uiOp_emit_qBanner (lfY, "   timeb : consumer recurrence period (read 8-bit data into Ascii)  ",    lfN);
    mesa_uiOp_emit_1 (Ascii_squareLBracket);
    ss_uiOp_emit_pBanner (lfN, fifos_default_fifo_service_period, lfN);
    mesa_uiOp_emit_1 (Ascii_squareRBracket);
    ss_uiOp_emit_qBanner (lfY, "   timec : ms field indicates producer's burst count each agency    ",    lfN);
    mesa_uiOp_emit_1 (Ascii_squareLBracket);
    ss_uiOp_emit_pBanner (lfN, fifos_default_producer_burstCt, lfN);
    mesa_uiOp_emit_1 (Ascii_squareRBracket);
    ss_uiOp_emit_qBanner (lfY, "   timed : ms field indicates consumer's burst count each agency    ",    lfN);
    mesa_uiOp_emit_1 (Ascii_squareLBracket);
    ss_uiOp_emit_pBanner (lfN, fifos_default_consumer_burstCt, lfN);
    mesa_uiOp_emit_1 (Ascii_squareRBracket);
    ss_uiOp_emit_qBanner (lfY, "   timee : Time added to current Time to find cycle Start Time      ",    lfN);
    mesa_uiOp_emit_1 (Ascii_squareLBracket);
    ss_uiOp_emit_pBanner (lfN, fifos_default_cycle_startsIn, lfN);
    mesa_uiOp_emit_1 (Ascii_squareRBracket);
    ss_uiOp_emit_qBanner (lfY, "   timef : Time added to cycle Start time to find cycle End Time    ",     lfN);
    mesa_uiOp_emit_1 (Ascii_squareLBracket);
    ss_uiOp_emit_pBanner (lfN, fifos_default_cycle_Length, lfN);
    mesa_uiOp_emit_1 (Ascii_squareRBracket);
}   // App_fifos_Show_Choices
// -------------------------------------------------------------------------------------------------
ssE_EvResult_t App_fifos_ParamsOrDefaults (void)
{
    // ------------------------------------------------------------------------------------------
    if (ssT_timeif_P1equalsZero (fifos_param_producer_service_period))
    {
        if (!ssT_stampOp_AsciiToBinary (fifos_default_producer_service_period, fifos_param_producer_service_period))
        {
            return (ssE_EvResult_notOK_data);
        }
    }
    if (ssT_timeif_P1equalsZero (fifos_param_fifo_service_period))
    {
        if (!ssT_stampOp_AsciiToBinary (fifos_default_fifo_service_period, fifos_param_fifo_service_period))
        {
            return (ssE_EvResult_notOK_data);
        }
    }
    if (ssT_timeif_P1equalsZero (fifos_param_producer_burstCt))
    {
        if (!ssT_stampOp_AsciiToBinary (fifos_default_producer_burstCt, fifos_param_producer_burstCt))
        {
            return (ssE_EvResult_notOK_data);
        }
    }
    if (ssT_timeif_P1equalsZero (fifos_param_consumer_burstCt))
    {
        if (!ssT_stampOp_AsciiToBinary (fifos_default_consumer_burstCt, fifos_param_consumer_burstCt))
        {
            return (ssE_EvResult_notOK_data);
        }
    }
    if (ssT_timeif_P1equalsZero (fifos_param_cycle_startsIn))
    {
        if (!ssT_stampOp_AsciiToBinary (fifos_default_cycle_startsIn, fifos_param_cycle_startsIn))
        {
            return (ssE_EvResult_notOK_data);
        }
    }
    ss_uiOp_Show_Time (fifos_param_cycle_Length, lfY);
    if (ssT_timeif_P1equalsZero (fifos_param_cycle_Length))
    {
        if (!ssT_stampOp_AsciiToBinary (fifos_default_cycle_Length, fifos_param_cycle_Length))
        {
            mesa_uiOp_emit_newline ();
            return (ssE_EvResult_notOK_data);
        }
        ss_uiOp_Show_Time (fifos_param_cycle_Length, lfY);
    }
    return (ssE_EvResult_OK_go);
}   //  App_fifos_ParamsOrDefaults
// -------------------------------------------------------------------------------------------------
// An example of an application coexisting with SimpleSal, ssUI, and ssTEA.  Values entered into
// variables named by ssUI can be accessed: provide a name-to-variable mapping like time variables.
// -------------------------------------------------------------------------------------------------
ssE_EvResult_t App_fifos_getParams (void)
{
    if (
        (ssT_timeif_P1equalsZero (fifos_param_producer_service_period)) &&
        (ssT_timeif_P1equalsZero (fifos_param_fifo_service_period)) &&
        (ssT_timeif_P1equalsZero (fifos_param_producer_burstCt)) &&
        (ssT_timeif_P1equalsZero (fifos_param_consumer_burstCt)) &&
        (ssT_timeif_P1equalsZero (fifos_param_cycle_startsIn)) &&
        (ssT_timeif_P1equalsZero (fifos_param_cycle_Length))
       )
    {   // if all params left at default, may be cry for help
        App_fifos_Show_Choices ();
    }

    if (App_fifos_ParamsOrDefaults () == ssE_EvResult_notOK_data)
    {
        return (ssE_EvResult_notOK_data);
    }

    ss_uiOp_emit_qBanner (lfY, "fifos computation bug:", lfY);
    ssT_mathOp_P1getsP2plusP3 (fifos_pComputedStartTime, ssT_gpTime_Current, fifos_param_cycle_startsIn);
    ss_uiOp_Show_Time (fifos_pComputedStartTime, lfY);
    ss_uiOp_Show_Time (fifos_param_cycle_Length, lfY);
    ssT_mathOp_P1getsP2plusP3 (fifos_pComputed_StopTime, fifos_pComputedStartTime, fifos_param_cycle_Length);
    ss_uiOp_Show_Time (fifos_pComputed_StopTime, lfY);

#ifdef FIFOS_OPTIN_ANNOUNCE
    ss_uiOp_emit_qBanner (lfY, "Setting App parameters into Time variables based on Current Time:", lfY);
    ss_uiOp_Show_Time (ssT_gpTime_Current, lfN);
    ss_uiOp_emit_qBanner (lfY, "      burst Period for Device  (timea)    ", lfN);
    ss_uiOp_Show_Time (fifos_param_producer_service_period, lfY);
    ss_uiOp_emit_qBanner (lfY, "      burst Period for fifo    (timeb)    ", lfN);
    ss_uiOp_Show_Time (fifos_param_fifo_service_period, lfN);
    ss_uiOp_emit_qBanner (lfY, "      Producer burst count     (timec)    ", lfN);
    ss_uiOp_Show_Time (fifos_param_producer_burstCt, lfN);
    ss_uiOp_emit_qBanner (lfY, "      Consumer burst count     (timed)    ", lfN);
    ss_uiOp_Show_Time (fifos_param_consumer_burstCt, lfN);
    ss_uiOp_emit_qBanner (lfY, "      Time until Start         (timee)    ", lfN);
    ss_uiOp_Show_Time (fifos_param_cycle_startsIn, lfN);
    ss_uiOp_emit_qBanner (lfY, "      Time to Start (CurrentTime + timee) ", lfN);
    ss_uiOp_Show_Time (fifos_pComputedStartTime, lfN);
    ss_uiOp_emit_qBanner (lfY, "      cycle for length of Time (timef)    ", lfN);
    ss_uiOp_Show_Time (fifos_param_cycle_Length, lfN);
    ss_uiOp_emit_qBanner (lfY, "      Time to Stop (Time to Start + timef)", lfN);
    ss_uiOp_Show_Time (fifos_pComputed_StopTime, lfY);
    mesa_uiOp_emit_newline ();
#endif  // FIFOS_OPTIN_ANNOUNCE

    return (ssE_EvResult_OK_go);
}   // App_fifos_getParams
// -------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------
void  devSerial_FromDevTofifo_UpTo (ssIO_pFifo_t pTofifo, int HowMany)
{
#ifdef APP_ONEOF_FIFO_SOURCE_DEV_RANDOM
    int avail_ct = HowMany;
#endif  // APP_ONEOF_FIFO_SOURCE_DEV_RANDOM

#ifdef APP_ONEOF_FIFO_SOURCE_DEV_ASCII
    int avail_ct = mesa_uiOp_recv_avail_ct ();
#endif  // APP_ONEOF_FIFO_SOURCE_DEV_ASCII

    Incr_fifoStat (pTofifo, Transfer_Ct_OK);

    while (true)
    {
#ifdef APP_ONEOF_FIFO_SOURCE_DEV_RANDOM
        if (avail_ct <= 0)                                          break;
#endif  // APP_ONEOF_FIFO_SOURCE_DEV_RANDOM
        if (HowMany <= 0)                                           break;

        if (ssIO_fifoOp_State (pTofifo) == ssIO_fifo_State_FullData)       break;

#ifdef APP_ONEOF_FIFO_SOURCE_DEV_RANDOM
        ssIO_fifoOp_Put_LastIn (pTofifo, mesa_Random_NextFrom256 ());
#endif  // APP_ONEOF_FIFO_SOURCE_DEV_RANDOM
#ifdef APP_ONEOF_FIFO_SOURCE_DEV_ASCII
        ssIO_fifoOp_Put_LastIn (pTofifo, mesa_uiOp_recv_1 ());
#endif  // APP_ONEOF_FIFO_SOURCE_DEV_ASCII

#ifdef APP_ONEOF_FIFO_SOURCE_DEV_RANDOM
        avail_ct--;
#endif  // APP_ONEOF_FIFO_SOURCE_DEV_RANDOM
        HowMany--;
    }   // while
}   // devSerial_FromDevTofifo_UpTo
// -------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------
void  devSerial_FromFIFOToSerial_UpTo (ssIO_pFifo_t pFromfifo, int HowMany)
{
    Incr_fifoStat (pFromfifo, Transfer_Ct_OK);

    while (true)
    {
        if (HowMany <= 0)                                           break;
        if (ssIO_fifoOp_State (pFromfifo) == ssIO_fifo_State_NoData)   break;

        mesa_uiOp_emit_1 (ssIO_fifoOp_Get_FirstIn (pFromfifo));
        HowMany--;
    }   // while
}   // devSerial_FromFIFOToSerial_UpTo
// -------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------
pAsciiA_t       pcappFifos_Producer       = S("producer");
ssUI_Token_t    appFifos_ProducerTkn      = { pcappFifos_Producer, 8, NoBananas };

ssIO_fifo_t     producer_fifo;
Bits8_t         producer_Buffer[PRODUCER_BUFFER_ALLOC];

// -------------------------------------------------------------------------------------------------
// In this <producer><consumer> pair, the serial port (physical or virtual) PRODUCES Ascii RS-232 data
// (based on a Time-driven or IReq-driven signal from the mesa that the port has produced data),
// and on a Time-driven basis, this software CONSUMES the device data to PRODUCE raw data in memory.
// On receiving a data signal, the FIFO_Consumer CONSUMES the raw data to PRODUCE Ascii data.
// App software may then receive a complex informational signal about the Ascii data.
// -------------------------------------------------------------------------------------------------
// In the case of a physical serial port, this is data received by some kind of Ascii connection.
// In the case of a virtual serial port, this is data created and provided "as if from an Ascii".
// -------------------------------------------------------------------------------------------------
// The FSM always takes all values from a Serial source each time it has Agency, as a "burst"
// mechanism tied to the recurrence period.  The Baud Rate determines the recurrence period,
// the calculation is made by the App to provide the run-time parameter controlling recurrence.
// When any producer has data that cannot be transfered to the consumer, the data will be lost.
// The data structures and control values for the produced content are managed for each burst.
// -------------------------------------------------------------------------------------------------
ssE_EvResult_t App_fifos_Producer (ssE_pAboutEvVoid_t param1, ssT_pTime_t pCurrentTime)
{
    static int          BurstCt;

    ssE_pAboutEv_t      ssE_pAboutEv;           // valid after param1 is qualified as non-NULL
    App_pEventData_t    P_App_pEventData;
    ssUI_db_pEvApi_t    Consumer_pEvApi;        // to signal consumer need a carrier to consumer
    boolean             SendResumeSignal;

    // This FSM does no device Init State processing, the "Serial" connection is owned by the App.
    // The FIFO that will be used to capture data from the device is initialized in init state.
    if (param1 == pVoidEvParam1Null)
    {
        App_Announce_State ("App_fifos_Producer in init state: FIFO reset");
        ssIO_fifoOp_Init (&producer_fifo, IS_CIRCULAR_fifo, producer_Buffer, PRODUCER_BUFFER_MAX_I);
        // the msecs Time Unit value of a Time Variable indicates the size of the producer's buffer
        if ((fifos_param_producer_burstCt->msecs == 0) ||
            (fifos_param_producer_burstCt->msecs > PRODUCER_BUFFER_MAX_I))
        {
            App_Announce_App ("producer burst count set to 1, given parameter error");
            fifos_param_producer_burstCt->msecs = 1;
        }
        BurstCt = fifos_param_producer_burstCt->msecs;
        return (ssE_EvResult_OK_go);
    }

    App_Announce_State ("App_fifos_Producer in agency state");

    // -----------------------------------------------------------------------------------------
    // set focus (ssE_pAboutEv) to ssTEA Event Agency data, as initialized by Init state validate.
    // For this App's purposes, this Event's App_pEventData points to Apps's pEvApi App_EventData.
    // There are two events, so there are two App_pEventData pointers: one Fifo_In, one Fifo_Out.
    // The producer event pulls device data into the Fifo_In fifo, consumer filters to Fifo_Out.
    // -----------------------------------------------------------------------------------------
    ssE_pAboutEv = (ssE_pAboutEv_t) param1;
    // find: App_EventData_Style_Any "ssE_AboutEv's App_EventData carries a signal to an Event"
    // find: App_EventData_Style_Basic  "App_EventData pointer in ssE_AboutEv -> EvApi App_EventData"
    P_App_pEventData = (App_pEventData_t) ssE_pAboutEv->App_pEventData;

    // ---------------------------------------------------------------------------------------------
    // ---------------------------------------------------------------------------------------------
    P_App_pEventData->StopYourSelf = App_isItTimeToStop (pCurrentTime, P_App_pEventData->pStopTime);
    if (P_App_pEventData->StopYourSelf)
    {
        // FSM reached termination condition, matching the stop time requested to the current time
        App_Announce_App ("Producer Event signals Consumer Event: stopping...");
    }   // the current Time has reached the Time to stop
    else
    {   // let's keep going
        App_Announce_App ("Producer Event signals Consumer Event: continue...");
    }   // let's keep going

    // design assumption is that the producer of the data we're receiving does not wait for us.
    // also that the serial port is producing data at a rate where BurstCt will read enough data.
    // This is the toughest problem when interfacing two devices, but also the reason we exist.
    //
    // find: "FIFO producer/consumer"
    // Once given the command to stop, don't read from the device anymore, leave fifo alone also
    if (!P_App_pEventData->StopYourSelf)
    {
        devSerial_FromDevTofifo_UpTo (P_App_pEventData->pFifo_In, BurstCt);
    }
    // note that this design creates the possibility of a signal to stop with no data added,
    // because the fifo may or may not have had data in the fifo before this Event occurred.
    Consumer_pEvApi = ssUI_dbOp_Lookup_UsingEvApi_i (FIFOS_CONSUMER_EVAPI);
    Bld_ApiData_Debug_Access_func (Consumer_pEvApi);

    SendResumeSignal = false;
    if (P_App_pEventData->StopYourSelf)
    {   // received signal stop, send signal stop to consumer whethere or not there is data
        Consumer_pEvApi->App_EventData.StopYourSelf = true;
        SendResumeSignal = true;
    }   // signal stop (there may be data in the fifo)
    else
    {   // if it is not time to stop, is there data?
        Consumer_pEvApi->App_EventData.StopYourSelf = false;
        if (ssIO_fifoOp_State (P_App_pEventData->pFifo_In) != ssIO_fifo_State_NoData)
        {
            SendResumeSignal = true;
        }   // signal data in FIFO
        else
        {   // don't signal stop, don't signal data in fifo
            SendResumeSignal = false;
        }   // not stopping but no data
    }   // time to stop

    if (SendResumeSignal)
    {
        if (!ssUI_apiSignalOp_Emit (Consumer_pEvApi, ss_ApiSigMsgValue_AgRunfsm_Resume))
        {
            return (ssE_EvResult_notOK_state);
        }   // signal consumer
    }   // send resume

    if (P_App_pEventData->StopYourSelf)
    {
        App_Announce_App ("Producer Event: stopping...");
        ssIO_fifostatOp_ShowAll (pcappFifos_Producer, P_App_pEventData->pFifo_In);
    }
    return (P_App_pEventData->StopYourSelf ? ssE_EvResult_OK_stopMe : ssE_EvResult_OK_go);
}   // App_fifos_Producer
// -------------------------------------------------------------------------------------------------
// This captures data received through a signal from the fifos_Producer event that serial data arrived.
// -------------------------------------------------------------------------------------------------
#define APP_FIFO_IN_BUFFER_ALLOC  (10)
#define APP_FIFO_IN_BUFFER_MAX_I  (APP_FIFO_IN_BUFFER_ALLOC-1)

// -------------------------------------------------------------------------------------------------
void appFifo_ClearBuffer (void)
{
}   // appFifo_ClearBuffer
// -------------------------------------------------------------------------------------------------
pAsciiA_t       pcappFifoin       = S("appFifoin");
ssUI_Token_t    appFifoinTkn      = { pcappFifoin, 8, NoBananas };

ssIO_fifo_t     APP_FIFO_in;
Bits8_t         APP_FIFO_in_Buffer[APP_FIFO_IN_BUFFER_ALLOC];

void appFifos_FilterBinIntoAscii (ssIO_pFifo_t pFromfifo, ssIO_pFifo_t pTofifo, int HowMany)
{
    Bits8_t FromBin;

    Incr_fifoStat (pFromfifo, Transfer_Ct_OK);
    Incr_fifoStat (pTofifo, Transfer_Ct_OK);

    while (true)
    {
        if (HowMany == 0)                                           break;
        if (ssIO_fifoOp_State (pFromfifo) == ssIO_fifo_State_NoData)  break;
        if (ssIO_fifoOp_State (pTofifo) == ssIO_fifo_State_FullData)      break;

        FromBin = ssIO_fifoOp_Get_FirstIn (pFromfifo);
        if (FromBin < Ascii_Space)
        {
            // control character
            if (FromBin == Ascii_CR)
            {
                // keep going to add to storage
            }
            else
            {
                if ((FromBin != Ascii_LF) && (FromBin != Ascii_BS) && (FromBin != Ascii_ESC))
                {
                    continue;       // to drop is to filter
                }   // LF, BS, ESC not filtered, keep going to storage
            }
        }
        if (FromBin > Ascii_Tilde)          // there can be value filtering of many kinds
        {
            continue;
        }
        HowMany--;
        ssIO_fifoOp_Put_LastIn (pTofifo, FromBin);
    }   // while
}   // appFifos_FilterBinIntoAscii
// -------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------
// In this <producer><consumer> pair, the serial port capture event stores Ascii dataa into memory.
// This consumer is a data event that is waiting for the signal that data is ready for consumption.
// -------------------------------------------------------------------------------------------------
// The FSM always takes all values available from a Serial producer each time it is granted Agency,
// so that the physical flow of data through the Serial producer always runs at full efficiency.
// -------------------------------------------------------------------------------------------------
pAsciiA_t       pcappFifos_Consumer       = S("consumer");
ssUI_Token_t    appFifos_ConsumerTkn      = { pcappFifos_Consumer, 8, NoBananas };

ssIO_fifo_t     consumer_fifo;
Bits8_t         consumer_Buffer[CONSUMER_BUFFER_ALLOC];

ssE_EvResult_t App_fifos_Consumer (ssE_pAboutEvVoid_t param1, ssT_pTime_t pCurrentTime)
{
    static int          BurstCt;
    ssE_pAboutEv_t      ssE_pAboutEv;      // valid after param1 is qualified as non-NULL
    App_pEventData_t    C_App_pEventData;

    if (param1 == pVoidEvParam1Null)
    {
    // This FSM does have Init State processing, to initialize the isCircular buffer used for data.
        App_Announce_State ("App_fifos_Consumer in init state: FIFO reset");

        ssIO_fifoOp_Init (&consumer_fifo, IS_CIRCULAR_fifo, consumer_Buffer, CONSUMER_BUFFER_MAX_I);
        // the msecs Time Unit value of a Time Variable indicates the size of the consumer's buffer
        if ((fifos_param_consumer_burstCt->msecs == 0) ||
            (fifos_param_consumer_burstCt->msecs > CONSUMER_BUFFER_MAX_I))
        {
            App_Announce_App ("consumer burst count set to 1, given parameter error");
            fifos_param_consumer_burstCt->msecs = 1;
        }
        // this FSM is controlled over time by the value in this static variable
        BurstCt = fifos_param_consumer_burstCt->msecs;
        return (ssE_EvResult_OK_go);
    }

    App_Announce_State ("App_fifos_Consumer in agency state");

    ssE_pAboutEv = (ssE_pAboutEv_t) param1;
    // find: App_EventData_Style_Any "ssE_AboutEv's App_EventData carries a signal to an Event"
    // find: App_EventData_Style_Basic "App_EventData pointer in ssE_AboutEv -> EvApi App_EventData"
    C_App_pEventData = (App_pEventData_t) ssE_pAboutEv->App_pEventData;

    // Considered bad form when a data ready signal was sent without any data attached?  Note that
    // when the producer is instructed "stop yourself" in a Time Event, with no data, signal needed.
    if (
        (C_App_pEventData->StopYourSelf)
        ||
        (ssIO_fifoOp_State (C_App_pEventData->pFifo_In) != ssIO_fifo_State_NoData)
       )
    {
        // find: "FIFO producer/consumer"   message signaler sends FIFO to read from, put in consumer FIFO
        appFifos_FilterBinIntoAscii (C_App_pEventData->pFifo_In, C_App_pEventData->pFifo_Out, BurstCt);
    }   // some binary data was captured

    if (C_App_pEventData->StopYourSelf)
    {
        App_Announce_App ("Consumer Event: stopping...");
        ssIO_fifostatOp_ShowAll (pcappFifos_Consumer, C_App_pEventData->pFifo_Out);
    }
    return ((C_App_pEventData->StopYourSelf) ? ssE_EvResult_OK_stopMe : ssE_EvResult_OK_go);
}   // App_fifos_Consumer
// -------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------
// This is an "example of launching an Event" using the Init State of the launching Event and ssUI.
//    The Init State occurs when the Event is granted Agency during Event data validation,
//          which can be triggered by a ssUI command: "evapi <name> apifsm validate"
// Within the activities of launching the FIFO are included a programmatic launch of producer/consumer.
// -------------------------------------------------------------------------------------------------
ssE_EvResult_t App_fifos_Launcher (ssE_pAboutEvVoid_t  param1, ssT_pTime_t pCurrentTime)
{
    ssE_EvResult_t      EvResult = ssE_EvResult_OK_go;

    if (param1 != ssE_pAboutEvVoid)
    {
        App_Announce_ssTEA ("Flaw: a 'launching' Event does not achieve Agency Api FSM <On_Running> state");
        return (ssE_EvResult_notOK_state);
    }
    App_Announce_App ("Executing Init State: App_fifos_Launcher");

    // ------------------------------------------------------------------------------------------
    // ------------------------------------------------------------------------------------------
    EvResult = App_fifos_getParams ();
    if (EvResult == ssE_EvResult_OK_go)
    {
        EvResult = App_fifos_Producer_Launcher ();
        if (EvResult == ssE_EvResult_OK_go)
        {   // producer launch success, attempt consumer
            EvResult = App_fifos_Consumer_Launcher ();

            // ------------------------------------------------------------------------------------------
            // clear all Time variables used to get parameters from the command line to unhide errors.
            // This also allows the parameter processing to work from defaults on the 2nd CMD run.
            // ------------------------------------------------------------------------------------------
            ssT_mathOp_P1getsZero (ssUI_gpTimeA);
            ssT_mathOp_P1getsZero (ssUI_gpTimeB);
            ssT_mathOp_P1getsZero (ssUI_gpTimeC);
            ssT_mathOp_P1getsZero (ssUI_gpTimeD);
            ssT_mathOp_P1getsZero (ssUI_gpTimeE);
            ssT_mathOp_P1getsZero (ssUI_gpTimeF);

            // consumer launch failed or succeeded
            return (EvResult);
        }   // producer launch success
        else
        {   // producer launch failed
            return (EvResult);
        }
    }   // FIFO params succeeded
    else
    {
        return(EvResult);
    }
}   // App_fifos_Launcher  (redefined as EvFunc0 for function pointers in EvApi and ApiSig)
// -------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------
ssE_EvResult_t App_fifos_Producer_Launcher (void)
{
    ssUI_db_pEvApi_t    Producers_pEvApi;
    ssTEA_pApiSig_t     pApiSig;
    ssE_pAboutEv_t      ssE_pAboutEv;

    // ------------------------------------------------------------------------------------------
    // CHANGE Producers_pEvApi to refer to a specific EvApiData database entry or record.
    // Note that both Applications MAY be driven through the configuration process using a
    // specific EvApi allocation that has been mapped with ssUI, and is therefore referencable.
    // ------------------------------------------------------------------------------------------
    Producers_pEvApi = ssUI_dbOp_Lookup_UsingEvApi_i (FIFOS_PRODUCER_EVAPI);
    Bld_ApiData_Debug_Access_func (Producers_pEvApi);
    Producers_pEvApi->pEvApiNameTkn_App = &appFifos_ProducerTkn;
    Producers_pEvApi->App_EventData.StopYourSelf = false;
    Producers_pEvApi->App_EventData.pFifo_In = &producer_fifo;
    Producers_pEvApi->App_EventData.pFifo_Out = &consumer_fifo;

    // ------------------------------------------------------------------------------------------
    // An example of using the App_EventData to keep data across Time, tuck away a pointer.
    // The fifos application could just as easily use the pTimeVar without indirection.
    // ------------------------------------------------------------------------------------------
    Producers_pEvApi->App_EventData.pStopTime = fifos_pComputed_StopTime;

    // ------------------------------------------------------------------------------------------
    // set signal carrier focus to request the Event EvApi change from Off
    // ------------------------------------------------------------------------------------------
    pApiSig = Producers_pEvApi->ssTEA_pApiSig;
    pApiSig->ssE_pAboutEv = ssE_pAboutEvNull;
    App_Announce_ssTEA ("fifo producer Launcher Api FSM on Request : ");
    if (!ssUI_apiSignalOp_Emit (Producers_pEvApi, ss_ApiSigMsgValue_AgApifsm_On))
    {
        return (ssE_EvResult_notOK_state);
    }

    // find: "ssE_AboutEv Allocation Rules" to understand why a copy would not be instantiated
#ifdef SSE_OPTIN_ABOUTEVDB_TRUSTED_APP
// -- trusted
#else   // not SSE_OPTIN_ABOUTEVDB_TRUSTED_APP
// -- not defined
    pApiSig->ssE_pAboutEv = AboutEvDB_dbOp_Get ();      // pointer to our copy
#endif  // SSE_OPTIN_ABOUTEVDB_TRUSTED_APP

    // change focus to the Agency info about the event.
    ssE_pAboutEv = pApiSig->ssE_pAboutEv;
    ssE_pAboutEv->Runfsm_EvType = ssE_EvType_Time;
    ssT_mathOp_P1getsP2 (&ssE_pAboutEv->OccursAt, fifos_pComputedStartTime);
    ssE_pAboutEv->recurs = true;
    ssE_pAboutEv->recursFromTrue = true;
    // period for a recurring event, while Cycling
    ssT_mathOp_P1getsP2 (&ssE_pAboutEv->Period, fifos_param_producer_service_period);

    // find: App_EventData_Style_Any "ssE_AboutEv's App_EventData carries a signal to an Event"
    // find: App_EventData_Style_Basic "App_EventData pointer in ssE_AboutEv -> EvApi App_EventData"
    // find: ssE_event_dcl.h to see why the pointer to the App Event Data is a pointer to void
    ssE_pAboutEv->App_pEventData = (EvEx_App_pEventData_t) &Producers_pEvApi->App_EventData;

    // Set the function pointer in the signal for the API before trying to validate.
    // ssTEA note: this trick solves the problem of the timing of the About_Ev allocation based on
    // a trusted App for the Event DB, versus the timing of the About_Ev information being assigned.
    // This is a result of the design of ssUI supporting N Events inherently for the App to use.
    ssE_pAboutEv->pEvFunc = &App_fifos_Producer;

    // launch the fifo producer event, it will be running when complete, within state of Time/Agency
    return (App_Launch_in_ssTEA (Producers_pEvApi));
}   // App_fifos_Producer_Launcher
// -------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------
ssE_EvResult_t App_fifos_Consumer_Launcher (void)
{
    ssUI_db_pEvApi_t    Consumers_pEvApi;
    ssTEA_pApiSig_t     pApiSig;
    ssE_pAboutEv_t      ssE_pAboutEv;
    ssE_EvResult_t      EvResult;

    Consumers_pEvApi = ssUI_dbOp_Lookup_UsingEvApi_i (FIFOS_CONSUMER_EVAPI);
    Bld_ApiData_Debug_Access_func (Consumers_pEvApi);
    Consumers_pEvApi->pEvApiNameTkn_App = &appFifos_ConsumerTkn;
    Consumers_pEvApi->App_EventData.StopYourSelf = false;
    Consumers_pEvApi->App_EventData.pFifo_In = &producer_fifo;
    Consumers_pEvApi->App_EventData.pFifo_Out = &consumer_fifo;

    // ------------------------------------------------------------------------------------------
    // set signal carrier focus to request the Event EvApi change from Off
    // ------------------------------------------------------------------------------------------
    pApiSig = Consumers_pEvApi->ssTEA_pApiSig;
    pApiSig->ssE_pAboutEv = ssE_pAboutEvNull;
    App_Announce_ssTEA ("fifo consumer Launcher Api FSM on Request : ");
    if (!ssUI_apiSignalOp_Emit (Consumers_pEvApi, ss_ApiSigMsgValue_AgApifsm_On))
    {
        return (ssE_EvResult_notOK_state);
    }

    // find: "ssE_AboutEv Allocation Rules"  : when the App is not trusted, alloc post-Api FSM On
#ifdef SSE_OPTIN_ABOUTEVDB_TRUSTED_APP
// -- trusted
#else   // not SSE_OPTIN_ABOUTEVDB_TRUSTED_APP
// -- not defined
    pApiSig->ssE_pAboutEv = AboutEvDB_dbOp_Get ();      // pointer to our copy
#endif  // SSE_OPTIN_ABOUTEVDB_TRUSTED_APP

    // fifos_param_fifo_service_period
    // change agency focus to the Agency information about the event.
    ssE_pAboutEv = pApiSig->ssE_pAboutEv;
    ssE_pAboutEv->Runfsm_EvType = ssE_EvType_Data;

    // find: App_EventData_Style_Any "ssE_AboutEv's App_EventData carries a signal to an Event"
    // find: App_EventData_Style_Basic  "App_EventData pointer in ssE_AboutEv -> EvApi App_EventData"
    // find: ssE_event_dcl.h to see why the pointer to the App Event Data is a pointer to void
    ssE_pAboutEv->App_pEventData = (EvEx_App_pEventData_t) &Consumers_pEvApi->App_EventData; // (fifo consumer)

    // Set the function pointer in the signal for the API before trying to validate.
    // ssTEA note: this trick solves the problem of the timing of the About_Ev allocation based on
    // a trusted App for the Event DB, versus the timing of the About_Ev information being assigned.
    // This is a result of the design of ssUI supporting N Events inherently for the App to use.
    ssE_pAboutEv->pEvFunc = &App_fifos_Consumer;

    EvResult = App_Launch_in_ssTEA (Consumers_pEvApi);
    if (EvResult == ssE_EvResult_OK_go)
    {
        if (!ssUI_apiSignalOp_Emit (Consumers_pEvApi, ss_ApiSigMsgValue_AgRunfsm_Pause))
        {
            EvResult = ssE_EvResult_notOK_data;
        }   // Paused after Launch
    }   // Launch OK

    return (EvResult);
}   // App_fifos_Consumer_Launcher

#endif // _APP_FIFOS_INO_H

