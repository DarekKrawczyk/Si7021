# Si7021 library

This is library for **Si7021** module. It has been created for RP2040 microcontroller placed inside Raspberry Pi Pico. Library is written in C++ and it uses Pico SDK **([link](https://www.raspberrypi.com/documentation/pico-sdk/index_doxygen.html#index))**. Si7021 is a temperature and humidity sensor, with I2C interface. Library was created based on IC documentation **([link](https://www.silabs.com/documents/public/data-sheets/Si7021-A20.pdf))**.

# Files
Si7021 documentation - https://www.silabs.com/documents/public/data-sheets/Si7021-A20.pdf
Pico SDK - https://www.raspberrypi.com/documentation/pico-sdk/index_doxygen.html#index

# Code

Example use of this library is demonstrated in example.cpp file located in src\ folder.

## Constructor
Default constructor, takes one argument which is utilized I2C controller. Using `i2c0` or `i2c1` macro is sufficient.

    Si7021::Si7021  si7021(i2c0);
   

## Types
Library provides 3 enum types dedicated to use in it's methods.

Resolution represents accuracy of temperature and humidity data measurement.

    enum  Resolution{
	    RH12T14,
	    RH08T12,
	    RH10T13,
	    RH11T11
    };

## Methods

- askFor...() methods get information from Si7021 module via I2C.
- get...() methods get information from class member variables that are updated each time askFor...() methods are used.
- set...() methods set parameters of Si7021 module via I2C.
