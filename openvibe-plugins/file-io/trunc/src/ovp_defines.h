#ifndef __OpenViBEPlugins_Defines_H__
#define __OpenViBEPlugins_Defines_H__

//___________________________________________________________________//
//                                                                   //
// Plugin Object Descriptor Class Identifiers                        //
//___________________________________________________________________//
//                                                                   //

#define OVP_ClassId_EEGStreamWriterGDFDesc                  OpenViBE::CIdentifier(0x375BF8EC, 0x023ACEEB)
#define OVP_ClassId_GDFReaderDesc                           OpenViBE::CIdentifier(0x2E2543C1, 0x47E3739E)
#define OVP_ClassId_DataStreamWriterDesc                    OpenViBE::CIdentifier(0x02817C77, 0x77FE3D6A)
#define OVP_ClassId_DataStreamReaderDesc                    OpenViBE::CIdentifier(0x1E8AAB1A, 0x085D72F6)

/*
#define OVP_ClassId_                                        OpenViBE::CIdentifier(0x00CC1B39, 0x206A17AE)
#define OVP_ClassId_                                        OpenViBE::CIdentifier(0x000C486C, 0x477C80B9)
#define OVP_ClassId_                                        OpenViBE::CIdentifier(0x002AEE72, 0x288D489E)
#define OVP_ClassId_                                        OpenViBE::CIdentifier(0x00627156, 0x55790CAE)
#define OVP_ClassId_                                        OpenViBE::CIdentifier(0x003CE019, 0x3DADE050)
#define OVP_ClassId_                                        OpenViBE::CIdentifier(0x00D3CABB, 0x339326C2)
#define OVP_ClassId_                                        OpenViBE::CIdentifier(0x0073A91A, 0x6D1D3D26)
#define OVP_ClassId_                                        OpenViBE::CIdentifier(0x008F7C49, 0x6ED710A9)
#define OVP_ClassId_                                        OpenViBE::CIdentifier(0x00BCF286, 0x6F5FA2F4)
#define OVP_ClassId_                                        OpenViBE::CIdentifier(0x00C3ACAB, 0x4E3DE014)
#define OVP_ClassId_                                        OpenViBE::CIdentifier(0x003CD062, 0x739F973E)
#define OVP_ClassId_                                        OpenViBE::CIdentifier(0x003462EA, 0x031FB8FA)
#define OVP_ClassId_                                        OpenViBE::CIdentifier(0x00B1E3DF, 0x3ABC6C5A)
#define OVP_ClassId_                                        OpenViBE::CIdentifier(0x00E7D5F9, 0x1471AFF2)
#define OVP_ClassId_                                        OpenViBE::CIdentifier(0x00426B2C, 0x3536CCD5)
*/

//___________________________________________________________________//
//                                                                   //
// Plugin Object Class Identifiers                                   //
//___________________________________________________________________//
//                                                                   //

#define OVP_ClassId_EEGStreamWriterGDF                      OpenViBE::CIdentifier(0x1E7B2155, 0x107289CE)
#define OVP_ClassId_GDFReader                               OpenViBE::CIdentifier(0x3EEB1264, 0x4EDFBD9A)
#define OVP_ClassId_DataStreamWriter                        OpenViBE::CIdentifier(0x78EA86B0, 0x2933E255)
#define OVP_ClassId_DataStreamReader                        OpenViBE::CIdentifier(0x0B1D880D, 0x02A17229)

