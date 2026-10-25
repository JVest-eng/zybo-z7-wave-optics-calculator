#include "FPGAMultiplier.hh"
#include <cstdio>
#include <xil_assert.h>

extern "C" void MyAssertHandler(const char8* File, s32 line) {
    xil_printf("Critical Exception: Assertion failed in the file %s at line %d\r\n", File, line);
    //would force system reset here.
} 

int main() {
    Xil_AssertSetCallback(&MyAssertHandler);
    u16 a, b; //q1.15
    FPGAMultiplier mult;
    u32 result;
    xil_printf("Multiply 0.5 and -0.64 \n\r");
    a = 0b0100'0000'0000'0000u; //0.5
    b = 0b1010'1110'0001'0100u; //-0.64
    if(!mult.TryMultiply(a, b, result))
        xil_printf("Failed to multiply %d \n\r", result);
    else {
        //convert q2.30 to float
        float val = mult.UnsignedToFloat_Q2_30(result);
        printf("output: %f\r\n", val); //output should be ~= -0.32 since 0.64 is not easily representable in binary
    }
    a = 54;
    b = 23;
    xil_printf("Multiply %d and %d",a,b);
    if(!mult.TryMultiply(a, b, result))
        xil_printf("Failed to multiply %d \n\r", result);
    else {
        xil_printf("output: %d\r\n", result);
    }
    xil_printf("Multiply 0.5 and 0.9 \n\r");
    a = 0b0100'0000'0000'0000u; //0.5
    b = 0b0111'0011'0011'0011u; //0.9
    if(!mult.TryMultiply(a, b, result))
        xil_printf("Failed to multiply %d \n\r", result);
    else {
        //convert q2.30 to float
        float val = mult.UnsignedToFloat_Q2_30(result);
        printf("output: %f\r\n", val);
    }

    a = 54;
    b = static_cast<uint16_t>(-23);
    xil_printf("Multiply %d and %d",a,b);
    if(!mult.TryMultiply(a, b, result))
        xil_printf("Failed to multiply %d \n\r", result);
    else {
        xil_printf("output: %d\r\n", result);
    }
    return 0;
}
