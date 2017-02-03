//-----------------------------------------------------------------------------
// <auto-generated>
//   This code was generated by a tool.
//
//   Changes to this file may cause incorrect behavior and will be lost if
//   the code is regenerated.
//
//   Tool: AllJoynCodeGenerator.exe
//
//   This tool is located in the Windows 10 SDK and the Windows 10 AllJoyn
//   Visual Studio Extension in the Visual Studio Gallery.
//
//   The generated code should be packaged in a Windows 10 C++/CX Runtime
//   Component which can be consumed in any UWP-supported language using
//   APIs that are available in Windows.Devices.AllJoyn.
//
//   Using AllJoynCodeGenerator - Invoke the following command with a valid
//   Introspection XML file and a writable output directory:
//     AllJoynCodeGenerator -i <INPUT XML FILE> -o <OUTPUT DIRECTORY>
// </auto-generated>
//-----------------------------------------------------------------------------
#include "pch.h"


using namespace concurrency;
using namespace Microsoft::WRL;
using namespace Platform;
using namespace Windows::Foundation;
using namespace Windows::Devices::AllJoyn;
using namespace org::alljoyn::Onboarding;

std::map<alljoyn_busobject, WeakReference*> OnboardingProducer::SourceObjects;
std::map<alljoyn_interfacedescription, WeakReference*> OnboardingProducer::SourceInterfaces;

OnboardingProducer::OnboardingProducer(AllJoynBusAttachment^ busAttachment)
    : m_busAttachment(busAttachment),
    m_sessionListener(nullptr),
    m_busObject(nullptr),
    m_sessionPort(0),
    m_sessionId(0)
{
    m_weak = new WeakReference(this);
    ServiceObjectPath = ref new String(L"/Onboarding");
    m_signals = ref new OnboardingSignals();
    m_busAttachmentStateChangedToken.Value = 0;
}

OnboardingProducer::~OnboardingProducer()
{
    UnregisterFromBus();
    delete m_weak;
}

void OnboardingProducer::UnregisterFromBus()
{
    if ((nullptr != m_busAttachment) && (0 != m_busAttachmentStateChangedToken.Value))
    {
        m_busAttachment->StateChanged -= m_busAttachmentStateChangedToken;
        m_busAttachmentStateChangedToken.Value = 0;
    }
    if (nullptr != SessionPortListener)
    {
        alljoyn_busattachment_unbindsessionport(AllJoynHelpers::GetInternalBusAttachment(m_busAttachment), m_sessionPort);
        alljoyn_sessionportlistener_destroy(SessionPortListener);
        SessionPortListener = nullptr;
    }
    if (nullptr != BusObject)
    {
        alljoyn_busattachment_unregisterbusobject(AllJoynHelpers::GetInternalBusAttachment(m_busAttachment), BusObject);
        alljoyn_busobject_destroy(BusObject);
        BusObject = nullptr;
    }
    if (nullptr != SessionListener)
    {
        alljoyn_sessionlistener_destroy(SessionListener);
        SessionListener = nullptr;
    }
}

bool OnboardingProducer::OnAcceptSessionJoiner(_In_ alljoyn_sessionport sessionPort, _In_ PCSTR joiner, _In_ const alljoyn_sessionopts opts)
{
    UNREFERENCED_PARAMETER(sessionPort); UNREFERENCED_PARAMETER(joiner); UNREFERENCED_PARAMETER(opts);

    return true;
}

void OnboardingProducer::OnSessionJoined(_In_ alljoyn_sessionport sessionPort, _In_ alljoyn_sessionid id, _In_ PCSTR joiner)
{
    UNREFERENCED_PARAMETER(joiner);

    // We initialize the Signals object after the session has been joined, because it needs
    // the session id.
    m_signals->Initialize(BusObject, id);
    m_sessionPort = sessionPort;
    m_sessionId = id;

    alljoyn_sessionlistener_callbacks callbacks =
    {
        AllJoynHelpers::SessionLostHandler<OnboardingProducer>,
        AllJoynHelpers::SessionMemberAddedHandler<OnboardingProducer>,
        AllJoynHelpers::SessionMemberRemovedHandler<OnboardingProducer>
    };

    SessionListener = alljoyn_sessionlistener_create(&callbacks, m_weak);
    alljoyn_busattachment_setsessionlistener(AllJoynHelpers::GetInternalBusAttachment(m_busAttachment), id, SessionListener);
}

