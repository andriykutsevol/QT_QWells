#ifndef AUTO_PLOT_H
#define AUTO_PLOT_H

#include <QWidget>
#include <QString>
#include <QVector>

namespace Ui {
class Auto_Plot;
}

class Auto_Plot : public QWidget
{
    Q_OBJECT

public:
    explicit Auto_Plot(QWidget *parent = 0);

    Auto_Plot(QVector<double> *diam_1 =  NULL,
              QVector<double> *diam_2 =  NULL,
              QVector<double> *energy1 = NULL,
              QVector<double> *energy2 = NULL,
              QVector<double> *b_par_x = NULL,
              double low_y = 0,
              double top_y = 17);

    void set_b_perp_label(QString text);
    ~Auto_Plot();

private:
    Ui::Auto_Plot *ui;

    QVector<double>* diam_1;
    QVector<double>* diam_2;
    QVector<double>* energy1;
    QVector<double>* energy2;
    QVector<double>* b_par_x;
    double low_y, top_y;

};

#endif // AUTO_PLOT_H
