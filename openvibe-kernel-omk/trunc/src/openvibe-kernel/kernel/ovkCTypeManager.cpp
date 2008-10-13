#include "ovkCTypeManager.h"

#include "../ovk_tools.h"

#include <string>

using namespace OpenViBE;
using namespace OpenViBE::Kernel;
using namespace std;

CTypeManager::CTypeManager(const IKernelContext& rKernelContext)
	:TKernelObject<ITypeManager>(rKernelContext)
{
	m_vName[OV_UndefinedIdentifier]="undefined";
}


CIdentifier CTypeManager::getNextTypeIdentifier(
	const CIdentifier& rPreviousIdentifier) const
{
	return getNextIdentifier< CString >(m_vName, rPreviousIdentifier);
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
	const CString& sTypeName)
{
	if(isRegistered(rTypeIdentifier))
	{
		return false;
	}
	m_vName[rTypeIdentifier]=sTypeName;
	m_vEnumeration[rTypeIdentifier];
	log() << LogLevel_Trace << "Registered enumeration type id " << rTypeIdentifier << " - " << sTypeName << "\n";
	return true;
}

boolean CTypeManager::registerEnumerationEntry(
	const CIdentifier& rTypeIdentifier,
	const CString& sEntryName,
	const uint64 ui64EntryValue)
{
	map<CIdentifier, map<uint64, CString> >::iterator itEnumeration=m_vEnumeration.find(rTypeIdentifier);
	if(itEnumeration==m_vEnumeration.end())
	{
		return false;
	}
	map<uint64, CString>::iterator itEnumerationEntry=itEnumeration->second.find(ui64EntryValue);
	if(itEnumerationEntry!=itEnumeration->second.end())
	{
		return false;
	}
	itEnumeration->second[ui64EntryValue]=sEntryName;
	return true;
}

boolean CTypeManager::registerBitMaskType(
	const CIdentifier& rTypeIdentifier,
	const CString& sTypeName)
{
	if(isRegistered(rTypeIdentifier))
	{
		return false;
	}
	m_vName[rTypeIdentifier]=sTypeName;
	m_vBitMask[rTypeIdentifier];
	log() << LogLevel_Trace << "Registered bitmask type id " << rTypeIdentifier << " - " << sTypeName << "\n";
	return true;
}

boolean CTypeManager::registerBitMaskEntry(
	const CIdentifier& rTypeIdentifier,
	const CString& sEntryName,
	const uint64 ui64EntryValue)
{
	map<CIdentifier, map<uint64, CString> >::iterator itBitMask=m_vBitMask.find(rTypeIdentifier);
	if(itBitMask==m_vBitMask.end())
	{
		return false;
	}
	map<uint64, CString>::iterator itBitMaskEntry=itBitMask->second.find(ui64EntryValue);
	if(itBitMaskEntry!=itBitMask->second.end())
	{
		return false;
	}
	for(uint32 l_ui32BitCount=0, i=0; i<64; i++)
	{
		if(ui64EntryValue&(1LL<<i))
		{
			l_ui32BitCount++;
			if(l_ui32BitCount>1)
			{
				this->getLogManager() << LogLevel_ImportantWarning << "Discarded bitmask entry (" << m_vName[rTypeIdentifier] << ":" << sEntryName << ") because value " << ui64EntryValue << " contains more than one bit\n";
				return false;
			}
		}
	}
	itBitMask->second[ui64EntryValue]=sEntryName;
	return true;
}

boolean CTypeManager::isRegistered(
	const CIdentifier& rTypeIdentifier) const
{
	return m_vName.find(rTypeIdentifier)!=m_vName.end()?true:false;
}

boolean CTypeManager::isStream(
	const CIdentifier& rTypeIdentifier) const
{
	return m_vStream.find(rTypeIdentifier)!=m_vStream.end()?true:false;
}

boolean CTypeManager::isEnumeration(
	const CIdentifier& rTypeIdentifier) const
{
	return m_vEnumeration.find(rTypeIdentifier)!=m_vEnumeration.end()?true:false;
}

boolean CTypeManager::isBitMask(
	const CIdentifier& rTypeIdentifier) const
{
	return m_vBitMask.find(rTypeIdentifier)!=m_vBitMask.end()?true:false;
}

CString CTypeManager::getTypeName(
	const CIdentifier& rTypeIdentifier) const
{
	if(!isRegistered(rTypeIdentifier))
	{
		return CString("");
	}
	return m_vName.find(rTypeIdentifier)->second;
}

CIdentifier CTypeManager::getStreamParentType(
	const CIdentifier& rTypeIdentifier) const
{
	if(!isStream(rTypeIdentifier))
	{
		return OV_UndefinedIdentifier;
	}
	return m_vStream.find(rTypeIdentifier)->second;
}

uint64 CTypeManager::getEnumerationEntryCount(
	const CIdentifier& rTypeIdentifier) const
{
	map<CIdentifier, map<uint64, CString> >::const_iterator itEnumeration=m_vEnumeration.find(rTypeIdentifier);
	if(itEnumeration==m_vEnumeration.end())
	{
		return 0;
	}
	return itEnumeration->second.size();
}

