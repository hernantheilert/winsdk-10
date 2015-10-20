//**********************************************************************`
//* This is an include file generated by Message Compiler.             *`
//*                                                                    *`
//* Copyright (c) Microsoft Corporation. All Rights Reserved.          *`
//**********************************************************************`
#pragma once
#include <wmistr.h>
#include <evntrace.h>
#include "evntprov.h"
//
//  Initial Defs
//
#if !defined(ETW_INLINE)
#define ETW_INLINE DECLSPEC_NOINLINE __inline
#endif

#if defined(__cplusplus)
extern "C" {
#endif

//
// Allow Diasabling of code generation
//
#ifndef MCGEN_DISABLE_PROVIDER_CODE_GENERATION
#if  !defined(McGenDebug)
#define McGenDebug(a,b)
#endif 


#if !defined(MCGEN_TRACE_CONTEXT_DEF)
#define MCGEN_TRACE_CONTEXT_DEF
typedef struct _MCGEN_TRACE_CONTEXT
{
    TRACEHANDLE            RegistrationHandle;
    TRACEHANDLE            Logger;
    ULONGLONG              MatchAnyKeyword;
    ULONGLONG              MatchAllKeyword;
    ULONG                  Flags;
    ULONG                  IsEnabled;
    UCHAR                  Level; 
    UCHAR                  Reserve;
    USHORT                 EnableBitsCount;
    PULONG                 EnableBitMask;
    const ULONGLONG*       EnableKeyWords;
    const UCHAR*           EnableLevel;
} MCGEN_TRACE_CONTEXT, *PMCGEN_TRACE_CONTEXT;
#endif

#if !defined(MCGEN_LEVEL_KEYWORD_ENABLED_DEF)
#define MCGEN_LEVEL_KEYWORD_ENABLED_DEF
FORCEINLINE
BOOLEAN
McGenLevelKeywordEnabled(
    _In_ PMCGEN_TRACE_CONTEXT EnableInfo,
    _In_ UCHAR Level,
    _In_ ULONGLONG Keyword
    )
{
    //
    // Check if the event Level is lower than the level at which
    // the channel is enabled.
    // If the event Level is 0 or the channel is enabled at level 0,
    // all levels are enabled.
    //

    if ((Level <= EnableInfo->Level) || // This also covers the case of Level == 0.
        (EnableInfo->Level == 0)) {

        //
        // Check if Keyword is enabled
        //

        if ((Keyword == (ULONGLONG)0) ||
            ((Keyword & EnableInfo->MatchAnyKeyword) &&
             ((Keyword & EnableInfo->MatchAllKeyword) == EnableInfo->MatchAllKeyword))) {
            return TRUE;
        }
    }

    return FALSE;

}
#endif

#if !defined(MCGEN_EVENT_ENABLED_DEF)
#define MCGEN_EVENT_ENABLED_DEF
FORCEINLINE
BOOLEAN
McGenEventEnabled(
    _In_ PMCGEN_TRACE_CONTEXT EnableInfo,
    _In_ PCEVENT_DESCRIPTOR EventDescriptor
    )
{

    return McGenLevelKeywordEnabled(EnableInfo, EventDescriptor->Level, EventDescriptor->Keyword);

}
#endif


//
// EnableCheckMacro
//
#ifndef MCGEN_ENABLE_CHECK
#define MCGEN_ENABLE_CHECK(Context, Descriptor) (Context.IsEnabled &&  McGenEventEnabled(&Context, &Descriptor))
#endif

#if !defined(MCGEN_CONTROL_CALLBACK)
#define MCGEN_CONTROL_CALLBACK

DECLSPEC_NOINLINE __inline
VOID
__stdcall
McGenControlCallbackV2(
    _In_ LPCGUID SourceId,
    _In_ ULONG ControlCode,
    _In_ UCHAR Level,
    _In_ ULONGLONG MatchAnyKeyword,
    _In_ ULONGLONG MatchAllKeyword,
    _In_opt_ PEVENT_FILTER_DESCRIPTOR FilterData,
    _Inout_opt_ PVOID CallbackContext
    )
/*++

Routine Description:

    This is the notification callback for Vista.

Arguments:

    SourceId - The GUID that identifies the session that enabled the provider. 

    ControlCode - The parameter indicates whether the provider 
                  is being enabled or disabled.

    Level - The level at which the event is enabled.

    MatchAnyKeyword - The bitmask of keywords that the provider uses to 
                      determine the category of events that it writes.

    MatchAllKeyword - This bitmask additionally restricts the category 
                      of events that the provider writes. 

    FilterData - The provider-defined data.

    CallbackContext - The context of the callback that is defined when the provider 
                      called EtwRegister to register itself.

Remarks:

    ETW calls this function to notify provider of enable/disable

--*/
{
    PMCGEN_TRACE_CONTEXT Ctx = (PMCGEN_TRACE_CONTEXT)CallbackContext;
    ULONG Ix;
#ifndef MCGEN_PRIVATE_ENABLE_CALLBACK_V2
    UNREFERENCED_PARAMETER(SourceId);
    UNREFERENCED_PARAMETER(FilterData);
#endif

    if (Ctx == NULL) {
        return;
    }

    switch (ControlCode) {

        case EVENT_CONTROL_CODE_ENABLE_PROVIDER:
            Ctx->Level = Level;
            Ctx->MatchAnyKeyword = MatchAnyKeyword;
            Ctx->MatchAllKeyword = MatchAllKeyword;
            Ctx->IsEnabled = EVENT_CONTROL_CODE_ENABLE_PROVIDER;

            for (Ix = 0; Ix < Ctx->EnableBitsCount; Ix += 1) {
                if (McGenLevelKeywordEnabled(Ctx, Ctx->EnableLevel[Ix], Ctx->EnableKeyWords[Ix]) != FALSE) {
                    Ctx->EnableBitMask[Ix >> 5] |= (1 << (Ix % 32));
                } else {
                    Ctx->EnableBitMask[Ix >> 5] &= ~(1 << (Ix % 32));
                }
            }
            break;

        case EVENT_CONTROL_CODE_DISABLE_PROVIDER:
            Ctx->IsEnabled = EVENT_CONTROL_CODE_DISABLE_PROVIDER;
            Ctx->Level = 0;
            Ctx->MatchAnyKeyword = 0;
            Ctx->MatchAllKeyword = 0;
            if (Ctx->EnableBitsCount > 0) {
                RtlZeroMemory(Ctx->EnableBitMask, (((Ctx->EnableBitsCount - 1) / 32) + 1) * sizeof(ULONG));
            }
            break;
 
        default:
            break;
    }

#ifdef MCGEN_PRIVATE_ENABLE_CALLBACK_V2
    //
    // Call user defined callback
    //
    MCGEN_PRIVATE_ENABLE_CALLBACK_V2(
        SourceId,
        ControlCode,
        Level,
        MatchAnyKeyword,
        MatchAllKeyword,
        FilterData,
        CallbackContext
        );
#endif
   
    return;
}

#endif
#endif // MCGEN_DISABLE_PROVIDER_CODE_GENERATION
//+
// Provider Microsoft-Windows-Kernel-Pep Event Count 7
//+
EXTERN_C __declspec(selectany) const GUID POP_PEP_ETW_PROVIDER = {0x5412704e, 0xb2e1, 0x4624, {0x8f, 0xfd, 0x55, 0x77, 0x7b, 0x8f, 0x73, 0x73}};

//
// Channel
//
#define POP_PEP_ETW_PROVIDER_CHANNEL_chidPepPower 0x10

//
// Tasks
//
#define POP_PEP_ETW_TASK_RAILRUNDOWN 0x1
#define POP_PEP_ETW_TASK_DEVICERUNDOWN 0x2
#define POP_PEP_ETW_TASK_COMPONENTRUNDOWN 0x3
#define POP_PEP_ETW_TASK_RAILVOLTAGECHANGE 0x4
#define POP_PEP_ETW_TASK_COMPONENTFREQUENCYCHANGE 0x5
#define POP_PEP_ETW_TASK_PLATFORMIDLERESIDENCY 0x6
#define POP_PEP_ETW_TASK_PLATFORMIDLETRANSITION 0x7
//
// Keyword
//
#define POP_PEP_ETW_KEYWORD_POWER 0x1

//
// Event Descriptors
//
EXTERN_C __declspec(selectany) const EVENT_DESCRIPTOR POP_PEP_ETW_RAIL_RUNDOWN = {0x1, 0x0, 0x10, 0x4, 0x0, 0x1, 0x8000000000000001};
#define POP_PEP_ETW_RAIL_RUNDOWN_value 0x1
EXTERN_C __declspec(selectany) const EVENT_DESCRIPTOR POP_PEP_ETW_DEVICE_RUNDOWN = {0x2, 0x0, 0x10, 0x4, 0x0, 0x2, 0x8000000000000001};
#define POP_PEP_ETW_DEVICE_RUNDOWN_value 0x2
EXTERN_C __declspec(selectany) const EVENT_DESCRIPTOR POP_PEP_ETW_COMPONENT_RUNDOWN = {0x3, 0x0, 0x10, 0x4, 0x0, 0x3, 0x8000000000000001};
#define POP_PEP_ETW_COMPONENT_RUNDOWN_value 0x3
EXTERN_C __declspec(selectany) const EVENT_DESCRIPTOR POP_PEP_ETW_RAIL_VOLTAGE_CHANGE = {0x4, 0x0, 0x10, 0x4, 0x0, 0x4, 0x8000000000000001};
#define POP_PEP_ETW_RAIL_VOLTAGE_CHANGE_value 0x4
EXTERN_C __declspec(selectany) const EVENT_DESCRIPTOR POP_PEP_ETW_COMPONENT_FREQUENCE_CHANGE = {0x5, 0x0, 0x10, 0x4, 0x0, 0x5, 0x8000000000000001};
#define POP_PEP_ETW_COMPONENT_FREQUENCE_CHANGE_value 0x5
EXTERN_C __declspec(selectany) const EVENT_DESCRIPTOR POP_PEP_ETW_PLATFORM_IDLE_RESIDENCY = {0x6, 0x0, 0x10, 0x4, 0x0, 0x6, 0x8000000000000001};
#define POP_PEP_ETW_PLATFORM_IDLE_RESIDENCY_value 0x6
EXTERN_C __declspec(selectany) const EVENT_DESCRIPTOR POP_PEP_ETW_PLATFORM_IDLE_TRANSITION = {0x7, 0x0, 0x10, 0x4, 0x0, 0x7, 0x8000000000000001};
#define POP_PEP_ETW_PLATFORM_IDLE_TRANSITION_value 0x7

//
// Note on Generate Code from Manifest Windows Vista and above
//
//Structures :  are handled as a size and pointer pairs. The macro for the event will have an extra 
//parameter for the size in bytes of the structure. Make sure that your structures have no extra padding.
//
//Strings: There are several cases that can be described in the manifest. For array of variable length 
//strings, the generated code will take the count of characters for the whole array as an input parameter. 
//
//SID No support for array of SIDs, the macro will take a pointer to the SID and use appropriate 
//GetLengthSid function to get the length.
//

//
// Allow Diasabling of code generation
//
#ifndef MCGEN_DISABLE_PROVIDER_CODE_GENERATION

//
// Globals 
//


//
// Event Enablement Bits
//

EXTERN_C __declspec(selectany) DECLSPEC_CACHEALIGN ULONG Microsoft_Windows_Kernel_PepEnableBits[1];
EXTERN_C __declspec(selectany) const ULONGLONG Microsoft_Windows_Kernel_PepKeywords[1] = {0x8000000000000001};
EXTERN_C __declspec(selectany) const UCHAR Microsoft_Windows_Kernel_PepLevels[1] = {4};
EXTERN_C __declspec(selectany) MCGEN_TRACE_CONTEXT POP_PEP_ETW_PROVIDER_Context = {0, 0, 0, 0, 0, 0, 0, 0, 1, Microsoft_Windows_Kernel_PepEnableBits, Microsoft_Windows_Kernel_PepKeywords, Microsoft_Windows_Kernel_PepLevels};

EXTERN_C __declspec(selectany) REGHANDLE Microsoft_Windows_Kernel_PepHandle = (REGHANDLE)0;

#if !defined(McGenEventRegisterUnregister)
#define McGenEventRegisterUnregister
DECLSPEC_NOINLINE __inline
ULONG __stdcall
McGenEventRegister(
    _In_ LPCGUID ProviderId,
    _In_opt_ PETWENABLECALLBACK EnableCallback,
    _In_opt_ PVOID CallbackContext,
    _Inout_ PREGHANDLE RegHandle
    )
/*++

Routine Description:

    This function register the provider with ETW KERNEL mode.

Arguments:
    ProviderId - Provider Id to be register with ETW.

    EnableCallback - Callback to be used.

    CallbackContext - Context for this provider.

    RegHandle - Pointer to Registration handle.

Remarks:

    If the handle != NULL will return ERROR_SUCCESS

--*/
{
    ULONG Error;


    if (*RegHandle) {
        //
        // already registered
        //
        return STATUS_SUCCESS;
    }

    Error = EtwRegister( ProviderId, EnableCallback, CallbackContext, RegHandle); 

    return Error;
}


DECLSPEC_NOINLINE __inline
ULONG __stdcall
McGenEventUnregister(_Inout_ PREGHANDLE RegHandle)
/*++

Routine Description:

    Unregister from ETW KERNEL mode

Arguments:
            RegHandle this is the pointer to the provider context
Remarks:
            If Provider has not register RegHandle = NULL,
            return ERROR_SUCCESS
--*/
{
    ULONG Error;


    if(!(*RegHandle)) {
        //
        // Provider has not registerd
        //
        return STATUS_SUCCESS;
    }

    Error = EtwUnregister(*RegHandle); 
    *RegHandle = (REGHANDLE)0;
    
    return Error;
}
#endif
//
// Register with ETW Vista +
//
#ifndef EventRegisterMicrosoft_Windows_Kernel_Pep
#define EventRegisterMicrosoft_Windows_Kernel_Pep() McGenEventRegister(&POP_PEP_ETW_PROVIDER, McGenControlCallbackV2, &POP_PEP_ETW_PROVIDER_Context, &Microsoft_Windows_Kernel_PepHandle) 
#endif

//
// UnRegister with ETW
//
#ifndef EventUnregisterMicrosoft_Windows_Kernel_Pep
#define EventUnregisterMicrosoft_Windows_Kernel_Pep() McGenEventUnregister(&Microsoft_Windows_Kernel_PepHandle) 
#endif

//
// Enablement check macro for POP_PEP_ETW_RAIL_RUNDOWN
//

#define EventEnabledPOP_PEP_ETW_RAIL_RUNDOWN() ((Microsoft_Windows_Kernel_PepEnableBits[0] & 0x00000001) != 0)

//
// Event Macro for POP_PEP_ETW_RAIL_RUNDOWN
//
#define EventWritePOP_PEP_ETW_RAIL_RUNDOWN(Activity, VoltageRailId, VoltageRailName, CurrentVoltageMv, MaxVoltageMv)\
        EventEnabledPOP_PEP_ETW_RAIL_RUNDOWN() ?\
        Template_qzqq(Microsoft_Windows_Kernel_PepHandle, &POP_PEP_ETW_RAIL_RUNDOWN, Activity, VoltageRailId, VoltageRailName, CurrentVoltageMv, MaxVoltageMv)\
        : STATUS_SUCCESS\

//
// Enablement check macro for POP_PEP_ETW_DEVICE_RUNDOWN
//

#define EventEnabledPOP_PEP_ETW_DEVICE_RUNDOWN() ((Microsoft_Windows_Kernel_PepEnableBits[0] & 0x00000001) != 0)

//
// Event Macro for POP_PEP_ETW_DEVICE_RUNDOWN
//
#define EventWritePOP_PEP_ETW_DEVICE_RUNDOWN(Activity, DeviceId, DeviceName, PlatformStateDependencyCount, PlatformStateDependency_Len_, PlatformStateDependency)\
        EventEnabledPOP_PEP_ETW_DEVICE_RUNDOWN() ?\
        Template_pzqNR2(Microsoft_Windows_Kernel_PepHandle, &POP_PEP_ETW_DEVICE_RUNDOWN, Activity, DeviceId, DeviceName, PlatformStateDependencyCount, PlatformStateDependency_Len_, PlatformStateDependency)\
        : STATUS_SUCCESS\

//
// Enablement check macro for POP_PEP_ETW_COMPONENT_RUNDOWN
//

#define EventEnabledPOP_PEP_ETW_COMPONENT_RUNDOWN() ((Microsoft_Windows_Kernel_PepEnableBits[0] & 0x00000001) != 0)

//
// Event Macro for POP_PEP_ETW_COMPONENT_RUNDOWN
//
#define EventWritePOP_PEP_ETW_COMPONENT_RUNDOWN(Activity, VoltageRailId, DeviceId, ComponentIndex, CurrentFrequencyKHz, MaxFrequencyKHz, PlatformStateDependencyCount, PlatformStateDependency_Len_, PlatformStateDependency, ComponentDescriptionLength, ComponentDescription)\
        EventEnabledPOP_PEP_ETW_COMPONENT_RUNDOWN() ?\
        Template_qpqqqqNR5qzr7(Microsoft_Windows_Kernel_PepHandle, &POP_PEP_ETW_COMPONENT_RUNDOWN, Activity, VoltageRailId, DeviceId, ComponentIndex, CurrentFrequencyKHz, MaxFrequencyKHz, PlatformStateDependencyCount, PlatformStateDependency_Len_, PlatformStateDependency, ComponentDescriptionLength, ComponentDescription)\
        : STATUS_SUCCESS\

//
// Enablement check macro for POP_PEP_ETW_RAIL_VOLTAGE_CHANGE
//

#define EventEnabledPOP_PEP_ETW_RAIL_VOLTAGE_CHANGE() ((Microsoft_Windows_Kernel_PepEnableBits[0] & 0x00000001) != 0)

//
// Event Macro for POP_PEP_ETW_RAIL_VOLTAGE_CHANGE
//
#define EventWritePOP_PEP_ETW_RAIL_VOLTAGE_CHANGE(Activity, VoltageRailId, OldRailVoltageMv, NewRailVoltageMv)\
        EventEnabledPOP_PEP_ETW_RAIL_VOLTAGE_CHANGE() ?\
        Template_qqq(Microsoft_Windows_Kernel_PepHandle, &POP_PEP_ETW_RAIL_VOLTAGE_CHANGE, Activity, VoltageRailId, OldRailVoltageMv, NewRailVoltageMv)\
        : STATUS_SUCCESS\

//
// Enablement check macro for POP_PEP_ETW_COMPONENT_FREQUENCE_CHANGE
//

#define EventEnabledPOP_PEP_ETW_COMPONENT_FREQUENCE_CHANGE() ((Microsoft_Windows_Kernel_PepEnableBits[0] & 0x00000001) != 0)

//
// Event Macro for POP_PEP_ETW_COMPONENT_FREQUENCE_CHANGE
//
#define EventWritePOP_PEP_ETW_COMPONENT_FREQUENCE_CHANGE(Activity, DeviceId, ComponentIndex, OldComponentFrequencyKHz, NewComponentFrequencyKHz)\
        EventEnabledPOP_PEP_ETW_COMPONENT_FREQUENCE_CHANGE() ?\
        Template_pqqq(Microsoft_Windows_Kernel_PepHandle, &POP_PEP_ETW_COMPONENT_FREQUENCE_CHANGE, Activity, DeviceId, ComponentIndex, OldComponentFrequencyKHz, NewComponentFrequencyKHz)\
        : STATUS_SUCCESS\

//
// Enablement check macro for POP_PEP_ETW_PLATFORM_IDLE_RESIDENCY
//

#define EventEnabledPOP_PEP_ETW_PLATFORM_IDLE_RESIDENCY() ((Microsoft_Windows_Kernel_PepEnableBits[0] & 0x00000001) != 0)

//
// Event Macro for POP_PEP_ETW_PLATFORM_IDLE_RESIDENCY
//
#define EventWritePOP_PEP_ETW_PLATFORM_IDLE_RESIDENCY(Activity, PlatformStateCount, PlatformIdleStateResidency_Len_, PlatformIdleStateResidency)\
        EventEnabledPOP_PEP_ETW_PLATFORM_IDLE_RESIDENCY() ?\
        Template_qNR0(Microsoft_Windows_Kernel_PepHandle, &POP_PEP_ETW_PLATFORM_IDLE_RESIDENCY, Activity, PlatformStateCount, PlatformIdleStateResidency_Len_, PlatformIdleStateResidency)\
        : STATUS_SUCCESS\

//
// Enablement check macro for POP_PEP_ETW_PLATFORM_IDLE_TRANSITION
//

#define EventEnabledPOP_PEP_ETW_PLATFORM_IDLE_TRANSITION() ((Microsoft_Windows_Kernel_PepEnableBits[0] & 0x00000001) != 0)

//
// Event Macro for POP_PEP_ETW_PLATFORM_IDLE_TRANSITION
//
#define EventWritePOP_PEP_ETW_PLATFORM_IDLE_TRANSITION(Activity, OldPlatformState, NewPlatformState)\
        EventEnabledPOP_PEP_ETW_PLATFORM_IDLE_TRANSITION() ?\
        Template_qq(Microsoft_Windows_Kernel_PepHandle, &POP_PEP_ETW_PLATFORM_IDLE_TRANSITION, Activity, OldPlatformState, NewPlatformState)\
        : STATUS_SUCCESS\

#endif // MCGEN_DISABLE_PROVIDER_CODE_GENERATION


//
// Allow Diasabling of code generation
//
#ifndef MCGEN_DISABLE_PROVIDER_CODE_GENERATION

//
// Template Functions 
//
//
//Template from manifest : tidPepRailRundown
//
#ifndef Template_qzqq_def
#define Template_qzqq_def
ETW_INLINE
ULONG
Template_qzqq(
    _In_ REGHANDLE RegHandle,
    _In_ PCEVENT_DESCRIPTOR Descriptor,
    _In_opt_ LPCGUID Activity,
    _In_ const unsigned int  _Arg0,
    _In_opt_ PCWSTR  _Arg1,
    _In_ const unsigned int  _Arg2,
    _In_ const unsigned int  _Arg3
    )
{
#define ARGUMENT_COUNT_qzqq 4

    EVENT_DATA_DESCRIPTOR EventData[ARGUMENT_COUNT_qzqq];

    EventDataDescCreate(&EventData[0], &_Arg0, sizeof(const unsigned int)  );

    EventDataDescCreate(&EventData[1], 
                        (_Arg1 != NULL) ? _Arg1 : L"NULL",
                        (_Arg1 != NULL) ? (ULONG)((wcslen(_Arg1) + 1) * sizeof(WCHAR)) : (ULONG)sizeof(L"NULL"));

    EventDataDescCreate(&EventData[2], &_Arg2, sizeof(const unsigned int)  );

    EventDataDescCreate(&EventData[3], &_Arg3, sizeof(const unsigned int)  );

    return EtwWrite(RegHandle, Descriptor, Activity, ARGUMENT_COUNT_qzqq, EventData);
}
#endif

//
//Template from manifest : tidPepDeviceRundown
//
#ifndef Template_pzqNR2_def
#define Template_pzqNR2_def
ETW_INLINE
ULONG
Template_pzqNR2(
    _In_ REGHANDLE RegHandle,
    _In_ PCEVENT_DESCRIPTOR Descriptor,
    _In_opt_ LPCGUID Activity,
    _In_opt_ const void *  _Arg0,
    _In_opt_ PCWSTR  _Arg1,
    _In_ const unsigned int  _Arg2,
    _In_ ULONG _Arg3_Len_,
    _In_ const PVOID  _Arg3
    )
{
#define ARGUMENT_COUNT_pzqNR2 4

    EVENT_DATA_DESCRIPTOR EventData[ARGUMENT_COUNT_pzqNR2];

    EventDataDescCreate(&EventData[0], &_Arg0, sizeof(PVOID)  );

    EventDataDescCreate(&EventData[1], 
                        (_Arg1 != NULL) ? _Arg1 : L"NULL",
                        (_Arg1 != NULL) ? (ULONG)((wcslen(_Arg1) + 1) * sizeof(WCHAR)) : (ULONG)sizeof(L"NULL"));

    EventDataDescCreate(&EventData[2], &_Arg2, sizeof(const unsigned int)  );

    EventDataDescCreate(&EventData[3], _Arg3, _Arg2 * _Arg3_Len_);

    return EtwWrite(RegHandle, Descriptor, Activity, ARGUMENT_COUNT_pzqNR2, EventData);
}
#endif

//
//Template from manifest : tidPepComponentRundown
//
#ifndef Template_qpqqqqNR5qzr7_def
#define Template_qpqqqqNR5qzr7_def
ETW_INLINE
ULONG
Template_qpqqqqNR5qzr7(
    _In_ REGHANDLE RegHandle,
    _In_ PCEVENT_DESCRIPTOR Descriptor,
    _In_opt_ LPCGUID Activity,
    _In_ const unsigned int  _Arg0,
    _In_opt_ const void *  _Arg1,
    _In_ const unsigned int  _Arg2,
    _In_ const unsigned int  _Arg3,
    _In_ const unsigned int  _Arg4,
    _In_ const unsigned int  _Arg5,
    _In_ ULONG _Arg6_Len_,
    _In_ const PVOID  _Arg6,
    _In_ const unsigned int  _Arg7,
    _In_reads_(_Arg7) PCWCH  _Arg8
    )
{
#define ARGUMENT_COUNT_qpqqqqNR5qzr7 9

    EVENT_DATA_DESCRIPTOR EventData[ARGUMENT_COUNT_qpqqqqNR5qzr7];

    EventDataDescCreate(&EventData[0], &_Arg0, sizeof(const unsigned int)  );

    EventDataDescCreate(&EventData[1], &_Arg1, sizeof(PVOID)  );

    EventDataDescCreate(&EventData[2], &_Arg2, sizeof(const unsigned int)  );

    EventDataDescCreate(&EventData[3], &_Arg3, sizeof(const unsigned int)  );

    EventDataDescCreate(&EventData[4], &_Arg4, sizeof(const unsigned int)  );

    EventDataDescCreate(&EventData[5], &_Arg5, sizeof(const unsigned int)  );

    EventDataDescCreate(&EventData[6], _Arg6, _Arg5 * _Arg6_Len_);

    EventDataDescCreate(&EventData[7], &_Arg7, sizeof(const unsigned int)  );

    EventDataDescCreate(&EventData[8], _Arg8, (ULONG)(sizeof(WCHAR)*_Arg7));

    return EtwWrite(RegHandle, Descriptor, Activity, ARGUMENT_COUNT_qpqqqqNR5qzr7, EventData);
}
#endif

//
//Template from manifest : tidPepRailVoltageChange
//
#ifndef Template_qqq_def
#define Template_qqq_def
ETW_INLINE
ULONG
Template_qqq(
    _In_ REGHANDLE RegHandle,
    _In_ PCEVENT_DESCRIPTOR Descriptor,
    _In_opt_ LPCGUID Activity,
    _In_ const unsigned int  _Arg0,
    _In_ const unsigned int  _Arg1,
    _In_ const unsigned int  _Arg2
    )
{
#define ARGUMENT_COUNT_qqq 3

    EVENT_DATA_DESCRIPTOR EventData[ARGUMENT_COUNT_qqq];

    EventDataDescCreate(&EventData[0], &_Arg0, sizeof(const unsigned int)  );

    EventDataDescCreate(&EventData[1], &_Arg1, sizeof(const unsigned int)  );

    EventDataDescCreate(&EventData[2], &_Arg2, sizeof(const unsigned int)  );

    return EtwWrite(RegHandle, Descriptor, Activity, ARGUMENT_COUNT_qqq, EventData);
}
#endif

//
//Template from manifest : tidPepComponentFrequencyChange
//
#ifndef Template_pqqq_def
#define Template_pqqq_def
ETW_INLINE
ULONG
Template_pqqq(
    _In_ REGHANDLE RegHandle,
    _In_ PCEVENT_DESCRIPTOR Descriptor,
    _In_opt_ LPCGUID Activity,
    _In_opt_ const void *  _Arg0,
    _In_ const unsigned int  _Arg1,
    _In_ const unsigned int  _Arg2,
    _In_ const unsigned int  _Arg3
    )
{
#define ARGUMENT_COUNT_pqqq 4

    EVENT_DATA_DESCRIPTOR EventData[ARGUMENT_COUNT_pqqq];

    EventDataDescCreate(&EventData[0], &_Arg0, sizeof(PVOID)  );

    EventDataDescCreate(&EventData[1], &_Arg1, sizeof(const unsigned int)  );

    EventDataDescCreate(&EventData[2], &_Arg2, sizeof(const unsigned int)  );

    EventDataDescCreate(&EventData[3], &_Arg3, sizeof(const unsigned int)  );

    return EtwWrite(RegHandle, Descriptor, Activity, ARGUMENT_COUNT_pqqq, EventData);
}
#endif

//
//Template from manifest : tidPepPlatformIdleResidency
//
#ifndef Template_qNR0_def
#define Template_qNR0_def
ETW_INLINE
ULONG
Template_qNR0(
    _In_ REGHANDLE RegHandle,
    _In_ PCEVENT_DESCRIPTOR Descriptor,
    _In_opt_ LPCGUID Activity,
    _In_ const unsigned int  _Arg0,
    _In_ ULONG _Arg1_Len_,
    _In_ const PVOID  _Arg1
    )
{
#define ARGUMENT_COUNT_qNR0 2

    EVENT_DATA_DESCRIPTOR EventData[ARGUMENT_COUNT_qNR0];

    EventDataDescCreate(&EventData[0], &_Arg0, sizeof(const unsigned int)  );

    EventDataDescCreate(&EventData[1], _Arg1, _Arg0 * _Arg1_Len_);

    return EtwWrite(RegHandle, Descriptor, Activity, ARGUMENT_COUNT_qNR0, EventData);
}
#endif

//
//Template from manifest : tidPepPlatformIdleTransition
//
#ifndef Template_qq_def
#define Template_qq_def
ETW_INLINE
ULONG
Template_qq(
    _In_ REGHANDLE RegHandle,
    _In_ PCEVENT_DESCRIPTOR Descriptor,
    _In_opt_ LPCGUID Activity,
    _In_ const unsigned int  _Arg0,
    _In_ const unsigned int  _Arg1
    )
{
#define ARGUMENT_COUNT_qq 2

    EVENT_DATA_DESCRIPTOR EventData[ARGUMENT_COUNT_qq];

    EventDataDescCreate(&EventData[0], &_Arg0, sizeof(const unsigned int)  );

    EventDataDescCreate(&EventData[1], &_Arg1, sizeof(const unsigned int)  );

    return EtwWrite(RegHandle, Descriptor, Activity, ARGUMENT_COUNT_qq, EventData);
}
#endif

#endif // MCGEN_DISABLE_PROVIDER_CODE_GENERATION

#if defined(__cplusplus)
};
#endif

#define MSG_PepProviderName                  0x90000001L
