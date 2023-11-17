#include "b_theta_form.h"
#include "ui_b_theta_form.h"
#include "units_trans.h"

#include "auto_plot.h"
#include "ui_auto_plot.h"

#include <math.h>
#include <QDebug>

#include <QVector>
#include <QString>
#include <QVectorIterator>
#include <QPen>

#include <iostream>
#include <fstream>

B_Theta_form::B_Theta_form(QWidget *parent,
                           QVector<double> energies_erg,
                           QVector<double> std_deviations_pow2_cm,
                           QVector<double> trans_info,
                           QVector<double> ef_mas_well) :
    QWidget(parent),
    energies_erg(energies_erg),
    std_deviations_pow2_cm(std_deviations_pow2_cm),
    trans_info(trans_info),
    ef_mas_well(ef_mas_well),
    ui(new Ui::B_Theta_form)
{

    ui->setupUi(this);
    QVectorIterator<double> energies_erg_iter(energies_erg);
    QVectorIterator<double> std_deviations_pow2_cm_iter(std_deviations_pow2_cm);
    QString eng_dev_str;
    while(energies_erg_iter.hasNext()){
        eng_dev_str = QString::number(erg_to_mev(energies_erg_iter.next())) + "<br>" +
                QString::number(cm_to_nano(std_deviations_pow2_cm_iter.next())) + "<br> ====";
        ui->EnergiesTextEdit->append(eng_dev_str);
    }



      B_tmp = 0;
      angle_rad = 0;

      g_col[0][0] = 255;  g_col[0][1] = 0;    g_col[0][2] = 0;
      g_col[1][0] = 255;  g_col[1][1] = 0;    g_col[1][2] = 0;
      g_col[2][0] = 102;  g_col[2][1] = 255;  g_col[2][2] = 0;
      g_col[3][0] = 102;  g_col[3][1] = 102;  g_col[3][2] = 255;


    //------------
    void (QDoubleSpinBox:: *signal)(double) = &QDoubleSpinBox::valueChanged;
    //------------

    QObject::connect(ui->B_par_spinBox, signal,
                     this, &B_Theta_form::spinbox_Bpar_changes_slot);

    QObject::connect(ui->B_par_Slider, &QSlider::valueChanged,
                      this, &B_Theta_form::slider_Bpar_changes_slot);

    //--------------------

    QObject::connect(ui->B_perp_spinBox, signal,
                     this, &B_Theta_form::spinbox_Bperp_changes_slot);

    QObject::connect(ui->B_perp_Slider, &QSlider::valueChanged,
                     this, &B_Theta_form::slider_Bperp_changes_slot);


    QObject::connect(ui->btot_SpinBox, signal,
                     this, &B_Theta_form::spinbox_btot_changes_slot);

    QObject::connect(ui->btot_slider, &QSlider::valueChanged,
                     this, &B_Theta_form::slider_btot_changes_slot);

    QObject::connect(ui->angle_SpinBox, signal,
                     this, &B_Theta_form::spinbox_angle_changes_slot);

    QObject::connect(ui->angle_Slider, &QSlider::valueChanged,
                     this, &B_Theta_form::slider_angle_changes_slot);

    //--------------------

    //=========================================

//    QObject::connect(ui->auto_plotButton, &QPushButton::clicked,
//                     this, &B_Theta_form::auto_plot_slot);

    QObject::connect(ui->PrintEquationButton, &QPushButton::clicked,
                     this, &B_Theta_form::print_equation_slot);

    QObject::connect(ui->Indep_checkBox, &QCheckBox::clicked,
                     this, &B_Theta_form::independence_changed_slot);

    QObject::connect(ui->PrintLevelsButton, &QPushButton::clicked,
                     this, &B_Theta_form::print_levels_slot);

    //==========================================

    QObject::connect(ui->replot_Button, &QPushButton::clicked,
                     this, &B_Theta_form::replot_slot);

    //===========================================


    //===========================================


    this->angle_rad = grad_rad(0);
    this->b_step = 1;
//    nu_i = trans_info[0]; nu_j = trans_info[1];
//    nu_g = trans_info[2]; nu_f = trans_info[3];

//    n_i = trans_info[4]; n_j = trans_info[5];
//    n_g = trans_info[6]; n_f = trans_info[7];

    nu_i = ui->nu_i_lineEdit->text().toInt() - 1;
    nu_j = ui->nu_j_lineEdit->text().toInt() - 1;
    nu_g = ui->nu_g_lineEdit->text().toInt() - 1;
    nu_f = ui->nu_f_lineEdit->text().toInt() - 1;

    n_i = ui->n_i_lineEdit->text().toInt();
    n_j = ui->n_j_lineEdit->text().toInt();
    n_g = ui->n_g_lineEdit->text().toInt();
    n_f = ui->n_f_lineEdit->text().toInt();



    c_pre = energies_erg[nu_i] + energies_erg[nu_j] - energies_erg[nu_g] - energies_erg[nu_f];

    a_pre = pre_deviations_a(ef_mas_well[0]) *
            (std_deviations_pow2_cm[nu_i] + std_deviations_pow2_cm[nu_j]
             - std_deviations_pow2_cm[nu_g] - std_deviations_pow2_cm[nu_f]);

    b_pre = pre_landau_b(ef_mas_well[0]) * (n_i + n_j - n_g - n_f) ;

//    low_limit_x = 0;
//    top_limit_x = 100; //tesla
//    low_limit_y = 0;
//    double i = low_limit_x;
//    double dataY = 0;
//    while(i < top_limit_x){
//        dataY = erg_to_mev(a_pre * pow(tesla_gauss(i) * sin(angle_rad),2) +
//                           b_pre* tesla_gauss(i) * cos(angle_rad) +
//                           c_pre);
//        x_data_eq.append(i);
//        y_data_eq.append(dataY);
//        if(dataY < low_limit_y)
//            low_limit_y = dataY;
//        if(dataY > top_limit_y)
//            top_limit_y = dataY;

//    i += b_step;
//    }
//    //qDebug()<<"stop";
//    ui->equation_angle_plot->addGraph();
//    QPen equationPen;
//    equationPen.setWidth(3);
//    equationPen.setColor(QColor(30,144,255));
//    ui->equation_angle_plot->graph(0)->setPen(equationPen);
//    ui->equation_angle_plot->graph(0)->setData(this->x_data_eq, this->y_data_eq);
//    ui->equation_angle_plot->xAxis->setRange(this->low_limit_x, this->top_limit_x);
//    ui->equation_angle_plot->yAxis->setRange(this->low_limit_y, this->top_limit_y);
//    ui->equation_angle_plot->rescaleAxes();
//    // Это была парабола.




    //======================================================
    // Это чисто энергии дна поздон.

    double dataY = 0;

    energy_vec.append(erg_to_mev(energies_erg[nu_i]));
    energy_vec.append(erg_to_mev(energies_erg[nu_j]));
    energy_vec.append(erg_to_mev(energies_erg[nu_g]));
    energy_vec.append(erg_to_mev(energies_erg[nu_f]));
    QVectorIterator<double> energy_vec_iter(energy_vec);


    // Диамагнитные сдвиги.
    //QVector<int> nu_vector;
    nu_vector.append(nu_i);
    nu_vector.append(nu_j);
    nu_vector.append(nu_g);
    nu_vector.append(nu_f);
    QVectorIterator<int> nu_vector_iter(nu_vector);
    int nu_tmp;

    while(nu_vector_iter.hasNext()){
        nu_tmp = nu_vector_iter.next();
        dataY =  erg_to_mev( pre_deviations_a(ef_mas_well[0]) * pow(B_tmp * sin(angle_rad),2) *  std_deviations_pow2_cm[nu_tmp] );
        diam_vec.append(dataY);

    }


    // Прибавляем к энегриям диамагнитные сдвиги.
    QVectorIterator<double> diam_vec_iter(diam_vec);
    while(energy_vec_iter.hasNext()){
        energy_diam_vec.append(energy_vec_iter.next() + diam_vec_iter.next());
    }

    // Уровни Ландау (изначально отсчитываются от своих (даже диам. сдвинутых) уровней энергий).
    // Собственно говоря, только их и можно рисовать.
    //QVector<int> n_vector;
    n_vector.append(n_i);
    n_vector.append(n_j);
    n_vector.append(n_g);
    n_vector.append(n_f);
    QVectorIterator<int> n_vector_iter(n_vector);
    QVectorIterator<double> energy_diam_vec_iter(energy_diam_vec);
    int n_tmp;
    while(n_vector_iter.hasNext()){
        n_tmp = n_vector_iter.next();
        dataY = erg_to_mev(pre_landau_b(ef_mas_well[0]) * B_tmp * cos(angle_rad) * (n_tmp + 0.5))
                + energy_diam_vec_iter.next();
        landau_vec.append(dataY);
    }

    //=====================================

    low_limit_x = 0;
    top_limit_x = 100; //tesla
    low_limit_y = top_limit_y = 0;
    double i = low_limit_x;
    double diam_tmp[4], energy_diam_tmp[4], landau_tmp[4], energy_tot_tmp[4];


    int j = 0;
    while(i < top_limit_x){
        j = 0;
        while(j < 4){
            diam_tmp[j] =  erg_to_mev( pre_deviations_a(ef_mas_well[0]) * pow(tesla_gauss(i) * sin(angle_rad),2)
                             *  std_deviations_pow2_cm[nu_vector[j]] );
            j++;
        }

        j = 0;
        while(j < 4){
            energy_diam_tmp[j] = energy_vec[j] + diam_tmp[j];
            j++;
        }

        j = 0;
        while(j < 4){
            landau_tmp[j] = erg_to_mev(pre_landau_b(ef_mas_well[0]) * tesla_gauss(i) * cos(angle_rad) * (n_vector[j] + 0.5))
                    + energy_diam_tmp[j];
            j++;
        }



        x_data_eq.append(i);
        y_data_eq.append(landau_tmp[0] + landau_tmp[1] - landau_tmp[2] - landau_tmp[3]);
//        y_data_eq.append( energy_vec[0] + diam_tmp[0] + landau_tmp[0]
//                        + energy_vec[1] + diam_tmp[1] + landau_tmp[1]
//                        - energy_vec[2] - diam_tmp[2] - landau_tmp[2]
//                        - energy_vec[3] - diam_tmp[3] - landau_tmp[3]);
        i += b_step;
    }




    ui->equation_angle_plot->addGraph();
    QPen equationPen;
    equationPen.setWidth(3);
    equationPen.setColor(QColor(30,144,255));
    ui->equation_angle_plot->graph(0)->setPen(equationPen);
    ui->equation_angle_plot->graph(0)->setData(this->x_data_eq, this->y_data_eq);
    ui->equation_angle_plot->xAxis->setRange(this->low_limit_x, this->top_limit_x);
    ui->equation_angle_plot->yAxis->setRange(this->low_limit_y, this->top_limit_y);
    ui->equation_angle_plot->rescaleAxes();
    // Это была парабола.


    //=====================================


    low_limit_engx = 0;
    top_limit_engx = 10;

    low_limit_engy = top_limit_engy = 0;

    double landau_tmp1;

    QVectorIterator<double> landau_vec_iter(landau_vec);
    int graph_n = 0;
    int g_c_i = 0;
    while (landau_vec_iter.hasNext()){

        landau_tmp1 = landau_vec_iter.next();

        if(low_limit_engy >  landau_tmp1){
            low_limit_engy = landau_tmp1;
        }

        if(top_limit_engy < landau_tmp1){
            top_limit_engy = landau_tmp1;
        }
        int x = low_limit_engx;
        while (x < top_limit_engx){
            eng_data_x.append(x);
            eng_data_y.append(landau_tmp1);
            x ++;
        }
        ui->energy_angle_Plot->addGraph();
        ui->energy_angle_Plot->graph(graph_n)->setData(eng_data_x, eng_data_y);
        ui->energy_angle_Plot->graph(graph_n)->setPen(QColor(g_col[graph_n][0],  g_col[graph_n][1], g_col[graph_n][2]));
        qDebug()<< g_col[graph_n][0] <<" "<<  g_col[graph_n][1] <<" "<< g_col[graph_n][2];
        ui->energy_angle_Plot->xAxis->setRange(low_limit_engx, top_limit_engx);
        ui->energy_angle_Plot->yAxis->setRange(low_limit_engy - top_limit_engy/4.0, top_limit_engy + top_limit_engy/4.0);
        graph_n ++;
        eng_data_x.clear();
        eng_data_y.clear();

    }




}