boolean CTypeManager::getEnumerationEntry(
	const CIdentifier& rTypeIdentifier,
	const uint64 ui64EntryIndex,
	CString& sEntryName,
	uint64& rEntryValue) const
{
	map<CIdentifier, map<uint64, CString> >::const_iterator itEnumeration=m_vEnumeration.find(rTypeIdentifier);
	if(itEnumeration==m_vEnumeration.end())
	{
		return false;
	}

	if(ui64EntryIndex>=itEnumeration->second.size())
	{
		return false;
	}

	map<uint64, CString>::const_iterator itEnumerationEntry=itEnumeration->second.begin();
	for(uint64 i=0; i<ui64EntryIndex && itEnumerationEntry!=itEnumeration->second.end(); i++, itEnumerationEntry++)
	{
	}

	rEntryValue=itEnumerationEntry->first;
	sEntryName=itEnumerationEntry->second;
	return true;

}

CString CTypeManager::getEnumerationEntryNameFromValue(
	const CIdentifier& rTypeIdentifier,
	const uint64 ui64EntryValue) const
{
	map<CIdentifier, map<uint64, CString> >::const_iterator itEnumeration=m_vEnumeration.find(rTypeIdentifier);
	if(itEnumeration==m_vEnumeration.end())
	{
		return "";
	}
	map<uint64, CString>::const_iterator itEnumerationEntry=itEnumeration->second.find(ui64EntryValue);
	if(itEnumerationEntry==itEnumeration->second.end())
	{
		return "";
	}
	return itEnumeration->second.find(ui64EntryValue)->second;
}

uint64 CTypeManager::getEnumerationEntryValueFromName(
	const CIdentifier& rTypeIdentifier,
	const CString& rEntryName) const
{
	map<CIdentifier, map<uint64, CString> >::const_iterator itEnumeration=m_vEnumeration.find(rTypeIdentifier);
	map<uint64, CString>::const_iterator itEnumerationEntry;
	if(itEnumeration==m_vEnumeration.end())
	{
		return 0xffffffffffffffffll;
	}

	// first looks at the exact string match
	for(itEnumerationEntry=itEnumeration->second.begin(); itEnumerationEntry!=itEnumeration->second.end(); itEnumerationEntry++)
	{
		if(itEnumerationEntry->second==rEntryName)
		{
			return itEnumerationEntry->first;
		}
	}

	// then looks at the caseless string match
	string l_sEntryName(rEntryName);
	string l_sEntryNameLower;
	std::transform(l_sEntryName.begin(), l_sEntryName.end(), l_sEntryNameLower.begin(), std::tolower);
	for(itEnumerationEntry=itEnumeration->second.begin(); itEnumerationEntry!=itEnumeration->second.end(); itEnumerationEntry++)
	{
		string l_sItEntryName(itEnumerationEntry->second);
		string l_sItEntryNameLower;
		std::transform(l_sItEntryName.begin(), l_sItEntryName.end(), l_sItEntryNameLower.begin(), std::tolower);
		if(l_sItEntryNameLower==l_sEntryNameLower)
		{
			return itEnumerationEntry->first;
		}
	}

	// then looks at the string beeing the value itself
	uint64 l_ui64Value;
	if(sscanf((const char*)rEntryName, "%lli", &l_ui64Value)==1)
	{
		if(itEnumeration->second.find(l_ui64Value)!=itEnumeration->second.end())
		{
			return l_ui64Value;
		}
	}

	return 0xffffffffffffffffll;
}

uint64 CTypeManager::getBitMaskEntryCount(
	const CIdentifier& rTypeIdentifier) const
{
	map<CIdentifier, map<uint64, CString> >::const_iterator itBitMask=m_vBitMask.find(rTypeIdentifier);
	if(itBitMask==m_vBitMask.end())
	{
		return 0;
	}
	return itBitMask->second.size();
}

boolean CTypeManager::getBitMaskEntry(
	const CIdentifier& rTypeIdentifier,
	const uint64 ui64EntryIndex,
	CString& sEntryName,
	uint64& rEntryValue) const
{
	map<CIdentifier, map<uint64, CString> >::const_iterator itBitMask=m_vBitMask.find(rTypeIdentifier);
	if(itBitMask==m_vBitMask.end())
	{
		return false;
	}

	if(ui64EntryIndex>=itBitMask->second.size())
	{
		return false;
	}

	map<uint64, CString>::const_iterator itBitMaskEntry=itBitMask->second.begin();
	for(uint64 i=0; i<ui64EntryIndex && itBitMaskEntry!=itBitMask->second.end(); i++, itBitMaskEntry++)
	{
	}

	rEntryValue=itBitMaskEntry->first;
	sEntryName=itBitMaskEntry->second;
	return true;

}

