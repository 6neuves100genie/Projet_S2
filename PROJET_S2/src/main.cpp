/**
 * @file main.cpp
 * @author Shawn Miller (mils2203@usherbrooke.ca)
 * @author Shawn Miller (mils2203@usherbrooke.ca)
 * @author Shawn Miller (mils2203@usherbrooke.ca)
 * @author Shawn Miller (mils2203@usherbrooke.ca)
 * @author Shawn Miller (mils2203@usherbrooke.ca)
 * @author Shawn Miller (mils2203@usherbrooke.ca)
 * @author Shawn Miller (mils2203@usherbrooke.ca)
 * @author Shawn Miller (mils2203@usherbrooke.ca)
 *
 * @brief
 * @version 0.1
 * @date 2022-02-11
 *
 * @copyright Copyright (c) 2022
 *
 */

#include <Arduino.h>
#include <ShiftRegister74HC595.h>
#include <accelerometre.hpp>
#include <memory.hpp>
#include <wire.hpp>
#include <keypad.hpp>
#include <padlock.hpp>
#include <accelerometre.hpp>
#include <stdio.h>

/**  module ID   **/
#define MODULE_WIRES 1
#define MODULE_PADLOCK 2
#define MODULE_MEMORY 3
#define MODULE_KEYPAD 4
#define MODULE_ACCELEROMETRE 5

/**     memory         **/
#define MEMORY_SRCLK_PIN 23 // clock
#define MEMORY_SER_PIN 25   // latch
#define MEMORY_RCLK_PIN 24  // data
#define MEMORY_ANALOG_PIN A3
#define MEMORY_DIGITAL_PIN 22

/**     accelerometre         **/
#define ACCELEROMETRE_X_PIN A5
#define ACCELEROMETRE_Y_PIN A6
#define ACCELEROMETRE_Z_PIN A7

/**     keypad         **/
#define KEYPAD_DIGITAL_SW1 50
#define KEYPAD_DIGITAL_SW2 51
#define KEYPAD_DIGITAL_SW3 52
#define KEYPAD_DIGITAL_SW4 53

/**     padlock         **/
#define PADLOCK_ANALOG_X_PIN A1
#define PADLOCK_ANALOG_Y_PIN A2

/**     wire         **/
#define WIRE_ANALOG_PIN A0

/**     Communication          **/
int indexData;
bool commandeValid;
int idModule;
bool BombeStateFlag;
bool BombeState;
bool LEDStateFlag;
uint8_t LEDState;

enum etatTransition
{
  START_DATA,
  COMMANDE_DATA,
  MEMORY_LED_DATA,
  LED_DATA,
  BOMBE_DATA,
  END_DATA
} EtatTransition;

bool etatTransitionModule(char rxData);
bool sendData(int module, int *tabData, uint8_t tabSize);

/**     memory         **/
uint8_t memoryLevel;
uint8_t memoryNumber;
bool memory_relacheBouton;

/**     accelerometre         **/
int accelerometre_tabValue[3]; // valeur pouvant aller de 0 a 1023

/**     keypad         **/
int keypad_touchePrecedente;
int keypad_touche;
int valueKeypad;
/**     padlock         **/
int padlock_positionPrecedente;
int padlock_position;
int valuePadlock;
/**     wire         **/
int wire_valeurPrecedente;
int wire_valeur;

ShiftRegister74HC595<2> gestionLED(MEMORY_RCLK_PIN, MEMORY_SRCLK_PIN, MEMORY_SER_PIN);
Memory memory(gestionLED, MEMORY_ANALOG_PIN, MEMORY_DIGITAL_PIN);
Accelerometre accelerometre(ACCELEROMETRE_X_PIN, ACCELEROMETRE_Y_PIN, ACCELEROMETRE_Z_PIN);
Keypad keypad(KEYPAD_DIGITAL_SW1, KEYPAD_DIGITAL_SW2, KEYPAD_DIGITAL_SW3, KEYPAD_DIGITAL_SW4);
Padlock padlock(PADLOCK_ANALOG_X_PIN, PADLOCK_ANALOG_Y_PIN);
Wire wire(WIRE_ANALOG_PIN);

/**     loop         **/
enum etatModule
{
  INIT,
  WIRE,
  PADLOCK,
  MEMORY,
  KEYPAD,
  LED
} EtatModule;

