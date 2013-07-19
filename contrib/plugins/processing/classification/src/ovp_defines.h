
#ifndef __OpenViBEPlugins_Defines_H__
#define __OpenViBEPlugins_Defines_H__

#define OVP_ClassId_Algorithm_FeatureExtractionLda                                      OpenViBE::CIdentifier(0xD1AA6C72, 0x0432E1C5)
#define OVP_ClassId_Algorithm_FeatureExtractionLdaDesc                                  OpenViBE::CIdentifier(0x2109D46A, 0x9A45E1DC)
#define OVP_Algorithm_FeatureExtractionLda_InputParameterId_EpochTable                  OpenViBE::CIdentifier(0xA7BE9AE6, 0xD09FAD58)
#define OVP_Algorithm_FeatureExtractionLda_InputParameterId_Class1SamplesNumber         OpenViBE::CIdentifier(0x7F99FBC6, 0x748B183B)
#define OVP_Algorithm_FeatureExtractionLda_OutputParameterId_MatrixFirstClass           OpenViBE::CIdentifier(0x76F84603, 0x1F5D7A10)
#define OVP_Algorithm_FeatureExtractionLda_OutputParameterId_MatrixSecondClass          OpenViBE::CIdentifier(0x0AF9008A, 0xE7A7B2B7)
#define OVP_Algorithm_FeatureExtractionLda_OutputParameterId_MatrixEpoch                OpenViBE::CIdentifier(0xD4B473F6, 0xD5626A12)
#define OVP_Algorithm_FeatureExtractionLda_InputTriggerId_Initialize                    OpenViBE::CIdentifier(0x06176684, 0x5FA2C7C7)
#define OVP_Algorithm_FeatureExtractionLda_InputTriggerId_ExtractFeature                OpenViBE::CIdentifier(0x3B505F2F, 0x024B2C52)

#define OVP_ClassId_Box_LDABoxAlgorithm                                                 OpenViBE::CIdentifier(0xF37C8DBC, 0x18FEA50E)
#define OVP_ClassId_Box_LDABoxAlgorithmDesc                                             OpenViBE::CIdentifier(0x89E0A991, 0x4E0680D5)
//#define OVP_Algorithm_LDABoxAlgorithm_InputParameterId_SignalMatrix                     OpenViBE::CIdentifier(0xEC68E737, 0x59E1B68B)

#define OVP_ClassId_Algorithm_ComputeFisherLdaFunction                                  OpenViBE::CIdentifier(0x124AC7D5, 0x39B975E2)
#define OVP_ClassId_Algorithm_ComputeFisherLdaFunctionDesc                              OpenViBE::CIdentifier(0x0ECE9FF0, 0xBA40F306)
#define OVP_Algorithm_ComputeFisherLdaFunction_InputParameterId_MatrixFirstClass         OpenViBE::CIdentifier(0x639846F9, 0x6637445E)
#define OVP_Algorithm_ComputeFisherLdaFunction_InputParameterId_MatrixSecondClass        OpenViBE::CIdentifier(0xE438158E, 0xB31603F2)
#define OVP_Algorithm_ComputeFisherLdaFunction_OutputParameterId_MatrixGlobalMean        OpenViBE::CIdentifier(0x8278CF64, 0x25804B86)
#define OVP_Algorithm_ComputeFisherLdaFunction_OutputParameterId_MatrixGlobalCovariance  OpenViBE::CIdentifier(0x6027E2D7, 0x878B5931)
#define OVP_Algorithm_ComputeFisherLdaFunction_OutputParameterId_MatrixGlobalProbability OpenViBE::CIdentifier(0xB568892E, 0x81020300)
#define OVP_Algorithm_ComputeFisherLdaFunction_InputTriggerId_Initialize                 OpenViBE::CIdentifier(0x0C320928, 0x6B2C5285)
#define OVP_Algorithm_ComputeFisherLdaFunction_InputTriggerId_ComputeFunction            OpenViBE::CIdentifier(0xCCD22C15, 0xB49F6AAD)

