/******************************************************************************
 * Copyright 2017 The Apollo Authors. All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *****************************************************************************/

/**
 * @file:
 **/

#ifndef MODULES_PLANNING_COMMON_INDEXED_QUEUE_H_
#define MODULES_PLANNING_COMMON_INDEXED_QUEUE_H_

#include <memory>
#include <queue>
#include <unordered_map>

namespace apollo {
namespace planning {

template <typename I, typename T>
class IndexedQueue {
 public:
  IndexedQueue(std::size_t max_queue_size) : max_queue_size_(max_queue_size){};
  const T *Find(const I id) const {
    auto iter = map_.find(id);
    if (iter == map_.end()) {
      return nullptr;
    } else {
      return iter->second.get();
    }
  }

  const T *Latest() const {
    if (queue_.empty()) {
      return nullptr;
    }
    return Find(queue_.back().first);
  }

  bool Add(const I id, std::unique_ptr<T> ptr) {
    if (Find(id)) {
      return false;
    }
    if (!queue_.empty() && queue_.size() == max_queue_size_) {
      const auto &front = queue_.front();
      map_.erase(front.first);
      queue_.pop();
    }
    queue_.push(std::make_pair(id, ptr.get()));
    map_[id] = std::move(ptr);
    return true;
  }

 public:
  std::size_t max_queue_size_ = 0;
  std::queue<std::pair<I, const T *>> queue_;
  std::unordered_map<I, std::unique_ptr<T>> map_;
};

}  // namespace planning
}  // namespace apollo

#endif  // MODULES_PLANNING_COMMON_DATA_CENTER_H_
