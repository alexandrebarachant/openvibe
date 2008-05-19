#include "ovaCClock.h"

#include <unistd.h>
#include <time.h>
#include <sys/time.h>

REGISTER_OBJECT_FACTORY(CClock, "ovaCClock");

CClock::CClock(OMK::Controller& rController, const OMK::ObjectDescriptor& rObjectDescriptor)
	:OMK::SimulatedObject(rController, rObjectDescriptor)
{
	struct timeval l_oTimeValue;
	gettimeofday(&l_oTimeValue, NULL);
	int l_iCurrentTime=l_oTimeValue.tv_sec*1000+l_oTimeValue.tv_usec/1000;

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
	struct timeval l_oTimeValue;
	gettimeofday(&l_oTimeValue, NULL);

	m_iCurrentSimulatedTime=this->getSimulatedDate();
	m_iCurrentTime=l_oTimeValue.tv_sec*1000+l_oTimeValue.tv_usec/1000;

	int l_iTimeDifference=(m_iCurrentSimulatedTime-m_iLastSimulatedTime)-(m_iCurrentTime-m_iLastTime);
	if(l_iTimeDifference>0)
	{
		::usleep(1000*l_iTimeDifference);
	}

	m_iLastSimulatedTime=m_iCurrentSimulatedTime;
	m_iLastTime=m_iCurrentTime;
}
