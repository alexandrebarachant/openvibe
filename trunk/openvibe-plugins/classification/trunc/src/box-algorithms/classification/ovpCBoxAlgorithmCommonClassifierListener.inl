#ifndef __OpenViBEPlugins_BoxAlgorithm_CommonClassifierListener_INL__
#define __OpenViBEPlugins_BoxAlgorithm_CommonClassifierListener_INL__

#include "../../ovp_defines.h"
#include <openvibe/ov_all.h>
#include <openvibe-toolkit/ovtk_all.h>
#include <cstdio>
#include <cstring>
#include <cstdlib>

namespace OpenViBEPlugins
{
	namespace Classification
	{
		class CBoxAlgorithmCommonClassifierListener : public OpenViBEToolkit::TBoxListener < OpenViBE::Plugins::IBoxListener >
		{
		public:

			CBoxAlgorithmCommonClassifierListener(const OpenViBE::uint32 ui32CustomSettingBase)
				:m_ui32CustomSettingBase(ui32CustomSettingBase)
			{
			}

			virtual OpenViBE::boolean initialize(void)
			{
				m_oClassifierClassIdentifier=OV_UndefinedIdentifier;
				m_pClassifier=NULL;
				return true;
			}
			virtual OpenViBE::boolean uninitialize(void)
			{
				if(m_pClassifier)
				{
					m_pClassifier->uninitialize();
					this->getAlgorithmManager().releaseAlgorithm(*m_pClassifier);
				}
				return true;
			}

			virtual OpenViBE::boolean onInputAddedOrRemoved(OpenViBE::Kernel::IBox& rBox)
			{
				while(rBox.getInputCount()<m_ui32CustomSettingBase)
				{
					rBox.addInput("", OV_UndefinedIdentifier);
				}

				rBox.setInputType(0, OV_TypeId_Stimulations);
				rBox.setInputName(0, "Stimulations");
				for(OpenViBE::uint32 i=1; i<rBox.getInputCount(); i++)
				{
					char l_sBuffer[1024];
					sprintf(l_sBuffer, "Features for class %i", i);
					rBox.setInputName(i, l_sBuffer);
					rBox.setInputType(i, OV_TypeId_FeatureVector);
				}
				return true;
			}

			virtual OpenViBE::boolean onInputAdded(OpenViBE::Kernel::IBox& rBox, const OpenViBE::uint32 ui32Index)
			{
				return this->onInputAddedOrRemoved(rBox);
			}

			virtual OpenViBE::boolean onInputRemoved(OpenViBE::Kernel::IBox& rBox, const OpenViBE::uint32 ui32Index)
			{
				return this->onInputAddedOrRemoved(rBox);
			}

			virtual OpenViBE::boolean onInitialized(OpenViBE::Kernel::IBox& rBox)
			{
				return this->onAlgorithmClassIdentifierChanged(rBox);
			}
			virtual OpenViBE::boolean onSettingValueChanged(OpenViBE::Kernel::IBox& rBox, const OpenViBE::uint32 ui32Index)
			{
				return ui32Index==0?this->onAlgorithmClassIdentifierChanged(rBox):true;
			}

