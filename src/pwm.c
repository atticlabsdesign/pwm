#include "../pwm.h"
#include <avr/io.h>
#include <stdint.h>



#define CLKMODEMSK  0x7

#define SET_CLK_MODE(mode) *channel.TCCRB = (*channel.TCCRB & ~CLKMODEMSK) | mode;
#define FORMAT_WAVE_GEN_MODE(channel) (*channel.a.TCCR & 0x3) | (*channel.b.TCCR & 0x4)


pwmError_t pwm8SetFrequency(pwmCh8_t channel, pwmClkSrc8_t prescaler, uint8_t top){ //this only supports phase correct PWM right now
    pwmError_t error = PWM_OK;

    pwmWavGenMode_t pwmMode = FORMAT_WAVE_GEN_MODE(channel);

    switch (pwmMode) {
        case PWM_PHASE_CORRECT:
        case FAST_PWM:
            *channel.b.TCCR |= prescaler & 0x7;
        break;

        case PWM_PHASE_CORRECT_OCRA:
        case FAST_PWM_OCRA:
            *channel.b.TCCR |= prescaler & 0x7;
            *channel.a.OCR = top;
        break;

        default:
            error = PWM_INVALID_SETTING_COMBINATION;
        break;
    }
    
    return error;
}

pwmError_t pwm8SetDutyCycle(pwmCh8_t channel, uint8_t dutyCycle, bool chOutput){
    pwmError_t error= PWM_OK;

    pwmWavGenMode_t pwmMode = FORMAT_WAVE_GEN_MODE(channel);

    switch (pwmMode) {
        case PWM_PHASE_CORRECT:
        case FAST_PWM:

            if (chOutput) { 
                *channel.b.OCR = dutyCycle;
            } else {
                *channel.b.OCR = dutyCycle;
            }     

        break;

        case PWM_PHASE_CORRECT_OCRA:
        case FAST_PWM_OCRA:

            if (chOutput) { //trying to assign a dutycycle to the TOP count, invalid
                error = PWM_INVALID_SETTING_COMBINATION;
                break;
            } else if (*channel.a.OCR < dutyCycle) {
                error = PWM_INVALID_SETTING_COMBINATION;
            } else {
                *channel.b.OCR = dutyCycle;
            }
            
        break;

        default:
            error = PWM_INVALID_SETTING_COMBINATION;
        break;
    }

    *channel.a.OCR = dutyCycle;
    return error;
}