#ifndef MAINWINDOW_H
#define MAINWINDOW_H

# include <correlation.H>

#include <QMainWindow>
# include <QDoubleSpinBox>
# include <QComboBox>
# include <QHBoxLayout>
# include <QLabel>

namespace Ui {
  class MainWindow;
}

class MainWindow : public QMainWindow
{
  Q_OBJECT

  using Par = tuple<QHBoxLayout*, QLabel*, QDoubleSpinBox*, QComboBox*, string, size_t>;

  DynList<string> types;
  DynList<string> subtypes;
  DynList<const Correlation*> correlations;
  DynList<Par> pars_vals;
  const Correlation * correlation = nullptr;

  pair<double, const Unit*> result = make_pair(0, nullptr);

  void set_substype_combo(const std::string & type_name);
  void set_corr_combo(const std::string & subtype_name);
  void set_tech_note(const string & note);
  void set_correlation(const Correlation * __correlation);
  void set_exception(const string & msg);
  void reset_status();
  void show_result();
  void set_result_unit(const VtlQuantity &r);
  void compute();

public:
  explicit MainWindow(QWidget *parent = 0);
  ~MainWindow();

private slots:

  void on_corr_type_combo_activated(const QString &arg1);

  void on_corr_subtype_combo_activated(const QString &arg1);

  void on_corr_combo_activated(const QString &arg1);

  void on_result_unit_combo_activated(const QString &arg1);

  void par_unit_changed(const QString &arg1);

  void par_val_changed(double val);

private:
  Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
