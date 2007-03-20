#ifndef __OpenViBEToolkit_Defines_H__
#define __OpenViBEToolkit_Defines_H__

//___________________________________________________________________//
//                                                                   //
// ISO 5218 conformant sex identifiers                               //
//___________________________________________________________________//
//                                                                   //

#define OVTK_Value_Sex_Unknown                                          0
#define OVTK_Value_Sex_Male                                             1
#define OVTK_Value_Sex_Female                                           2
#define OVTK_Value_Sex_NotSpecified                                     9

//___________________________________________________________________//
//                                                                   //
// Standard node identifiers                                         //
//___________________________________________________________________//
//                                                                   //

#define OVTK_NodeId_StreamVersion                                       EBML::CIdentifier(0x006F5A08, 0x7796EBC5)

#define OVTK_NodeId_StreamedMatrix_Header                               EBML::CIdentifier(0x0072F560, 0x7ED2CBED)
#define OVTK_NodeId_StreamedMatrix_DimmensionCount                      EBML::CIdentifier(0x003FEBD4, 0x2725D428)
#define OVTK_NodeId_StreamedMatrix_Dimmension                           EBML::CIdentifier(0x0000E3C0, 0x3A7D5141)
#define OVTK_NodeId_StreamedMatrix_Dimmension_Size                      EBML::CIdentifier(0x001302F7, 0x36D8438A)
#define OVTK_NodeId_StreamedMatrix_Dimmension_Label                     EBML::CIdentifier(0x00153E40, 0x190227E0)
#define OVTK_NodeId_StreamedMatrix_Buffer                               EBML::CIdentifier(0x00120663, 0x08FBC165)
#define OVTK_NodeId_StreamedMatrix_RawBuffer                            EBML::CIdentifier(0x00B18C10, 0x427D098C)

//___________________________________________________________________//
//                                                                   //
// Acquisition stream node identifiers                               //
//___________________________________________________________________//
//                                                                   //

/*
 * Acquisition stream description (fixed on july 2006)
 *
 * version 1 :
 * ----------------------------------------------------------------- *
 * OVTK_NodeId_Acquisition_Header
 *   OVTK_NodeId_Acquisition_AcquisitionInformation
 *     OVTK_NodeId_Acquisition_ExperimentId (integer)
 *     OVTK_NodeId_Acquisition_SubjectAge (integer)
 *     OVTK_NodeId_Acquisition_SubjectSex (integer)
 *   OVTK_NodeId_Acquisition_ChannelCount (integer)
 *   OVTK_NodeId_Acquisition_SamplingFrequency (integer)
 *   OVTK_NodeId_Acquisition_GainFactors
 *     OVTK_NodeId_Acquisition_GainFactor (integer)
 *     OVTK_NodeId_Acquisition_GainFactor (integer)
 *     ...
 *   OVTK_NodeId_Acquisition_ChannelNames
 *     OVTK_NodeId_Acquisition_ChannelName (string)
 *     OVTK_NodeId_Acquisition_ChannelName (string)
 *     ...
 *   OVTK_NodeId_Acquisition_ChannelLocations
 *     OVTK_NodeId_Acquisition_ChannelLocation (array of 3 float64)
 *     OVTK_NodeId_Acquisition_ChannelLocation (array of 3 float64)
 *     ...
 * OVTK_NodeId_Acquisition_Buffer
 *   OVTK_NodeId_Acquisition_Samples
 *     OVTK_NodeId_Acquisition_SamplesPerChannelCount (integer)
 *     OVTK_NodeId_Acquisition_SampleBlock (array of float32)
 *     OVTK_NodeId_Acquisition_SampleBlock (array of float32)
 *     ...
 *   OVTK_NodeId_Acquisition_Stimulations
 *     OVTK_NodeId_Acquisition_StimulationsCount (integer)
 *     OVTK_NodeId_Acquisition_Stimulation
 *       OVTK_NodeId_Acquisition_StimulationSampleIndex (integer)
 *       OVTK_NodeId_Acquisition_StimulationIdentifier (integer)
 *     OVTK_NodeId_Acquisition_Stimulation
 *     ...
 * OVTK_NodeId_Acquisition_Buffer
 * ...
 */

