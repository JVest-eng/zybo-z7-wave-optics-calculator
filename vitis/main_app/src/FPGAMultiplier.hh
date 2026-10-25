#pragma once

#include "xgpio.h"
#include "xparameters.h"
#include "xstatus.h"
#include "xil_printf.h"
#include "xil_assert.h"
#include "xil_types.h"
/*
Control - channel 1 => reset bit, channel 2 => start bit
Operand - channel 1 => 32 bit packed output
Result - channel 1 => 32 bit input, channel 2 => 2 bit input (busy lsb, done msb)
*/
class FPGAMultiplier {
    private:
        XGpio _operands, 
        _control, 
        _result;
    public:
        FPGAMultiplier();
        bool TryMultiply(uint16_t a, uint16_t b, uint32_t& outValue);
        static float UnsignedToFloat_Q2_30(u32 value) {
            constexpr float scale = 1.0f / static_cast<float>(1 << 30); 
            return static_cast<float>(static_cast<int32_t>(value))*scale;
        }
};