//-------------------------------------
//=====================================


B_Theta_form::~B_Theta_form()
{
    delete ui;
}


//===============================================
//===============================================
// parallel

void B_Theta_form::slider_Bpar_changes_slot(int field){
    ui->B_par_spinBox->setValue(field/1000.0);
}

void B_Theta_form::spinbox_Bpar_changes_slot(double field){

    ui->B_par_Slider->setValue(field * 1000.0);
    this->B_par = tesla_gauss(field);
    this->B_tmp = sqrt(pow(B_par,2) + pow(B_perp,2));
    this->angle_grad = rad_grad(atan(B_par/B_perp));
    this->angle_rad = grad_rad(angle_grad);
    if(ui->Indep_checkBox->isChecked()){
        ui->btot_SpinBox->setValue(gauss_tesla(B_tmp));
        ui->angle_SpinBox->setValue(angle_grad);
    }
    this->angle_field_changes();

}

//===============================================
// perpendicular

void B_Theta_form::slider_Bperp_changes_slot(int field){
    ui->B_perp_spinBox->setValue(field/1000.0);

}

void B_Theta_form::spinbox_Bperp_changes_slot(double field){
    ui->B_perp_Slider->setValue(field * 1000.0);
    this->B_perp = tesla_gauss(field);
    this->B_tmp = sqrt(pow(B_par,2) + pow(B_perp,2));
    this->angle_grad = rad_grad(atan(B_par/B_perp));
    this->angle_rad = grad_rad(angle_grad);
    if(ui->Indep_checkBox->isChecked()){
        ui->btot_SpinBox->setValue(gauss_tesla(B_tmp));
        ui->angle_SpinBox->setValue(angle_grad);
    }

  this->angle_field_changes();

}


