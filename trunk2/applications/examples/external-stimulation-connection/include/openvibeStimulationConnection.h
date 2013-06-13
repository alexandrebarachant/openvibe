#ifndef __openvibeStimulationConnection_hpp__
#define __openvibeStimulationConnection_hpp__

#include <sys/timeb.h>
#include <boost/interprocess/ipc/message_queue.hpp>

#include <iostream> // log

namespace OpenViBE
{
	class StimulationConnection
	{
		private:
			typedef unsigned long long uint64;

		public:
			/**
			 * Create a new OpenvibeStimulationConnection object.
			 *
			 * Initiates the message queue.
			 *
			 * @param queueName The name of the queue.
			 * @throw boost::interprocess::interprocess_exception Throws an
			 * interprocess exception in the queue fails to be created.
			 */
			StimulationConnection(std::string queueName = "openvibeExternalStimulations");

			/**
			 * Send a stimulation to the OpenViBE Acquisition server on the initiated
			 * queue.
			 *
			 * @throw boost::interprocess::interprocess_exception Throws an exception
			 * if the message sending failed.
			 */
			void sendStimulation(uint64 stimulationIdentifier);

		private:

			std::string m_messageQueueName;
			int m_chunkLength;
			int m_maxMessages;

			// openvibe currently uses messages of length of 3

			boost::interprocess::message_queue* m_messageQueue;

	};

	StimulationConnection::StimulationConnection(std::string queueName) :
		m_messageQueueName(queueName),
		m_chunkLength(3),
		m_maxMessages(5000)
	{
		boost::interprocess::message_queue::remove(m_messageQueueName.c_str());

		try
		{
			m_messageQueue = new boost::interprocess::message_queue(
					boost::interprocess::create_only,
					m_messageQueueName.c_str(),
					m_maxMessages,
					m_chunkLength * sizeof(uint64)
					);
		}
		catch(boost::interprocess::interprocess_exception &exception)
		{
			std::cout << exception.what() << std::endl; // log
			throw;
		}
	}

	void StimulationConnection::sendStimulation(uint64 stimulationIdentifier)
	{
		struct timeb currentTime;

		ftime(&currentTime);

		uint64 stimulationTime = currentTime.time * 1000 + currentTime.millitm;

		uint64 message[3];

		message[0] = 0; // unused at the moment
		message[1] = stimulationIdentifier;
		message[2] = stimulationTime;

		try
		{
			m_messageQueue->send(&message, sizeof(message), 0);
		}
		catch(boost::interprocess::interprocess_exception &exception)
		{
			std::cout << exception.what() << std::endl; // log
			throw;
		}

	}
}

#endif // __openvibeStimulationConnection_hpp__
