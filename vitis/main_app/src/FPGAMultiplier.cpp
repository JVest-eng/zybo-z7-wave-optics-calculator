#include "FPGAMultiplier.hh"
#include <sys/_stdint.h>

FPGAMultiplier::FPGAMultiplier() {
    XGpio_Initialize(&_operands, XPAR_OPERAND_0_BASEADDR);
    XGpio_SetDataDirection(&_operands, 1, 0x0);
    XGpio_Initialize(&_control, XPAR_CONTROL_0_BASEADDR);
    XGpio_SetDataDirection(&_control, 1, 0x0);
    XGpio_SetDataDirection(&_control, 2, 0x0);
    XGpio_Initialize(&_result, XPAR_RESULT_0_BASEADDR);
    XGpio_SetDataDirection(&_result, 1, 0xFFFFFFFF);
    XGpio_SetDataDirection(&_result, 2, 0b11);
}

bool FPGAMultiplier::TryMultiply(uint16_t a, uint16_t b, uint32_t &outValue)  {

    outValue = 0;
    XGpio_DiscreteWrite(&(this->_control), 2, 0u); //deassert start pin

    XGpio_DiscreteWrite(&(this->_control), 1, 0b1u); //reset the multiplier RTL
    XGpio_DiscreteWrite(&(this->_control), 1, 0b0u);

    //confirm that old completion is clear.
    if(int c = XGpio_DiscreteRead(&(this->_result), 2) & 0b10u) {
        outValue = 1; //flag to say hey, read output is not 1.
        return false;
    }
    //pack and write product to operands.
    XGpio_DiscreteWrite(&(this->_operands), 1, (static_cast<uint32_t>(a) << 16) | b);

    XGpio_DiscreteWrite(&(this->_control), 2, 0b1u); //start multiplier.
    XGpio_DiscreteWrite(&(this->_control), 2, 0b0u); //deassert start pin.


    //poll until complete.
    for (u32 i = 0; i < 1000; i++) {
        const u32 status = XGpio_DiscreteRead(&this->_result, 2);
        if((status & 0b10u)) {
            outValue = XGpio_DiscreteRead(&this->_result, 1);
            XGpio_DiscreteWrite(&(this->_control), 2, 0b0u); //deassert start pin
            return true;
        }
    }
    return false;
}