void OnboardingProducer::OnSessionLost(_In_ alljoyn_sessionid sessionId, _In_ alljoyn_sessionlostreason reason)
{
    if (sessionId == m_sessionId)
    {
        AllJoynSessionLostEventArgs^ args = ref new AllJoynSessionLostEventArgs(static_cast<AllJoynSessionLostReason>(reason));
        AllJoynHelpers::DispatchEvent([=]() {
            SessionLost(this, args);
        });
    }
}

void OnboardingProducer::OnSessionMemberAdded(_In_ alljoyn_sessionid sessionId, _In_ PCSTR uniqueName)
{
    if (sessionId == m_sessionId)
    {
        auto args = ref new AllJoynSessionMemberAddedEventArgs(AllJoynHelpers::MultibyteToPlatformString(uniqueName));
        AllJoynHelpers::DispatchEvent([=]() {
            SessionMemberAdded(this, args);
        });
    }
}

void OnboardingProducer::OnSessionMemberRemoved(_In_ alljoyn_sessionid sessionId, _In_ PCSTR uniqueName)
{
    if (sessionId == m_sessionId)
    {
        auto args = ref new AllJoynSessionMemberRemovedEventArgs(AllJoynHelpers::MultibyteToPlatformString(uniqueName));
        AllJoynHelpers::DispatchEvent([=]() {
            SessionMemberRemoved(this, args);
        });
    }
}

void OnboardingProducer::BusAttachmentStateChanged(_In_ AllJoynBusAttachment^ sender, _In_ AllJoynBusAttachmentStateChangedEventArgs^ args)
{
    if (args->State == AllJoynBusAttachmentState::Connected)
    {
        QStatus result = AllJoynHelpers::CreateProducerSession<OnboardingProducer>(m_busAttachment, m_weak);
        if (ER_OK != result)
        {
            StopInternal(result);
            return;
        }
    }
    else if (args->State == AllJoynBusAttachmentState::Disconnected)
    {
        StopInternal(ER_BUS_STOPPING);
    }
}

void OnboardingProducer::CallConfigureWifiHandler(_Inout_ alljoyn_busobject busObject, _In_ alljoyn_message message)
{
    auto source = SourceObjects.find(busObject);
    if (source == SourceObjects.end())
    {
        return;
    }

    OnboardingProducer^ producer = source->second->Resolve<OnboardingProducer>();
    if (producer->Service != nullptr)
    {
        AllJoynMessageInfo^ callInfo = ref new AllJoynMessageInfo(AllJoynHelpers::MultibyteToPlatformString(alljoyn_message_getsender(message)));

        Platform::String^ inputArg0;
        TypeConversionHelpers::GetAllJoynMessageArg(alljoyn_message_getarg(message, 0), "s", &inputArg0);
        Platform::String^ inputArg1;
        TypeConversionHelpers::GetAllJoynMessageArg(alljoyn_message_getarg(message, 1), "s", &inputArg1);
        int16 inputArg2;
        TypeConversionHelpers::GetAllJoynMessageArg(alljoyn_message_getarg(message, 2), "n", &inputArg2);

        create_task(producer->Service->ConfigureWifiAsync(callInfo, inputArg0, inputArg1, inputArg2)).then([busObject, message](OnboardingConfigureWifiResult^ result)
        {
            int32 status;

            if (nullptr == result)
            {
                alljoyn_busobject_methodreply_status(busObject, message, ER_BUS_NO_LISTENER);
                return;
            }

            status = result->Status;
            if (AllJoynStatus::Ok != status)
            {
                alljoyn_busobject_methodreply_status(busObject, message, static_cast<QStatus>(status));
                return;
            }

            size_t argCount = 1;
            alljoyn_msgarg outputs = alljoyn_msgarg_array_create(argCount);

            status = TypeConversionHelpers::SetAllJoynMessageArg(alljoyn_msgarg_array_element(outputs, 0), "n", result->Status2);
            if (AllJoynStatus::Ok != status)
            {
                alljoyn_busobject_methodreply_status(busObject, message, static_cast<QStatus>(status));
                alljoyn_msgarg_destroy(outputs);
                return;
            }

            alljoyn_busobject_methodreply_args(busObject, message, outputs, argCount);
            alljoyn_msgarg_destroy(outputs);
        }).wait();
    }
}

