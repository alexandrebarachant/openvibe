#include "Ogre.h"
#include "OgreSceneNode.h"
#include "OgreMovableObject.h"
#include "dotSceneInterface.h"

#include "ovkCMaterial.h"
#include "ovkCTransformAnimator.h"
#include "ovkCColorAnimator.h"
#include "ovkCVertexBufferAnimator.h"
#include "ovkCOgreObject.h"
#include "ovkCOgreVisualisation.h"

using namespace OBT;
using namespace OpenViBE;
using namespace OpenViBE::Kernel;

COgreObject::COgreObject(const IKernelContext& rKernelContext, OpenViBE::CIdentifier oIdentifier, COgreVisualisation* pOgreVis, Ogre::SceneManager* pSceneManager, const CString& rGeometryFileName) :
	m_rKernelContext(rKernelContext),
	m_oIdentifier(oIdentifier),
	m_sName(oIdentifier.toString()),
	m_pOgreVis(pOgreVis),
	m_pSceneNode(NULL),
	m_pSceneManager(pSceneManager),
	m_pNodeMaterial(NULL),
	m_sResourceGroupName("General"),
	m_eGeometryFileType(GeometryFileType_Invalid),
	m_sGeometryFileName(rGeometryFileName),
	m_pTransformAnimator(NULL),
	m_pColorAnimator(NULL),
	m_pVertexBufferAnimator(NULL)
{
	if(strstr(m_sGeometryFileName.toASCIIString(), ".scene") != NULL)
	{
		m_eGeometryFileType = GeometryFileType_Scene;
	}
	else if(strstr(m_sGeometryFileName.toASCIIString(), ".mesh") != NULL)
	{
		m_eGeometryFileType = GeometryFileType_Mesh;
	}
	else
	{
		m_rKernelContext.getLogManager() << LogLevel_Warning << "Couldn't determine type of geometry file for object \"" << m_sName << "\"\n";
	}

	m_pSceneNode = m_pSceneManager->getRootSceneNode()->createChildSceneNode(m_sName.toASCIIString());

	if(m_pSceneNode == NULL)
	{
		m_rKernelContext.getLogManager() << LogLevel_Error << "Failed to create SceneNode \"" << m_sName.toASCIIString() << "\" for object \"" << m_sName << "\"\n";
	}

	//don't create node material before geometry is loaded!
	//m_pNodeMaterial = new NodeMaterialOrig(m_pSceneNode);

	//don't create animators before geometry is loaded!
	//m_pColorAnimator = new CColorAnimator(*this, OV_UndefinedIdentifier, "DCS_position");
}

COgreObject::COgreObject(const IKernelContext& rKernelContext, OpenViBE::CIdentifier oIdentifier, COgreVisualisation* pOgreVis, Ogre::SceneManager* pSceneManager) :
	m_rKernelContext(rKernelContext),
	m_oIdentifier(oIdentifier),
	m_sName(oIdentifier.toString()),
	m_pOgreVis(pOgreVis),
	m_pSceneNode(NULL),
	m_pSceneManager(pSceneManager),
	m_pNodeMaterial(NULL),
	m_sResourceGroupName("General"),
	m_eGeometryFileType(GeometryFileType_Invalid),
	m_pTransformAnimator(NULL),
	m_pColorAnimator(NULL),
	m_pVertexBufferAnimator(NULL)
{
	m_pSceneNode = m_pSceneManager->getRootSceneNode()->createChildSceneNode(m_sName.toASCIIString());

	if(m_pSceneNode == NULL)
	{
		m_rKernelContext.getLogManager() << LogLevel_Error << "Failed to create SceneNode \"" << m_sName.toASCIIString() << "\" for object \"" << m_sName << "\"\n";
	}
}

COgreObject::~COgreObject()
{
	try
	{
		m_pSceneManager->destroySceneNode(m_pSceneNode->getName());
	}
	catch(Ogre::Exception& e)
	{
		m_rKernelContext.getLogManager()
			<< LogLevel_Trace << "<" << LogColor_PushStateBit << LogColor_ForegroundBlue << "Ogre3D" << LogColor_PopStateBit << "::Exception> "
			<< "Failed to destroy scene node : " << e.what() << "\n";
	}

	if(m_pNodeMaterial != NULL)
	{
		delete m_pNodeMaterial;
	}
}

