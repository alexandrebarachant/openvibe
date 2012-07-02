#ifndef __OpenViBE_AcquisitionServer_TCustomConcurrentQueue_H__
#define __OpenViBE_AcquisitionServer_TCustomConcurrentQueue_H__
 
#include <iostream>  
#include <boost/thread.hpp>  
#include <boost/date_time.hpp>  
#include <queue>

using namespace std;

namespace OpenViBEAcquisitionServer

{
	/**
	* class TCustomConcurrentQueue
	* Author : Anton Andreev (Gipsa-lab), patched by Laurent Bonnet (Inria)
	* Integration Date : 02.07.2010
	* This class implement a concurrent queue threaded using boost::thread.
	**/
	template<typename Data>
	class TCustomConcurrentQueue
	{

	private:

		std::queue<Data> m_qDataQueue;
		mutable boost::mutex m_oMutex;
		boost::condition_variable m_oConditionVariable;

	public:
		void push(Data const& data)
		{	
			boost::mutex::scoped_lock lock(m_oMutex);
			m_qDataQueue.push(data);
			lock.unlock();
			m_oConditionVariable.notify_one();
		}

		bool empty() const
		{
			boost::mutex::scoped_lock lock(m_oMutex);
			return m_qDataQueue.empty();
		}

		bool tryPop(Data& rPoppedValue)
		{
			boost::mutex::scoped_lock lock(m_oMutex);
			if(m_qDataQueue.empty())
			{
				return false;
			}

			rPoppedValue=m_qDataQueue.front();
			m_qDataQueue.pop();
			return true;
		}

		bool waitAndPop(Data& rPoppedValue)
		{
			boost::mutex::scoped_lock lock(m_oMutex);
		
			m_oConditionVariable.wait(lock);
		

			rPoppedValue=m_qDataQueue.front();
			m_qDataQueue.pop();
			return true;
		}

	};

};


#endif
