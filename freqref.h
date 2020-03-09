/* File     : freqref.h
 * Author   : Mike Purtell
 * Date     : Nove 18, 2017
 *
 * Purpose  :
 *
 * Header file for the
 * frequency reference table
 * that all output reports
 * are derived from.
 *
 *
 * */

#ifndef FREQREF_H
#define FREQREF_H

#include "acfdatapoint.h"
#include <QDebug>
#include <QVector>
#include <QString>
#include <QFile>
#include <QStringList>
#include <QFileInfo>

class FreqRef
{
public:
    FreqRef(const QString &path);
    QString getPath();
    QString getFilename();

    QVector <ACFDataPoint> getPoints();

private:

    // variables
    QVector <ACFDataPoint> _pointList;
    QStringList _masterList;
    QString _filname;
    QString _path;
    QString _filename;

    // methods
    void init(QString fileToLoad);
    QStringList loadListfromFile(QString fileName);
    void setWorkingPath(QString path);
    void setWorkingFilename(QString path);
    int setPointList();
    void listLines();

};

#endif // FREQREF_H
