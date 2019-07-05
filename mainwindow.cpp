#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QObject>
#include <QDir>
#include <QMessageBox>
#include <QFile>
#include <QFileDialog>
#include <QTextStream>
#include <cmath>
#include <QDebug>
#include "judge.h"
#include <algorithm>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    label << "From" << "To" << "Number";
    initjudge();
    ui->tableWidget->setHorizontalHeaderLabels(label);
    //ui->aim_comboBox->setCurrentIndex(label_combobox.size());
    uimf = new MultiFilter(label_combobox, vmin, vmax, mp, this);
    uimf->setWindowTitle("MultiFilter");
    ui->divLineEdit->setValidator(new QIntValidator());
    ui->minLineEdit->setValidator(new QDoubleValidator());
    ui->maxLineEdit->setValidator(new QDoubleValidator());
    ui->eps_lineEdit->setValidator(new QDoubleValidator());
}


MainWindow::~MainWindow()
{
    delete ui;
    delete uimf;
    delete uie;
    delete d;
}

void MainWindow::on_pushButton_clicked()
{
    if(v.empty())
    {
        QMessageBox::information(this, "Error", "No Data");
        return;
    }

    on_divLineEdit_textChanged("update");

    c.resize(div + 1);

    if(ui->percent_checkBox->isChecked())
    {
        ans.resize(div + 1);
        count.resize(div + 1);
    }
    else
    {
        ans.resize(div);
        count.resize(div);
    }

    for (int k = 0; k <= div; ++k)
        c[k] = min + k * (max - min) / div;

    for(int i = 0; i <= div; ++i)
        count[i] = ans[i] = 0;

    //find ParticleMass and ParticleNumberinParcel
    int pm = ui->comboBox->findText("ParticleMass"), pnp = ui->comboBox->findText("ParticleNumberinParcel");
    int aim = ui->aim_comboBox->currentIndex();
    int type;

    if(aim < label_combobox.size())type = 1;//original series
    else if(aim == label_combobox.size())type = 2;//number
    else type = 3;//Total Mass

    if(type == 3 && (pm == -1 || pnp == -1))
    {
        QMessageBox::information(this, "Error", "ParticleMass or ParticleNumberinParcel not exist");
        return;
    }

    // update count and ans
    if(ui->face_checkBox->checkState())
    {
        double eps = ui->eps_lineEdit->text().toDouble();

        for(int j = 0; j < tot; ++j)
        {
            bool jd;

            if(ui->mf_checkBox->isChecked())
                jd = range::inrange(v[j], mp, label_combobox);
            else jd = true;

            if(jd)
                for(int e = 0; e <= div; ++e)
                {
                    if (fabs(v[j].a[kt] - c[e]) < eps)
                    {
                        ++count[e];

                        if(type == 1)
                            ans[e] += v[j].a[aim];
                        else if(type == 3)
                            ans[e] += v[j].a[pm] * v[j].a[pnp];
                        else ++ans[e];

                        break;
                    }
                }
        }

        /*
                if(type == 3 && kt == ui->comboBox->findText("ParticleYPosition"))
                    for(int e = 0; e <= div; ++e)
                        ans[e] /= getArea(c[e]) * 2 * eps;
        */
    }
    else
    {
        for (int j = 0; j < tot; ++j)
        {
            bool jd;

            if(ui->mf_checkBox->isChecked())
                jd = range::inrange(v[j], mp, label_combobox);
            else jd = true;

            if(jd)
                for (int e = 0; e < div; ++e)
                {
                    if (v[j].a[kt] >= c[e] && v[j].a[kt] < c[e + 1])
                    {
                        ++count[e];

                        if(type == 1)
                            ans[e] += v[j].a[aim];
                        else if(type == 3)
                            ans[e] += v[j].a[pm] * v[j].a[pnp];
                        else ++ans[e];

                        break;
                    }
                }
        }
    }

    if(ui->ave_checkBox->isChecked())
        for(int e = 0; e <= div; ++e)
            if(count[e] != 0)
                ans[e] /= count[e];

    int ssize = ui->face_checkBox->isChecked() ? div + 1 : div;

    for(int e = 0; e < ssize; ++e)
    {
        ui->tableWidget->setItem(e, 0, new QTableWidgetItem(QString::number(c[e])));
        ui->tableWidget->setItem(e, 1, new QTableWidgetItem(QString::number(c[e + 1])));
    }

    if(ui->percent_checkBox->checkState())
    {
        for (int e = 0; e < ssize; ++e)
        {
            ui->tableWidget->setItem(e, 2, new QTableWidgetItem(QString::number((double)count[e] / tot)));
        }
    }
    else
    {
        for (int e = 0; e < ssize; ++e)
        {
            ui->tableWidget->setItem(e, 2, new QTableWidgetItem(QString::number(ans[e])));
        }
    }
}