			virtual OpenViBE::boolean onAlgorithmClassIdentifierChanged(OpenViBE::Kernel::IBox& rBox)
			{
					OpenViBE::CString l_sClassifierName;
					OpenViBE::CIdentifier l_oClassifierIdentifier;
					OpenViBE::CIdentifier l_oIdentifier;

					rBox.getSettingValue(0, l_sClassifierName);

					l_oClassifierIdentifier=this->getTypeManager().getEnumerationEntryValueFromName(OVTK_TypeId_ClassificationAlgorithm, l_sClassifierName);
					if(l_oClassifierIdentifier != m_oClassifierClassIdentifier)
					{
						this->uninitialize();
						this->initialize();

						while(rBox.getSettingCount()>m_ui32CustomSettingBase)
						{
							rBox.removeSetting(m_ui32CustomSettingBase);
						}

						m_oClassifierClassIdentifier=l_oClassifierIdentifier;
						m_pClassifier=&this->getAlgorithmManager().getAlgorithm(this->getAlgorithmManager().createAlgorithm(m_oClassifierClassIdentifier));
						m_pClassifier->initialize();

						while((l_oIdentifier=m_pClassifier->getNextInputParameterIdentifier(l_oIdentifier))!=OV_UndefinedIdentifier)
						{
							if((l_oIdentifier!=OVTK_Algorithm_Classifier_InputParameterId_FeatureVector)
							&& (l_oIdentifier!=OVTK_Algorithm_Classifier_InputParameterId_FeatureVectorSet)
							&& (l_oIdentifier!=OVTK_Algorithm_Classifier_InputParameterId_Configuration))
							{
								OpenViBE::CString l_sParameterName=m_pClassifier->getInputParameterName(l_oIdentifier);
								OpenViBE::Kernel::IParameter* l_pParameter=m_pClassifier->getInputParameter(l_oIdentifier);
								OpenViBE::Kernel::TParameterHandler < OpenViBE::int64 > ip_i64Parameter(l_pParameter);
								OpenViBE::Kernel::TParameterHandler < OpenViBE::uint64 > ip_ui64Parameter(l_pParameter);
								OpenViBE::Kernel::TParameterHandler < OpenViBE::float64 > ip_f64Parameter(l_pParameter);
								OpenViBE::Kernel::TParameterHandler < OpenViBE::boolean > ip_bParameter(l_pParameter);
								char l_sBuffer[1024];
								switch(l_pParameter->getType())
								{
									case OpenViBE::Kernel::ParameterType_Enumeration:
										strcpy(l_sBuffer, this->getTypeManager().getEnumerationEntryNameFromValue(l_pParameter->getSubTypeIdentifier(), ip_ui64Parameter).toASCIIString());
										rBox.addSetting(l_sParameterName, l_pParameter->getSubTypeIdentifier(), l_sBuffer);
										break;

									case OpenViBE::Kernel::ParameterType_Integer:
									case OpenViBE::Kernel::ParameterType_UInteger:
										sprintf(l_sBuffer, "%lli", (OpenViBE::int64)ip_i64Parameter);
										rBox.addSetting(l_sParameterName, OV_TypeId_Integer, l_sBuffer);
										break;

									case OpenViBE::Kernel::ParameterType_Boolean:
										sprintf(l_sBuffer, "%s", ((OpenViBE::boolean)ip_bParameter)?"true":"false");
										rBox.addSetting(l_sParameterName, OV_TypeId_Boolean, l_sBuffer);
										break;

									case OpenViBE::Kernel::ParameterType_Float:
										sprintf(l_sBuffer, "%lf", (OpenViBE::float64)ip_f64Parameter);
										rBox.addSetting(l_sParameterName, OV_TypeId_Float, l_sBuffer);
										break;

									default:
										break;
								}
							}
						}
					}
/*
				OpenViBE::CString l_sClassifierName;
				OpenViBE::CIdentifier l_oClassifierIdentifier;

				rBox.getSettingValue(0, l_sClassifierName);

				l_oClassifierIdentifier=this->getTypeManager().getEnumerationEntryValueFromName(OVTK_TypeId_ClassificationAlgorithm, l_sClassifierName);
				if(l_oClassifierIdentifier != m_oClassifierClassIdentifier)
				{
					this->uninitialize();
					this->initialize();
				}
*/
				return true;
			}


			_IsDerivedFromClass_Final_(OpenViBEToolkit::TBoxListener < OpenViBE::Plugins::IBoxListener >, OV_UndefinedIdentifier);

		protected:

			OpenViBE::CIdentifier m_oClassifierClassIdentifier;
			OpenViBE::Kernel::IAlgorithmProxy* m_pClassifier;
			const OpenViBE::uint32 m_ui32CustomSettingBase;
		};
	};
};

#endif // __OpenViBEPlugins_BoxAlgorithm_CommonClassifierListener_INL__
