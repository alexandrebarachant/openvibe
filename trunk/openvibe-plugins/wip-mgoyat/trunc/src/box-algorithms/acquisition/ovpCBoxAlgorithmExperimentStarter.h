#ifndef __OpenViBEPlugins_BoxAlgorithm_ExperimentStarter_H__
#define __OpenViBEPlugins_BoxAlgorithm_ExperimentStarter_H__

#include "../../ovp_defines.h"
#include <openvibe/ov_all.h>
#include <openvibe-toolkit/ovtk_all.h>
#include <iostream>
#include <fstream>

#if defined OVP_OS_Windows

#include <socket/IConnectionServer.h>
#include <time.h>
#include <windows.h>

#define OVP_ClassId_BoxAlgorithm_ExperimentStarter     OpenViBE::CIdentifier(0x69B21499, 0x197C6EFA)
#define OVP_ClassId_BoxAlgorithm_ExperimentStarterDesc OpenViBE::CIdentifier(0x233D04B0, 0x50561D56)

namespace OpenViBEPlugins
{
	namespace Acquisition
	{
		class CBoxAlgorithmExperimentStarter : public OpenViBEToolkit::TBoxAlgorithm < OpenViBE::Plugins::IBoxAlgorithm >
		{
		public:

			virtual void release(void) { delete this; }

			virtual OpenViBE::uint64 getClockFrequency();
			virtual OpenViBE::boolean processClock(OpenViBE::CMessageClock &rMessageClock);

			virtual OpenViBE::boolean initialize(void);
			virtual OpenViBE::boolean uninitialize(void);
			virtual OpenViBE::boolean processInput(OpenViBE::uint32 ui32Index);
			virtual OpenViBE::boolean process(void);

			_IsDerivedFromClass_Final_(OpenViBEToolkit::TBoxAlgorithm < OpenViBE::Plugins::IBoxAlgorithm >, OVP_ClassId_BoxAlgorithm_ExperimentStarter);

		protected:

			OpenViBE::Kernel::IAlgorithmProxy* m_pStimulationEncoder;
			OpenViBE::Kernel::TParameterHandler < OpenViBE::IStimulationSet* > ip_pStimulationSet;

			Socket::IConnectionServer* m_pConnectionServer;
			Socket::IConnection* m_pConnectionClient;

		private:

			clock_t mc_StartClock;
			SYSTEMTIME mo_st;
			SYSTEMTIME mo_st_Current;
			OpenViBE::uint64 timeStartConnection;
			OpenViBE::boolean addTimeSTsecond(SYSTEMTIME& most, OpenViBE::uint32 ts);
			OpenViBE::boolean TimeSTover(SYSTEMTIME& mostref, SYSTEMTIME& most);

			OpenViBE::boolean HeaderToSend;
			OpenViBE::boolean SomethingToSend;
			OpenViBE::boolean SomethingToSendInOV;

			std::fstream m_fileSequence;

			};

		class CBoxAlgorithmExperimentStarterDesc : public OpenViBE::Plugins::IBoxAlgorithmDesc
		{
		public:

			virtual void release(void) { }

			virtual OpenViBE::CString getName(void) const                { return OpenViBE::CString("Experiment Starter"); }
			virtual OpenViBE::CString getAuthorName(void) const          { return OpenViBE::CString("Matthieu GOYAT"); }
			virtual OpenViBE::CString getAuthorCompanyName(void) const   { return OpenViBE::CString("Gipsa-lab"); }
			virtual OpenViBE::CString getShortDescription(void) const    { return OpenViBE::CString(""); }
			virtual OpenViBE::CString getDetailedDescription(void) const { return OpenViBE::CString(""); }
			virtual OpenViBE::CString getCategory(void) const            { return OpenViBE::CString("Acquisition"); }
			virtual OpenViBE::CString getVersion(void) const             { return OpenViBE::CString("1.0"); }
			//virtual OpenViBE::CString getStockItemName(void) const       { return OpenViBE::CString("gtk-select-font"); }

			virtual OpenViBE::CIdentifier getCreatedClass(void) const    { return OVP_ClassId_BoxAlgorithm_ExperimentStarter; }
			virtual OpenViBE::Plugins::IPluginObject* create(void)       { return new OpenViBEPlugins::Acquisition::CBoxAlgorithmExperimentStarter; }

			virtual OpenViBE::boolean getBoxPrototype(
				OpenViBE::Kernel::IBoxProto& rBoxAlgorithmPrototype) const
			{
				rBoxAlgorithmPrototype.addOutput ("Time stimulations",           OV_TypeId_Stimulations);
				//
				rBoxAlgorithmPrototype.addSetting("Time list",      OV_TypeId_Filename,       "../");
				rBoxAlgorithmPrototype.addSetting("server port",    OV_TypeId_Integer,		  "700");
				return true;
			}

			_IsDerivedFromClass_Final_(OpenViBE::Plugins::IBoxAlgorithmDesc, OVP_ClassId_BoxAlgorithm_ExperimentStarterDesc);
		};
	};
};

#endif

#endif // __OpenViBEPlugins_BoxAlgorithm_ExperimentStarter_H__
