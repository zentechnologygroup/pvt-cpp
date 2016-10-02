
# include <correlation.H>

#include "mainwindow.h"
#include "ui_mainwindow.h"


void MainWindow::set_substype_combo(const string &type_name)
{
  subtypes = Correlation::subtype_list(type_name);

  ui->corr_subtype_combo->clear();
  for (auto it = subtypes.get_it(); it.has_curr(); it.next())
    ui->corr_subtype_combo->addItem(QString::fromStdString(it.get_curr()));

  auto first = subtypes.get_first();
  set_corr_combo(first);
}

void MainWindow::set_corr_combo(const string &subtype_name)
{
  correlations = Correlation::list(subtype_name);

  ui->corr_combo->clear();
  for (auto it = correlations.get_it(); it.has_curr(); it.next())
    {
      auto ptr = it.get_curr();
      ui->corr_combo->addItem(QString::fromStdString(ptr->name));
    }

  auto correlation = correlations.get_first();
  auto text = correlation->full_desc(40);
  set_tech_note(text);
  build_corr_entries(correlation->name);
}

void MainWindow::set_tech_note(const string &note)
{
  ui->tech_note->setText(QString::fromStdString(note));
}

void MainWindow::build_corr_entries(const string &corr_name)
{
  auto frame = ui->parameters_area;

  pars_vals.for_each([] (auto ptr) { delete ptr; });
  pars_vals.empty();
  auto correlation = Correlation::search_by_name(corr_name);
  auto pars = correlation->get_preconditions();
  for (auto it = pars.get_it(); it.has_curr(); it.next())
    {
      const auto & par = it.get_curr();
      QLineEdit * edt = new QLineEdit(this);
      pars_vals.append(edt);
    }
}

MainWindow::MainWindow(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::MainWindow)
{
  ui->setupUi(this);

  types = Correlation::type_list();
  for (auto it = types.get_it(); it.has_curr(); it.next())
    ui->corr_type_combo->addItem(QString::fromStdString(it.get_curr()));

  set_substype_combo(types.get_first());
}

MainWindow::~MainWindow()
{
  delete ui;
}


void MainWindow::on_corr_type_combo_activated(const QString &arg1)
{
  auto type_name = arg1.toStdString();
  set_substype_combo(type_name);
}

void MainWindow::on_corr_subtype_combo_activated(const QString &arg1)
{
  auto subtype_name = arg1.toStdString();
  set_corr_combo(subtype_name);
}

void MainWindow::on_corr_combo_activated(const QString &arg1)
{
  auto corr_name = arg1.toStdString();
  auto correlation = Correlation::search_by_name(corr_name);
  set_tech_note(correlation->full_desc());
}