void OnboardingProducer::CallConnectHandler(_Inout_ alljoyn_busobject busObject, _In_ alljoyn_message message)
{
    auto source = SourceObjects.find(busObject);
    if (source == SourceObjects.end())
    {
        return;
    }

    OnboardingProducer^ producer = source->second->Resolve<OnboardingProducer>();
    if (producer->Service != nullptr)
    {
        AllJoynMessageInfo^ callInfo = ref new AllJoynMessageInfo(AllJoynHelpers::MultibyteToPlatformString(alljoyn_message_getsender(message)));


        create_task(producer->Service->ConnectAsync(callInfo)).then([busObject, message](OnboardingConnectResult^ result)
        {
            int32 status;

            if (nullptr == result)
            {
                alljoyn_busobject_methodreply_status(busObject, message, ER_BUS_NO_LISTENER);
                return;
            }

            status = result->Status;
            if (AllJoynStatus::Ok != status)
            {
                alljoyn_busobject_methodreply_status(busObject, message, static_cast<QStatus>(status));
                return;
            }

            size_t argCount = 0;
            alljoyn_msgarg outputs = alljoyn_msgarg_array_create(argCount);

            alljoyn_busobject_methodreply_args(busObject, message, outputs, argCount);
            alljoyn_msgarg_destroy(outputs);
        }).wait();
    }
}

void OnboardingProducer::CallOffboardHandler(_Inout_ alljoyn_busobject busObject, _In_ alljoyn_message message)
{
    auto source = SourceObjects.find(busObject);
    if (source == SourceObjects.end())
    {
        return;
    }

    OnboardingProducer^ producer = source->second->Resolve<OnboardingProducer>();
    if (producer->Service != nullptr)
    {
        AllJoynMessageInfo^ callInfo = ref new AllJoynMessageInfo(AllJoynHelpers::MultibyteToPlatformString(alljoyn_message_getsender(message)));


        create_task(producer->Service->OffboardAsync(callInfo)).then([busObject, message](OnboardingOffboardResult^ result)
        {
            int32 status;

            if (nullptr == result)
            {
                alljoyn_busobject_methodreply_status(busObject, message, ER_BUS_NO_LISTENER);
                return;
            }

            status = result->Status;
            if (AllJoynStatus::Ok != status)
            {
                alljoyn_busobject_methodreply_status(busObject, message, static_cast<QStatus>(status));
                return;
            }

            size_t argCount = 0;
            alljoyn_msgarg outputs = alljoyn_msgarg_array_create(argCount);

            alljoyn_busobject_methodreply_args(busObject, message, outputs, argCount);
            alljoyn_msgarg_destroy(outputs);
        }).wait();
    }
}

void OnboardingProducer::CallGetScanInfoHandler(_Inout_ alljoyn_busobject busObject, _In_ alljoyn_message message)
{
    auto source = SourceObjects.find(busObject);
    if (source == SourceObjects.end())
    {
        return;
    }

    OnboardingProducer^ producer = source->second->Resolve<OnboardingProducer>();
    if (producer->Service != nullptr)
    {
        AllJoynMessageInfo^ callInfo = ref new AllJoynMessageInfo(AllJoynHelpers::MultibyteToPlatformString(alljoyn_message_getsender(message)));


        create_task(producer->Service->GetScanInfoAsync(callInfo)).then([busObject, message](OnboardingGetScanInfoResult^ result)
        {
            int32 status;

            if (nullptr == result)
            {
                alljoyn_busobject_methodreply_status(busObject, message, ER_BUS_NO_LISTENER);
                return;
            }

            status = result->Status;
            if (AllJoynStatus::Ok != status)
            {
                alljoyn_busobject_methodreply_status(busObject, message, static_cast<QStatus>(status));
                return;
            }

            size_t argCount = 2;
            alljoyn_msgarg outputs = alljoyn_msgarg_array_create(argCount);

            status = TypeConversionHelpers::SetAllJoynMessageArg(alljoyn_msgarg_array_element(outputs, 0), "q", result->Age);
            if (AllJoynStatus::Ok != status)
            {
                alljoyn_busobject_methodreply_status(busObject, message, static_cast<QStatus>(status));
                alljoyn_msgarg_destroy(outputs);
                return;
            }

            status = TypeConversionHelpers::SetAllJoynMessageArg(alljoyn_msgarg_array_element(outputs, 1), "a(sn)", result->ScanList);
            if (AllJoynStatus::Ok != status)
            {
                alljoyn_busobject_methodreply_status(busObject, message, static_cast<QStatus>(status));
                alljoyn_msgarg_destroy(outputs);
                return;
            }

            alljoyn_busobject_methodreply_args(busObject, message, outputs, argCount);
            alljoyn_msgarg_destroy(outputs);
        }).wait();
    }
}

