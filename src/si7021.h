#pragma once
#include "hardware/i2c.h"

#pragma region SI7021_Definitions
// Si7021 address
#define SI7021_ADR 0x40

// Si7021 I2C commands
#define SI7021_MRH_HMM 0xE5
#define SI7021_MRH_NHMM 0xF5
#define SI7021_MT_HMM 0xE3
#define SI7021_MT_NHMM 0xF3
#define SI7021_RTVFPRHM 0xE0
#define SI7021_RESET 0xFE
#define SI7021_REID1B_1 0xFA
#define SI7021_REID1B_2 0x0F
#define SI7021_REID2B_1 0xFC
#define SI7021_REID2B_2 0xC9
#define SI7021_RFR_1 0x84
#define SI7021_RFR_2 0xB8

// Si7021 registers
#define SI7021_WRHTUR1 0xE6
#define SI7021_RRHTUR1 0xE7
#define SI7021_WHCR 0x51
#define SI7021_RHCR 0x11
#pragma endregion

namespace Si7021
{
    enum Resolution
    {
        RH12T14,
        RH08T12,
        RH10T13,
        RH11T11
    };

    enum Register
    {
        Heater,
        User
    };

    enum HeaterConfig{
        hc0000,
        hc0001,
        hc0010,
        hc0011,
        hc0100,
        hc0101,
        hc0110,
        hc0111,
        hc1000,
        hc1001,
        hc1010,
        hc1011,
        hc1100,
        hc1101,
        hc1110,
        hc1111
    };

    /*
    Si7021 class:
        - askFor...() methods get information from Si7021 module via I2C.
        - get...() methods get information from class member variables that are updated
          each time askFor...() methods is called.
        - set...() methods set parameters of Si7021 module via I2C.
    */

    class Si7021
    {
    private:
        i2c_inst_t *_i2c;
        uint64_t _serialNumber;
        uint8_t _firmwareRevision;
        float _humidity;
        float _temperature;
        bool _heaterEnabled;
        Resolution _resolution;
        HeaterConfig _heaterConfiguration;

    public:
        Si7021(i2c_inst_t *i2c);
        // Serial number and Firmware
        uint64_t askForSerialNumber();
        uint64_t getSerialNumber() const;
        uint8_t askForFirmwareRev();
        uint8_t getFirmwareRev() const;
        // Humidity
        float askForHumidity();
        float getHumidity() const;
        // Temperature
        float askForTemperature();
        float getTemperature() const;
        float askForTemperaturePrevRH();
        // Registers
        bool resetRegister(Register Register);
        uint8_t askForRegisterData(Register Register);
        bool setRegister(uint8_t newRegisterByte, Register Register);
        // Resolution
        Resolution getResolution() const;
        Resolution askForResolution();
        bool setResolution(Resolution res);
        // Heater
        bool getHeater() const;
        bool askForHeater();
        bool setHeater(bool status);
        bool setHeaterConfig(HeaterConfig heater);
        HeaterConfig askForHeaterConfig();
        HeaterConfig getHeaterConfig() const;
        // Device
        void resetDevice();

    private:
        bool calculateCRC(uint8_t value);
    };
}