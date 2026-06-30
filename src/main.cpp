#include <iostream>
#include <memory>
#include <unordered_set>


class Value {
public:
  struct node {
    double data;
    double grad = 0.0;
    std::function<void()> _backward;
    std::unordered_set<std::shared_ptr<Value>> _prev;
    std::string _op;
  };

};



int main(int argc, char* argv[]) {
  std::cout << "A tiny neural network with 300 lines of code in C++";
  return 0;
}
