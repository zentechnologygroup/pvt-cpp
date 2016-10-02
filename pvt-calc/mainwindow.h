#ifndef MAINWINDOW_H
#define MAINWINDOW_H

# include <correlation.H>

#include <QMainWindow>
# include <QLineEdit>
# include <QComboBox>

namespace Ui {
  class MainWindow;
}

class MainWindow : public QMainWindow
{
  Q_OBJECT

  DynList<string> types;
  DynList<string> subtypes;
  DynList<const Correlation*> correlations;
  DynList<QLineEdit*> pars_vals;
  DynList<QComboBox*> pars_units;

  void set_substype_combo(const std::string & type_name);
  void set_corr_combo(const std::string & subtype_name);
  void set_tech_note(const string & note);
  void build_corr_entries(const string & corr_name);

public:
  explicit MainWindow(QWidget *parent = 0);
  ~MainWindow();

private slots:

  void on_corr_type_combo_activated(const QString &arg1);

  void on_corr_subtype_combo_activated(const QString &arg1);

  void on_corr_combo_activated(const QString &arg1);

private:
  Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
