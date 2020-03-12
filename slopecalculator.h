#ifndef SLOPECALCULATOR_H
#define SLOPECALCULATOR_H

#include <QMainWindow>
#include <QtDebug>
#include <QSettings>
#include <QDoubleValidator>
#include <QtCore>


namespace Ui {
class slopecalculator;
}

class slopecalculator : public QMainWindow
{
    Q_OBJECT

public:
    explicit slopecalculator(QWidget *parent = nullptr);
    ~slopecalculator();

private slots:
    void on_btnClose_clicked();

    void on_btnSolve_clicked();

private:
    Ui::slopecalculator *ui;

    QDoubleValidator *dblValidator;

    void saveSettings();
    void loadSettings();

    void fillHint();
    void fillSampleValues();

    double solveProblem(double MinIn, double MaxIn, double MinOut, double MaxOut, double value);
};

#endif // SLOPECALCULATOR_H
