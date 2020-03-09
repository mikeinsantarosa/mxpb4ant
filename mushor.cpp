/* File     : mushor.cpp
 * Author   : Mike Purtell
 * Date     : Nove 18, 2017
 *
 * Purpose  :
 *
 * Responsible for translating
 * a vendors  * acf data file
 * against the frequency reference
 * table.
 *
 *
 * */

#include "mushor.h"

Mushor::Mushor(const QString &key, QObject *parent) :
    QObject(parent)
{
    _key = key.trimmed();
}

int Mushor::setFreqList(QVector<ACFDataPoint> list)
{
    int result = -1;
    if (list.count() > 0)
    {
        _freqList = list;
        result = list.count();
        _numFreqRefPoints = list.count();
    }
    return result;
}

int Mushor::setListToTranslate(QVector<ACFDataPoint> list)
{
   int result = -1;
   if (list.count() > 0)
   {
       _baseList = list;
       result = list.count();
       _numBaseListPoints = list.count();

   }
   return result;
}

int Mushor::setReportableFileNames(QString txFileName, QString rxFileName)
{
    int result = -1;
    int goodHits = 0;

    if (!txFileName.isEmpty())
    {
        _txFilename = txFileName;
        goodHits += 1;
        result = goodHits;
    }
    if (!rxFileName.isEmpty())
    {
        _rxFilename = rxFileName;
        goodHits += 1;
        result = goodHits;
    }
    _numFilesToReport =  result;

    return result;

}

void Mushor::setOutputFolder(QString folder)
{
    if (!folder.isEmpty())
    {
       _OutputFolder = folder;
    }
}

void Mushor::showData()
{
    if (dataIsValid()==true)
    {
        listFreqRefLines();
        listBaseListLines();
    }
    else
    {
        qDebug() << "Data isn't valid yet";
        qDebug() << "Number of freq ref points - " << _numFreqRefPoints;
        qDebug() << "Number of base list points - " << _numBaseListPoints;
        qDebug() << "Number of Files - " << _numFilesToReport;
    }
}

void Mushor::clearData()
{
    _baseList.clear();
    _freqList.clear();
}

void Mushor::MushData()
{

    if (mush())
    {
        printReports();
        //listReportLines();

    }
    else
    {
        qDebug() << "Failed to mush data";
    }
}

QString Mushor::getKey()
{
    return _key;
}

int Mushor::getReportListCount()
{
    return _ReportList.count();
}

void Mushor::listFreqRefLines()
{
    for (int i = 0; i < _freqList.count(); i++)
    {
        qDebug() << "Point # " << i << " is " << _freqList[i].getfreq();
    }

}

void Mushor::listBaseListLines()
{
    for (int i = 0; i < _baseList.count(); i++)
    {
        qDebug() << "Point # " << i << " is " << _baseList[i].getfreq();
    }
}

void Mushor::listReportLines()
{
    for (int i = 0; i < _ReportList.count(); i++)
    {
        qDebug() << "Point # " << i << " is " << _ReportList[i].getfreq() << " - " << _ReportList[i].getLevel();
    }
}

int Mushor::printReports()
{
    int result = -1;
    QString file1;
    QString file2;
    if (!_OutputFolder.isEmpty())
    {
        file1.clear();
        file1.append(_OutputFolder);
        file1.append("/");
        file1.append(_rxFilename);

        file2.clear();
        file2.append(_OutputFolder);
        file2.append("/");
        file2.append(_txFilename);

       writeFileToOutputFolder(file1, _ReportList);
       writeFileToOutputFolder(file2, _ReportList);
        result =1;
    }
    return result;
}

void Mushor::setCurrentParams(double outMax, double outMin, double inMax, double inMin)
{
    _mValue = (outMax - outMin) / (inMax - inMin);
    _bValue = (outMin - (_mValue * inMin));
}

double Mushor::calcSlopedValue(double x)
{
    double result;

    result = (_mValue * x) + _bValue;
    return result;
}

bool Mushor::mush()
{
    double slopedACF;
    double currentAcfFreq;
    bool result = false;
    int startIDX = 0;
    _ReportList.clear();


    _currentACFIDX = 0;
    startIDX = 0;

    for (int i = 0; i < _freqList.count(); i++)
    {
        currentAcfFreq = _freqList[i].getfreq();
        _currentACFIDX = getPriorIndex(currentAcfFreq, startIDX);
        _nextACFIDX = getNextIndex(currentAcfFreq, _currentACFIDX);
        setCurrentParams(_baseList[_nextACFIDX].getLevel(), _baseList[_currentACFIDX].getLevel(), _baseList[_nextACFIDX].getfreq(), _baseList[_currentACFIDX].getfreq());
        slopedACF = calcSlopedValue(currentAcfFreq);
        ACFDataPoint * dPoint = new ACFDataPoint(currentAcfFreq, slopedACF);
        _ReportList.append(*dPoint);

        startIDX = _currentACFIDX;

    }

    if (_ReportList.count() == _freqList.count())
    {
        result = true;
    }

    return result;

}


// find the index that is @ the freq just before going over
// ---------------------------------------------
int Mushor::getPriorIndex(double freqToCheck, int startIndex)
{
    int IDX = startIndex;
    bool found = false;

    double thisFreq;


    int lastIDX = _baseList.count() - 2;

    if (startIndex < lastIDX)
    {
        while (!found)
        {
           thisFreq = _baseList[IDX].getfreq();

           if (thisFreq > freqToCheck)
           {
               found = true;
           }
           else
           {
               IDX = IDX +1;
           }
       }
       IDX = IDX - 1;
    }
   else
   {
       IDX = startIndex;
   }

    return IDX ;

}

int Mushor::getNextIndex(double freqToCheck, int startIndex)
{
    int IDX = startIndex;
    bool found = false;
    int lastIDX = _baseList.count() - 1;

    double nextFreq;

    if (startIndex < lastIDX)
    {

        while (!found)
        {
            nextFreq = _baseList[IDX].getfreq();

            if (nextFreq > freqToCheck)
            {
                found = true;
            }
            else
            {
                IDX = IDX +1;
            }
        }

    }
    else
    {
        IDX = _baseList.count() - 1;
    }


    return IDX;
}

int Mushor::writeFileToOutputFolder(QString file,QVector <ACFDataPoint> list)
{
    QFile outputFile(file);
    outputFile.open(QIODevice::WriteOnly);
    QString msg;
    // qDebug() << "Writing to this file - " << file;

    // Check it opened OK
    if(!outputFile.isOpen())
    {
        return 1;
    }

    // Point a QTextStream object at the file
    QTextStream outStream(&outputFile);

    // Write the line to the file

    for (int i = 0; i < list.count(); i++)
    {

        msg.clear();
        msg.append(QString::number(list[i].getfreq()));
        msg.append(",");
        msg.append(QString::number(list[i].getLevel()));
        msg.append("\n");
        outStream << msg;
    }

    // Close the file
    outputFile.close();

    return 0;
}



bool Mushor::dataIsValid()
{
    bool result = false;
    if ((_numFreqRefPoints > 0) && (_numBaseListPoints > 0) && (_numFilesToReport == 2))
    {
        result = true;
    }

    return result;

}
