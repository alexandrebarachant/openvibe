#include "ovaCTieBCI.h"
#define M_PI  3.141592654f

REGISTER_OBJECT_FACTORY(CTieBCI, "ovaCTieBCI");

using namespace OMK::Type;

static const float g_fAttenuation=.99;
static const float g_fRotationSpeed=0.25;
static const float g_fMoveSpeed=0.01; // 0.004;
static const float g_fScoreGrowingSpeed=0.1;

using namespace OMK::Type;

CTieBCI::CTieBCI(OMK::Controller& rController, const OMK::ObjectDescriptor& rObjectDescriptor)
	:OMK::ExtensibleSimulatedObject(rController, rObjectDescriptor)
{
}

CTieBCI::~CTieBCI(void)
{
}

void CTieBCI::init(void)
{
	OMK::ExtensibleSimulatedObject::init();

	registerForSignal(g_sVrpnButtonStateUpdate);
	registerForSignal(g_sVrpnAnalogStateUpdate);

	OMK::ParametersAccessor::get < OMK::Name > (getConfigurationParameters(), "visu",              m_sVisuName);
	OMK::ParametersAccessor::get < OMK::Name > (getConfigurationParameters(), "vador_transform",   m_sVadorTransformName);
	OMK::ParametersAccessor::get < OMK::Name > (getConfigurationParameters(), "tie_transform",     m_sTieTransformName);
	OMK::ParametersAccessor::get < OMK::Name > (getConfigurationParameters(), "score_transform",   m_sScoreTransformName);
	OMK::ParametersAccessor::get < OMK::Name > (getConfigurationParameters(), "move_transform",    m_sMoveTransformName);
	OMK::ParametersAccessor::get < OMK::Name > (getConfigurationParameters(), "no_move_transform", m_sNoMoveTransformName);

	m_iScore=0;
	m_iPhase=Phase_Rest;
	m_iLastPhase=Phase_Rest;
	m_dFeedback=0;
	m_dLastFeedback=0;
	m_bShouldScore=false;
	m_fTieHeight=0;
	m_fScoreScale=0;
}

