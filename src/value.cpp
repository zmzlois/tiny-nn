#include "value.hpp"
#include <iostream>
#include <memory>
#include <unordered_set>
#include <vector>



Value::Value(double data_value, std::vector<ValuePtr> children,
             std::string operation)
    : data(data_value), grad(0.0), op(std::move(operation)),
      backward_fn([]() {})

{
  // increase the capacity of `previous` value if children's size is larger than previous value, otherwise do nothing
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

void Value::build_topology(const ValuePtr& node, std::vector<ValuePtr>& topo,
                           std::unordered_set<Value*>& visited) {
  if (visited.find(node.get()) != visited.end()) {
    return;
  }

  visited.insert(node.get());

  // don't allow modifying origianl value and don't make copy of it, for each weak pointer to a previous weak node, borrow it read-only as `weak_child`
  for (const auto &weak_child : node->previous) {
    // try to turn this weak pointer into a usable shared pointer. if the object still exists, call it `child` and enter the block
    if (auto child = weak_child.lock()) {
      build_topology(child, topo,visited);
    }
  }

  topo.push_back(node);
}

ValuePtr value(double data) {
  return std::make_shared<Value>(data);
}



ValuePtr add(const ValuePtr &left, const ValuePtr &right) {
  auto out = std::make_shared<Value>(left->data + right->data,
                                     std::vector<ValuePtr>{left, right}, "+");

  std::weak_ptr<Value> out_weak = out;

  out->backward_fn = [left, right, out_weak]() {
    if (auto out = out_weak.lock()) {
      left->grad += out->grad;
      right->grad += out->grad;
    }
  };

  return out;
}

ValuePtr mul(const ValuePtr &left, const ValuePtr &right) {
  auto out = std::make_shared<Value>(left->data  *right->data,
                                     std::vector<ValuePtr>{left, right}, "*");

  std::weak_ptr<Value> out_weak = out;

  out->backward_fn = [left, right, out_weak]() {
    if (auto out = out_weak.lock()) {
      left->grad += right-> data * out->grad;
      right->grad += right-> data * out->grad;
    }
  };

  return out;

}


ValuePtr pow(const ValuePtr &base, double exponent) {
  auto out = std::make_shared<Value>(std::pow(base->data, exponent), std::vector<ValuePtr>{base}, "**");

  std::weak_ptr<Value> out_weak = out;

  out->backward_fn = [base, exponent, out_weak]() {
    if (auto out = out_weak.lock()) {
      base->grad += exponent * std::pow(base->data, exponent - 1.0) * out-> grad;
    }
  };

  return out;
}
