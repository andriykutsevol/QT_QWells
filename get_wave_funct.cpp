#include "units_trans.h"
#include <myqwsystem.h>
#include <QDebug>

#include <math.h>
#include <gsl/gsl_integration.h>

#include <QVector>
#include <QVectorIterator>
#include <QString>
#include <QProcess>

#include <fstream>



double wf_phi(double x_point, void *p){

    double tmp_wf;
    //qDebug()<<"x_point: " << x_point;

    wf_phi_for_gsl_params *wf_params = (wf_phi_for_gsl_params *)p;

    int qw_numb = wf_params->qw_numb;



    int block_id;
    int blocks_amount = qw_numb * 2 + 1;            // _amount -> с единицы;
    int x_amount = wf_params->qw_numb * 2;

    int NS_amount = qw_numb * 4;
    int shif_amount = qw_numb;

    int k2_amount = qw_numb + 1;


    int x_count = 0;




    while(x_count < x_amount){
          if(x_point < wf_params->Qx_array[x_count]){
            block_id = x_count;
            //qDebug()<<"x_point: "<< x_point << " <  Qx_array: " << wf_params->Qx_array[x_count];
            break;
        }
        x_count ++;
    }

    // Так потому, что дальше последнего x итератор не идет
    // а точка там может находится.
    if(x_point >= wf_params->Qx_array[x_amount - 1]){
        //block_id = blocks_amount - 1;      // Это значит, что в последнем блоке.
        block_id = -1;
        //qDebug()<<"x_point: "<< x_point << " >=  Qx_array: " << wf_params->Qx_array[x_amount - 1];
    }


    double NS_tmp, NS_tmp2, k2_tmp, k1_tmp, shift_tmp, shift_tmp2;
    int ish;

    // Эффективные массы входят в k1 и k2;
    // Знаки входят НЕ Null Space;
    // они входят в коэффициэнты системы кторые являются элементами A.
    // например -0.0391055 а это и есть -exp(k2_tmp * x_point);
    // поэтому тут учитываем правильно знаки.
    // НО там на самом деле минус, потому, что перенесли через равно.
    // какой там может быть минус, если тода функция будет внизу? Там плюс!
    // Ну и подставляем.



    switch (block_id){
    case 0:
        NS_tmp = wf_params->NS[0];
        k2_tmp = wf_params->k2[0];
        // Тут координата < 0 поэтому экспонента убывает.
        tmp_wf = NS_tmp * exp(k2_tmp * x_point);
        //qDebug()<<"block_id = 0";
        break;
    case -1:
        NS_tmp = wf_params->NS[NS_amount - 1];          // последний индекс.
        k2_tmp = wf_params->k2[k2_amount - 1];          // последний индекс.
        // тут координата > 0 поэтому решение с -k2;
        shift_tmp = wf_params->shift[shif_amount - 1];  // последний индекс.
        tmp_wf = NS_tmp * exp(- k2_tmp * (x_point - shift_tmp));
        //qDebug()<<"block_id = last";
        break;
    default:
        //qDebug()<< "BLOCK ID"<< block_id;
        if(block_id % 2 == 0){
            // even
            //qDebug()<< "block_id: " << block_id;
            ish= block_id / 2;
            shift_tmp = wf_params->shift[ish -1];
            shift_tmp2 = wf_params->shift[ish];
            // одна экспонента убывающая другая возрастающая.
            NS_tmp = wf_params->NS[block_id * 2 - 1];
            NS_tmp2 = wf_params->NS[block_id * 2];      // это ж не последний блок.
            k2_tmp = wf_params->k2[block_id/2];
            tmp_wf = NS_tmp * exp(- k2_tmp * (x_point - shift_tmp))
                    + NS_tmp2 * exp(k2_tmp * (x_point - shift_tmp2));
            //tmp_wf = 0;
            // (x_point - shift_tmp2) < 0;


        }else{
            //odd
            *wf_params->sessionFile << "=======================" << std::endl;
            *wf_params->sessionFile << "blockId: " << block_id << std::endl;
            ish= block_id / 2;
            *wf_params->sessionFile << "ish: " << ish << std::endl;
            shift_tmp = wf_params->shift[ish];
            *wf_params->sessionFile << "shift_tmp: " << shift_tmp << std::endl;

            k1_tmp = wf_params->k1[block_id / 2];
            *wf_params->sessionFile << "k1_tmp: " << k1_tmp << std::endl;


            NS_tmp = wf_params->NS[block_id * 2 - 1];
            *wf_params->sessionFile << "NS_tmp: " << NS_tmp << std::endl;
            *wf_params->sessionFile << "sin: " << sin(k1_tmp * (x_point - shift_tmp)) << std::endl;

            NS_tmp2 = wf_params->NS[block_id * 2];
            *wf_params->sessionFile << "NS_tmp2: " << NS_tmp2 << std::endl;
            *wf_params->sessionFile << "cos: " << cos(k1_tmp * (x_point - shift_tmp)) << std::endl ;



            tmp_wf = NS_tmp * sin(k1_tmp * (x_point - shift_tmp))
                   + NS_tmp2 * cos(k1_tmp * (x_point - shift_tmp));
            *wf_params->sessionFile << "tmp_wf: " << tmp_wf << std::endl;

        }
        break;
    }

 //   return tmp_wf;


    switch (wf_params->power) {
    case 1:
        //qDebug()<<"power 1";
        return pow(wf_params->NC * tmp_wf,2) * x_point;
        break;
    case 2:
        //qDebug()<<"power 2";
        return pow(wf_params->NC * tmp_wf,2) * pow(x_point,2);
        break;

    case 0:
       // qDebug()<<"power 0";
        return pow(wf_params->NC * tmp_wf,2);
        break;

    case -1:
       // qDebug()<<"power 0";
        return pow(tmp_wf,2);
        break;
    default:
        //qDebug()<<"power error";
        return wf_params->NC * tmp_wf;
        break;
    }
}




