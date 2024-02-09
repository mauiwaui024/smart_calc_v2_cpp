#ifndef CALCULATORCONTROLLER_H
#define CALCULATORCONTROLLER_H

#include "CalcModel.h"

class MainWindow;
namespace s21 {
class CalcController {
 public:
  CalcController() = default;
  ~CalcController() = default;
  CalcController(const CalcController&) = default;
  CalcController(CalcController&&) noexcept = default;
  CalcController& operator=(const CalcController&) = default;
  CalcController& operator=(CalcController&&) noexcept = default;

  void CalculateValue(MainWindow* main_window);

 private:
  CalcModel model_;
};
}  // namespace s21
#endif
