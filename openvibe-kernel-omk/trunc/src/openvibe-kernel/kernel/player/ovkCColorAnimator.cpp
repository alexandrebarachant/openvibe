#include "ovkMaterialAction.h"
#include "ovkCColorAnimator.h"

using namespace OpenViBE;
using namespace OpenViBE::Kernel;
using std::string;
using Ogre::SceneNode;
using Ogre::MovableObject;
using Ogre::Entity;

CColorAnimator::CColorAnimator(const IKernelContext& rKernelContext, COgreObject& rOgreObject, const CIdentifier& rIdentifier) :
	COgreAnimator(rKernelContext, rOgreObject, rIdentifier),
	m_pNodeMaterial(NULL),
	m_oColor (1.f,1.f,1.f,1.f)
{
}

CColorAnimator::~CColorAnimator()
{
	/*if(m_pNodeMaterial != NULL)
	{
		delete m_pNodeMaterial;
	}*/
}

boolean CColorAnimator::init(const std::string& rNodeName)
{
	if(attachToNode(rNodeName) == false)
	{
		return false;
	}
	m_pNodeMaterial = m_rOgreObject.getNodeMaterialOrig()->find(&getSceneNode());
	if(m_pNodeMaterial == NULL)
	{
		//TODO couldn't find node!
		return false;
	}
	return true;
}

void CColorAnimator::setDiffuse(const Ogre::ColourValue& diffuse)
{
	if(m_pNodeMaterial != NULL)
	{
		m_pNodeMaterial->doAction(MaterialSetDiffuse(m_rKernelContext, diffuse));
	}
}

void CColorAnimator::setTransparency(const Ogre::Real& transparency)
{
	if(m_pNodeMaterial != NULL)
	{
		m_pNodeMaterial->doAction(MaterialSetTransparency(m_rKernelContext, transparency));
	}
}