#define OVTK_NodeId_Acquisition_Header                              EBML::CIdentifier(0x00000000, 0x00004239)
#define OVTK_NodeId_Acquisition_AcquisitionInformation              EBML::CIdentifier(0x00000000, 0x00004240)
#define OVTK_NodeId_Acquisition_ExperimentId                        EBML::CIdentifier(0x00000000, 0x00004241)
#define OVTK_NodeId_Acquisition_SubjectAge                          EBML::CIdentifier(0x00000000, 0x00004242)
#define OVTK_NodeId_Acquisition_SubjectSex                          EBML::CIdentifier(0x00000000, 0x00004243)
#define OVTK_NodeId_Acquisition_ChannelCount                        EBML::CIdentifier(0x00000000, 0x00004244)
#define OVTK_NodeId_Acquisition_SamplingFrequency                   EBML::CIdentifier(0x00000000, 0x00004245)
#define OVTK_NodeId_Acquisition_ChannelNames                        EBML::CIdentifier(0x00000000, 0x00004246)
#define OVTK_NodeId_Acquisition_ChannelName                         EBML::CIdentifier(0x00000000, 0x00004247)
#define OVTK_NodeId_Acquisition_GainFactors                         EBML::CIdentifier(0x00000000, 0x00004248)
#define OVTK_NodeId_Acquisition_GainFactor                          EBML::CIdentifier(0x00000000, 0x00004249)
#define OVTK_NodeId_Acquisition_ChannelLocations                    EBML::CIdentifier(0x00000000, 0x00004250)
#define OVTK_NodeId_Acquisition_ChannelLocation                     EBML::CIdentifier(0x00000000, 0x00004251)
#define OVTK_NodeId_Acquisition_Buffer                              EBML::CIdentifier(0x00000000, 0x0000005A)
#define OVTK_NodeId_Acquisition_Samples                             EBML::CIdentifier(0x00000000, 0x0000005B)
#define OVTK_NodeId_Acquisition_SamplesPerChannelCount              EBML::CIdentifier(0x00000000, 0x0000005C)
#define OVTK_NodeId_Acquisition_SampleBlock                         EBML::CIdentifier(0x00000000, 0x0000005D)
#define OVTK_NodeId_Acquisition_Stimulations                        EBML::CIdentifier(0x00000000, 0x00000060)
#define OVTK_NodeId_Acquisition_StimulationsCount                   EBML::CIdentifier(0x00000000, 0x00000061)
#define OVTK_NodeId_Acquisition_Stimulation                         EBML::CIdentifier(0x00000000, 0x00000062)
#define OVTK_NodeId_Acquisition_StimulationSampleIndex              EBML::CIdentifier(0x00000000, 0x00000063)
#define OVTK_NodeId_Acquisition_StimulationIdentifier               EBML::CIdentifier(0x00000000, 0x00000064)

//___________________________________________________________________//
//                                                                   //
// Signal stream node identifiers                                    //
//___________________________________________________________________//
//                                                                   //

/*
 * Signal stream description (fixed on november 6th 2006)
 *
 * version 1 :
 * ----------------------------------------------------------------- *
 * OVTK_NodeId_Signal_Header
 *   OVTK_NodeId_StreamVersion (integer:1)
 *   OVTK_NodeId_Signal_SamplingRate (integer)
 *   OVTK_NodeId_StreamedMatrix_Header
 *     OVTK_NodeId_StreamedMatrix_DimmensionCount (integer:2)
 *     OVTK_NodeId_StreamedMatrix_Dimmension
 *       OVTK_NodeId_StreamedMatrix_Dimmension_Size (integer:channel count)
 *       OVTK_NodeId_StreamedMatrix_Dimmension_Label (string:channel 1 name)
 *       OVTK_NodeId_StreamedMatrix_Dimmension_Label (string:channel 2 name)
 *       ...
 *     OVTK_NodeId_StreamedMatrix_Dimmension
 *       OVTK_NodeId_StreamedMatrix_Dimmension_Size (integer:number of samples per buffer)
 *       OVTK_NodeId_StreamedMatrix_Dimmension_Label (string)
 *       OVTK_NodeId_StreamedMatrix_Dimmension_Label (string)
 *       ...
 * OVTK_NodeId_Signal_Buffer
 *   OVTK_NodeId_StreamedMatrix_Buffer
 *     OVTK_NodeId_StreamedMatrix_RawBuffer (array of float64)
 * OVTK_NodeId_Signal_Buffer
 *   OVTK_NodeId_StreamedMatrix_Buffer
 *     OVTK_NodeId_StreamedMatrix_RawBuffer (array of float64)
 * ...
 * ----------------------------------------------------------------- *
 */

#define OVTK_NodeId_Signal_Header                                       EBML::CIdentifier(0x002B395F, 0x108ADFAE)
#define OVTK_NodeId_Signal_Buffer                                       EBML::CIdentifier(0x00CF2101, 0x02375310)

#define OVTK_NodeId_Signal_SamplingRate                                 EBML::CIdentifier(0x00141C43, 0x0C37006B)

//___________________________________________________________________//
//                                                                   //
// Stimulation stream node identifiers                               //
//___________________________________________________________________//
//                                                                   //

