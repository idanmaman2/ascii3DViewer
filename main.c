#include <stdio.h>
#include "objReader.h"


void printVertex(vertex * ve){
    printf("v : x: %f , y:%f , z:%f\n",ve->x,ve->y,ve->z);
}
void printFace(face * fe,vector * ver){
    for(size_t i=0;i<fe->sidesNum;i++){
        vertex *  vi = getIndexVector(fe->sides_array[i],ver);
        printf("side %d.%d , point : %f %f %f ",i+1 ,fe->sides_array[i],vi->x,vi->y,vi->z);
    }
    printf("\n");
}



int main() {
    obj data = getObjData("//Users//idang//CLionProjects//ascii3DViewer//models//pirate-ship-giant.obj");
    forEachIter(printVertex,data.vertexes,NULL);
    forEachIter(printFace,data.faces,data.vertexes);
    return 0;
}
