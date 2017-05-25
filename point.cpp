#include "point.h"

point::point() {

}
point::point(int c) {
    a.resize(c);
}

double point::getRad(double &x, double &y) {
    if(x>=0)
        return atan(y/x);
    if(y>=0)
        return pi-atan(-y/x);
    return -(pi-atan(y/x));
}

double point::getDegree(double &x, double &y) {
    return getRad(x,y)/pi*180;
}
