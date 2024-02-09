#include "CalcController.h"

#include "mainwindow.h"
namespace s21 {
void CalcController::CalculateValue(MainWindow *main_window) {
  model_.CalculateAnswer(main_window->GetInputString(),
                         main_window->GetInputStringX());
  main_window->SetAnswer(model_.GetAnswer());
}
}  // namespace s21
