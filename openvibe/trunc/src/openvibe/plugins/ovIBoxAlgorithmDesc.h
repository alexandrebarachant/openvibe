#ifndef __OpenViBE_Plugins_IBoxAlgorithmDesc_H__
#define __OpenViBE_Plugins_IBoxAlgorithmDesc_H__

#include "ovIPluginObjectDesc.h"

namespace OpenViBE
{
	namespace Plugins
	{
		class IBoxProto;

		/**
		 * \class IBoxAlgorithmDesc
		 * \author Yann Renard (INRIA/IRISA)
		 * \date 2006-06-19
		 * \brief Box algorithm prototype
		 *
		 * This class should be derived by any plugin developper in
		 * order to describe a specific §OpenViBE§ box algorithm.
		 *
		 * \sa OpenViBE::Plugins::IBoxAlgorithm
		 *
		 * \todo details about building new plugins
		 */
		class OV_API IBoxAlgorithmDesc : virtual public OpenViBE::Plugins::IPluginObjectDesc
		{
		public:
		
			/** \name Box prototype */
			//@{

			/**
			 * \brief Gets the box prototype for this algorithm
			 * \param rPrototype [out] : the box prototype to fill
			 * \return \e true in case of success \e false in other cases.
			 *
			 * When this function is called by the §OpenViBE§
			 * platform, the plugin descriptor should fill in
			 * the structure to let the §OpenViBE§ platform know
			 * whet the corresponding box should look like
			 * (inputs/outputs/settings).
			 *
			 * \sa OpenViBE::Plugins::IBoxProto
			 */
			virtual OpenViBE::boolean getBoxPrototype(
				OpenViBE::Plugins::IBoxProto& rPrototype) const=0;

			//@}
			/** \name Behavioral configuration */
			//@{

			/**
			 * \brief Gets the clock frequency to call this algorithm
			 * \return The clock frequency to call this algorithm
			 *
			 * This function is used for algorithms that are triggered on
			 * clock signals. The returned frequency should be the
			 * frequency itself multiplied by a factor 1 000. Thus
			 * returning 1 000 means the algorithm would be clock
			 * activated each second. Returning 25 000 would clock
			 * activate the block each 40ms etc...
			 *
			 * \note Returning 0 means the algorithm should not be
			 *       clock activated.
			 */
			virtual OpenViBE::uint32 getClockFrequency(void) const=0;

			//@}

			_IsDerivedFromClass_(OpenViBE::Plugins::IPluginObjectDesc, OV_ClassId_Plugins_BoxAlgorithmDesc)
		};
	};
};

#endif // __OpenViBE_Plugins_IBoxAlgorithmDesc_H__
