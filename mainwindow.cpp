#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "wf_plot.h"
#include "ui_wf_plot.h"
#include "b_theta_form.h"
#include "ui_b_theta_form.h"

#include "ui_form_myplot.h"
#include "form_myplot.h"
#include "opengl_form.h"
#include "cuda_form.h"
#include "cudanew_class.h"
#include "cuda_oglnew_form.h"

#include "monte_cuda_lib.h"


#include <boost/filesystem.hpp>
#include <iostream>


#include "myqwsystem.h"
#include <QDebug>

#include <fstream>
#include <math.h>


#include <QPushButton>
#include <QGroupBox>
#include <QString>
#include <QVectorIterator>
#include <QTextCursor>
#include <QTextBlock>
#include <QTextBlockFormat>
#include <QTextDocument>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    myQwSystem()       // а можно и так.
{

    this->tmp = 0;
    ui->setupUi(this);
    //myQwSystem well_sys(2); //можно так.
    this->qw_numb = 0;
    //this->selected_block = 0;
    ui->bar_width->setEnabled(false);
    ui->remove_sel_button->setEnabled(false);
    //ui->get_energy_Button->setEnabled(false);
    ui->eng_step_LineEdit->setText("1");

    //======================

    ui->ef_mas_lbar->setText("0.067");
    ui->ef_mas_well->setText("0.067");
    ui->ef_mas_rbar->setText("0.067");

    ui->qw_left_height->setText("240");
    ui->qw_right_height->setText("240");
    ui->well_width->setText("10");



    //======================

    QObject::connect(ui->get_energy_Button, &QPushButton::clicked,
                     this, &MainWindow::get_energy_slot);


    QObject::connect(ui->clear_all_Button, &QPushButton::clicked,
                     this, &MainWindow::clear_all_slot);


    QObject::connect(ui->add_well_Button, &QPushButton::clicked,
                     this, &MainWindow::add_well_slot);


    QObject::connect(ui->remove_sel_button, &QPushButton::clicked,
                     this, &MainWindow::remove_sel_slot);

    QObject::connect(ui->input_data_TextEdit, &qw_input_TextEdit::block_selected,
                     this, &MainWindow::select_rem_block_slot);



    QObject::connect(this, &MainWindow::reprint_input_data,
                     this, &MainWindow::reprint_input_data_slot);

    QObject::connect(ui->clear_field_Button, &QPushButton::clicked,
                     this, &MainWindow::clear_field_slot);



//    QObject::connect(ui->get_b_fields_Button, &QPushButton::clicked,
//                     this, &MainWindow::get_fields_from_n_slot);-->

    QObject::connect(ui->get_transitions_Button, &QPushButton::clicked,
                     this, &MainWindow::get_transitions_slot);



    QObject::connect(ui->B_fields_Edit, &qw_input_TextEdit::block_selected,
                     this, &MainWindow::selected_mag_field_slot);

    QObject::connect(ui->get_Ws_for_B_Button, &QPushButton::clicked,
                     this, &MainWindow::eveluate_Ws_for_current_field_slot);


    QObject::connect(ui->plotButton, &QPushButton::clicked,
                     this, &MainWindow::plot_wf_slot);

    QObject::connect(ui->plot_averages_Button, &QPushButton::clicked,
                     this, &MainWindow::plot_averages_slot);

    QObject::connect(ui->res_en_wf_PlainEdit, &qw_input_TextEdit::block_selected,
                     this, &MainWindow::energy_selected_slot);

    QObject::connect(ui->plot_b_angle_Button, &QPushButton::clicked,
                     this, &MainWindow::b_theta_plot_slot);

    QObject::connect(ui->get_transitionsButton, &QPushButton::clicked,
                     this, &MainWindow::b_theta_plot_slot);

    //-----------------------------

    QObject::connect(ui->MyPlotButton, &QPushButton::clicked,
                     this, &MainWindow::my_plot_slot);
    //------------------------------


    QObject::connect(ui->monteCarlo_Button, &QPushButton::clicked,
                     this, &MainWindow::monte_slot);

    QObject::connect(ui->OpenGL_Button, &QPushButton::clicked,
                     this, &MainWindow::openGL_slot);

    QObject::connect(ui->cuda_Button, &QPushButton::clicked,
                     this, &MainWindow::cuda_slot);

    QObject::connect(ui->OpenGLnew_Button, &QPushButton::clicked,
                     this, &MainWindow::cudanew_slot);

    //======================================

}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::add_well_slot(){
    this->clear_before_add();

    //ui->input_data_TextEdit->append(QString::number(ui->input_data_TextEdit->document()->blockCount()));
//    ui->input_data_TextEdit->createStandardContextMenu();
//    ui->input_data_TextEdit->append("block block block");
    QTextBlock block = ui->input_data_TextEdit->document()->lastBlock();
    QTextCursor cursor(block);
//    //cursor.visualNavigation();
//    cursor.select(QTextCursor::BlockUnderCursor);
//    ui->input_data_TextEdit->append("from cursor: " + cursor.selectedText());
//    ui->input_data_TextEdit->append("block number: " + QString::number(cursor.blockNumber()));

    this->qw_numb ++;
    QTextBlock tblock;
    QString qw_string;

    if(this->qw_numb == 1){
        // т.е это будет добавлятся только в первый раз.
        this->ef_mas_bar.append(ui->ef_mas_lbar->text().toDouble());
        this->ef_mas_bar_str.append("ef_mas_lbar:           " + ui->ef_mas_lbar->text());
    }

    // после чего эти поля очистятся и больше добавлятся не будут.
    ui->ef_mas_lbar->clear();
    ui->ef_mas_lbar->setEnabled(false);


    this->ef_mas_well.append(ui->ef_mas_well->text().toDouble());
    this->ef_mas_well_str.append("ef_mas_well:           " + ui->ef_mas_well->text());

    this->ef_mas_bar.append(ui->ef_mas_rbar->text().toDouble());
    this->ef_mas_bar_str.append("ef_mas_rbar:           "+ui->ef_mas_rbar->text());


    if(this->qw_numb == 1){
        this->qw_height.append(ui->qw_left_height->text().toDouble());
        qw_height_str.append("qw_left_height:      "+ui->qw_left_height->text());
    }

    ui->qw_left_height->clear();
    ui->qw_left_height->setEnabled(false);

    this->qw_height.append(ui->qw_right_height->text().toDouble());
    qw_height_str.append("qw_right_height:    "+ui->qw_right_height->text());


    if(this->qw_numb != 1){
        this->qw_width.append(ui->bar_width->text().toDouble());
        qw_width_str.append("bar_width:               " + ui->bar_width->text());
    }

    this->qw_width.append(ui->well_width->text().toDouble());
    qw_width_str.append("well_width:              "+ui->well_width->text());


    //ui->input_data_TextEdit->insertPlainText(qw_string);
    //QTextBlockFormat format;
    //format.setBackground(Qt::red);
    //cursor.setBlockFormat(format);
    //cursor.insertBlock(format);
    //block.
    //cursor.insertText(qw_string);
    //ui->input_data_TextEdit->append(qw_string);
    ui->bar_width->setEnabled(true);
    //ui->remove_sel_button->setEnabled(true);
    emit reprint_input_data();

    ui->get_energy_Button->setEnabled(true);

}

