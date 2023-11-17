#include "auto_plot.h"
#include "ui_auto_plot.h"

#include <QMouseEvent>

Auto_Plot::Auto_Plot(QVector<double> *diam_1,
                     QVector<double> *diam_2,
                     QVector<double> *energy1,
                     QVector<double> *energy2,
                     QVector<double> *b_par_x,
                     double low_y,
                     double top_y):
    ui(new Ui::Auto_Plot),
    diam_1(diam_1),
    diam_2(diam_2),
    energy1(energy1),
    energy2(energy2),
    b_par_x(b_par_x),
    low_y(low_y),
    top_y(top_y)
{
    ui->setupUi(this);

    ui->auto_plotCust->addGraph();
    ui->auto_plotCust->graph(0)->setData(*b_par_x, *diam_1);
    ui->auto_plotCust->graph(0)->setPen(QPen(Qt::red));


    ui->auto_plotCust->addGraph();
    ui->auto_plotCust->graph(1)->setData(*b_par_x, *diam_2);
    ui->auto_plotCust->graph(1)->setPen(QPen(Qt::blue));


    ui->auto_plotCust->addGraph();
    ui->auto_plotCust->graph(2)->setData(*b_par_x, *energy1);
    ui->auto_plotCust->graph(2)->setPen(QPen(Qt::black));


    ui->auto_plotCust->addGraph();
    ui->auto_plotCust->graph(3)->setData(*b_par_x, *energy2);
    ui->auto_plotCust->graph(3)->setPen(QPen(Qt::black));

    ui->auto_plotCust->xAxis->setRange(0,b_par_x->last());
    ui->auto_plotCust->yAxis->setRange(low_y, 60);

}

Auto_Plot::~Auto_Plot()
{
    delete ui;
}

void Auto_Plot::set_b_perp_label(QString text){
    ui->b_perp_label->setText(text);
}
