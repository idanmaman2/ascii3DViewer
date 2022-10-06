#include <stdio.h>
#include "objReader.h"

// ____             _   _       ___
//|  _ \  ___ _ __ | |_| |__   |_ _|_ __ ___   __ _  __ _  ___
//| | | |/ _ \ '_ \| __| '_ \   | || '_ ` _ \ / _` |/ _` |/ _ \
//| |_| |  __/ |_) | |_| | | |  | || | | | | | (_| | (_| |  __/
//|____/ \___| .__/ \__|_| |_| |___|_| |_| |_|\__,_|\__, |\___|
//           |_|                                    |___/
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

size_t buffer [100][100];


int main() {
    obj data = getObjData("//Users//idang//CLionProjects//ascii3DViewer//models//pirate-ship-giant.obj");
    forEachIter(printVertex,data.vertexes,NULL);
    forEachIter(printFace,data.faces,data.vertexes);
    return 0;
}