CString CTypeManager::getBitMaskEntryNameFromValue(
	const CIdentifier& rTypeIdentifier,
	const uint64 ui64EntryValue) const
{
	map<CIdentifier, map<uint64, CString> >::const_iterator itBitMask=m_vBitMask.find(rTypeIdentifier);
	if(itBitMask==m_vBitMask.end())
	{
		return "";
	}
	map<uint64, CString>::const_iterator itBitMaskEntry=itBitMask->second.find(ui64EntryValue);
	if(itBitMaskEntry==itBitMask->second.end())
	{
		return "";
	}
	return itBitMask->second.find(ui64EntryValue)->second;
}

uint64 CTypeManager::getBitMaskEntryValueFromName(
	const CIdentifier& rTypeIdentifier,
	const CString& rEntryName) const
{
	map<CIdentifier, map<uint64, CString> >::const_iterator itBitMask=m_vBitMask.find(rTypeIdentifier);
	map<uint64, CString>::const_iterator itBitMaskEntry;
	if(itBitMask==m_vBitMask.end())
	{
		return 0xffffffffffffffffll;
	}

	// first looks at the exact string match
	for(itBitMaskEntry=itBitMask->second.begin(); itBitMaskEntry!=itBitMask->second.end(); itBitMaskEntry++)
	{
		if(itBitMaskEntry->second==rEntryName)
		{
			return itBitMaskEntry->first;
		}
	}

	// then looks at the caseless string match
	string l_sEntryName(rEntryName);
	string l_sEntryNameLower;
	std::transform(l_sEntryName.begin(), l_sEntryName.end(), l_sEntryNameLower.begin(), std::tolower);
	for(itBitMaskEntry=itBitMask->second.begin(); itBitMaskEntry!=itBitMask->second.end(); itBitMaskEntry++)
	{
		string l_sItEntryName(itBitMaskEntry->second);
		string l_sItEntryNameLower;
		std::transform(l_sItEntryName.begin(), l_sItEntryName.end(), l_sItEntryNameLower.begin(), std::tolower);
		if(l_sItEntryNameLower==l_sEntryNameLower)
		{
			return itBitMaskEntry->first;
		}
	}

	// then looks at the string beeing the value itself
	uint64 l_ui64Value;
	if(sscanf((const char*)rEntryName, "%lli", &l_ui64Value)==1)
	{
		if(itBitMask->second.find(l_ui64Value)!=itBitMask->second.end())
		{
			return l_ui64Value;
		}
	}

	return 0xffffffffffffffffll;
}

CString CTypeManager::getBitMaskEntryCompositionNameFromValue(
	const CIdentifier& rTypeIdentifier,
	const uint64 ui64EntryCompositionValue) const
{
	map<CIdentifier, map<uint64, CString> >::const_iterator itBitMask=m_vBitMask.find(rTypeIdentifier);
	if(itBitMask==m_vBitMask.end())
	{
		return "";
	}

	string l_sResult;
	for(uint32 i=0; i<64; i++)
	{
		if(ui64EntryCompositionValue&(1LL<<i))
		{
			map<uint64, CString>::const_iterator itBitMaskEntry=itBitMask->second.find(ui64EntryCompositionValue&(1LL<<i));
			if(itBitMaskEntry==itBitMask->second.end())
			{
				return "";
			}
			if(l_sResult=="")
			{
				l_sResult=itBitMaskEntry->second.toASCIIString();
			}
			else
			{
				;
				l_sResult+=string(1, OV_Value_EnumeratedStringSeparator);
				l_sResult+=itBitMaskEntry->second.toASCIIString();
			}
		}
	}
	return CString(l_sResult.c_str());
}

uint64 CTypeManager::getBitMaskEntryCompositionValueFromName(
	const CIdentifier& rTypeIdentifier,
	const CString& rEntryCompositionName) const
{
	map<CIdentifier, map<uint64, CString> >::const_iterator itBitMask=m_vBitMask.find(rTypeIdentifier);
	if(itBitMask==m_vBitMask.end())
	{
		return 0;
	}

	uint64 l_ui64Result=0;
	string l_sEntryCompositionName=rEntryCompositionName.toASCIIString();
	string::size_type i=0;
	string::size_type j=0;
	do
	{
		i=l_sEntryCompositionName.find(OV_Value_EnumeratedStringSeparator, i);
		if(i==string::npos)
		{
			i=l_sEntryCompositionName.length();
		}

		if(i!=j)
		{
			string l_sEntryName;
			l_sEntryName.assign(l_sEntryCompositionName, j, i-j);

			boolean l_bFound=false;
			map<uint64, CString>::const_iterator itBitMaskEntry;
			for(itBitMaskEntry=itBitMask->second.begin(); itBitMaskEntry!=itBitMask->second.end(); itBitMaskEntry++)
			{
				if(itBitMaskEntry->second==CString(l_sEntryName.c_str()))
				{
					l_ui64Result|=itBitMaskEntry->first;
					l_bFound=true;
				}
			}

			if(!l_bFound)
			{
				return 0;
			}
		}

		i++;
		j=i;
	}
	while(i<l_sEntryCompositionName.length());

	return l_ui64Result;
}
