#include "ovkCTypeManager.h"

using namespace OpenViBE;
using namespace OpenViBE::Kernel;
using namespace std;

CTypeManager::CTypeManager(const IKernelContext& rKernelContext)
	:TKernelObject<ITypeManager>(rKernelContext)
{
	m_vName[OV_UndefinedIdentifier]="undefined";

	registerType(OV_TypeId_Boolean, "boolean");
	registerType(OV_TypeId_Integer, "integer");
	registerType(OV_TypeId_Float, "float");
	registerType(OV_TypeId_String, "string");

	registerStreamType(OV_TypeId_EBMLStream, "EBML stream", OV_UndefinedIdentifier);
	registerStreamType(  OV_TypeId_ChannelLocalisation, "channel localisation", OV_TypeId_EBMLStream);
	registerStreamType(  OV_TypeId_ExperimentationInformation, "experiment information", OV_TypeId_EBMLStream);
	registerStreamType(  OV_TypeId_Stimulations, "stimulations", OV_TypeId_EBMLStream);
	registerStreamType(  OV_TypeId_StreamedMatrix, "streamed matrix", OV_TypeId_EBMLStream);
	registerStreamType(    OV_TypeId_FeatureVector, "feature vector", OV_TypeId_StreamedMatrix);
	registerStreamType(    OV_TypeId_Signal, "signal", OV_TypeId_StreamedMatrix);
	registerStreamType(    OV_TypeId_Spectrum, "spectrum", OV_TypeId_StreamedMatrix);
}

boolean CTypeManager::registerType(
	const CIdentifier& rTypeIdentifier,
	const CString& sTypeName)
{
	if(isRegistered(rTypeIdentifier))
	{
		return false;
	}
	m_vName[rTypeIdentifier]=sTypeName;
	log() << LogLevel_Trace << "Registered type id " << rTypeIdentifier << " - " << sTypeName << "\n";
	return true;
}

boolean CTypeManager::registerStreamType(
	const CIdentifier& rTypeIdentifier,
	const CString& sTypeName,
	const CIdentifier& rParentTypeIdentifier)
{
	if(isRegistered(rTypeIdentifier))
	{
		return false;
	}
	if(rParentTypeIdentifier!=OV_UndefinedIdentifier && !isStream(rParentTypeIdentifier))
	{
		return false;
	}
	m_vName[rTypeIdentifier]=sTypeName;
	m_vStream[rTypeIdentifier]=rParentTypeIdentifier;
	log() << LogLevel_Trace << "Registered stream type id " << rTypeIdentifier << "::" << rParentTypeIdentifier << " - " << sTypeName << "\n";
	return true;
}

boolean CTypeManager::registerEnumerationType(
	const CIdentifier& rTypeIdentifier,
	const CString& sTypeName,
	const CString& sEnumerationString)
{
	if(isRegistered(rTypeIdentifier))
	{
		return false;
	}
	m_vName[rTypeIdentifier]=sTypeName;
	m_vEnumeration[rTypeIdentifier]=sEnumerationString;
	log() << LogLevel_Trace << "Registered enumeration type id " << rTypeIdentifier << " - " << sTypeName << "\n";
	return true;
}

boolean CTypeManager::registerBitMaskType(
	const CIdentifier& rTypeIdentifier,
	const CString& sTypeName,
	const CString& sBitMaskString)
{
	if(isRegistered(rTypeIdentifier))
	{
		return false;
	}
	m_vName[rTypeIdentifier]=sTypeName;
	m_vBitMask[rTypeIdentifier]=sBitMaskString;
	log() << LogLevel_Trace << "Registered bitmask type id " << rTypeIdentifier << " - " << sTypeName << "\n";
	return true;
}

boolean CTypeManager::isRegistered(
	const CIdentifier& rTypeIdentifier)
{
	return m_vName.find(rTypeIdentifier)!=m_vName.end()?true:false;
}

boolean CTypeManager::isStream(
	const CIdentifier& rTypeIdentifier)
{
	return m_vStream.find(rTypeIdentifier)!=m_vStream.end()?true:false;
}

boolean CTypeManager::isEnumeration(
	const CIdentifier& rTypeIdentifier)
{
	return m_vEnumeration.find(rTypeIdentifier)!=m_vEnumeration.end()?true:false;
}

boolean CTypeManager::isBitMask(
	const CIdentifier& rTypeIdentifier)
{
	return m_vBitMask.find(rTypeIdentifier)!=m_vBitMask.end()?true:false;
}

CString CTypeManager::getTypeName(
	const CIdentifier& rTypeIdentifier)
{
	if(!isRegistered(rTypeIdentifier))
	{
		return CString("");
	}
	return m_vName[rTypeIdentifier];
}

CIdentifier CTypeManager::getStreamParentType(
	const CIdentifier& rTypeIdentifier)
{
	if(!isStream(rTypeIdentifier))
	{
		return OV_UndefinedIdentifier;
	}
	return m_vStream[rTypeIdentifier];
}

CString CTypeManager::getEnumerationString(
	const CIdentifier& rTypeIdentifier)
{
	if(!isEnumeration(rTypeIdentifier))
	{
		return CString("");
	}
	return m_vEnumeration[rTypeIdentifier];
}

CString CTypeManager::getBitMaskString(
	const CIdentifier& rTypeIdentifier)
{
	if(!isBitMask(rTypeIdentifier))
	{
		return CString("");
	}
	return m_vBitMask[rTypeIdentifier];
}
