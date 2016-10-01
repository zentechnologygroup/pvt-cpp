
# include <correlation.H>

#include "mainwindow.h"
#include "ui_mainwindow.h"


void MainWindow::set_substype_combo(const string &type_name)
{
  subtypes = Correlation::subtype_list(type_name);
  for (auto it = subtypes.get_it(); it.has_curr(); it.next())
    ui->corr_subtype_combo->addItem(QString::fromStdString(it.get_curr()));
}

MainWindow::MainWindow(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::MainWindow)
{
  ui->setupUi(this);

  auto types = Correlation::type_list();
  for (auto it = types.get_it(); it.has_curr(); it.next())
    ui->corr_type_combo->addItem(QString::fromStdString(it.get_curr()));

  set_substype_combo(types.get_first());

  auto correlations = Correlation::list(subtypes.get_first());
  auto correlation = correlations.get_first();

  for (auto it = correlations.get_it(); it.has_curr(); it.next())
    ui->corr_combo->addItem(QString::fromStdString(it.get_curr()->name));

  ui->tech_note->setText(QString::fromStdString(correlation->full_desc(50)));
}

MainWindow::~MainWindow()
{
  delete ui;
}

void MainWindow::on_corr_type_combo_activated(int index)
{

}