/*
 * Stimulation stream description (fixed on november 6th 2006)
 *
 * version 1 :
 * ----------------------------------------------------------------- *
 * OVTK_NodeId_Stimulation_Header
 *   OVTK_NodeId_StreamVersion (integer:1)
 * OVTK_NodeId_Stimulation_Buffer
 *   OVTK_NodeId_Stimulation_NumberOfStimulations (integer)
 *   OVTK_NodeId_Stimulation_Stimulation
 *     OVTK_NodeId_Stimulation_Stimulation_Identifier (integer)
 *     OVTK_NodeId_Stimulation_Stimulation_Date (integer)
 *   OVTK_NodeId_Stimulation_Stimulation
 *   ...
 * OVTK_NodeId_Stimulation_Buffer
 * OVTK_NodeId_Stimulation_Buffer
 * ...
 * ----------------------------------------------------------------- *
 */

#define OVTK_NodeId_Stimulation_Header                                  EBML::CIdentifier(0x00D9DDC3, 0x0B12873A)
#define OVTK_NodeId_Stimulation_Buffer                                  EBML::CIdentifier(0x006DEABE, 0x7FC05A20)

#define OVTK_NodeId_Stimulation_NumberOfStimulations                    EBML::CIdentifier(0x00BB790B, 0x2B8574D8)
#define OVTK_NodeId_Stimulation_Stimulation                             EBML::CIdentifier(0x0016EAC6, 0x29FBCAA1)
#define OVTK_NodeId_Stimulation_Stimulation_Identifier                  EBML::CIdentifier(0x006FA5DB, 0x4BAC31E9)
#define OVTK_NodeId_Stimulation_Stimulation_Date                        EBML::CIdentifier(0x00B866D8, 0x14DA5374)

//___________________________________________________________________//
//                                                                   //
// Experimentation information node identifiers                      //
//___________________________________________________________________//
//                                                                   //

/*
 * Experimentation information stream description (fixed on november 6th 2006)
 *
 * version 1 :
 * ----------------------------------------------------------------- *
 * OVTK_NodeId_ExperimentInformation_Header
 *   OVTK_NodeId_StreamVersion (integer:1)
 *   OVTK_NodeId_ExperimentInformation_Experiment
 *     OVTK_NodeId_ExperimentInformation_Experiment_Identifier (integer)
 *     OVTK_NodeId_ExperimentInformation_Experiment_Date (date)
 *   OVTK_NodeId_ExperimentInformation_Subject
 *     OVTK_NodeId_ExperimentInformation_Subject_Identifier (integer)
 *     OVTK_NodeId_ExperimentInformation_Subject_Name (string)
 *     OVTK_NodeId_ExperimentInformation_Subject_Age (integer)
 *     OVTK_NodeId_ExperimentInformation_Subject_Sex (integer)
 *   OVTK_NodeId_ExperimentInformation_Context
 *     OVTK_NodeId_ExperimentInformation_Context_LaboratoryIdentifier (integer)
 *     OVTK_NodeId_ExperimentInformation_Context_LaboratoryName (string)
 *     OVTK_NodeId_ExperimentInformation_Context_TechnicianIdentifier (integer)
 *     OVTK_NodeId_ExperimentInformation_Context_TechnicianName (string)
 * ----------------------------------------------------------------- *
 */

#define OVTK_NodeId_ExperimentInformation_Header                        EBML::CIdentifier(0x007855DE, 0x3748D375)
#define OVTK_NodeId_ExperimentInformation_Experiment                    EBML::CIdentifier(0x0011D6B7, 0x48F1AA39)
#define OVTK_NodeId_ExperimentInformation_Experiment_Identifier         EBML::CIdentifier(0x006ACD74, 0x1C960C26)
#define OVTK_NodeId_ExperimentInformation_Experiment_Date               EBML::CIdentifier(0x002F8FB7, 0x6DA7552D)
#define OVTK_NodeId_ExperimentInformation_Subject                       EBML::CIdentifier(0x003EC620, 0x333E0A94)
#define OVTK_NodeId_ExperimentInformation_Subject_Identifier            EBML::CIdentifier(0x00D62974, 0x473D4AA5)
#define OVTK_NodeId_ExperimentInformation_Subject_Name                  EBML::CIdentifier(0x0041FD0A, 0x6BCD9A99)
#define OVTK_NodeId_ExperimentInformation_Subject_Age                   EBML::CIdentifier(0x00DF7DD9, 0x33336C51)
#define OVTK_NodeId_ExperimentInformation_Subject_Sex                   EBML::CIdentifier(0x0069BB84, 0x3FC8E149)
#define OVTK_NodeId_ExperimentInformation_Context                       EBML::CIdentifier(0x0018C291, 0x7985DFDD)
#define OVTK_NodeId_ExperimentInformation_Context_LaboratoryIdentifier  EBML::CIdentifier(0x003F11B9, 0x26D76D9C)
#define OVTK_NodeId_ExperimentInformation_Context_LaboratoryName        EBML::CIdentifier(0x00EB1F23, 0x51C23B83)
#define OVTK_NodeId_ExperimentInformation_Context_TechnicianIdentifier  EBML::CIdentifier(0x00874A7F, 0x60DC34C2)
#define OVTK_NodeId_ExperimentInformation_Context_TechnicianName        EBML::CIdentifier(0x00C8C393, 0x31CE5B3E)

