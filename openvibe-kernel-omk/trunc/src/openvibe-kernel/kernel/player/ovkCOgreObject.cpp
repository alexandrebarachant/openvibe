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

COgreObject::COgreObject(const IKernelContext& rKernelContext, const CString& rName, COgreVisualisation* pOgreVis, Ogre::SceneManager* pSceneManager, const CString& rGeometryFileName) :
	m_rKernelContext(rKernelContext),
	m_sName(rName),
	m_pOgreVis(pOgreVis),
	m_pSceneNode(NULL),
	m_pSceneManager(pSceneManager),
	m_pNodeMaterial(NULL),
	m_sResourceGroupName("General"),
	m_sGeometryFileName(rGeometryFileName),
	m_pTransformAnimator(NULL),
	m_pColorAnimator(NULL),
	m_pVertexBufferAnimator(NULL)
{
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

boolean COgreObject::loadGeometry()
{
	if(std::string(m_sGeometryFileName).empty() == true)
	{
		return false;
	}
/*
	if(bClone == true)
	{
		//TODO
	}
*/
	try
	{
		Singleton<Ogre::dsi::dotSceneLoader>::getInstance().
			load(m_sGeometryFileName.toASCIIString(),
				m_sResourceGroupName.toASCIIString(),
				m_pSceneManager,
				NULL,
				m_pSceneNode,
				false,
				false,
				true);
	}
	catch(Ogre::Exception& e)
	{
		m_rKernelContext.getLogManager()
			<< LogLevel_Trace << "<" << LogColor_PushStateBit << LogColor_ForegroundBlue << "Ogre3D" << LogColor_PopStateBit << "::Exception> "
			<< "Failed to load file [" << m_sGeometryFileName << "] : " << e.what() << "\n";
		return false;
	}

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
	return true;
}

const CString& COgreObject::getName()
{
	return m_sName;
}

NodeMaterialOrig* COgreObject::getNodeMaterialOrig()
{
	return m_pNodeMaterial;
}

Ogre::SceneNode& COgreObject::getSceneNode() const
{
	return *m_pSceneNode;
}

boolean COgreObject::setDiffuseColor(Ogre::ColourValue diffuse)
{
	if(m_pColorAnimator != NULL)
	{
		m_pColorAnimator->setDiffuse(diffuse);
	}
	return true;
}

boolean COgreObject::setTransparency(Ogre::Real transparency)
{
	if(m_pColorAnimator!= NULL)
	{
		m_pColorAnimator->setTransparency(transparency);
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
	//TODO : support shared vertices (they are stored at the Mesh level)
	if(l_pSubMesh == NULL || l_pSubMesh->useSharedVertices)
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
