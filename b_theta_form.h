#ifndef B_THETA_FORM_H
#define B_THETA_FORM_H

#include <QWidget>
#include <QVector>

namespace Ui {
class B_Theta_form;
}

class B_Theta_form : public QWidget
{
    Q_OBJECT

public:
    explicit B_Theta_form(QWidget *parent,
                          QVector<double> energies_erg,
                          QVector<double> std_deviations_pow2_cm,
                          QVector<double> trans_info,
                          QVector<double> mw);
    ~B_Theta_form();
    void angle_field_changes();

private:
    Ui::B_Theta_form *ui;
    QVector<double> energies_erg;
    QVector<double> eng_data_x;
    QVector<double> eng_data_y;
    QVector<double> eng_data_x2;
    QVector<double> eng_data_y2;

    QVector<double> energy_vec;
    QVector<double> landau_vec;
    QVector<double> diam_vec;
    QVector<double> energy_diam_vec;

    QVector<double> std_deviations_pow2_cm;
    QVector<double> trans_info;
    QVector<double> ef_mas_well;
    QVector<double> x_data_eq, y_data_eq;
    double a_pre,b_pre,c_pre;
    int nu_i, nu_j, nu_g, nu_f, n_i, n_j, n_g, n_f;
    QVector<int> nu_vector, n_vector;
    double low_limit_x, top_limit_x;
    double low_limit_y, top_limit_y;
    int low_limit_engx, top_limit_engx;
    double low_limit_engy, top_limit_engy;
    double b_step;
    double angle_rad, angle_grad;
    double B_tmp, B_tmp_tesla;
    double B_perp, B_perp_tesla, B_par, B_par_tesla;

    int g_col[4][3];





public slots:

    void slider_Bpar_changes_slot(int field);
    void spinbox_Bpar_changes_slot(double field);

    void slider_Bperp_changes_slot(int field);
    void spinbox_Bperp_changes_slot(double field);

    void slider_btot_changes_slot(int field);
    void spinbox_btot_changes_slot(double field);

    void slider_angle_changes_slot(int angle);
    void spinbox_angle_changes_slot(double angle);

    void auto_plot_slot();
    void print_equation_slot();

    void print_levels_slot();


    void independence_changed_slot();

    void replot_slot();




};

#endif // B_THETA_FORM_H