void MainWindow::on_actionOpen_triggered(bool checked)
{
    Q_UNUSED(checked)

    QString temp = saveFileName;

    if(saveFileName.isEmpty())
        saveFileName = QFileDialog::getOpenFileName(this, "Open File", QDir::currentPath(), tr("Fluent DPM CurrentPosition File(*.his)"));
    else
        saveFileName = QFileDialog::getOpenFileName(this, "Open File", saveFileName, tr("Fluent DPM CurrentPosition File(*.his)"));

    if(saveFileName.isEmpty())
    {
        //QMessageBox::information(this,"Error Message","Please Select a his file");
        saveFileName = temp;
        return;
    }

    QFile *file = new QFile;
    file->setFileName(saveFileName);
    bool ok = file->open(QIODevice::ReadOnly);

    if(ok)
    {
        QTextStream in(file);
        fore.clear();
        mp.clear();
        /********************read describe information****************************/
        fore << in.readLine(); //line 1 TYPE 1
        QString s = in.readLine(); //line 2 COLUMNS: 13
        fore << s;
        cn = s.mid(9).toInt();

        for(int ii = 0; ii < 4; ++ii) //skip 4 lines
            fore << in.readLine();

        label_combobox.clear();

        for(int ii = 0; ii < cn; ++ii)   //read cn lines to get variable name
        {
            s = in.readLine();
            fore << s;
            label_combobox.push_back(s.split(QRegExp(" +"))[3]);//split QString with 1 or more blank
        }

        label_aim = label_combobox;
        label_aim.push_back("Number");
        label_aim.push_back("TotalMass");

//        qDebug() << label_aim.size() << "  " << label_combobox.size() << "\n";

        for(int ii = 0; ii < 2; ++ii) //skip 2 lines
            fore << in.readLine();

        /************************************************************************/
        tot = 0;
        point temp(cn);
        v.clear();
        kt = 0;
        vmin.resize(cn);
        vmax.resize(cn);
        std::fill(vmin.begin(), vmin.end(), (double)INT_MAX);
        std::fill(vmax.begin(), vmax.end(), (double)INT_MIN);

        while (!in.atEnd())
        {
            for (int t = 0; t < cn; ++t)
            {
                in >> temp.a[t];
            }

            in.readLine();//skip \n

            if(ui->checkBox->checkState() && !isok(temp))continue;

            //update range
            for (int t = 0; t < cn; ++t)
            {
                if (temp.a[t] < vmin[t])vmin[t] = temp.a[t];

                if (temp.a[t] > vmax[t])vmax[t] = temp.a[t];
            }

            v.push_back(temp);
            tot++;
        }

        file->close();
        delete file;
        /*----------------generate map------------------- */
        mp.clear();

        for(int i = 0; i < cn; ++i)
        {
            mp[label_combobox[i]] = range(vmin[i], vmax[i]);
            //mp.insert(std::map<QString,range>::value_type(label_combobox[i],range(vmin[i],vmax[i])));
        }

        uimf->initTable();
        /*--------------end map-----------------*/
        ui->comboBox->clear();
        ui->comboBox->addItems(label_combobox);
        ui->aim_comboBox->clear();
        ui->aim_comboBox->addItems(label_aim);
        ui->aim_comboBox->setCurrentIndex(label_combobox.size());
        ui->tableWidget->setRowCount(div);
        ui->maxLineEdit->setText(QString::number(max));
        ui->minLineEdit->setText(QString::number(min));
        ui->divLineEdit->setText(QString::number(div));
        ui->total->setText(QString::number(tot));
        ui->eps_lineEdit->setText(QString::number(1e-5));
    }
    else
    {
        file->close();
        delete file;
        QMessageBox::critical(this, "Error Message", "File Open Error " + file->errorString());
    }

}

void MainWindow::on_minLineEdit_textChanged(const QString &arg1)
{
    Q_UNUSED(arg1)
    min = ui->minLineEdit->text().toDouble();
}

void MainWindow::on_maxLineEdit_textChanged(const QString &arg1)
{
    Q_UNUSED(arg1)
    max = ui->maxLineEdit->text().toDouble();
}

