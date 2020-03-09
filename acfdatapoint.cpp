/* File     : acfdatapoint.cpp
 * Author   : Mike Purtell
 * Date     : Nove 18, 2017
 *
 * Purpose  :
 *
 * Maintains a frequency
 * vs level data point.
 *
 *
 * */

#include "acfdatapoint.h"

ACFDataPoint::ACFDataPoint()
{
    _freq = 0;
    _level = 0;
}

ACFDataPoint::ACFDataPoint(double freq, double level)
{
    _freq = freq;
    _level = level;
}

void ACFDataPoint::setFreq(double value)
{
    _freq = value;
}

void ACFDataPoint::setLevel(double value)
{
    _level = value;
}

double ACFDataPoint::getfreq()
{
    return _freq;
}

double ACFDataPoint::getLevel()
{
    return _level;
}