void myQwSystem::get_nc_and_averages() {


    gsl_function gsl_wf_function;
    wf_phi_for_gsl_params wf_params;
    gsl_wf_function.function = &wf_phi;
    gsl_wf_function.params = &wf_params;
    gsl_integration_workspace * w_exp_sin_cos
            = gsl_integration_workspace_alloc (1000);

    // упаковка параметров
    // для вызова wf_phi_for_gsl--------------------------




    QVectorIterator<double> energies_iter(this->energies);
    std::ofstream integral_results_file("../DPL_QT/TXT_RES/ENG_WF/INTEGRALS.txt");

    int energy_count = 0;
    while(energies_iter.hasNext()){
        integral_results_file<<"---------- " << energies_iter.peekNext() << " -----------" << std::endl;
        // Массив иксов от энергии не зависит.
        wf_params.Qx_array = this->x;       // this->x is a QArray!
        // И шифты тоже не зависят от энергии.
        wf_params.shift = this->shift;

        wf_params.qw_numb = this->qw_numb;

        //wf_params.f_debug = new std::ofstream("../f_debug.txt" , std::ios::out);

        //*wf_params.f_debug << "hello";


        wf_params.sessionFile = new std::ofstream( "../f_debug.txt", std::ios::out );



        QVectorIterator<double> Qx_array_iter(wf_params.Qx_array);
        // Пределы
        //double lower_limit = Qx_array_iter.next() - 5;
        Qx_array_iter.toBack();
        //double upper_limit = Qx_array_iter.previous() + 5 ;
        integral_results_file<< "low: " << lower_limit << "; up: " << upper_limit  << std::endl;
        //-----------------------------------------------------
        // Только эти величины зависят от энергии.
        wf_params.k2 = this->k2_stored[energy_count];
        wf_params.k1 = this->k1_stored[energy_count];
        wf_params.NS = this->NS_stored[energy_count];

        //=========================================================
        // Норм пост. И норм ВФ.

        //  Итак с gsl_wf_function идет упакованная для каждой энергии структура.
        //  которая и используется в wf_phi_for_gsl
        double result, error;

        wf_params.power = -1;
        gsl_integration_qags(&gsl_wf_function,lower_limit, upper_limit, 0.01, 1e-3, 1000,w_exp_sin_cos, &result, &error);

        //    gsl_integration_qags (&My_function, lower_limit, upper_limit,
        //                abs_error, rel_error, 1000, work_ptr, &result, &error);


        integral_results_file<<"Not norm wf: " << result  << std::endl;

        //--------------
        wf_params.NC = sqrt(1/result);
        //--------------

        integral_results_file << "norm_const: " << wf_params.NC  << std::endl;
        this->norm_consts.append(wf_params.NC);




        // pure norm wf
        wf_params.power = 0;
        gsl_integration_qags(&gsl_wf_function,lower_limit, upper_limit, 0.01, 1e-3, 1000,w_exp_sin_cos, &result, &error);
        integral_results_file << "pure norm wf: " << result  << std::endl;   //== 1

        //--------------------------------
        // печатаем нормированную ВФ.
        wf_params.power = 3;
        this->wf_params_array.append(wf_params);
        //double j_low = wf_params.Qx_array[0] - 5;
        //double j_top = wf_params.Qx_array[this->qw_numb * 2 - 1] + 5;
        double j = lower_limit;
        while (j < upper_limit){
            *wf_files[energy_count] << j<< " " << wf_phi(j, &wf_params) << std::endl;
            j = j+x_step;
        }


        //=========================================================
        // average, averages_pow2_cm;
        wf_params.power = 1;
        gsl_integration_qags(&gsl_wf_function,lower_limit, upper_limit, 0.01, 1e-3, 1000,w_exp_sin_cos, &result, &error);
        this->averages.append(result);
        this->averages_pow2.append(pow(result,2));
        this->averages_cm.append(nano_to_cm(result));
        this->averages_pow2_cm.append(pow(nano_to_cm(result),2));
        integral_results_file << "average: " << result  << std::endl;
        integral_results_file << "average_pow2: " << pow(result,2)  << std::endl;
        integral_results_file << "average_cm: " << nano_to_cm(result)  << std::endl;
        integral_results_file << "average_cm_pow2: " << pow(nano_to_cm(result),2)  << std::endl;
        double aver_pow2_cm_temp = pow(nano_to_cm(result),2);
        // печатаем.
        j = lower_limit;
        while (j < upper_limit){
            *wf_average_files[energy_count] << j<< " " << wf_phi(j, &wf_params) << std::endl;
            j = j+x_step;
        }

        j = lower_limit;
        while (j < upper_limit){
            *wf_average_pow2_files[energy_count] << j<< " " << pow( wf_phi(j, &wf_params),2) << std::endl;
            j = j+x_step;
        }


       //=========================================================
        // get pow2_averages
        wf_params.power = 2;

        gsl_integration_qags(&gsl_wf_function,lower_limit, upper_limit, 0.01, 1e-3, 1000,w_exp_sin_cos, &result, &error);

        this->pow2_averages.append(result);
        this->pow2_averages_cm.append(nano_to_cm(nano_to_cm(result)));    // переход два раза так как квадрат.

        integral_results_file << "pow2_average: " << result  << std::endl;
        integral_results_file << "pow2_average - average_pow2 = " << result - averages_pow2.last()<< std::endl;
        this->std_deviations_pow2.append(result - averages_pow2.last()) ;
        integral_results_file << "pow2_average_cm - average_pow2_cm: " << this->pow2_averages_cm.last() - aver_pow2_cm_temp  << std::endl;
        //integral_results_file << "standard deviation: " <<      this->pow2_averages_cm.last() - aver_pow2_cm_temp << std::endl;
        this->std_deviations_pow2_cm.append(this->pow2_averages_cm.last() - aver_pow2_cm_temp);
        j = lower_limit;
        while (j < upper_limit){
            *wf_pow2_average_files[energy_count] << j<< " " << wf_phi(j, &wf_params) << std::endl;
            j = j+x_step;
        }

        energy_count ++;
        energies_iter.next();
        //break;

   }

    // QW_LOT
    std::ofstream qw_plot_file("../DPL_QT/TXT_RES/ENG_WF/QW_PLOT.txt");

    QVectorIterator<double> qw_x_iter(x);
    float j;
    while (qw_x_iter.hasNext()){
        j = 0;
        while(j<1){
            qw_plot_file<<qw_x_iter.peekNext() << " " << j <<std::endl;
            j = j+0.00001;
        }
        qw_x_iter.next();
    }

}


