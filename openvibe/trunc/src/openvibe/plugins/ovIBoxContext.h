#ifndef __OpenViBE_Plugins_IBoxContext_H__
#define __OpenViBE_Plugins_IBoxContext_H__

#include "ovIPluginObjectContext.h"

namespace OpenViBE
{
	namespace Plugins
	{
		/**
		 * \class IBoxContext
		 * \author Yann Renard (INRIA/IRISA)
		 * \date 2006-06-21
		 * \brief Main §OpenViBE§ box interface to communicate with the kernel
		 *
		 * This box context is given to plugin objects such as box
		 * behavior or box algorithm so they can access the "seen"
		 * box interface. This context allows the object which uses
		 * it to check inputs states, read them, write outputs. Also
		 * it allows the object which uses this context to send
		 * or broadcast event messages or to request signal from
		 * the Kernel.
		 */
		class OV_API IBoxContext : virtual public OpenViBE::Plugins::IPluginObjectContext
		{
		public:

			/**
			 * \class IConnectorBase
			 * \author Yann Renard (INRIA/IRISA)
			 * \date 2006-06-23
			 * \brief Base class for box inputs/outputs interfaces
			 */
			class OV_API IConnectorBase : virtual public OpenViBE::Plugins::IPluginObjectContext
			{
			public:

				/** \name Information related to this connector */
				//@{

				/**
				 * \brief Gets the name of this connector
				 * \return The name of this connector.
				 */
				virtual OpenViBE::CString getName(void)=0;
				/**
				 * \brief Gets the type identifier this connector handles
				 * \return The type identifier this connector handles.
				 */
				virtual OpenViBE::CIdentifier getTypeIdentifier(void)=0;

				//@}

				_IsDerivedFromClass_(OpenViBE::IObject, OV_ClassId_Plugins_BoxContext_ConnectorBase)
			};

			/**
			 * \class IInput
			 * \author Yann Renard (INRIA/IRISA)
			 * \date 2006-06-23
			 * \brief Input interface for §OpenViBE§ boxes
			 * \sa IOutput
			 */
			class OV_API IInput : virtual public OpenViBE::Plugins::IBoxContext::IConnectorBase
			{
			public:

				/** \name Accessing the input chunks informations */
				//@{

				/**
				 * \brief Gets the number of pending chunks for this input
				 * \return The number of pending chunks for this input
				 */
				virtual OpenViBE::uint32 getChunkCount(void)=0;
				/**
				 * \brief Gets an input chunk and its time validity.
				 * \param ui32Index [in] : the index of the desired
				 *        input chunk.
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
				virtual OpenViBE::boolean getChunk(
					const OpenViBE::uint32 ui32Index,
					OpenViBE::uint64& rStartTime,
					OpenViBE::uint64& rEndTime,
					OpenViBE::uint64& rChunkSize,
					const OpenViBE::uint8*& rpChunkBuffer)=0;
				/**
				 * \brief Releases an input chunk
				 * \param ui32Index [in] : the index of the chunk to release
				 * \return \e true in case of success.
				 * \return \e false in case of error.
				 * \warning The chunks are ordered like they arrived
				 *          to the box, this means chunk 0 arrived
				 *          before chunk 1, that arrived before
				 *          chunk 2 and so on...
				 * \warning Due to chunk ordering, it's logic that chunk
				 *          O is treated first. Releasing this chunk would
				 *          change the indices so chunk 1 becomes chunk 0.
				 *          Finaly, next chunk to work on is chunk 0 :)
				 *
				 * This function discards a chunk when it's been read
				 * and processed. This allows the kernel to know a chunk
				 * has been treated successfully and will not be used
				 * any more so it can be removed from memory.
				 *
				 * \sa getChunkCount
				 * \sa getChunk
				 */
				virtual OpenViBE::boolean releaseChunk(
					const OpenViBE::uint32 ui32Index)=0;

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

				_IsDerivedFromClass_(OpenViBE::IObject, OV_ClassId_Plugins_BoxContext_Input)
			};

			/**
			 * \class IOutput
			 * \author Yann Renard (INRIA/IRISA)
			 * \date 2006-06-23
			 * \brief Output interface for §OpenViBE§ boxes
			 * \sa IInput
			 */
			class OV_API IOutput : virtual public OpenViBE::Plugins::IBoxContext::IConnectorBase
			{
			public:

