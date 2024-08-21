#ifndef MYCAMERA_H
#define MYCAMERA_H

#include <esp_camera.h>

#include "definitions.h"

class MyCamera {
private:
  camera_config_t _conf;
  esp_err_t       _err;
  sensor_t      * _sens;

public:
  MyCamera();
  ~MyCamera();

  bool initCamera();
  void finishCamera();

  void startStream();
  camera_fb_t* captureFrame();

  void setResolution(framesize_t resolution);
  void setQuality(int quality);
private:
  void setupLEDFlash(int pin);
};

// ===========================
// Constructor and destructor
// ===========================
MyCamera::MyCamera() {
  // pin configuration
  _conf.ledc_channel  = LEDC_CHANNEL_0;
  _conf.ledc_timer    = LEDC_TIMER_0;
  _conf.pin_d0        = Y2_GPIO_NUM;
  _conf.pin_d1        = Y3_GPIO_NUM;
  _conf.pin_d2        = Y4_GPIO_NUM;
  _conf.pin_d3        = Y5_GPIO_NUM;
  _conf.pin_d4        = Y6_GPIO_NUM;
  _conf.pin_d5        = Y7_GPIO_NUM;
  _conf.pin_d6        = Y8_GPIO_NUM;
  _conf.pin_d7        = Y9_GPIO_NUM;
  _conf.pin_xclk      = XCLK_GPIO_NUM;
  _conf.pin_pclk      = PCLK_GPIO_NUM;
  _conf.pin_vsync     = VSYNC_GPIO_NUM;
  _conf.pin_href      = HREF_GPIO_NUM;
  _conf.pin_sccb_sda  = SIOD_GPIO_NUM;
  _conf.pin_sccb_scl  = SIOC_GPIO_NUM;
  _conf.pin_pwdn      = PWDN_GPIO_NUM;
  _conf.pin_reset     = RESET_GPIO_NUM;

  _conf.xclk_freq_hz  = 20000000;
  _conf.frame_size    = FRAMESIZE_QVGA;
  _conf.pixel_format  = PIXFORMAT_JPEG;

  _conf.grab_mode     = CAMERA_GRAB_LATEST;
  _conf.fb_location   = CAMERA_FB_IN_PSRAM;
  _conf.jpeg_quality  = 10;
  _conf.fb_count      = 4;

  setupLEDFlash(LED_GPIO_NUM);
}
MyCamera::~MyCamera() { }

// ===========================
// Public methods
// ===========================
bool MyCamera::initCamera() {
  // camera init
  if ((_err = esp_camera_init(&_conf)) != ESP_OK) {
    Serial.printf("Camera init failed with error 0x%x", _err);
    return false;
  }
  return true;
}

void MyCamera::finishCamera() {
  esp_camera_deinit();
}

void MyCamera::startStream() {
  sensor_t *s = esp_camera_sensor_get();
  s->set_framesize(s, FRAMESIZE_QVGA);
}

inline camera_fb_t *MyCamera::captureFrame() {
  return esp_camera_fb_get();
}


// ===========================
// Getters and seters
// ===========================
void MyCamera::setResolution(framesize_t resolution) {
  _sens = esp_camera_sensor_get();
  // initial sensors are flipped vertically and colors are a bit saturated
  if (_sens->id.PID == OV3660_PID) {
    _sens->set_vflip(_sens, 1);        // flip it back
    _sens->set_brightness(_sens, 1);   // up the brightness just a bit
    _sens->set_saturation(_sens, -2);  // lower the saturation
  }

  // drop down frame size for higher initial frame rate
  if (_conf.pixel_format == PIXFORMAT_JPEG) {
    _sens->set_framesize(_sens, resolution);
  }
}

void MyCamera::setQuality(int quality) {
  _sens = esp_camera_sensor_get();
  _sens->set_quality(_sens, quality);
}

// ===========================
// Private methods
// ===========================
inline void MyCamera::setupLEDFlash(int pin) {
  ledcAttach(pin, 5000, 8);
}


#endif