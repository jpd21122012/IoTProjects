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
#pragma once

namespace org { namespace alljoyn { namespace Icon {

// This class, and the associated EventArgs classes, exist for the benefit of JavaScript developers who
// do not have the ability to implement IIconService. Instead, IconServiceEventAdapter
// provides the Interface implementation and exposes a set of compatible events to the developer.
public ref class IconServiceEventAdapter sealed : [Windows::Foundation::Metadata::Default] IIconService
{
public:
    // Method Invocation Events
    event Windows::Foundation::TypedEventHandler<IconServiceEventAdapter^, IconGetUrlCalledEventArgs^>^ GetUrlCalled;
    event Windows::Foundation::TypedEventHandler<IconServiceEventAdapter^, IconGetContentCalledEventArgs^>^ GetContentCalled;

    // Property Read Events
    event Windows::Foundation::TypedEventHandler<IconServiceEventAdapter^, IconGetVersionRequestedEventArgs^>^ GetVersionRequested;
    event Windows::Foundation::TypedEventHandler<IconServiceEventAdapter^, IconGetMimeTypeRequestedEventArgs^>^ GetMimeTypeRequested;
    event Windows::Foundation::TypedEventHandler<IconServiceEventAdapter^, IconGetSizeRequestedEventArgs^>^ GetSizeRequested;
    
    // Property Write Events

    // IIconService Implementation
    virtual Windows::Foundation::IAsyncOperation<IconGetUrlResult^>^ GetUrlAsync(_In_ Windows::Devices::AllJoyn::AllJoynMessageInfo^ info);
    virtual Windows::Foundation::IAsyncOperation<IconGetContentResult^>^ GetContentAsync(_In_ Windows::Devices::AllJoyn::AllJoynMessageInfo^ info);

    virtual Windows::Foundation::IAsyncOperation<IconGetVersionResult^>^ GetVersionAsync(_In_ Windows::Devices::AllJoyn::AllJoynMessageInfo^ info);
    virtual Windows::Foundation::IAsyncOperation<IconGetMimeTypeResult^>^ GetMimeTypeAsync(_In_ Windows::Devices::AllJoyn::AllJoynMessageInfo^ info);
    virtual Windows::Foundation::IAsyncOperation<IconGetSizeResult^>^ GetSizeAsync(_In_ Windows::Devices::AllJoyn::AllJoynMessageInfo^ info);

};

} } } 
