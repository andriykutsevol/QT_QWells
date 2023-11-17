#include <units_trans.h>


double abf(double abu){

    /* 1 нм = 0,000000001 м [ 10^ -9 ]
     *  1 м = 100 см
     *  1 нм = 0,0000001 см [ 10^ -7 ]}
     */

    // !!!!!!!!!!!!!!!!!!!!!!!!
    // разделили на 10 нанометров ( в сантиметрах)
    //return (abu*pow(10,-7)/(pow(10,-7) ) );			// x_tilde = x/a
    // размерность сократилась
    return abu;


}

double v0f(double v0){
    /* 1 эВ = 1,602 176 565(35)·10−19 Дж =
     * =1,602 176 565(35)·10−12 эрг.
     * 1 эрг = 1 г·см2/с2 = 10−7 Дж (точно)
     * = 6,24150965(16)·1011 эВ ≈ 0,6 ТэВ
     */
    // !!!!!!!!!!!!!!!!!!!!!!!!					U_tilde = U/U0
    //return ((v0*pow(10,-3)*1.60217656*pow(10,-12))/(240*pow(10,-3)*1.60217656*pow(10,-12)));
    // размерность сократилась
    return (v0/240);
    //return (v0*pow(10,-3)*1.60217656*pow(10,-12));
}


double m_ef (double motn){
    //me=9,10953*10-28 г
    return (motn*9.10938291*pow(10,-28));
}


double sc (double mef){
    // домножаем на a^2 и на U0 в целях обезразмеривания.
    // a x и E будем делить на a и U0 соответственно.

    // !!!!!!!!!!!!!!!!!!!!!!!!				gamma = sc * a^2 * U0;
    double v0_ob = 240*pow(10,-3)*1.60217656*pow(10,-12);
    double a_ob = pow(10,-(7*2));
    return ((2*mef * v0_ob * a_ob )/pow(h_bar,2));
    //return (2*mef/pow(hbar(),2));
}


double k1f (double eng, double sct){

    double tmp;
    tmp = sqrt(fabs(sct*eng));

    return (tmp);
}

double k2f (double eng, double sct, double v0){
    // сюда v0 и sct заходят уже обезразмеренные.
    // уже заходят обезразмеренными.
    return (sqrt(fabs(sct*(v0-eng))));
}

//----   Magnetic Field    -----------------------


double eng_to_erg(double engu){
    /* 1 эВ = 1,602 176 565(35)·10−19 Дж =
     * =1,602 176 565(35)·10−12 эрг.
     * 1 эрг = 1 г·см2/с2 = 10−7 Дж (точно)
     * = 6,24150965(16)·1011 эВ ≈ 0,6 ТэВ
     */
    // сначала наше число из мэв в эв. потом в эрги.
    return (engu*pow(10,-3)*1.60217656*pow(10,-12));
    // размерность сократилась
}


double erg_to_mev(double eng_erg){
    // милли pow(10,-3) значит 11 + 3 = 14
    return(eng_erg * 6.24150636 * pow(10,14));
}


double nano_to_cm(double x){
    /* 1 нм = 0,000000001 м [ 10^ -9 ]
     *  1 м = 100 см
     *  1 нм = 0,0000001 см [ 10^ -7 ]}
     */
    return (x * pow(10,-7));
}

double cm_to_nano(double x_cm){
    return (x_cm * pow(10,7));
}


double pre_deviations_a(double ef_mas_well){

    return( (m_ef(ef_mas_well) /2) * pow(e_charge/(c_veloc * m_ef(ef_mas_well)),2) );
}


double pre_landau_b(double ef_mas_well){

    return (h_bar * e_charge / (c_veloc * m_ef(ef_mas_well)));
}

double grad_rad(double grad){
    return (grad * pi/180.0);
}

double rad_grad(double rad){
    return (rad * 180.0 / pi);
}


double tesla_gauss(double tesla){
    return (tesla * pow(10,4));
}

double gauss_tesla(double gauss){
    return (gauss * pow(10,-4));
}
