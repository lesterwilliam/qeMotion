# qeMotion

## Introduction
This project originated from my need make leaning in first-person-shooter games easiert. Games that enable you to lean around corners to peak or take a shot with giving the enemy the smallest possible target, like PlayerUnknown Battleground or Tom Clancy's Six Siege usually implemet leaning with the keyboard keys 'Q' and 'E'. While it is quite the obvious choice to use the keys close to the usual movement-keys 'W'/'A'/'S'/'D', it is quite hard to use if you need to crouch, move and lean at the same time. There just aren't enough fingers.
My idea to the problem was to implement a simple device that tracks my head motion to trigger and hold the respective lean movement.

## Hardware
I tried to keep stuff simple. The essential two parts for this project are the Arduino Pro Micro and an MPU6050 Gyro-/Accelerometer unit. You can use other Arduino boards, but it has to have USB support. The ATmega32u4 does that, which is found of the Arduino Leonardo too for example.
In my implementation are two buttons, two LED to show the virtual press of the 'Q'/'E' keys and four LEDs to show a mode. The LEDs are controlled via bipolar Transistors, either high- or low-side switches. This might not be of importance, I wasn't sure if the Arduino pins can deliver that much current. If you decide to use low-current LEDs you should be fine with driving them directly from the Arduino.
The MPU6050 sensor unit is fixed to the headset, while the Arduino sits on my desk. I went for this approach to minimize weight on my head. Since sensor and Arduino communicate via I2C, which is not meant to be used over such distances, I used an old USB cable which at least has some sort of shielding. Don't know if this was necessairy, but it seems to work perfectly with a cable length of 1.2m (4ft).

## Software
To make development easier and usage more versatile I've implemented multiple modes for the device. Only two of which are actually in use right now, one of which does exactly nothing, and the other is my first try at motion tracking (which works to some degree).
The usual loop() function calls subfunctions in defined time intervals, which can be tuned in the #defines.

This whole software is not nearly perfect nor finished. But I've tried to design it in a way it is the easiest to understand and modify to someones need. If you struggle to understand it's structure and thoughts behind it, feel free to ask me anything. I'd be glad to help!

## Case
I've designed a 3d-printable case for the device. This may not suit you and your print layout, but I'll try to add some photos of my PCB for reference.
There is also a secondary case which doesn't really have to do anything with this sensor. But since my headset (HyperX Cloud 2) has this remote near where the Arduino will sit, I've designed a case that snaps those two together nicely.
