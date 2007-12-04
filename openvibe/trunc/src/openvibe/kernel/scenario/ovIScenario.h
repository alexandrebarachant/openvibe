#ifndef __OpenViBE_Kernel_Scenario_IScenario_H__
#define __OpenViBE_Kernel_Scenario_IScenario_H__

#include "ovIAttributable.h"

namespace OpenViBE
{
	namespace Kernel
	{
		class IBox;
		class ILink;
		class IProcessingUnit;

		/**
		 * \class IScenario
		 * \author Yann Renard (IRISA/INRIA)
		 * \date 2006-08-16
		 * \brief A static §OpenViBE§ scenario
		 *
		 * This class is a static scenario description.
		 * It is used to load/save/modify an §OpenViBE§
		 * box/connection collection...
		 *
		 * \todo Add meta information for this scenario
		 */
		class OV_API IScenario : virtual public OpenViBE::Kernel::IAttributable
		{
		public:

#if 0
			class OV_API IBoxEnum
			{
			public:
				virtual ~IBoxEnum(void) { }
				virtual void preCallback(void) { }
				virtual OpenViBE::boolean callback(
					const OpenViBE::Kernel::IScenario& rScenario,
					OpenViBE::Kernel::IBox& rBox)=0;
				virtual void postCallback(void) { }
			};

			class OV_API ILinkEnum
			{
			public:
				virtual ~ILinkEnum(void) { }
				virtual void preCallback(void) { }
				virtual OpenViBE::boolean callback(
					const OpenViBE::Kernel::IScenario& rScenario,
					OpenViBE::Kernel::ILink& rLink)=0;
				virtual void postCallback(void) { }
			};
#endif

			/** \name Input / Output from files */
			//@{

			/**
			 * \brief Clears the scenario
			 * \return \e true in case of success.
			 * \return \e false in case of error.
			 */
			virtual OpenViBE::boolean clear(void)=0;
			/**
			 * \brief Loads a scenario from a file
			 * \param sFileName [in] : The file to load the scenario from
			 * \return \e true in case of success.
			 * \return \e false in case of error.
			 */
			virtual OpenViBE::boolean load(
				const OpenViBE::CString& sFileName)=0;
			/**
			 * \brief Loads a scenario from a file with a specific loader
			 * \param sFileName [in] : The file to load the scenario from
			 * \param rLoaderIdentifier [in] : The identifier of the loader to use
			 * \return \e true in case of success.
			 * \return \e false in case of error.
			 */
			virtual OpenViBE::boolean load(
				const OpenViBE::CString& sFileName,
				const OpenViBE::CIdentifier& rLoaderIdentifier)=0;
			/**
			 * \brief Saves a scenario to a file
			 * \param sFileName [in] : The file to save the scenario to
			 * \return \e true in case of success.
			 * \return \e false in case of error.
			 */
			virtual OpenViBE::boolean save(
				const OpenViBE::CString& sFileName)=0;
			/**
			 * \brief Saves a scenario to a file with a specific saver
			 * \param sFileName [in] : The file to save the scenario to
			 * \param rSaverIdentifier [in] : The identifier of the saver to use
			 * \return \e true in case of success.
			 * \return \e false in case of error.
			 */
			virtual OpenViBE::boolean save(
				const OpenViBE::CString& sFileName,
				const OpenViBE::CIdentifier& rSaverIdentifier)=0;
			/**
			 * \brief Merges a scenario into the current scenario
			 * \param rScenario [in] : The scenario to merge in
			 *        this scenario
			 * \return \e true in case of success.
			 * \return \e false in case of error.
			 * \warning Source scenario are ignored, only boxes
			 *          and links are effectively merged
			 */
			virtual OpenViBE::boolean merge(
				const OpenViBE::Kernel::IScenario& rScenario)=0;
			/**
			 * \brief Sets identifier of IVisualisationTree object corresponding to this scenario
			 * \param rVisualisationTreeIdentifier [in] : identifier to be stored
			 */
			virtual OpenViBE::boolean setVisualisationTreeIdentifier(
				const OpenViBE::CIdentifier& rVisualisationTreeIdentifier)=0;
			/**
			 * \brief Returns identifier of IVisualisationTree object corresponding to this scenario
			 */
			virtual OpenViBE::CIdentifier getVisualisationTreeIdentifier(void) const=0;

			//@{
			/** \name Box management */
			//@{

#if 0
			/**
			 * \brief Enumerates all the boxes of this scenario
			 * \param rCallback [in] : The user callback
			 *        to use for each found box in the scenario
			 * \return \e true in case of success.
			 * \return \e false in case of error.
			 */
			virtual OpenViBE::boolean enumerateBoxes(
				OpenViBE::Kernel::IScenario::IBoxEnum& rCallback) const=0;
#endif

