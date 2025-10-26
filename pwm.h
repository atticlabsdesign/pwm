#ifndef PWM_H
#define PWM_H

#include <stdint.h>
#include "../pinctrl/pinctrl.h"

//this is currently meant for atmega 328p for now
// #ifdef __AVR_ATmega328P__

// #endif

typedef enum {
    PWM_OK,
    PWM_FREQ_TOO_HIGH,
    PWM_INVALID_SETTING_COMBINATION,
    PWM_UNSUPPORTED_SETTING,
} pwmError_t;

typedef enum { //high bit is in TCCRB
    // NORMAL                  = 0x0,
    PWM_PHASE_CORRECT       = 0x1,
    // CTC                     = 0x2,
    FAST_PWM                = 0x3,
    PWM_PHASE_CORRECT_OCRA  = 0x5,
    FAST_PWM_OCRA           = 0x7,
} pwmWavGenMode_t;

typedef enum {
    PWM_DISABLED,
    PWM_INT_CLK,
    PWM_INT_CLK_8,
    PWM_INT_CLK_64,
    PWM_INT_CLK_256,
    PWM_INT_CLK_1024,
    PWM_EXT_CLK_NEGEDGE,
    PWM_EXT_CLK_POSEDGE,

} pwmClkSrc8_t;



struct pwmCh8SubChannel{
    volatile uint8_t *TCCR;
    volatile uint8_t *OCR;
    pinId_t OCPin;
};


//this needs to change eventually
typedef struct {
    volatile struct pwmCh8SubChannel a;
    volatile struct pwmCh8SubChannel b;
    volatile uint8_t *TCNT;
    volatile uint8_t *TIMSK;
    volatile uint8_t *TIFR;
    
} pwmCh8_t;

pwmError_t pwm8InitCh(pwmCh8_t channel, pwmWavGenMode_t mode);
pwmError_t pwm8SetFrequency(pwmCh8_t channel, pwmClkSrc8_t prescaler, uint8_t top);
pwmError_t pwm8SetDutyCycle(pwmCh8_t channel, uint8_t dutyCycle, bool chOutput);

#endif