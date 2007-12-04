#ifndef __OpenViBE_All_H__
#define __OpenViBE_All_H__

//___________________________________________________________________//
//                                                                   //
// Basic includes                                                    //
//___________________________________________________________________//
//                                                                   //

#include "ov_types.h"
#include "ov_defines.h"

#include "ovCIdentifier.h"
#include "ovCString.h"

#include "ovIObject.h"
#include "ovIKernelLoader.h"

//___________________________________________________________________//
//                                                                   //
// Kernel includes                                                   //
//___________________________________________________________________//
//                                                                   //

#include "kernel/ovIKernel.h"
#include "kernel/ovIKernelContext.h"
#include "kernel/ovIKernelDesc.h"
#include "kernel/ovIKernelObject.h"
#include "kernel/ovIKernelObjectFactory.h"

#include "kernel/ovITypeManager.h"

#include "kernel/log/ovILogManager.h"
#include "kernel/log/ovILogListener.h"

#include "kernel/player/ovIBoxAlgorithmContext.h"
#include "kernel/player/ovIMessage.h"
#include "kernel/player/ovIMessageClock.h"
#include "kernel/player/ovIMessageEvent.h"
#include "kernel/player/ovIMessageSignal.h"
#include "kernel/player/ovIPlayer.h"
#include "kernel/player/ovIPlayerContext.h"
#include "kernel/player/ovIPlayerManager.h"

#include "kernel/plugins/ovIPluginManager.h"
#include "kernel/plugins/ovIPluginModule.h"
#include "kernel/plugins/ovIPluginModuleContext.h"

#include "kernel/scenario/ovIAttributable.h"
#include "kernel/scenario/ovIBox.h"
#include "kernel/scenario/ovIBoxIO.h"
#include "kernel/scenario/ovIBoxProto.h"
#include "kernel/scenario/ovILink.h"
#include "kernel/scenario/ovIProcessingUnit.h"
#include "kernel/scenario/ovIScenario.h"
#include "kernel/scenario/ovIScenarioExporterContext.h"
#include "kernel/scenario/ovIScenarioImporterContext.h"
#include "kernel/scenario/ovIScenarioManager.h"

#include "kernel/visualisation/ovIVisualisationContext.h"
#include "kernel/visualisation/ovIVisualisationTree.h"
#include "kernel/visualisation/ovIVisualisationManager.h"
#include "kernel/visualisation/ovIVisualisationWidget.h"

//___________________________________________________________________//
//                                                                   //
// Plugins includes                                                  //
//___________________________________________________________________//
//                                                                   //

#include "plugins/ovIPluginObject.h"
#include "plugins/ovIPluginObjectDesc.h"

#include "plugins/ovIBoxAlgorithm.h"
#include "plugins/ovIBoxAlgorithmDesc.h"

#include "plugins/ovIScenarioImporter.h"
#include "plugins/ovIScenarioImporterDesc.h"

#include "plugins/ovIScenarioExporter.h"
#include "plugins/ovIScenarioExporterDesc.h"

namespace OpenViBE
{
	namespace Plugins
	{
		// Backward compatibility
		typedef OpenViBE::Kernel::IBox IStaticBoxContext;
		typedef OpenViBE::Kernel::IBoxIO IDynamicBoxContext;
		typedef OpenViBE::Kernel::IBoxProto IBoxProto;
		typedef OpenViBE::Kernel::IBoxAlgorithmContext IBoxAlgorithmContext;
		typedef OpenViBE::Kernel::IScenarioImporterContext IScenarioImporterContext;
		typedef OpenViBE::Kernel::IScenarioExporterContext IScenarioExporterContext;
		typedef OpenViBE::Kernel::IPlayerContext IPlayerContext;
	};
};

#endif // __OpenViBE_All_H__
