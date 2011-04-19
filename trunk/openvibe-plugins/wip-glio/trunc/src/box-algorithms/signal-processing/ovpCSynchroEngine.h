#ifndef __OpenViBEPlugins_SynchroEngine_H__
#define __OpenViBEPlugins_SynchroEngine_H__

#include "ovpCSynchroBuffer.h"

namespace OpenViBEPlugins
{
	namespace SignalProcessing
	{
		class CSynchroEngine
		{
			
		public:
			CSynchroEngine();

			void				Build(const OpenViBE::uint32 group, const OpenViBE::uint64 samplingRate, const OpenViBE::uint32 nbChannels, const OpenViBE::uint32 nbSamples, const OpenViBE::uint32 durationBuffer); 
			void				Push(const OpenViBE::uint32 group, const OpenViBE::IMatrix& data);
			OpenViBE::boolean	IsInitialized() const
								{	return m_SynchroBuffer1.IsInitialized() &&  m_SynchroBuffer2.IsInitialized();	}
			OpenViBE::uint32	NbChunks(const OpenViBE::uint32 outputChunkSize);
			void				GetResult(OpenViBE::IMatrix& result);
			OpenViBE::uint32	NbChannels(const OpenViBE::uint32 group)		{	return group ?  m_SynchroBuffer2.NbChannels() : m_SynchroBuffer1.NbChannels();		}
			OpenViBE::uint32	NbSamples(const OpenViBE::uint32 group)			{	return group ?  m_SynchroBuffer2.NbSamples() : m_SynchroBuffer1.NbSamples();		}
			OpenViBE::boolean	IsDetected(const OpenViBE::uint32 group)		{	return group ?  m_SynchroBuffer2.IsDetected() : m_SynchroBuffer1.IsDetected();		}
			void				TagUndetected()
								{	m_SynchroBuffer1.TagUndetected();	m_SynchroBuffer2.TagUndetected();			}

		private:
			OpenViBE::boolean	HasSynchro(OpenViBE::uint32& offset1, OpenViBE::uint32& dim1, OpenViBE::uint32& offset2, OpenViBE::uint32& dim2) const
								{	return m_SynchroBuffer1.HasSynchro(offset1, dim1) &&  m_SynchroBuffer2.HasSynchro(offset2, dim2);	}

		private:
			CSynchroBuffer		m_SynchroBuffer1;
			CSynchroBuffer		m_SynchroBuffer2;
		};

	};
};

#endif // __OpenViBEPlugins_SynchroBuffer_H__
