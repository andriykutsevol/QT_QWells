#include "wf_plot.h"
#include "ui_wf_plot.h"
#include "myqwsystem.h"
#include "units_trans.h"

#include <math.h>
#include <gsl/gsl_integration.h>
#include <QDebug>

#include <QString>
#include <QVector>
#include <QVectorIterator>

wf_plot::wf_plot(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::wf_plot)
{
    ui->setupUi(this);

}

wf_plot::~wf_plot()
{
    delete ui;
}
//====================================


wf_plot::wf_plot(int blockID) :
    ui(new Ui::wf_plot)
{
    ui->setupUi(this);
    ui->wf_graphQCustom->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectAxes |
                                         QCP::iSelectLegend | QCP::iSelectPlottables);

    QObject::connect(ui->wf_graphQCustom, &QCustomPlot::selectionChangedByUser,
                     this, &wf_plot::selectionChanged_slot);


    // график для ямы
    blockID_vec.append(blockID);

    low_limit_y = 0; top_limit_y = 0;
    graph_n = 0;
    graph_count = 0; graph_well_count = 0;
    qDebug()<<"from plot enID"<< energy;
}


//=====================================



void wf_plot::set_engValue_toPlot(double energy){
    qDebug()<<"set_engValue_toPlot"<< energy;
    ui->selEnergyLineEdit->setText(QString::number(energy));
    this->energy_vec.append(energy);

}


void wf_plot::set_x_range_toPlot(QVector<double> xy_range){
    this->low_limit = xy_range[0];
    this->top_limit = xy_range[1];
    qDebug()<<"xy: " << xy_range[0];
    qDebug()<<"xy: " << xy_range[1];
}




void wf_plot::set_xstep(double x_step){
    this->x_step = x_step;
}



void wf_plot::set_xy_toPlot(wf_phi_for_gsl_params* wf_params, double average, double std_deviat_pow2){
    // Данные сюда упаковываются в get_wave_func.cpp, в get_nc_and_averages();
    double j = this->low_limit;
    x.clear();
    y.clear();
    while(j< this->top_limit){
        this->x.append(j);
        dataY = wf_phi(j, wf_params);
        if(low_limit_y > dataY)
            low_limit_y = dataY;
        if(top_limit_y < dataY)
            top_limit_y = dataY;
        this->y.append(dataY);
        j = j + this->x_step;
    }


// plot well ==============================================
        QVectorIterator<double> Qx_array_iter(wf_params->Qx_array);
        double k;
        if(graph_n == 0){
            while(Qx_array_iter.hasNext()){
                k = low_limit_y;
                well_x.clear();
                well_y.clear();
                while(k < top_limit_y){
                    well_x.append(Qx_array_iter.peekNext());
                    well_y.append(k);
                    k = k + (top_limit_y - low_limit_y)/100;
                }
                this->ui->wf_graphQCustom->addGraph();
                this->ui->wf_graphQCustom->graph(graph_n)->setPen(QPen(Qt::black));
                this->ui->wf_graphQCustom->graph(graph_n)->setData(well_x, well_y);
                graph_n ++;
                graph_well_count ++;
                Qx_array_iter.next();
            }
        }else{
            graph_n =  0;
            while(Qx_array_iter.hasNext()){
                k = low_limit_y;
                well_x.clear();
                well_y.clear();
                while(k < top_limit_y){
                    well_x.append(Qx_array_iter.peekNext());
                    well_y.append(k);
                    k = k + (top_limit_y - low_limit_y)/100;
                }
                //this->ui->wf_graphQCustom->addGraph();
                this->ui->wf_graphQCustom->graph(graph_n)->setData(well_x, well_y);
                graph_n ++;
                Qx_array_iter.next();
            }
        }

//===========================================================
    graph_n += graph_count;
    graph_count ++;

    //------------------------------------
    this->ui->wf_graphQCustom->addGraph();
    this->ui->wf_graphQCustom->graph(graph_n)->setData(this->x, this->y);
    this->ui->wf_graphQCustom->xAxis->setRange(this->low_limit, this->top_limit);
    graph_n ++;

    //-------------------------------------

    QVectorIterator<double> k1_iter(wf_params->k1);
    k1_str = "";
    while(k1_iter.hasNext()){
        k1_str += QString::number(k1_iter.next()) + "<br>";
    }
    k1_str_vec.append(k1_str);

    QVectorIterator<double>k2_iter(wf_params->k2);
    k2_str = "";
    while(k2_iter.hasNext()){
        k2_str += QString::number(k2_iter.next()) + "<br>";
    }
    k2_str_vec.append(k2_str);

    average_vec.append(average);
    std_deviat_pow2_vec.append(std_deviat_pow2);

    ui->K_wellPlotText->clear();
    ui->K_wellPlotText->append(k1_str_vec.last());
    ui->K_barPlotText->clear();
    ui->K_barPlotText->append(k2_str_vec.last());
    ui->average_lineEdit->setText(QString::number(average_vec.last()));
    ui->deviat_lineEdit->setText(QString::number(std_deviat_pow2_vec.last()));

    //--------------------------------------

    this->ui->wf_graphQCustom->yAxis->setRange(low_limit_y, top_limit_y);
    this->ui->wf_graphQCustom->replot();

}

void wf_plot::selectionChanged_slot(){
    qDebug()<<"selection changed;";
    int i = graph_well_count;
    while(i < (graph_n)){
        QCPGraph *graph_tmp = ui->wf_graphQCustom->graph(i);
        if(graph_tmp->selected()){
            qDebug()<<"graph: "<< (i - graph_well_count) << " selected";
            ui->K_wellPlotText->clear();
            ui->K_wellPlotText->append(k1_str_vec[i - graph_well_count]);
            ui->K_barPlotText->clear();
            ui->K_barPlotText->append(k2_str_vec[i - graph_well_count]);
            ui->selEnergyLineEdit->setText(QString::number(energy_vec[i - graph_well_count]));
            ui->average_lineEdit->setText(QString::number(average_vec[i - graph_well_count]));
            ui->deviat_lineEdit->setText(QString::number(std_deviat_pow2_vec[i - graph_well_count]));
        }
        i++;
    }
}
