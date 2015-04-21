#include <iostream>
#include <stdio.h>
#include "GPIO.h"
#define KEY_UP 72
#define KEY_DOWN 80
#define KEY_LEFT 75
#define KEY_RIGHT 77


using namespace std;

int main()
{
    GPIO *gpio = GPIO::instance();
    char c = 0;
        float turn = 0;
        float speed = 0;

    gpio->enableHBridge();
    cout << "Control command:" << endl;
    while(1)
    {
        cin.get(c);
        switch(c) {
        case 'w':
            cout << endl << "Up" << endl;//key upa
            speed += 0.05f;
            break;
        case 's':
            cout << endl << "Down" << endl;   // key down
            speed -= 0.05f;
            break;
        case 'a':
            cout << endl << "Right" << endl;  // key right
            turn -= 1;
            break;
        case 'd':
            cout << endl << "Left" << endl;  // key left
            turn += 1;
            break;
        case 'q':
            speed = 0;
            gpio->disableHBridge();
            return 0;
            break;
        case 'r':
            turn = 0;
            break;
        default:
            cout << endl << c << endl;  // key left
            break;
        }
      gpio->setTurn(turn);
      gpio->setSpeed(speed);
      printf("Speed: %0.2f Turn: %0.2f\n",speed,turn);
    }
    return 0;
  
}
