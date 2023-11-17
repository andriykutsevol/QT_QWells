#include "myqwsystem.h"
#include "units_trans.h"
#include <QVarLengthArray>
#include <QVectorIterator>

#include <math.h>
#include <iostream>
#include <QDebug>

// LU
#include <boost/numeric/ublas/lu.hpp>
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/vector.hpp>



#include <lapacke.h>




myQwSystem::myQwSystem()
{
    //x.append(2.34);
    //qDebug()<<x[0];
    this->test_max_field = 6;
    this->Ws_eval_str = "";
}



void myQwSystem::get_const_data(){

    v0.clear();
    QVectorIterator<double> iter_qw_height(this->qw_height);
    while(iter_qw_height.hasNext()){
        v0.append(v0f(iter_qw_height.next()));
        //qDebug()<< "v0: " << v0.last();
    }


    //m
    mb.clear();  // это для интерфейса.
    QVectorIterator<double> iter_emas_bar(this->ef_mas_bar);
    while(iter_emas_bar.hasNext()){
        mb.append(m_ef(iter_emas_bar.next()));
        //qDebug()<< "mb: " << mb.last();
    }

    mw.clear();
    QVectorIterator<double> iter_emas_well(this->ef_mas_well);
    while(iter_emas_well.hasNext()){
        mw.append(m_ef(iter_emas_well.next()));
        //qDebug()<< "mw: " << mw.last();
    }

    scw.clear();
    QVectorIterator<double> iter_scw(this->mw);
    while(iter_scw.hasNext()){
        scw.append(sc(iter_scw.next()));
        //qDebug()<< "scw: " << scw.last();
    }

    scb.clear();
    QVectorIterator<double> iter_scb(this->mb);
    while(iter_scb.hasNext()){
        scb.append(sc(iter_scb.next()));
        //qDebug()<< "scb: " << scb.last();
    }

    QVectorIterator<double> iter_qw_width(this->qw_width);
    iter_qw_width.toFront();
    x.clear();

    this->x.push_back(- abf(iter_qw_width.next()) / 2.0);
    iter_qw_width.toFront();
    //qDebug()<< "x: " << x.last();


    this->x.push_back( abf(iter_qw_width.next()) / 2.0);
    iter_qw_width.toFront();
    //qDebug()<< "x: " << x.last();



    QVectorIterator<double> iter_x(this->x);
    iter_x.next(); // щелкнуть коорнитату из отриц. знач. на ноль.
    iter_qw_width.next();

    double tmp1 = 0, tmp2 = 0;
    while (iter_qw_width.hasNext()) {
        tmp1 = iter_x.next();
        tmp2 = abf(iter_qw_width.next());
        this->x.push_back(tmp1 + tmp2);
        iter_x = this->x;
        iter_x.toBack();
        iter_x.previous();
        //qDebug()<< "x: " << x.last();
    }

    iter_x = this->x;
    while(iter_x.hasNext())
        this->x_nanometers.append(nano_to_cm(iter_x.next()));


    shift.clear();
    int ish = 0;
    this->shift.fill(0, qw_numb);
    for(int i=1; i <= (qw_numb - 1); i++){
                shift[i] =
                shift[i-1]
                + abf(qw_width[i-1+ish]) / 2.0
                + abf(qw_width[i+ish])
                + abf(qw_width[i+ish+1])/2.0;
         ish = ish + 1;
         //qDebug()<<"shift: " << shift[i];
    }

    //double append = (x[qw_numb * 2 -1] - 2*x[1])/2;
    QVectorIterator<double> qw_width_iter(qw_width);
    double append = 0;
    while(qw_width_iter.hasNext())
        append += qw_width_iter.next();

    lower_limit = x[0] - append/2;
    upper_limit = x[qw_numb * 2 -1] + append/2;
    x_step = (upper_limit - lower_limit)/200;

}

