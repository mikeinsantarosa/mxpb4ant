#ifndef MXPB4ANT_H
#define MXPB4ANT_H

#include "freqref.h"

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

private:
    Ui::mxpb4ant *ui;

    // methods
   void saveSettings();
   void loadSettings();

   void setWorkingPath(QString path);
   void setFreqRefPath(QString path);

   QStringList loadListfromFile(QString fileName);

   void fillFreqList(QStringList list);
   void fillFreqRefList(QStringList list, int startAt);
   void fillInputList(QStringList list, int startAt);

   QString getDelim(QString fileName);

   int findFirstDataRow(QStringList list, QString delimiter);


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

   //objects
  // FreqRef _freqRef;
};
#endif // MXPB4ANT_H
