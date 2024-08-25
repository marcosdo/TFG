#ifndef DEFINITIONS_H
#define DEFINITIONS_H

// ===========================
// CAMERA
// ===========================
#define CAMERA_MODEL_AI_THINKER
#if defined(CAMERA_MODEL_AI_THINKER)
  #define PWDN_GPIO_NUM  32
  #define RESET_GPIO_NUM -1
  #define XCLK_GPIO_NUM  0
  #define SIOD_GPIO_NUM  26
  #define SIOC_GPIO_NUM  27

  #define Y9_GPIO_NUM    35
  #define Y8_GPIO_NUM    34
  #define Y7_GPIO_NUM    39
  #define Y6_GPIO_NUM    36
  #define Y5_GPIO_NUM    21
  #define Y4_GPIO_NUM    19
  #define Y3_GPIO_NUM    18
  #define Y2_GPIO_NUM    5
  #define VSYNC_GPIO_NUM 25
  #define HREF_GPIO_NUM  23
  #define PCLK_GPIO_NUM  22

  #define LED_GPIO_NUM   4
#else
  #error "Camera model not selected"
#endif

// ===========================
// MOTOR
// ===========================
#define MOTOR_START 1
#define MOTOR_STOP  0
#define GO_FORWARD  1
#define GO_BACKWARD 0
#define MAX_SPEED   255
#define MIN_SPEED   0
#define MAX_ACCELERATION  3
#define MIN_ACCELERATION  1


#endif