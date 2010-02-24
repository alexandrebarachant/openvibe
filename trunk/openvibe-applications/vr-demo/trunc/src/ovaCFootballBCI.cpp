#include "ovaCFootballBCI.h"
#define M_PI  3.141592654f

template <typename T> const T _abs_(const T& t) { return t<0?-t:t; }
template <typename T> const T _min_(const T& t1, const T& t2) { return t1<t2?t1:t2; }
template <typename T> const T _max_(const T& t1, const T& t2) { return t1<t2?t2:t1; }
template <typename T> const T _crop_(const T& t1, const T& t, const T& t2) { return _min_(_max_(t1, t), t2); }

REGISTER_OBJECT_FACTORY(CFootballBCI, "ovaCFootballBCI");

using namespace OMK::Type;

static const float g_fViscosity=0.003;
static const float g_fGrowScoreingSpeed=0.1;
static const float g_fEpsilon=1E-5;

using namespace OMK::Type;

CFootballBCI::CFootballBCI(OMK::Controller& rController, const OMK::ObjectDescriptor& rObjectDescriptor)
	:OMK::ExtensibleSimulatedObject(rController, rObjectDescriptor)
{
}

CFootballBCI::~CFootballBCI(void)
{
}

void CFootballBCI::init(void)
{
	OMK::ExtensibleSimulatedObject::init();

	registerForSignal(g_sVrpnButtonStateUpdate);
	registerForSignal(g_sVrpnAnalogStateUpdate);

	OMK::ParametersAccessor::get < OMK::Name > (getConfigurationParameters(), "visu",                           m_sVisuName);
	OMK::ParametersAccessor::get < OMK::Name > (getConfigurationParameters(), "active_ball_transform",          m_sActiveBallTransformName);
	OMK::ParametersAccessor::get < OMK::Name > (getConfigurationParameters(), "get_ready_ball_transform",       m_sGetReadyBallTransformName);
	OMK::ParametersAccessor::get < OMK::Name > (getConfigurationParameters(), "passive_ball_transform",         m_sPassiveBallTransformName);
	OMK::ParametersAccessor::get < OMK::Name > (getConfigurationParameters(), "arrow_transform",                m_sArrowTransformName);
	OMK::ParametersAccessor::get < OMK::Name > (getConfigurationParameters(), "goal_score_transform",           m_sGoalScoreTransformName);
	OMK::ParametersAccessor::get < OMK::Name > (getConfigurationParameters(), "side_score_transform",           m_sSideScoreTransformName);
	OMK::ParametersAccessor::get < OMK::Name > (getConfigurationParameters(), "classification_score_transform", m_sClassificationScoreTransformName);
	OMK::ParametersAccessor::get < OMK::Name > (getConfigurationParameters(), "feedback_transform",             m_sFeedbackTransformName);

	OMK::ParametersAccessor::get < OMK::Name > (getConfigurationParameters(), "basic_arrow_transform",     m_sBasicArrowTransformName);
	OMK::ParametersAccessor::get < OMK::Name > (getConfigurationParameters(), "cross_transform",                m_sCrossTransformName);

	OMK::ParametersAccessor::get < OMK::Type::Transform > (getConfigurationParameters(), "left_goal_arrow_position",  m_oLeftGoalArrowPosition);
	OMK::ParametersAccessor::get < OMK::Type::Transform > (getConfigurationParameters(), "right_goal_arrow_position", m_oRightGoalArrowPosition);

	m_iGoalScore=0;
	m_iSideScore=0;
	m_iClassificationScore=0;
	m_iFeedbackLoopCount=0;
	m_iPhase=Phase_Passive;
	m_iLastPhase=Phase_Passive;
	m_dFeedback=0;
	m_dLastFeedback=0;

	m_fBallSpeed=0;
	m_fBallPosition=0;
	m_fBallOrientation=0;
	m_fGoalScoreScale=0;
	m_fSideScoreScale=0;
	m_fClassificationScoreScale=0;

	m_iMark=Mark_None;
	m_iLastMark=Mark_None;
	m_bShowMark=false;

	m_dMinAnalog=0;
	m_dMaxAnalog=0;
}

