// ++++++++++++++++++++++++++++++++++++++++++++ //
// File: about.cpp
// Description: Provides an about dialog
//
// Date: 2019-03-07
//
//
//
//
// TODO:
//
//
//
// ++++++++++++++++++++++++++++++++++++++++++++ //
#include "about.h"
#include "ui_about.h"


About::About(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::About)
{

    ui->setupUi(this);
    QString versionDate = getRevDate();


    QString labelText = "<html><head/><body>";
    labelText.append("<span style= font-size:10pt;font-weight:500;>Application Name: </span>");
    labelText.append(" ");
    labelText.append("<span style= font-size:10pt;font-weight:300;>");
    labelText.append(QApplication::applicationName());
    labelText.append("</span>");
    //labelText.append("<br/>");

    // version info
    QString ver = QString("%1.%2.%3").arg(MY_MAJOR_VERSION).arg(MY_MEAN_VERSION).arg(MY_MINOR_VERSION);

    labelText.append("<p><span style= font-size:10pt;font-weight:500;>Application Version: </span>");
    labelText.append(" ");
    labelText.append("<span style= font-size:10pt;font-weight:300;>");
    labelText.append(ver);
    labelText.append("</span>");
    //labelText.append(QApplication::applicationVersion());



    labelText.append("<p><span style= font-size:10pt;font-weight:500;>Application Revision Date: </span>");
    labelText.append(" ");
    labelText.append("<span style= font-size:10pt;font-weight:300;>");
    labelText.append(versionDate);

    labelText.append("</span>");

    labelText.append("<p><span style= font-size:10pt;font-weight:500;>Description : </span>");
    labelText.append(" ");
    labelText.append("<span style= font-size:10pt;font-weight:300;>");
    labelText.append("mx + b slope calculator for correction files calculating new antenna correction data by frequency refeence table.");
    labelText.append("</span>");

    labelText.append("<p><span style= font-size:10pt;font-weight:500;>Company : </span>");
    labelText.append(" ");
    labelText.append("<span style= font-size:10pt;font-weight:300;>");
    labelText.append(VENDOR_NAME);
    labelText.append("</span>");

    labelText.append("<p><span style= font-size:10pt;font-weight:500;>Department : </span>");
    labelText.append(" ");
    labelText.append("<span style= font-size:10pt;font-weight:300;>");
    labelText.append(VENDOR_DEPT);
    labelText.append("</span>");

    labelText.append("<p><span style= font-size:10pt;font-weight:500;>Author : </span>");
    labelText.append(" ");
    labelText.append("<span style= font-size:10pt;font-weight:300;>");
    labelText.append("Mike Purtell");
    labelText.append("</span>");


    labelText.append("</p></body></html>");
    ui->lblAbout->setText(labelText);
    ui->lblAbout->setStyleSheet("QLabel { background-color : white; color : black; padding:10px; }");


}

About::~About()
{
    delete ui;
}


void About::on_btnAbout_clicked()
{
    this->close();
}

QString About::getRevDate()
{

    QString result = "";
    result.append(MY_REV_MONTH);
    result.append("/");
    result.append(MY_REV_DAY);
    result.append("/");
    result.append(MY_REV_YEAR);

    return result;

}

//QString About::getAboutLabelMarkup()
//{
//   // QString temp = "<html><head/><body><p><span style=" font-weight:600;">NSAacfGen: </span>version: " + APP_VERSION + "0.9.1</p><p><span style=" font-weight:600;">Description:</span> Normalized Site Attenuation Verification acf Corrections file(s) generator.</p><p><span style=" font-weight:600;">Keysight Technologies</span></p><p><span style=" font-weight:600;">Santa Rosa Hardware Test Center</span></p><p>Date: Nov 18, 2017</p></body></html>";
//    //temp.append()
//}