void B_Theta_form::slider_btot_changes_slot(int field){
    //qDebug()<< "slider_btot_changes_slot() " << field;
    ui->btot_SpinBox->setValue(field / 1000.0);
    this->B_tmp_tesla = ui->btot_SpinBox->value();

}

void B_Theta_form::spinbox_btot_changes_slot(double field){
  //qDebug()<< "spinbox_btot_changes_slot()" << field;
  ui->btot_slider->setValue(field * 1000.0);
  if(! ui->Indep_checkBox->isChecked()){
    ui->B_perp_spinBox->setValue(field * cos(grad_rad(ui->angle_SpinBox->value())));
    ui->B_par_spinBox->setValue(field *  sin(grad_rad(ui->angle_SpinBox->value())));
  }

  this->B_tmp_tesla = ui->btot_SpinBox->value();
}

void B_Theta_form::slider_angle_changes_slot(int angle){
    //qDebug()<< "slider_angle_changes_slot()";
    ui->angle_SpinBox->setValue(angle/1000.0);
}

void B_Theta_form::spinbox_angle_changes_slot(double angle){
    //qDebug()<< "spinbox_angle_changes_slot()";
    ui->angle_Slider->setValue(angle * 1000.0);
    if(! ui->Indep_checkBox->isChecked()){
        ui->B_perp_spinBox->setValue(this->B_tmp_tesla * cos(grad_rad(angle)));
        ui->B_par_spinBox->setValue(this->B_tmp_tesla *  sin(grad_rad(angle)));
    }
}