void MainWindow::reprint_input_data_slot(){
    qDebug()<< "reprint input data";
    QVector<QString> qw_height_str_tmp = qw_height_str;
    QVector<QString> qw_width_str_tmp = qw_width_str;
    QVector<QString> ef_mas_bar_str_tmp = ef_mas_bar_str;
    QVector<QString> ef_mas_well_str_tmp = ef_mas_well_str;

    ui->input_data_TextEdit->clear();

    QString qw_string = "";
    // надо ж так перепечатать, что бы блоки остались.
    int i = 0;
    while(i<qw_numb){
        // сформировать строку.
        qw_string = "--- qw_numb: " + QString::number(i+1) + " --- <br>";

        if(i == 0){
            qw_string = ef_mas_bar_str_tmp.first() + "<br>";
            ef_mas_bar_str_tmp.pop_front();

        }

        qw_string += ef_mas_well_str_tmp.first() + "<br>";
        ef_mas_well_str_tmp.pop_front();

        qw_string += ef_mas_bar_str_tmp.first() + "<br>";
        ef_mas_bar_str_tmp.pop_front();


        if(i == 0){
           qw_string += qw_height_str_tmp.first() + "<br>";
           qw_height_str_tmp.pop_front();
        }

        qw_string += qw_height_str_tmp.first() + "<br>";
        qw_height_str_tmp.pop_front();


        if(i != 0){
            qw_string += qw_width_str_tmp.first() + "<br>";
            qw_width_str_tmp.pop_front();
        }

        qw_string += qw_width_str_tmp.first() + "<br>";
        qw_width_str_tmp.pop_front();

        // распечать строку.
        ui->input_data_TextEdit->append(qw_string);
        i++;
    }

}