				/** \name Accessing the output chunk information */
				//@{

				/**
				 * \brief Gets the chunk size
				 * \return The chunk size in bytes
				 */
				virtual OpenViBE::uint64 getChunkSize(void)=0;
				/**
				 * \brief Sets the output chunk size
				 * \param ui64Size [in] : the new size of the output chunk
				 * \return \e true in case of success.
				 * \return \e false in case of error.
				 */
				virtual OpenViBE::boolean setChunkSize(
					const OpenViBE::uint64 ui64Size)=0;
				/**
				 * \brief Gets a pointer to the current output chunk buffer
				 * \return A pointer to the current output chunk buffer
				 * \warning The returned pointer may change if
				 *          the caller resizes the chunk buffer
				 *          using \c setChunkSize !
				 */
				virtual OpenViBE::uint8* getChunkBuffer(void)=0;

				/**
				 * \brief Effectively sends the data chunk
				 * \param rTimeValidity [in] : the time validity interval 
				 *        for the output chunk.
				 * \return \e true in case of success.
				 * \return \e false in case of error.
				 *
				 * The output chunk should first be filled. For
				 * that, one will have to get a reference on it
				 * thanks to the getChunk method !
				 *
				 * \sa getChunk
				 */
				virtual OpenViBE::boolean send(
					const OpenViBE::uint64 ui64StartTime,
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

				_IsDerivedFromClass_(OpenViBE::IObject, OV_ClassId_Plugins_BoxContext_Output)
			};

			/** \name Box identification */
			//@{

			/**
			 * \brief Gets the identifier of the related box
			 * \return The identifier of the related box.
			 *
			 * This function allows the caller to know the identifier
			 * of the box it's working on. Each box has a unique
			 * instance identifier which is used when connecting
			 * boxes together and whend sending messages between
			 * boxes.
			 */
			virtual OpenViBE::CIdentifier getIdentifier(void) const=0;
			/**
			 * \brief Gets the name of the related box
			 * \return The name of the related box.
			 */
			virtual OpenViBE::CString getName(void) const=0;

			//@}
			/** \name Input management */
			//@{

			/**
			 * \brief Gets the input count for the related box
			 * \return The input count for the related box.
			 */
			virtual OpenViBE::uint32 getInputCount(void) const=0;
			/**
			 * \brief Gets an input handle
			 * \param ui32InputIndex [in] : the input index
			 * \return The input handle.
			 */
			virtual OpenViBE::Plugins::IBoxContext::IInput* getInput(
				const OpenViBE::uint32 ui32InputIndex)=0;

			//@}
			/* \name Output management */
			//@{

			/**
			 * \brief Gets the output count for the related box
			 * \return The output count for the related box.
			 */
			virtual OpenViBE::uint32 getOutputCount(void) const=0;
			/**
			 * \brief Gets an output handle
			 * \param ui32OutputIndex [in] : the output index
			 * \return The output handle.
			 */
			virtual OpenViBE::Plugins::IBoxContext::IOutput* getOutput(
				const uint32 ui32OutputIndex)=0;

			//@}
			/* \name Setting management */
			//@{

			/**
			 * \brief Gets the setting count for the related box
			 * \return The setting count for the related box.
			 */
			virtual OpenViBE::uint32 getSettingCount(void) const=0;
			/**
			 * \brief Gets the setting type for the related box
			 * \param ui32SettingIndex [in] : the setting index
			 * \param rTypeIdentifier [out] : the identifier of the
			 *        requested setting.
			 * \return \e true in case of success, \e false in other cases.
			 */
			virtual OpenViBE::boolean getSettingType(
				const OpenViBE::uint32 ui32SettingIndex,
				OpenViBE::CIdentifier& rTypeIdentifier) const=0;
			/**
			 * \brief Gets the setting value for the related box
			 * \param ui32SettingIndex [in] : the setting index
			 * \param rValue [out] : the setting value
			 * \return \e true in case of success, \e false in other cases.
			 */
			virtual OpenViBE::boolean getSettingValue(
				const OpenViBE::uint32 ui32SettingIndex,
				OpenViBE::CString& sValue) const=0;

			//@}

			_IsDerivedFromClass_(OpenViBE::IObject, OV_ClassId_Plugins_BoxContext)
		};
	};
};

#endif // __OpenViBE_Plugins_IBoxContext_H__
