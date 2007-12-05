#ifndef __OpenViBE_Plugins_IAlgorithm_H__
#define __OpenViBE_Plugins_IAlgorithm_H__

#include "ovIPluginObject.h"

namespace OpenViBE
{
	namespace Kernel
	{
		class IAlgorithmContext;
	}

	namespace Plugins
	{
		/**
		 * \class IAlgorithm
		 * \author Yann Renard (INRIA/IRISA)
		 * \date 2007-11-06
		 * \brief Abstract algorithm, base element of §OpenViBE§ processing objects
		 *
		 * This class should be derived by any plugin that is related
		 * to data processing. It basically has a parameterable interface
		 * contained in the IAlgorithmContext object. This interface
		 * stores several typed parameters which can be modified either by
		 * outside world in order to provide input paramters to this algorithm
		 * or by this algorithm itself in order to produce output parameters.
		 *
		 * This is the heart of the extension mechanism of the
		 * §OpenViBE§ platform.
		 *
		 * \sa OpenViBE::Kernel::IAlgorithmDesc
		 * \sa OpenViBE::Kernel::IAlgorithmContext
		 *
		 * \todo details about building new plugins
		 */
		class OV_API IAlgorithm : virtual public OpenViBE::Plugins::IPluginObject
		{
		public:

			/**
			 * \brief Initializes this algorithm
			 * \param rAlgorithmContext [in] : the execution context for this algorithm
			 * \return \e true in case of success.
			 * \return \e false in case of error.
			 *
			 * When called, this function should intializes its internal states and
			 * configure its own interface through the algorithm context. This done,
			 * the outside world will be able to connect "input" parameters to owned
			 * objects or other "output" parameters.
			 */
			virtual OpenViBE::boolean initialize(
				OpenViBE::Kernel::IAlgorithmContext& rAlgorithmContext) { return true; }
			/**
			 * \brief Unitializes this algorithm
			 * \param rAlgorithmContext [in] : the extecution context for this algorithm
			 * \return \e true in case of success.
			 * \return \e false in case of error.
			 *
			 * When called, this function should cleanup its own objects and interface.
			 */
			virtual OpenViBE::boolean uninitialize(
				OpenViBE::Kernel::IAlgorithmContext& rAlgorithmContext) { return true; }
			/**
			 * \brief Locks this algorithm so no more change can be done in term of structure to the interface
			 * \param rAlgorithmContext [in] : the extecution context for this algorithm
			 * \return \e true in case of success.
			 * \return \e false in case of error.
			 *
			 * When called, this function should check its "input" parameters and verify
			 * every thing is ok. If case \c process can be called, \e true should be returned.
			 * If \e false is returned, the \c process function won't be called.
			 */
			virtual OpenViBE::boolean lock(
				OpenViBE::Kernel::IAlgorithmContext& rAlgorithmContext) { return true; }
			/**
			 * \brief Unlocks this algorithm so changes can be done in term of structure to the interface
			 * \param rAlgorithmContext [in] : the extecution context for this algorithm
			 * \return \e true in case of success.
			 * \return \e false in case of error.
			 *
			 * When called, this algorithm is notified that the structure will be changed.
			 */
			virtual OpenViBE::boolean unlock(
				OpenViBE::Kernel::IAlgorithmContext& rAlgorithmContext) { return true; }
			/**
			 * \brief Effectively executes this algorithm
			 * \param rAlgorithmContext [in] : the extecution context for this algorithm
			 * \return \e true in case of success.
			 * \return \e false in case of error.
			 *
			 * When called, this function should get its "input" parameters, do stuffs with them
			 * and finally produce "ouput" parameters.
			 */
			virtual OpenViBE::boolean process(
				OpenViBE::Kernel::IAlgorithmContext& rAlgorithmContext)=0;

			_IsDerivedFromClass_(OpenViBE::Plugins::IPluginObject, OV_ClassId_Plugins_Algorithm)
		};
	};
};

#endif // __OpenViBE_Plugins_IAlgorithm_H__
