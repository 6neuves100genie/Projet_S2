#include "wire.hpp"

Wire::Wire(uint8_t analogPin1)
{
    pinMode(analogPin1, INPUT);
    this->analogPin1 = analogPin1;
}

Wire::~Wire()
{
}
 
int Wire::getCutWires()
{

    int wireValue = analogRead(analogPin1);
    //Serial.println(wireValue);
    uint8_t wireState = 0b0000;


    if (abs(WIRE_0000 - wireValue) <= WIRE_SENSIBILITY)
        wireState = 0b0000;
    else if (abs(WIRE_0001 - wireValue) <= WIRE_SENSIBILITY)
        wireState = 0b0001;
    else if (abs(WIRE_0010 - wireValue) <= WIRE_SENSIBILITY)
        wireState = 0b0010;
    else if (abs(WIRE_0011 - wireValue) <= WIRE_SENSIBILITY)
        wireState = 0b0011;
    else if (abs(WIRE_0100 - wireValue) <= WIRE_SENSIBILITY)
        wireState = 0b0100;
    else if (abs(WIRE_0101 - wireValue) <= WIRE_SENSIBILITY)
        wireState = 0b0101;
    else if (abs(WIRE_0110 - wireValue) <= WIRE_SENSIBILITY)
        wireState = 0b0110;
    else if (abs(WIRE_0111 - wireValue) <= WIRE_SENSIBILITY)
        wireState = 0b0111;
    else if (abs(WIRE_1000 - wireValue) <= WIRE_SENSIBILITY)
        wireState = 0b1000;
    else if (abs(WIRE_1001 - wireValue) <= WIRE_SENSIBILITY)
        wireState = 0b1001;
    else if (abs(WIRE_1010 - wireValue) <= WIRE_SENSIBILITY)
        wireState = 0b1010;
    else if (abs(WIRE_1011 - wireValue) <= WIRE_SENSIBILITY)
        wireState = 0b1011;
    else if (abs(WIRE_1100 - wireValue) <= WIRE_SENSIBILITY)
        wireState = 0b1100;
    else if (abs(WIRE_1101 - wireValue) <= WIRE_SENSIBILITY)
        wireState = 0b1101;
    else if (abs(WIRE_1110 - wireValue) <= WIRE_SENSIBILITY)
        wireState = 0b1110;
    else if (abs(WIRE_1111 - wireValue) <= WIRE_SENSIBILITY)
        wireState = 0b1111;

    return wireState;
}