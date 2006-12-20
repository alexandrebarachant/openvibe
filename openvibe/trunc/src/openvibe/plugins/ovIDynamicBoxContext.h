#ifndef __OpenViBE_Plugins_IDynamicBoxContext_H__
#define __OpenViBE_Plugins_IDynamicBoxContext_H__

#include "ovIPluginObjectContext.h"

namespace OpenViBE
{
	namespace Plugins
	{
		/**
		 * \class IDynamicBoxContext
		 * \author Yann Renard (INRIA/IRISA)
		 * \date 2006-06-21
		 * \brief Main §OpenViBE§ box interface to communicate with the kernel
		 *
		 * This context allows the object which uses it to check
		 * inputs states, read them, write outputs while beeing
		 * in a dynamic context (used by player).
		 */
		class OV_API IDynamicBoxContext : virtual public OpenViBE::Plugins::IPluginObjectContext
		{
		public:

			/** \name Accessing the input chunks informations */
			//@{

			/**
			 * \brief Gets the number of pending chunks for an input
			 * \param ui32InputIndex [in] : the input to work on.
			 * \return The number of pending chunks for an input
			 */
			virtual OpenViBE::uint32 getInputChunkCount(
				const OpenViBE::uint32 ui32InputIndex)=0;
			/**
			 * \brief Gets an input chunk and its time validity.
			 * \param ui32InputIndex [in] : the index of the desired input.
			 * \param ui32ChunkIndex [in] : the index of the desired
			 *        chunk in this input.
			 * \param rStartTime [out] : the time which the chunk starts at
			 * \param rEndTime [out] : the time which the chunk ends at
			 * \param rChunkSize [out] : the chunk buffer size in bytes
			 * \param rpChunkBuffer [out] : the chunk data itself
			 * \return \e true in case of success.
			 * \return \e false in case of error.
			 * \warning When returning \e false, none of the value
			 *          \c rStartTime, \c rEndTime, \c rChunkSize nor
			 *          \c rpChunkBuffer are defined.
			 * \warning The chunks are ordered like they arrived
			 *          to the box, this means chunk 0 arrived
			 *          before chunk 1, that arrived before
			 *          chunk 2 and so on...
			 * \sa getChunkCount
			 * \sa releaseChunk
			 */
			virtual OpenViBE::boolean getInputChunk(
				const OpenViBE::uint32 ui32InputIndex,
				const OpenViBE::uint32 ui32ChunkIndex,
				OpenViBE::uint64& rStartTime,
				OpenViBE::uint64& rEndTime,
				OpenViBE::uint64& rChunkSize,
				const OpenViBE::uint8*& rpChunkBuffer)=0;
			/**
			 * \brief Marks an input chunk as deprecated
			 * \param ui32InputIndex [in] : the index of the desired input.
			 * \param ui32ChunkIndex [in] : the index of the chunk to mark.
			 * \return \e true in case of success.
			 * \return \e false in case of error.
			 * \warning The chunks are ordered like they arrived
			 *          to the box, this means chunk 0 arrived
			 *          before chunk 1, that arrived before
			 *          chunk 2 and so on...
			 *
			 * This function discards a chunk when it's been read
			 * and processed. This allows the kernel to know a chunk
			 * has been treated successfully and will not be used
			 * any more so it can be removed from memory.
			 *
			 * \sa getChunkCount
			 * \sa getChunk
			 */
			virtual OpenViBE::boolean markInputAsDeprecated(
				const OpenViBE::uint32 ui32InputIndex,
				const OpenViBE::uint32 ui32ChunkIndex)=0;

			//@}

#if 0
/** \name Information related to this input */
//@{

/**
 * \brief Gets the source information for this input
 * \param rBoxIdentifier [out] : the box identifier
 *        for this input
 * \param rBoxOutputIndex [out] : the output index
 *        of the box which is connected to this input
 * \return \e true in case of success.
 * \return \e false in case of error.
 */
virtual OpenViBE::boolean getSource(
	OpenViBE::CIdentifier& rBoxIdentifier,
	OpenViBE::uint32& rBoxOutputIndex)=0;

//@}
#endif

