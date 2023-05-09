# Si7021 library

This is library for **Si7021** module. It has been created for RP2040 microcontroller placed inside Raspberry Pi Pico. Library is written in C++ and it uses Pico SDK **([link](https://www.raspberrypi.com/documentation/pico-sdk/index_doxygen.html#index))**. Si7021 is a temperature and humidity sensor, with I2C interface. Library was created based on IC documentation **([link](https://www.silabs.com/documents/public/data-sheets/Si7021-A20.pdf))**.

# Files
+ Si7021 documentation - https://www.silabs.com/documents/public/data-sheets/Si7021-A20.pdf
+ Pico SDK - https://www.raspberrypi.com/documentation/pico-sdk/index_doxygen.html#index

# Code

Example use of this library is demonstrated in example.cpp file located in src\ folder.

## Example code
```cpp
#include  <stdio.h>
#include  "pico/stdlib.h"
#include  "hardware/i2c.h"
#include  <iostream>
#include  <list>
#include  <inttypes.h>
#include  "si7021.h"

int  main()  {
	stdio_init_all();
	gpio_init(21);
	gpio_set_dir(21,  true);
	i2c_init(i2c0,  100000);
	gpio_set_function(0,  GPIO_FUNC_I2C);
	gpio_set_function(1,  GPIO_FUNC_I2C);
	gpio_pull_up(0);
	gpio_pull_up(1);

	Si7021::Si7021  si7021(i2c0);
	si7021.setHeater(true);
	si7021.setResolution(Si7021::Resolution::RH11T11);
	si7021.setHeaterConfig(Si7021::HeaterConfig::hc0100);
	int  counter  =  0;
	while(true){
		printf("Si7021 Serial number: %#"  PRIx64  "\n",  si7021.askForSerialNumber());
		printf("Si7021 Firmware: %#x\n",  si7021.askForFirmwareRev());
		printf("Si7021 Humidity: %.2f[%%]\n",  si7021.askForHumidity());
		printf("Si7021 Temperature: %.2f[°C]\n",  si7021.askForTemperature());
		printf("Si7021 User register: %i\n", si7021.askForRegisterData(Si7021::Register::User));
		printf("Si7021 Heater register: %i\n", si7021.askForRegisterData(Si7021::Register::Heater));
		printf("Si7021 Resolution: %i\n",  si7021.askForResolution());
		printf("Si7021 Heater status: %i\n",  si7021.askForHeater());
		printf("Si7021 Heater config: %i\n",  si7021.askForHeaterConfig());
		printf("-----------------------------------------------------\n");
		if(counter++  ==  20){
			printf("Si7021 Heater and User register reset!\n");
			si7021.resetRegister(Si7021::Heater);
			si7021.resetRegister(Si7021::User);
		}
		sleep_ms(1000);
	}
}
```

## Constructor
Default constructor, takes one argument which is utilized I2C controller. Using `i2c0` or `i2c1` macro is sufficient.
```cpp
Si7021::Si7021  Si7021(i2c0);
```
## Methods
This Si7021 library provides methods that enable user to interface with most of the hardware included inside Si7021 module.

Methods of this class can be  divided into three types of functions. This type of functions can be differentiated by its suffix:
- askFor...() - this type of methods get information from Si7021 module directly via I2C commands. Every time this type of method is used, returned data is coming directly from the module. During execution of this methods, class type variables associated with returned type of data are being updated.
- get...() - this type of functions are using class membervariables, they DO NOT send I2C commands.
- set...() - this methods are used to set parameters of Si7021 module via I2C.

List of availablefunctions:
```cpp
// Serial number and Firmware
uint64_t  askForSerialNumber();
uint64_t  getSerialNumber()  const;
uint8_t  askForFirmwareRev();
uint8_t  getFirmwareRev()  const;

// Humidity
float  askForHumidity();
float  getHumidity()  const;

// Temperature
float  askForTemperature();
float  getTemperature()  const;
float  askForTemperaturePrevRH();

// Registers
bool  resetRegister(Register  Register);
uint8_t  askForRegisterData(Register  Register);
bool  setRegister(uint8_t  newRegisterByte,  Register  Register);

// Resolution
Resolution  getResolution()  const;
Resolution  askForResolution();
bool  setResolution(Resolution  res);

// Heater
bool  getHeater()  const;
bool  askForHeater();
bool  setHeater(bool  status);
bool  setHeaterConfig(HeaterConfig  heater);
HeaterConfig  askForHeaterConfig();
HeaterConfig  getHeaterConfig()  const;

// Device
void  resetDevice();
```

## Types
Library has built in 3 enum types dedicated to use in its methods.  
Resolution represents accuracy of temperature and humidity data measurement.
```cpp
enum  Resolution{
	RH12T14,
	RH08T12,
	RH10T13,
	RH11T11
};
```
Register enum used for pointing which register to use.
```cpp
enum  Register{
	Heater,
	User
};
```
Heater config enum used for defining configuration of the heater.
```cpp
enum  HeaterConfig{
	hc0000,
	hc0001,
	hc0010,
	...
	...
	hc1111
};
```
