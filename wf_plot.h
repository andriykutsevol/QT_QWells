#ifndef WF_PLOT_H
#define WF_PLOT_H

#include <fstream>
//#include <units_trans.h>

#include <myqwsystem.h>
#include <QWidget>
#include <QString>

namespace Ui {
class wf_plot;
}

class wf_plot : public QWidget
{
    Q_OBJECT

public:
    explicit wf_plot(QWidget *parent = 0);
    wf_plot(int blockID);
    int graph_n, graph_count, graph_well_count;
    ~wf_plot();

    // set data for plot

    void set_engValue_toPlot(double energy);
    void set_x_range_toPlot(QVector<double> xy_range);
    void set_xstep(double x_step);
    void set_xy_toPlot(wf_phi_for_gsl_params *wf_params, double average, double std_deviat_pow2);

    void is_energy_present();

    //-----------------


private:
     wf_phi_for_gsl_params wf_params;
     Ui::wf_plot *ui;
     QVector<int> blockID_vec;
     double energy, dataY;
     QString k1_str, k2_str;
     QVector<double> energy_vec, average_vec, std_deviat_pow2_vec;
     QVector<QString> k1_str_vec, k2_str_vec;
     double low_limit;
     double top_limit;
     double low_limit_y, top_limit_y;
     double x_step;
     QVector<double> x, y;
     QVector<double> well_x, well_y;

public slots:
     void selectionChanged_slot();
};

#endif // WF_PLOT_H
