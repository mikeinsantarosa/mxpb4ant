/* File     : mushor.cpp
 * Author   : Mike Purtell
 * Date     : Nove 18, 2017
 *
 * Purpose  :
 *
 * Object responsible for
 * translating a vendors
 * acf data file against the
 * frequency reference table
 *
 *
 * */

#include "freqref.h"

FreqRef::FreqRef(const QString &path)
{
    _pointList.clear();
    _masterList.clear();

    init(path);
}

QString FreqRef::getPath()
{
    return _path;
}

QString FreqRef::getFilename()
{
    return _filname;;
}

QVector<ACFDataPoint> FreqRef::getPoints()
{
    return _pointList;
}

void FreqRef::init(QString fileToLoad)
{

    int numLines;
    _masterList = loadListfromFile(fileToLoad);
    setWorkingPath(fileToLoad);
    setWorkingFilename(fileToLoad);

    numLines = setPointList();

    //listLines();
}

QStringList FreqRef::loadListfromFile(QString fileName)
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

void FreqRef::setWorkingPath(QString path)
{
    if(!path.isEmpty())
    {
        QFileInfo fileinfo(path);
        _filename = fileinfo.fileName();
    }
}

void FreqRef::setWorkingFilename(QString path)
{
    if(!path.isEmpty())
    {
        QFileInfo fileinfo(path);
        _path = fileinfo.path();
    }
}

int FreqRef::setPointList()
{
    int result = -1;
    double freq = -1;
    double level = -1;
    QStringList group;

    for (int i = 0; i < _masterList.count(); i++)
    {
        group = _masterList[i].split("\t");
        freq = group[0].toDouble();
        level = QString("-1").toDouble();

        // not sure if this should be a pointer
        // -------------------------------------
        ACFDataPoint *apoint = new ACFDataPoint(freq, level);
        _pointList.append(*apoint);

    }

    result = _pointList.count();



    return result;
}

void FreqRef::listLines()
{
    for (int i = 0; i < _pointList.count(); i++)
    {
        qDebug() << "Point # " << i << " is " << _pointList[i].getfreq();
    }
}
