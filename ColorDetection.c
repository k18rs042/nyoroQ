//ColorDetection.c
#include "ColorDetection.h"
#include "ev3api.h"

#define REFLECT_WHITE	95      //白検出用反射光の閾値
#define REFLECT_BLACK	35      //黒検出用反射光の閾値
#define REFLECT_RED		48      //赤検出用反射光の閾値

int midpoint = (REFLECT_WHITE - REFLECT_BLACK) / 2 + REFLECT_BLACK; //62.5

//色検出
int DetectionColor(int color_sensor){

    int reflect = 0;            //反射光の値
    int color = 0;              //検出色
    

    //カラーセンサで反射光の強さを測定
    reflect = ev3_color_sensor_get_reflect(color_sensor);
    printf("reflect = %d\n", reflect);
    


    if(midpoint-8 < reflect && reflect < midpoint+8){
        color = GLAY;
    }else if(reflect >= 95){
        color = WHITE;
    }else if(reflect <= 30){
        color = BLACK;
    }else if(midpoint+8 < reflect){
        color = NEARWHITE;//白
    }else if(reflect < midpoint-8){
        color = NEARBLACK;
    }else{
        color = RED;
    }
    return color;
}