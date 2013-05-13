#ifndef __OpenViBEPluginInspector_CPluginObjectDescEnumAlgorithmGlobalDefinesGenerator_H__
#define __OpenViBEPluginInspector_CPluginObjectDescEnumAlgorithmGlobalDefinesGenerator_H__

#include "ovpiCPluginObjectDescEnum.h"

#include <fstream>
#include <map>

class CPluginObjectDescEnumAlgorithmGlobalDefinesGenerator : public CPluginObjectDescEnum
{
public:

	CPluginObjectDescEnumAlgorithmGlobalDefinesGenerator(const OpenViBE::Kernel::IKernelContext& rKernelContext, const OpenViBE::CString& sFilename);
	virtual ~CPluginObjectDescEnumAlgorithmGlobalDefinesGenerator(void);
	virtual OpenViBE::boolean callback(const OpenViBE::Plugins::IPluginObjectDesc& rPluginObjectDesc);

protected:

	std::ofstream m_oFile;

	std::map<OpenViBE::CIdentifier, OpenViBE::CString> m_oUsedIdentifiers;
	// Adds the define to m_usedIdentifiers and m_oFile
	void addIdentifier(const OpenViBE::CString &objectName, const OpenViBE::CIdentifier candidate, const OpenViBE::CString &sSpaces);

};

#endif // __OpenViBEPluginInspector_CPluginObjectDescEnumAlgorithmGlobalDefinesGenerator_H__