void MainWindow::get_energy_slot(){

    boost::filesystem::remove_all("../DPL_QT/TXT_RES/ENG_WF");
    boost::filesystem::create_directory("../DPL_QT/TXT_RES/ENG_WF");

    ui->res_en_wf_PlainEdit->clear();
    ui->energies_for_fields_Edit->clear();
    this->eng_step = ui->eng_step_LineEdit->text().toDouble();
    this->clear_res();
    this->clear_before_add();
    if(ui->txt_ui_checkBox->isChecked())
        txt_iface();
    get_const_data();
    engu = 1;               // private: in myQwsysterm();
    while(engu < 240){
        //qDebug()<< "====engu====: "<< engu;
        this->make_data();
        this->fill_data();
        // просто берет каждую а и находит det.
        this->find_det();       // заполняет energyes_tmp and dets_tmp
        engu = engu + eng_step;
    }

    // Т.е преобразование в СГС происходит в make_data, и все,
    // а заполнение и сканирование идет по engu.

    // Сканируется по energies_tmp, а
    // energies_tmp.append(engu) вы полняется в find_det().
    // И energies.append(energies_tmp_iter.next()); выполняется
    // в find_roots();
    this->find_roots();  // ищет среди них energies, заполняет.
    this->save_states();  // на основе energies заново make_data(), fill_data()
                         // и заполнить A_stored,
    this->create_res_files();

    this->get_null_space();
    this->get_norm_consts();
    this->get_nc_and_averages();


    // И НАПЕЧАТАТЬ РЕЗУЛЬТАТ!
    //ui->res_en_wf_PlainEdit->clear();
    QVectorIterator<double> energies_iter(this->energies);
    while(energies_iter.hasNext()){
        ui->res_en_wf_PlainEdit->append(QString::number(energies_iter.next()));
        ui->energies_for_fields_Edit->append(QString::number(energies_iter.peekPrevious()));
    }



    ui->M_Field_Tab->setEnabled(true);
    ui->B_MAX_Edit->setEnabled(false);

    ui->B_current_Edit->setEnabled(false);

    ui->get_Ws_for_B_Button->setEnabled(false);
    ui->get_transitionsButton->setEnabled(true);


}

void MainWindow::remove_sel_slot(){
    int selected_block = ui->input_data_TextEdit->block_id;
    qDebug()<< "removing block: " << selected_block;
    ui->input_data_TextEdit->cursor_selectBlockUnder->removeSelectedText();
    this->remove_sel(selected_block);
    emit reprint_input_data();
    ui->remove_sel_button->setEnabled(false);


}

//void MainWindow::select_rem_block_slot(int block_numb){
void MainWindow::select_rem_block_slot(){
    int selected_block = ui->input_data_TextEdit->block_id;
    if(selected_block != 0){
        ui->remove_sel_button->setEnabled(true);
    }else{
        ui->remove_sel_button->setEnabled(false);
    }
    qDebug()<<"from sel_rem_block_slot: " << ui->input_data_TextEdit->block_id;
}



void MainWindow:: clear_all_slot(){

    ui->input_data_TextEdit->clear();
    ui->res_en_wf_PlainEdit->clear();

   // ui->bar_width->setEnabled(false);
    //ui->ef_mas_lbar->setEnabled(true);
    //ui->qw_left_height->setEnabled(true);

    ui->ef_mas_lbar->clear();
    ui->ef_mas_rbar->clear();
    ui->ef_mas_well->clear();
    ui->qw_left_height->clear();
    ui->qw_right_height->clear();
    ui->bar_width->clear();
    ui->well_width->clear();

    //ui->M_Field_Tab->setEnabled(false);
    //ui->get_energy_Button->setEnabled(false);

    ui->ef_mas_lbar->setText("0.067");
    ui->ef_mas_well->setText("0.067");
    ui->ef_mas_rbar->setText("0.067");

    ui->qw_left_height->setText("240");
    ui->qw_right_height->setText("240");
    ui->well_width->setText("10");

    ui->B_MIN_Edit->setText("3");
    ui->B_MAX_Edit->clear();

    ui->nu_i_Edit->setText("2");
    ui->n_i_Edit->setText("0");

    ui->nu_j_Edit->setText("2");
    ui->n_j_Edit->setText("0");

    ui->nu_g_Edit->setText("1");
    ui->nu_f_Edit->setText("1");

    ui->B_current_Edit->clear();
    ui->Theta_from->setText("0");
    ui->Theta_to->setText("45");
    ui->Theta_step_Edit->setText("1");
    //ui->get_Ws_for_B_Button->setEnabled(false);
    ui->energies_for_fields_Edit->clear();

    ui->B_fields_Edit->clear();
    ui->Ws_for_B_field_Edit->clear();

    this->clear_all();
}


