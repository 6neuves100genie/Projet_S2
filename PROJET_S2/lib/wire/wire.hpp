/**
 * @file memory.h
 * @author Shawn Miller (mils2203@usherbrooke.ca)
 * @brief 
 * @version 0.1
 * @date 2022-02-11
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#ifndef WIRE_H_
#define WIRE_H_
 
#include <Arduino.h>

#define WIRE_0000 0
#define WIRE_0001 100
#define WIRE_0010 208
#define WIRE_0011 303
#define WIRE_0100 372
#define WIRE_0101 458
#define WIRE_0110 543
#define WIRE_0111 625
#define WIRE_1000 599
#define WIRE_1001 667
#define WIRE_1010 731
#define WIRE_1011 795
#define WIRE_1100 819
#define WIRE_1101 880
#define WIRE_1110 933
#define WIRE_1111 991
#define WIRE_SENSIBILITY 7
 
class Wire 
{
public:
    Wire(uint8_t analogPin1); 
    ~Wire();
 
    int getCutWires();
 
private:
    uint8_t analogPin1;
};

#endif /* WIRE_H_ */
