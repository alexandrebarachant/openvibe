#ifndef __OpenViBEApplication_CFootballBCI_H__
#define __OpenViBEApplication_CFootballBCI_H__

#include "ova_defines.h"

class CFootballBCI : public OMK::ExtensibleSimulatedObject
{
public:

	DECLARE_OBJECT_FACTORY(CFootballBCI);

	virtual void init(void);
	virtual void compute(void);
	virtual bool processEvent(OMK::Event* pEvent);

	int m_iGoalScore;
	int m_iSideScore;
	int m_iClassificationScore;
	int m_iFeedbackLoopCount;
	int m_iPhase;
	int m_iLastPhase;
	int m_iMark;
	int m_iLastMark;

	double m_dFeedback;
	double m_dLastFeedback;

	bool m_bShowMark;

protected:

	bool m_bGoalMarkedAtThisPhase;
	bool m_bScoreChanged;
	float m_fBallSpeed;
	float m_fBallPosition;
	float m_fBallOrientation;
	float m_fGoalScoreScale;
	float m_fSideScoreScale;
	float m_fClassificationScoreScale;

	OMK::Name m_sVisuName;
	OMK::Name m_sActiveBallTransformName;
	OMK::Name m_sGetReadyBallTransformName;
	OMK::Name m_sPassiveBallTransformName;
	OMK::Name m_sArrowTransformName;
	OMK::Name m_sGoalScoreTransformName;
	OMK::Name m_sSideScoreTransformName;
	OMK::Name m_sClassificationScoreTransformName;
	OMK::Name m_sFeedbackTransformName;

	OMK::Name m_sBasicArrowTransformName;
	OMK::Name m_sCrossTransformName;

	OMK::Type::Transform m_oLeftGoalArrowPosition;
	OMK::Type::Transform m_oRightGoalArrowPosition;

	double m_dMinAnalog;
	double m_dMaxAnalog;
	std::list < double > m_vAnalogHistory;

private:

	enum
	{
		Mark_None,
		Mark_Left,
		Mark_Right,
	} EMark;

	enum
	{
		Phase_Passive,
		Phase_GetReady,
		Phase_Active,
	} EPhase;
};

#endif // __OpenViBEApplication_CFootballBCI_H__
