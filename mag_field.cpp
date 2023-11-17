#include"units_trans.h"
#include<myqwsystem.h>
#include<QDebug>
#include<QVector>
#include<QVectorIterator>
#include <QString>

#include<iostream>
#include <math.h>
#include <gsl/gsl_integration.h>

using namespace std;



void myQwSystem::get_transitions(){

    Ws_eval_str = "";

    // Поиск b_min;
    mag_field_file = new std::ofstream("../DPL_QT/TXT_RES/MAG_FIELD/mag_field.txt", std::ios::out);

    n_g = 0; n_f = 1;
    theta_current = theta_from;
    theta_current_rad = theta_current * (pi/180);
    //get_B_theta_0();
    this->B_0 = 0;

    //theta_current = theta_from + theta_step;
    while(theta_current < theta_to){
        this->b_current = 0;
        theta_current_rad = theta_current * (pi/180);

        c = energies_erg[nu_i] + energies_erg[nu_j]
                -energies_erg[nu_g] - energies_erg[nu_f];

        a = (mw[0] /2) * pow(e_charge/(c_veloc * mw[0]) *sin(theta_current_rad),2)
                * (std_deviations_pow2_cm[nu_i] + std_deviations_pow2_cm[nu_j]
                   - std_deviations_pow2_cm[nu_g] - std_deviations_pow2_cm[nu_f]);

        b = (h_bar * e_charge * cos(theta_current_rad))/(c_veloc * mw[0]) *
                (n_i + n_j - n_g - n_f);




        *mag_field_file <<"theta: "<< theta_current << std::endl;
        *mag_field_file << "B_TESLA: " << B_find_root() * 1.0/10000.0 << std::endl;
        *mag_field_file << "DISCRIMINANT: " << (pow(b,2) - 4 * a * c) << std::endl;
        *mag_field_file << "+++++++++++++++++++++" << std::endl;

        theta_current += theta_step;
    }
}


double myQwSystem::B_find_root(){
    this->b_step = 10;

    this->b_current = this->B_0;   //gauss.
    while(b_current < 1000000){
        if(this->peek_B_current()){     // если на данном и следующем поле
                                        // функция поменяла знак
            //*mag_field_file << "********** SIGN CHANGED *******************************" << std::endl;
            return b_current;
        }

        b_current += b_step;
    }

    return -1;
}

bool myQwSystem::peek_B_current(){
    double tmp1, tmp2;

    tmp1 = c + pow(b_current,2)*a + b_current * b;
    tmp2 = c + pow(b_current + b_step,2)*a + (b_current + b_step) * b;
    //*mag_field_file << "theta: " << theta_current << std::endl;
    //*mag_field_file << "a: " << a << std::endl;
    //*mag_field_file << "b: " << b << std::endl;
    //*mag_field_file << "c: " << c << std::endl;
    //*mag_field_file << "peek_B_current: " << b_current << " ; " << tmp1 << std::endl;

    if((tmp1 * tmp2) < 0)
        return true;
    return false;

}


void myQwSystem::get_B_theta_0(){
    double tmp,c,b,a;
    c = energies_erg[nu_i] + energies_erg[nu_j]
            -energies_erg[nu_g] - energies_erg[nu_f];

    a = (mw[0] /2) * pow(e_charge/(c_veloc * mw[0]) *sin(theta_current_rad),2)
            * (std_deviations_pow2_cm[nu_g] + std_deviations_pow2_cm[nu_f]
               - std_deviations_pow2_cm[nu_i] - std_deviations_pow2_cm[nu_j]);


    b = (h_bar * e_charge * cos(theta_current_rad))/(c_veloc * mw[0]) *
            (n_g + n_f - n_i - n_j);

    this->B_0 = c/b;
    //qDebug()<< "B_0: " << this->B_0;
    //qDebug()<<"peek_B_0: " << c - pow(b_current,2)* a - b_current * b;

}





