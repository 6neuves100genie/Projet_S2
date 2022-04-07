/**
 * @file memory.c
 * @author Shawn Miller (mils2203@usherbrooke.ca)
 * @brief
 * @version 0.1
 * @date 2022-02-11
 *
 * @copyright Copyright (c) 2022
 *
 */
#include "memory.hpp"

uint8_t send_pin;
uint8_t switch_pin;
bool stateSWFlag;

/**
 * @brief Construct a new Memory:: Memory object
 *
 * @param _gestionLED
 * @param switchReadPin
 * @param sendReadPin
 */
Memory::Memory(ShiftRegister74HC595<2> *_gestionLED, uint8_t analogReadPin, uint8_t sendReadPin)
{
    gestionLED = _gestionLED;

    pinMode(analogReadPin, INPUT);

    pinMode(sendReadPin, INPUT);
    
    send_pin = sendReadPin;
    switch_pin = analogReadPin;
    stateSWFlag = false;
}

/**
 * @brief Destroy the Memory:: Memory object
 *
 */
Memory::~Memory()
{
}

/**
 * @brief affiche les possibilite possible pour le 7 segment et le bargraph
 *
 */
void Memory::MemoryInit()
{
    setNumber(0);
    delay(DELAY_INIT_MS);

    setNumber(1);
    delay(DELAY_INIT_MS);

    setNumber(2);
    delay(DELAY_INIT_MS);

    setNumber(3);
    delay(DELAY_INIT_MS);

    setNumber(4);
    delay(DELAY_INIT_MS);

    setNumber(5);
    delay(DELAY_INIT_MS);

    setNumber(6);
    delay(DELAY_INIT_MS);

    setNumber(7);
    delay(DELAY_INIT_MS);

    setNumber(8);
    delay(DELAY_INIT_MS);

    setNumber(9);
    delay(DELAY_INIT_MS);

    setNumber(10);
    delay(DELAY_INIT_MS);

    setLevel(1);
    delay(DELAY_INIT_MS);

    setLevel(2);
    delay(DELAY_INIT_MS);

    setLevel(3);
    delay(DELAY_INIT_MS);

    setLevel(4);
    delay(DELAY_INIT_MS);

    setLevel(5);
    delay(DELAY_INIT_MS);

    setLevel(1);
    delay(DELAY_INIT_MS);
}

/**
 * @brief set le 7 segment selon le niveau en parametre
 *
 * @param level
 */
void Memory::setNumber(uint8_t number)
{
    uint8_t *DigitalValue = gestionLED->getAll();
    
    //Serial.print(DigitalValue[0]);
    //Serial.print(DigitalValue[1]);

    if (number == 0)
        DigitalValue[SEGMENT_ID] = ((SEGMENT_MASQUE & DigitalValue[SEGMENT_ID]) | SEGMENT_0);
    else if (number == 1)
        DigitalValue[SEGMENT_ID] = ((SEGMENT_MASQUE & DigitalValue[SEGMENT_ID]) | SEGMENT_1);
    else if (number == 2)
        DigitalValue[SEGMENT_ID] = ((SEGMENT_MASQUE & DigitalValue[SEGMENT_ID]) | SEGMENT_2);
    else if (number == 3)
        DigitalValue[SEGMENT_ID] = ((SEGMENT_MASQUE & DigitalValue[SEGMENT_ID]) | SEGMENT_3);
    else if (number == 4)
        DigitalValue[SEGMENT_ID] = ((SEGMENT_MASQUE & DigitalValue[SEGMENT_ID]) | SEGMENT_4);
    else if (number == 5)
        DigitalValue[SEGMENT_ID] = ((SEGMENT_MASQUE & DigitalValue[SEGMENT_ID]) | SEGMENT_5);
    else if (number == 6)
        DigitalValue[SEGMENT_ID] = ((SEGMENT_MASQUE & DigitalValue[SEGMENT_ID]) | SEGMENT_6);
    else if (number == 7)
        DigitalValue[SEGMENT_ID] = ((SEGMENT_MASQUE & DigitalValue[SEGMENT_ID]) | SEGMENT_7);
    else if (number == 8)
        DigitalValue[SEGMENT_ID] = ((SEGMENT_MASQUE & DigitalValue[SEGMENT_ID]) | SEGMENT_8);
    else if (number == 9)
        DigitalValue[SEGMENT_ID] = ((SEGMENT_MASQUE & DigitalValue[SEGMENT_ID]) | SEGMENT_9);
    else if (number == 10)
        DigitalValue[SEGMENT_ID] = ((SEGMENT_MASQUE & DigitalValue[SEGMENT_ID]) | SEGMENT_INITIALE);

    gestionLED->setAll(DigitalValue);
}