boolean COgreObject::cloneMeshes()
{
	Ogre::Entity* l_pFirstEntity = NULL;
	getFirstEntityInHierarchy(m_pSceneNode, l_pFirstEntity);

	if(l_pFirstEntity == NULL)
	{
		return false;
	}

	try
	{
		//clone mesh
		const Ogre::MeshPtr& l_pMesh = l_pFirstEntity->getMesh();
		std::string l_sNewMeshName = l_pMesh->getName() + m_sName.toASCIIString();
		Ogre::MeshPtr l_pNewMesh = l_pMesh->clone(l_sNewMeshName);

		//create new entity based on new mesh
		std::string l_sEntityName = l_pFirstEntity->getName();
		Ogre::SceneNode* l_pSceneNode = l_pFirstEntity->getParentSceneNode();
		l_pSceneNode->detachObject(l_pFirstEntity);
		m_pSceneManager->destroyEntity(l_pFirstEntity);
		Ogre::Entity* l_pNewEntity = m_pSceneManager->createEntity(l_sEntityName, l_sNewMeshName);

		//attach new entity
		l_pSceneNode->attachObject(l_pNewEntity);
	}
	catch(Ogre::Exception& e)
	{
		m_rKernelContext.getLogManager()
			<< LogLevel_Trace << "<" << LogColor_PushStateBit << LogColor_ForegroundBlue << "Ogre3D" << LogColor_PopStateBit << "::Exception> "
			<< "Failed to clone mesh : " << e.what() << "\n";
	}

	return true;
}

boolean COgreObject::cloneMaterials()
{
	try
	{
	Ogre::Entity* l_pFirstEntity = NULL;
	getFirstEntityInHierarchy(m_pSceneNode, l_pFirstEntity);

	if(l_pFirstEntity == NULL)
	{
		return false;
	}

#if 0
	std::map<std::string, std::string> m_mClonedMaterials;

	//clone materials of all subentities
	for(uint32 i=0;  i<l_pFirstEntity->getNumSubEntities(); i++)
	{
		Ogre::SubEntity* l_pSubEntity = l_pFirstEntity->getSubEntity(i);
		std::string l_sMaterialName(l_pSubEntity->getMaterial()->getName());
		//if material of current subentity hasn't been cloned yet
		if(m_mClonedMaterials.find(l_sMaterialName) == m_mClonedMaterials.end())
		{
			//generate new name
			m_mClonedMaterials[l_sMaterialName] = l_sMaterialName + m_sName.toASCIIString();
			//clone material
			l_pSubEntity->getMaterial()->clone(m_mClonedMaterials[l_sMaterialName]);
		}

		//set cloned material on subentity
		l_pSubEntity->setMaterialName(m_mClonedMaterials[l_sMaterialName]);
	}
#else
	std::string l_sMaterialName = l_pFirstEntity->getSubEntity(0)->getMaterialName();
	std::string l_sNewMaterialName = l_sMaterialName + m_sName.toASCIIString();
	Ogre::MaterialPtr newMat = l_pFirstEntity->getSubEntity(0)->getMaterial()->clone(l_sNewMaterialName);
	l_pFirstEntity->setMaterialName(l_sNewMaterialName);
#endif
	}
	catch(Ogre::Exception& e)
	{
		m_rKernelContext.getLogManager()
			<< LogLevel_Trace << "<" << LogColor_PushStateBit << LogColor_ForegroundBlue << "Ogre3D" << LogColor_PopStateBit << "::Exception> "
			<< "Failed to clone materials : " << e.what() << "\n";
	}

	return true;
}

