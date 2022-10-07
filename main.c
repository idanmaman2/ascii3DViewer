
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
#define  HEIGHT 150
enum bool {false =0 , true = 1 } ;

const char depth [] =".,-~:;=!*#$@";
double pushX = -2 ;
double pushY=-1 ;
#define mapLen 12
double  scale =1;
double scaleModel =  0.1;
#define Maxcolor WHITE
#define GREEN 32
#define RED 31
#define YELLOW 33
#define  PURPLE 35
#define CYAN 36
#define WHITE 37

double  denX =0.1;
double  denY = 0.1 ;
const short colors [] = {GREEN , RED , YELLOW ,PURPLE ,CYAN};
#define  colorLen 5

#define MaxChar ' '
void changeColor(int color ) {
    printf("\033[0;%dm",color);
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
        if(!((sign < 0 && dotProd < 0) || (sign > 0 && dotProd > 0)) ){
            return false ;
        }
    }
    return  true;


}
typedef struct angleAxsis{
    double angle ;
    vectorM axsis ;
};
typedef struct minVer{
    double y ;
    double x ;
    double min ;
    short changed ;
    vector * vertex ;
}minVer;
char map(minVer z){
    double  sz = z.min * scale ;
    return !z.changed  ? MaxChar : depth[(int)fmin(abs(sz),mapLen-1)];
}
char mapColor(minVer z){
    double  sz = z.min * scale ;
    return !z.changed ? Maxcolor : colors[(int)fmin(abs(sz),colorLen-1)];
}
void findMin(face * fc , minVer * min){
    if(isInsideFace(min->y , min->x , *fc , min->vertex)){
        vertex p1 = *((vertex * )getIndexVector(fc->sides_array[0],min->vertex));
        vertex p2= *((vertex * )getIndexVector(fc->sides_array[1],min->vertex));
        vertex p3 =*((vertex * )getIndexVector(fc->sides_array[2],min->vertex));
        double val = getZFromPlane(p1,p2,p3,min->y,min->x);
        val  = abs(val);
        if(!min->changed ||(val >= 0 &&val < min->min))
        {
            min->min=val ;
            min->changed = true;
        }


    }
}
void rotateVertexes(vertex * data , struct angleAxsis * ax ){
    *data = rotate(*data ,ax->axsis,ax->angle );

}

void scaleModelI(vertex * data , double * scale){
    data->x*=*scale;
    data->y*=*scale;
    data->z*=*scale;
}


int main() {
    struct angleAxsis ax2 ;
    vectorM axsis = {
            .x=0,
            .z=0,
            .y=0
    };
    double angle = 0;
    obj data = getObjData("//Users//idang//CLionProjects//ascii3DViewer//models//mr-president.obj");
    printf("\x1b[2J");
    struct pollfd pfd = { .fd = 0, .events = POLLIN };
    set_term_quiet_input();
    angle = 1 ;
    while (true) {
        if (poll(&pfd, 1, 0)>0) {
            int c = getchar();
            switch(c){
                case 'a':
                    pushX+=10;
                    break;
                case 'd':
                    pushX-=10;
                    break;
                case 'w':
                    pushY+=10;
                    break;
                case 's':
                    pushY-=10;
                    break;
                case 'h':
                    scaleModel= 1.5;
                    forEachIter(scaleModelI,data.vertexes,&scaleModel);
                    break;
                case 'j':
                    scaleModel= 1/1.5;
                    forEachIter(scaleModelI,data.vertexes,&scaleModel);
                    break;
                case 'y':
                    scale *=1.1;
                    break;
                case 'u':
                    scale /=1.1;
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
                case '0':
                    angle =0;
                    break;
                case '[':
                    denX/=10;
                    break;
                case ']':
                    denX*=10;
                    break;
                case 'f':
                    ax2.angle = 30 ;
                    ax2.axsis.x=0;
                    ax2.axsis.y=1;
                    ax2.axsis.z=0;
                    forEachIter(rotateVertexes,data.vertexes,&ax2);
                    angle = 0 ;
                    break;
                case 'r':
                    ax2.angle = 30 ;
                    ax2.axsis.x=1;
                    ax2.axsis.y=0;
                    ax2.axsis.z=0;
                    forEachIter(rotateVertexes,data.vertexes,&ax2);
                    angle = 0 ;
                    break;
                    case 't':
                    ax2.angle = 30 ;
                    ax2.axsis.x=0;
                    ax2.axsis.y=0;
                    ax2.axsis.z=1;
                    forEachIter(rotateVertexes,data.vertexes,&ax2);
                    angle = 0 ;
                    break;



            }
        }

        struct angleAxsis ax ;
        ax.angle = angle ;

        ax.axsis = axsis;
        forEachIter(rotateVertexes,data.vertexes,&ax);
        printf("\x1b[H");
        for (double y = 0; y < HEIGHT*denX; y+=denX) {
            for (double x = 0; x < WIDTH*denY; x+=denY) {
                minVer min;
                min.min = -1;
                min.vertex = data.vertexes;
                min.changed = false ;
                min.y = y + pushY ;
                min.x = x  + pushX ;
                forEachIter(findMin, data.faces, &min);
                changeColor(mapColor(min));
                printf("%3c",map(min));
                fflush(stdout);
            }
            putchar(10);
        }
    }


    return 0;
}