void myQwSystem::make_data(){

    
    eng = v0f(engu);

    QVectorIterator<double> iter_scw(this->mw);
    k1.clear();
    iter_scw = this->scw;
    iter_scw.toFront();
    while(iter_scw.hasNext()){
        this->k1.append(k1f(eng, iter_scw.next()));
        //qDebug()<< "k1: " << k1.last();
    }

    QVectorIterator<double> iter_scb(this->mb);
    QVectorIterator<double> iter_qw_height(this->qw_height);
    k2.clear();
    iter_scb = this->scb;
    iter_scb.toFront();
    iter_qw_height = v0;
    iter_qw_height.toFront();
    while(iter_scb.hasNext()){
        this->k2.append(k2f(eng, iter_scb.next(), iter_qw_height.next()));
        //qDebug()<< "k2: " << k2.last();
    }




    
}


void myQwSystem::fill_data(){

    int dl, dll, dll2, xdl, shdl, mwdl, mbdl, dln, i, xsh;
    double nc;
    this->A.fill(0, pow(this->qw_numb * 4,2));


    A[0] = - exp(k2[0]*(x[0] - shift[0]));
    A[1] =   sin(k1[0]*(x[0] - shift[0]));
    A[2] =   cos(k1[0]*(x[0] - shift[0]));

    dl = qw_numb * 4;

    A[dl] = - k2[0] * exp(k2[0]*(x[0] - shift[0]));

    A[dl+1] = (1/mw[0])/(1/mb[0]) * k1[0] * cos(k1[0]*x[0] - shift[0]);

    A[dl+2] = - (1/mw[0])/(1/mb[0]) * k1[0] * sin(k1[0]*x[0] - shift[0]);

    dln = qw_numb * 4 +  qw_numb * 4 + 1;
    xsh = 0;
    nc = (qw_numb * 4 - 4)/4;
    i = 1;

    while(i <= nc){

        A[dln] = sin(k1[i-1]* (x[i+xsh]- shift[i-1]));
        A[dln+1] = cos(k1[i-1]* (x[i+xsh]- shift[i-1]));
        A[dln+2] = -exp(- k2[i]* (x[i+xsh]- shift[i-1]));
        A[dln+3] = -exp(  k2[i]* (x[i+xsh]- shift[i]));

        dln = dln + qw_numb * 4;

        A[dln]   =     k1[i-1] * (1/ mw[i-1])/(1/ mb[i])  * cos(k1[i-1] * (x[i+xsh]- shift[i-1]));
        A[dln+1] = - k1[i-1] * (1/ mw[i-1])/(1/ mb[i])  * sin(k1[i-1] * (x[i+xsh]- shift[i-1]));
        A[dln+2] =   k2[i] *   exp(- k2[i]* (x[i+xsh]- shift[i-1]));
        A[dln+3] = - k2[i] *   exp(  k2[i]* (x[i+xsh]- shift[i]));

        dln = dln + 3;
        dln = dln + qw_numb * 4 - 1;

        A[dln] =   -exp(- k2[i]* (x[i+1+xsh]- shift[i-1]));
        A[dln+1] = -exp(  k2[i]* (x[i+1+xsh]- shift[i]));
        A[dln+2] = sin(k1[i]* (x[i+1+xsh]- shift[i]));
        A[dln+3] = cos(k1[i]* (x[i+1+xsh]- shift[i]));

        dln = dln + qw_numb * 4;

        A[dln] =     k2[i] * exp (- k2[1]* (x[i+1+xsh]- shift[i-1]));
        A[dln+1] = - k2[i] * exp (  k2[1]* (x[i+1+xsh]- shift[i]));
        A[dln+2] =   k1[i] * (1/ mw[i])/(1/ mb[i]) * cos(k1[i]* (x[i+1+xsh]- shift[i]));
        A[dln+3] = - k1[i] * (1/ mw[i])/(1/ mb[i]) * sin(k1[i]* (x[i+1+xsh]- shift[i]));

        i = i+1;
        xsh = xsh + 1;
        dln = dln + 3;
        dln = dln + qw_numb * 4 - 1;


    }



    dll = (qw_numb * 4) * (qw_numb * 4) - 1;
    xdl = (qw_numb * 2) - 1;
    shdl = qw_numb - 1;
    mwdl = qw_numb - 1;
    mbdl = qw_numb;

    A[dll] = k2[mbdl] * exp(- k2[mbdl]*(x[xdl]- shift[shdl]));

    A[dll-1] = -(1/mw[mwdl])/(1/mb[mbdl]) * k1[mwdl] * sin(k1[mwdl] * (x[xdl] - shift[shdl]));

    A[dll-2] =  (1/mw[mwdl])/(1/mb[mbdl]) * k1[mwdl] * cos(k1[mwdl] * (x[xdl] - shift[shdl]));

    dll2 = (qw_numb * 4) * (qw_numb * 4) - 1 - qw_numb * 4;

    A[dll2] = -exp(- k2[mbdl] * (x[xdl] - shift[shdl]));

    A[dll2-1] =  cos(k1[mwdl] * (x[xdl] - shift[shdl]));

    A[dll2-2] =  sin(k1[mwdl] * (x[xdl] - shift[shdl]));

}


