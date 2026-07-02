#pragma once


#include "value.hpp"
#include <vector>
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
public:
  std::vector<ValuePtr> weights;
  ValuePtr bias;
  bool nonlin;

  explicit Neuron(int input_count, bool use_nonlin = true);

  ValuePtr forward(const std::vector<ValuePtr> &input) const;

  std::vector<ValuePtr> parameters() const override;

  std::string repr() const;
};


class Layer : public Module {
public:
  std::vector<Neuron> neurons;

  Layer(int input_count, int output_count, bool use_nonlin = true);

  std::vector<ValuePtr> forward(const std::vector<ValuePtr> &inputs) const;

  std::vector<ValuePtr> parameters() const override;

  std::string repr() const;

};


// multi-layer perceptron
class MLP : public Module {

public:
  std::vector<Layer> layer;

  MLP(int input_count, const std::vector<int> &output_counts);

  std::vector<ValuePtr> forward(const std::vector<ValuePtr> &inputs) const;

  std::vector<ValuePtr> parameters() const override;

  std::string repr() const;
};