			/**
			 * \brief Gets next box identifier
			 * \param rPreviousIdentifier [in] : The identifier
			 *        for the preceeding box
			 * \return The identifier of the next box in case of success.
			 * \return \c OV_UndefinedIdentifier on error.
			 * \note Giving \c OV_UndefinedIdentifier as \c rPreviousIdentifier
			 *       will cause this function to return the first box
			 *       identifier.
			 */
			virtual OpenViBE::CIdentifier getNextBoxIdentifier(
				const OpenViBE::CIdentifier& rPreviousIdentifier) const=0;
			/**
			 * \brief Gets next box identifier running on a given process unit
			 * \param rPreviousIdentifier [in] : The identifier
			 *        for the preceeding box
			 * \param rProcessingUnitIdentifier [in] : The identifier
			 *        for the processing unit which the box should
			 *        be ran on.
			 * \return The identifier of the next box in case of success.
			 * \return \c OV_UndefinedIdentifier on error.
			 * \note Giving \c OV_UndefinedIdentifier as \c rPreviousIdentifier
			 *       will cause this function to return the first box
			 *       identifier.
			 * \note Giving \c OV_UndefinedIdentifier as \c rProcessingUnitIdentifier
			 *       will cause this function to return an unaffected box
			 *       identifier.
			 */
			virtual OpenViBE::CIdentifier getNextBoxIdentifierOnProcessingUnit(
				const OpenViBE::CIdentifier& rPreviousIdentifier,
				const OpenViBE::CIdentifier& rProcessingUnitIdentifier) const=0;
			/**
			 * \brief Tests whether a given identifier is a box or not
			 * \param rBoxIdentifier [in] : the identifier to test
			 * \return \e true if the identified object is a box
			 * \return \e false if the identified object is not a box
			 * \note Requesting a bad identifier returns \e false
			 */
			virtual OpenViBE::boolean isBox(
				const OpenViBE::CIdentifier& rBoxIdentifier) const=0;
			/**
			 * \brief Gets the details for a specific box
			 * \param rBoxIdentifier [in] : The identifier
			 *        of the box which details should be
			 *        sent.
			 * \return The box details
			 */
			virtual const OpenViBE::Kernel::IBox* getBoxDetails(
				const OpenViBE::CIdentifier& rBoxIdentifier) const=0;
			/// \copydoc getBoxDetails(const OpenViBE::CIdentifier&)const
			virtual OpenViBE::Kernel::IBox* getBoxDetails(
				const OpenViBE::CIdentifier& rBoxIdentifier)=0;
			/**
			 * \brief Adds a new box in the scenario
			 * \param rBoxIdentifier [out] : The identifier of
			 *        the created box
			 * \return \e true in case of success.
			 * \return \e false in case of error. In such case,
			 *         \c rBoxIdentifier remains unchanged.
			 * \note This produces an empty and unconfigured box !
			 */
			virtual OpenViBE::boolean addBox(
				OpenViBE::CIdentifier& rBoxIdentifier)=0;
			/**
			 * \brief Adds a new box in the scenario based on an existing box
			 * \param rBox [in] : the box to copy in this scenario
			 * \param rBoxIdentifier [out] : The identifier of
			 *        the created box
			 * \return \e true in case of success.
			 * \return \e false in case of error. In such case,
			 *         \c rBoxIdentifier remains unchanged.
			 */
			virtual OpenViBE::boolean addBox(
				const OpenViBE::Kernel::IBox& rBox,
				OpenViBE::CIdentifier& rBoxIdentifier)=0;
			/**
			 * \brief Adds a new box in the scenario
			 * \param rBoxAlgorithmClassIdentifier [in] : The
			 *        class identifier of the algorithm for
			 *        this box
			 * \param rBoxIdentifier [out] : The identifier of
			 *        the created box
			 * \return \e true in case of success.
			 * \return \e false in case of error. In such case,
			 *         \c rBoxIdentifier remains unchanged.
			 * \note This function prepares the box according
			 *       to its algorithm class identifier !
			 */
			virtual OpenViBE::boolean addBox(
				const OpenViBE::CIdentifier& rBoxAlgorithmClassIdentifier,
				OpenViBE::CIdentifier& rBoxIdentifier)=0;
			/**
			 * \brief Removes a box of the scenario
			 * \param rBoxIdentifier [in] : The box identifier
			 * \return \e true in case of success.
			 * \return \e false in case of error.
			 * \note Each link related to this box is also removed
			 */
			virtual OpenViBE::boolean removeBox(
				const OpenViBE::CIdentifier& rBoxIdentifier)=0;

