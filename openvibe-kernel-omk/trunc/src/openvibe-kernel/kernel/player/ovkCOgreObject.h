#ifndef __OpenViBEKernel_Kernel_Player_COgreObject_H__
#define __OpenViBEKernel_Kernel_Player_COgreObject_H__

#include <openvibe/ov_all.h>

namespace Ogre
{
	class SceneNode;
	class SceneManager;
};

class COgreVisualisation;
class NodeMaterialOrig;
class CColorAnimator;
class CTransformAnimator;
class CVertexBufferAnimator;

/**
 * \class COgreObject
 * \author Vincent Delannoy (INRIA/IRISA)
 * \date 2008-06
 * \brief An OgreObject manages a 3D object in an Ogre scene
 */
class COgreObject
{
public:
	/**
	 * \brief Constructor
	 * \param[in] rKernelContext OpenViBE kernel context
	 * \param[in] rName Object name
	 * \param[in] pOgreVis pointer to OgreVisualisation object
	 * \param[in] pSceneManager pointer to scene manager this object is to be added to
	 * \param[in] rGeometryfileName name of file containing this object's geometry
	 */
	COgreObject(
		const OpenViBE::Kernel::IKernelContext& rKernelContext,
		const OpenViBE::CString& rName,
		COgreVisualisation* pOgreVis,
		Ogre::SceneManager* pSceneManager,
		const OpenViBE::CString& rGeometryFileName);

	/// Destructor
	~COgreObject();

	/// Load the geometry using the dotSceneInterface
	OpenViBE::boolean loadGeometry();

	/// Return object name
	const OpenViBE::CString& getName();

	/// Return the node to change the materials
  NodeMaterialOrig* getNodeMaterialOrig() ;

	/// Retrieve scene node corresponding to this object
	Ogre::SceneNode& getSceneNode() const;

	/**
	 * \brief Set the color of an object
	 * \param f32ColorRed Red component (0 to 1)
	 * \param f32ColorGreen Green component (0 to 1)
	 * \param f32ColorBlue Blue component (0 to 1)
	 * \return True if color could be set, false otherwise
	 */
	OpenViBE::boolean setDiffuseColor(
		Ogre::ColourValue diffuse);

	/**
	 * \brief Set the transparency of an object
	 * \param f32Transparency Transparency (0 to 1, 1 being totally transparent)
	 * \return True if transparency could be set, false otherwise
	 */
	OpenViBE::boolean setTransparency(
		Ogre::Real transparency);

	/**
	 * \brief Set the position of an object
	 * \param f32PositionX X coordinate of position
	 * \param f32PositionY Y coordinate of position
	 * \param f32PositionZ Z coordinate of position
	 * \return True if object position could be set, false otherwise
	 */
	OpenViBE::boolean setPosition(
		Ogre::Real positionX,
		Ogre::Real positionY,
		Ogre::Real positionZ);

	/**
	 * \brief Set the orientation of an object
	 * \return True if orientation could be set, false otherwise
	 */
	OpenViBE::boolean setRotation(
		Ogre::Real rotationX,
		Ogre::Real rotationY,
		Ogre::Real rotationZ);

	/**
	 * \brief Set the orientation of an object
	 * \param f32OrientationX X component of a quaternion
	 * \param f32OrientationY Y component of a quaternion
	 * \param f32OrientationZ Z component of a quaternion
	 * \param f32OrientationW W component of a quaternion
	 * \return True if orientation could be set, false otherwise
	 */
	OpenViBE::boolean setRotation(
		Ogre::Real rotationX,
		Ogre::Real rotationY,
		Ogre::Real rotationZ,
		Ogre::Real rotationW);

	/**
	 * \brief Set the scale of an object
	 * \param f32ScaleX X component of scale
	 * \param f32ScaleY Y component of scale
	 * \param f32ScaleZ Z component of scale
	 * \return True if scale could be set, false otherwise
	 */
	OpenViBE::boolean setScale(
		Ogre::Real scaleX,
		Ogre::Real scaleY,
		Ogre::Real scaleZ);