//___________________________________________________________________//
//                                                                   //
//                                                                   //
//___________________________________________________________________//
//                                                                   //

/*
#define OVTK_NodeId_                                                    EBML::CIdentifier(0x00746BA0, 0x115AE04D)
#define OVTK_NodeId_                                                    EBML::CIdentifier(0x00CDD0F7, 0x46B0278D)
#define OVTK_NodeId_                                                    EBML::CIdentifier(0x00CCFA4B, 0x14F37D4D)
#define OVTK_NodeId_                                                    EBML::CIdentifier(0x0010983C, 0x21F8BDE5)
#define OVTK_NodeId_                                                    EBML::CIdentifier(0x00AA5654, 0x2403A2CB)
#define OVTK_NodeId_                                                    EBML::CIdentifier(0x00A44C82, 0x05BE50D5)
#define OVTK_NodeId_                                                    EBML::CIdentifier(0x0027615F, 0x2243F7B5)
#define OVTK_NodeId_                                                    EBML::CIdentifier(0x00866CC6, 0x1EFE4BDC)
#define OVTK_NodeId_                                                    EBML::CIdentifier(0x00C91900, 0x55E50FF0)
#define OVTK_NodeId_                                                    EBML::CIdentifier(0x00E0E260, 0x646BF99E)
#define OVTK_NodeId_                                                    EBML::CIdentifier(0x00DCE72C, 0x386A40C5)
#define OVTK_NodeId_                                                    EBML::CIdentifier(0x00C520C6, 0x09AE98B5)
#define OVTK_NodeId_                                                    EBML::CIdentifier(0x00F1CBCB, 0x56BD6821)
*/
#define OVTK_ClassId_                                                   OpenViBE::CIdentifier(0x00C6D56F, 0x30890D27)

//___________________________________________________________________//
//                                                                   //
// Operating System identification                                   //
//___________________________________________________________________//
//                                                                   //

// #define OVTK_OS_Linux
// #define OVTK_OS_Windows
// #define OVTK_OS_MacOS
// #define OVTK_OS_

#if defined TARGET_OS_Windows
 #define OVTK_OS_Windows
#elif defined TARGET_OS_Linux
 #define OVTK_OS_Linux
#elif defined TARGET_OS_MacOS
 #define OVTK_OS_MacOS
#else
 #warning "No target operating system defined !"
#endif

//___________________________________________________________________//
//                                                                   //
// Hardware Architecture identification                              //
//___________________________________________________________________//
//                                                                   //

// #define OVTK_ARCHITECTURE_i386
// #define OVTK_ARCHITECTURE_

#if defined TARGET_ARCHITECTURE_i386
 #define OVTK_ARCHITECTURE_i386
#else
 #warning "No target architecture defined !"
#endif

//___________________________________________________________________//
//                                                                   //
// Compilator software identification                                //
//___________________________________________________________________//
//                                                                   //

// #define OVTK_COMPILATOR_GCC
// #define OVTK_COMPILATOR_VisualStudio
// #define OVTK_COMPILATOR_

#if defined TARGET_COMPILATOR_GCC
 #define OVTK_COMPILATOR_GCC
#elif defined TARGET_COMPILATOR_VisualStudio
 #define OVTK_COMPILATOR_VisualStudio
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
#if defined OVTK_Shared
 #if defined OVTK_OS_Windows
  #define OVTK_API_Export __declspec(dllexport)
  #define OVTK_API_Import __declspec(dllimport)
 #elif defined OVTK_OS_Linux
  #define OVTK_API_Export __attribute__((visibility("default")))
  #define OVTK_API_Import __attribute__((visibility("default")))
 #else
  #define OVTK_API_Export
  #define OVTK_API_Import
 #endif
#else
 #define OVTK_API_Export
 #define OVTK_API_Import
#endif

#if defined OVTK_Exports
 #define OVTK_API OVTK_API_Export
#else
 #define OVTK_API OVTK_API_Import
#endif

//___________________________________________________________________//
//                                                                   //
// NULL Definition                                                   //
//___________________________________________________________________//
//                                                                   //

#ifndef NULL
#define NULL 0
#endif

#endif // __OpenViBEToolkit_Defines_H__
