#ifndef CALC_MODEL_H
#define CALC_MODEL_H
#include <math.h>

#include <cstring>
#include <iostream>
#include <queue>
#include <stack>

#include "Token.h"

namespace s21 {
class CalcModel {
 public:
  CalcModel() = default;
  ~CalcModel() = default;
  CalcModel(const CalcModel&) = default;
  CalcModel(CalcModel&&) noexcept = default;
  CalcModel& operator=(const CalcModel&) = default;
  CalcModel& operator=(CalcModel&&) noexcept = default;
  ///
  double GetAnswer();
  void CalculateAnswer(const std::string& input_expression,
                       const std::string& input_x);
  //график
  XYGraph GetGraph() const;
  void CalculateGraph(const std::string& input_expression, int number_of_points,
                      double x_start, double x_end, double y_min, double y_max);
  void CalculateXY(int number_of_points, double x_start, double x_end,
                   double y_min, double y_max);

 private:
  void Tokenize(const std::string& expression);
  std::string ReadWord(const std::string& expression, size_t& start_index);
  double ReadNumber(const std::string& expression, size_t& start_index);
  std::string ReadSingleChar(const std::string& expression,
                             size_t& start_index);
  void TryToPushToken(std::string token);
  void ConvertInfixToPostfix();
  void MoveTokenFromInputToOutput();
  void MoveTokenFromInputToStack();
  void MoveTokenFromStackToOutput();
  void ModifyQueue();
  void ValidateQueue();
  double PostfixExpressionCalculate(double x_value);
  void PushToResult(const function_variant& value);
  double PopFromResult();
  std::string toLowerCase(const std::string& str);

  std::queue<Token> input_;
  std::queue<Token> output_;
  std::stack<Token> stack_;
  std::stack<double> result_;
  double answer_ = 0.0;

  XYGraph answer_graph_;

  std::map<std::string, Token> token_map_;
  static constexpr bool kAdjacencyMatrix[kNumTokenTypes][kNumTokenTypes] = {
      {0, 1, 0, 1, 0, 0, 1}, {1, 0, 1, 0, 1, 1, 0}, {1, 0, 1, 0, 1, 1, 0},
      {0, 1, 0, 1, 0, 0, 1}, {0, 0, 0, 0, 0, 1, 0}, {1, 0, 1, 0, 1, 1, 0},
      {0, 1, 0, 1, 0, 0, 1},
  };
};
}  // namespace s21

#endif  // CALC_MODEL_H