void myQwSystem::find_det(){

    int por = qw_numb * 4;
    boost::numeric::ublas::matrix<double> Array(por,por);

    int k=0, i=0, j=0;
    for(i=0;i<por;i++){
        for(j=0;j<por;j++){
            // Матрица А, прежде заполняется для определенной энергии
            // И эту нергию мы сохраняем: energies_tmp.append(engu);
            Array(i,j) = this->A[k];
            k++;
        }
    }

    boost::numeric::ublas::lu_factorize(Array);

    double tmp = 1;
    for (i=0;i<por;i++){
        tmp *= Array(i,i);
    }
    // Матрицу а мы не сохраняем для каждой энергии.
    // а определители и энергии сохраняем. A, мы перезаполняем в fill_data.
    dets_tmp.append(tmp);
    energies_tmp.append(engu);
}


void myQwSystem::find_roots(){
    QVectorIterator<double> dets_tmp_iter(dets_tmp);
    QVectorIterator<double> energies_tmp_iter(energies_tmp);


    while(dets_tmp_iter.hasNext()){
        if((dets_tmp_iter.next() * dets_tmp_iter.peekNext()) < 0 ){
            energies_erg.append(eng_to_erg(energies_tmp_iter.peekNext()));
            energies.append(energies_tmp_iter.next());


        }else{
        energies_tmp_iter.next();
        }

    }

}

void myQwSystem::save_states(){

    QVectorIterator<double> energies_iter(energies);
    while (energies_iter.hasNext()){
        this->engu = energies_iter.next();
        this->make_data();
        this->fill_data();
        A_stored.append(A);
        k1_stored.append(k1);
        k2_stored.append(k2);
    }

}


void myQwSystem::get_null_space(){

    QVectorIterator<double> energies_iter(energies);
    QVectorIterator<QVector<double> > A_stored_iter(A_stored);


    int i;
    while(energies_iter.hasNext()){

        energies_iter.next();


        int por = qw_numb*4;
        double *A_temp = new double[por*por];



        QVectorIterator<double> A_stored_elem_iter(A_stored_iter.next());
        i = 0;
        while(A_stored_elem_iter.hasNext()){
            A_temp[i] = A_stored_elem_iter.next();
            //qDebug()<< "   A: " << A_temp[i];
            i++;
        }

        lapack_int m = por, n = por;
        lapack_int ldu = por, ldvt = por, info, lwork, lda=por;

        double *s = new double[por];
        double *v = new double[por*por];
        double *u = new double[por*por];
        double *superb = new double[por - 1];

        info = LAPACKE_dgesvd( LAPACK_COL_MAJOR, 'A', 'A', m, n, A_temp, lda,
                               s, u, ldu, v, ldvt, superb );

        //Сингулярных значений столько скольно строк в матрице.
        int zero_sv_pos;
        int k = 0;
        for(i=0; i<por; i++){
            //qDebug()<< "sv:" << s[i];
            if (s[i]<0.01){	// Отбор нулевых синг. значений.
                zero_sv_pos = k;
                //qDebug()<< "zevo_sv_position: " << k;
            }
            //qDebug()<< "sing_val: " << s[i];
            k++;
        }


        // вычленить из u, нуль спейс и сохранить в NS_stored
        // для одной энергии.
        QVector<double> NS_stored_string;
//        QVectorIterator<int>zero_sv_pos_iter(zero_sv_pos);
//        while(zero_sv_pos_iter.hasNext()){
        for(k=0;k<por;k++){
            NS_stored_string.append(u[por*zero_sv_pos+k]);
//                qDebug()<<"  NS: "<< NS_stored_string[k];
        }
            NS_stored.append(NS_stored_string);
//            zero_sv_pos_iter.next();
//        }

        delete A_temp, s, v, u, superb;




    }
}












    //qDebug() << "k1_stored_iter.next()" << k1_stored_iter.next();
   //qDebug() << "k1_sotred_elem_iter:" << k1_stored_elem_iter.next();
   //k1_stored_elem_iter = k1_stored_iter.next();
   //qDebug() << "k1_sotred_elem_iter:" << k1_stored_elem_iter.next();



