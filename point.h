#ifndef POINT_H
#define POINT_H
#include <vector>

const double pi=4*atan(1.0);

class point {
  public:
    point();
    point(int c);
    std::vector<double> a;
    bool operator <(point &b) {
        if(a[1]<b.a[1])return true;//x
        if(fabs(a[1]-b.a[1])<1e-8)return a[3]<b.a[3];//z
        return false;
    }
    static double getRad(double &x,double &y);
    static double getDegree(double &x,double &y);
};

#endif // POINT_H
