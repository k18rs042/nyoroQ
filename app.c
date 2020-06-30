// app.c
#include "ev3api.h"
#include "app.h"
#include "RunControl.h"
#include "ColorDetection.h"

#define DEBUG

#ifdef DEBUG
#define _debug(x) (x)
#else
#define _debug(x)
#endif

#define SPEED_DEFAULT	35			//デフォルト速度
#define SPEED_LOW		20			//高速速度
#define SPEED_HIGH		80			//低速速度


#define SPEED_TURN_LOW_S   30      //軽微なカーブ  
#define SPEED_TURN_LOW_L   40     //S=>small L=>LARGE

#define SPEED_TURN_HIGH_S   5     //強引なカーブ
#define SPEED_TURN_HIGH_L   70     //S=>small L=>LARGE 




//モータ・センサのポート
const int color_sensor = EV3_PORT_1, left_motor = EV3_PORT_A, right_motor = EV3_PORT_B;

void main_task(intptr_t unused) {
    

	static int rotate = 0;			//モータ回転角

	//モータ・センサのポート設定
    ev3_sensor_config(color_sensor, COLOR_SENSOR);
    ev3_motor_config(left_motor, LARGE_MOTOR);
    ev3_motor_config(right_motor, LARGE_MOTOR);

    syslog(LOG_NOTICE, "#### motor control start");
    while(1) {

		//検出した色ごとにモータ速度を変化
        switch(DetectionColor(color_sensor)){
            case GLAY:		//灰色を検出
				//デフォルト速度で直進
                forward(left_motor, right_motor, SPEED_DEFAULT);
                break;
            case BLACK:		//黒を検出
				//右に曲がる
                turn(left_motor, right_motor, SPEED_TURN_HIGH_L,SPEED_TURN_HIGH_S);
                break;
            case NEARBLACK:		//黒に近い色を検出
				//右に曲がる
                turn(left_motor, right_motor, SPEED_TURN_LOW_L,SPEED_TURN_LOW_S);
                break;
            case WHITE:		//白を検出
				//右に曲がる
                turn(left_motor, right_motor, SPEED_TURN_HIGH_S,SPEED_TURN_HIGH_L);
                break;
            case NEARWHITE://白に近い色を検知
                turn(left_motor, right_motor, SPEED_TURN_LOW_S,SPEED_TURN_LOW_L);
                break;
            case RED:
                forward(left_motor, right_motor, SPEED_DEFAULT);
                break;
            default :
                stop(left_motor, right_motor);
                break;
        }

		tslp_tsk(100000); /* 100msec */
    }

    //モータ停止
	stop(left_motor, right_motor);
	ext_tsk();
}

