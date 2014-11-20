#ifndef CHESS_STATIC_FOREST_H
#define CHESS_STATIC_FOREST_H

#include <climits>
#include <memory>
#include <vector>
#include <glog/logging.h>

namespace chess {

template<typename DataType> class StaticForest {
  public:
    typedef uint32_t Index;
    enum : uint32_t { NULL_INDEX = std::numeric_limits<Index>::max() };
    struct Node {
      template<typename T> Node(T&& data) : data_(data) {}
      DataType data_;
      std::vector<Index> children_;
    };

    template<typename T> Index add(T&& data) {
      container.emplace_back(data);
      return container.size() - 1;
    }

    template<typename T> Index add(T&& data, Index parent) {
      container.emplace_back(data);
      Index idx = container.size() - 1;
      if (parent != NULL_INDEX) container[parent].children_.push_back(idx);
      return idx;
    }

    void addChild(Index parent, Index child) {
      container[parent].children_.push_back(child);
    }

    const Node& node(Index idx) const { 
      CHECK_LT(idx, container.size());
      return container[idx]; 
    }

    template<typename Callback>
    void dfs(Index rootIdx, int depth, Callback cb) const {
      auto curNode = node(rootIdx);
      cb(curNode.data_, depth);
      for (auto& child : curNode.children_) {
        dfs(child, depth + 1, cb);
      }
    }
    size_t size() const { return container.size(); }
    void clear() { container.clear(); }
  private:
    std::vector<Node> container;
};  

}
#endif
