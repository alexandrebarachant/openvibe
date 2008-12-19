#ifndef __OpenViBE_Kernel_Visualisation_CVisualisationContext_H__
#define __OpenViBE_Kernel_Visualisation_CVisualisationContext_H__

#include "../ovkTKernelObject.h"

namespace OpenViBE
{
	namespace Kernel
	{
		/**
		 * \class CVisualisationContext
		 * \author Vincent Delannoy (INRIA/IRISA)
		 * \date 2008-06
		 * \brief Visualisation context made available to plugins and allowing them to interact with a 3D scene
		 * This class offers a simplified, library independent 3D API to be used by plugin developers.
		 */
		class CVisualisationContext : public OpenViBE::Kernel::TKernelObject<OpenViBE::Kernel::IVisualisationContext>
		{
		public:
			/**
			 * \brief Constructor
			 */
			CVisualisationContext(const OpenViBE::Kernel::IKernelContext& rKernelContext, OpenViBE::Kernel::CSimulatedBox* pSimulatedBox);

			/**
			 * \brief Destructor
			 */
			virtual ~CVisualisationContext(void);

			/**
			 * \brief Set toolbar widget used by a plugin
			 * \param pToolbarWidget Pointer to toolbar widget
			 * \return True if widget was successfully set, false otherwise
			 */
			virtual OpenViBE::boolean setToolbar(
				::GtkWidget* pToolbarWidget);

			/**
			 * \brief Set topmost widget used by a plugin
			 * \param pTopmostWidget Pointer to topmost widget
			 * \return True if widget was successfully set, false otherwise
			 */
			virtual OpenViBE::boolean setWidget(
				::GtkWidget* pTopmostWidget);

			/**
			 * \brief Create a 3D widget
			 * \param[out] p3DWidget Pointer to a 3D widget
			 * \return True if 3D widget was successfully created, false otherwise
			 */
			virtual OpenViBE::CIdentifier create3DWidget(
				::GtkWidget*& p3DWidget);

			/**
			 * \brief Request 3D widget realization status
			 * \param[in] rWidgetIdentifier identifier of the 3D widget which has to be tested
			 * \return True if the widget has been realized.
			 * \return False if the widget has not been realized.
			 */
			virtual OpenViBE::boolean is3DWidgetRealized(
				const OpenViBE::CIdentifier& rWidgetIdentifier) const;

			/**
			 * \brief Create a resource group
			 * \param [out] rResourceGroupIdentifier Identifier of resource group created
			 * \param [in] rResourceGroupName Resource group name
			 * \return True if resource group could be created, false otherwise
			 */
			virtual OpenViBE::boolean createResourceGroup(
				OpenViBE::CIdentifier& rResourceGroupIdentifier,
				const OpenViBE::CString& rResourceGroupName);

			/**
			 * \brief Add a resource location
			 * \remark Resources can't be added once Resource Group has been initialized
			 * \param rResourceGroupIdentifier Resource group identifier
			 * \param rPath Resources location path
			 * \param type Resources type (path or zip file)
			 * \param bRecursive Resources are added recursively from location passed in parameter if true
			 * \return True if resource location could be added, false otherwise
			 */
			virtual OpenViBE::boolean addResourceLocation(
				const OpenViBE::CIdentifier& rResourceGroupIdentifier,
				const OpenViBE::CString& rPath,
				OpenViBE::Kernel::EResourceType type,
				bool bRecursive);

			/**
			 * \brief Add resource locations from a file
			 * \remark Resources can't be added once Resource Group has been initialized
			 * \param rResourceGroupIdentifier Resource group identifier
			 * \param rResourcesFileName Resources file in OV format
			 * \return True if location could be added, false otherwise
			 */
			virtual OpenViBE::boolean addResourceLocationFromFile(
				const OpenViBE::CIdentifier& rResourceGroupIdentifier,
				const OpenViBE::CString& rResourcesFileName);

			/**
			 * \brief Initialize resource group
			 * \remark Each call to this method increments the group's internal reference counter.
			 * When it is set to one (upon first call to this method) the group is actually initialized, but isn't upon
			 * subsequent calls. It has to be destroyed (its ref count set back to 0) and recreated to be reinitialized.
			 * \param rResourceGroupIdentifier Resource group identifier
			 * \return True if resource group could be initialized (or was initialized already), false if an error occured
			 */
			virtual OpenViBE::boolean initializeResourceGroup(
				const OpenViBE::CIdentifier& rResourceGroupIdentifier);

			/**
			 * \brief Destroy resource group
			 * \remark Each call to this method decrements the group's internal reference counter.
			 * Only when this counter reaches 0 is the group actually destroyed.
			 * \param rResourceGroupIdentifier Resource group identifier
			 * \return True if resource group could be destroyed (or was initialized already), false if an error occured
			 */
			virtual OpenViBE::boolean destroyResourceGroup(
				const OpenViBE::CIdentifier& rResourceGroupIdentifier);

			/**
			 * \brief Update 3D window
			 * \param rWidgetIdentifier Identifier of 3D widget to be refreshed
			 * \return True if widget could be refreshed
			 */
			virtual OpenViBE::boolean update3DWidget(
				const OpenViBE::CIdentifier& rWidgetIdentifier);

