#ifndef MYQWSYSTEM_H
#define MYQWSYSTEM_H

#include <QObject>
#include <QVector>
#include <QVectorIterator>
#include <QString>
#include <fstream>

// NORM_CONSTS. For C GNU Scientific Library.
struct exp_params {
    double k2;
    double shift;
    double NS;
};

struct sin_cos_params {
    double k1;
    double shift;
    double NS;
};

double f_left_exp (double x, void *p);

double f_right_exp (double x, void *p);

double f_cycle_exp (double x, void *p);

double f_sin (double x, void *p);

double f_cos (double x, void *p);

//-------------------------------


struct wf_phi_for_gsl_params {
     int power;  // true 1, false 2;
     int qw_numb;
     double NC;
     QVector<double> Qx_array;
     QVector<double> k2;
     QVector<double> k1;
     QVector<double> NS;
     QVector<double> shift;
     std::ofstream* sessionFile = NULL;

};

double wf_phi(double x_point, void *p);


class myQwSystem
{

private:
    // тут уже все в СГС или обезразмеренное.
    // т.е. данные непосредственно для расчета.
    QVector<QVector<double> > A_stored;     // A is a one dim array!
//    QVector<QVector<double> > NS_stored;
//    QVector<QVector<double> > k1_stored;
//    QVector<QVector<double> > k2_stored;

    QVector<std::ofstream*>   wf_files;
    QVector<std::ofstream*>   wf_average_files;
    QVector<std::ofstream*>   wf_average_pow2_files;
    QVector<std::ofstream*>   wf_pow2_average_files;
    std::ofstream*   wfs_for_aver_0;
    std::ofstream*   wfs_for_aver_1;
    std::ofstream*   just_averages_file;
    std::ofstream*   mag_field_file;
    QVector<double> just_averages_0;
    QVector<double> just_averages_1;

    // тут связь, индекс массива полей = максимальный n_f -1.
    QVector<double> B_stored;          // Этот массив индексируется tmp_f_max - 1;


    QVector<double> v0, x, shift, mw, scw, mb, scb;  // const data;
    QVector<double> k1, k2, A;                       // data to store.

    // это записывается в store_dets()
    // и используется в find_roots()
    QVector<double> energies_tmp;
    QVector<double> dets_tmp;


    double eng;   // engu задается при переборе энергий(поиске det = 0).
    //const double pi = 3.14159265358;


    double theta_current_rad;

    double lower_limit;     // устанавливаются в get_const_data();
    double upper_limit;


    double B_0;

public:

    int     qw_numb;        // будет увеличиваться или уменьшатся.
    double engu;            // задается при сканировании энергий.
                            // в void MainWindow::get_energy_slot()
    double eng_step;
    double x_step;
    QVector<double> energies;
    QVector<double> energies_erg;
    QVector<double> x_nanometers;
    QVector<double> norm_consts;

    // eff. mas.
   // QVector<double> qw_ef_mas;
    QVector<double> qw_height;              // v0u;
    QVector<QString> qw_height_str;

    QVector<double> qw_width;               // abu
    QVector<QString> qw_width_str;

    QVector<double> ef_mas_bar;        // Тут еще не в СГС
    QVector<QString> ef_mas_bar_str;        // mob

    QVector<double> ef_mas_well;            // mow
    QVector<QString> ef_mas_well_str;

    QVector<double> averages;
    QVector<double> averages_pow2;
    QVector<double> averages_cm;
    QVector<double> averages_pow2_cm;
    QVector<double> pow2_averages_cm;
    QVector<double> pow2_averages;
    QVector<double> std_deviations_pow2;
    QVector<double> std_deviations_pow2_cm;        // we use without sqrt() !

    //-------- mag_field ------------

    int nu_i, n_i,
        nu_j, n_j;

    int nu_g, n_g,
        nu_f, n_f;
    int n_f_max;

    double b_min, b_max, b_current;
    double theta_from, theta_to, theta_step, theta_current;
    QVector<double> B;
    QString Ws_eval_str;
    QVector<QString> Transitions_string;

    int test_max_field;

    //-------------------------------

    myQwSystem();


    void get_const_data();
    void make_data();
    void fill_data();
    void find_det();
    void find_roots();
    void save_states();
    void create_res_files();

    void clear_all();
    void remove_sel(int);
    void clear_res();
    void clear_before_add();

    void get_null_space();
    void get_norm_consts();
    void get_nc_and_averages();
    void get_just_averages();

    double wf_phi_for_monte();
    double wf_PHI_for_monte();


    void get_transitions();
    void get_B_theta_0();
    double B_find_root();
    double b_step;
    double a,b,c;
    bool peek_B_current();





    void get_fields_from_n();
    double find_field_from_n();
    void clear_field();
    void eval_Ws_for_current_B_field();
    double get_field_from_n_theta();


    void plot_arevages();
    //----------------
    // get data for plot

    QVector<QVector<double> > NS_stored;
    QVector<QVector<double> > k1_stored;
    QVector<QVector<double> > k2_stored;

    double get_engValue_toPlot(int energyID);
    QVector<double> get_x_range_toPlot();
    QVector<QVector<double> > get_k1k2_toPlot(int energyID);
    QVector<double> get_NS_toPlot(int energyID);
    QVector<double> get_Qx_array_toPlot();
    QVector<double> get_shift_toPlot();

    QVector<wf_phi_for_gsl_params> wf_params_array;
    void get_xy_toPlot(int energyID);

    //-----------------
};





//vectorA = QVector::fromList(listA);
#endif // MYQWSYSTEM_H
