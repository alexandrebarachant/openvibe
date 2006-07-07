#ifndef __OpenViBE_Plugins_IBoxContext_H__
#define __OpenViBE_Plugins_IBoxContext_H__

#include "ovIPluginObjectContext.h"

namespace OpenViBE
{
	class CTimeInterval;
	class CMessageEvent;
	class CMessageSignal;

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
		protected:

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
				virtual std::string getName(void)=0;
				/**
				 * \brief Gets the type identifier this connector handles
				 * \return The type identifier this connector handles.
				 */
				virtual OpenViBE::CIdentifier getTypeIdentifier(void)=0;

				//@}

				_IsDerivedFromClass_(OpenViBE::IObject, OV_ClassIdentifier_BoxContext_ConnectorBase)
			};

		public:

			class OV_API IInputChunk : virtual public OpenViBE::Plugins::IPluginObjectContext
			{
			public:

				/** \name Simple type reading */
				//@{
				virtual OpenViBE::boolean unpack(const OpenViBE::boolean& bValue)=0;
				virtual OpenViBE::boolean unpack(const OpenViBE::uint8& ui8Value)=0;
				virtual OpenViBE::boolean unpack(const OpenViBE::uint16& ui16Value)=0;
				virtual OpenViBE::boolean unpack(const OpenViBE::uint32& ui32Value)=0;
				virtual OpenViBE::boolean unpack(const OpenViBE::uint64& ui64Value)=0;
				virtual OpenViBE::boolean unpack(const OpenViBE::int8& i8Value)=0;
				virtual OpenViBE::boolean unpack(const OpenViBE::int16& i16Value)=0;
				virtual OpenViBE::boolean unpack(const OpenViBE::int32& i32Value)=0;
				virtual OpenViBE::boolean unpack(const OpenViBE::int64& i64Value)=0;
				virtual OpenViBE::boolean unpack(const OpenViBE::float32& f32Value)=0;
				virtual OpenViBE::boolean unpack(const OpenViBE::float64& f64Value)=0;
				virtual OpenViBE::boolean unpack(const std::string& sValue)=0;
				//@}

				/** \name Array type reading */
				//@{
				virtual OpenViBE::boolean unpack(const OpenViBE::boolean** ppArray, const OpenViBE::uint32 ui32ArrayCount)=0;
				virtual OpenViBE::boolean unpack(const OpenViBE::uint8** ppArray, const OpenViBE::uint32 ui32ArrayCount)=0;
				virtual OpenViBE::boolean unpack(const OpenViBE::uint16** ppArray, const OpenViBE::uint32 ui32ArrayCount)=0;
				virtual OpenViBE::boolean unpack(const OpenViBE::uint32** ppArray, const OpenViBE::uint32 ui32ArrayCount)=0;
				virtual OpenViBE::boolean unpack(const OpenViBE::uint64** ppArray, const OpenViBE::uint32 ui32ArrayCount)=0;
				virtual OpenViBE::boolean unpack(const OpenViBE::int8** ppArray, const OpenViBE::uint32 ui32ArrayCount)=0;
				virtual OpenViBE::boolean unpack(const OpenViBE::int16** ppArray, const OpenViBE::uint32 ui32ArrayCount)=0;
				virtual OpenViBE::boolean unpack(const OpenViBE::int32** ppArray, const OpenViBE::uint32 ui32ArrayCount)=0;
				virtual OpenViBE::boolean unpack(const OpenViBE::int64** ppArray, const OpenViBE::uint32 ui32ArrayCount)=0;
				virtual OpenViBE::boolean unpack(const OpenViBE::float32** ppArray, const OpenViBE::uint32 ui32ArrayCount)=0;
				virtual OpenViBE::boolean unpack(const OpenViBE::float64** ppArray, const OpenViBE::uint32 ui32ArrayCount)=0;
				//@}

				_IsDerivedFromClass_(OpenViBE::IObject, OV_ClassIdentifier_BoxContext_InputChunk)
			};

			class OV_API IOutputChunk : virtual public OpenViBE::Plugins::IPluginObjectContext
			{
			public:

				/**
				 * \name Simple type writing
				 * \param value [in] : the value to pack in the chunk
				 * \return \e true in case of success, \e false in other cases.
				 *
				 * Those functions will pack the specified value
				 * in the chunk. The size of data to pack is
				 * chosen thanks to the type of the given value.
				 *
				 * \sa OpenViBE::Plugins::IBoxContext::IInputChunk
				 * \sa OpenViBE::Plugins::IBoxContext::IInputChunk::unpack
				 */
				//@{
				virtual OpenViBE::boolean pack(const OpenViBE::boolean value)=0;
				virtual OpenViBE::boolean pack(const OpenViBE::uint8 value)=0;
				virtual OpenViBE::boolean pack(const OpenViBE::uint16 value)=0;
				virtual OpenViBE::boolean pack(const OpenViBE::uint32 value)=0;
				virtual OpenViBE::boolean pack(const OpenViBE::uint64 value)=0;
				virtual OpenViBE::boolean pack(const OpenViBE::int8 value)=0;
				virtual OpenViBE::boolean pack(const OpenViBE::int16 value)=0;
				virtual OpenViBE::boolean pack(const OpenViBE::int32 value)=0;
				virtual OpenViBE::boolean pack(const OpenViBE::int64 value)=0;
				virtual OpenViBE::boolean pack(const OpenViBE::float32 value)=0;
				virtual OpenViBE::boolean pack(const OpenViBE::float64 value)=0;
				virtual OpenViBE::boolean pack(const std::string& value)=0;
				//@}

				/**
				 * \name Array type writing
				 * \param pArray [in] : the pointer for the array to pack
				 * \param ui32ArrayCount [in] : the number of elements
				 *        in the array
				 * \return \e true in case of success, \e false in other cases.
				 *
				 * Those functions will pack an array in the chunk. The size
				 * of data to pack is chosen thanks to the type of the
				 * pArray pointer !
				 *
				 * \sa OpenViBE::Plugins::IBoxContext::IInputChunk
				 * \sa OpenViBE::Plugins::IBoxContext::IInputChunk::unpack
				 */
				//@{
				virtual OpenViBE::boolean pack(const OpenViBE::boolean* pArray, const OpenViBE::uint32 ui32ArrayCount)=0;
				virtual OpenViBE::boolean pack(const OpenViBE::uint8* pArray, const OpenViBE::uint32 ui32ArrayCount)=0;
				virtual OpenViBE::boolean pack(const OpenViBE::uint16* pArray, const OpenViBE::uint32 ui32ArrayCount)=0;
				virtual OpenViBE::boolean pack(const OpenViBE::uint32* pArray, const OpenViBE::uint32 ui32ArrayCount)=0;
				virtual OpenViBE::boolean pack(const OpenViBE::uint64* pArray, const OpenViBE::uint32 ui32ArrayCount)=0;
				virtual OpenViBE::boolean pack(const OpenViBE::int8* pArray, const OpenViBE::uint32 ui32ArrayCount)=0;
				virtual OpenViBE::boolean pack(const OpenViBE::int16* pArray, const OpenViBE::uint32 ui32ArrayCount)=0;
				virtual OpenViBE::boolean pack(const OpenViBE::int32* pArray, const OpenViBE::uint32 ui32ArrayCount)=0;
				virtual OpenViBE::boolean pack(const OpenViBE::int64* pArray, const OpenViBE::uint32 ui32ArrayCount)=0;
				virtual OpenViBE::boolean pack(const OpenViBE::float32* pArray, const OpenViBE::uint32 ui32ArrayCount)=0;
				virtual OpenViBE::boolean pack(const OpenViBE::float64* pArray, const OpenViBE::uint32 ui32ArrayCount)=0;
				//@}

				_IsDerivedFromClass_(OpenViBE::IObject, OV_ClassIdentifier_BoxContext_OutputChunk)
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
				 * \param rpChunk [out] : a pointer to the input chunk
				 * \param rValidity [out] : the time validity for the
				 *        returned chunk.
				 * \return \e true in case of success, \e false in other case.
				 * \warning When returning \e false, both the value of
				 *          rpChunk and rValidity are undefined.
				 * \warning The chunks are ordered like they arrived
				 *          to the box, this means chunk 0 arrived
				 *          before chunk 1, that arrived before
				 *          chunk 2 and so on...
				 * \sa getChunkCount
				 * \sa releaseChunk
				 */
				virtual OpenViBE::boolean getChunk(
					const OpenViBE::uint32 ui32Index,
					OpenViBE::Plugins::IBoxContext::IInputChunk*& rpChunk,
					OpenViBE::CTimeInterval& rValidity)=0;
				/**
				 * \brief Releases an input chunk
				 * \param ui32Index [in] : the index of the chunk to release
				 * \return \e true in case of success \e false in other cases.
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
				/** \name Information related to this input */
				//@{

				/**
				 * \brief Gets the source information for this input
				 * \param rSourceIdentifier [out] : the box identifier
				 *        for this input
				 * \param rSourceOutputIndex [out] : the output index
				 *        of the box which is connected to this input
				 * \return \e true in case of success, \e false in other cases.
				 */
				virtual OpenViBE::boolean getSource(
					OpenViBE::CIdentifier& rSourceIdentifier,
					OpenViBE::uint32& rSourceOutputIndex)=0;

				//@}

				_IsDerivedFromClass_(OpenViBE::IObject, OV_ClassIdentifier_BoxContext_Input)
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
				 * \brief Gets a reference on the current output chunk
				 * \return A reference to the current output chunk
				 *
				 * This chunk should be used to effectively send
				 * data outside the concerned box. First, one should
				 * fill in the chunk. Then one should call the
				 * send method in order to effectively send the
				 * chunk data specifying its time validity.
				 *
				 * \sa send
				 */
				virtual OpenViBE::Plugins::IBoxContext::IOutputChunk& getChunk(void)=0;
				/**
				 * \brief Effectively sends the data chunk
				 * \param rTimeValidity [in] : the time validity interval 
				 *        for the output chunk.
				 * \return \e true in case of succcess, \e false in other cases.
				 *
				 * The output chunk should first be filled. For
				 * that, one will have to get a reference on it
				 * thanks to the getChunk method !
				 *
				 * \sa getChunk
				 */
				virtual OpenViBE::boolean send(
					const OpenViBE::CTimeInterval& rTimeValidity)=0;
				/**
				 * \brief Gets the target information for this output
				 * \param rTargetIdentifiers [out] : the target box
				 *        identifiers for this output
				 * \param rTargetInputIndices [out] : the input indices
				 *        of the boxes which are connected to this output
				 * \return \e true in case of success, \e false in other cases.
				 */

				//@}
				/** \name Information related to this output */
				//@{

				virtual OpenViBE::uint32 getOutputTargetCount(void)=0;

				virtual OpenViBE::boolean getOutputTarget(
					const OpenViBE::uint32 ui32OutputIndex,
					OpenViBE::CIdentifier& rTargetIdentifiers,
					OpenViBE::uint32& rTargetInputIndices)=0;

				//@}

				_IsDerivedFromClass_(OpenViBE::IObject, OV_ClassIdentifier_BoxContext_Output)
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
			virtual OpenViBE::CIdentifier getIdentifier(void)=0;
			/**
			 * \brief Gets the name of the related box
			 * \return The name of the related box.
			 */
			virtual std::string getName(void)=0;

			//@}
			/** \name Messages sending */
			//@{

			/**
			 * \brief Sends signal to the kernel
			 * \param rMessageSignal [in] : the signal message to send
			 * \return \e true in case of success, \e false in other cases.
			 */
			virtual OpenViBE::boolean sendSignal(
				const OpenViBE::CMessageSignal& rMessageSignal)=0;
			/**
			 * \brief Sends a message to another box
			 * \param rMessageEvent [in] : the message to send to the other box
			 * \param rTargetIdentifier [in] : the identifier of the other box
			 * \return \e true in case of success, \e false in other cases.
			 */
			virtual OpenViBE::boolean sendMessage(
				const OpenViBE::CMessageEvent& rMessageEvent,
				const OpenViBE::CIdentifier& rTargetIdentifier)=0;
			/**
			 * \brief Broadcasts a message to several other boxes
			 * \param rMessageEvent [in] : the message to send
			 * \param pTargetIdentifier [in] : a pointer containing all
			 *        the targets which should receive the sent message.
			 * \param ui32TargetIdentifierCount [in] : the number of
			 *        targets contained in the \c pTargetIdentifier array.
			 * \return \e true in case of success, \e false in other cases.
			 */
			virtual OpenViBE::boolean sendMessage(
				const OpenViBE::CMessageEvent& rMessageEvent,
				const OpenViBE::CIdentifier* pTargetIdentifier,
				const OpenViBE::uint32 ui32TargetIdentifierCount)=0;

			//@}
			/** \name Input management */
			//@{

			/**
			 * \brief Gets the input count for the related box
			 * \return The input count for the related box.
			 */
			virtual OpenViBE::uint32 getInputCount(void)=0;
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
			virtual OpenViBE::uint32 getOutputCount(void)=0;
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
			virtual OpenViBE::uint32 getSettingCount(void)=0;
			/**
			 * \brief Gets the setting type for the related box
			 * \param ui32SettingIndex [in] : the setting index
			 * \param rTypeIdentifier [out] : the identifier of the
			 *        requested setting.
			 * \return \e true in case of success, \e false in other cases.
			 */
			virtual OpenViBE::boolean getSettingType(
				const OpenViBE::uint32 ui32SettingIndex,
				OpenViBE::CIdentifier& rTypeIdentifier)=0;
			/**
			 * \brief Gets the setting value for the related box
			 * \param ui32SettingIndex [in] : the setting index
			 * \param rValue [out] : the setting value
			 * \return \e true in case of success, \e false in other cases.
			 */
			virtual OpenViBE::boolean getSettingValue(
				const OpenViBE::uint32 ui32SettingIndex,
				std::string& rValue)=0;

			//@}

			_IsDerivedFromClass_(OpenViBE::IObject, OV_ClassIdentifier_BoxContext)
		};
	};
};

#endif // __OpenViBE_Plugins_IBoxContext_H__