void CFootballBCI::compute(void)
{
	OMK::ExtensibleSimulatedObject::compute();

	// -------------------------------------------------------------------------------
	// Football stuffs

	// Checks phase change
	if(m_iPhase!=m_iLastPhase)
	{
		switch(m_iPhase)
		{
			case Phase_Passive:
			case Phase_GetReady:
				if(m_iLastMark==Mark_Left && m_fBallPosition<0) { m_iSideScore++; m_bScoreChanged=true; }
				if(m_iLastMark==Mark_Right&& m_fBallPosition>0) { m_iSideScore++; m_bScoreChanged=true; }
				m_fBallPosition=0;
				m_fBallSpeed=0;

				std::cout << "### ScOrE StAtUs = GoAl:" << m_iGoalScore << " SiDe:" << m_iSideScore << " ClAsSiFiCaTiOn:" << m_iClassificationScore << " LoOpCoUnT: " << m_iFeedbackLoopCount << "\n";
				break;

			case Phase_Active:
				m_bGoalMarkedAtThisPhase=false;
				break;
		}
	}

	// Checks phase
	switch(m_iPhase)
	{
		case Phase_Passive:
			break;

		case Phase_GetReady:
			break;

		case Phase_Active:
			m_fBallSpeed=m_dFeedback * 0.1;
			if(m_iMark==Mark_Left && m_fBallPosition==-6 && !m_bGoalMarkedAtThisPhase) { m_iGoalScore++; m_bScoreChanged=true; m_bGoalMarkedAtThisPhase=true; }
			if(m_iMark==Mark_Right&& m_fBallPosition== 6 && !m_bGoalMarkedAtThisPhase) { m_iGoalScore++; m_bScoreChanged=true; m_bGoalMarkedAtThisPhase=true; }
			if(m_iMark==Mark_Left && m_dFeedback<0) { m_iClassificationScore++; /* m_bScoreChanged=true; */ }
			if(m_iMark==Mark_Right&& m_dFeedback>0) { m_iClassificationScore++; /* m_bScoreChanged=true; */ }
			m_iFeedbackLoopCount++;
			break;
	}

	m_fBallOrientation+=m_fBallSpeed;
	m_fBallPosition+=m_fBallSpeed;
	m_fBallPosition=_crop_(-6.0f, m_fBallPosition, 6.0f);
	m_fBallSpeed*=(1.-g_fViscosity);

	m_fGoalScoreScale           = (1-g_fGrowScoreingSpeed)*m_fGoalScoreScale +           g_fGrowScoreingSpeed*m_iGoalScore;
	m_fSideScoreScale           = (1-g_fGrowScoreingSpeed)*m_fSideScoreScale +           g_fGrowScoreingSpeed*m_iSideScore;
	m_fClassificationScoreScale = (1-g_fGrowScoreingSpeed)*m_fClassificationScoreScale + g_fGrowScoreingSpeed*m_iClassificationScore/500.0f;

	double m_fFeedbackScale = m_dFeedback * 2.5;
	if(m_fFeedbackScale <= 0 && m_fFeedbackScale > -g_fEpsilon) m_fFeedbackScale=-g_fEpsilon;
	if(m_fFeedbackScale >= 0 && m_fFeedbackScale <  g_fEpsilon) m_fFeedbackScale= g_fEpsilon;

	// -------------------------------------------------------------------------------
	// Object displacement stuffs

	OMK::Type::Transform l_oPlaneOffset;

	l_oPlaneOffset.setTranslate(Wm4::Vector3f(0.5,0,0));

	OMK::Type::Transform l_oArrowOffset;
/*
	float t=getSimulatedDate()/1000.0;
	l_oArrowOffset.setTranslate(Wm4::Vector3f(
		0.2*sin(0.3*t),
		0.2*sin(5*t),
		0.2*sin(0.2*t)));
	l_oArrowOffset.setOrientation(
		t,
		0.2*sin(0.7*t),
		0.2*sin(0.11*t));
*/
	OMK::Type::Transform l_oActiveBallTransform;
	OMK::Type::Transform l_oGetReadyBallTransform;
	OMK::Type::Transform l_oPassiveBallTransform;
	OMK::Type::Transform l_oArrowTransform;
	OMK::Type::Transform l_oGoalScoreTransform;
	OMK::Type::Transform l_oSideScoreTransform;
	OMK::Type::Transform l_oClassificationScoreTransform;
	OMK::Type::Transform l_oFeedbackTransform;

	OMK::Type::Transform l_oBasicArrowTransform;
	OMK::Type::Transform l_oCrossTransform;

	l_oActiveBallTransform.setTranslate(Wm4::Vector3f(m_iPhase==Phase_Active?0:1000, 0, m_fBallPosition));
	l_oActiveBallTransform.setOrientation(0, m_fBallOrientation*4, 0);

	l_oGetReadyBallTransform.setTranslate(Wm4::Vector3f(m_iPhase==Phase_GetReady?0:1000, 0, m_fBallPosition));
	l_oGetReadyBallTransform.setOrientation(0, m_fBallOrientation*4, 0);

	l_oPassiveBallTransform.setTranslate(Wm4::Vector3f(m_iPhase==Phase_Passive?0:1000, 0, m_fBallPosition));
	l_oPassiveBallTransform.setOrientation(0, m_fBallOrientation*4, 0);

	l_oCrossTransform.setTranslate(Wm4::Vector3f(m_iPhase==Phase_Passive?1000:0, 0, 0));

	if(!m_bShowMark)
	{
		l_oArrowTransform.setTranslate(Wm4::Vector3f(1000, 0, 0));
		l_oBasicArrowTransform.setTranslate(Wm4::Vector3f(1000, 0, 0));
	}
	else
	{
		switch(m_iMark)
		{
			default:
			case Mark_None:
				l_oArrowTransform.setTranslate(Wm4::Vector3f(1000, 0, 0));
				l_oBasicArrowTransform.setTranslate(Wm4::Vector3f(1000, 0, 0));
				break;

			case Mark_Left:
				l_oArrowTransform=m_oLeftGoalArrowPosition;
				l_oBasicArrowTransform.setOrientation(0, 0, M_PI/2);
				break;

			case Mark_Right:
				l_oArrowTransform=m_oRightGoalArrowPosition;
				l_oBasicArrowTransform.setOrientation(0, 0, -M_PI/2);
				break;
		}
	}

	l_oGoalScoreTransform.setScale(Wm4::Vector3f(0.1+m_fGoalScoreScale, 1, 1));

	l_oSideScoreTransform.setScale(Wm4::Vector3f(0.1+m_fSideScoreScale, 1, 1));

	l_oClassificationScoreTransform.setScale(Wm4::Vector3f(0.1+m_fClassificationScoreScale, 1, 1));

	l_oFeedbackTransform.setTranslate(Wm4::Vector3f(m_iPhase==Phase_Active?0:1000, 0, 0));
	l_oFeedbackTransform.setScale(Wm4::Vector3f(_abs_(m_fFeedbackScale)*5, 1, 1));
	l_oFeedbackTransform.setOrientation(m_fFeedbackScale>0?0:M_PI, 0, 0);

	sendValuedEvent(m_sVisuName,          m_sActiveBallTransformName, OMK::Type::TransformType(l_oActiveBallTransform));
	sendValuedEvent(m_sVisuName,        m_sGetReadyBallTransformName, OMK::Type::TransformType(l_oGetReadyBallTransform));
	sendValuedEvent(m_sVisuName,         m_sPassiveBallTransformName, OMK::Type::TransformType(l_oPassiveBallTransform));
	sendValuedEvent(m_sVisuName,               m_sArrowTransformName, OMK::Type::TransformType(product(l_oArrowTransform, l_oArrowOffset)));
	sendValuedEvent(m_sVisuName,           m_sGoalScoreTransformName, OMK::Type::TransformType(product(l_oGoalScoreTransform, l_oPlaneOffset)));
	sendValuedEvent(m_sVisuName,           m_sSideScoreTransformName, OMK::Type::TransformType(product(l_oSideScoreTransform, l_oPlaneOffset)));
	sendValuedEvent(m_sVisuName, m_sClassificationScoreTransformName, OMK::Type::TransformType(product(l_oClassificationScoreTransform, l_oPlaneOffset)));
	sendValuedEvent(m_sVisuName,            m_sFeedbackTransformName, OMK::Type::TransformType(product(l_oFeedbackTransform, l_oPlaneOffset)));

	sendValuedEvent(m_sVisuName,          m_sBasicArrowTransformName, OMK::Type::TransformType(l_oBasicArrowTransform));
	sendValuedEvent(m_sVisuName,               m_sCrossTransformName, OMK::Type::TransformType(l_oCrossTransform));

	// -------------------------------------------------------------------------------
	// End of computation

	if(m_bScoreChanged)
	{
		// std::cout << "### NeW ScOrE ! GoAl:" << m_iGoalScore << " SiDe:" << m_iSideScore << " ClAsSiFiCaTiOn:" << m_iClassificationScore << "\n";
		m_bScoreChanged=false;
	}

	m_dLastFeedback=m_dFeedback;
	m_iLastPhase=m_iPhase;
	m_iLastMark=m_iMark;
}