void myQwSystem::get_just_averages(){


    gsl_function gsl_wf_function;
    wf_phi_for_gsl_params wf_params;
    gsl_wf_function.function = &wf_phi;
    gsl_wf_function.params = &wf_params;
    gsl_integration_workspace * w_exp_sin_cos
            = gsl_integration_workspace_alloc (1000);

    // упаковка параметров
    // для вызова wf_phi_for_gsl--------------------------

    QVectorIterator<double> energies_iter(this->energies);
    wf_params.sessionFile = new std::ofstream( "../f_debug.txt", std::ios::out );

    int energy_count = 0;
    while(energy_count < 2){
        wf_params.Qx_array = this->x;
        wf_params.shift = this->shift;
        wf_params.qw_numb = this->qw_numb;

        QVectorIterator<double> Qx_array_iter(wf_params.Qx_array);
        //double lower_limit = Qx_array_iter.next() - 5;
        Qx_array_iter.toBack();
        //double upper_limit = Qx_array_iter.previous() + 5 ;

        //-----------------------------------------------------
        // Только эти величины зависят от энергии.
        wf_params.k2 = this->k2_stored[energy_count];
        wf_params.k1 = this->k1_stored[energy_count];
        wf_params.NS = this->NS_stored[energy_count];

        double result, error;
        wf_params.power = -1;


        gsl_integration_qags(&gsl_wf_function,lower_limit, upper_limit, 0.01, 1e-3, 1000,w_exp_sin_cos, &result, &error);

        wf_params.NC = sqrt(1/result);
        //this->norm_consts.append(wf_params.NC);





        //------------------------------------------------------

        wf_params.power = 1;
        gsl_integration_qags(&gsl_wf_function,lower_limit, upper_limit, 0.01, 1e-3, 1000,w_exp_sin_cos, &result, &error);


        if(energy_count == 0){
            wf_params.power = 3;
            double j = lower_limit;
            while (j < upper_limit){
                *wfs_for_aver_0<< j<< " " << wf_phi(j, &wf_params) << std::endl;
                j = j+x_step;
            }
            just_averages_0.append(result);
        }else{
            wf_params.power = 3;
            double j = lower_limit;
            while (j < upper_limit){
                *wfs_for_aver_1 << j<< " " << wf_phi(j, &wf_params) << std::endl;
                j = j+x_step;
            }
            just_averages_1.append(result);
        }


        energy_count ++;
    }


}




double myQwSystem::wf_phi_for_monte(){

    return 0;
}


void myQwSystem::get_norm_consts(){


}