void CTieBCI::compute(void)
{
	OMK::ExtensibleSimulatedObject::compute();

	// -------------------------------------------------------------------------------
	// Score stuffs

	if(m_iLastPhase!=m_iPhase)
	{
		switch(m_iPhase)
		{
			case Phase_NoMove:
				m_bShouldScore=true;
				break;
		}
	}

	const Wm4::Vector3f l_vHide(100, 100, 100);
	const Wm4::Vector3f l_vShow(0, 0, 0);

	switch(m_iPhase)
	{
		case Phase_Rest:
			m_vMovePosition=l_vHide;
			m_vNoMovePosition=l_vHide;
			break;

		case Phase_Move:
			m_vMovePosition=l_vShow;
			m_vNoMovePosition=l_vHide;
			break;

		case Phase_NoMove:
			m_vMovePosition=l_vHide;
			m_vNoMovePosition=l_vShow;
			break;
	}

	if(m_bShouldScore)
	{
		const double l_dThreshold=1E-3;
		if(m_dLastFeedback<l_dThreshold && m_dFeedback>=l_dThreshold)
		{
			m_iScore++;
			m_bShouldScore=false;
		}
	}

	m_fScoreScale = (1-g_fScoreGrowingSpeed)*m_fScoreScale + g_fScoreGrowingSpeed*m_iScore;

	// -------------------------------------------------------------------------------
	// Tie stuffs

	if(m_dFeedback <= 0)
	{
		m_vTieOrientation[0] *= g_fAttenuation;
		m_vTieOrientation[1] *= g_fAttenuation;
		m_vTieOrientation[2] *= g_fAttenuation;
		m_fTieHeight *= g_fAttenuation;
	}
	else
	{
		m_vTieOrientation[0] += g_fRotationSpeed * ((rand()&1)==0?-1:1);
		m_vTieOrientation[1] += g_fRotationSpeed * ((rand()&1)==0?-1:1);
		m_vTieOrientation[2] += g_fRotationSpeed * ((rand()&1)==0?-1:1);
		m_fTieHeight += m_dFeedback* g_fMoveSpeed;

		if(m_vTieOrientation[0]>5)  m_vTieOrientation[0]=5;
		if(m_vTieOrientation[1]>5)  m_vTieOrientation[1]=5;
		if(m_vTieOrientation[2]>5)  m_vTieOrientation[2]=5;
		if(m_vTieOrientation[0]<-5) m_vTieOrientation[0]=-5;
		if(m_vTieOrientation[1]<-5) m_vTieOrientation[1]=-5;
		if(m_vTieOrientation[2]<-5) m_vTieOrientation[2]=-5;
		if(m_fTieHeight>6) m_fTieHeight=6;
	}

	// -------------------------------------------------------------------------------
	// Object displacement stuffs

	OMK::Type::Transform l_oPlaneOffset;

	l_oPlaneOffset.setTranslate(Wm4::Vector3f(0.5,0,0));

	OMK::Type::Transform m_oVadorTransform;
	OMK::Type::Transform m_oTieTransform;
	OMK::Type::Transform m_oScoreTransform;
	OMK::Type::Transform m_oMoveTransform;
	OMK::Type::Transform m_oNoMoveTransform;

	m_oTieTransform.setTranslate(Wm4::Vector3f(0, m_fTieHeight, 0));
	m_oTieTransform.setOrientation(m_vTieOrientation[0]*M_PI/180, m_vTieOrientation[1]*M_PI/180, m_vTieOrientation[2]*M_PI/180);

	m_oScoreTransform.setScale(Wm4::Vector3f(0.1+m_fScoreScale, 1, 1));

	m_oMoveTransform.setTranslate(m_vMovePosition);

	m_oNoMoveTransform.setTranslate(m_vNoMovePosition);

	sendValuedEvent(m_sVisuName,  m_sVadorTransformName, OMK::Type::TransformType(m_oVadorTransform));
	sendValuedEvent(m_sVisuName,    m_sTieTransformName, OMK::Type::TransformType(m_oTieTransform));
	sendValuedEvent(m_sVisuName,  m_sScoreTransformName, OMK::Type::TransformType(product(m_oScoreTransform, l_oPlaneOffset)));
	sendValuedEvent(m_sVisuName,   m_sMoveTransformName, OMK::Type::TransformType(m_oMoveTransform));
	sendValuedEvent(m_sVisuName, m_sNoMoveTransformName, OMK::Type::TransformType(m_oNoMoveTransform));

	// -------------------------------------------------------------------------------
	// End of computation

	m_dLastFeedback=m_dFeedback;
	m_iLastPhase=m_iPhase;
}

bool CTieBCI::processEvent(OMK::Event* pEvent)
{
	OMK::ExtensibleSimulatedObject::processEvent(pEvent);

	if(pEvent->eventId==g_sVrpnAnalogStateUpdate)
	{
		VrpnAnalogStateEvent* l_pValuedEvent=dynamic_cast < VrpnAnalogStateEvent* >(pEvent);
		if(l_pValuedEvent)
		{
			VrpnAnalogState l_oValue=l_pValuedEvent->value;

			m_dFeedback=*(l_oValue.begin());

// std::cout << "feedback : " << m_dFeedback << std::endl;

			return true;
		}
	}

	if(pEvent->eventId==g_sVrpnButtonStateUpdate)
	{
		VrpnButtonStateEvent* l_pValuedEvent=dynamic_cast < VrpnButtonStateEvent* >(pEvent);
		if(l_pValuedEvent)
		{
			VrpnButtonState l_oValue=l_pValuedEvent->value;

			if(l_oValue.second)
			{
				switch(l_oValue.first)
				{
					case 1:  m_iPhase=Phase_Rest; break;
					case 2:  m_iPhase=Phase_Move; break;
					case 3:  m_iPhase=Phase_NoMove; break;
				}
			}

// std::cout << l_oValue.first << (l_oValue.second?"true":"false") << std::endl;

			return true;
		}
	}

	return false;
}
