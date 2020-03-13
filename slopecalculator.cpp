#include "slopecalculator.h"
#include "ui_slopecalculator.h"

slopecalculator::slopecalculator(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::slopecalculator)
{
    ui->setupUi(this);
    this->setWindowTitle("MX + B Slope Single Pont Calculator");
    loadSettings();
    fillHint();
    dblValidator = new QDoubleValidator();
    dblValidator->setNotation(QDoubleValidator::ScientificNotation);
    dblValidator->setRange(-9.9E19, 9.9E19,12);
    ui->lineMinIn->setValidator(dblValidator);
    ui->lineMaxIn->setValidator(dblValidator);
    ui->lineMinOut->setValidator(dblValidator);
    ui->lineMaxOut->setValidator(dblValidator);
    ui->lineSolveFor->setValidator(dblValidator);
    ui->lineSolvedResult->setValidator(dblValidator);

    fillSampleValues();

}

slopecalculator::~slopecalculator()
{
    delete ui;
}

void slopecalculator::on_btnClose_clicked()
{
    saveSettings();
    close();
}

void slopecalculator::saveSettings()
{
    QSettings setting("Keysight","MXPBAntennaSloper");

    setting.beginGroup("ProgramFolders");

    setting.setValue("SC-StartupPosition",this->geometry());

    setting.endGroup();
}

void slopecalculator::loadSettings()
{
    QSettings setting("Keysight","MXPBAntennaSloper");

    setting.beginGroup("ProgramFolders");

    QRect rect = setting.value("SC-StartupPosition").toRect();

    if (rect.x() > -1)
    {
        setGeometry(rect);
    }



    setting.endGroup();
}

void slopecalculator::fillHint()
{
    QString hintMessage = "";
    hintMessage.append("Solves for Y using the formula (M * X) + B");
    hintMessage.append("\n");
    hintMessage.append("Solves For :");
    hintMessage.append("\n");
    hintMessage.append("\n");
    hintMessage.append("Slope(M) : (OutMax - OutMin)/(InMax - InMin) = M");
    hintMessage.append("\n");
    hintMessage.append("Offset(B): (OutMin - (M * InMin) = B");




    ui->textHint->setText(hintMessage);
    ui->textHint->setTextInteractionFlags(Qt::NoTextInteraction);
    //ui->textHint->setTextInteractionFlags(QtCore.Qt.NoTextInteraction);

}

void slopecalculator::fillSampleValues()
{
    ui->lineMinIn->setText("4.00");
    ui->lineMaxIn->setText("20.0");
    ui->lineMinOut->setText("0.0");
    ui->lineMaxOut->setText("100.0");
}

double slopecalculator::solveProblem(double MinIn, double MaxIn, double MinOut, double MaxOut, double value)
{
    double result = -1;
    double MVal, BVal;

   // m_dMVal = (OutMax - OutMin) / (InMax - InMin)
   //   m_dBVal = (OutMin - (m_dMVal * InMin))
   // Calc = (m_dMVal * X) + m_dBVal

    if ((MaxIn > MinIn) && (MaxOut > MaxIn))
    {
        MVal = (MaxOut - MinOut) / (MaxIn - MinIn);
        BVal = (MinOut - (MVal * MinIn));
        result = (MVal * value) + BVal;
    }

    return result;
}

void slopecalculator::on_btnSolve_clicked()
{
    // solve the problem
    double answer;
    double MinIn = ui->lineMinIn->text().toDouble();
    double MaxIn = ui->lineMaxIn->text().toDouble();
    double MinOut = ui->lineMinOut->text().toDouble();
    double MaxOut = ui->lineMaxOut->text().toDouble();
    double value = ui->lineSolveFor->text().toDouble();


    answer = solveProblem(MinIn,MaxIn,MinOut,MaxOut,value);

    ui->lineSolvedResult->setText(QString::number(answer));

}
