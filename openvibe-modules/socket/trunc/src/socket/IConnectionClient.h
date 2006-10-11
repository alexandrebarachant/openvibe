#ifndef __Socket_IConnectionClient_H__
#define __Socket_IConnectionClient_H__

#include "IConnection.h"

namespace Socket
{
	class Socket_API IConnectionClient : virtual public Socket::IConnection
	{
	public:

		virtual Socket::boolean connect(
			const char* sServerName,
			Socket::uint32 ui32ServerPort)=0;

	};

	extern Socket_API Socket::IConnectionClient* createConnectionClient(void);
};

#endif // __Socket_IConnectionClient_H__