			//@}
			/** \name Connection management */
			//@{

#if 0
			/**
			 * \brief Enumerates all the links of this scenario
			 * \param rCallback [in] : The user callback
			 *        to use for each found link in the scenario
			 * \return \e true in case of success.
			 * \return \e false in case of error.
			 */
			virtual OpenViBE::boolean enumerateLinks(
				OpenViBE::Kernel::IScenario::ILinkEnum& rCallback) const=0;
			/**
			 * \brief Enumerates the links of this scenario starting from a given box
			 * \param rCallback [in] : The user callback
			 *        to use for each found link in the scenario
			 * \param rBoxIdentifier [in] : The box identifier
			 *        which the link should start from
			 * \return \e true in case of success.
			 * \return \e false in case of error.
			 */
			virtual OpenViBE::boolean enumerateLinksFromBox(
				OpenViBE::Kernel::IScenario::ILinkEnum& rCallback,
				const OpenViBE::CIdentifier& rBoxIdentifier) const=0;
			/**
			 * \brief Enumerates the links of this scenario ending to a given box
			 * \param rCallback [in] : The user callback
			 *        to use for each found link in the scenario
			 * \param rBoxIdentifier [in] : The box identifier
			 *        which the link should end to
			 * \return \e true in case of success.
			 * \return \e false in case of error.
			 */
			virtual OpenViBE::boolean enumerateLinksToBox(
				OpenViBE::Kernel::IScenario::ILinkEnum& rCallback,
				const OpenViBE::CIdentifier& rBoxIdentifier) const=0;
#endif

