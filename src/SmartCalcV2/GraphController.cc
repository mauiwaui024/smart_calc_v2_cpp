
#include "GraphController.h"

#include "mainwindow.h"

namespace s21 {
void GraphController::CalculateGraph(MainWindow* main_window) {
  model_.CalculateGraph(main_window->GetInputString(),
                        main_window->GetNumPoints(), main_window->GetXStart(),
                        main_window->GetXEnd(), main_window->GetYMin(),
                        main_window->GetYMax());
  XYGraph graphData = model_.GetGraph();
  main_window->UpdateGraph(graphData);
}
}  // namespace s21
