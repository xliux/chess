#ifndef CHESS_STATIC_FOREST_H
#define CHESS_STATIC_FOREST_H

#include <climits>
#include <memory>
#include <type_traits>
#include <vector>

#include <glog/logging.h>

namespace chess {

template<typename DataType> class StaticForest {
  public:
    typedef uint32_t Index;
    enum : uint32_t { NULL_INDEX = std::numeric_limits<Index>::max() };
    struct Node {
      // explicit Node(const DataType& data) : data_(data) {}
      
      template<typename T, typename std::enable_if<
          std::is_constructible<DataType, T>::value, bool>::type=0>
      explicit Node(T&& data) : data_(std::forward<T>(data)) {}
      
      DataType data_;
      std::vector<Index> children_;
    };

    template<typename T> Index add(T&& data) {
      container.emplace_back(std::forward<T>(data));
      return container.size() - 1;
    }

    template<typename T> Index add(T&& data, Index parent) {
      container.emplace_back(std::forward<T>(data));
      container.emplace_back(data);
      Index idx = container.size() - 1;
      if (parent != NULL_INDEX) {
        CHECK_LT(parent, idx);
        container[parent].children_.push_back(idx);
      }
      return idx;
    }

    void addChild(Index parent, Index child) {
      container[parent].children_.push_back(child);
    }

    const Node& node(Index idx) const { 
      DCHECK_LT(idx, container.size());
      return container[idx]; 
    }

    size_t size() const { return container.size(); }
    void clear() { container.clear(); }
    size_t memsize() const { return container.capacity() * sizeof(Node); }

  private:
    std::vector<Node> container;
};  

}
#endif
