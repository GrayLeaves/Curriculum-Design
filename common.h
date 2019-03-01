#ifndef COMMON_H
#define COMMON_H

#include <iostream>
#include <initializer_list>

const QString rsrcPath = ":/images";

namespace cv {
    class Mat;
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

template<class T>
const T Max (const std::initializer_list<T>& lst)
{
    T maxItem = *lst.begin();
    if(lst.size() == 1) return maxItem;
    for(auto beg = lst.begin()+1; beg != lst.end(); ++beg){
        if(*beg > maxItem) maxItem = *beg;
    }
    return maxItem;
}

#endif // COMMON_H
