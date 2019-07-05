#include "range.h"

range::range()
{
    min = max = 0;
}
range::range(double mi, double ma): min(mi), max(ma)
{

}

bool range::inrange(double &k)
{
    if(k >= min && k <= max)return true;

    return false;
}

bool range::inrange(range &temp, double &k)
{
    return temp.inrange(k);
}

bool range::inrange(point &Point, std::map<QString, range> &MapRange, QStringList &Label)
{
    size_t cn = Point.a.size();

    if(Label.size() != cn)return false;

    for(int i = 0; i < cn; ++i)
    {
        auto it = MapRange.find(Label[i]);

        if(it != MapRange.end() && !it->second.inrange(Point.a[i]))
        {
            return false;
        }
    }

    return true;
}

void range::update(double &mi, double &ma)
{
    min = mi;
    max = ma;
}
