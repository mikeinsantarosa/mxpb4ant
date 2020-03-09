/* File     : acfdatapoint.h
 * Author   : Mike Purtell
 * Date     : Nov 18, 2017
 *
 * Purpose  :
 *
 * Header file for a
 * freq/level data point
 *
 *
 * */
#ifndef ACFDATAPOINT_H
#define ACFDATAPOINT_H

class ACFDataPoint
{
public:
    ACFDataPoint();
    ACFDataPoint(double freq, double level);

    void setFreq(double value);
    void setLevel(double value);
    double getfreq();
    double getLevel();

private:
    double _freq;
    double _level;
};

#endif // ACFDATAPOINT_H
