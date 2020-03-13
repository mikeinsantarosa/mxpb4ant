#include "mxpb4ant.h"
#include "ui_mxpb4ant.h"
#include "about.h"

mxpb4ant::mxpb4ant(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::mxpb4ant)
{
    ui->setupUi(this);

    //initForLinux();
    initForWindows();

    QString ver = QString("%1.%2.%3").arg(MY_MAJOR_VERSION).arg(MY_MEAN_VERSION).arg(MY_MINOR_VERSION);

    QString chartTitle = "";
    chartTitle.append("Hardware Test Center - MX+B Corrections Calculator: ");
    chartTitle.append(ver);
    this->setWindowTitle(chartTitle);

    _inputFileHasBeenSet = false;
    _outputFileHasBeenSet = false;
    _freqRefHasBeenSet = false;
    _currentOutputFileName = "";
    _currentInputFileName = "";



    loadSettings();

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

QStringList mxpb4ant::cleanList(QStringList list, QString fileName)
{

    QStringList group;
    QString current;
    QStringList result;
    QString target = "";


    if (!result.isEmpty() == true)
    {
        result.clear();
    }

    QString delim = getDelim(fileName);

    // strip any header if there is one.
    int startAt = findFirstDataRow(list, delim);


    for (int listRow = startAt; listRow < list.count(); listRow++)
    {
        current = list.at(listRow);
        group = current.split(delim);

        target.clear();
        // just get the 1st 2 columns
        target.append(group.at(0) + delim + group.at(1));

        result.append(target);

    }

    return result;


}

void mxpb4ant::showOutOfRangeMessage()
{
    QMessageBox msgBox;
        QString message = "";

        message.append("The range of input frequencies ");
        message.append(QString::number(freqRanges.startInFreq) + "Hz to " + QString::number(freqRanges.stopInFreq) + "Hz ");
        message.append(" is outside the bounds of your corection data frequency range ");
        message.append(QString::number(freqRanges.startOutFreq) + "Hz to " + QString::number(freqRanges.stopOutFreq) + "Hz. ");
        message.append("You need to adjust one of the ranges.");


        msgBox.setText(message);
        msgBox.setInformativeText("The input range must be within the range of the correction data.");
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.setDefaultButton(QMessageBox::Ok);

        int ret = msgBox.exec();

}

void mxpb4ant::showMissingInfoMessage()
{
    //_inputFileHasBeenSet == true && _outputFileHasBeenSet == true &&  _freqRefHasBeenSet == true
    if(_freqRefHasBeenSet == false)
    {
        //show missing freq ref file
        QMessageBox::StandardButton reply;
        reply = QMessageBox::critical(this,"Missing file set", "The freq ref file hasn't been set yet!",QMessageBox::Ok);
    }
    else if (_inputFileHasBeenSet == false)
    {
        //show missing input file
        QMessageBox::StandardButton reply;
        reply = QMessageBox::critical(this,"Missing file set", "The input corrections file hasn't been set yet!",QMessageBox::Ok);
    }
    else if (_outputFileHasBeenSet == false)
    {
        //show missing output file
        QMessageBox::StandardButton reply;
        reply = QMessageBox::critical(this,"Missing file set", "The output destination file hasn't been set yet!",QMessageBox::Ok);
    }
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

void mxpb4ant::fillOutputList()
{
    ACFDataPoint point;
    ui->listOutputs->clear();
    QString target, fVal, lVal;
    double freq, level;
    QString delim = getDelim(_currentOutputFileName);

    for(int i = 0; i < resultList.count(); i++)
    {
        target.clear();
        point = resultList.at(i);
        freq = point.getfreq();
        level = point.getLevel();
        fVal = QString::number(freq);
        lVal = QString::number(level);

        ui->listOutputs->addItem(fVal + delim + lVal);
    }
}

QString mxpb4ant::getDelim(QString fileName)
{
    QFileInfo info = QFileInfo(fileName);
    QString result = "";

    if (info.suffix() == "csv")
    {
        result = ",";
    }
    else if(info.suffix() == "txt")
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

bool mxpb4ant::CheckForValidRanges(QStringList freqList, QStringList valuesList, QString vFname)
{
    double f1;
    double f2;
    double f1Val;
    double f2Val;

    bool result = true;

    // do the freq list
    int fOffset = freqList.count()-1;
    f1 = freqList.at(0).toDouble();
    f2 = freqList.at(fOffset).toDouble();

    // do the values list
    int vOffset = valuesList.count() -1;
    QString s1 = valuesList.at(0);
    QString s2 = valuesList.at(vOffset);
    QString delim = getDelim(vFname);

    QStringList d1 = s1.split(delim);
    QStringList d2 = s2.split(delim);

    f1Val = d1.at(0).toDouble();
    f2Val = d2.at(0).toDouble();


    if (f1 < f1Val)
    {
        result = false;
    }
    else if (f2 > f2Val)
    {
        result = false;
    }


    freqRanges.startInFreq = f1;
    freqRanges.stopInFreq = f2;
    freqRanges.startOutFreq = f1Val;
    freqRanges.stopOutFreq = f2Val;

    return result;


}

void mxpb4ant::on_btnClose_clicked()
{
    saveSettings();
    QCoreApplication::quit();
}



void mxpb4ant::on_btnInputFile_clicked()
{

    QString InputFile = QFileDialog::getOpenFileName(this,"Open File", _workingPath,
                                                      "Text Files (*.txt);; CSV Files (*.csv)");

    if(!InputFile.isEmpty())
    {
        QFileInfo fileinf = QFileInfo(InputFile);
        _workingPath = fileinf.path();

        _currentInputFileName = InputFile;
        ui->lblInputFile->setText(InputFile);

        // Turn the file into a list
        _InputListRaw = loadListfromFile(InputFile);
        QString del = getDelim(InputFile);

        _InputListCleaned = cleanList(_InputListRaw, InputFile);

        fillInputList(_InputListCleaned, 0);

        setValuesVector(_InputListCleaned);


        _inputFileHasBeenSet = true;

    }


    saveSettings();
}

void mxpb4ant::on_btnOutputFile_clicked()
{
    QString OutputFile = QFileDialog::getSaveFileName(this,"Save File", _outputFilePath,
                                                   "Text Files (*.txt);; CSV Files (*.csv)");


    QString defaultExtension = ".txt";
    QString defaultMatch = "txt";
    QString alternateExtension = ".csv";
    QString altMatch = "csv";


    if(!OutputFile.isEmpty())
    {
        QFileInfo fileinf = QFileInfo(OutputFile);
        _outputFilePath = fileinf.path();

        if(fileinf.suffix().isEmpty())
        {
            //qDebug() << "adding default file extension";
            OutputFile.append(defaultExtension);
        }
        else if (fileinf.suffix()== defaultMatch)
        {
            // do nothing
            //qDebug() << "file extension is " << fileinf.suffix();
        }
        else if (fileinf.suffix()== altMatch)
        {
            // do nothing
        }
        else
        {
            QString path = fileinf.path();
            QString fname = fileinf.baseName();
            QString newFile = "";
            newFile.append(path + "/");
            newFile.append(fname);
            newFile.append(defaultExtension);
            OutputFile = newFile;

        }

       // qDebug() << "will be using " << OutputFile;


        _currentOutputFileName = OutputFile;
        ui->lblOutputFile->setText(OutputFile);

        _outputFileHasBeenSet = true;
    }


    saveSettings();
}

void mxpb4ant::initForWindows()
{
    if (_workingPath == "")
    {
        //linux
        //_workingPath = "/home/mandbx/temp/NSA_cert_parts/";
        _workingPath = "C:/Users/mpurtell/Desktop/Desktop-files/3110C-3110C_pair_data";
    }


    if(_outputFilePath == "")
    {
        //linux
       // _outputFilePath = "/home/mandbx/temp";
        _outputFilePath = "C:/Temp/";
    }


    if (_freqRefPath == "")
    {
        //linux
        //_freqRefPath = "/home/mandbx/Qt-training/mxpb4ant/";
        _freqRefPath = "C:/qtTraining/mxpb4ant/mxpb4ant";
    }
}

void mxpb4ant::initForLinux()
{
    if (_workingPath == "")
    {
        //linux
        _workingPath = "/home/mandbx/temp/NSA_cert_parts/";
        //_workingPath = "C:/Users/mpurtell/Desktop/Desktop-files/3110C-3110C_pair_data";
    }


    if(_outputFilePath == "")
    {
        //linux
        _outputFilePath = "/home/mandbx/temp";
       // _outputFilePath = "C:/Temp/";
    }


    if (_freqRefPath == "")
    {
        //linux
        _freqRefPath = "/home/mandbx/Qt-training/mxpb4ant/";
        //_freqRefPath = "C:/qtTraining/mxpb4ant/mxpb4ant";
    }
}

void mxpb4ant::listThisList(QStringList list)
{
    for (int i = 0; i < list.count(); i++)
    {
        qDebug() << "line " << i << " - " << list.at(i);
    }
}

void mxpb4ant::setFreqVector(QStringList list)
{
    double defaultLevel = 0;

    if(!freqVectorList.isEmpty())
   {
       freqVectorList.clear();
   }

    for (int i = 0; i < list.count(); i++)
    {
        ACFDataPoint *point = new ACFDataPoint(list.at(i).toDouble(), defaultLevel);
        freqVectorList.append(*point);
    }

}

void mxpb4ant::setValuesVector(QStringList list)
{
    QStringList group;
    QString target;
    QString delim = getDelim(_currentInputFileName);

    if(!valuesVectorList.isEmpty())
   {
       valuesVectorList.clear();
   }

    for (int i = 0; i < list.count(); i++)
    {
        target = list.at(i);
        group = target.split(delim);

        ACFDataPoint *point = new ACFDataPoint(group.at(0).toDouble(), group.at(1).toDouble());
        valuesVectorList.append(*point);
    }
}

void mxpb4ant::writeListToFile(QString filename, QVector<ACFDataPoint> list)
{
    QString target, fVal, lVal;
    double freq, level;
    ACFDataPoint point;

    QString delim = getDelim(filename);

    QFile fOut(filename);
    if(fOut.open(QFile::WriteOnly | QFile::Text))
    {
        QTextStream s(&fOut);
        for(int i = 0; i < resultList.count(); i++)
        {
            target.clear();
            point = resultList.at(i);
            freq = point.getfreq();
            level = point.getLevel();
            fVal = QString::number(freq);
            lVal = QString::number(level);

            s << fVal <<  delim << lVal << '\n';
        }
    }
    else
    {
        QMessageBox::StandardButton reply;
        reply = QMessageBox::critical(this,"File Write problem","Couldn't open the file [" + filename + "] for writing!",QMessageBox::Ok);
    }



}

void mxpb4ant::setFreqRefFile()
{

    QString fileToOpen = QFileDialog::getOpenFileName(this,
                                                      "Open File", _freqRefPath,"Text Files (*.txt);; CSV Files (*.csv)");

    if(!fileToOpen.isEmpty())
    {


        QFileInfo fileinf = QFileInfo(fileToOpen);
        _freqRefPath = fileinf.path();

        _currentFreqRefFileName = fileToOpen;
        ui->lblFreqRefFile->setText(fileToOpen);

        QString delim = getDelim(fileToOpen);

        // do list work
        _FreqRefListRaw = loadListfromFile(fileToOpen);

       // int startPos = findFirstDataRow(_FreqRefListRaw, delim);

        fillFreqRefList(_FreqRefListRaw, 0);
        setFreqVector(_FreqRefListRaw);
        saveSettings();
        _freqRefHasBeenSet = true;
    }


}

void mxpb4ant::on_btnExecute_clicked()
{

    bool OKtoGo;

    if(_inputFileHasBeenSet == true && _outputFileHasBeenSet == true &&  _freqRefHasBeenSet == true)
    {
        QFileInfo info = QFileInfo(_currentOutputFileName);
        QString folder = info.path();
        OKtoGo = CheckForValidRanges(_FreqRefListRaw, _InputListCleaned, _currentInputFileName);

        if (OKtoGo == true)
        {
            QString f1 = "/home/mandbx/temp/tx.txt";
            QString f2 = "/home/mandbx/temp/rx.txt";

            Mushor *m = new Mushor("keyMe",this);
            m->setFreqList(freqVectorList);
            m->setListToTranslate(valuesVectorList);
            m->setReportableFileNames(f1,f2);
            m->setOutputFolder(folder);

            m->MushData();

            resultList.clear();
            if (m->getReportListCount()>0)
            {
                resultList = m->getOutputVector();

                // fill the list and save the data
                fillOutputList();

                writeListToFile(_currentOutputFileName, resultList);

            }

        }
        else
        {
            showOutOfRangeMessage();
        }
    }
    else
    {
       showMissingInfoMessage();
    }

}


void mxpb4ant::on_btnSetFreqRef_clicked()
{
    // set the freq ref file
    setFreqRefFile();
}

void mxpb4ant::on_actionSlope_Calculator_triggered()
{
    //qDebug() << "CLicked open slope calculator";
    sc->show();
}
