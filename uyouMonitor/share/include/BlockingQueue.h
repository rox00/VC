// Use of this source code is governed by a BSD-style license
// that can be found in the License file.
//
// Author: Shuo Chen (chenshuo at chenshuo dot com)

#ifndef MUDUO_BASE_BLOCKINGQUEUE_H
#define MUDUO_BASE_BLOCKINGQUEUE_H

#include <boost/noncopyable.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/thread/condition_variable.hpp>
#include <deque>
#include <assert.h>

template<typename T>
class BlockingQueue : boost::noncopyable
{
public:
	BlockingQueue()
		: mutex_(),
		notEmpty_(),
		queue_()
	{
	}

	void put(const T& x)
	{
		boost::mutex::scoped_lock lock(mutex_);
		queue_.push_back(x);
		notEmpty_.notify_one(); // TODO: move outside of lock
	}

	T take()
	{
		boost::mutex::scoped_lock lock(mutex_);
		while (queue_.empty())
		{
			notEmpty_.wait(lock);
		}
		assert(!queue_.empty());
		T front(queue_.front());
		queue_.pop_front();
		return front;
	}

	size_t size() const
	{
		boost::mutex::scoped_lock  lock(mutex_);
		return queue_.size();
	}

private:
	mutable boost::mutex mutex_;
	boost::condition_variable    notEmpty_;
	std::deque<T>     queue_;
};

#endif  // MUDUO_BASE_BLOCKINGQUEUE_H