void OnboardingProducer::CallConnectionResultSignalHandler(_In_ const alljoyn_interfacedescription_member* member, _In_ alljoyn_message message)
{
    auto source = SourceInterfaces.find(member->iface);
    if (source == SourceInterfaces.end())
    {
        return;
    }

    auto producer = source->second->Resolve<OnboardingProducer>();
    if (producer->Signals != nullptr)
    {
        auto callInfo = ref new AllJoynMessageInfo(AllJoynHelpers::MultibyteToPlatformString(alljoyn_message_getsender(message)));
        auto eventArgs = ref new OnboardingConnectionResultReceivedEventArgs();
        eventArgs->MessageInfo = callInfo;

        Onboarding^ argument0;
        TypeConversionHelpers::GetAllJoynMessageArg(alljoyn_message_getarg(message, 0), "(ns)", &argument0);
        eventArgs->Arg = argument0;

        producer->Signals->CallConnectionResultReceived(producer->Signals, eventArgs);
    }
}

QStatus OnboardingProducer::AddMethodHandler(_In_ alljoyn_interfacedescription interfaceDescription, _In_ PCSTR methodName, _In_ alljoyn_messagereceiver_methodhandler_ptr handler)
{
    alljoyn_interfacedescription_member member;
    if (!alljoyn_interfacedescription_getmember(interfaceDescription, methodName, &member))
    {
        return ER_BUS_INTERFACE_NO_SUCH_MEMBER;
    }

    return alljoyn_busobject_addmethodhandler(
        m_busObject,
        member,
        handler,
        m_weak);
}

QStatus OnboardingProducer::AddSignalHandler(_In_ alljoyn_busattachment busAttachment, _In_ alljoyn_interfacedescription interfaceDescription, _In_ PCSTR methodName, _In_ alljoyn_messagereceiver_signalhandler_ptr handler)
{
    alljoyn_interfacedescription_member member;
    if (!alljoyn_interfacedescription_getmember(interfaceDescription, methodName, &member))
    {
        return ER_BUS_INTERFACE_NO_SUCH_MEMBER;
    }

    return alljoyn_busattachment_registersignalhandler(busAttachment, handler, member, NULL);
}

QStatus OnboardingProducer::OnPropertyGet(_In_ PCSTR interfaceName, _In_ PCSTR propertyName, _Inout_ alljoyn_msgarg value)
{
    UNREFERENCED_PARAMETER(interfaceName);

    if (0 == strcmp(propertyName, "Version"))
    {
        auto task = create_task(Service->GetVersionAsync(nullptr));
        auto result = task.get();

        if (AllJoynStatus::Ok != result->Status)
        {
            return static_cast<QStatus>(result->Status);
        }

        return static_cast<QStatus>(TypeConversionHelpers::SetAllJoynMessageArg(value, "q", result->Version));
    }
    if (0 == strcmp(propertyName, "State"))
    {
        auto task = create_task(Service->GetStateAsync(nullptr));
        auto result = task.get();

        if (AllJoynStatus::Ok != result->Status)
        {
            return static_cast<QStatus>(result->Status);
        }

        return static_cast<QStatus>(TypeConversionHelpers::SetAllJoynMessageArg(value, "n", result->State));
    }
    if (0 == strcmp(propertyName, "LastError"))
    {
        auto task = create_task(Service->GetLastErrorAsync(nullptr));
        auto result = task.get();

        if (AllJoynStatus::Ok != result->Status)
        {
            return static_cast<QStatus>(result->Status);
        }

        return static_cast<QStatus>(TypeConversionHelpers::SetAllJoynMessageArg(value, "(ns)", result->LastError));
    }

    return ER_BUS_NO_SUCH_PROPERTY;
}

