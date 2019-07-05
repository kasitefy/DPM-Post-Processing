#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <vector>
#include <QStringList>
#include <QDialog>
#include "exportdata.h"
#include "point.h"
#include "cplot.h"
#include "multifilter.h"
#include "range.h"
#include <map>

namespace Ui
{
class MainWindow;

}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    friend class cplot;
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    //calculate button
    void on_pushButton_clicked();

    //open file
    void on_actionOpen_triggered(bool checked);

    //assign min value
    void on_minLineEdit_textChanged(const QString &arg1);

    //assign max value
    void on_maxLineEdit_textChanged(const QString &arg1);

    //assign div value
    void on_divLineEdit_textChanged(const QString &arg1);

    //save button
    void on_actionSave_triggered();

    //when filter variable changed, update min and max
    void on_comboBox_currentIndexChanged(int index);

    //when calculate method changed, change the row count of table
    void on_face_checkBox_clicked();

    //show result with percentage
    void on_percent_checkBox_clicked();

    //show help
    void on_actionHelp_triggered();

    //show information about Qt
    void on_actionAbout_Qt_triggered();

    //assign eps
    void on_eps_lineEdit_editingFinished();

    //show plot window
    void on_actiondraw_triggered();

    //exit
    void on_actionExit_triggered();

    //show export data window
    void on_actionExport_triggered();

    //enable multifilter
    void on_mf_checkBox_clicked(bool checked);

    //show multifilter window
    void on_actionMultifillter_triggered();

    void on_aim_comboBox_currentIndexChanged(const QString &arg1);

    void on_divLineEdit_editingFinished();

private:
    Ui::MainWindow *ui;//main window

    std::vector<point> v;//origin data

    cplot *d;//plot window

    ExportData *uie;//export window

    MultiFilter *uimf;//MultiFilter window

    std::vector<double> c;//divide point

    std::vector<double> ans, count; //result

    QString saveFileName;//save path

    int div = 10, tot = 0; //number of divide part and number of particle

    std::vector<double> vmax, vmin; //range of variable, calculated in the open file step

    double max, min; //user seted range

    QStringList label;//table title

    QStringList label_combobox;//combobox title, calculated in the open file step

    QStringList label_aim;//aim_combobox titles

    QStringList fore;//discribe informaton

    int kt = 0; //current cumbo box index

    int cn = 12; //number of variables

    std::map<QString, range> mp; //store multifilter range

};

#endif // MAINWINDOW_H
