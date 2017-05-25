#ifndef JUDGE_H
#define JUDGE_H
#include "point.h"
#include <cmath>
#include <vector>

class xy {
  public:
    double x,y;
    xy() {}
    xy(double _x,double _y):x(_x),y(_y) {}
};

static std::vector<xy> vxy;

void initjudge() {
    double lmax=8.57487,lmin=5.94223,arcmin=2.785;
    int n=100;
    double tempy=lmax,div=(lmax-lmin)/n;
    for(int i=n; i>0; i--,tempy-=div)
        vxy.push_back(xy(6.1-tempy,tempy));
    div=(lmin-arcmin)/n;
    for(int i=n; i>0; i--,tempy-=div)
        vxy.push_back(xy(sqrt(1.84946*1.84946-(tempy-4.63446)*(tempy-4.63446))-1.15,tempy));
}
bool isok(point& temp) {
    double x=temp.a[1],y=temp.a[2],z=temp.a[3],t=temp.a[0];
    if(t<2)return false;
    //judge x,y,z
    if(z>0.3||z<-0.3||x>1||y>8.78701||y<2.485)return true;
    std::vector<xy>::iterator it;
    for(it=vxy.begin(); it!=vxy.end(); ++it) {
        if((x-it->x)*(x-it->x)+(y-it->y)*(y-it->y)+z*z<=0.09)
            return true;
    }
    return false;
}
double getArea(double y) {
    double r;
    if(y<-3.89785)return pi*0.425*0.425;

    if(y<-1.7426) {
        r=-3.89785+(3.18357-0.85)/2.0/(3.89785-1.7426)*(y+3.89785);
        return pi*r*r;
    }
    if(y<0) {
        r=-1.7426+(2-1.592)/(1.7426)*(y+1.7426);
        return pi*r*r;
    }
    if(y<2.785)return pi*4;
    if(y<6.537)return pi*4-pi*0.3*0.3*sqrt(2.0);
    r=sqrt(4-(y-6.537)*(y-6.537));
    if(y<8.0)
        return pi*r*r-pi*0.09*sqrt(2.0);
    return pi*r*r;
}

#endif // JUDGE_H