double myQwSystem::get_field_from_n_theta(){

     double a,b,c,d;
     double x1,x2;

     c = energies_erg[nu_i] + energies_erg[nu_j]
             -energies_erg[nu_g] - energies_erg[nu_f];



     a = (mw[0] /2) * pow(e_charge/(c_veloc * mw[0]) *sin(theta_current_rad),2)
             * (std_deviations_pow2_cm[nu_g] + std_deviations_pow2_cm[nu_f]
                - std_deviations_pow2_cm[nu_i] - std_deviations_pow2_cm[nu_j]);

     b = (h_bar * e_charge * cos(theta_current_rad))/(c_veloc * mw[0]) *
             (n_g + n_f - n_i - n_j);

     *mag_field_file << "before_dev: " << (mw[0] /2) * pow(e_charge/(c_veloc * mw[0]) *sin(theta_current_rad),2) << std::endl;
     *mag_field_file << "dev_res: " << (std_deviations_pow2_cm[nu_g] + std_deviations_pow2_cm[nu_f]
                                        - std_deviations_pow2_cm[nu_i] - std_deviations_pow2_cm[nu_j]) << std::endl ;
     *mag_field_file << "averages_cm[nu_g]: " << this->averages_cm[nu_g] << std::endl;
     *mag_field_file << "averages_cm[nu_i]: " << this->averages_cm[nu_i] << std::endl;
     *mag_field_file << "averages_pow2_cm[nu_g]: " << this->averages_pow2_cm[nu_g] << std::endl;
     *mag_field_file << "averages_pow2_cm[nu_i]: " << this->averages_pow2_cm[nu_i] << std::endl;

     *mag_field_file << "pow2_averages_cm[nu_g]: " << this->pow2_averages_cm[nu_g] << std::endl;
     *mag_field_file << "pow2_averages_cm[nu_i]: " << this->pow2_averages_cm[nu_i] << std::endl;

     *mag_field_file << "std_deviations_pow2_cm[nu_g]: " << std_deviations_pow2_cm[nu_g] << std::endl;
     *mag_field_file << "std_deviations_pow2_cm[nu_f]: " << std_deviations_pow2_cm[nu_f] << std::endl;
     *mag_field_file << "std_deviations_pow2_cm[nu_i]: " << std_deviations_pow2_cm[nu_i] << std::endl;
     *mag_field_file << "std_deviations_pow2_cm[nu_j]: " << std_deviations_pow2_cm[nu_j] << std::endl;
     *mag_field_file << "a: "<< a << std::endl;
     *mag_field_file <<"b: "<< b << std::endl;
     *mag_field_file <<"emch:" << emch << std::endl;
     *mag_field_file <<"c: "<< c << std::endl;

     *mag_field_file << "(" << nu_i << "," << n_i << "),("
                            << nu_j << "," << n_j << ")";
     *mag_field_file << " --> ";
     *mag_field_file << "(" << nu_g << "," << n_g << "),("
             << nu_f << "," << n_f << ")";
     *mag_field_file << std::endl;


     if(a==0)
         {
             *mag_field_file << "The equation isn't square " << std::endl;;
             x1 = (mw[0] * c_veloc)/(h_bar * e_charge)
                     * (energies_erg[nu_i] + energies_erg[nu_j] - energies_erg[nu_g] - energies_erg[nu_f])
                     /(n_g + n_f + n_i + n_j);  // gauss

             x2 = x1 * 1/tl_gauss;      // tesla.

             *mag_field_file <<"c/b: " << c/b << std::endl;
             *mag_field_file << "B_GAUSS: " << x1 << std::endl;
             *mag_field_file << "B_TESLA: "    << x2 << std::endl;
             *mag_field_file << "h_bar * e_charge/(m_w * c_veloc) * B_GAUSS: " << emch * x1 << std::endl;
             *mag_field_file << "2[E2 - E1]: " << 2*(energies_erg[nu_i] - energies_erg[nu_g]) << std::endl;
             *mag_field_file << "again B: " <<
                                2*(energies_erg[nu_i] - energies_erg[nu_g]) * mw[0] * c_veloc / (h_bar * e_charge)
                                << std::endl;


         }
         else
         {
             d=b*b-4*a*c;
             if(d<0)
             {
                 *mag_field_file << "The equation has no solution " << std::endl;

             }
             if(d==0)
             {
                 *mag_field_file << "The equation has 2 identical roots of the equation: " << std::endl;
                 x1=-b/(2*a);
                 x2 = x1 * 1/tl_gauss;      // tesla.
                 *mag_field_file <<  "B_gauss: " << x1  << std::endl;
                 *mag_field_file << "B_Tl: " << x2 << std::endl;
             }
             if(d>0)
             {
                 x1=(-b+sqrt(d))/(2*a);
                 x2=(-b-sqrt(d))/(2*a);

                 if( (x1 < 0) && (x2 < 0) ){
                     *mag_field_file << "x1 AND x2 < 0  !!" << std::endl;
                 }else{
                     //if(x1 > 0){
                         *mag_field_file << "B1_GAUSS = " << x1 << std::endl;
                         *mag_field_file << "B1_TESLA = " << x1/tl_gauss  << std::endl;
                     //}else{  // x2 > 0
                         *mag_field_file << "B2_GAUSS = " << x2 << std::endl;
                         *mag_field_file << "B2_TESLA = " << x2/tl_gauss  << std::endl;
                     //}
                 }
             }
         }



//    gsl_function gsl_wf_function;
//    wf_phi_for_gsl_params wf_params;
//    gsl_wf_function.function = &wf_phi;
//    gsl_wf_function.params = &wf_params;
//    gsl_integration_workspace * w_exp_sin_cos
//            = gsl_integration_workspace_alloc (1000);

//    // упаковка параметров
//    // для вызова wf_phi_for_gsl--------------------------

//    QVectorIterator<double> energies_iter(this->energies);

//    int energy_count = 0;
//    qDebug()<< "energy count: "<< energy_count;
//    wf_params.power = 1;   // 1;
//    // Массив иксов от энергии не зависит.
//    wf_params.Qx_array = this->x;       // this->x is a QArray!
//    // И шифты тоже не зависят от энергии.
//    wf_params.shift = this->shift;

//    wf_params.qw_numb = this->qw_numb;

//    //wf_params.f_debug = new std::ofstream("../f_debug.txt" , std::ios::out);

//    //*wf_params.f_debug << "hello";


//    wf_params.sessionFile = new std::ofstream( "../f_debug.txt", std::ios::out );



//    QVectorIterator<double> Qx_array_iter(wf_params.Qx_array);
//    // Пределы
//    double lower_limit = Qx_array_iter.next() - 0.5;
//    Qx_array_iter.toBack();
//    double upper_limit = Qx_array_iter.previous() + 0.5 ;
//    qDebug()<<"low: "<<lower_limit << " ; up: "<<upper_limit;

//    //-----------------------------------------------------
//    // Только эти величины зависят от энергии.
//    wf_params.k2 = this->k2_stored[energy_count];
//    wf_params.k1 = this->k1_stored[energy_count];
//    wf_params.NS = this->NS_stored[energy_count];


//    //  Итак с gsl_wf_function идет упакованная для каждой энергии структура.
//    //  которая и используется в wf_phi_for_gsl
//    double result, error;
//    gsl_integration_qags(&gsl_wf_function,lower_limit, upper_limit, 1, 1e-7, 1000,w_exp_sin_cos, &result, &error);

//    //-------------------------------------------------------

//    qDebug()<<"get_field_from_n_theta. Average x= " << result;


//    Ws_eval_str += "-----B_field: " + QString::number(this-> test_max_field);
//    Ws_eval_str += "<br>";
//    Ws_eval_str += "theta: " + QString::number(this->theta_current);
//    Ws_eval_str += "<br>";
//    Ws_eval_str += "(" + QString::number(nu_i) + "," + QString::number(n_i) + "),("
//                       + QString::number(nu_j) + "," + QString::number(n_j) + ")";
//    Ws_eval_str += " --> ";
//    Ws_eval_str += "(" + QString::number(nu_g) + "," + QString::number(n_g) + "),("
//                       + QString::number(nu_f) + "," + QString::number(n_f) + ")";
//    Ws_eval_str += "<br>";

}


