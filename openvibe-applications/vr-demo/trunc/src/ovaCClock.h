#ifndef __OpenViBEApplication_CClock_H__
#define __OpenViBEApplication_CClock_H__

#include "ova_defines.h"

class CClock : public OMK::SimulatedObject
{
public:

	DECLARE_OBJECT_FACTORY(CClock);

	virtual void compute(void);

	int m_iLastSimulatedTime;
	int m_iCurrentSimulatedTime;

	int m_iLastTime;
	int m_iCurrentTime;
};

#endif // __OpenViBEApplication_CClock_H__
