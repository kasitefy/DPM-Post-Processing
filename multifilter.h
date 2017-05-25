#ifndef MULTIFILTER_H
#define MULTIFILTER_H

#include <QDialog>
#include "range.h"
#include <map>
#include <QStringList>
#include <vector>

namespace Ui {
class MultiFilter;
}



class MultiFilter : public QDialog {
    Q_OBJECT

  public:
    explicit MultiFilter(QStringList &Label,std::vector<double> &Vmin,std::vector<double> &Vmax,std::map<QString,range>& MapRange,QWidget *parent = 0);
    ~MultiFilter();

    void initTable();

  private slots:
    void on_cancel_pushButton_clicked();

    void on_apply_pushButton_clicked();

    void on_ok_pushButton_clicked();

  private:
    Ui::MultiFilter *ui;
    QStringList& label;//store variable name
    std::map<QString,range> &mp;//reference to map
    std::vector<double> &vmin,&vmax;//reference to vmin and vmax
    QStringList colTitle;//
};

#endif // MULTIFILTER_H