	/**
	 * \brief Set vertex colors of an object
	 * \remark This method applies vertex colors to the first mesh of the first
	 * Ogre::Entity encountered in the object hierarchy only. For it to work, the
	 * Ogre::Mesh needs to have been flagged as using vertex colors in authoring tools.
	 * \param ui32VertexColorCount Number of colors contained in pVertexColorArray
	 * \param pVertexColorArray Array of color triplets
	 * \return True if colors could be set, false otherwise
	 */
	OpenViBE::boolean setVertexColorArray(
		Ogre::uint32 ui32VertexColorCount,
		Ogre::Real* pVertexColorArray);

	/**
	 * \brief Get object vertex count
	 * \remark This method returns the number of vertices in the first mesh of the first
	 * Ogre::Entity encountered in the object hierarchy only.
	 * \param ui32VertexCount Number of vertices found in first mesh of first entity
	 * \return True if vertex count could be retrieved, false otherwise
	 */
	OpenViBE::boolean getVertexCount(
		Ogre::uint32& ui32VertexCount) const;

	/**
	 * \brief Get vertex position array
	 * \remark This method returns vertex positions of the first mesh of the first
	 * Ogre::Entity encountered in the object hierarchy only.
	 * \param pVertexPositionArray Preallocated array destined to store vertex position triplets
	 * \return True if vertex positions could be retrieved, false otherwise
	 */
	OpenViBE::boolean getVertexPositionArray(
		Ogre::uint32 ui32VertexPositionCount,
		Ogre::Real* pVertexPositionArray) const;

	/**
	 * \brief Get world bounding box of an object
	 * \param rMinX X lower bound
	 * \param rMinY Y lower bound
	 * \param rMinZ Z lower bound
	 * \param rMaxX X upper bound
	 * \param rMaxY Y upper bound
	 * \param rMaxZ Z upper bound
	 * \return True if bounding box could be retrieved, false otherwise
	 */
	OpenViBE::boolean getWorldBoundingBox(
		Ogre::Real& rMinX,
		Ogre::Real& rMinY,
		Ogre::Real& rMinZ,
		Ogre::Real& rMaxX,
		Ogre::Real& rMaxY,
		Ogre::Real& rMaxZ);

private:
	/**
	 * \brief Retrieve first Entity in Ogre hierarchy starting at pNode
	 * \param pNode Pointer to an Ogre node from which to start looking for an Entity
	 * \param pEntity Pointer to first Entity found in an Ogre hierarchy
	 * \return True if an Entity could be found (pEntity then points to it), false otherwise
	 */
	OpenViBE::boolean getFirstEntityInHierarchy(
		Ogre::SceneNode* pNode,
		Ogre::Entity*& pEntity) const;

	/**
	 * \brief Retrieve first SubMesh referenced in an Entity
	 * \param pEntity Pointer to an entity
	 * \param pSubMesh Pointer to first SubMesh referenced in pEntity
	 * \return True if a SubMesh could be found (pSubMesh then points to it), false otherwise
	 */
	OpenViBE::boolean getFirstSubMesh(
		Ogre::Entity* pEntity,
		Ogre::SubMesh*& pSubMesh) const;

private:
	/// OpenViBE kernel context
	const OpenViBE::Kernel::IKernelContext& m_rKernelContext;

	// name of the visual object
	OpenViBE::CString m_sName;

	/// reference to the associated visualisation
	COgreVisualisation* m_pOgreVis;

	/// root node of the loaded geometry
	Ogre::SceneNode* m_pSceneNode;

	/// scene manager to attach this object to
	Ogre::SceneManager* m_pSceneManager;

	/// MaterialAnimation stack
  NodeMaterialOrig* m_pNodeMaterial;

	/// name of the resource group of the geometry file
	OpenViBE::CString m_sResourceGroupName;

	/// name of the geometry file
	OpenViBE::CString m_sGeometryFileName;

	/** \name Animators */
	//@{
	/**
	 * \brief Transform animator
   * This allows for modification of the object position, orientation and scale
	 */
	CTransformAnimator* m_pTransformAnimator;
	/**
	 * \brief Color animator
   * This allows for modification of the color and transparency of the material used by the object
	 */
	CColorAnimator* m_pColorAnimator;
	/**
	 * \brief Vertex buffer animator
   * This allows for updating vertex buffer contents
	 */
	CVertexBufferAnimator* m_pVertexBufferAnimator;
	//@}
};

#endif // __OpenViBEKernel_Kernel_Player_COgreObject_H__
