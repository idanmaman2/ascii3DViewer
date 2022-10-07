//
// Created by Idan Maman on 06/10/2022.
//

#ifndef ASCII3DVIEWER_VECTORM_H
#define ASCII3DVIEWER_VECTORM_H


#include <tgmath.h>
#include "objReader.h"

typedef struct vectorM{
    double x ;
    double y;
    double z;

}vectorM;

vectorM createVectorM(vertex p1 , vertex p2 ){
    vectorM  v={
    .x = p2.x - p1.x ,
    .y = p2.y - p1.y,
    .z = p2.z - p1.z
    } ;
    return v;
}

vectorM crossProduct(vectorM v1 , vectorM v2 ){
    vectorM  v={
            .x = v1.y * v2.z - v1.z * v2.y ,
            .y = v1.z * v2.x - v1.x*v2.z,
            .z = v1.x*v2.y - v1.y * v2.x
    } ;
    return v;
}
double dotProduct(vectorM v1 , vectorM v2 ){
    return v1.x*v2.x + v1.y * v2.y + v1.z * v2.z;

}
vertex rotate(vertex vec,vectorM axsis , double angle ){
    angle = angle / 180 * M_PI ;
    double cosa = cos(angle ) , sina = sin(angle);
    double  x = axsis.x, y = axsis.y , z=axsis.z ,x2 = x*x ,y2 =y*y, z2 = z*z;
    double tx = vec.x, ty = vec.y ,tz =vec.z ;
    vertex vecN = {
            .x=(x2*(1-cosa)+ cosa )*tx +  (x*y*(1-cosa)-sina * z )*ty + (x*z*(1-cosa)+y*sina)*tz ,
            .y=(x*y*(1-cosa)+z*sina)*tx + (y2*(1-cosa)+cosa)*ty + (y*z*(1-cosa)-x*sina)*tz ,
           .z= (x*z*(1-cosa)-y*sina)*tx + (y*z*(1-cosa)+x*sina)*ty + (z2*(1-cosa)+cosa)*tz

    };

    return vecN;

}

#endif //ASCII3DVIEWER_VECTORM_H
