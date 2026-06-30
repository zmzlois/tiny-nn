#include "value.hpp"

Value::Value(double data_value, std::vector<ValuePtr> children,
             std::string operation)
    : data(data_value), grad(0.0), op(std::move(operation)),
      backward_fn([]() {})

{
  previous.reserve(children.size());

  for (const auto &child : children) {
    previous.push_back(child);
  }
}
