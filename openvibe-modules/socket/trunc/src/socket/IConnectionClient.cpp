#include "IConnection.h"
#include "IConnectionClient.h"
#include "IConnection.inl"

#include <string.h>

#if defined Socket_OS_Linux
 #include <netinet/in.h>
 #include <netinet/tcp.h>
 #include <netdb.h>
#elif defined Socket_OS_Windows
#else
#endif

namespace Socket
{
	class CConnectionClient : virtual public TConnection<IConnectionClient>
	{
	public:

		virtual boolean connect(
			const char* sServerName,
			uint32 ui32ServerPort)
		{
			if(!open())
			{
				return false;
			}

			struct hostent* l_pServerHostEntry=gethostbyname(sServerName);
			if(!l_pServerHostEntry)
			{
				return false;
			}

			struct sockaddr_in l_oServerAddress;
			memset(&l_oServerAddress, 0, sizeof(l_oServerAddress));
			l_oServerAddress.sin_family=AF_INET;
			l_oServerAddress.sin_port=htons((unsigned short)ui32ServerPort);
			l_oServerAddress.sin_addr=*((struct in_addr*)l_pServerHostEntry->h_addr);
			if(::connect(m_i32Socket, (struct sockaddr*)&l_oServerAddress, sizeof(struct sockaddr))==-1)
			{
				close();
				return false;
			}
			return true;
		}
	};

	IConnectionClient* createConnectionClient(void)
	{
		return new CConnectionClient();
	}
};