void B_Theta_form::angle_field_changes(){

    x_data_eq.clear();
    y_data_eq.clear();
    double dataY;
    double i = low_limit_x;


    double diam_tmp[4], energy_diam_tmp[4], landau_tmp[4], energy_tot_tmp[4];


    int j = 0;
    while(i < top_limit_x){
        j = 0;
        while(j < 4){
            diam_tmp[j] =  erg_to_mev( pre_deviations_a(ef_mas_well[0]) * pow(tesla_gauss(i) * sin(angle_rad),2)
                    *  std_deviations_pow2_cm[nu_vector[j]] );
            j++;
        }

        j = 0;
        while(j < 4){
            energy_diam_tmp[j] = energy_vec[j] + diam_tmp[j];
            j++;
        }

        j = 0;
        while(j < 4){
            landau_tmp[j] = erg_to_mev(pre_landau_b(ef_mas_well[0]) * tesla_gauss(i) * cos(angle_rad) * (n_vector[j] + 0.5))
                    + energy_diam_tmp[j];
            j++;
        }



        x_data_eq.append(i);
        y_data_eq.append(landau_tmp[0] + landau_tmp[1] - landau_tmp[2] - landau_tmp[3]);
//        y_data_eq.append( energy_vec[0] + diam_tmp[0] + landau_tmp[0]
//                        + energy_vec[1] + diam_tmp[1] + landau_tmp[1]
//                        - energy_vec[2] - diam_tmp[2] - landau_tmp[2]
//                        - energy_vec[3] - diam_tmp[3] - landau_tmp[3]);

        i += b_step;
    }


    ui->equation_angle_plot->graph(0)->setData(this->x_data_eq, this->y_data_eq);
    //ui->equation_angle_plot->rescaleAxes();
    ui->equation_angle_plot->replot();




//    // Диамагнитные сдвиги.

    QVectorIterator<int> nu_vector_iter(nu_vector);
    int nu_tmp;
    diam_vec.clear();
    while(nu_vector_iter.hasNext()){
        nu_tmp = nu_vector_iter.next();
        qDebug()<<"nu_tmp: " << nu_tmp;
        dataY =  erg_to_mev( pre_deviations_a(ef_mas_well[0]) * pow(B_tmp * sin(angle_rad),2) *  std_deviations_pow2_cm[nu_tmp] );
        qDebug()<<"dataY: " << dataY;
        diam_vec.append(dataY);

    }


    // Прибавляем к энегриям диамагнитные сдвиги.
    QVectorIterator<double> diam_vec_iter(diam_vec);
    QVectorIterator<double> energy_vec_iter(energy_vec);
    energy_diam_vec.clear();
    while(energy_vec_iter.hasNext()){
        energy_diam_vec.append(energy_vec_iter.next() + diam_vec_iter.next());
    }

    // Уровни Ландау (изначально отсчитываются от своих (даже диам. сдвинутых) уровней энергий).
    // Собственно говоря, только их и можно рисовать.

    QVectorIterator<int> n_vector_iter(n_vector);
    QVectorIterator<double> energy_diam_vec_iter(energy_diam_vec);
    int n_tmp;
    landau_vec.clear();
    while(n_vector_iter.hasNext()){
        n_tmp = n_vector_iter.next();
        qDebug()<<"n_tmp"<< n_tmp;
        dataY = erg_to_mev(pre_landau_b(ef_mas_well[0]) * B_tmp * cos(angle_rad) * (n_tmp + 0.5))
                + energy_diam_vec_iter.next();
        qDebug()<<"dataY: " << dataY;
        landau_vec.append(dataY);
    }

    low_limit_engy = top_limit_engy = 0;

    double landau_tmp1;

    QVectorIterator<double> landau_vec_iter(landau_vec);
    int graph_n = 0;
    while (landau_vec_iter.hasNext()){

        landau_tmp1 = landau_vec_iter.next();

        if(low_limit_engy >  landau_tmp1){
            low_limit_engy = landau_tmp1;
        }

        if(top_limit_engy < landau_tmp1){
            top_limit_engy = landau_tmp1;
        }
        int x = low_limit_engx;
        while (x < top_limit_engx){
            eng_data_x.append(x);
            eng_data_y.append(landau_tmp1);
            x ++;
        }
        ui->energy_angle_Plot->graph(graph_n)->setData(eng_data_x, eng_data_y);
        ui->energy_angle_Plot->xAxis->setRange(low_limit_engx, top_limit_engx);
        ui->energy_angle_Plot->yAxis->setRange(low_limit_engy - top_limit_engy/4.0, top_limit_engy + top_limit_engy/4.0);
        graph_n ++;
        eng_data_x.clear();
        eng_data_y.clear();
    }


    ui->energy_angle_Plot->replot();


}





