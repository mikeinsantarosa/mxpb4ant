#include "mxpb4ant.h"
#include "ui_mxpb4ant.h"
#include "about.h"

mxpb4ant::mxpb4ant(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::mxpb4ant)
{
    ui->setupUi(this);

    loadSettings();

    qDebug() << "workingPath = " << _workingPath;

    if (_workingPath == "")
    {
        //linux
        //_workingPath = "/home/mandbx/temp/NSA_cert_parts/";
        _workingPath = "C:/Users/mpurtell/Desktop/Desktop-files/3110C-3110C_pair_data";
    }

    qDebug() << "outputFilePath = " << _outputFilePath;

    if(_outputFilePath == "")
    {
        //linux
       // _outputFilePath = "/home/mandbx/temp";
        _outputFilePath = "C:/Temp/";
    }

    qDebug() << "freqRefpath == " << _freqRefPath;

    if (_freqRefPath == "")
    {
        //linux
        //_freqRefPath = "/home/mandbx/Qt-training/mxpb4ant/";
        _freqRefPath = "C:/qtTraining/mxpb4ant/mxpb4ant";
    }


}

mxpb4ant::~mxpb4ant()
{
    delete ui;
}


void mxpb4ant::on_actionAbout_triggered()
{
    // show about dialog
    About *about = new About;
    about->show();

}

void mxpb4ant::saveSettings()
{
    QSettings setting("Keysight","MXPBAntennaSloper");

    setting.beginGroup("ProgramFolders");

    setting.setValue("freqRefPath",_freqRefPath);
    setting.setValue("workingPath",_workingPath);
    setting.setValue("outputPath",_outputFilePath);

    setting.setValue("freqRefFile",_currentFreqRefFileName);
    setting.setValue("inputFile",_currentInputFileName);
    setting.setValue("outputFile",_currentOutputFileName);


//    setting.setValue("freqRefFile",_currentFreqRefFileName);
//    setting.setValue("reportFolder",_currentReportFolder);
    setting.setValue("StartupPosition",this->geometry());

    setting.endGroup();

}

void mxpb4ant::loadSettings()
{
    QSettings setting("Keysight","MXPBAntennaSloper");

    setting.beginGroup("ProgramFolders");

    _freqRefPath = setting.value("freqRefPath").toString();
    _workingPath = setting.value("workingPath").toString();
    _outputFilePath = setting.value("outputPath").toString();

    _currentFreqRefFileName = setting.value("freqRefFile").toString();
    _currentInputFileName = setting.value("inputFile").toString();
    _currentOutputFileName = setting.value("outputFile").toString();

    QRect rect = setting.value("StartupPosition").toRect();
    setGeometry(rect);

    setting.endGroup();
    // qDebug() << "Loaded freq ref file in settings - " << _currentFreqRefFileName;
//    ui->linefreqRef->setText(_currentFreqRefFileName);
    //    ui->lineReportPath->setText(_currentReportFolder);
}

void mxpb4ant::setWorkingPath(QString path)
{

}

void mxpb4ant::setFreqRefPath(QString path)
{
    if(!path.isEmpty())
    {
        QFileInfo fileinfo(path);
        _freqRefPath = fileinfo.path();
        //ui->linefreqRef->setText(path);
        saveSettings();
    }
}

QStringList mxpb4ant::loadListfromFile(QString fileName)
{
    QFile file(fileName);
    QStringList strings;
    strings.clear();

    if (file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QTextStream in(&file);
        while (!in.atEnd())
        {
            strings += in.readLine();
        }
            file.close();
        }

    return strings;
}

void mxpb4ant::fillFreqRefList(QStringList list, int startAt)
{
    if(!list.isEmpty())
    {
       ui->listFreqRefs->clear();
    }

    for (int i = startAt; i < list.count(); i++)
    {
        ui->listFreqRefs->addItem(list.at(i));
    }

}

void mxpb4ant::fillInputList(QStringList list, int startAt)
{
    if(!list.isEmpty())
    {
       ui->listInputs->clear();
    }

    for (int i = startAt; i < list.count(); i++)
    {
        ui->listInputs->addItem(list.at(i));
    }
}

