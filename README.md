# Digital-Analog-Converter

## Overview
This project implements the use of an ARM Microcontroller (TM4C123) and an 8-bit R2R circuit that converts digital signals to be played on an analog speaker. One mode in this project is a "piano key mode" where a sine wave is played at different frequencies that would match notes of a piano. Notes are played through 8 different buttons. Circuit verification is executed through the use of in-lab oscilliscopes to test for saw-tooth, triangle, square, and sine waves at 440Hz each.

### Testing R2R Circuit with Oscilliscope
<img src = "https://github.com/jonicmecija/Digital-Analog-Converter/blob/master/images/inlab.png" width = "300">

## Software

There are 8 digital pins that are used for output. We can represent 2^8 different values (0-256) through the combination of switching digital pins on and off. By manipulating these combinations, we can produce different sounds by outputting different voltages from the circuit. Main software is found in the main.c file.

There are 5 modes in the project:

Mode 1: A saw tooth wave is produced by incrementing through a variable 256 times, then setting the output of the pins to 0 and repeating. 

Mode 2: A triangle wave is produced by incrementing from 1-256, then decrementing back to 0 and repeating. 

Mode 3: A square wave is produced by outputting 1s in all digital pins for some time, and then outputting 0s in all pins. 

Mode 4: By incrementing through an array that holds values from 1-256 in a certain order, we produce a sine wave. 

<img src = "https://github.com/jonicmecija/Digital-Analog-Converter/blob/master/images/getSine.png" width = "500">

Mode 5: was a piano key mode where the sine wave was played at different frequencies that would match the notes of a piano.

### Software Flow Chart

<img src = "https://github.com/jonicmecija/Digital-Analog-Converter/blob/master/images/softwareflow.png" width = "500">

## Hardware

### Breadboarded Circuit

The following picture is the R2R circuit that is used to convert digital signals sent from the board to analog signals played through a speaker. This breadboard prototype includes the 4 button configuration for testing purposes.

<img src = "https://github.com/jonicmecija/Digital-Analog-Converter/blob/master/images/R2Rcircuit_.jpg" width = "500">

### Hardware Schematic 

This schematic shows the connections between the TM4C123 Microcontroller, R2R circuit, buttons, and speaker.

<img src = "https://github.com/jonicmecija/Digital-Analog-Converter/blob/master/images/schematic1.png" width = "700">

## Testing and Validation

The following pictures display the successful validation of the R2R circuits in their respective modes showing the saw-tooth, triangle, sqaure, and sine wave at 440 Hz.

### Sawtooth Wave @ 440 Hz
<img src = "https://github.com/jonicmecija/Digital-Analog-Converter/blob/master/images/sawtooth.png" width = "300">

### Triangle Wave @ 440 Hz
<img src = "https://github.com/jonicmecija/Digital-Analog-Converter/blob/master/images/triangle.png" width = "300">

### Sqaure Wave @ 440 Hz
<img src = "https://github.com/jonicmecija/Digital-Analog-Converter/blob/master/images/square.png" width = "300">

### Sine Wave @ 440 Hz
<img src = "https://github.com/jonicmecija/Digital-Analog-Converter/blob/master/images/sine.png" width = "300">