void B_Theta_form::auto_plot_slot(){

    QVector<double> Enu_1, Enu_2, diam_1, diam_2, b_par_x;
    double low_y= 0, top_y = 0;

    double dataY;

    double b_par = 0;
    while (b_par < this->B_par){
        this->B_tmp = sqrt(pow(b_par,2) + pow(B_perp,2));
        this->angle_grad = rad_grad(atan(B_par/B_perp));
        dataY =  erg_to_mev( pre_deviations_a(ef_mas_well[0])
                * pow(B_tmp * sin(angle_rad),2)
                *  std_deviations_pow2_cm[nu_i]);
        if(low_y > dataY)
            low_y = dataY;
        if(top_y < dataY)
            top_y = dataY;
        diam_1.append(erg_to_mev(this->energies_erg[0]) + dataY);

        dataY =  erg_to_mev( pre_deviations_a(ef_mas_well[0])
                * pow(B_tmp * sin(angle_rad),2)
                *  std_deviations_pow2_cm[nu_g]);
        if(low_y > dataY)
            low_y = dataY;
        if(top_y < dataY)
            top_y = dataY;
        diam_2.append(erg_to_mev(this->energies_erg[1]) + dataY);

        Enu_1.append(erg_to_mev(energies_erg[nu_i]));
        if(low_y > dataY)
            low_y = dataY;
        if(top_y < dataY)
            top_y = dataY;
        Enu_2.append(erg_to_mev(energies_erg[nu_g]));
        if(low_y > dataY)
            low_y = dataY;
        if(top_y < dataY)
            top_y = dataY;



        b_par_x.append(gauss_tesla(b_par));
        b_par = b_par + 1000;
    }


    Auto_Plot *auto_pl = new Auto_Plot(&diam_1, &diam_2,
                                       &Enu_1, &Enu_2,
                                       &b_par_x, low_y, top_y);
    auto_pl->set_b_perp_label("B_perp = " + QString::number(gauss_tesla(B_perp)));
    auto_pl->show();

    qDebug()<<"auto plot: ";
}


