#include "IConnection.h"

#if defined Socket_OS_Linux
 #include <sys/time.h>
 #include <sys/types.h>
 #include <sys/socket.h>
 // #include <netinet/in.h>
 // #include <netinet/tcp.h>
 #include <arpa/inet.h>
 #include <unistd.h>
 // #include <netdb.h>
 #include <time.h>
#elif defined Socket_OS_Windows
 #include <windows.h>
#else

#endif

#include <iostream>

namespace Socket
{
	template <class T> class TConnection : virtual public T
	{
	public:

		TConnection(void)
			:m_i32Socket(-1)
		{
		}

		TConnection(int32 i32Socket)
			:m_i32Socket(i32Socket)
		{
		}

	protected:

		virtual boolean open(void)
		{
			if(isConnected())
			{
				return false;
			}

#if defined Socket_OS_Linux
#elif defined Socket_OS_Windows
			int32 l_i32VersionHigh=2;
			int32 l_i32VersionLow=0;
			WORD l_oWinsockVersion=MAKEWORD(l_i32VersionHigh, l_i32VersionLow);
			WSADATA l_oWSAData;
			::WSAStartup(l_oWinsockVersion, &l_oWSAData);
#else
#endif

			m_i32Socket=::socket(AF_INET, SOCK_STREAM, 0);
			if(m_i32Socket==-1)
			{
				return false;
			}

			return true;
		}

	public:

		virtual boolean close(void)
		{
			if(!isConnected())
			{
				return false;
			}

			::shutdown(m_i32Socket, SHUT_RDWR);

#if defined Socket_OS_Linux
#elif defined Socket_OS_Windows
			::closesocket(m_i32Socket);
			::WSACleanup();
#else
#endif

			m_i32Socket=-1;
			return true;
		}

		virtual boolean isReadyToSend(
			uint32 ui32TimeOut=0) const
		{
			if(!isConnected())
			{
				return false;
			}

			struct timeval l_oTimeVal;
			l_oTimeVal.tv_sec=(ui32TimeOut/1000);
			l_oTimeVal.tv_usec=((ui32TimeOut-l_oTimeVal.tv_sec*1000)*1000);

			fd_set l_oWriteFileDescriptors;
			FD_ZERO(&l_oWriteFileDescriptors);
			FD_SET(m_i32Socket, &l_oWriteFileDescriptors);

			if(::select(m_i32Socket+1, NULL, &l_oWriteFileDescriptors, NULL, &l_oTimeVal)<0)
			{
				return false;
			}
			if(!FD_ISSET(m_i32Socket, &l_oWriteFileDescriptors))
			{
				return false;
			}
			return true;
		}

		virtual boolean isReadyToReceive(
			uint32 ui32TimeOut=0) const
		{
			if(!isConnected())
			{
				return false;
			}

			struct timeval l_oTimeVal;
			l_oTimeVal.tv_sec=(ui32TimeOut/1000);
			l_oTimeVal.tv_usec=((ui32TimeOut-l_oTimeVal.tv_sec*1000)*1000);

			fd_set l_oReadFileDescriptors;
			FD_ZERO(&l_oReadFileDescriptors);
			FD_SET(m_i32Socket, &l_oReadFileDescriptors);

			if(::select(m_i32Socket+1, &l_oReadFileDescriptors, NULL, NULL, &l_oTimeVal)<0)
			{
				return false;
			}
			if(!(FD_ISSET(m_i32Socket, &l_oReadFileDescriptors)))
			{
				return false;
			}
			return true;
		}

		virtual uint32 sendBuffer(
			const void* pBuffer,
			const uint32 ui32BufferSize)
		{
			if(!isConnected())
			{
				return 0xffffffff;
			}
#if 0
			int l_iTrue=1;
			setsockopt(m_i32Socket, IPPROTO_TCP, TCP_NODELAY, (char*)&l_iTrue, sizeof(l_iTrue));
#endif
			int l_iResult=::send(m_i32Socket, pBuffer, ui32BufferSize, 0);
			if(l_iResult<0)
			{
				close();
			}
			return l_iResult<0?0:(uint32)l_iResult;
		}

		virtual uint32 receiveBuffer(
			void* pBuffer,
			const uint32 ui32BufferSize)
		{
			if(!isConnected())
			{
				return 0xffffffff;
			}
#if 0
			int l_iTrue=1;
			setsockopt(m_i32Socket, IPPROTO_TCP, TCP_NODELAY, (char*)&l_iTrue, sizeof(l_iTrue));
#endif
			int l_iResult=::recv(m_i32Socket, pBuffer, ui32BufferSize, 0);
			if(l_iResult<0)
			{
				close();
			}
			return l_iResult<0?0:(uint32)l_iResult;
		}

		virtual boolean isConnected(void) const
		{
			return m_i32Socket!=-1;
		}

		virtual void release(void)
		{
			if(isConnected())
			{
				close();
			}
			delete this;
		}

	protected:

		int32 m_i32Socket;
	};
};