			/**
			 * \brief Set background color of a 3D widget
			 * \param rWidgetIdentifier Identifier of 3D widget
			 * \param f32ColorRed Red component (0 to 1)
			 * \param f32ColorGreen Green component (0 to 1)
			 * \param f32ColorBlue Blue component (0 to 1)
			 * \return True if background color could be set, false otherwise
			 */
			virtual OpenViBE::boolean setBackgroundColor(
				const OpenViBE::CIdentifier& rWidgetIdentifier,
				OpenViBE::float32 f32ColorRed,
				OpenViBE::float32 f32ColorGreen,
				OpenViBE::float32 f32ColorBlue);

			/**
			 * \brief Set camera position so that it encompasses all scene objects
			 * \remark This method also sets translation speed so that it is proportionate
			 * to scene size
			 * \param rWindowIdentifier Identifier of 3D widget whose camera is to be set
			 * \return True if camera could be set, false otherwise
			 */
			virtual OpenViBE::boolean setCameraToEncompassObjects(
				const OpenViBE::CIdentifier& rWidgetIdentifier);

			/**
			 * \brief Create a new 3D object
			 * \param rObjectFileName Filename (without extension) from which to load object
			 * \param pObjectParams Optional object parameters
			 * \return Identifier of newly created object, or OV_UndefinedIdentifier if failed
			 */
			virtual OpenViBE::CIdentifier createObject(
				const OpenViBE::CString& rObjectFileName,
				const CNameValuePairList* pObjectParams);

			/**
			 * \brief Create a new 3D object
			 * \param eStandard3DObject Enumeration member of standard object to create
			 * \param pObjectParams Optional object parameters
			 * \return Identifier of newly created object, or OV_UndefinedIdentifier if failed
			 */
			virtual OpenViBE::CIdentifier createObject(
				const OpenViBE::Kernel::EStandard3DObject eStandard3DObject,
				const CNameValuePairList* pObjectParams);

			/**
			 * \brief Remove a 3D object
			 * \param rIdentifier Identifier of object to remove from 3D scene
			 * \return True if object was successfully removed, false otherwise
			 */
			virtual OpenViBE::boolean removeObject(
				const OpenViBE::CIdentifier& rIdentifier);

			/**
			 * \brief Set visibility of a 3D object
			 * \param rIdentifier Identifier of object which visibility is to be set
			 * \param bVisible Visibility flag
			 * \return True if object visibility could be set, false otherwise
			 */
			virtual OpenViBE::boolean setObjectVisible(
				const OpenViBE::CIdentifier& rIdentifier,
				OpenViBE::boolean bVisible);

			/**
			 * \brief Set the position of an object
			 * \param rIdentifier Identifier of the object the position of which is to be set
			 * \param f32PositionX X coordinate of position
			 * \param f32PositionY Y coordinate of position
			 * \param f32PositionZ Z coordinate of position
			 * \return True if object position could be set, false otherwise
			 */
			virtual OpenViBE::boolean setObjectPosition(
				const OpenViBE::CIdentifier& rIdentifier,
				OpenViBE::float32 f32PositionX,
				OpenViBE::float32 f32PositionY,
				OpenViBE::float32 f32PositionZ);

			/**
			 * \brief Set the orientation of an object
			 * \param rIdentifier Identifier of the object the orientation of which is to be set
			 * \param f32OrientationX X component of a quaternion
			 * \param f32OrientationY Y component of a quaternion
			 * \param f32OrientationZ Z component of a quaternion
			 * \param f32OrientationW W component of a quaternion
			 * \return True if orientation could be set, false otherwise
			 */
			virtual OpenViBE::boolean setObjectOrientation(
				const OpenViBE::CIdentifier& rIdentifier,
				OpenViBE::float32 f32OrientationX,
				OpenViBE::float32 f32OrientationY,
				OpenViBE::float32 f32OrientationZ,
				OpenViBE::float32 f32OrientationW);

			/**
			 * \brief Set the scale of an object
			 * \param rIdentifier Identifier of the object the scale of which is to be set
			 * \param f32ScaleX X component of scale
			 * \param f32ScaleY Y component of scale
			 * \param f32ScaleZ Z component of scale
			 * \return True if scale could be set, false otherwise
			 */
			virtual OpenViBE::boolean setObjectScale(
				const OpenViBE::CIdentifier& rIdentifier,
				OpenViBE::float32 f32ScaleX,
				OpenViBE::float32 f32ScaleY,
				OpenViBE::float32 f32ScaleZ);

			/**
			 * \brief Set the uniform scale of an object
			 * \param rIdentifier Identifier of the object the scale of which is to be set
			 * \param f32Scale Uniform scale to be set
			 * \return True if uniform scale could be set, false otherwise
			 */
			virtual OpenViBE::boolean setObjectScale(
				const OpenViBE::CIdentifier& rIdentifier,
				OpenViBE::float32 f32Scale);