void B_Theta_form::print_equation_slot(){
    // print to file 2(2,0) - (1,0) - (1,1)
    std::ofstream total_energy_file;
    total_energy_file.open("../DPL_QT/TXT_RES/total_energy.txt");
    int angle = 16;
    double i;

    double dataY = 0;
    while(i < top_limit_x){
        dataY = erg_to_mev(a_pre * pow(tesla_gauss(i) * sin(angle_rad),2) +
                           b_pre* tesla_gauss(i) * cos(angle_rad) +
                           c_pre);
        x_data_eq.append(i);
        y_data_eq.append(dataY);
        total_energy_file << i << " " << dataY << " "<< 0 << "\n";

    i += b_step;
    }


}


void B_Theta_form::print_levels_slot(){

    std::ofstream levels_file;
    levels_file.open("../DPL_QT/TXT_RES/levels.txt");


    double diam_tmp[4], energy_diam_tmp[4], landau_tmp[4], energy_tot_tmp[4];

    int j = 0;
    int i = 0;
    while(i < ui->btot_SpinBox->value()){
        j = 0;
        while(j < 4){
            diam_tmp[j] =  erg_to_mev( pre_deviations_a(ef_mas_well[0]) * pow(tesla_gauss(i) * sin(angle_rad),2)
                    *  std_deviations_pow2_cm[nu_vector[j]] );
            j++;
        }

        j = 0;
        while(j < 4){
            energy_diam_tmp[j] = energy_vec[j] + diam_tmp[j];
            j++;
        }

        j = 0;
        while(j < 4){
            landau_tmp[j] = erg_to_mev(pre_landau_b(ef_mas_well[0]) * tesla_gauss(i) * cos(angle_rad) * (n_vector[j] + 0.5))
                    + energy_diam_tmp[j];
            j++;
        }

        levels_file << i << " " << landau_tmp[0] << " " << landau_tmp[1] << " " << landau_tmp[2] << " " << landau_tmp[3] << "\n";
        i += b_step;
    }




}