/**
 * @brief set le bar graph selon le niveau en parametre
 *
 * @param number
 */
void Memory::setLevel(uint8_t level)
{
    uint8_t *DigitalValue = gestionLED->getAll();

    if (level == 1)
        DigitalValue[BARGRAPH_ID] = ((NIVEAU_MASQUE & DigitalValue[BARGRAPH_ID]) | NIVEAU_1);
    else if (level == 2)
        DigitalValue[BARGRAPH_ID] = ((NIVEAU_MASQUE & DigitalValue[BARGRAPH_ID]) | NIVEAU_2);
    else if (level == 3)
        DigitalValue[BARGRAPH_ID] = ((NIVEAU_MASQUE & DigitalValue[BARGRAPH_ID]) | NIVEAU_3);
    else if (level == 4)
        DigitalValue[BARGRAPH_ID] = ((NIVEAU_MASQUE & DigitalValue[BARGRAPH_ID]) | NIVEAU_4);
    else if (level == 5)
        DigitalValue[BARGRAPH_ID] = ((NIVEAU_MASQUE & DigitalValue[BARGRAPH_ID]) | NIVEAU_5);

    gestionLED->setAll(DigitalValue);
}

/**
 * @brief
 *
 * @return true
 * @return false
 */
bool Memory::getSendBTNState()
{
    return digitalRead(send_pin);
}

/**
 * @brief
 *
 * @param state
 */
void Memory::setSendBTNState(bool state)
{
    stateSWFlag = state;
}

/**
 * @brief
 *
 * @return uint16_t
 */
int Memory::getSwitchState()
{
    int switchValue = analogRead(switch_pin);
    //Serial.println(switchValue);
    uint8_t switchState = 0b0000;

    if (abs(SWITCH_0000 - switchValue) <= SWITCH_SENSIBILITY)
        switchState = 0b0000;
    else if (abs(SWITCH_0001 - switchValue) <= SWITCH_SENSIBILITY)
        switchState = 0b0001;
    else if (abs(SWITCH_0010 - switchValue) <= SWITCH_SENSIBILITY)
        switchState = 0b0010;
    else if (abs(SWITCH_0011 - switchValue) <= SWITCH_SENSIBILITY)
        switchState = 0b0011;
    else if (abs(SWITCH_0100 - switchValue) <= SWITCH_SENSIBILITY)
        switchState = 0b0100;
    else if (abs(SWITCH_0101 - switchValue) <= SWITCH_SENSIBILITY)
        switchState = 0b0101;
    else if (abs(SWITCH_0110 - switchValue) <= SWITCH_SENSIBILITY)
        switchState = 0b0110;
    else if (abs(SWITCH_0111 - switchValue) <= SWITCH_SENSIBILITY)
        switchState = 0b0111;
    else if (abs(SWITCH_1000 - switchValue) <= SWITCH_SENSIBILITY)
        switchState = 0b1000;
    else if (abs(SWITCH_1001 - switchValue) <= SWITCH_SENSIBILITY)
        switchState = 0b1001;
    else if (abs(SWITCH_1010 - switchValue) <= SWITCH_SENSIBILITY)
        switchState = 0b1010;
    else if (abs(SWITCH_1011 - switchValue) <= SWITCH_SENSIBILITY)
        switchState = 0b1011;
    else if (abs(SWITCH_1100 - switchValue) <= SWITCH_SENSIBILITY)
        switchState = 0b1100;
    else if (abs(SWITCH_1101 - switchValue) <= SWITCH_SENSIBILITY)
        switchState = 0b1101;
    else if (abs(SWITCH_1110 - switchValue) <= SWITCH_SENSIBILITY)
        switchState = 0b1110;
    else if (abs(SWITCH_1111 - switchValue) <= SWITCH_SENSIBILITY)
        switchState = 0b1111;

    return switchState;
}