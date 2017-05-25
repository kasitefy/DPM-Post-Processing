#include "cplot.h"
#include "ui_cplot.h"
#include <QVector>

cplot::cplot(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::cplot) {
    ui->setupUi(this);
    ui->widget->addGraph();
    ui->widget->xAxis->setLabel("y /m");
    ui->widget->yAxis->setLabel("Number");
}

cplot::~cplot() {
    delete ui;
}

void cplot::add(std::vector<double> &x,std::vector<double> &y ) {
    QVector<double> c,ans;
    for(int i=0; i<y.size(); ++i) {
        c.push_back(x[i]);
        ans.push_back(y[i]);
    }
    ui->widget->graph(0)->setData(c,ans);
    ui->widget->graph(0)->rescaleAxes();
    ui->widget->setInteractions(QCP::iRangeDrag|QCP::iRangeZoom);
    return ;
}