boolean COgreObject::createPlane(const CNameValuePairList& rNameValuePairList)
{
	try
	{
		float64 l_f64PlaneWidth = 1.f;
		rNameValuePairList.getValue("PlaneWidth", l_f64PlaneWidth);
		float64 l_f64PlaneHeight = 1.f;
		rNameValuePairList.getValue("PlaneHeight", l_f64PlaneWidth);

		Ogre::MeshManager::getSingleton().createPlane(
			m_sName.toASCIIString(),//"myPlane", //mesh name
			m_sResourceGroupName.toASCIIString(),
			Ogre::Plane(Ogre::Vector3::UNIT_Z, 0.0), //plane orientation and distance from origin
			l_f64PlaneWidth, l_f64PlaneHeight, //width/height
			1, 1, //x and y segment count
			true, //generate normals?
			1, //texcoord set count
			1, 1,//u/v tiling
			Ogre::Vector3::UNIT_Y, //plane up vector
			Ogre::HardwareBuffer::HBU_WRITE_ONLY, //vertex buffer usage
			Ogre::HardwareBuffer::HBU_STATIC); //index buffer usage

		Ogre::Entity* l_pEntity = m_pSceneManager->createEntity(Ogre::String(m_sName.toASCIIString()) + "Entity", m_sName.toASCIIString());//"myPlane"); //FIXME!!
		//l_pEntity->setMaterialName("Examples/RustySteel");
		m_pSceneNode->attachObject(l_pEntity);
	}
	catch(Ogre::Exception& e)
	{
		m_rKernelContext.getLogManager()
			<< LogLevel_Trace << "<" << LogColor_PushStateBit << LogColor_ForegroundBlue << "Ogre3D" << LogColor_PopStateBit << "::Exception> "
			<< "Failed to create plane : " << e.what() << "\n";
		return false;
	}

	return true;
}

boolean COgreObject::loadGeometry()
{
	if(m_eGeometryFileType == GeometryFileType_Invalid)
	{
		return false;
	}

	try
	{
		if(m_eGeometryFileType == GeometryFileType_Scene)
		{
			Singleton<Ogre::dsi::dotSceneLoader>::getInstance().
				load(m_sGeometryFileName.toASCIIString(),
					m_sResourceGroupName.toASCIIString(),
					m_pSceneManager,
					NULL,
					m_pSceneNode,
					false,
					false,
					false);
		}
		else if(m_eGeometryFileType == GeometryFileType_Mesh)
		{
			//TODO!
		}
	}
	catch(Ogre::Exception& e)
	{
		m_rKernelContext.getLogManager()
			<< LogLevel_Trace << "<" << LogColor_PushStateBit << LogColor_ForegroundBlue << "Ogre3D" << LogColor_PopStateBit << "::Exception> "
			<< "Failed to load file [" << m_sGeometryFileName << "] : " << e.what() << "\n";
		return false;
	}

	return true;
}

CIdentifier COgreObject::getIdentifier()
{
	return m_oIdentifier;
}

const CString& COgreObject::getName()
{
	return m_sName;
}

const CString& COgreObject::getGeometryFileName()
{
	return m_sGeometryFileName;
}

NodeMaterialOrig* COgreObject::getNodeMaterialOrig()
{
	return m_pNodeMaterial;
}

Ogre::SceneNode& COgreObject::getSceneNode() const
{
	return *m_pSceneNode;
}

boolean COgreObject::setVisible(boolean bVisible)
{
	Ogre::Entity* l_pFirstEntity = NULL;
	if(getFirstEntityInHierarchy(m_pSceneNode, l_pFirstEntity) == true)
	{
		l_pFirstEntity->setVisible(bVisible);
		return true;
	}

	return false;
}

boolean COgreObject::setDiffuseColor(const Ogre::ColourValue& oDiffuse)
{
	if(m_pColorAnimator != NULL)
	{
		m_pColorAnimator->setDiffuseColor(oDiffuse);
	}
	return true;
}

boolean COgreObject::setDiffuseColor(Ogre::Real r, Ogre::Real g, Ogre::Real b)
{
	if(m_pColorAnimator != NULL)
	{
		m_pColorAnimator->setDiffuseColor(r, g, b);
	}
	return true;
}

boolean COgreObject::setTransparency(Ogre::Real t)
{
	if(m_pColorAnimator!= NULL)
	{
		m_pColorAnimator->setTransparency(t);
	}
	return true;
}

boolean COgreObject::setPosition(Ogre::Real positionX, Ogre::Real positionY, Ogre::Real positionZ)
{
	if(m_pTransformAnimator != NULL)
	{
		m_pTransformAnimator->setPosition(positionX, positionY, positionZ);
	}
	return true;
}

boolean COgreObject::setRotation(Ogre::Real rotationX, Ogre::Real rotationY, Ogre::Real rotationZ)
{
	if(m_pTransformAnimator != NULL)
	{
		m_pTransformAnimator->setRotation(rotationX, rotationY, rotationZ);
	}
	return true;
}

