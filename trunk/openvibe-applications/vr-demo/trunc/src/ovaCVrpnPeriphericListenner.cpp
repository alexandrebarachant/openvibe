#include "ovaCVrpnPeriphericListenner.h"

REGISTER_OBJECT_FACTORY(CVrpnPeriphericListenner, "ovaCVrpnPeriphericListenner");

CVrpnPeriphericListenner::CVrpnPeriphericListenner(OMK::Controller& rController, const OMK::ObjectDescriptor& rObjectDescriptor)
	:CAbstractVrpnPeripheric(rController, rObjectDescriptor)
{
}

CVrpnPeriphericListenner::~CVrpnPeriphericListenner(void)
{
}

void CVrpnPeriphericListenner::compute(void)
{
	CAbstractVrpnPeripheric::compute();

	while(!m_vButton.empty())
	{
		VrpnButtonState& l_rVrpnButtonState=m_vButton.front();

		fireValuedSignal(g_sVrpnButtonStateUpdate, VrpnButtonStateType(l_rVrpnButtonState));

		m_vButton.pop_front();
	}

	if(!m_vAnalog.empty())
	{
		VrpnAnalogState& l_rVrpnAnalogState=m_vAnalog.front();

		fireValuedSignal(g_sVrpnAnalogStateUpdate, VrpnAnalogStateType(l_rVrpnAnalogState));

		m_vAnalog.pop_front();
	}
}
