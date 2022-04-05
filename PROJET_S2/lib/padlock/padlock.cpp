/**
 * @file padlock.cpp
 * @author Gabriel Rioux (you@domain.com)
 * @brief définition des méthodes de la classe Padlock
 * @version 0.1
 * @date 2022-03-10
 *
 * @copyright Copyright (c) 2022
 *
 */
#include "padlock.hpp"

// positions possibles du jostick et leur valeur associée
#define RIGHT 2
#define LEFT 4
#define UP 1
#define DOWN 3

// constructeur set les pin du joystick
Padlock::Padlock(uint8_t inputX, uint8_t inputY)
{
    pinMode(inputX, INPUT);
    pinMode(inputY, INPUT);
    analogPinX = inputX;
    analogPinY = inputY;
}

/**
 * @brief Destroy the Padlock object
 * 
 */
Padlock::~Padlock(){

}

// Retourne la valeur associée à la position du joystick
int Padlock::getPosition()
{
    if (analogRead(analogPinY) > VAL_POT_HAUT)
    {
        return UP;
    }

    else if (analogRead(analogPinY) < VAL_POT_BAS)
    {
        return DOWN;
    }
    else if (analogRead(analogPinX) < VAL_POT_BAS)
    {
        return LEFT;
    }

    else if (analogRead(analogPinX) > VAL_POT_HAUT)
    {
        return RIGHT;
    }

     else{
    return 0;
  }
  
}