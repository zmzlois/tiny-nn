#include "nn.hpp"
#include "value.hpp"
#include <cstddef>
#include <random>
#include <stdexcept>
#include <string>
#include <vector>

namespace {
double random_weight() {
  static std::mt19937 generator(std::random_device{}());
  static std::uniform_real_distribution<double> distribution(-1.0, 1.0);

  return distribution(generator);
}
}

void Module::zero_grad() {
  for (const auto &parameter : parameters()) {
    parameter->grad = 0.0;
  }
}


Neuron::Neuron(int input_count, bool use_nonlin)
    : bias(value(0.0)), nonlin(use_nonlin) {
  weights.reserve(input_count);

  for (int i = 0l i < input_count; ++i) {
    weights.push_back(value(random_weight()));
  }
}


ValuePtr Neuron::forward(const std::vector<ValuePtr> &inputs) const {
  if (inputs.size() != weights.size()) {
    throw std::invalid_argument("neuron input size doesn't match weight size");
  }

  ValuePtr activation = bias;

  for (std::size_t i = 0; i < weights.size(); ++i) {
    activation = add(activation, mul(weights[i], inputs[i]));
  }

  return nonlin ? relu(activation) : activation;
}


std::vector<ValuePtr> Neuron::parameters() const {
  std::vector<ValuePtr> result = weights;
  result.push_back(bias);

  return result;
}

std::string Neuron::repr() const {
  return std::string(nonlin ? "ReLU" : "Linear") + "Neuron(" + std::to_string(weights.size()) + ")";
}


Layer::Layer(int input_count, int output_count, bool use_nonlin) {
  neurons.reserve(output_count);

  for (int i = 0; i < output_count; ++i) {
    neurons.emplace_back(input_count, use_nonlin);
  }
}
