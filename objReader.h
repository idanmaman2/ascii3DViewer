//
// Created by Idan Maman on 04/10/2022.
//

#ifndef ASCII3DVIEWER_OBJREADER_H
#define ASCII3DVIEWER_OBJREADER_H

#include <stdio.h>
#include <sys/fcntl.h>
#include "DataStructers/vector.h"

typedef struct vertex {
    float x ;
    float y;
    float z ;
} vertex;

typedef struct face {
    size_t sidesNum ;
    size_t * sides_array;
} face ;

typedef  struct  obj {
    vector * faces ;
    vector * vertexes;
}obj;


struct vertex  *  atov (char * ascii ){
  vertex * ve = (struct vertex * ) malloc(sizeof(struct  vertex));
    char * tokens [3] = {
            strtok(ascii, " \0"),
            strtok(NULL, " \0"),
            strtok(NULL, " \0")
    } ;
    for(int i=0;i<3;i++){
        if(!tokens[i]){
            fprintf(stderr,"bad file 1");
        }
    }
    if(strtok(NULL, ",/0")){
        fprintf(stderr,"bad file 2 ");
    }
    ve ->x = atof(tokens[0]);
    ve->y= atof(tokens[1]);
    ve->z = atof(tokens[2]);
    return ve ;
}

struct face * atofa (char * ascii ){
    face * fe = malloc(sizeof(face));
    fe->sidesNum =  0;
    for(size_t i=0;i<strlen(ascii);i++){
        if(ascii[i] == ' ')
            fe->sidesNum ++;
    }
    fe->sidesNum++;
    fe->sides_array = malloc(sizeof(fe->sidesNum)*sizeof(size_t ));
    char * token = strtok(ascii," \0");
    for(size_t i=0;i<fe->sidesNum && token ;i++){
        fe->sides_array[i]=atoi(token);
        token= strtok(NULL," \0");
    }
    return fe ;
}


obj getObjData(char *  path ){
    vector * vertexes  = createVector(sizeof(vertex));
    vector * faces = createVector(sizeof(face));
    char buffer[1000];;
    FILE* ptr = fopen(path, "r");
    if (NULL == ptr) {
        fprintf("file can't be opened",stderr);
        exit(-2);
    }
    while(fgets(buffer,1000,ptr))
    {
        buffer[strlen(buffer)-1]='\0';
        switch(*buffer){
            case '#' :
                break;
            case 'f' :
                addVector(atofa(buffer+2),faces);
                break;
            case 'v':
                switch(buffer[1]){
                    case ' ':
                        addVector(atov(buffer+2),vertexes);
                        break;
                    case 't':
                        break;
                    default:
                        continue;
                }
                break;
            default:
                continue;
        }
    }
    obj ob;
    ob.vertexes = vertexes;
    ob.faces=faces;
    return ob;

};


#endif //ASCII3DVIEWER_OBJREADER_H