QStatus OnboardingProducer::OnPropertySet(_In_ PCSTR interfaceName, _In_ PCSTR propertyName, _In_ alljoyn_msgarg value)
{
    UNREFERENCED_PARAMETER(interfaceName);

    return ER_BUS_NO_SUCH_PROPERTY;
}

void OnboardingProducer::Start()
{
    if (nullptr == m_busAttachment)
    {
        StopInternal(ER_FAIL);
        return;
    }

    QStatus result = AllJoynHelpers::CreateInterfaces(m_busAttachment, c_OnboardingIntrospectionXml);
    if (result != ER_OK)
    {
        StopInternal(result);
        return;
    }

    result = AllJoynHelpers::CreateBusObject<OnboardingProducer>(m_weak);
    if (result != ER_OK)
    {
        StopInternal(result);
        return;
    }

    alljoyn_interfacedescription interfaceDescription = alljoyn_busattachment_getinterface(AllJoynHelpers::GetInternalBusAttachment(m_busAttachment), "org.alljoyn.Onboarding");
    if (interfaceDescription == nullptr)
    {
        StopInternal(ER_FAIL);
        return;
    }
    alljoyn_busobject_addinterface_announced(BusObject, interfaceDescription);


    // NOTE: The Allseen Alliance onboarding spec (14.12 and earlier) list three possible names for the Wifi Onboarding Configuration method.
    // (See https://allseenalliance.org/framework/documentation/learn/base-services/onboarding/interface).  There is currently an open
    // issue with the Allseen Alliance https://jira.allseenalliance.org/browse/ASADOC-57, but we are conforming to the Windows 10 DAFIoT
    // and Lifx Light-Bulb standard
    result = AddMethodHandler(
        interfaceDescription,
        "ConfigureWiFi",
        [](alljoyn_busobject busObject, const alljoyn_interfacedescription_member* member, alljoyn_message message) { UNREFERENCED_PARAMETER(member); CallConfigureWifiHandler(busObject, message); });
    if (result != ER_OK)
    {
        StopInternal(result);
        return;
    }

    result = AddMethodHandler(
        interfaceDescription,
        "Connect",
        [](alljoyn_busobject busObject, const alljoyn_interfacedescription_member* member, alljoyn_message message) { UNREFERENCED_PARAMETER(member); CallConnectHandler(busObject, message); });
    if (result != ER_OK)
    {
        StopInternal(result);
        return;
    }

    result = AddMethodHandler(
        interfaceDescription,
        "Offboard",
        [](alljoyn_busobject busObject, const alljoyn_interfacedescription_member* member, alljoyn_message message) { UNREFERENCED_PARAMETER(member); CallOffboardHandler(busObject, message); });
    if (result != ER_OK)
    {
        StopInternal(result);
        return;
    }

    result = AddMethodHandler(
        interfaceDescription,
        "GetScanInfo",
        [](alljoyn_busobject busObject, const alljoyn_interfacedescription_member* member, alljoyn_message message) { UNREFERENCED_PARAMETER(member); CallGetScanInfoHandler(busObject, message); });
    if (result != ER_OK)
    {
        StopInternal(result);
        return;
    }

    result = AddSignalHandler(
        AllJoynHelpers::GetInternalBusAttachment(m_busAttachment),
        interfaceDescription,
        "ConnectionResult",
        [](const alljoyn_interfacedescription_member* member, PCSTR srcPath, alljoyn_message message) { UNREFERENCED_PARAMETER(srcPath); CallConnectionResultSignalHandler(member, message); });
    if (result != ER_OK)
    {
        StopInternal(result);
        return;
    }

    SourceObjects[m_busObject] = m_weak;
    SourceInterfaces[interfaceDescription] = m_weak;

    unsigned int noneMechanismIndex = 0;
    bool authenticationMechanismsContainsNone = m_busAttachment->AuthenticationMechanisms->IndexOf(AllJoynAuthenticationMechanism::None, &noneMechanismIndex);
    QCC_BOOL interfaceIsSecure = alljoyn_interfacedescription_issecure(interfaceDescription);

    // If the current set of AuthenticationMechanisms supports authentication,
    // determine whether a secure BusObject is required.
    if (AllJoynHelpers::CanSecure(m_busAttachment->AuthenticationMechanisms))
    {
        // Register the BusObject as "secure" if the org.alljoyn.Bus.Secure XML annotation
        // is specified, or if None is not present in AuthenticationMechanisms.
        if (!authenticationMechanismsContainsNone || interfaceIsSecure)
        {
            result = alljoyn_busattachment_registerbusobject_secure(AllJoynHelpers::GetInternalBusAttachment(m_busAttachment), BusObject);
        }
        else
        {
            result = alljoyn_busattachment_registerbusobject(AllJoynHelpers::GetInternalBusAttachment(m_busAttachment), BusObject);
        }
    }
    else
    {
        // If the current set of AuthenticationMechanisms does not support authentication
        // but the interface requires security, report an error.
        if (interfaceIsSecure)
        {
            result = ER_BUS_NO_AUTHENTICATION_MECHANISM;
        }
        else
        {
            result = alljoyn_busattachment_registerbusobject(AllJoynHelpers::GetInternalBusAttachment(m_busAttachment), BusObject);
        }
    }

    if (result != ER_OK)
    {
        StopInternal(result);
        return;
    }

    m_busAttachmentStateChangedToken = m_busAttachment->StateChanged += ref new TypedEventHandler<AllJoynBusAttachment^,AllJoynBusAttachmentStateChangedEventArgs^>(this, &OnboardingProducer::BusAttachmentStateChanged);
    m_busAttachment->Connect();
}

