#ifndef __OpenViBEApplication_CTieBCI_H__
#define __OpenViBEApplication_CTieBCI_H__

#include "ova_defines.h"

class CTieBCI : public OMK::ExtensibleSimulatedObject
{
public:

	DECLARE_OBJECT_FACTORY(CTieBCI);

	virtual void init(void);
	virtual void compute(void);
	virtual bool processEvent(OMK::Event* pEvent);

	int m_iScore;
	int m_iPhase;
	int m_iLastPhase;
	double m_dFeedback;

protected:

	double m_dLastFeedback;
	bool m_bShouldScore;

	OMK::Name m_sVisuName;
	OMK::Name m_sVadorTransformName;
	OMK::Name m_sTieTransformName;
	OMK::Name m_sScoreTransformName;
	OMK::Name m_sMoveTransformName;
	OMK::Name m_sNoMoveTransformName;

	float m_fTieHeight;
	Wm4::Vector3f m_vTieOrientation;

	float m_fScoreScale;

	Wm4::Vector3f m_vMovePosition;
	Wm4::Vector3f m_vNoMovePosition;

private:

	enum
	{
		Phase_Rest,
		Phase_Move,
		Phase_NoMove,
	};
};

#endif // __OpenViBEApplication_CTieBCI_H__
