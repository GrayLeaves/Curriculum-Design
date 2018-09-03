#ifndef COMMON_H
#define COMMON_H

#include <iostream>

const QString rsrcPath = ":/images";

namespace cv {
    class Mat;
}

template<class T>
inline const T& Max (const T& _X, const T& _Y)
{return (_X > _Y ? _X : _Y);}

template<class T>
inline const T& Min (const T& _X, const T& _Y)
{return (_X < _Y ? _X : _Y);}

// round double to int
inline int Round (const double& x)
{
    if (x > 0.0)
        return static_cast<int>(x + 0.5);
    else
        return static_cast<int>(x - 0.5);
}

template<class T>
void delete_s(T *&ptr)
{
    if(ptr)
    {
        delete ptr;
        ptr = 0;
    }
}

#endif // COMMON_H
