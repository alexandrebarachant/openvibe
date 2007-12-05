#ifndef __OpenViBE_Plugins_IAlgorithmDesc_H__
#define __OpenViBE_Plugins_IAlgorithmDesc_H__

#include "ovIPluginObjectDesc.h"

namespace OpenViBE
{
	namespace Plugins
	{
		class OV_API IAlgorithmDesc : virtual public OpenViBE::Plugins::IPluginObjectDesc
		{
		public:

			/**
			 * \brief Gets the prototype for this algorithm
			 * \param rPrototype [out] : the prototype to fill
			 * \return \e true in case of success \e false in other cases.
			 *
			 * When this function is called by the §OpenViBE§
			 * platform, the plugin descriptor should fill in
			 * the structure to let the §OpenViBE§ platform know
			 * what the algorithm should look like
			 * (inputs/outputs/triggers).
			 *
			 * \sa OpenViBE::Plugins::IBoxProto
			 */
			virtual OpenViBE::boolean getAlgorithmPrototype(
				OpenViBE::Kernel::IAlgorithmProto& rAlgorithmPrototype) const=0;

			_IsDerivedFromClass_(OpenViBE::Plugins::IPluginObjectDesc, OV_ClassId_Plugins_AlgorithmDesc)
		};
	};
};

#endif // __OpenViBE_Plugins_IAlgorithmDesc_H__