void MainWindow::on_divLineEdit_textChanged(const QString &arg1)
{
    Q_UNUSED(arg1)

}

void MainWindow::on_actionSave_triggered()
{

    if(c.empty())
    {
        QMessageBox::critical(this, "Fatal Error", "No data!");
        return;
    }

    QString temp = saveFileName;

    if(saveFileName.isEmpty())
        saveFileName = QFileDialog::getSaveFileName(this, tr("Save File"), QDir::currentPath() + tr("/file.txt"), "Text File(*.txt);;All(*)");
    else
        saveFileName = QFileDialog::getSaveFileName(this, tr("Save File"), saveFileName + tr("/file.txt"), "Text File(*.txt);;All(*)");

    if(saveFileName.isEmpty())
    {
        //QMessageBox::information(this,"Error Message","Please Select A Text File");
        saveFileName = temp;
        return;
    }

    QFile *file = new QFile;
    file->setFileName(saveFileName);
    bool ok = file->open(QIODevice::WriteOnly | QIODevice::Text);

    if(ok)
    {
        QTextStream out(file);

        if(ui->face_checkBox->isChecked())
            for(int i = 0 ; i < div; ++i)
            {
                out << c[i] << " " << ans[i] << "\n";
            }
        else
            for(int i = 0 ; i < div; ++i)
            {
                out << (c[i] + c[i + 1]) / 2 << " " << ans[i] << "\n";
            }
    }

    file->close();
    delete file;
}

void MainWindow::on_comboBox_currentIndexChanged(int index)
{
    Q_UNUSED(index)
    kt = ui->comboBox->currentIndex();

    if(v.size())
    {
        ui->maxLineEdit->setText(QString::number(vmax[kt]));
        ui->minLineEdit->setText(QString::number(vmin[kt]));
    }
}



void MainWindow::on_face_checkBox_clicked()
{
    if(ui->face_checkBox->checkState())
    {
        ui->tableWidget->setRowCount(div + 1);
    }
    else
    {
        ui->tableWidget->setRowCount(div);
    }
}


void MainWindow::on_percent_checkBox_clicked()
{
    if(ui->percent_checkBox->checkState())
    {
        label[2] = QString("Percentage");
        ui->tableWidget->setHorizontalHeaderLabels(label);
    }
    else
    {
        label[2] = QString("TotalMass");
        ui->tableWidget->setHorizontalHeaderLabels(label);
    }
}

void MainWindow::on_actionHelp_triggered()
{
    QMessageBox::information(this, "Help", "");
}

void MainWindow::on_actionAbout_Qt_triggered()
{
    QMessageBox::aboutQt(this);
}

void MainWindow::on_eps_lineEdit_editingFinished()
{
    if(ui->eps_lineEdit->text() == QString(""))
    {
        QMessageBox::warning(this, "Warning", "eps not set, set to default value 1e-5");
        ui->eps_lineEdit->setText(QString::number(1e-5));
    }
}

void MainWindow::on_actiondraw_triggered()
{
    d = new cplot(this);
    d->setWindowTitle("Curve Plot");
    d->add(c, ans);
    d->show();
}

void MainWindow::on_actionExit_triggered()
{
    exit(0);
}

void MainWindow::on_actionExport_triggered()
{
    uie = new ExportData(v, fore, label_combobox, saveFileName, vmin, vmax, mp, this);
    uie->setWindowTitle("Export Data");
    uie->setEnableMultiFilter(ui->mf_checkBox->isChecked());
    uie->show();
}

void MainWindow::on_mf_checkBox_clicked(bool checked)
{
    Q_UNUSED(checked)

    //when the mf_checkBox is active, pop-up a windows
    if(ui->mf_checkBox->isChecked())
    {
        uimf->show();
    }
}

void MainWindow::on_actionMultifillter_triggered()
{
    if(ui->mf_checkBox->isChecked())
    {
        uimf->show();
    }
}

void MainWindow::on_aim_comboBox_currentIndexChanged(const QString &arg1)
{
    Q_UNUSED(arg1)
    label[2] = ui->aim_comboBox->currentText();
    ui->tableWidget->setHorizontalHeaderLabels(label);
}

void MainWindow::on_divLineEdit_editingFinished()
{
    div = ui->divLineEdit->text().toInt();

    if(div < 1)
    {
        QMessageBox::critical(this, "Fatal Error", "div cannot be less than 1, set to 10");
        ui->divLineEdit->setText(QString::number(10));
        div = 10;
    }

    ui->tableWidget->setRowCount(div);
}
