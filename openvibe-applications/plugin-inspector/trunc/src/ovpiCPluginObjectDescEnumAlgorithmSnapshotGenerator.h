#ifndef __OpenViBEPluginInspector_CPluginObjectDescEnumAlgorithmSnapshotGenerator_H__
#define __OpenViBEPluginInspector_CPluginObjectDescEnumAlgorithmSnapshotGenerator_H__

#include "ovpiCPluginObjectDescEnum.h"

#include <map>
#include <vector>
#include <string>

#include <gtk/gtk.h>

// ------------------------------------------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------------------------------------------

class CPluginObjectDescEnumAlgorithmSnapshotGenerator : public CPluginObjectDescEnum
{
public:

	CPluginObjectDescEnumAlgorithmSnapshotGenerator(const OpenViBE::Kernel::IKernelContext& rKernelContext);
	virtual ~CPluginObjectDescEnumAlgorithmSnapshotGenerator(void);
	virtual OpenViBE::boolean callback(const OpenViBE::Plugins::IPluginObjectDesc& rPluginObjectDesc);

protected:

	std::vector < std::pair < std::string, std::string > > m_vCategories;
	std::map<OpenViBE::uint32, ::GdkColor> m_vColors;
	::GtkWidget* m_pWindow;
	::GtkWidget* m_pWidget;
};

#endif // __OpenViBEPluginInspector_CPluginObjectDescEnumAlgorithmSnapshotGenerator_H__
