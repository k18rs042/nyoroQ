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

#define SPEED_DEFAULT	50			//デフォルト速度
#define SPEED_LOW		20			//高速速度
#define SPEED_HIGH		80			//低速速度

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
            case WHITE:		//白を検出
				//デフォルト速度で直進
                forward(left_motor, right_motor, SPEED_DEFAULT);
                break;
            case BLACK:		//黒を検出
				//低速速度で直進
                forward(left_motor, right_motor, SPEED_LOW);
                break;
            case RED:		//赤を検出
				//高速速度で直進
                forward(left_motor, right_motor, SPEED_HIGH);
                break;
            default:
                //モータ停止
                stop(left_motor, right_motor);
                break;
        }

		tslp_tsk(100000); /* 100msec */
    }

    //モータ停止
	stop(left_motor, right_motor);
	ext_tsk();
}

