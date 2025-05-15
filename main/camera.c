#include "camera.h"

static camera_config_t jpcam_config = {
	.pin_pwdn       = PIN_PWDN,
	.pin_reset      = PIN_RESET,
	.pin_xclk       = PIN_XCLK,
	.pin_sccb_sda   = PIN_SIOD,
	.pin_sccb_scl   = PIN_SIOC,
	.pin_d7         = PIN_D7,
	.pin_d6         = PIN_D6,
	.pin_d5         = PIN_D5,
	.pin_d4         = PIN_D4,
	.pin_d3         = PIN_D3,
	.pin_d2         = PIN_D2,
	.pin_d1         = PIN_D1,
	.pin_d0         = PIN_D0,
	.pin_vsync      = PIN_VSYNC,
	.pin_href       = PIN_HREF,
	.pin_pclk       = PIN_PCLK,

	.xclk_freq_hz   = 20000000,
	.ledc_timer     = LEDC_TIMER_0,
	.ledc_channel   = LEDC_CHANNEL_0,
	.pixel_format   = PIXFORMAT_JPEG,
	.frame_size     = FRAMESIZE_XGA,
	.jpeg_quality   = 12,
	.fb_count       = 1,
	/*.grab_mode      = CAMERA_GRAB_WHEN_EMPTY*/
};

esp_err_t jpcam_init(){
	return esp_camera_init(&jpcam_config);
}

esp_err_t jpcam_capture(){
	//capture a frame
	camera_fb_t *fb = esp_camera_fb_get();
	if (!fb) {
		ESP_LOGE("CAMERA", "Frame buffer could not be acquired");
		return ESP_FAIL;
	}

	//return the frame buffer back to be reused
	esp_camera_fb_return(fb);

	return ESP_OK;
}