void setup()
{
  // put your setup code here, to run once:
  Serial.begin(9600);
  sei();

  // etatTransition
  EtatTransition = START_DATA;
  commandeValid = false;
  idModule = 0;

  // bombe
  BombeStateFlag = false;

  // LED
  LEDStateFlag = false;
  LEDState = 0;

  // memory
  indexData = 0;
  memoryLevel = 0;
  memoryNumber = 10; // 10 = aucun chiffre afficher
  memory_relacheBouton = false;
  memory.MemoryInit();

  // padlock
  padlock_positionPrecedente = 0;

  // keypad
  keypad_touchePrecedente = 0;

  // wire
  wire_valeurPrecedente = wire.getCutWires();

  // loop
  EtatModule = INIT;
}

void loop()
{
  //Serial.println(EtatModule);

  switch (EtatModule)
  {
  case INIT:
    memory.setNumber(10);
    memory.setLevel(1);

    break;
  case WIRE:
    delay(200);
    wire_valeur = wire.getCutWires();
    //Serial.println(wire_valeurPrecedente);
    if (wire_valeurPrecedente != wire_valeur)
    {
      int wireValue[1] = {wire_valeur};
      //Serial.println(wire_valeurPrecedente);
      sendData(MODULE_WIRES, wireValue, 1);
    }
    wire_valeurPrecedente = wire_valeur;

    break;
  case PADLOCK:

    valuePadlock = padlock.getPosition();

    if (valuePadlock && (padlock_positionPrecedente != valuePadlock))
    {
      int padlockValue[1] = {valuePadlock};
      sendData(MODULE_PADLOCK, padlockValue, 1);
    }

    padlock_positionPrecedente = valuePadlock;

    break;

  case MEMORY:

    memory.setNumber(memoryNumber);
    memory.setLevel(memoryLevel);

    if (memory.getSendBTNState() && !memory_relacheBouton)
    {
      memory_relacheBouton = true;
    }
    else if (!memory.getSendBTNState() && memory_relacheBouton)
    {
      memory_relacheBouton = false;
      int memoryValue[1] = {memory.getSwitchState()};
      sendData(MODULE_MEMORY, memoryValue, 1);
    }

    break;
  case KEYPAD:

    valueKeypad = keypad.detecterTouche();

    if (valueKeypad && (keypad_touchePrecedente != valueKeypad))
    {
      int keypadValue[1] = {valueKeypad};
      sendData(MODULE_KEYPAD, keypadValue, 1);
    }
    keypad_touchePrecedente = valueKeypad;

    break;
  case LED:

    uint8_t *DigitalValue = gestionLED.getAll();

    DigitalValue[0] = ((0b01111111 & DigitalValue[0]) | ((LEDState & 0b1000)<< 4)); //led 4
    DigitalValue[1] = ((0b00011111 & DigitalValue[1]) | ((LEDState & 0b0111)<< 5)); //led 3 2 1

    gestionLED.setAll(DigitalValue);

    break;
  }

  if (accelerometre.getStateFlag())
  {
    accelerometre.setStateFlag(false);

    accelerometre_tabValue[0] = accelerometre.getX_value();
    accelerometre_tabValue[1] = accelerometre.getY_value();
    accelerometre_tabValue[2] = accelerometre.getZ_value();

     sendData(MODULE_ACCELEROMETRE, accelerometre_tabValue, 3);
  }

  if (BombeStateFlag)
  {
    BombeStateFlag = false;

    // voir avec team ce qui ce passe

    delay(2000);

    EtatModule = INIT;
  }
}

/**
 * Possibiliter de reception
 *  -<0,> //initialiser le board
 *  -<1,> //wire
 *  -<2,> //Padlock
 *  -<3,number,level,> //Memory   'I' pour rien afficher sur le 7 segments
 *  -<4,> //Keypad
 *  -<6,etat_LED_module,> //LED 0b1000 = module 4/0b0100 = module 3/0b0010 = module 2/0b0001 = module 1
 *  -<7,etat_moteur,> //moteur vibrant 0 = OFF/1 = ON
 *
 * Possibiliter de d'envoie
 *  -<1,cut_wire,> //wire
 *  -<2,padluck_Xvalue,padloc_Yvalue,> //Padlock
 *  -<3,memory_switch_value,> //Memory
 *  -<4,keypad_digital_value,> //Keypad
 *  -<5,accelerometre_analog_Xvalue,accelerometre_analog_Yvalue,accelerometre_analog_Zvalue,> //accelerometre
 */

