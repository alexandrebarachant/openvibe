#include "ovtkIBoxAlgorithmContextHandler.h"

#include <stack>

using namespace OpenViBE;
using namespace OpenViBEToolkit;
using namespace std;

namespace OpenViBEToolkit
{
	class CBoxAlgorithmContextHandler : virtual public IBoxAlgorithmContextHandler
	{
	public:

		virtual boolean push(Plugins::IBoxAlgorithmContext* pBoxAlgorithmContext);
		virtual boolean pop(void);
		virtual Plugins::IBoxAlgorithmContext* top(void);

		_IsDerivedFromClass_Final_(IBoxAlgorithmContextHandler, OVTK_ClassId_);

	protected:

		stack<Plugins::IBoxAlgorithmContext*> m_vBoxAlgorithmContext;
	};
};

boolean CBoxAlgorithmContextHandler::push(Plugins::IBoxAlgorithmContext* pBoxAlgorithmContext)
{
	m_vBoxAlgorithmContext.push(pBoxAlgorithmContext);
	return true;
}

boolean CBoxAlgorithmContextHandler::pop(void)
{
	if(m_vBoxAlgorithmContext.size()==0)
	{
		return false;
	}
	m_vBoxAlgorithmContext.pop();
	return true;
}

Plugins::IBoxAlgorithmContext* CBoxAlgorithmContextHandler::top(void)
{
	if(m_vBoxAlgorithmContext.size()==0)
	{
		return NULL;
	}
	return m_vBoxAlgorithmContext.top();
}

IBoxAlgorithmContextHandler* OpenViBEToolkit::createBoxAlgorithmContextHandler(void)
{
	return new CBoxAlgorithmContextHandler();
}

void OpenViBEToolkit::releaseBoxAlgorithmContextHandler(IBoxAlgorithmContextHandler* pBoxAlgorithmContextHandler)
{
	delete pBoxAlgorithmContextHandler;
}
