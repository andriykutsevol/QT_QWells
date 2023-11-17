#ifndef FORM_MYPLOT_H
#define FORM_MYPLOT_H

#include <QWidget>

namespace Ui {
class Form_MyPlot;
}

class Form_MyPlot : public QWidget
{
    Q_OBJECT

public:
    explicit Form_MyPlot(QWidget *parent = 0);
    ~Form_MyPlot();

private:
    Ui::Form_MyPlot *ui;
};

#endif // FORM_MYPLOT_H