			/** \name Accessing the output chunks information */
			//@{

			/**
			 * \brief Gets the output chunk size
			 * \param ui32OutputIndex [in] : the index of the output to work on
			 * \return The output chunk size in bytes
			 */
			virtual OpenViBE::uint64 getOutputChunkSize(
				const OpenViBE::uint32 ui32OutputIndex)=0;
			/**
			 * \brief Sets an output chunk size
			 * \param ui32OutputIndex [in] : the index of the output to work on
			 * \param ui64Size [in] : the new size of the output chunk
			 * \param bDiscard [in] : tells if existing buffer should be discarded or not
			 * \return \e true in case of success.
			 * \return \e false in case of error.
			 */
			virtual OpenViBE::boolean setOutputChunkSize(
				const OpenViBE::uint32 ui32OutputIndex,
				const OpenViBE::uint64 ui64Size,
				const OpenViBE::boolean bDiscard=true)=0;
			/**
			 * \brief Gets a pointer to the current output chunk buffer
			 * \param ui32OutputIndex [in] : the index of the output to work on
			 * \return A pointer to the current output chunk buffer
			 * \warning The returned pointer may change if
			 *          the caller resizes the chunk buffer
			 *          using \c setChunkSize !
			 */
			virtual OpenViBE::uint8* getOutputChunkBuffer(
				const OpenViBE::uint32 ui32OutputIndex)=0;
			/**
			 * \brief Appends data to the output chunk
			 * \param ui32OutputIndex [in] : the index of the output to work on
			 * \param pBuffer [in] : the buffer to append to the current buffer
			 * \param ui64BufferSize [in] : the size of the appended buffer
			 * \return \e true in case of success.
			 * \return \e false in case of error.
			 * \warning This function resizes the chunk size
			 *          calling \c setOutputChunkSize and then
			 *          appends data doing a copy.
			 */
			virtual OpenViBE::boolean appendOutputChunkData(
				const OpenViBE::uint32 ui32OutputIndex,
				const OpenViBE::uint8* pBuffer,
				const OpenViBE::uint64 ui64BufferSize)=0;
			/**
			 * \brief Marks output buffer as 'ready to send'
			 * \param ui32OutputIndex [in] : the index of the output to work on
			 * \param ui64EndTime [in] : the end time for the
			 *        related buffer. The start time is deduced
			 *        from the last end time for this output.
			 * \return \e true in case of success.
			 * \return \e false in case of error.
			 *
			 * The output chunk should first be filled. For
			 * that, one will have to get a reference on it
			 * thanks to the getChunkBuffer method ! The
			 * player will then know the buffer can be sent.
			 *
			 * \sa getChunk
			 * \note It is important to call the \c send 
			 *       even if no data should be sent. In order
			 *       to do so, first request a buffer resize to
			 *       0, then call 'send'. This is to tell
			 *       following boxes that there won't be
			 *       anything more for this time fork.
			 */
			virtual OpenViBE::boolean markOutputAsReadyToSend(
				const OpenViBE::uint32 ui32OutputIndex,
				const OpenViBE::uint64 ui64EndTime)=0;

			//@}

#if 0
/** \name Information related to this output */
//@{

/**
 * \brief Gets the number of connections from this output
 * \return The number of connections starting from this output
 */
virtual OpenViBE::uint32 getTargetCount(void)=0;
/**
 * \brief Gets the target information for this output
 * \param ui32TargetIndex [in] : the target index
 * \param rBoxIdentifier [out] : the target box
 *        identifier for this output
 * \param rBoxInputIndex [out] : the input index
 *        of the boxes which are connected to this output
 * \return \e true in case of success.
 * \return \e false in case of error.
 */
virtual OpenViBE::boolean getTarget(
	const OpenViBE::uint32 ui32TargetIndex,
	OpenViBE::CIdentifier& rBoxIdentifier,
	OpenViBE::uint32& rBoxInputIndex)=0;

//@}
#endif

			_IsDerivedFromClass_(OpenViBE::IObject, OV_ClassId_Plugins_DynamicBoxContext)
		};
	};
};

#endif // __OpenViBE_Plugins_IDynamicBoxContext_H__
