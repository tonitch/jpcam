#pragma once
#include "esp_camera.h"
#include "esp_log.h"

#define PIN_PWDN	32
#define PIN_RESET	-1
#define PIN_XCLK	0
#define PIN_SIOD	26
#define PIN_SIOC	27
#define PIN_D7		37
#define PIN_D6		34
#define PIN_D5		39
#define PIN_D4		36
#define PIN_D3		21
#define PIN_D2		19
#define PIN_D1		18
#define PIN_D0		5
#define PIN_VSYNC	25
#define PIN_HREF	23
#define PIN_PCLK	22


esp_err_t jpcam_init();
esp_err_t jpcam_capture();
