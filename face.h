//
// Created by Idan Maman on 09/10/2022.
//

#ifndef ASCII3DVIEWER_FACE_H
#define ASCII3DVIEWER_FACE_H

#include <stdint.h>
#include "DataStructers/vector.h"

typedef struct vertex {
    double x ;
    double y;
    double z ;
} vertex;

typedef struct face {
    size_t sidesNum ;
    size_t * sides_array;
} face ;

typedef  struct  minMax{
    vertex min ;
    vertex max ;
}minMax;

minMax * getMinMaxFace(face fc ,vector * vx){
    minMax * mx = malloc(sizeof(minMax));
    mx->min.x=mx->min.y=mx->min.z=INT32_MAX ;
    mx->max.x=mx->max.y=mx->max.z=INT32_MIN;
    for(size_t i=0 ; i < fc.sidesNum ; i++){
        vertex vi = *((vertex * ) getIndexVector(fc.sides_array[i]-1,vx));

        if(vi.x> mx->max.x) { mx->max.x = vi.x;}
        if(vi.y > mx->max.y) { mx->max.y = vi.y;}
        if(vi.z> mx->max.z) { mx->max.z = vi.z;}
        if(vi.x < mx->min.x) { mx->min.x = vi.x;}
        if(vi.y < mx->min.y) { mx->min.y = vi.y;}
        if(vi.z< mx->min.z) { mx->min.z  = vi.z;}
    }
    return  mx;


}


static void findMinMax(minMax * mx , minMax *  bound){
    if(bound->min.x >  mx->min.x) { bound->min.x = mx->min.x;}
    if(bound->min.y >  mx->min.y) { bound->min.y = mx->min.y;}
    if(bound->min.z >  mx->min.z) { bound->min.z = mx->min.z;}
    if(bound->max.x <   mx->max.x) { bound->max.x = mx->max.x;}
    if(bound->max.y <   mx->max.y) { bound->max.y = mx->max.y;}
    if(bound->max.z <  mx->max.z) { bound->max.z = mx->max.z;}


}


minMax getMinMaxVector(vector * minmax){
    minMax  newBound ;
    newBound.max.x=newBound.max.y=newBound.max.z=INT32_MIN;
    newBound.min.x=newBound.min.y=newBound.min.z=INT32_MAX;
    forEachIter(findMinMax,minmax,&newBound);
    return  newBound ;
}

char rayInsideBound(minMax mx , double  x , double  y){
    return  x <= mx.max.x && y <= mx.max.y && x >= mx.min.x && y >= mx.min.y ;
}




#endif //ASCII3DVIEWER_FACE_H
