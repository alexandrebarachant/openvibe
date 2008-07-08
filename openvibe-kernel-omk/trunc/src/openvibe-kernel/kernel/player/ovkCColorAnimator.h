#ifndef __OpenViBEKernel_Kernel_Player_CColorAnimator_H__
#define __OpenViBEKernel_Kernel_Player_CColorAnimator_H__

#include "ovkCOgreAnimator.h"

class NodeMaterialOrig;

class CColorAnimator : public COgreAnimator
{
public:
	CColorAnimator(
		const OpenViBE::Kernel::IKernelContext& rKernelContext,
		COgreObject& rOgreObject,
		const OpenViBE::CIdentifier& rIdentifier);

	virtual ~CColorAnimator();

	virtual OpenViBE::boolean init(const std::string& rNodeName);

	void setDiffuse(const Ogre::ColourValue& diffuse);

	void setTransparency(const Ogre::Real& transparency);

	//TODO : have one animator per parameter or have a generic one?
	//void set(const Ogre::ColourColourValue& diffuse);
	//void set(const Ogre::ColourValue& diffuse);

protected:
	NodeMaterialOrig* m_pNodeMaterial;
	Ogre::Vector4 m_oColor;
};

#endif // __OpenViBEKernel_Kernel_Player_CColorAnimator_H__