			/**
			 * \brief Set the color of an object
			 * \param rIdentifier Identifier of the object the color of which is to be set
			 * \param f32ColorRed Red component (0 to 1)
			 * \param f32ColorGreen Green component (0 to 1)
			 * \param f32ColorBlue Blue component (0 to 1)
			 * \return True if color could be set, false otherwise
			 */
			virtual OpenViBE::boolean setObjectColor(
				const OpenViBE::CIdentifier& rIdentifier,
				OpenViBE::float32 f32ColorRed,
				OpenViBE::float32 f32ColorGreen,
				OpenViBE::float32 f32ColorBlue);

			/**
			 * \brief Set the transparency of an object
			 * \param rIdentifier Identifier of the object the transparency of which is to be set
			 * \param f32Transparency Transparency (0 to 1, 1 being totally transparent)
			 * \return True if transparency could be set, false otherwise
			 */
			virtual OpenViBE::boolean setObjectTransparency(
				const OpenViBE::CIdentifier& rIdentifier,
				OpenViBE::float32 f32Transparency);

			virtual OpenViBE::boolean setObjectVertexCount(
				const OpenViBE::CIdentifier& rIdentifier,
				const OpenViBE::uint32 ui32VertexCount);

			virtual OpenViBE::boolean setObjectVertexPositionArray(
				const OpenViBE::CIdentifier& rIdentifier,
				const OpenViBE::float32* pVertexPositionArray);

			/**
			 * \brief Set vertex colors of an object
			 * \remark This method applies vertex colors to the first mesh of the first
			 * Ogre::Entity encountered in the object hierarchy only. For it to work, the
			 * Ogre::Mesh needs to have been flagged as using vertex colors in authoring tools.
			 * \param rIdentifier Identifier of the object whose vertex colors are to be set
			 * \param ui32VertexColorCount Number of colors contained in pVertexColorArray
			 * \param pVertexColorArray Array of color triplets
			 * \return True if colors could be set, false otherwise
			 */
			virtual OpenViBE::boolean setObjectVertexColorArray(
				const OpenViBE::CIdentifier& rIdentifier,
				const uint32 ui32VertexColorCount,
				const OpenViBE::float32* pVertexColorArray);

			virtual OpenViBE::boolean setObjectTriangleCount(
				const OpenViBE::CIdentifier& rIdentifier,
				const OpenViBE::uint32 ui32TriangleCount);

			virtual OpenViBE::boolean setObjectTriangleIndexArray(
				const OpenViBE::CIdentifier& rIdentifier,
				const OpenViBE::uint32* pTriangleIndexArray);

			// =====================================

			virtual OpenViBE::boolean getObjectAxisAlignedBoundingBox(
				const OpenViBE::CIdentifier& rIdentifier,
				OpenViBE::float32* pMinimum,
				OpenViBE::float32* pMaximum);

			/**
			 * \brief Get object vertex count
			 * \remark This method returns the number of vertices in the first mesh of the first
			 * Ogre::Entity encountered in the object hierarchy only.
			 * \param rIdentifier Identifier of the object whose vertex count is to be retrieved
			 * \param ui32VertexCount Number of vertices found in first mesh of first entity
			 * \return True if vertex count could be retrieved, false otherwise
			 */
			virtual OpenViBE::boolean getObjectVertexCount(
				const OpenViBE::CIdentifier& rIdentifier,
				OpenViBE::uint32& ui32VertexCount) const;

			/**
			 * \brief Get vertex position array
			 * \remark This method returns vertex positions of the first mesh of the first
			 * Ogre::Entity encountered in the object hierarchy only.
			 * \param rIdentifier Identifier of the object whose vertex positions are to be retrieved
			 * \param pVertexPositionArray Preallocated array destined to store vertex position triplets
			 * \return True if vertex positions could be retrieved, false otherwise
			 */
			virtual OpenViBE::boolean getObjectVertexPositionArray(
				const OpenViBE::CIdentifier& rIdentifier,
				const OpenViBE::uint32 ui32VertexColorCount,
				OpenViBE::float32* pVertexPositionArray) const;

			virtual OpenViBE::boolean getObjectVertexColorArray(
				const OpenViBE::CIdentifier& rIdentifier,
				OpenViBE::float32* pVertexColorArray) const;

			virtual OpenViBE::boolean getObjectTriangleCount(
				const OpenViBE::CIdentifier& rIdentifier,
				OpenViBE::uint32& ui32TriangleCount) const;

			virtual OpenViBE::boolean getObjectTriangleIndexArray(
				const OpenViBE::CIdentifier& rIdentifier,
				OpenViBE::uint32* pTriangleIndexArray) const;

			_IsDerivedFromClass_Final_(OpenViBE::Kernel::TKernelObject<OpenViBE::Kernel::IVisualisationContext>, OVK_ClassId_Kernel_Visualisation_VisualisationContext)

		protected:

			OpenViBE::Kernel::CSimulatedBox* m_pSimulatedBox;
			OpenViBE::Kernel::IVisualisationManager* m_pVisualisationManagerBridge;
		};
	};
};

#endif // __OpenViBE_Kernel_Visualisation_CVisualisationContext_H__
