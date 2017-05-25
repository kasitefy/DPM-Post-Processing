#include "exportdata.h"
#include "ui_exportdata.h"
#include <QMessageBox>
#include <QDir>
#include <QFile>
#include <QTextStream>
#include <QFileDialog>
#include <algorithm>


ExportData::ExportData(std::vector<point> &Data, QStringList &frontInfo, QStringList &Label, QString& SavePath, std::vector<double> &min, std::vector<double> &max, std::map<QString, range> &MapRange, QWidget *parent) :
    data(Data),
    qsl(frontInfo),
    label(Label),
    savePath(SavePath),
    vmin(min),
    vmax(max),
    mp(MapRange),
    QDialog(parent),
    ui(new Ui::ExportData) {
    ui->setupUi(this);
    ui->comboBox->addItems(Label);
    on_comboBox_currentIndexChanged(ui->comboBox->currentText());
    enableMF=false;
}

ExportData::~ExportData() {
    delete ui;
}

void ExportData::on_comboBox_currentIndexChanged(const QString &arg1) {
    Q_UNUSED(arg1)
    kt=ui->comboBox->currentIndex();
    size_t tot=data.size();
    if(tot) {
        ui->max_label->setText(QString::number(vmax[kt]));
        ui->min_label->setText(QString::number(vmin[kt]));
    }
}

void ExportData::on_valuelineEdit_editingFinished() {
    if(ui->valuelineEdit->text().isEmpty()) {
        QMessageBox::information(this,"Warning","value is empty!");
    }
}

void ExportData::on_epslineEdit_editingFinished() {
    if(ui->epslineEdit->text().isEmpty()) {
        QMessageBox::information(this,"Warning","eps is empty!");
    }
}

void ExportData::on_pushButton_clicked() {
    on_valuelineEdit_editingFinished();
    on_epslineEdit_editingFinished();
    double value=ui->valuelineEdit->text().toDouble();
    double eps=ui->epslineEdit->text().toDouble();
    if(value>max||value<min) {
        QMessageBox::information(this,"Warning","Out of Range!");
        return;
    }
    if(data.empty()) {
        QMessageBox::information(this,"Warning","No Data");
        return;
    }
    QString temp=savePath;
    if(savePath.isEmpty())
        savePath=QFileDialog::getSaveFileName(this,tr("Save File"),QDir::currentPath()+tr("/file.his"),"Fluent DPM CurrentPosition File(*.his)");
    else
        savePath=QFileDialog::getSaveFileName(this,tr("Save File"),savePath+tr("/file.his"),"Fluent DPM CurrentPosition File(*.his)");
    if(savePath.isEmpty()) {
        savePath=temp;
        return;
    }
    QFile *file=new QFile;
    file->setFileName(savePath);
    bool ok = file->open(QIODevice::WriteOnly|QIODevice::Text);
    if(ok) {
        QTextStream out(file);
        QStringList::iterator it=qsl.begin();
        while(it!=qsl.end()) {
            out<<*it<<"\n";
            ++it;
        }
        std::vector<point>::iterator i=data.begin();
        size_t cn=data[0].a.size(),sum=0;
        while(i!=data.end()) {
            bool jd;
            if(enableMF)
                jd=range::inrange(*i,mp,label);
            else jd=true;
            if(jd&&fabs((*i).a[kt]-value)<eps) {
                out<<(*i).a[0];
                for(int j=1; j<cn; ++j) {
                    out<<" "<<(*i).a[j];
                }
                out<<"\n";
                ++sum;
            }
            ++i;
        }
        QMessageBox::information(this,"Successful","Export "+QString::number(sum)+" Particles.");
    }
    file->close();
    delete file;
    return;
}

void ExportData::on_mexport_pushButton_clicked() {
    on_valuelineEdit_editingFinished();
    on_epslineEdit_editingFinished();
    double value=ui->valuelineEdit->text().toDouble();
    double eps=ui->epslineEdit->text().toDouble();
    if(value>max||value<min) {
        QMessageBox::information(this,"Warning","Out of Range!");
        return;
    }
    if(data.empty()) {
        QMessageBox::information(this,"Warning","No Data");
        return;
    }
    QString temp=savePath;
    if(savePath.isEmpty())
        savePath=QFileDialog::getSaveFileName(this,tr("Save File"),QDir::currentPath()+tr("/file.m"),"Matlab Script(*.m)");
    else
        savePath=QFileDialog::getSaveFileName(this,tr("Save File"),savePath+tr("/file.m"),"Matlab Script(*.m)");
    if(savePath.isEmpty()) {
        savePath=temp;
        return;
    }
    QFile *file=new QFile;
    file->setFileName(savePath);
    bool ok = file->open(QIODevice::WriteOnly|QIODevice::Text);
    std::vector<point> tempv;
    if(ok) {
        QTextStream out(file);
        std::vector<point>::iterator i=data.begin();
        int sum=0;
        while(i!=data.end()) {
            bool jd;
            if(enableMF)
                jd=range::inrange(*i,mp,label);
            else jd=true;
            if(jd&&fabs((*i).a[kt]-value)<eps) {
                tempv.push_back(*i);
                ++sum;
            }
            ++i;
        }
        //std::sort(tempv.begin(),tempv.end());
        out<<"clear;\nclc;\n"
           <<"x=[";
        for(int j=0; j<sum; ++j) {
            out<<" "<<tempv[j].a[1];
        }
        out<<"];\nz=[";
        for(int j=0; j<sum; ++j) {
            out<<" "<<tempv[j].a[3];
        }
        out<<"];\nvy=[";
        for(int j=0; j<sum; ++j) {
            out<<" "<<tempv[j].a[5];
        }
        out<<"];\n";
        out<<"theta=[";
        for(int j=0; j<sum; ++j) {
            out<<" "<<atan(tempv[j].a[5]/tempv[j].a[4])/pi*180;
        }
        out<<"];\n"
           <<"t=0:pi/100:2*pi;\nx1=2*sin(t);\ny1=2*cos(t);"
           <<"plot(x1,y1,'black');\nhold on;\n"
           <<"scatter(x,z,10,theta,'filled');\n axis equal;"
           <<"scatter(x,z,10,vy,'filled');\nfigure;\n"
           <<"[X,Y,Z]=griddata(x,z,vy,linspace(min(x),max(x))',linspace(min(z),max(z)),'v4');%interpolation\n"
           <<"% pcolor(X,Y,Z);shading interp\n"
           << "% figure,contourf(X,Y,Z);\n"
           <<"figure,meshc(X,Y,Z);\n";
        QMessageBox::information(this,"Successful","Export "+QString::number(sum)+" Particles.");
    }
    file->close();
    delete file;
    return;
}

void ExportData::setEnableMultiFilter(bool st) {
    enableMF=st;
}
