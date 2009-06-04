#include "ovaCClock.h"

#include <system/Time.h>

REGISTER_OBJECT_FACTORY(CClock, "ovaCClock");

CClock::CClock(OMK::Controller& rController, const OMK::ObjectDescriptor& rObjectDescriptor)
	:OMK::SimulatedObject(rController, rObjectDescriptor)
{
	int l_iCurrentTime=(int)System::Time::getTime();

	m_iCurrentSimulatedTime=0;
	m_iCurrentTime=l_iCurrentTime;

	m_iLastSimulatedTime=0;
	m_iLastTime=l_iCurrentTime;
}

CClock::~CClock(void)
{
}

void CClock::compute(void)
{
	m_iCurrentSimulatedTime=this->getSimulatedDate();
	m_iCurrentTime=(int)System::Time::getTime();

	int l_iTimeDifference=(m_iCurrentSimulatedTime-m_iLastSimulatedTime)-(m_iCurrentTime-m_iLastTime);
	if(l_iTimeDifference>0)
	{
		System::Time::sleep(l_iTimeDifference);
	}

	m_iLastSimulatedTime=m_iCurrentSimulatedTime;
	m_iLastTime=m_iCurrentTime;
}
