#ifndef MYCAMERA_H
#define MYCAMERA_H

class MyCamera {
private:
  camera_config_t _conf;
  esp_err_t       _err;
  sensor_t      * _sens;

public:
  MyCamera();
  ~MyCamera();

  void setupCamera();
private:
  void setupLEDFlash(int pin);
};

// ===========================
// Constructor and destructor
// ===========================
MyCamera::MyCamera() { }
MyCamera::~MyCamera() { }

// ===========================
// Public methods
// ===========================
void setupCamera() {
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
  _conf.frame_size    = FRAMESIZE_UXGA;
  _conf.pixel_format  = PIXFORMAT_JPEG;

  _conf.grab_mode     = CAMERA_GRAB_WHEN_EMPTY;
  _conf.fb_location   = CAMERA_FB_IN_PSRAM;
  _conf.jpeg_quality  = 12;
  _conf.fb_count      = 1;

  // If PSRAM IC present, init with UXGA resolution and higher JPEG quality
  //                      for larger pre-allocated frame buffer.
  if (_conf.pixel_format == PIXFORMAT_JPEG) {
    if (psramFound()) {
      _conf.jpeg_quality  = 10;
      _conf.fb_count      = 2;
      _conf.grab_mode     = CAMERA_GRAB_LATEST;
    } else {
      // Limit the frame size when PSRAM is not available
      _conf.frame_size  = FRAMESIZE_SVGA;
      _conf.fb_location = CAMERA_FB_IN_DRAM;
    }
  }

  // camera init
  if ((_err = esp_camera_init(&_conf)) != ESP_OK) {
    Serial.printf("Camera init failed with error 0x%x", _err);
    return;
  }

  
  _sens = esp_camera_sensor_get();
  // initial sensors are flipped vertically and colors are a bit saturated
  if (_sens->id.PID == OV3660_PID) {
    _sens->set_vflip(s, 1);        // flip it back
    _sens->set_brightness(s, 1);   // up the brightness just a bit
    _sens->set_saturation(s, -2);  // lower the saturation
  }
  // drop down frame size for higher initial frame rate
  if (config.pixel_format == PIXFORMAT_JPEG) {
    _sens->set_framesize(s, FRAMESIZE_QVGA);
  }

  setupLEDFlash(LED_GPIO_NUM);
}

// ===========================
// Private methods
// ===========================
void MyCamera::setupLEDFlash(int pin) {
  ledcAttach(pin, 5000, 8);
}


#endif