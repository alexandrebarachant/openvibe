#ifndef __OpenViBEApplication_CAbstractVrpnPeripheric_H__
#define __OpenViBEApplication_CAbstractVrpnPeripheric_H__

#include "ova_defines.h"

class CDeviceInfo;

class CAbstractVrpnPeripheric : public OMK::SimulatedObject
{
public:

	DECLARE_OBJECT_FACTORY(CAbstractVrpnPeripheric);

	virtual void init(void);
	virtual void compute(void);

	std::list < VrpnButtonState > m_vButton;
	std::list < VrpnAnalogState > m_vAnalog;

	double m_dAnalogScale;
	double m_dAnalogOffset;

	std::string m_sDeviceName;
	CDeviceInfo* m_pDevice;
};

#endif