void B_Theta_form::independence_changed_slot(){
    qDebug()<<"Independence: ";
    if(ui->Indep_checkBox->isChecked()){
        ui->angle_Slider->setEnabled(false);
        ui->angle_SpinBox->setEnabled(false);
        ui->btot_slider->setEnabled(false);
        ui->btot_SpinBox->setEnabled(false);

        ui->B_par_Slider->setEnabled(true);
        ui->B_par_spinBox->setEnabled(true);
        ui->B_perp_Slider->setEnabled(true);
        ui->B_perp_spinBox->setEnabled(true);
    }else{

        ui->angle_Slider->setEnabled(true);
        ui->angle_SpinBox->setEnabled(true);
        ui->btot_slider->setEnabled(true);
        ui->btot_SpinBox->setEnabled(true);

        ui->B_par_Slider->setEnabled(false);
        ui->B_par_spinBox->setEnabled(false);
        ui->B_perp_Slider->setEnabled(false);
        ui->B_perp_spinBox->setEnabled(false);

    }

}


void B_Theta_form::replot_slot(){

    nu_i = ui->nu_i_lineEdit->text().toInt() - 1;
    nu_j = ui->nu_j_lineEdit->text().toInt() - 1;
    nu_g = ui->nu_g_lineEdit->text().toInt() - 1;
    nu_f = ui->nu_f_lineEdit->text().toInt() - 1;

    n_i = ui->n_i_lineEdit->text().toInt();
    n_j = ui->n_j_lineEdit->text().toInt();
    n_g = ui->n_g_lineEdit->text().toInt();
    n_f = ui->n_f_lineEdit->text().toInt();

    nu_vector.clear();
    n_vector.clear();

    nu_vector.append(nu_i);
    nu_vector.append(nu_j);
    nu_vector.append(nu_g);
    nu_vector.append(nu_f);

    n_vector.append(n_i);
    n_vector.append(n_j);
    n_vector.append(n_g);
    n_vector.append(n_f);

    energy_vec.clear();

    energy_vec.append(erg_to_mev(energies_erg[nu_i]));
    energy_vec.append(erg_to_mev(energies_erg[nu_j]));
    energy_vec.append(erg_to_mev(energies_erg[nu_g]));
    energy_vec.append(erg_to_mev(energies_erg[nu_f]));

    this->angle_field_changes();

    qDebug()<<"replot_slot;";

}

