#ifndef UNITS_TRANS_HPP
#define UNITS_TRANS_HPP

#endif // UNITS_TRANS_HPP

#include <math.h>

const double pi = 3.14159265358;

//h=6,626176*10-27 эрг*с
const double h_pl = 6.62606957 * pow(10,-27) ;

const double h_bar = 1.054571726 * pow(10,-27);

const double m_e = 9.10958215*pow(10,-28);
const double m_w = 0.067 * m_e;
const double e_charge = 4.803*pow(10,-10);
const double c_veloc = 2.998*pow(10,10);
const double emch = h_bar * e_charge/(m_w * c_veloc);
const double tl_gauss = 10000;  // Тл -> СГС.



double abf(double abu);

double v0f(double v0);


double m_ef (double motn);



double sc (double mef);



double k1f (double eng, double sct);



double k2f (double eng, double sct, double v0);


//----   Magnetic Field    -----------------------

double eng_to_erg(double engu);

double erg_to_mev(double eng_erg);

double nano_to_cm(double x);
double cm_to_nano(double x_cm);

//-----

double pre_deviations_a(double ef_mas_well);
double pre_landau_b(double ef_mas_well);

double grad_rad(double grad);
double rad_grad(double rad);

double tesla_gauss(double tesla);
double gauss_tesla(double gauss);





