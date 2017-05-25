#ifndef EXPORTDATA_H
#define EXPORTDATA_H

#include <QWidget>
#include <QDialog>
#include <QString>
#include <QStringList>
#include <vector>
#include "point.h"
#include <map>
#include "range.h"

namespace Ui {
class ExportData;
}

class ExportData : public QDialog {
    Q_OBJECT

  public:
    explicit ExportData(std::vector<point>& Data, QStringList& frontInfor,
                        QStringList& Label,QString& SavePath,std::vector<double> &Min,
                        std::vector<double> &Max,std::map<QString,range> &MapRange, QWidget *parent = 0);
    ~ExportData();

    void setEnableMultiFilter(bool st);

  private slots:
    void on_comboBox_currentIndexChanged(const QString &arg1);

    void on_valuelineEdit_editingFinished();

    void on_epslineEdit_editingFinished();

    void on_pushButton_clicked();

    void on_mexport_pushButton_clicked();

  private:
    Ui::ExportData *ui;
    QStringList& qsl;//reference to front information
    QStringList& label;//reference to label_combobox
    std::vector<point>& data;//reference to original data
    int kt=0;
    double min=0,max=0;
    std::vector<double>&vmin,&vmax;
    QString& savePath;//reference to original save path
    std::map<QString,range> &mp;
    bool enableMF;
};

#endif // EXPORTDATA_H