void myQwSystem::clear_all(){
    this->qw_numb = 0;
    this->ef_mas_bar.clear();
    this->ef_mas_well.clear();
    this->qw_height.clear();
    this->qw_width.clear();

    this->ef_mas_bar_str.clear();
    this->ef_mas_well_str.clear();
    this->qw_height_str.clear();
    this->qw_width_str.clear();

//    this->v0.clear();
//    this->x.clear();
//    this->shift.clear();
//    this->k1.clear();
//    this->mw.clear();
//    this->scw.clear();
//    this->k2.clear();
//    this->mb.clear();
//    this->scb.clear();
    this->A.clear();
    this->energies.clear();
    this->energies_tmp.clear();
    this->dets_tmp.clear();
    this->NS_stored.clear();
    this->k1_stored.clear();
    this->k2_stored.clear();
    this->A_stored.clear();
    this->norm_consts.clear();
    this->B_stored.clear();
    this->B.clear();
    this->Transitions_string.clear();
    this->averages_pow2_cm.clear();
    this->pow2_averages_cm.clear();
    this->energies_erg.clear();
    this->x_nanometers.clear();
    this->Ws_eval_str.clear();
    this->Transitions_string.clear();
}


void myQwSystem::remove_sel(int selected_block){
    this->qw_numb --;


   ef_mas_well.remove(selected_block);
   ef_mas_well_str.remove(selected_block);

   ef_mas_bar.remove(selected_block + 1);
   ef_mas_bar_str.remove(selected_block + 1);

   qw_height.remove(selected_block + 1);
   qw_height_str.remove(selected_block + 1);


   qw_width.remove((selected_block * 2)-1);
   qw_width_str.remove((selected_block * 2)-1);

   qw_width.remove((selected_block * 2)-1);
   qw_width_str.remove((selected_block * 2)-1);
}


void myQwSystem::clear_res(){

    this->A.clear();
    this->energies.clear();
    this->energies_tmp.clear();
    this->dets_tmp.clear();
}



void myQwSystem::clear_before_add(){
    A_stored.clear();
    NS_stored.clear();
    k1_stored.clear();
    k2_stored.clear();
    v0.clear(); x.clear(); shift.clear(); mw.clear(); scw.clear(); mb.clear(); scb.clear();
    k1.clear(); k2.clear(); A.clear(); energies_tmp.clear(); dets_tmp.clear();
    energies.clear(); norm_consts.clear();

}