/*
#define OVP_ClassId_                                        OpenViBE::CIdentifier(0x0085B814, 0x6E45DA7E)
#define OVP_ClassId_                                        OpenViBE::CIdentifier(0x00708DD4, 0x451591A0)
#define OVP_ClassId_                                        OpenViBE::CIdentifier(0x00962006, 0x3669F46C)
#define OVP_ClassId_                                        OpenViBE::CIdentifier(0x002089B5, 0x55EF27A6)
#define OVP_ClassId_                                        OpenViBE::CIdentifier(0x00B44655, 0x63C9F129)
#define OVP_ClassId_                                        OpenViBE::CIdentifier(0x00BD34BB, 0x1B2A01A2)
#define OVP_ClassId_                                        OpenViBE::CIdentifier(0x001B8BE7, 0x2F2BD5AA)
#define OVP_ClassId_                                        OpenViBE::CIdentifier(0x00FD1CCE, 0x6489D3C4)
#define OVP_ClassId_                                        OpenViBE::CIdentifier(0x00E45EAD, 0x55DCA825)
#define OVP_ClassId_                                        OpenViBE::CIdentifier(0x0090D564, 0x35B4183A)
#define OVP_ClassId_                                        OpenViBE::CIdentifier(0x00A559A0, 0x21995AF7)
#define OVP_ClassId_                                        OpenViBE::CIdentifier(0x002E71DC, 0x7ED572A4)
#define OVP_ClassId_                                        OpenViBE::CIdentifier(0x000B7B5F, 0x12E69666)
#define OVP_ClassId_                                        OpenViBE::CIdentifier(0x00012121, 0x6375CD36)
#define OVP_ClassId_                                        OpenViBE::CIdentifier(0x00822759, 0x12AEFC43)
*/

//___________________________________________________________________//
//                                                                   //
// Operating System identification                                   //
//___________________________________________________________________//
//                                                                   //

// #define OVP_OS_Linux
// #define OVP_OS_Windows
// #define OVP_OS_MacOS
// #define OVP_OS_

#if defined TARGET_OS_Windows
 #define OVP_OS_Windows
#elif defined TARGET_OS_Linux
 #define OVP_OS_Linux
#elif defined TARGET_OS_MacOS
 #define OVP_OS_MacOS
#else
 #warning "No target operating system defined !"
#endif

//___________________________________________________________________//
//                                                                   //
// Hardware Architecture identification                              //
//___________________________________________________________________//
//                                                                   //

// #define OVP_ARCHITECTURE_i386
// #define OVP_ARCHITECTURE_

#if defined TARGET_ARCHITECTURE_i386
 #define OVP_ARCHITECTURE_i386
#else
 #warning "No target architecture defined !"
#endif

//___________________________________________________________________//
//                                                                   //
// Compilator software identification                                //
//___________________________________________________________________//
//                                                                   //

// #define OVP_COMPILATOR_GCC
// #define OVP_COMPILATOR_VisualStudio
// #define OVP_COMPILATOR_

#if defined TARGET_COMPILATOR_GCC
 #define OVP_COMPILATOR_GCC
#elif defined TARGET_COMPILATOR_VisualStudio
 #define OVP_COMPILATOR_VisualStudio
#else
 #warning "No target compilator defined !"
#endif

//___________________________________________________________________//
//                                                                   //
// API Definition                                                    //
//___________________________________________________________________//
//                                                                   //

// Taken from
// - http://people.redhat.com/drepper/dsohowto.pdf
// - http://www.nedprod.com/programs/gccvisibility.html
#if defined OVP_Shared
 #if defined OVP_OS_Windows
  #define OVP_API_Export __declspec(dllexport)
  #define OVP_API_Import __declspec(dllimport)
 #elif defined OVP_OS_Linux
  #define OVP_API_Export __attribute__((visibility("default")))
  #define OVP_API_Import __attribute__((visibility("default")))
 #else
  #define OVP_API_Export
  #define OVP_API_Import
 #endif
#else
 #define OVP_API_Export
 #define OVP_API_Import
#endif

#if defined OVP_Exports
 #define OVP_API OVP_API_Export
#else
 #define OVP_API OVP_API_Import
#endif

//___________________________________________________________________//
//                                                                   //
// NULL Definition                                                   //
//___________________________________________________________________//
//                                                                   //

#ifndef NULL
#define NULL 0
#endif

#endif // __OpenViBEPlugins_Defines_H__
