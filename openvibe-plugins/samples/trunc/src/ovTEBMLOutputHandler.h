#ifndef __OpenViBEPlugins_Acquisition_TEBMLOutputHandler_H__
#define __OpenViBEPlugins_Acquisition_TEBMLOutputHandler_H__

#include <openvibe/ov_all.h>

#include <ebml/IWriter.h>
#include <ebml/IWriterHelper.h>

namespace OpenViBEPlugins
{
	namespace Common
	{
		template <class T> class TEBMLOutputHandler : virtual public EBML::IWriterCallBack
		{
		public:

			TEBMLOutputHandler(T& rPlugin, const OpenViBE::uint32 ui32OutputIndex)
				:m_rPlugin(rPlugin)
				,m_ui32OutputIndex(ui32OutputIndex)
				,m_pEBMLWriter(NULL)
				,m_pEBMLWriterHelper(NULL)
			{
				m_pEBMLWriter=EBML::createWriter(*this);
				m_pEBMLWriterHelper=EBML::createWriterHelper();
				m_pEBMLWriterHelper->connect(m_pEBMLWriter);
			}

			virtual ~TEBMLOutputHandler(void)
			{
				m_pEBMLWriterHelper->disconnect();
				m_pEBMLWriterHelper->release();
				m_pEBMLWriter->release();
			}

			virtual void write(const void* pBuffer, const EBML::uint64 ui64BufferSize)
			{
				m_rPlugin.write(m_ui32OutputIndex, pBuffer, ui64BufferSize);
			}

			operator EBML::IWriterHelper* (void)
			{
				return m_pEBMLWriterHelper;
			}

			operator EBML::IWriter* (void)
			{
				return m_pEBMLWriter;
			}

		protected:

			T& m_rPlugin;
			const OpenViBE::uint32 m_ui32OutputIndex;
			EBML::IWriter* m_pEBMLWriter;
			EBML::IWriterHelper* m_pEBMLWriterHelper;
		};
	};
};

#endif // __OpenViBEPlugins_Acquisition_TEBMLOutputHandler_H__