bool CFootballBCI::processEvent(OMK::Event* pEvent)
{
	OMK::ExtensibleSimulatedObject::processEvent(pEvent);

	if(pEvent->eventId==g_sVrpnAnalogStateUpdate)
	{
		VrpnAnalogStateEvent* l_pValuedEvent=dynamic_cast < VrpnAnalogStateEvent* >(pEvent);
		if(l_pValuedEvent)
		{
			VrpnAnalogState l_oValue=l_pValuedEvent->value;

			double l_dAnalog=*(l_oValue.begin());
			if(l_dAnalog>m_dMaxAnalog) m_dMaxAnalog=l_dAnalog;
			if(l_dAnalog<m_dMinAnalog) m_dMinAnalog=l_dAnalog;
			double l_dAbsoluteMinMax=_max_(m_dMaxAnalog, -m_dMinAnalog);

			m_vAnalogHistory.push_back(l_dAnalog);
			if(m_vAnalogHistory.size()>5)
			{
				m_vAnalogHistory.pop_front();
			}

			double l_dAnalogMean=0;
			std::list < double >::const_iterator i;
			for(i=m_vAnalogHistory.begin(); i!=m_vAnalogHistory.end(); i++)
			{
				l_dAnalogMean+=*i;
			}
			l_dAnalogMean/=m_vAnalogHistory.size();
			l_dAnalogMean/=(l_dAbsoluteMinMax?l_dAbsoluteMinMax:1);
			m_dFeedback=l_dAnalogMean;

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
					case 0:
						m_iPhase=Phase_Passive;
						break;

					case 1:
						m_iPhase=Phase_GetReady;
						break;

					case 2:
						m_iPhase=Phase_Active;
						break;

					case 3:
						// m_iMark=Mark_None;
						m_bShowMark=false;
						break;

					case 4:
						m_iMark=Mark_Left;
						m_bShowMark=true;
						break;

					case 5:
						m_iMark=Mark_Right;
						m_bShowMark=true;
						break;

					default:
						break;
				}
			}

			return true;
		}
	}

	return false;
}
