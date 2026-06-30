#include "value.hpp"
#include <iostream>
#include <unordered_set>
#include <vector>

Value::Value(double data_value, std::vector<ValuePtr> children,
             std::string operation)
    : data(data_value), grad(0.0), op(std::move(operation)),
      backward_fn([]() {})

{
  previous.reserve(children.size());

  for (const auto& child : children) {
    previous.push_back(child);
  }
}

void Value::backward() {
  std::vector<ValuePtr> topo;
  std::unordered_set<Value*> visited;

  build_topology(shared_from_this(), topo, visited);

  grad = 1.0;

  for (auto it = topo.rbegin(); it != topo.rend(); ++it) {
    (*it) -> backward_fn();
  }
}

void Value::print() const {
  std::cout << "Value - data: " << data << ", grad: " << grad << "\n";
}

void Value::build_topology(const ValuePtr &node, std::vector<ValuePtr> &topo,
                           std::unordered_set<Value*>& visited) {
  if (visited.find(node.get()) != visited.end()) {
    return;
  }
}
