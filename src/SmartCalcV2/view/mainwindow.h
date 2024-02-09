#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "../CalcController.h"
#include "../GraphController.h"
#include "QInputDialog"
#include "qcustomplot.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
  Q_OBJECT

 public:
  MainWindow(QWidget* parent = nullptr);
  ~MainWindow();
  std::string GetInputString() const;
  std::string GetInputStringX() const;
  void SetAnswer(double answer);

  //
  double GetXStart() const;
  double GetXEnd() const;
  double GetYMin() const;
  double GetYMax() const;
  double GetNumPoints() const;

  void UpdateGraph(const s21::XYGraph& graphData);

 private:
  Ui::MainWindow* ui;
  s21::CalcController controller_;
  s21::GraphController graphController_;
  QCustomPlot* customPlot;

 public slots:
  void on_equal_clicked();
  void on_graphButton_clicked();
};
#endif  // MAINWINDOW_H