boolean COgreObject::setRotation(Ogre::Real rotationX, Ogre::Real rotationY, Ogre::Real rotationZ, Ogre::Real rotationW)
{
	if(m_pTransformAnimator != NULL)
	{
		m_pTransformAnimator->setRotation(rotationX, rotationY, rotationZ, rotationW);
	}
	return true;
}

boolean COgreObject::setScale(Ogre::Real scaleX, Ogre::Real scaleY, Ogre::Real scaleZ)
{
	if(m_pTransformAnimator != NULL)
	{
		m_pTransformAnimator->setScale(scaleX, scaleY, scaleZ);
	}
	return true;
}

boolean COgreObject::setVertexColorArray(Ogre::uint32 ui32VertexColorCount, Ogre::Real* pVertexColorArray)
{
	if(m_pVertexBufferAnimator != NULL)
	{
		m_pVertexBufferAnimator->setVertexColorArray(ui32VertexColorCount, pVertexColorArray);
	}
	return true;
}

boolean COgreObject::getVertexCount(Ogre::uint32& ui32VertexCount) const
{
	Ogre::Entity* l_pEntity;
	if(getFirstEntityInHierarchy(&getSceneNode(), l_pEntity) == false)
	{
		return false;
	}

	Ogre::SubMesh* l_pSubMesh;
	if(getFirstSubMesh(l_pEntity, l_pSubMesh) == false)
	{
		return false;
	}

	if(l_pSubMesh->useSharedVertices == true)
	{
		m_rKernelContext.getLogManager() << LogLevel_Trace << "Failed to retrieve vertex count : submesh uses shared vertices!\n";
		return false;
	}

	ui32VertexCount = (uint32)l_pSubMesh->vertexData->vertexCount;

	return true;
}

boolean COgreObject::getVertexPositionArray(Ogre::uint32 ui32VertexPositionCount, Ogre::Real* pVertexPositionArray) const
{
	if(pVertexPositionArray == NULL)
	{
		return false;
	}

	Ogre::Entity* l_pEntity;
	if(getFirstEntityInHierarchy(&getSceneNode(), l_pEntity) == false)
	{
		return false;
	}

	Ogre::SubMesh* l_pSubMesh;
	if(getFirstSubMesh(l_pEntity, l_pSubMesh) == false)
	{
		return false;
	}

	if(l_pSubMesh->useSharedVertices == true)
	{
		m_rKernelContext.getLogManager() << LogLevel_Trace << "Failed to retrieve vertex count : submesh uses shared vertices!\n";
		return false;
	}

	if(l_pSubMesh->vertexData->vertexCount != ui32VertexPositionCount)
	{
		return false;
	}

	//get world matrix
	Ogre::Matrix4 l_oMatrix4;
	l_pEntity->getParentSceneNode()->getWorldTransforms(&l_oMatrix4);

	//get pointer to POSITION element
	const Ogre::VertexElement* l_pPositionElement = l_pSubMesh->vertexData->vertexDeclaration->findElementBySemantic(Ogre::VES_POSITION);

	//retrieve VB for POSITION element
	Ogre::HardwareVertexBufferSharedPtr l_pVertexBuffer = l_pSubMesh->vertexData->vertexBufferBinding->getBuffer(l_pPositionElement->getSource());

	//lock VB for reading
	unsigned char* l_pVertexBufferPointer = static_cast<unsigned char*>(l_pVertexBuffer->lock(Ogre::HardwareBuffer::HBL_READ_ONLY));

	//iterate through VB
	float* l_pReal;

	Ogre::Vector3 l_oWorldVector;

	for( size_t j=0; j<l_pSubMesh->vertexData->vertexCount; ++j, l_pVertexBufferPointer += l_pVertexBuffer->getVertexSize())
	{
		//get pointer to POSITION data
		l_pPositionElement->baseVertexPointerToElement(l_pVertexBufferPointer, &l_pReal);
#if 0
		//store it
		pVertexPositionArray[j*3] = l_pReal[0];
		pVertexPositionArray[j*3+1] = l_pReal[1];
		pVertexPositionArray[j*3+2] = l_pReal[2];
#else
		Ogre::Vector3 tmp(l_pReal[0], l_pReal[1], l_pReal[2]);
		l_oWorldVector = l_oMatrix4 * tmp;
		pVertexPositionArray[j*3] = l_oWorldVector[0];
		pVertexPositionArray[j*3+1] = l_oWorldVector[1];
		pVertexPositionArray[j*3+2] = l_oWorldVector[2];
#endif
	}

	//unlock VB
	l_pVertexBuffer->unlock();

	return true;
}

