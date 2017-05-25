#ifndef CPLOT_H
#define CPLOT_H

#include <QDialog>
#include <QVector>
#include <vector>
#include "qcustomplot.h"

namespace Ui {
class cplot;
}

class cplot : public QDialog
{
    Q_OBJECT

public:
    explicit cplot(QWidget *parent = 0);
    ~cplot();
    void add(std::vector<double> &x,std::vector<double> &y);


private:
    Ui::cplot *ui;
};

#endif // CPLOT_H
