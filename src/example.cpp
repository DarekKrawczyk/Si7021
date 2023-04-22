#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include <iostream>
#include <list>
#include <inttypes.h>
#include "si7021.h"

int main() {
    stdio_init_all();
    gpio_init(21);
    gpio_set_dir(21, true);
    i2c_init(i2c0, 100000);
    gpio_set_function(0, GPIO_FUNC_I2C);
    gpio_set_function(1, GPIO_FUNC_I2C);
    gpio_pull_up(0);
    gpio_pull_up(1);

    Si7021::Si7021 si7021(i2c0);
    si7021.setHeater(false);
    si7021.setResolution(Si7021::Resolution::RH11T11);
    while(true){
        printf("Si7021 Serial number: %#" PRIx64 "\n", si7021.askForSerialNumber());
        printf("Si7021 Firmware: %#x\n", si7021.askForFirmwareRev());
        printf("Si7021 Humidity: %.2f[%%]\n", si7021.askForHumidity());
        printf("Si7021 Temperature: %.2f[°C]\n", si7021.askForTemperature());
        printf("Si7021 User register: %i\n", si7021.askForRegisterData(Si7021::Register::User));
        printf("Si7021 Heater register: %i\n", si7021.askForRegisterData(Si7021::Register::Heater));
        printf("Si7021 Resolution: %i\n", si7021.askForResolution());
        printf("Si7021 Heater status: %i\n", si7021.askForHeater());
        printf("Si7021 Heater config: %i\n", si7021.askForHeaterConfig());
        printf("-----------------------------------------------------\n");
        sleep_ms(1000);
    }
}