#include "IConnection.h"
#include "IConnectionClient.h"
#include "IConnection.inl"

#include <string.h>
#include <iostream>

#if defined Socket_OS_Linux
 #include <netinet/in.h>
 #include <netinet/tcp.h>
 #include <netdb.h>
 #include <unistd.h>
 #include <fcntl.h>
 #include <errno.h>
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
			uint32 ui32ServerPort,
			uint32 ui32TimeOut)
		{
			long l_iValue;
			if(!open())
			{
				return false;
			}

			// Looks up host name
			struct hostent* l_pServerHostEntry=gethostbyname(sServerName);
			if(!l_pServerHostEntry)
			{
				close();
				return false;
			}

			// Sets non blocking
			if((l_iValue=::fcntl(m_i32Socket, F_GETFL, NULL))<0)
			{
				close();
				return false;
			}
			l_iValue|=O_NONBLOCK;
			if(::fcntl(m_i32Socket, F_SETFL, l_iValue)<0)
			{
				close();
				return false;
			}

			// Connects
			struct sockaddr_in l_oServerAddress;
			memset(&l_oServerAddress, 0, sizeof(l_oServerAddress));
			l_oServerAddress.sin_family=AF_INET;
			l_oServerAddress.sin_port=htons((unsigned short)ui32ServerPort);
			l_oServerAddress.sin_addr=*((struct in_addr*)l_pServerHostEntry->h_addr);
			if(::connect(m_i32Socket, (struct sockaddr*)&l_oServerAddress, sizeof(struct sockaddr))<0)
			{
				if(errno==EINPROGRESS)
				{
					// Performs time out
					if(ui32TimeOut==0xffffffff)
					{
						ui32TimeOut=125;
					}

					struct timeval l_oTimeVal;
					l_oTimeVal.tv_sec=(ui32TimeOut/1000);
					l_oTimeVal.tv_usec=((ui32TimeOut-l_oTimeVal.tv_sec*1000)*1000);

					fd_set l_oWriteFileDescriptors;
					FD_ZERO(&l_oWriteFileDescriptors);
					FD_SET(m_i32Socket, &l_oWriteFileDescriptors);

					if(::select(m_i32Socket+1, NULL, &l_oWriteFileDescriptors, NULL, &l_oTimeVal)<0)
					{
						close();
						return false;
					}
					if(!FD_ISSET_PROXY(m_i32Socket, &l_oWriteFileDescriptors))
					{
						close();
						return false;
					}

					// Checks error status
					int l_iOption;
					socklen_t l_iOptionLength=sizeof(l_iOption);
					::getsockopt(m_i32Socket, SOL_SOCKET, SO_ERROR, (void*)(&l_iOption), &l_iOptionLength);
					if(l_iOption!=0)
					{
						close();
						return false;
					}
				}
				else
				{
					close();
					return false;
				}
			}

			// Sets back to blocking
			if((l_iValue=::fcntl(m_i32Socket, F_GETFL, NULL))<0)
			{
				close();
				return false;
			}
			l_iValue&=~O_NONBLOCK;
			if(::fcntl(m_i32Socket, F_SETFL, l_iValue)<0)
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
