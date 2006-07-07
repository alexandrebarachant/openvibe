#ifndef __OV_DOCNAMESPACES_H__
#define __OV_DOCNAMESPACES_H__

/**
 * \namespace OpenViBE
 * \author Yann Renard (INRIA/IRISA)
 * \date 2006-06
 * \brief Main OpenViBE namespace
 *
 * All the classes defined in the §OpenViBE§ platform are included in
 * this namespace, including kernel and plugins.
 */
namespace OpenViBE
{
	/**
     * \namespace OpenViBE::Kernel
	 * \author Yann Renard (INRIA/IRISA)
	 * \date 2006-06
	 * \brief Kernel specifications of the §OpenViBE§ platform
	 *
	 * This OpenViBE subnamespace contains the specifications of
	 * the kernel classes. Those classes are overided by the kernel
	 * developper to build a complete kernel.
	 */
	namespace Kernel
	{
	};

	/**
	 * \namespace OpenViBE::Plugins
	 * \author Yann Renard (INRIA/IRISA)
	 * \date 2006-06
	 * \brief Plugins specifications of the §OpenViBE§ platform
	 *
	 * This OpenViBE subnamespace contains the specifications of
	 * the plugin classes. Those classes should be used by the
	 * kernel in order to extend the capabilities of the platform.
	 * Some of the classes are overloaded by the plugin developper,
	 * some are overloaded by the kernel developper in order to let
	 * the plugin objects have the tools they need.
	 */
	namespace Plugins
	{
	};
};

#endif // __OV_DOCNAMESPACES_H__