#define OVP_ClassId_Algorithm_ApplyFisherLdaFunction                                    OpenViBE::CIdentifier(0xFCA53C1A, 0xC4024D00)
#define OVP_ClassId_Algorithm_ApplyFisherLdaFunctionDesc                                OpenViBE::CIdentifier(0xF4D7A3FE, 0x52D8F1EB)
#define OVP_Algorithm_ApplyFisherLdaFunction_InputParameterId_MatrixSignal              OpenViBE::CIdentifier(0x505BB8BC, 0xDFFB05D8)
#define OVP_Algorithm_ApplyFisherLdaFunction_InputParameterId_MatrixGlobalMean          OpenViBE::CIdentifier(0xFD147AF9, 0xC8FEBF3C)
#define OVP_Algorithm_ApplyFisherLdaFunction_InputParameterId_MatrixGlobalCovariance    OpenViBE::CIdentifier(0xF07B6450, 0x8C00B24B)
#define OVP_Algorithm_ApplyFisherLdaFunction_InputParameterId_MatrixGlobalProbability   OpenViBE::CIdentifier(0xE791F510, 0x6BD004E7)
#define OVP_Algorithm_ApplyFisherLdaFunction_OutputParameterId_FisherLdaDecision        OpenViBE::CIdentifier(0xE347EE5A, 0xA74645F1)
#define OVP_Algorithm_ApplyFisherLdaFunction_InputTriggerId_Initialize                  OpenViBE::CIdentifier(0xC84D2FF2, 0x66228A70)
#define OVP_Algorithm_ApplyFisherLdaFunction_InputTriggerId_ApplyFunction               OpenViBE::CIdentifier(0xA07276DB, 0x9E775318)

#define OVP_ClassId_Box_NaiveBayesApplyBoxAlgorithm                                                 OpenViBE::CIdentifier(0xCCE94653, 0x690907D9)
#define OVP_ClassId_Box_NaiveBayesApplyBoxAlgorithmDesc                                             OpenViBE::CIdentifier(0x7C0C9AFC, 0xC806CC84)
#define OVP_ClassId_Algorithm_NaiveBayesApplyFunction                                	OpenViBE::CIdentifier(0xF9C21D2C, 0x9E6ABE8E)
#define OVP_ClassId_Algorithm_NaiveBayesApplyDesc                                  	OpenViBE::CIdentifier(0xCDE5EFEB, 0x0689C756)
#define OVP_Algorithm_NaiveBayesApplyFunction_InputParameterId_MatrixSignal		OpenViBE::CIdentifier(0xF1D46621, 0x9CAE8931)
#define OVP_Algorithm_NaiveBayesApplyFunction_InputParameterId_MatrixMean		OpenViBE::CIdentifier(0xDD1479AC, 0xB2E13471)
#define OVP_Algorithm_NaiveBayesApplyFunction_InputParameterId_MatrixVariance		OpenViBE::CIdentifier(0xEB3CD401, 0x54E4692D)
#define OVP_Algorithm_NaiveBayesApplyFunction_InputParameterId_LogTerm			OpenViBE::CIdentifier(0xB10D8B3C, 0x2A1D3705)
#define OVP_Algorithm_NaiveBayesApplyFunction_OutputParameterId_MatrixClassFunctional	OpenViBE::CIdentifier(0x57A78D33, 0x724CE7F9)
#define OVP_Algorithm_NaiveBayesApplyFunction_InputTriggerId_Initialize			OpenViBE::CIdentifier(0x0B3F7D98, 0x70D9D6E6)
#define OVP_Algorithm_NaiveBayesApplyFunction_InputTriggerId_Apply			OpenViBE::CIdentifier(0x622D05B7, 0x002F601F)
// @END inserm-gpl

//___________________________________________________________________//
//                                                                   //
// Global defines                                                   //
//___________________________________________________________________//
//                                                                   //

#ifdef TARGET_HAS_ThirdPartyOpenViBEPluginsGlobalDefines
 #include "ovp_global_defines.h"
#endif // TARGET_HAS_ThirdPartyOpenViBEPluginsGlobalDefines


#endif // __OpenViBEPlugins_Defines_H__