/**
 * @brief lorsqu'une commande est recus, serialEvent est automatiquement appelee
 *
 */
void serialEvent()
{
  while (Serial.available())
  {
    etatTransitionModule((char)Serial.read());
  }
}

/**
 * @brief fonction qui traite la donnee recus
 *
 * @param rxData
 * @return true
 * @return false
 */
bool etatTransitionModule(char rxData)
{

  switch (EtatTransition)
  {
  case START_DATA:
    if (rxData == '<')
    {
      EtatTransition = COMMANDE_DATA;
      indexData = 0;
    }
    else
    {
      EtatTransition = START_DATA;
      Serial.print("ERROR OF COMMUNICATION\n\r");
      commandeValid = false;
    }
    return 0;
    break;

  case COMMANDE_DATA:

    if (('0' <= rxData) && (rxData <= '7'))
    {
      idModule = rxData - '0';
      // Serial.println(idModule);
    }
    else if (rxData == ',')
    {
      if (idModule == 3)
        EtatTransition = MEMORY_LED_DATA;
      else if (idModule == 6)
        EtatTransition = LED_DATA;
      else if (idModule == 7)
        EtatTransition = BOMBE_DATA;
      else if ((idModule != 7) && (idModule != 6) && (idModule != 3))
        EtatTransition = END_DATA;
    }
    else
    {
      EtatTransition = START_DATA;
      Serial.print("COMMANDE NOT AVAILABLE\n\r");
      commandeValid = false;
    }
    return 0;
    break;

  case MEMORY_LED_DATA:

    if ((indexData == 0) && ((rxData >= '0') && (rxData <= '9')))
      memoryNumber = rxData - '0';
    if ((indexData == 0) && (rxData == 'I'))
      memoryNumber = 10;
    else if ((indexData == 2) && ((rxData >= '1') && (rxData <= '5')))
      memoryLevel = rxData - '0';
    else if ((indexData == 3) && (rxData == ','))
      EtatTransition = END_DATA;
    else if ((indexData == 1) && (rxData != ','))
      EtatTransition = START_DATA;

    indexData++;

    return 0;
    break;

  case LED_DATA:
    char tabLEDData[2];

    if ((rxData >= '0') && (rxData <= '9'))
      tabLEDData[indexData++] = rxData;
    else if (rxData == ',')
    {
      LEDState = atoi(tabLEDData);
      EtatTransition = END_DATA;
    }
    else
      EtatTransition = START_DATA;

    return 0;
    break;

  case BOMBE_DATA:

    if ((rxData == '0') || (rxData == '1'))
    {
      BombeState = rxData - '0';
      BombeStateFlag = true;
    }
    else if (rxData == ',')
      EtatTransition = END_DATA;
    else
    {
      BombeStateFlag = false;
      EtatTransition = START_DATA;
      Serial.println("ERROR OF COMMUNICATION\n\r");
      commandeValid = false;
    }

    return 0;
    break;

  case END_DATA:

    if (rxData == '>')
    {
      commandeValid = true;
    }
    else
    {
      Serial.println("ERROR OF COMMUNICATION\n\r");
      commandeValid = false;
    }

    if (commandeValid)
    {
      if (idModule == 0)
        EtatModule = INIT;
      else if (idModule == 1)
        EtatModule = WIRE;
      else if (idModule == 2)
        EtatModule = PADLOCK;
      else if (idModule == 3)
        EtatModule = MEMORY;
      else if (idModule == 4)
        EtatModule = KEYPAD;
      else if (idModule == 6)
        EtatModule = LED;
    }

    EtatTransition = START_DATA;

    return 0;
    break;
  }

  return 0;
}

/**
 * @brief fonction qui envoie une commande par le port serie
 *
 * @param module
 * @param tabData
 * @param tabSize
 * @return true
 * @return false
 */
bool sendData(int module, int *tabData, uint8_t tabSize)
{

  char txData[30];
  int index = 0;

  txData[index++] = '<';
  txData[index++] = module + '0';

  for (int i = 0; i < tabSize; i++)
  {
    txData[index++] = ',';
    char tempTab[15];

    int sizeTempTab = sprintf(tempTab, "%d", tabData[i]);

    for (int j = 0; j < sizeTempTab; j++)
    {
      txData[index++] = tempTab[j];
    }
  }

  txData[index++] = ',';
  txData[index++] = '>';
  txData[index] = '\0';

  Serial.print(txData);
  Serial.println();

  return 0;
}