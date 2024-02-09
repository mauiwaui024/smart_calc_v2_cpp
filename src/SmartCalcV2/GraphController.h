
#ifndef GRAPHCONTROLLER_H
#define GRAPHCONTROLLER_H

#include "CalcModel.h"

class MainWindow;
namespace s21 {
class GraphController {
 public:
  GraphController() = default;
  ~GraphController() = default;
  GraphController(const GraphController&) = default;
  GraphController(GraphController&&) noexcept = default;
  GraphController& operator=(const GraphController&) = default;
  GraphController& operator=(GraphController&&) noexcept = default;
  void CalculateGraph(MainWindow* main_window);

 private:
  CalcModel model_;
};
}  // namespace s21

#endif  // GRAPHCONTROLLER_H
