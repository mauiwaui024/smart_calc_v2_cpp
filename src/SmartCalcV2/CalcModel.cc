
#include "CalcModel.h"

#include <iostream>
#include <regex>

#include "Token.h"
namespace s21 {
void CalcModel::Tokenize(const std::string& expression) {
  if (expression.length() > 255) {
    throw std::logic_error("Expression is too long");
  }
  std::string lowercaseExpression = toLowerCase(expression);
  Token::CreateTokenMap(token_map_);
  size_t start_index = 0;
  while (start_index != lowercaseExpression.length()) {
    if (isalpha(lowercaseExpression[start_index])) {
      std::string token = ReadWord(lowercaseExpression, start_index);
      TryToPushToken(token);
    } else if (isdigit(lowercaseExpression[start_index])) {
      double numericValue = ReadNumber(lowercaseExpression, start_index);
      Token numericToken(std::to_string(numericValue), kDefault, kNone, kNumber,
                         numericValue);
      input_.push(numericToken);
    } else {
      std::string token = ReadSingleChar(lowercaseExpression, start_index);
      TryToPushToken(token);
    }
  }
}
std::string CalcModel::toLowerCase(const std::string& str) {
  std::string result;
  for (char c : str) {
    result += std::tolower(static_cast<unsigned char>(c));
  }
  return result;
}
std::string CalcModel::ReadWord(const std::string& expression,
                                size_t& start_index) {
  std::regex word_regex("([a-z]+)");
  std::sregex_iterator regex_iterator = std::sregex_iterator(
      expression.begin() + start_index, expression.end(), word_regex);
  std::smatch match = *regex_iterator;
  start_index += match.length();
  return match.str();
}

double CalcModel::ReadNumber(const std::string& expression,
                             size_t& start_index) {
  std::regex double_regex("\\d+([.]\\d+)?(e([-+])?\\d+)?");
  std::sregex_iterator regex_iterator = std::sregex_iterator(
      expression.begin() + start_index, expression.end(), double_regex);
  std::smatch match = *regex_iterator;
  start_index += match.length();
  std::istringstream stream(match.str());
  double numericValue;
  stream >> numericValue;
  return numericValue;
}

std::string CalcModel::ReadSingleChar(const std::string& expression,
                                      size_t& start_index) {
  if (start_index < expression.length()) {
    std::string character(1, expression[start_index]);
    start_index++;
    return character;
  } else {
    return "";
  }
}

void CalcModel::TryToPushToken(std::string token) {
  auto token_map_it = token_map_.find(token);
  if (token_map_it == token_map_.end()) {
    throw std::logic_error("incorrect symbol: " + token);
  }
  input_.push(token_map_it->second);
}

void CalcModel::ModifyQueue() {
  std::queue<Token> modifiedQueue;
  while (!input_.empty()) {
    Token currentToken = input_.front();
    input_.pop();
    if (currentToken.GetInput() == "-") {
      bool isUnary;
      if (!modifiedQueue.empty() &&
          (modifiedQueue.back().GetType() == kClosedBracket ||
           modifiedQueue.back().GetType() == kNumber)) {
        isUnary = false;
      } else {
        isUnary = true;
      }
      Type newType = isUnary ? kUnaryPrefixOperator : kBinaryOperator;
      Precedence newPrecendece = isUnary ? kUnaryOperator : kLow;
      Associativity newAssociativity = isUnary ? kRight : kLeft;
      function_variant newFunction =
          isUnary ? std::negate<double>() : currentToken.GetFunction();
      Token token(currentToken.GetInput(), newPrecendece, newAssociativity,
                  newType, newFunction);
      modifiedQueue.push(token);
    } else if (currentToken.GetInput() == "+") {
      bool isUnary;
      if (!modifiedQueue.empty() &&
          (modifiedQueue.back().GetType() == kClosedBracket ||
           modifiedQueue.back().GetType() == kNumber)) {
        isUnary = false;
      } else {
        isUnary = true;
      }
      Type newType = isUnary ? kUnaryPrefixOperator : kBinaryOperator;
      Precedence newPrecendece = isUnary ? kUnaryOperator : kLow;
      Associativity newAssociativity = isUnary ? kRight : kLeft;
      // Определение унарной функции для плюса
      auto unary_plus = [](double x) { return x; };
      // Создание function_variant с унарным плюсом
      function_variant newFunction =
          isUnary ? unary_plus : currentToken.GetFunction();
      Token token(currentToken.GetInput(), newPrecendece, newAssociativity,
                  newType, newFunction);
      modifiedQueue.push(token);
    } else if (currentToken.GetType() != kNumber &&
               currentToken.GetInput() == " ") {
      continue;
    } else {
      modifiedQueue.push(currentToken);
    }
  }
  std::swap(input_, modifiedQueue);
}

void CalcModel::ValidateQueue() {
  std::queue<Token> tempQueue = input_;
  std::vector<int> allowedTypesForFirst = {1, 0, 1, 0, 1, 1, 0};
  std::vector<int> allowedTypesForLast = {1, 0, 0, 1, 0, 0, 1};
  if (tempQueue.empty()) {
    throw std::logic_error("Empty queue. Validation failed.");
  }
  Type firstTokenType = tempQueue.front().GetType();
  Type lastTokenType = tempQueue.back().GetType();
  if (firstTokenType >= 0 && firstTokenType < kNumTokenTypes) {
    if (allowedTypesForFirst[firstTokenType] != 1) {
      while (!input_.empty()) {
        input_.pop();
      }
      throw std::logic_error("Invalid first token type: " + firstTokenType);
    }
  } else {
    while (!input_.empty()) {
      input_.pop();
    }
    throw std::logic_error("Invalid first token type: " + firstTokenType);
  }
  if (lastTokenType >= 0 && lastTokenType < kNumTokenTypes) {
    if (allowedTypesForLast[lastTokenType] != 1) {
      while (!input_.empty()) {
        input_.pop();
      }
      throw std::logic_error("Invalid last token type.");
    }
  } else {
    while (!input_.empty()) {
      input_.pop();
    }
    throw std::logic_error("Invalid last token type.");
  }

  while (!tempQueue.empty()) {
    Token currentToken = tempQueue.front();
    tempQueue.pop();

    if (!tempQueue.empty()) {
      Token nextToken = tempQueue.front();
      if (!kAdjacencyMatrix[currentToken.GetType()][nextToken.GetType()]) {
        while (!input_.empty()) {
          input_.pop();
        }
        throw std::logic_error(
            "Invalid sequence of tokens: " + currentToken.GetInput() +
            " followed by " + nextToken.GetInput());
      }
    }
  }
}

void CalcModel::ConvertInfixToPostfix() {
  while (!input_.empty()) {
    switch (input_.front().GetType()) {
      case Type::kNumber:
        MoveTokenFromInputToOutput();
        break;
      case Type::kUnaryFunction:
      case Type::kUnaryPrefixOperator:
      case Type::kOpenBracket:
        MoveTokenFromInputToStack();
        break;
      case Type::kBinaryOperator:
        while (
            !stack_.empty() &&
            ((stack_.top().GetPrecedence() > input_.front().GetPrecedence()) ||
             (stack_.top().GetPrecedence() == input_.front().GetPrecedence() &&
              stack_.top().GetAssociativity() == kLeft))) {
          MoveTokenFromStackToOutput();
        }
        MoveTokenFromInputToStack();
        break;
      case Type::kClosedBracket:
        while (!stack_.empty() && stack_.top().GetType() != kOpenBracket) {
          MoveTokenFromStackToOutput();
        }
        if (!stack_.empty() && stack_.top().GetType() == kOpenBracket) {
          stack_.pop();
        } else if (!stack_.empty() &&
                   (stack_.top().GetType() == kUnaryFunction)) {
          MoveTokenFromStackToOutput();
        } else {
          throw std::logic_error("Malformed expression!");
        }
        input_.pop();
        break;
      default:
        break;
    }
  }
  if (input_.empty()) {
    while (!stack_.empty()) {
      MoveTokenFromStackToOutput();
    }
  }
}

// move functions
void CalcModel::MoveTokenFromInputToOutput() {
  if (!input_.empty()) {
    Token token = input_.front();
    input_.pop();
    output_.push(token);
  }
}
void CalcModel::MoveTokenFromInputToStack() {
  if (!input_.empty()) {
    Token token = input_.front();
    input_.pop();
    stack_.push(token);
  }
}

void CalcModel::MoveTokenFromStackToOutput() {
  if (!stack_.empty()) {
    Token token = stack_.top();
    stack_.pop();
    output_.push(token);
  }
}

double CalcModel::PostfixExpressionCalculate(double x_value) {
  input_ = output_;
  while (!input_.empty()) {
    std::visit(overloaded{[&](double function) { PushToResult(function); },
                          [&](unary_function function) {
                            double operand = PopFromResult();
                            PushToResult(function(operand));
                          },
                          [&](binary_function function) {
                            double right_operand = PopFromResult();
                            double left_operand = PopFromResult();

                            PushToResult(function(left_operand, right_operand));
                          },
                          [&](auto) { result_.push(x_value); }},
               input_.front().GetFunction());

    input_.pop();
  }

  answer_ = PopFromResult();

  return answer_;
}

void CalcModel::PushToResult(const function_variant& value) {
  std::visit(overloaded{[&](double number) { result_.push(number); },
                        [&](unary_function unary) {
                          result_.push(unary(result_.top()));
                        },
                        [&](binary_function binary) {
                          double right_operand = result_.top();
                          result_.pop();
                          double left_operand = result_.top();
                          result_.pop();
                          result_.push(binary(left_operand, right_operand));
                        },
                        [&](auto) {}},
             value);
}

double CalcModel::PopFromResult() {
  if (!result_.empty()) {
    double top_value = result_.top();
    result_.pop();
    return top_value;
  } else {
    return 0.0;
  }
}

double CalcModel::GetAnswer() { return answer_; }

void CalcModel::CalculateAnswer(const std::string& input_expression,
                                const std::string& input_x) {
  Tokenize(input_expression);
  ModifyQueue();
  ValidateQueue();
  ConvertInfixToPostfix();
  size_t start_i = 0;
  PostfixExpressionCalculate(ReadNumber(input_x, start_i));
}

//// Логика для графикa!
XYGraph CalcModel::GetGraph() const { return answer_graph_; }

void CalcModel::CalculateGraph(const std::string& input_expression,
                               int number_of_points, double x_start,
                               double x_end, double y_min, double y_max) {
  Tokenize(input_expression);
  ModifyQueue();
  ValidateQueue();
  ConvertInfixToPostfix();
  CalculateXY(number_of_points, x_start, x_end, y_min, y_max);
}

void CalcModel::CalculateXY(int number_of_points, double x_start, double x_end,
                            double y_min, double y_max) {
  std::vector<double> x_values, y_values;
  double range = y_max - y_min;
  double step = (x_end - x_start) / (number_of_points - 1);
  for (int i = 0; i < number_of_points; ++i) {
    x_values.push_back(x_start + step * i);
    y_values.push_back(PostfixExpressionCalculate(x_values.back()));
    range = range - y_min;
  }
  answer_graph_ = std::make_pair(x_values, y_values);
}

}  // namespace s21
