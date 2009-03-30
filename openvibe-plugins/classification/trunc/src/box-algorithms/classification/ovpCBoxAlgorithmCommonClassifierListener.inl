#ifndef __OpenViBEPlugins_BoxAlgorithm_CommonClassifierListener_INL__
#define __OpenViBEPlugins_BoxAlgorithm_CommonClassifierListener_INL__

#include "../../ovp_defines.h"
#include <openvibe/ov_all.h>
#include <openvibe-toolkit/ovtk_all.h>
#include <cstdio>

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

			// FOR MULTI INPUT CLASSIFIER TRAINER ONLY
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

			// FOR ADDITIONAL BOX SETTINGS
			virtual OpenViBE::boolean onInitialized(OpenViBE::Kernel::IBox& rBox)
			{
				return onAlgorithmClassIdentifierChanged(rBox);
			}
			virtual OpenViBE::boolean onAlgorithmClassIdentifierChanged(OpenViBE::Kernel::IBox& rBox)
			{
				OpenViBE::CString l_sClassifierName;
				OpenViBE::CIdentifier l_oClassifierType;
				OpenViBE::CIdentifier l_oClassifierIdentifier;

				rBox.getSettingValue(0, l_sClassifierName);
				rBox.getSettingType(0, l_oClassifierType);

				l_oClassifierIdentifier=this->getTypeManager().getEnumerationEntryValueFromName(l_oClassifierType, l_sClassifierName);
				if(l_oClassifierIdentifier != m_oClassifierClassIdentifier)
				{
					this->uninitialize();
					this->initialize();
				}
				return true;
			}
			virtual OpenViBE::boolean onSettingValueChanged(OpenViBE::Kernel::IBox& rBox, const OpenViBE::uint32 ui32Index)
			{
				if(ui32Index==0)
				{
					OpenViBE::CString l_sClassifierName;
					OpenViBE::CIdentifier l_oClassifierType;
					OpenViBE::CIdentifier l_oClassifierIdentifier;
					OpenViBE::CIdentifier l_oIdentifier;

					rBox.getSettingValue(0, l_sClassifierName);
					rBox.getSettingType(0, l_oClassifierType);

					l_oClassifierIdentifier=this->getTypeManager().getEnumerationEntryValueFromName(l_oClassifierType, l_sClassifierName);
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
							if(l_oIdentifier!=OVTK_Algorithm_ClassifierTrainer_InputParameterId_FeatureVectorSet)
							{
								OpenViBE::CString l_sParameterName=m_pClassifier->getInputParameterName(l_oIdentifier);
								OpenViBE::Kernel::IParameter* l_pParameter=m_pClassifier->getInputParameter(l_oIdentifier);
								OpenViBE::CIdentifier l_oSettingType;
								switch(l_pParameter->getType())
								{
									case OpenViBE::Kernel::ParameterType_Enumeration:
										rBox.addSetting(l_sParameterName, l_pParameter->getSubTypeIdentifier(), "");
										break;

									case OpenViBE::Kernel::ParameterType_Integer:
									case OpenViBE::Kernel::ParameterType_UInteger:
										rBox.addSetting(l_sParameterName, OV_TypeId_Integer, "");
										break;

									case OpenViBE::Kernel::ParameterType_Boolean:
										rBox.addSetting(l_sParameterName, OV_TypeId_Boolean, "");
										break;

									case OpenViBE::Kernel::ParameterType_Float:
										rBox.addSetting(l_sParameterName, OV_TypeId_Float, "");
										break;

									case OpenViBE::Kernel::ParameterType_String:
										rBox.addSetting(l_sParameterName, OV_TypeId_String, "");
										break;

									default:
										break;
								}
							}
						}
					}
				}
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
