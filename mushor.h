/* File     : mushor.h
 * Author   : Mike Purtell
 * Date     : Nove 18, 2017
 *
 * Purpose  :
 *
 * Header file for the object
 * that translates a vendors
 * acf data file against the
 * frequency reference table
 *
 *
 * */
#ifndef MUSHOR_H
#define MUSHOR_H

#include <QObject>
#include <QDebug>
#include "acfdatapoint.h"
#include <QString>
#include <QFile>
#include <QTextStream>

class Mushor : public QObject
{
    Q_OBJECT
public:
    explicit Mushor(const QString &key, QObject *parent = 0);

    int setFreqList(QVector <ACFDataPoint> list);
    int setListToTranslate(QVector <ACFDataPoint> list);
    int setReportableFileNames(QString txFileName, QString rxFileName);
    void setOutputFolder(QString folder);

    void showData();
    void clearData();

    void MushData();
    QString getKey();

    int getReportListCount();
    QVector <ACFDataPoint> getOutputVector();

private:

    // variables
    QVector <ACFDataPoint> _freqList;
    QVector <ACFDataPoint> _baseList;
    QVector <ACFDataPoint> _ReportList;



    QString _txFilename;
    QString _rxFilename;
    QString _OutputFolder;
    QString _key;

    int _numFreqRefPoints;
    int _numBaseListPoints;
    bool _numFilesToReport;
    int _currentACFIDX;
    int _nextACFIDX;

    double _mValue;
    double _bValue;

    // methods
    bool dataIsValid();
    void listFreqRefLines();
    void listBaseListLines();
    void listReportLines();

    int printReports();

    void setCurrentParams(double outMax, double outMin, double inMax, double inMin);
    double calcSlopedValue(double x);
    bool mush();

    int getPriorIndex(double freqToCheck, int startIndex);
    int getNextIndex(double freqToCheck, int startIndex);

    int writeFileToOutputFolder(QString file, QVector <ACFDataPoint> list);






signals:

public slots:

};

#endif // MUSHOR_H
