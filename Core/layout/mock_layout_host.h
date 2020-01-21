// Copyright 2020 Infinite Synthesis(T.C.V.). All rights reserved.

#ifndef STARLIGHT_LAYOUT_MOCK_LAYOUT_HOST_H_
#define STARLIGHT_LAYOUT_MOCK_LAYOUT_HOST_H_

#include <queue>

#include "layout/layout_node.h"

namespace starlight {

class MockLayoutHost {
 public:
  MockLayoutHost() { SetUp(); }
  ~MockLayoutHost() { TearDown(); }

  LayoutNode* body() { return root_; }

 private:
  void SetUp() {
    root_ = new LayoutNode();
    root_->SetStyle("flexDirection", "column");
  }

  void TearDown() {
    std::queue<LayoutNode*> node_queue;
    node_queue.push(root_);
    while (!node_queue.empty()) {
      LayoutNode* node = node_queue.front();
      LayoutNode* child = node->first_child();
      while (child) {
        node_queue.push(child);
        child = child->next();
      }
      delete node;
      node_queue.pop();
    }
    root_ = nullptr;
  }

  LayoutNode* root_;
};

}  // namespace starlight

#endif
