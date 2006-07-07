#ifndef __OpenViBE_Plugins_IBoxAlgorithm_H__
#define __OpenViBE_Plugins_IBoxAlgorithm_H__

#include "ovIPluginObject.h"

namespace OpenViBE
{
	namespace Plugins
	{
		class IBoxAlgorithmContext;

		/**
		 * \class IBoxAlgorithm
		 * \author Yann Renard (INRIA/IRISA)
		 * \date 2006-06-19
		 * \brief Algorithm to create/process/transform §OpenViBE§ data
		 *
		 * This class should be derived by any plugin that is related
		 * to data processing. It can be data acquisition/production
		 * from an hardware device or from a file. It can be data
		 * processing/transforming, moving time information into
		 * frequency space for example. It can be data classification
		 * generating discrete classification events better than
		 * continuous data flow.
		 *
		 * This is the heart of the extension mechanism of the
		 * §OpenViBE§ platform.
		 *
		 * \sa OpenViBE::Plugins::IBoxAlgorithmDesc
		 *
		 * \todo details about building new plugins
		 */
		class OV_API IBoxAlgorithm : virtual public OpenViBE::Plugins::IPluginObject
		{
		public:

			/** \name Processing */
			//@{

			/**
			 * \brief Processing function
			 * \param rBoxAlgorithmContext [in] : the box algorithm context to use
			 * \return \e true on success, \e false when something went wrong.
			 *
			 * This function is used to process the arrived data and
			 * eventually generate results. It is called depending
			 * on the associated box behavior. See §OpenViBE§ global
			 * architecture to understand how the commponents interact
			 * and how an §OpenViBE§ box works internally.
			 *
			 * The processing function may use the provided context
			 * in order to read its inputs and write its outputs...
			 * Also it could use the provided context to send 
			 * messages/events to other boxes. Finally, it may use
			 * the provided context in order to perform rendering
			 * tasks !
			 *
			 * \sa OpenViBE::Kernel::IBoxAlgorithmContext
			 */
			virtual OpenViBE::boolean process(OpenViBE::Plugins::IBoxAlgorithmContext& rBoxAlgorithmContext)=0;

			//@}

			_IsDerivedFromClass_(OpenViBE::Plugins::IBoxAlgorithm, OV_ClassIdentifier_BoxAlgorithm)
		};
	};
};

#endif // __OpenViBE_Plugins_IBoxAlgorithm_H__