void myQwSystem::get_fields_from_n(){

    int b_count = 1;
    while(b_count <= b_min){
        B_stored.append(b_count);
        b_count ++;
    }
    B = B_stored;


}


double myQwSystem::find_field_from_n(){
    double field = 0;
    cout<<"("<<this->nu_i<<","<<this->n_i<<"),("<<this->nu_j<<","<<this->n_j<<")";
    cout<<"-->";
    cout<<"("<<this->nu_g<<","<<this->n_g<<"),("<<this->nu_f<<","<<this->n_f<<")"<<endl;

    return field;
}


void myQwSystem::clear_field(){
    this->B_stored.clear();

}


void myQwSystem::eval_Ws_for_current_B_field(){


    int low_ind =  0;
    int top_ind = this->n_f_max;
    this->Ws_eval_str = "------ B _field: ";
    this->Ws_eval_str += QString::number(B[this->n_f_max - 1]);
    this->Ws_eval_str += "------ <br>";

    while (low_ind <= top_ind / 2){
        this->n_f = n_f_max - low_ind;
        this->n_g = low_ind;
        Ws_eval_str += "(" + QString::number(nu_i) + "," + QString::number(n_i) + "),("
                           + QString::number(nu_j) + "," + QString::number(n_j) + ")";
        Ws_eval_str += " --> ";
        Ws_eval_str += "(" + QString::number(nu_g) + "," + QString::number(n_g) + "),("
                           + QString::number(nu_f) + "," + QString::number(n_f) + ")";
        Ws_eval_str += "<br>";
        //cout<<"(    "<<this->nu_i<<","<<this->n_i<<"),("<<this->nu_j<<","<<this->n_j<<")";
        //cout<<"-->";
        //cout<<"("<<this->nu_g<<","<<this->n_g<<"),("<<this->nu_f<<","<<this->n_f<<")"<<endl;
        low_ind ++;
    }


    //cout<<"----f_max: "<<this->n_f_max<<endl;

}



