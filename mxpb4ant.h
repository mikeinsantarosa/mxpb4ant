#ifndef MXPB4ANT_H
#define MXPB4ANT_H

#include "freqref.h"
#include "acfdatapoint.h"
#include "mushor.h"
#include "slopecalculator.h"


#include <QMainWindow>
#include <QDialog>
#include <QFileDialog>
#include <QMessageBox>
#include <QFile>
#include <QString>
#include <QDebug>
#include <QStringList>
#include <QTextStream>
#include <QSettings>
#include <QFile>
#include <QFileInfo>
#include <QRegExp>
#include <algorithm>
#include <QVector>


QT_BEGIN_NAMESPACE
namespace Ui { class mxpb4ant; }
QT_END_NAMESPACE

class mxpb4ant : public QMainWindow
{
    Q_OBJECT

public:
    mxpb4ant(QWidget *parent = nullptr);
    ~mxpb4ant();



private slots:
    void on_actionAbout_triggered();

    void on_btnClose_clicked();

//    void on_actionSet_Freq_Ref_File_triggered();

    void on_actionSet_Freq_Ref_File_triggered();

    void on_btnInputFile_clicked();

    void on_btnOutputFile_clicked();

    void on_btnExecute_clicked();

    void on_btnSetFreqRef_clicked();

    void on_actionSlope_Calculator_triggered();

private:
    Ui::mxpb4ant *ui;
    slopecalculator *sc = new slopecalculator;


    // debug methods
    void initForWindows();
    void initForLinux();
    void listThisList(QStringList list);

    void setFreqVector(QStringList list);
    void setValuesVector(QStringList list);

    void writeListToFile(QString filename, QVector<ACFDataPoint> list);

    void setFreqRefFile();

    // methods
   void saveSettings();
   void loadSettings();

   void setWorkingPath(QString path);
   void setFreqRefPath(QString path);

   QStringList loadListfromFile(QString fileName);
   QStringList cleanList(QStringList list, QString fileName);
   QVector<ACFDataPoint> freqVectorList;
   QVector<ACFDataPoint> valuesVectorList;
   QVector<ACFDataPoint> resultList;

   enum class _mxpbRanges
     {
        InStartFreq = 0,
        InStopFreq,
        OutStartFreq,
        OutStopFreq
     };


   struct FreqRanges
   {
   double startInFreq = 0;
   double stopInFreq = 0;
   double startOutFreq = 0;
   double stopOutFreq = 0;
   } freqRanges;



   // error messages
   void showOutOfRangeMessage();
   void showMissingInfoMessage();


   void fillFreqList(QStringList list);
   void fillFreqRefList(QStringList list, int startAt);
   void fillInputList(QStringList list, int startAt);

   void fillOutputList();

   QString getDelim(QString fileName);

   int findFirstDataRow(QStringList list, QString delimiter);

   bool CheckForValidRanges(QStringList freqList, QStringList valuesList, QString vFname);

   // variables
   QString _workingPath;
   QString _freqRefPath;
   QString _outputFilePath;
   QString _currentFreqRefFileName;
   QString _currentInputFileName;
   QString _currentOutputFileName;
   QString NotFoundMarker;

   // lists
   QStringList _FreqRefListRaw;
   QStringList _InputListRaw;
   QStringList _InputListCleaned;

   bool _freqRefHasBeenSet = false;
   bool _inputFileHasBeenSet = false;
   bool _outputFileHasBeenSet = false;

   //objects
  // FreqRef _freqRef;
};
#endif // MXPB4ANT_H