void myQwSystem::plot_arevages(){
    qDebug()<<"plot_averages";
//    this->clear_res();
//    this->clear_before_add();

    qw_numb = 2;                                        // !!
    eng_step = 0.1;

    //---------------------------
    ef_mas_bar.append(0.067);
    //ef_mas_bar_str.append("ef_mas_lbar");
    ef_mas_well.append(0.067);
    //ef_mas_well_str.append("ef_mas_well");
    ef_mas_bar.append(0.067);
    //ef_mas_bar_str.append("ef_mas_lbar");

    qw_height.append(240);
    //qw_height_str.append("qw_left_height");

    qw_height.append(240);
    //qw_height_str.append("qw_left_height");

    qw_width.append(25);
    //qw_height_str.append("qw_left_height");
    qw_width.append(2);
    //qw_width_str.append("bar_width");
    //----------------------------
    ef_mas_well.append(0.067);
    ef_mas_well_str.append("well");
    ef_mas_bar.append(0.067);
    ef_mas_bar_str.append("ef_bar_well");

    qw_height.append(240);
    qw_height_str.append("qw_left_height");

    qw_width.append(25);         // задаем в цикле
    qw_width_str.append("bar_width");
    //-----------------------------

    just_averages_file = new std::ofstream("../DPL_QT/TXT_RES/WIDTH_WF/just_averages.txt");
    QString wfs_for_aver_fname;
    float var_well = 25;
    while(var_well >= 5){
        wfs_for_aver_fname = "../DPL_QT/TXT_RES/WIDTH_WF/" + QString::number(var_well) + "_width_wf_0.txt";
        char *wfs_for_aver_fname_char = wfs_for_aver_fname.toLocal8Bit().data();
        wfs_for_aver_0 = new std::ofstream(wfs_for_aver_fname_char, std::ios::out);

        wfs_for_aver_fname = "../DPL_QT/TXT_RES/WIDTH_WF/" + QString::number(var_well) + "_width_wf_1.txt";
        wfs_for_aver_fname_char = wfs_for_aver_fname.toLocal8Bit().data();
        wfs_for_aver_1 = new std::ofstream(wfs_for_aver_fname_char, std::ios::out);

        qw_width[2] = var_well;             // !!
        get_const_data();
        engu = 1;
        while(engu < 240){
            make_data();
            fill_data();
            find_det();
            engu = engu + eng_step;
        }

        find_roots();
        save_states();
        get_null_space();
        // мы их там добавляем в приват.
        get_just_averages();
        clear_res();
        clear_before_add();
        var_well = var_well - 0.1;          //!!

        delete wfs_for_aver_0;
        delete wfs_for_aver_1;
    }

    QVectorIterator<double> aver_0_iter(just_averages_0);
    QVectorIterator<double> aver_1_iter(just_averages_1);

    var_well = 25;
    while(aver_0_iter.hasNext()){
        *just_averages_file<<var_well << " " << (aver_0_iter.next()) - 13.5 << " " << (aver_1_iter.next()) - 13.5 << std::endl;
        var_well = var_well - 0.1;          //!!
    }


}



void myQwSystem::create_res_files(){
    qDebug()<<"create_res_files";
    QVectorIterator<double> energies_iter(energies);
    while (energies_iter.hasNext()){
        this->engu = energies_iter.next();
        QString tmp_str = "../DPL_QT/TXT_RES/ENG_WF/" + QString::number(engu) + "_wf.txt";
        char *wf_fname = tmp_str.toLocal8Bit().data();
        std::ofstream *wf_file =
                new std::ofstream( wf_fname, std::ios::out);
        this->wf_files.append(wf_file);


        tmp_str = "../DPL_QT/TXT_RES/ENG_WF/" + QString::number(engu) + "_aver_wf.txt";
        wf_fname = tmp_str.toLocal8Bit().data();
        wf_file =
                new std::ofstream( wf_fname, std::ios::out);
        this->wf_average_files.append(wf_file);

        tmp_str = "../DPL_QT/TXT_RES/ENG_WF/" + QString::number(engu) + "_pow2_aver_wf.txt";
        qDebug()<< tmp_str << " is created";
        wf_fname = tmp_str.toLocal8Bit().data();
        wf_file =
                new std::ofstream( wf_fname, std::ios::out);
        this->wf_pow2_average_files.append(wf_file);

        tmp_str = "../DPL_QT/TXT_RES/ENG_WF/" + QString::number(engu) + "_aver_pow2_wf.txt";
        wf_fname = tmp_str.toLocal8Bit().data();
        wf_file =
                 new std::ofstream( wf_fname, std::ios::out);
         this->wf_average_pow2_files.append(wf_file);

    }

}


double myQwSystem::get_engValue_toPlot(int energyID){
    qDebug() << "get_engValut_toPlot: " << energyID;
    return this->energies[energyID];

}

QVector<double> myQwSystem::get_x_range_toPlot(){
    QVector<double> xy_range;
    xy_range.append(this->lower_limit);
    xy_range.append(this->upper_limit);
    return xy_range;
}


QVector< QVector<double> > myQwSystem::get_k1k2_toPlot(int energyID){
    QVector <QVector<double> > k1k2;
    k1k2.append(this->k1_stored[energyID] );
    k1k2.append(this->k2_stored[energyID] );
    return k1k2;
}

QVector<double> myQwSystem::get_NS_toPlot(int energyID){
    return this->NS_stored[energyID];
}

QVector<double> myQwSystem::get_Qx_array_toPlot(){
    return this->x;
}


