#ifndef __OpenViBEPlugins_VRPN_CVRPNAnalogServer_H__
#define __OpenViBEPlugins_VRPN_CVRPNAnalogServer_H__

#if defined TARGET_HAS_ThirdPartyVRPN

#include "ovp_defines.h"
#include <openvibe/ov_all.h>
#include <openvibe-toolkit/ovtk_all.h>

namespace OpenViBEPlugins
{
	namespace VRPN
	{
		class CVRPNAnalogServer
			:public OpenViBEToolkit::TBoxAlgorithm<OpenViBE::Plugins::IBoxAlgorithm>
			,public OpenViBEToolkit::IBoxAlgorithmStreamedMatrixInputReaderCallback::ICallback
		{
		public:

			CVRPNAnalogServer();
			virtual void release(void) { delete this; }
			virtual OpenViBE::boolean initialize(void);
			virtual OpenViBE::boolean uninitialize(void);
			virtual OpenViBE::boolean processInput(OpenViBE::uint32 ui32InputIndex);
			virtual OpenViBE::boolean process(void);

			_IsDerivedFromClass_Final_(OpenViBE::Plugins::IBoxAlgorithm, OVP_ClassId_VRPNAnalogServer)

			virtual void setMatrixDimmensionCount(const OpenViBE::uint32 ui32DimmensionCount);
			virtual void setMatrixDimmensionSize(const OpenViBE::uint32 ui32DimmensionIndex, const OpenViBE::uint32 ui32DimmensionSize);
			virtual void setMatrixDimmensionLabel(const OpenViBE::uint32 ui32DimmensionIndex, const OpenViBE::uint32 ui32DimmensionEntryIndex, const char* sDimmensionLabel);
			virtual void setMatrixBuffer(const OpenViBE::float64* pBuffer);

		protected:

			// Ebml
			EBML::IReader* m_pReader;
			OpenViBEToolkit::IBoxAlgorithmStreamedMatrixInputReaderCallback* m_pStreamedMatrixReaderCallBack;

			// Start and end time of the last buffer
			OpenViBE::uint64 m_ui64StartTime;
			OpenViBE::uint64 m_ui64EndTime;

			OpenViBE::CIdentifier m_oServerIdentifier;
			OpenViBE::uint32 m_ui32AnalogCount;
			OpenViBE::boolean m_bAnalogSet;
		};

		class CVRPNAnalogServerDesc : public OpenViBE::Plugins::IBoxAlgorithmDesc
		{
		public:

			virtual OpenViBE::CString getName(void) const                { return OpenViBE::CString("Analog VRPN server"); }
			virtual OpenViBE::CString getAuthorName(void) const          { return OpenViBE::CString("Bruno Renier/Yann Renard"); }
			virtual OpenViBE::CString getAuthorCompanyName(void) const   { return OpenViBE::CString("INRIA/IRISA"); }
			virtual OpenViBE::CString getShortDescription(void) const    { return OpenViBE::CString("Creates VRPN analog servers (one per input)."); }
			virtual OpenViBE::CString getDetailedDescription(void) const { return OpenViBE::CString("Creates VRPN analog servers to make data from the plugin's inputs available to VRPN client applications."); }
			virtual OpenViBE::CString getCategory(void) const            { return OpenViBE::CString("VRPN"); }
			virtual OpenViBE::CString getVersion(void) const             { return OpenViBE::CString("0.6"); }
			virtual void release(void)                                   { }
			virtual OpenViBE::CIdentifier getCreatedClass(void) const    { return OVP_ClassId_VRPNAnalogServer; }
			virtual OpenViBE::Plugins::IPluginObject* create(void)       { return new OpenViBEPlugins::VRPN::CVRPNAnalogServer(); }

			virtual OpenViBE::boolean getBoxPrototype(OpenViBE::Kernel::IBoxProto& rPrototype) const
			{
				rPrototype.addInput("Input", OV_TypeId_StreamedMatrix);

				rPrototype.addSetting("Peripheral name", OV_TypeId_String, "openvibe-vrpn");

				return true;
			}

			_IsDerivedFromClass_Final_(OpenViBE::Plugins::IBoxAlgorithmDesc, OVP_ClassId_VRPNAnalogServerDesc)
		};
	};
};

#endif // OVP_HAS_Vrpn

#endif // __OpenViBEPlugins_VRPN_CVRPNAnalogServer_H__
