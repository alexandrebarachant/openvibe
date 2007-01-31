#ifndef __EBML_TReaderCallbackProxy__
#define __EBML_TReaderCallbackProxy__

#include "IReader.h"

namespace EBML
{

// ________________________________________________________________________________________________________________
//

	template <class COwnerClass>
	class EBML_API TReaderCallbackProxy1 : virtual public EBML::IReaderCallback
	{
	public:
		TReaderCallbackProxy1(
			COwnerClass& rOwnerObject,
			EBML::boolean (COwnerClass::*mfpIsMasterChild)(const EBML::CIdentifier& rIdentifier),
			void (COwnerClass::*mfpOpenChild)(const EBML::CIdentifier& rIdentifier),
			void (COwnerClass::*mfpProcessChildData)(const void* pBuffer, const EBML::uint64 ui64BufferSize),
			void (COwnerClass::*mfpCloseChild)(void))
			:m_rOwnerObject(rOwnerObject)
			,m_mfpIsMasterChild(mfpIsMasterChild)
			,m_mfpOpenChild(mfpOpenChild)
			,m_mfpProcessChidData(mfpProcessChildData)
			,m_mfpCloseChild(mfpCloseChild)
		{
		}
		virtual EBML::boolean isMasterChild(const EBML::CIdentifier& rIdentifier)
		{
			if(m_mfpIsMasterChild)
			{
				return (m_rOwnerObject.*m_mfpIsMasterChild)(rIdentifier);
			}
			return false;
		}
		virtual void openChild(const EBML::CIdentifier& rIdentifier)
		{
			if(m_mfpOpenChild)
			{
				(m_rOwnerObject.*m_mfpOpenChild)(rIdentifier);
			}
		}
		virtual void processData(const void* pBuffer, const EBML::uint64 ui64BufferSize)
		{
			if(m_mfpProcessChidData)
			{
				(m_rOwnerObject.*m_mfpProcessChidData)(pBuffer, ui64BufferSize);
			}
		}
		virtual void closeChild(void)
		{
			if(m_mfpCloseChild)
			{
				(m_rOwnerObject.*m_mfpCloseChild)();
			}
		}
	protected:
		COwnerClass& m_rOwnerObject;
		EBML::boolean (COwnerClass::*m_mfpIsMasterChild)(const EBML::CIdentifier& rIdentifier);
		void (COwnerClass::*m_mfpOpenChild)(const EBML::CIdentifier& rIdentifier);
		void (COwnerClass::*m_mfpProcessChildData)(const void* pBuffer, const EBML::uint64 ui64BufferSize);
		void (COwnerClass::*m_mfpCloseChild)(void);
	};

// ________________________________________________________________________________________________________________
//

	template <class COwnerClass, EBML::boolean (COwnerClass::*mfpIsMasterChild)(const EBML::CIdentifier& rIdentifier), void (COwnerClass::*mfpOpenChild)(const EBML::CIdentifier& rIdentifier), void (COwnerClass::*mfpProcessChildData)(const void* pBuffer, const EBML::uint64 ui64BufferSize), void (COwnerClass::*mfpCloseChild)(void)>
	class EBML_API TReaderCallbackProxy2 : virtual public EBML::IReaderCallback
	{
	public:
		TReaderCallbackProxy1(
			COwnerClass& rOwnerObject)
			:m_rOwnerObject(rOwnerObject)
			,m_mfpIsMasterChild(mfpIsMasterChild)
			,m_mfpOpenChild(mfpOpenChild)
			,m_mfpProcessChidData(mfpProcessChildData)
			,m_mfpCloseChild(mfpCloseChild)
		{
		}
		virtual EBML::boolean isMasterChild(const EBML::CIdentifier& rIdentifier)
		{
			if(m_mfpIsMasterChild)
			{
				return (m_rOwnerObject.*m_mfpIsMasterChild)(rIdentifier);
			}
			return false;
		}
		virtual void openChild(const EBML::CIdentifier& rIdentifier)
		{
			if(m_mfpOpenChild)
			{
				(m_rOwnerObject.*m_mfpOpenChild)(rIdentifier);
			}
		}
		virtual void processData(const void* pBuffer, const EBML::uint64 ui64BufferSize)
		{
			if(m_mfpProcessChidData)
			{
				(m_rOwnerObject.*m_mfpProcessChidData)(pBuffer, ui64BufferSize);
			}
		}
		virtual void closeChild(void)
		{
			if(m_mfpCloseChild)
			{
				(m_rOwnerObject.*m_mfpCloseChild)();
			}
		}
	protected:
		COwnerClass& m_rOwnerObject;
		EBML::boolean (COwnerClass::*m_mfpIsMasterChild)(const EBML::CIdentifier& rIdentifier);
		void (COwnerClass::*m_mfpOpenChild)(const EBML::CIdentifier& rIdentifier);
		void (COwnerClass::*m_mfpProcessChildData)(const void* pBuffer, const EBML::uint64 ui64BufferSize);
		void (COwnerClass::*m_mfpCloseChild)(void);
	};

// ________________________________________________________________________________________________________________
//

};

#endif // __EBML_TReaderCallbackProxy__
