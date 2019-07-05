#ifndef RANGE_H
#define RANGE_H
#include <QString>
#include <vector>
#include <map>
#include "point.h"
#include <QStringList>

class range
{
public:
    //constructor
    range();
    range(double mi, double ma);
    //public data
    double min, max;
    //method
    bool inrange(double& k);

    void update(double &mi, double &ma);

    static bool inrange(range& temp, double &k);

    static bool inrange(point &Point, std::map<QString, range> &MapRange, QStringList &Label);
};



#endif // RANGE_H