boolean COgreObject::createAnimators()
{
	//create node material once model hierarchy is loaded
	m_pNodeMaterial = new NodeMaterialOrig(m_pSceneNode);

	//create animators once node material is loaded
	m_pTransformAnimator = new CTransformAnimator(m_rKernelContext, *this, OV_UndefinedIdentifier);
	if(m_pTransformAnimator->init(""/*"DCS_position"*/) == false)
	{
		delete m_pTransformAnimator;
		m_pTransformAnimator = NULL;
	}

	m_pColorAnimator = new CColorAnimator(m_rKernelContext, *this, OV_UndefinedIdentifier);
	if(m_pColorAnimator->init(""/*"DCS_position"*/) == false)
	{
		delete m_pColorAnimator;
		m_pColorAnimator = NULL;
	}

	m_pVertexBufferAnimator = new CVertexBufferAnimator(m_rKernelContext, *this, OV_UndefinedIdentifier);
	if(m_pVertexBufferAnimator->init(""/*"DCS_position"*/) == false)
	{
		delete m_pVertexBufferAnimator;
		m_pVertexBufferAnimator = NULL;
	}

	//REMOVE ME
	/*
	Ogre::Entity* l_pEntity = NULL;
	getFirstEntityInHierarchy(m_pSceneNode, l_pEntity);
	if(l_pEntity != NULL)
	{
		l_pEntity->setNormaliseNormals(true);
	}*/

	return true;
}

boolean COgreObject::getFirstEntityInHierarchy(Ogre::SceneNode* pNode, Ogre::Entity*& pEntity) const
{
	for(uint32 i=0; i<pNode->numAttachedObjects(); i++)
	{
		Ogre::MovableObject* pMovableObject = pNode->getAttachedObject(0);
		if(pMovableObject->getMovableType() == Ogre::EntityFactory::FACTORY_TYPE_NAME)
		{
			pEntity = dynamic_cast<Ogre::Entity*>(pMovableObject);
			return true;
		}
	}

	for(uint32 i=0; i<pNode->numChildren(); i++)
	{
		Ogre::SceneNode* l_pChildNode = dynamic_cast<Ogre::SceneNode*>(pNode->getChild(i));

		if(l_pChildNode != NULL && getFirstEntityInHierarchy(l_pChildNode, pEntity) == true)
		{
			return true;
		}
	}

	return false;
}

boolean COgreObject::getFirstSubMesh(Ogre::Entity* l_pEntity, Ogre::SubMesh*& l_pSubMesh) const
{
	Ogre::Mesh* l_pMesh = l_pEntity->getMesh().get();
	if(l_pMesh == NULL)
	{
		return false;
	}

	l_pSubMesh = l_pMesh->getSubMesh(0);

	if(l_pSubMesh == NULL)
	{
		m_rKernelContext.getLogManager() << LogLevel_Trace << "Failed to retrieve SubMesh\n";
		return false;
	}

	return true;
}

boolean COgreObject::getWorldBoundingBox(float32& rMinX, float32& rMinY, float32& rMinZ, float32& rMaxX, float32& rMaxY, float32& rMaxZ)
{
	Ogre::Entity* l_pEntity;
	if(getFirstEntityInHierarchy(&getSceneNode(), l_pEntity) == false)
	{
		return false;
	}

	const Ogre::AxisAlignedBox& l_rWorldAABB = l_pEntity->getWorldBoundingBox(true);

	rMinX = l_rWorldAABB.getMinimum().x;
	rMinY = l_rWorldAABB.getMinimum().y;
	rMinZ = l_rWorldAABB.getMinimum().z;

	rMaxX = l_rWorldAABB.getMaximum().x;
	rMaxY = l_rWorldAABB.getMaximum().y;
	rMaxZ = l_rWorldAABB.getMaximum().z;

	return true;
}
