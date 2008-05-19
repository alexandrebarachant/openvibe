#ifndef __OpenViBEApplication_CVrpnPeriphericListenner_H__
#define __OpenViBEApplication_CVrpnPeriphericListenner_H__

#include "ovaCAbstractVrpnPeripheric.h"

class CVrpnPeriphericListenner : public CAbstractVrpnPeripheric
{
public:

	DECLARE_OBJECT_FACTORY(CVrpnPeriphericListenner);

	virtual void compute(void);
};

#endif // __OpenViBEApplication_CVrpnPeriphericListenner_H__
