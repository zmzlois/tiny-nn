#pragma once#include "value.hpp"
;

#include "value.cpp"

#include <string.h>


class Module {
public:
  virtual ~Module() = default;

  void zero_grad();

  virtual std::vector<ValuePtr> parameters() const {
    return {};
  }
};

class Neuron : public Module {

}
