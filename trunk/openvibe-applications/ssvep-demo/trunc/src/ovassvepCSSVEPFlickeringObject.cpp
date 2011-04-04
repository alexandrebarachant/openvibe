#include "ovassvepCSSVEPFlickeringObject.h"
#include <iostream>

using namespace OpenViBESSVEP;

CSSVEPFlickeringObject::CSSVEPFlickeringObject(Ogre::SceneNode* poObjectNode, OpenViBE::uint32 ui32LitFrames, OpenViBE::uint32 ui32DarkFrames) :
	m_poObjectNode( poObjectNode ),
	m_ui32LitFrames( ui32LitFrames ),
	m_ui32DarkFrames( ui32DarkFrames ),
	m_bVisible( true )
{
}

void CSSVEPFlickeringObject::setVisible( bool bVisibility )
{
	if ( (!m_bVisible && bVisibility) || (m_bVisible && !bVisibility) )
	{
		m_poObjectNode->flipVisibility();
	}
	m_bVisible = bVisibility;
}

void CSSVEPFlickeringObject::processFrame(OpenViBE::uint32 ui32CurrentFrame)
{
	if (ui32CurrentFrame % ( m_ui32LitFrames + m_ui32DarkFrames ) < m_ui32LitFrames)
	{
		this->setVisible( true );
	}
	else
	{
		this->setVisible( false );
	}
}