QString mxpb4ant::getDelim(QString fileName)
{
    QFileInfo info = QFileInfo(fileName);
    QString result = "";

    if (info.suffix() == ".csv")
    {
        result = ",";
    }
    else if(info.suffix() == ".txt")
    {
        result = "\t";
    }

    return result;
}

int mxpb4ant::findFirstDataRow(QStringList list, QString delimiter)
{
    QStringList group;
        QString current;
        QString dataItem;
        int numFinds;
        bool isNumber;
        int result = -1;
        bool found = false;
        QRegExp re("^-?\\d*\\.?\\d+");
        QRegExp re2("((\\b[0-9]+)?\\.)?\\b[0-9]+([eE][-+]?[0-9]+)?\\b");



        for (int listRow = 0; listRow < list.count(); listRow++)
        {

            if (!found)
            {
                numFinds = 0;
                current = list[listRow];
                group = current.split(delimiter);

                for (int i = 0; i < group.count(); i++)
                {
                    dataItem = group[i];
                    dataItem = dataItem.trimmed();
                    isNumber = false;

                    if (!dataItem.isEmpty())
                    {

                        if(re.exactMatch(dataItem) || re2.exactMatch(dataItem))
                        {

                            numFinds = numFinds + 1;

                             if (numFinds > 1)
                            {
                                result = listRow;
                                found = true;
                                break;

                            }
                        }
                        else
                        {
                            break;
                        }


                    }

                 }

            }
            else
            {
                break;
            }


        }

        return result;

}

void mxpb4ant::on_btnClose_clicked()
{
    saveSettings();
    QCoreApplication::quit();
}

//void mxpb4ant::on_actionSet_Freq_Ref_File_triggered()
//{
//    QString fileToOpen = QFileDialog::getOpenFileName(this,"Open File", freqRefPath,"Text File (*.txt)");

//    if(!fileToOpen.isEmpty())
//    {
//        FreqRef * rFreq = new FreqRef(fileToOpen);
//        _currentFreqRefFileName = fileToOpen;

//        ui->linefreqRef->setText(fileToOpen);

//        setFreqRefPath(fileToOpen);

//        saveSettings();
//    }

//}

void mxpb4ant::on_actionSet_Freq_Ref_File_triggered()
{
    qDebug() << "getting freq Ref file";
    QString fileToOpen = QFileDialog::getOpenFileName(this,
                                                      "Open File", _freqRefPath,"Text Files (*.txt);; CSV Files (*.csv)");

    if(!fileToOpen.isEmpty())
    {


        QFileInfo fileinf = QFileInfo(fileToOpen);
        _freqRefPath = fileinf.path();

        _currentFreqRefFileName = fileToOpen;
        ui->lblFreqRefFile->setText(fileToOpen);

        QString del = getDelim(fileToOpen);

        // do list work
        _FreqRefListRaw = loadListfromFile(fileToOpen);

        int startPos = findFirstDataRow(_FreqRefListRaw, del);

        fillFreqRefList(_FreqRefListRaw, startPos);

        saveSettings();
    }

}

void mxpb4ant::on_btnInputFile_clicked()
{
    qDebug() << "getting input file";


    QString InputFile = QFileDialog::getOpenFileName(this,"Open File", _workingPath,
                                                      "Text Files (*.txt);; CSV Files (*.csv)");

    if(!InputFile.isEmpty())
    {
        QFileInfo fileinf = QFileInfo(InputFile);
        _workingPath = fileinf.path();

        _currentInputFileName = InputFile;
        ui->lblInputFile->setText(InputFile);

        // do list work
        _InputListRaw = loadListfromFile(InputFile);
        QString del = getDelim(InputFile);

        int startPos = findFirstDataRow(_InputListRaw, del);

        fillInputList(_InputListRaw, startPos);

    }


    saveSettings();
}

void mxpb4ant::on_btnOutputFile_clicked()
{
    qDebug() << "getting output file";
    QString OutputFile = QFileDialog::getSaveFileName(this,"Save File", _outputFilePath,
                                                      "Text Files (*.txt);; CSV Files (*.csv)");

    if(!OutputFile.isEmpty())
    {
        QFileInfo fileinf = QFileInfo(OutputFile);
        _outputFilePath = fileinf.path();

        _currentOutputFileName = OutputFile;
        ui->lblOutputFile->setText(OutputFile);
    }


    saveSettings();
}
