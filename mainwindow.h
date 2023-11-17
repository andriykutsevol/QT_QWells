#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <myqwsystem.h>

#include "wf_plot.h"
#include "ui_wf_plot.h"

#include "ui_form_myplot.h"
#include "form_myplot.h"

#include <fstream>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow, public myQwSystem
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    void txt_iface();
    int tmp;
    wf_plot *wf_graph = NULL;
    int graph_n;

    ~MainWindow();

private:
    Ui::MainWindow *ui;

    //int selected_block;

signals:
   void reprint_input_data();
   void remove_all_sig();
   void remove_sel_sig(int);

private slots:
    void add_well_slot();
    void get_energy_slot();
    void clear_all_slot();
    void remove_sel_slot();
    //void select_rem_block_slot(int);
    void select_rem_block_slot();
    void reprint_input_data_slot();

    void get_transitions_slot();
    void get_fields_from_n_slot();
    void clear_field_slot();
    void selected_mag_field_slot();
    void eveluate_Ws_for_current_field_slot();

    void plot_wf_slot();
    void plot_averages_slot();

    void energy_selected_slot();
    void b_theta_plot_slot();

    void my_plot_slot();

    void monte_slot();

    void openGL_slot();

    void cuda_slot();

    void cudanew_slot();
};

#endif // MAINWINDOW_H
