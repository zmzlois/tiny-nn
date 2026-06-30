#pragma once

#include <functional>
#include <memory>
#include <string>
#include <unordered_set>
#include <vector>


class Value;
using ValuePtr = std::shared_ptr<Value>;

class Value : public std::enable_shared_from_this<Value> {
public:
  double data;
  double grad;
  std::string op;

  std::function<void()> backward_fn;
  std::vector<std::weak_ptr<Value>> previous;

  explicit Value(double data_value, std::vector<ValuePtr> children = {},
                 std::string operation = "");

  void backward();
  void print() const; // don't modify original value

private:
  static void build_topology(const ValuePtr &node, std::vector<ValuePtr> &topo,
    std::unordered_set<Value*>& visited
  );
};

// replace magic method - these are private so user can call backward() only
ValuePtr value(double data);
ValuePtr add(const ValuePtr& left, const ValuePtr& right);
ValuePtr mul(const ValuePtr& left, const ValuePtr& right);
ValuePtr power(const ValuePtr& base, double exponent);
ValuePtr relu(const ValuePtr& input);
ValuePtr neg(const ValuePtr& input);
ValuePtr sub(const ValuePtr& left, const ValuePtr &right);
ValuePtr div(const ValuePtr& left, const ValuePtr& right);
