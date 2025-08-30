#ifndef CONFIG_H
#define CONFIG_H

#define MOTOR_LEFT_IN1 25
#define MOTOR_LEFT_IN2 26
#define MOTOR_RIGHT_IN1 13
#define MOTOR_RIGHT_IN2 27

#define MOTOR_FREQUENCY 50000
#define MOTOR_RESOLUTION 8


#define ENCODER_LEFT_CHA 35
#define ENCODER_LEFT_CHB 34
#define ENCODER_RIGHT_CHA 32
#define ENCODER_RIGHT_CHB 33

#define IR_XSHUT_1 16
#define IR_XSHUT_2 5
#define IR_XSHUT_3 4

#define TICKS_PER_CELL 1700
#define TICKS_90_DEGREES_LEFT 840
#define TICKS_90_DEGREES_RIGHT 840
#define TICKS_180_DEGREES 1850

// #define KP_BAL 0.34
// #define KP_DIST 0.002

#define ROW  24
#define COL  24   // maze size
#define CELL  30  // pixel size of each cell
#define endr  16
#define endc  16

#define FRONT_WALL_THRESHOLD 160
#define SIDE_WALL_THRESHOLD 160

#define FRONT_ERROR 11
#define LEFT_ERROR  18
#define RIGHT_ERROR 19

#endif
