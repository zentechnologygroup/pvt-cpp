#include "mainwindow.h"
#include "ui_mainwindow.h"

# include <correlation.H>

MainWindow::MainWindow(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::MainWindow)
{
  ui->setupUi(this);

  for (auto it = Corr
}

MainWindow::~MainWindow()
{
  delete ui;
}
