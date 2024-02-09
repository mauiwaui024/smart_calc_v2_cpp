#ifndef TOKEN_H
#define TOKEN_H
#include <cmath>
#include <functional>
#include <iostream>
#include <map>
#include <string>
#include <variant>
namespace s21 {
enum Type {
  kNumber,               // числа
  kBinaryOperator,       // + - / * mod
  kUnaryPrefixOperator,  // - and + unary
  kUnaryPostfixOperator,
  kUnaryFunction,  // cos, sin, tan
  kOpenBracket,
  kClosedBracket,
  kNumTokenTypes
};

enum Precedence {
  kDefault,  // числа скобки - все то что не оператор и не функция
  kLow,      //  + -
  kMedium,   //  * / mod
  kHigh,     // возведение в степень
  kUnaryOperator,  // выше чем у возведения в степень чтобы отрицательные числа
                   // можно было записывать без скобок
  kFunction
};

enum Associativity {
  kNone,
  kLeft,  // у минуса и деления, и остатка от деления, если несколько раз подряд
          // ---- мы считаем слева направо
  kRight  //если степень написана неск раз подряд, считаем справа налево
};

template <class... Ts>
struct overloaded : Ts... {
  using Ts::operator()...;
};
template <class... Ts>
overloaded(Ts...) -> overloaded<Ts...>;

using unary_function = std::function<double(double)>;
using binary_function = std::function<double(double, double)>;
using function_variant =
    std::variant<double, unary_function, binary_function, std::nullptr_t>;
using XYGraph = std::pair<std::vector<double>, std::vector<double>>;

class Token {
 public:
  Token() = default;
  explicit Token(const std::string input, Precedence precedence,
                 Associativity associativity, Type type,
                 function_variant function)
      : input_(std::move(input)),
        precedence_(precedence),
        associativity_(associativity),
        type_(type),
        function_(std::move(function)) {}
  ~Token() = default;
  std::string GetInput() const;
  Precedence GetPrecedence() const;
  Associativity GetAssociativity() const;
  Type GetType() const;
  function_variant GetFunction() const;
  void SetFunction(const function_variant& function);

  static void CreateTokenMap(std::map<std::string, s21::Token>& token_map);
  void setNumericTofunction();

 private:
  std::string input_;
  Precedence precedence_;
  Associativity associativity_;
  Type type_;
  function_variant function_;
};
}  // namespace s21
#endif