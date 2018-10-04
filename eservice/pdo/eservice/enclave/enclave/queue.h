/* Copyright 2018 Intel Corporation
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>

template <typename T>
class EnclaveQueue
{
 public:

  T pop()
  {
    printf("EnclaveQueue - POPPING!!!\n");
    std::unique_lock<std::mutex> mlock(mutex_);
    while (queue_.empty())
    {
      printf("EnclaveQueue - EMPTY: WAITING TO POP!!\n");
      cond_.wait(mlock);
      printf("EnclaveQueue - POPPED!!\n");
    }

    auto item = queue_.front();
    queue_.pop();
    return item;
  }

  void pop(T& item)
  {
    printf("EnclaveQueue - POPPING!!!\n");
    std::unique_lock<std::mutex> mlock(mutex_);
    while (queue_.empty())
    {
      printf("EnclaveQueue - EMPTY: WAITING TO POP!!\n");
      cond_.wait(mlock);
      printf("EnclaveQueue - POPPED!!\n");
    }
    item = queue_.front();
    queue_.pop();
  }

  void push(const T& item)
  {
    printf("EnclaveQueue - PUSHING!!! %i\n",(int) item);
    std::unique_lock<std::mutex> mlock(mutex_);
    queue_.push(item);
    mlock.unlock();
    cond_.notify_one();
  }

  void push(T&& item)
  {
    printf("EnclaveQueue - PUSHING!!! %i\n",(int) item);
    std::unique_lock<std::mutex> mlock(mutex_);
    queue_.push(std::move(item));
    mlock.unlock();
    cond_.notify_one();
  }

 private:
  std::queue<T> queue_;
  std::mutex mutex_;
  std::condition_variable cond_;
};
