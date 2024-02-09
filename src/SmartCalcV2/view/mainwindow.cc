#include "mainwindow.h"

#include <QMessageBox>
#include <cstddef>

#include "ui_mainwindow.h"
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);
  connect(ui->equals_btn, SIGNAL(clicked()), this, SLOT(on_equal_clicked()));
  connect(ui->graphButton, &QPushButton::clicked, this,
          &MainWindow::on_graphButton_clicked);
  //    customPlot = new QCustomPlot(this);
  //    customPlot->setSizePolicy(QSizePolicy::Expanding,
  //    QSizePolicy::Expanding);

  //    customPlot->show();
}

MainWindow::~MainWindow() { delete ui; }
void MainWindow::on_equal_clicked() {
  try {
    controller_.CalculateValue(this);
  } catch (const std::exception &e) {
    // Обработка исключений
    QMessageBox::critical(this, "Опа, че-то неправильно", e.what());
    ui->line_edit->clear();
  }
}

std::string MainWindow::GetInputString() const {
  // Извлекаем текст из QLineEdit
  QString qtext = ui->line_edit->text();
  // Преобразуем QString в std::string
  //    std::cout << "стринга в лайн эдите " << qtext<< "\n";
  return qtext.toStdString();
}
std::string MainWindow::GetInputStringX() const {
  QString qtext = ui->line_edit->text();
  if (qtext.contains('x', Qt::CaseInsensitive)) {
    bool ok;
    QString xText = QInputDialog::getText(nullptr, tr("Введите значение X"),
                                          tr("X:"), QLineEdit::Normal, "", &ok);

    // Проверяем, что введенное значение - числовое
    bool isNumeric = false;
    xText.toDouble(&isNumeric);

    if (ok && isNumeric && !xText.isEmpty()) {
      // Преобразуйте QString в std::string
      return xText.toStdString();
    } else {
      // Выводим сообщение об ошибке, если введенное значение не является числом
      QMessageBox::critical(
          nullptr, tr("Ошибка"),
          tr("Неверное значение для X. Введите числовое значение."));
    }
  }
  return "0";
}

void MainWindow::SetAnswer(double answer) {
  QString answerString = QString::number(answer);
  // Set the text of the QLineEdit
  ui->line_edit->clear();
  ui->line_edit->setText(answerString);
}

//////////////////GRAPHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHH
///
void MainWindow::on_graphButton_clicked() {
  graphController_.CalculateGraph(this);
}
void MainWindow::UpdateGraph(const s21::XYGraph &graphData) {
  // Очистите существующий график, если есть
  ui->customPlot->clearGraphs();

  // Создайте график и установите данные
  QCPGraph *graph = ui->customPlot->addGraph();

  // Преобразуйте данные из std::vector в QVector
  QVector<double> xData(graphData.first.begin(), graphData.first.end());
  QVector<double> yData(graphData.second.begin(), graphData.second.end());

  graph->setData(xData, yData);

  // Опционально, настройте оси и другие параметры графика
  ui->customPlot->xAxis->setLabel("X Axis");
  ui->customPlot->yAxis->setLabel("Y Axis");
  ui->customPlot->xAxis->setRange(GetXStart(), GetXEnd());
  ui->customPlot->yAxis->setRange(GetYMin(), GetYMax());
  // Перерисовать график
  ui->customPlot->replot();
}

double MainWindow::GetNumPoints() const {
  QString numPointsStr = ui->number_of_points->text();
  bool conversionOk;
  double numPointsValue = numPointsStr.toDouble(&conversionOk);
  if (!conversionOk) {
    return 0.0;
  }
  return numPointsValue;
}
double MainWindow::GetXStart() const {
  QString xStartText = ui->x_start->text();
  // Convert QString to double
  bool conversionOk;
  double xStartValue = xStartText.toDouble(&conversionOk);
  if (!conversionOk) {
    return 0.0;
  }
  return xStartValue;
}

double MainWindow::GetXEnd() const {
  QString xEndText = ui->x_end->text();
  // Convert QString to double
  bool conversionOk;
  double xEndValue = xEndText.toDouble(&conversionOk);
  if (!conversionOk) {
    return 0.0;
  }
  return xEndValue;
}
double MainWindow::GetYMin() const {
  QString yMinText = ui->y_min->text();
  // Convert QString to double
  bool conversionOk;
  double yMinValue = yMinText.toDouble(&conversionOk);
  if (!conversionOk) {
    return 0.0;
  }
  return yMinValue;
}
double MainWindow::GetYMax() const {
  QString yMaxText = ui->y_max->text();
  // Convert QString to double
  bool conversionOk;
  double yMaxValue = yMaxText.toDouble(&conversionOk);
  if (!conversionOk) {
    return 0.0;
  }
  return yMaxValue;
}
