#ifndef __OpenViBEPlugins_SynchroBuffer_H__
#define __OpenViBEPlugins_SynchroBuffer_H__

#include "../../ovp_defines.h"
#include <openvibe/ov_all.h>
#include <openvibe-toolkit/ovtk_all.h>

namespace OpenViBEPlugins
{
	namespace SignalProcessing
	{
		class CSynchroBuffer : public OpenViBE::CMatrix
		{
			
		public:
			CSynchroBuffer(const OpenViBE::uint32 groupId);

			void				Build(const OpenViBE::uint64 samplingRate, const OpenViBE::uint32 nbChannels, const OpenViBE::uint32 nbSamples, const OpenViBE::uint32 durationBuffer); 
			OpenViBE::boolean	IsInitialized() const	{	return m_initialized;				}
			OpenViBE::boolean	IsDetected() const		{	return	m_detected;					}
			OpenViBE::boolean	HasSynchro(OpenViBE::uint32& offset, OpenViBE::uint32& dim) const
								{	offset	= m_synchroPtr - m_beginSynchroPtr;
									dim		= m_synchroPtr - m_synchroOldPtr;
									return	m_inSynchro && m_detected;
								}
			void				Push(const OpenViBE::IMatrix& data);
			OpenViBE::uint32	NbChannels()	{	return m_nbChannels;			}
			OpenViBE::uint32	NbSamples()		{	return m_nbChankSamples;		}
			void				GetResult(OpenViBE::IMatrix& result, const OpenViBE::uint32& offChannels);
			void				Correct(const OpenViBE::uint32 shift);
			void				TagUndetected();

		private:
			void				FindEdge();
			void				Shift(const OpenViBE::uint32 shift);

		private:
			OpenViBE::uint32	m_groupId;

			OpenViBE::boolean	m_initialized;
			OpenViBE::boolean	m_inSynchro;
			OpenViBE::boolean	m_detected;
			
			OpenViBE::uint32	m_nbChannels;
			OpenViBE::uint32	m_nbChankSamples;
			OpenViBE::uint32	m_nbChannelSamples;
			
			OpenViBE::float64*	m_beginPtr;
			OpenViBE::float64*	m_endPtr;
			OpenViBE::float64*	m_limitPtr;
			
			OpenViBE::float64*	m_beginSynchroPtr;
			OpenViBE::float64*	m_endSynchroPtr;
			
			OpenViBE::float64*	m_synchroOldPtr;
			OpenViBE::float64*	m_synchroPtr;
		};

	};
};

#endif // __OpenViBEPlugins_SynchroBuffer_H__
