#include "IConnection.h"
#include "IConnectionServer.h"
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
	class CConnectionServer : virtual public TConnection<IConnectionServer>
	{
	public:

		virtual boolean listen(
			uint32 ui32Port)
		{
			if(!open())
			{
				return false;
			}

			struct sockaddr_in l_oLocalHostAddress;
			memset(&l_oLocalHostAddress, 0, sizeof(l_oLocalHostAddress));
			l_oLocalHostAddress.sin_family=AF_INET;
			l_oLocalHostAddress.sin_port=htons((unsigned short)ui32Port);
			l_oLocalHostAddress.sin_addr.s_addr=INADDR_ANY;

			if(::bind(m_i32Socket, (struct sockaddr*)&l_oLocalHostAddress, sizeof(l_oLocalHostAddress))==-1)
			{
				return false;
			}

			if(::listen(m_i32Socket, 10)==-1)
			{
				return false;
			}

			return true;
		}

		virtual IConnection* accept(void)
		{
			struct sockaddr_in l_oClientAddress;
#if defined Socket_OS_Linux
			socklen_t l_iClientAddressSize=sizeof(l_oClientAddress);
#elif defined Socket_OS_Windows
			int32 l_iClientAddressSize=sizeof(l_oClientAddress);
#else
#endif
			int32 l_i32ClientSocket=::accept(m_i32Socket, (struct sockaddr*)&l_oClientAddress, &l_iClientAddressSize);
			if(l_iClientAddressSize==-1)
			{
				return NULL;
			}
			return new TConnection<IConnection>(l_i32ClientSocket);
		}
	};

	IConnectionServer* createConnectionServer(void)
	{
		return new CConnectionServer();
	}
};