void OnboardingProducer::Stop()
{
    StopInternal(AllJoynStatus::Ok);
}

void OnboardingProducer::StopInternal(int32 status)
{
    UnregisterFromBus();
    Stopped(this, ref new AllJoynProducerStoppedEventArgs(status));
}

int32 OnboardingProducer::RemoveMemberFromSession(_In_ String^ uniqueName)
{
    return alljoyn_busattachment_removesessionmember(
        AllJoynHelpers::GetInternalBusAttachment(m_busAttachment),
        m_sessionId,
        AllJoynHelpers::PlatformToMultibyteString(uniqueName).data());
}

PCSTR org::alljoyn::Onboarding::c_OnboardingIntrospectionXml = "<interface name=\"org.alljoyn.Onboarding\">"
"  <description>A secure onboarding interface.  See https://allseenalliance.org/framework/documentation/learn/base-services/onboarding/interface </description>"
"  <annotation name=\"org.alljoyn.Bus.Secure\" value=\"true\" />"
"  <property name=\"Version\" type=\"q\" access=\"read\" />"
"  <property name=\"State\" type=\"n\" access=\"read\" />"
"  <property name=\"LastError\" type=\"(ns)\" access=\"read\" />"
"  <method name=\"ConfigureWiFi\">"
"    <arg name=\"SSID\" type=\"s\" direction=\"in\" />"
"    <arg name=\"passphrase\" type=\"s\" direction=\"in\" />"
"    <arg name=\"authType\" type=\"n\" direction=\"in\" />"
"    <arg name=\"status\" type=\"n\" direction=\"out\" />"
"  </method>"
"  <method name=\"Connect\">"
"    <annotation name=\"org.freedesktop.DBus.Method.NoReply\" value=\"true\" />"
"  </method>"
"  <method name=\"Offboard\">"
"    <annotation name=\"org.freedesktop.DBus.Method.NoReply\" value=\"true\" />"
"  </method>"
"  <method name=\"GetScanInfo\">"
"    <arg name=\"age\" type=\"q\" direction=\"out\" />"
"    <arg name=\"scanList\" type=\"a(sn)\" direction=\"out\" />"
"  </method>"
"  <signal name=\"ConnectionResult\">"
"    <arg type=\"(ns)\" />"
"  </signal>"
"</interface>"
;
