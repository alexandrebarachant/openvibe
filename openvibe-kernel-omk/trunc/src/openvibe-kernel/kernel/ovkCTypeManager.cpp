#include "ovkCTypeManager.h"

using namespace OpenViBE;
using namespace OpenViBE::Kernel;
using namespace std;

CTypeManager::CTypeManager(const IKernelContext& rKernelContext)
	:TKernelObject<ITypeManager>(rKernelContext)
{
}

boolean CTypeManager::registerEnumerationType(
	const CIdentifier& rTypeIdentifier,
	const CString& rEnumerationString)
{
	if(isRegistered(rTypeIdentifier))
	{
		return false;
	}
	m_vEnumeration[rTypeIdentifier]=rEnumerationString;
	return true;
}

boolean CTypeManager::registerBitMaskType(
	const CIdentifier& rTypeIdentifier,
	const CString& rBitMaskString)
{
	if(isRegistered(rTypeIdentifier))
	{
		return false;
	}
	m_vBitMask[rTypeIdentifier]=rBitMaskString;
}

boolean CTypeManager::isRegistered(
	const CIdentifier& rTypeIdentifier)
{
	if(rTypeIdentifier==OV_TypeId_Boolean) return true;
	if(rTypeIdentifier==OV_TypeId_Integer) return true;
	if(rTypeIdentifier==OV_TypeId_Float) return true;
	if(rTypeIdentifier==OV_TypeId_String) return true;
	if(isEnumeration(rTypeIdentifier)) return true;
	if(isBitMask(rTypeIdentifier)) return true;
	return false;
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
