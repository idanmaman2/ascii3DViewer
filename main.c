
// ____             _   _       ___
//|  _ \  ___ _ __ | |_| |__   |_ _|_ __ ___   __ _  __ _  ___
//| | | |/ _ \ '_ \| __| '_ \   | || '_ ` _ \ / _` |/ _` |/ _ \
//| |_| |  __/ |_) | |_| | | |  | || | | | | | (_| | (_| |  __/
//|____/ \___| .__/ \__|_| |_| |___|_| |_| |_|\__,_|\__, |\___|
//           |_|                                    |___/

//the idea is to shoot straight rays from xy and get the min z and map it to ascii and color to determine it is distance
// to get a 3d - depth image so it will be simple - fast - and with tons of stitches .
// the idea comes from something like the moving sticks box that creates 3d shapes
#include <stdio.h>
#include <unistd.h>
#include <sys/termios.h>
#include <sys/poll.h>
#include "objReader.h"
#include "vectorM.h"

#define WIDTH 200
#define  HEIGHT 80
enum bool {false =0 , true = 1 } ;

const char depth [] =".,-~:;=!*#$@%nahsji";
double pushX = -2 ;
double pushY=-1 ;
#define mapLen 18
double  scale =10;
double scaleModel =  0.1;
#define MaxChar 'X'

char buffer [HEIGHT][WIDTH]; //distance from the viewer -> yz plane
char map(double z){
    double  sz = z * scale ;
    return (sz>=0 &&  sz< mapLen) ? depth[(int)sz] : MaxChar ;
}


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

void set_term_quiet_input()
{
    struct termios tc;
    tcgetattr(0, &tc);
    tc.c_lflag &= ~(ICANON | ECHO);
    tc.c_cc[VMIN] = 0;
    tc.c_cc[VTIME] = 0;
    tcsetattr(0, TCSANOW, &tc);
}

double getZFromPlane(vertex p1  , vertex p2  , vertex p3 ,double y , double x){
    vectorM p12 = createVectorM(p1,p2);
    vectorM  p13 = createVectorM(p1,p3);
    vectorM  normal = crossProduct(p12,p13);
    return (-normal.y*y - normal.x*x)/normal.z;

}

short isInsideFace( double y , double x , face f  , vector * vertexes ){

    vertex p0 ={ .x =x , .y = y , .z =0 };
    int len = f.sidesNum ;
    vectorM  v = {.x =0 , .y=0 , .z = 1 } ;
    int sign = 0;
    for(size_t i=0 ;i < len ; i ++ ){
        vectorM veci = createVectorM(*((vertex * )getIndexVector(f.sides_array[i],vertexes)),p0);
       vectorM  veci2 =  createVectorM(*((vertex * )getIndexVector(f.sides_array[(i+1)%len],vertexes)),p0);
        vectorM normal = crossProduct(veci , veci2);
        float dotProd = dotProduct(v,normal);
        if(i==0){
            sign = dotProd > 0 ? 1 : -1;
        }
        if(!((sign < 0 && dotProd < 0) || (sign > 0 && dotProd > 0)) || dotProd == 0 ){
            return false ;
        }
    }
    return  true;


}


typedef struct minVer{
    double y ;
    double x ;
    double min ;
    vector * vertex ;
    double angle ;
    vectorM  axsis ;
}minVer;

void findMin(face * fc , minVer * min){
    if(isInsideFace(min->y , min->x , *fc , min->vertex)){
        vertex p1 = rotate(*((vertex * )getIndexVector(fc->sides_array[0],min->vertex)),min->axsis,min->angle);
        vertex p2= rotate(*((vertex * )getIndexVector(fc->sides_array[1],min->vertex)),min->axsis,min->angle);
        vertex p3 = rotate(*((vertex * )getIndexVector(fc->sides_array[2],min->vertex)),min->axsis,min->angle);
        double val = getZFromPlane(p1,p2,p3,min->y,min->x);
        if(val < min->min)
            min->min=val ;

    }
}





int main() {
    vectorM axsis = {
            .x=0,
            .z=0,
            .y=0
    };
    double spinZ = 0;
    double spinY = 0;
    int angle = 0;
    obj data = getObjData("//Users//idang//CLionProjects//ascii3DViewer//models//cube.obj");
    printf("\x1b[2J");
    struct pollfd pfd = { .fd = 0, .events = POLLIN };
    set_term_quiet_input();




    while (true) {
        if (poll(&pfd, 1, 0)>0) {
            int c = getchar();
            switch(c){
                case 'a':
                    pushX+=1;
                    break;
                case 'd':
                    pushX-=1;
                    break;
                case 'w':
                    pushY+=1;
                    break;
                case 's':
                    pushY-=1;
                    break;
                case 'h':
                    scale*=2;
                    break;
                case 'j':
                    scale /=2;
                    break;
                case 'y':
                    scaleModel *=2;
                    break;
                case 'u':
                    scaleModel /=2;
                    break;
                case '1':
                    axsis.x+=1;
                    break;
                case '2':
                    axsis.x-=1;
                    break;
                case '3':
                    axsis.y+=1;
                    break;
                case '4':
                    axsis.y-=1;
                    break;
                case '5':
                    axsis.z+=1;
                    break;
                case '6':
                    axsis.z-=1;
                    break;

            }
        }



            angle += 1;
            for (size_t i = 0; i < HEIGHT; i++) {
                memset(buffer[i], 0, WIDTH);
            }
            //spinY=2*sin(angle);
            //spinZ=2*cos(angle);
            printf("\x1b[H");
            for (size_t y = 0; y < HEIGHT; y++) {
                for (size_t x = 0; x < WIDTH; x++) {
                    minVer min;
                    min.min = INT32_MAX;
                    min.vertex = data.vertexes;
                    min.y = y * scaleModel + pushY + spinY;
                    min.x = x * scaleModel + pushX + spinZ;
                    min.axsis = axsis;
                    min.angle = angle % 360;

                    forEachIter(findMin, data.faces, &min);
                   putchar(map(min.min));
                }
                putchar(10);
            }
        }


        return 0;
    }