void MainWindow::get_transitions_slot(){
    //ui->B_fields_Edit->append("Transitions");
    //this->get_transitions();

    boost::filesystem::remove_all("../DPL_QT/TXT_RES/MAG_FIELD");
    boost::filesystem::create_directory("../DPL_QT/TXT_RES/MAG_FIELD");

    //this->get_energy_slot();

    this->nu_i = ui->nu_i_Edit->text().toInt();
    this->n_i = ui->n_i_Edit->text().toInt();

    this->nu_j = ui->nu_j_Edit->text().toInt();
    this->n_j = ui->n_j_Edit->text().toInt();

    this->nu_g = ui->nu_g_Edit->text().toInt();

    this->nu_f = ui->nu_f_Edit->text().toInt();

    this->b_min = ui->B_MIN_Edit->text().toDouble();
    // B_Max вычисляется на кнопке

    this->theta_from = ui->Theta_from->text().toDouble();
    this->theta_to = ui->Theta_to->text().toDouble();
    this->theta_step = ui->Theta_step_Edit->text().toDouble();

   // this->get_transitions();

    ui->B_fields_Edit->append(this->Ws_eval_str);


}



void MainWindow::get_fields_from_n_slot(){

    this->nu_i = ui->nu_i_Edit->text().toInt();
    this->n_i = ui->n_i_Edit->text().toInt();

    this->nu_j = ui->nu_j_Edit->text().toInt();
    this->n_j = ui->n_j_Edit->text().toInt();

    this->nu_g = ui->nu_g_Edit->text().toInt();

    this->nu_f = ui->nu_f_Edit->text().toInt();

    this->b_min = ui->B_MIN_Edit->text().toDouble();
    // B_Max вычисляется на кнопке

    this->theta_from = ui->Theta_from->text().toDouble();
    this->theta_to = ui->Theta_to->text().toDouble();
    this->theta_step = ui->Theta_step_Edit->text().toDouble();

    //------------------------------------

    this->get_fields_from_n();

    ui->B_MAX_Edit->setText(QString::number(this->B[0]));

    QVectorIterator<double> B_iter(B);
    while(B_iter.hasNext()){
        ui->B_fields_Edit->append(QString::number(B_iter.next()));
    }


    //-------------------------------------

    ui->B_current_Edit->setEnabled(true);
    ui->Theta_from->setEnabled(true);
    ui->Theta_to->setEnabled(true);
    ui->Theta_step_Edit->setEnabled(true);
    //ui->get_Ws_for_B_Button->setEnabled(true);

    ui->Theta_from->setText("0");
    ui->Theta_to->setText("45");
    ui->Theta_step_Edit->setText("1");


    qDebug()<<"B Block_Id: "<< ui->B_fields_Edit->block_id;
}


void MainWindow::clear_field_slot(){

    ui->B_current_Edit->setEnabled(false);

    ui->Theta_from->setText("0.0001");
    ui->Theta_to->setText("45");
    ui->Theta_step_Edit->setText("1");

    ui->get_Ws_for_B_Button->setEnabled(false);
    ui->B_fields_Edit->clear();
    ui->Ws_for_B_field_Edit->clear();
    qDebug()<<"cloar_field_slot.";

    ui->B_MIN_Edit->setText("3");
    ui->B_MAX_Edit->clear();

    ui->nu_i_Edit->setText("2");
    ui->n_i_Edit->setText("0");

    ui->nu_j_Edit->setText("2");
    ui->n_j_Edit->setText("0");

    ui->nu_g_Edit->setText("1");
    ui->nu_f_Edit->setText("1");

    ui->B_current_Edit->clear();

    this->Ws_eval_str.clear();
    this->Transitions_string.clear();


    this->clear_field();        // лежит в mag_field.cpp

}

void MainWindow::selected_mag_field_slot(){

    qDebug()<<"B field ID: " << ui->B_fields_Edit->block_id;

    this->n_f_max = ui->B_fields_Edit->block_id + 1;
    QTextBlock block;
    block = ui->B_fields_Edit->document()->findBlockByNumber(ui->B_fields_Edit->block_id);
    QTextCursor cursor(block);
    cursor.select(QTextCursor::BlockUnderCursor);
    ui->B_current_Edit->setText(cursor.selectedText());
    ui->get_Ws_for_B_Button->setEnabled(true);

}


