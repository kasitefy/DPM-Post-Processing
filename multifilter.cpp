#include "multifilter.h"
#include "ui_multifilter.h"

MultiFilter::MultiFilter(QStringList &Label, std::vector<double> &Vmin,std::vector<double> &Vmax, std::map<QString, range> &MapRange, QWidget *parent) :
    label(Label),
    vmin(Vmin),
    vmax(Vmax),
    mp(MapRange),
    QDialog(parent),
    ui(new Ui::MultiFilter) {
    ui->setupUi(this);
    colTitle<<"Variable"<<"Min"<<"Max";
    //setAttribute(Qt::WA_DeleteOnClose);
}

MultiFilter::~MultiFilter() {
    delete ui;
}

void MultiFilter::on_cancel_pushButton_clicked() {
    MultiFilter::reject();
}

void MultiFilter::initTable() {
    int cn=label.size();
    ui->tableWidget->clear();
    ui->tableWidget->setHorizontalHeaderLabels(colTitle);
    ui->tableWidget->setRowCount(cn);
    for(int i=0; i<cn; ++i) {
        ui->tableWidget->setItem(i,0,new QTableWidgetItem(label[i]));
        ui->tableWidget->setItem(i,1,new QTableWidgetItem(QString::number(vmin[i])));
        ui->tableWidget->setItem(i,2,new QTableWidgetItem(QString::number(vmax[i])));
    }
}

void MultiFilter::on_apply_pushButton_clicked() {
    int cn=ui->tableWidget->rowCount();
    for(int i=0; i<cn; ++i) {
        QString q=label[i];
        std::map<QString,range>::iterator it=mp.find(q);
        if(it!=mp.end()) {
            double _min=ui->tableWidget->item(i,1)->text().toDouble();
            double _max=ui->tableWidget->item(i,2)->text().toDouble();
            it->second.update(_min,_max);
        }
    }
}

void MultiFilter::on_ok_pushButton_clicked() {
    on_apply_pushButton_clicked();
    MultiFilter::accept();
}
