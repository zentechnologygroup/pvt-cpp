#ifndef MAINWINDOW_H
#define MAINWINDOW_H

# include <correlation.H>

#include <QMainWindow>


namespace Ui {
  class MainWindow;
}

class MainWindow : public QMainWindow
{
  Q_OBJECT

  DynList<string> subtypes;
  DynList<const Correlation*> correlations;

  void set_substype_combo(const std::string & type_name);

public:
  explicit MainWindow(QWidget *parent = 0);
  ~MainWindow();

private slots:
  void on_corr_type_combo_activated(int index);

private:
  Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
