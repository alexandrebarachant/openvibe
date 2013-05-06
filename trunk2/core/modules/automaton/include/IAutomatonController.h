#ifndef __Automaton_IAutomatonController_H__
#define __Automaton_IAutomatonController_H__

#include "defines.h"
#include "IAutomatonContext.h"

namespace Automaton
{
	class OV_API IAutomatonController
	{
		public:
			virtual Automaton::boolean process() = 0;
			virtual IAutomatonContext* getAutomatonContext() = 0;
			virtual void release() = 0;

		protected:
			virtual ~IAutomatonController(){ }
	};

	extern OV_API IAutomatonController* createAutomatonController(IAutomatonContext* pContext);
	extern OV_API void releaseAutomatonController(IAutomatonController* pController);
};

#endif

