#ifndef MATHMETHOD_H
#define MATHMETHOD_H



template <class vectorType>
double average(vectorType& c)
{
    double ans = 0;

    for(auto i = c.begin(); i != c.end(); ++i)
    {
        ans += *i;
    }

    return ans / c.size();
}

template<class vectorType>
double variance(vectorType& c)
{
    double ans = 0;
    double ave = average(c);

    for(auto i = c.begin(); i != c.end(); ++i)
    {
        ans += (*i - ave) * (*i - ave);
    }

    return ans / c.size();
}



#endif // MATHMETHOD_H
