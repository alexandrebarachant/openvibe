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
#include "kernel/ovIObjectFactory.h"
#include "kernel/ovIPluginModule.h"
#include "kernel/ovIPluginManager.h"
#include "kernel/ovIAttributable.h"
#include "kernel/ovIBox.h"
#include "kernel/ovIBoxIO.h"
#include "kernel/ovILink.h"
#include "kernel/ovIProcessingUnit.h"
#include "kernel/ovIScenario.h"
#include "kernel/ovIScenarioManager.h"
#include "kernel/ovITypeManager.h"
#include "kernel/ovILogManager.h"
#include "kernel/ovILogListener.h"

#include "kernel/ovIMessage.h"
#include "kernel/ovIMessageClock.h"
#include "kernel/ovIMessageEvent.h"
#include "kernel/ovIMessageSignal.h"

#include "kernel/ovIBoxProto.h"
#include "kernel/ovIPlayerContext.h"
#include "kernel/ovIBoxAlgorithmContext.h"
#include "kernel/ovIScenarioImporterContext.h"
#include "kernel/ovIScenarioExporterContext.h"


//___________________________________________________________________//
//                                                                   //
// Player includes                                                   //
//___________________________________________________________________//
//                                                                   //

#include "kernel/player/ovIPlayer.h"

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
