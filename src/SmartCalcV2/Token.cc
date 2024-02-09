#include "Token.h"

#include "cmath"
namespace s21 {
void Token::CreateTokenMap(std::map<std::string, s21::Token>& token_map) {
  using std::initializer_list;
  using std::pair;
  using std::string;
  using namespace s21;
  initializer_list<pair<const string, Token>> list = {
      {" ", Token("space", kDefault, kNone, kNumber, nullptr)},
      {"x", Token("x", kDefault, kNone, kNumber, nullptr)},
      {"(", Token("(", kDefault, kNone, kOpenBracket, nullptr)},
      {")", Token(")", kDefault, kNone, kClosedBracket, nullptr)},
      {"+", Token("+", kLow, kLeft, kBinaryOperator, std::plus<double>())},
      {"-", Token("-", kLow, kLeft, kBinaryOperator, std::minus<double>())},
      {"*",
       Token("*", kMedium, kLeft, kBinaryOperator, std::multiplies<double>())},
      {"/",
       Token("/", kMedium, kLeft, kBinaryOperator, std::divides<double>())},
      {"^", Token("^", kHigh, kRight, kBinaryOperator, powl)},
      {"mod", Token("mod", kMedium, kLeft, kBinaryOperator, fmodl)},
      {"cos", Token("cos", kFunction, kRight, kUnaryFunction, cosl)},
      {"sin", Token("sin", kFunction, kRight, kUnaryFunction, sinl)},
      {"tan", Token("tan", kFunction, kRight, kUnaryFunction, tanl)},
      {"acos", Token("acos", kFunction, kRight, kUnaryFunction, acosl)},
      {"asin", Token("asin", kFunction, kRight, kUnaryFunction, asinl)},
      {"atan", Token("atan", kFunction, kRight, kUnaryFunction, atanl)},
      {"sqrt", Token("sqrt", kFunction, kRight, kUnaryFunction, sqrtl)},
      {"ln", Token("ln", kFunction, kRight, kUnaryFunction, logl)},
      {"log", Token("log", kFunction, kRight, kUnaryFunction, log10l)},
      {"e", Token("e", kDefault, kNone, kNumber, M_E)},
      {"exp", Token("exp", kFunction, kRight, kUnaryFunction, expl)},

  };
  for (auto it = list.begin(); it != list.end(); ++it) {
    token_map.insert(*it);
  }
}

void Token::setNumericTofunction() {
  std::string name = GetInput();
  if (isdigit(name[0])) {
    double num_double = std::stod(GetInput());
    SetFunction(num_double);
  }
}

std::string Token::GetInput() const { return input_; }
Precedence Token::GetPrecedence() const { return precedence_; }
Associativity Token::GetAssociativity() const { return associativity_; }
Type Token::GetType() const { return type_; }
function_variant Token::GetFunction() const { return function_; }
void Token::SetFunction(const function_variant& function) {
  function_ = function;
}

}  // namespace s21

// parse string
// понять что такое бинарный, а остальное в унарный скидывать