void MainWindow::energy_selected_slot(){
    qDebug()<<"en_ID: " << ui->res_en_wf_PlainEdit->block_id;
}


void MainWindow::eveluate_Ws_for_current_field_slot(){
    this->eval_Ws_for_current_B_field();
    ui->Ws_for_B_field_Edit->append(this->Ws_eval_str);

}


void MainWindow::plot_wf_slot(){
    int blockID = ui->res_en_wf_PlainEdit->block_id;
    if(wf_graph){
        qDebug()<< "add graph";
        wf_graph->set_engValue_toPlot(get_engValue_toPlot(blockID));
        //wf_graph->set_x_range_toPlot(get_x_range_toPlot());
        //wf_graph->set_xstep(this->x_step);
        //---------
        wf_graph->set_xy_toPlot(&this->wf_params_array[blockID], averages[blockID], std_deviations_pow2[blockID]);
    }else{
        wf_graph = new wf_plot(blockID);
        //--------
        wf_graph->set_engValue_toPlot(get_engValue_toPlot(blockID));
        wf_graph->set_x_range_toPlot(get_x_range_toPlot());
        wf_graph->set_xstep(this->x_step);
        //---------
        wf_graph->set_xy_toPlot(&this->wf_params_array[blockID], averages[blockID], std_deviations_pow2[blockID]);
        //---------
        wf_graph->show();
    }


}


void MainWindow::plot_averages_slot(){
    boost::filesystem::remove_all("../DPL_QT/TXT_RES/WIDTH_WF");
    boost::filesystem::create_directory("../DPL_QT/TXT_RES/WIDTH_WF");
    clear_all_slot();
    plot_arevages();
}

void MainWindow::txt_iface(){

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

    qw_width.append(11);         // задаем в цикле
    qw_width_str.append("bar_width");
    //-----------------------------

}

void MainWindow::b_theta_plot_slot(){
    //this->txt_iface();
    //get_transitions_slot();
    QVector<double> trans_info;
//    trans_info.append(1); trans_info.append(1);
//    trans_info.append(0); trans_info.append(0);
//    trans_info.append(0); trans_info.append(0);
//    trans_info.append(0); trans_info.append(1);


    B_Theta_form *b_theta_form = new B_Theta_form(0,energies_erg, std_deviations_pow2_cm,
                                                  trans_info, this->ef_mas_well);
    b_theta_form->show();

}


void MainWindow::my_plot_slot(){
    qDebug()<<"My_Plot";

    Form_MyPlot *myplot = new Form_MyPlot;
    myplot->show();

}


void MainWindow::monte_slot(){
//    // gnuplot > splot "test_int.txt"
//    // gnuplot > replot x*x + x*y
//    // получается два графика в одном окне.
//    qDebug()<<"do monte";

//    float x_min = 1;
//    float x_max = 3;
//    float x_loc;

//    float y_min = 1;
//    float y_max = 27;
//    float y_loc;

//    float z_min = 0;
//    float z_max = 90;
//    float z_loc;

//    std::ofstream out;
//    out.open("./test_int.txt");
//    x_loc = x_min;
//    while(x_loc <= x_max){

//        y_loc = y_min;
//        while(y_loc <= y_max){
//            if((y_loc > x_loc) && (y_loc < pow(x_loc,3))){
//                z_loc = z_min;
//                while(z_loc <= z_max){
//                    if((z_loc > 0) && (z_loc < (pow(x_loc,2)+ x_loc * y_loc))){
//                        out<< x_loc <<" " << y_loc << " " << z_loc << " " << std::endl;
//                    }
//                    z_loc += 5;
//                }
//            }
//            y_loc += 1;
//        }


//        x_loc += 0.01;
//    }

    f_monte_cudev();
    //qDebug()<< test_lib_func(2,2);
}

void MainWindow::openGL_slot(){
    OpenGL_Form *ogl_form = new OpenGL_Form();
    ogl_form->show();
    qDebug()<<"openGL slot";
}

void MainWindow::cuda_slot(){
    Cuda_Form *cuda_form = new Cuda_Form();
    cuda_form->show();
    qDebug()<<"cuda_form";

}

void MainWindow::cudanew_slot(){
    Cuda_Oglnew_Form *cudanew_form = new Cuda_Oglnew_Form();
    cudanew_form->show();
}