			/**
			 * \brief Gets next link identifier
			 * \param rPreviousIdentifier [in] : The identifier
			 *        for the preceeding link
			 * \return The identifier of the next link in case of success.
			 * \return \c OV_UndefinedIdentifier on error.
			 * \note Giving \c OV_UndefinedIdentifier as \c rPreviousIdentifier
			 *       will cause this function to return the first link
			 *       identifier.
			 */
			virtual OpenViBE::CIdentifier getNextLinkIdentifier(
				const OpenViBE::CIdentifier& rPreviousIdentifier) const=0;
			/**
			 * \brief Gets next link identifier from fixed box
			 * \param rPreviousIdentifier [in] : The identifier
			 *        for the preceeding link
			 * \param rBoxIdentifier [in] : The box identifier
			 *        which the link should end to
			 * \return The identifier of the next link in case of success.
			 * \return \c OV_UndefinedIdentifier on error.
			 * \note Giving \c OV_UndefinedIdentifier as \c rPreviousIdentifier
			 *       will cause this function to return the first link
			 *       identifier.
			 */
			virtual OpenViBE::CIdentifier getNextLinkIdentifierFromBox(
				const OpenViBE::CIdentifier& rPreviousIdentifier,
				const OpenViBE::CIdentifier& rBoxIdentifier) const=0;
			/**
			 * \brief Gets next link identifier from fixed box output
			 * \param rPreviousIdentifier [in] : The identifier
			 *        for the preceeding link
			 * \param rBoxIdentifier [in] : The box identifier
			 *        which the link should end to
			 * \param ui32OutputIndex [in] : The input index
			 *        which the link should end to
			 * \return The identifier of the next link in case of success.
			 * \return \c OV_UndefinedIdentifier on error.
			 * \note Giving \c OV_UndefinedIdentifier as \c rPreviousIdentifier
			 *       will cause this function to return the first link
			 *       identifier.
			 */
			virtual OpenViBE::CIdentifier getNextLinkIdentifierFromBoxOutput(
				const OpenViBE::CIdentifier& rPreviousIdentifier,
				const OpenViBE::CIdentifier& rBoxIdentifier,
				const OpenViBE::uint32 ui32OutputIndex) const=0;
			/**
			 * \brief Gets next link identifier from fixed box
			 * \param rPreviousIdentifier [in] : The identifier
			 *        for the preceeding link
			 * \param rBoxIdentifier [in] : The box identifier
			 *        which the link should start from
			 * \return The identifier of the next link in case of success.
			 * \return \c OV_UndefinedIdentifier on error.
			 * \note Giving \c OV_UndefinedIdentifier as \c rPreviousIdentifier
			 *       will cause this function to return the first link
			 *       identifier.
			 */
			virtual OpenViBE::CIdentifier getNextLinkIdentifierToBox(
				const OpenViBE::CIdentifier& rPreviousIdentifier,
				const OpenViBE::CIdentifier& rBoxIdentifier) const=0;
			/**
			 * \brief Gets next link identifier from fixed box input
			 * \param rPreviousIdentifier [in] : The identifier
			 *        for the preceeding link
			 * \param rBoxIdentifier [in] : The box identifier
			 *        which the link should start from
			 * \param ui32InputInex [in] : The input index
			 *        which the link should start from
			 * \return The identifier of the next link in case of success.
			 * \return \c OV_UndefinedIdentifier on error.
			 * \note Giving \c OV_UndefinedIdentifier as \c rPreviousIdentifier
			 *       will cause this function to return the first link
			 *       identifier.
			 */
			virtual OpenViBE::CIdentifier getNextLinkIdentifierToBoxInput(
				const OpenViBE::CIdentifier& rPreviousIdentifier,
				const OpenViBE::CIdentifier& rBoxIdentifier,
				const OpenViBE::uint32 ui32InputInex) const=0;
			/**
			 * \brief Tests whether a given identifier is a link or not
			 * \param rIdentifier [in] : the identifier to test
			 * \return \e true if the identified object is a link
			 * \return \e false if the identified object is not a link
			 * \note Requesting a bad identifier returns \e false
			 */
			virtual OpenViBE::boolean isLink(
				const OpenViBE::CIdentifier& rIdentifier) const=0;
			/**
			 * \brief Gets the details for a specific link
			 * \param rLinkIdentifier [in] : The identifier
			 *        of the link which details should be
			 *        sent.
			 * \return The link details
			 */
			virtual const OpenViBE::Kernel::ILink* getLinkDetails(
				const OpenViBE::CIdentifier& rLinkIdentifier) const=0;
			/// \copydoc getLinkDetails(const OpenViBE::CIdentifier&)const
			virtual OpenViBE::Kernel::ILink* getLinkDetails(
				const OpenViBE::CIdentifier& rLinkIdentifier)=0;
			/**
			 * \brief Creates a connection between two boxes
			 * \param rSourceBoxIdentifier [in] : The source
			 *        box identifier
			 * \param ui32SourceBoxOutputIndex [in] : The output
			 *        index for the given source box
			 * \param rTargetBoxIdentifier [in] : The target
			 *        box identifier
			 * \param ui32TargetBoxInputIndex [in] : The input
			 *        index for the given target box
			 * \param rLinkIdentifier [out] : The created link
			 *        identifier.
			 * \return \e true in case of success.
			 * \return \e false in case of error. In such case,
			 *         rLinkIdentifier remains unchanged.
			 */
			virtual OpenViBE::boolean connect(
				const OpenViBE::CIdentifier& rSourceBoxIdentifier,
				const OpenViBE::uint32 ui32SourceBoxOutputIndex,
				const OpenViBE::CIdentifier& rTargetBoxIdentifier,
				const OpenViBE::uint32 ui32TargetBoxInputIndex,
				OpenViBE::CIdentifier& rLinkIdentifier)=0;
			/**
			 * \brief Deletes a connection between two boxes
			 * \param rSourceBoxIdentifier [in] : The source
			 *        box identifier
			 * \param ui32SourceBoxOutputIndex [in] : The output
			 *        index for the given source box
			 * \param rTargetBoxIdentifier [in] : The target
			 *        box identifier
			 * \param ui32TargetBoxInputIndex [in] : The input
			 *        index for the given target box
			 * \return \e true in case of success.
			 * \return \e false in case of error.
			 */
			virtual OpenViBE::boolean disconnect(
				const OpenViBE::CIdentifier& rSourceBoxIdentifier,
				const OpenViBE::uint32 ui32SourceBoxOutputIndex,
				const OpenViBE::CIdentifier& rTargetBoxIdentifier,
				const OpenViBE::uint32 ui32TargetBoxInputIndex)=0;
			/**
			 * \brief Deletes a connection between two boxes
			 * \param rLinkIdentifier [out] : The identifier
			 *        for the link to be deleted
			 * \return \e true in case of success.
			 * \return \e false in case of error.
			 */
			virtual OpenViBE::boolean disconnect(
				const OpenViBE::CIdentifier& rLinkIdentifier)=0;

			//@}
			/** \name Processing units management */
			//@{

			virtual OpenViBE::CIdentifier getNextProcessingUnitIdentifier(
				const OpenViBE::CIdentifier& rPreviousIdentifier) const=0;
			virtual OpenViBE::boolean isProcessingUnit(
				const OpenViBE::CIdentifier& rIdentifier) const=0;
			virtual const OpenViBE::Kernel::IProcessingUnit* getProcessingUnitDetails(
				const OpenViBE::CIdentifier& rProcessingUnitIdentifier) const=0;
			virtual OpenViBE::Kernel::IProcessingUnit* getProcessingUnitDetails(
				const OpenViBE::CIdentifier& rProcessingUnitIdentifier)=0;
			virtual OpenViBE::boolean addProcessingUnit(
				OpenViBE::CIdentifier& rProcessingUnitIdentifier)=0;
			virtual OpenViBE::boolean removeProcessingUnit(
				const OpenViBE::CIdentifier& rProcessingUnitIdentifier)=0;

			//@}

			_IsDerivedFromClass_(OpenViBE::Kernel::IAttributable, OV_ClassId_Kernel_Scenario_Scenario)
		};
	};
};

#endif // __OpenViBE_Kernel_Scenario_IScenario_H__