//     Ws_eval_str += "a: " + QString::number(a);
//     Ws_eval_str += "<br>";
//     Ws_eval_str += "b: " + QString::number(b);
//     Ws_eval_str += "<br>";
//     Ws_eval_str += "c: " + QString::number(c);
//     Ws_eval_str += "<br>";
//     Ws_eval_str += "theta_grad: " + QString::number(this->theta_current);
//     Ws_eval_str += "<br>";
//     Ws_eval_str += "theta_rad: " + QString::number(this->theta_current_rad);
//     Ws_eval_str += "<br>";
//     Ws_eval_str += "cos(theta): " + QString::number(cos(this->theta_current_rad));
//     Ws_eval_str += "<br>";
//     Ws_eval_str += "sin(theta): " + QString::number(sin(this->theta_current_rad));
//     Ws_eval_str += "<br>";


//    bool field_limit_min_flg = false;
//    this->n_f_max = 1;   // взяли первый n_f_max;
//    // и будем их увеличивать пока поле не станет меньше b_min;
//    while(1){
//    //--------------------------------------------------------------
//        int low_ind =  0;               // установил границы
//        int top_ind = this->n_f_max;
//        qDebug()<<"get_transitions";
//        //------------------------------------------------
//        while (low_ind <= top_ind / 2){   // и начал перебирать по одному переходу.
//            qDebug()<<"get_transitions1";
//            this->n_f = n_f_max - low_ind;
//            this->n_g = low_ind;
//            // в каждом переходе просканировать угол, с вычислением поля.
//            this->theta_current = this->theta_from;
//            //---------------------------------
//            while(this->theta_current < this->theta_to){

//                // 1 градус = pi/180 радиан
//                // 1 радиан = 180/pi градусов
//                this->theta_current_rad = this->theta_current * 180/pi;

//                //this->test_max_field = get_field_from_n_theta();
//                get_field_from_n_theta();
//                // Если поле меньше минимального устанвить влаг и выйти.
//                if (this->test_max_field < this->b_min){
//                    field_limit_min_flg = true;
//                    break;
//                }

//                if (field_limit_min_flg)
//                    break;
//                this->theta_current = this->theta_current + this->theta_step;
//            }
//            //----------------------------------
//            this->test_max_field --;
//            low_ind ++;
//         }
//        //------------------------------------------------

//        if(field_limit_min_flg)
//            break;
//        this->n_f_max ++;
//     }
//    //--------------------------------------------------------------------


