#include "si7021.h"

/// @brief Default constructor for Si7021 module.
/// @param i2c I2C instance.
Si7021::Si7021::Si7021(i2c_inst_t *i2c)
{
    _i2c = i2c;
    _serialNumber = this->askForSerialNumber();
    _firmwareRevision = this->askForFirmwareRev();
    _temperature = this->askForTemperature();
    _humidity = this->askForHumidity();
    _heaterEnabled = this->askForHeater();
    _resolution = this->askForResolution();
    _heaterConfiguration = this->askForHeaterConfig();
}

#pragma region Serial number and Firmware
/// @brief Ask Si7021 module for it's serial number.
/// @return Serial number.
uint64_t Si7021::Si7021::askForSerialNumber()
{
    uint64_t serialNumber = 0;
    uint8_t outputID1[4] = {0, 0, 0, 0};
    uint8_t outputID2[4] = {0, 0, 0, 0};
    uint8_t ID1[2] = {SI7021_REID1B_1, SI7021_REID1B_2};
    uint8_t ID2[2] = {SI7021_REID2B_1, SI7021_REID2B_2};
    // First block
    i2c_write_blocking(_i2c, SI7021_ADR, ID1, 2, true);
    i2c_read_blocking(_i2c, SI7021_ADR, outputID1, 4, true);
    // Second block
    i2c_write_blocking(_i2c, SI7021_ADR, ID2, 2, true);
    i2c_read_blocking(_i2c, SI7021_ADR, outputID2, 4, true);
    for (int i = 0; i < 4; i++)
    {
        serialNumber = (serialNumber << 8) + outputID1[i];
    }
    for (int i = 0; i < 4; i++)
    {
        serialNumber = (serialNumber << 8) + outputID2[i];
    }
    _serialNumber = serialNumber;
    _firmwareRevision = outputID2[0];
    return serialNumber;
}

/// @brief Serial number accessor.
/// @return Serial number.
uint64_t Si7021::Si7021::getSerialNumber() const
{
    return _serialNumber;
}

/// @brief Ask Si7021 module for it's firmware revision.
/// @return Firmware revision.
uint8_t Si7021::Si7021::askForFirmwareRev()
{
    uint8_t firmwareRev = 0;
    uint8_t ID2[2] = {SI7021_REID2B_1, SI7021_REID2B_2};
    i2c_write_blocking(_i2c, SI7021_ADR, ID2, 2, true);
    i2c_read_blocking(_i2c, SI7021_ADR, &firmwareRev, 1, true);
    _firmwareRevision = firmwareRev;
    return firmwareRev;
}

/// @brief Firmware revision accessor.
/// @return Firmware revision.
uint8_t Si7021::Si7021::getFirmwareRev() const
{
    return _firmwareRevision;
}
#pragma endregion
#pragma region Humidity
/// @brief Ask Si7021 module for current relative humidity.
/// @return Relative humidity.
float Si7021::Si7021::askForHumidity(){
    float rh = 0;
    uint8_t returnData[3] = {0,0,0};
    uint8_t dataRegister = SI7021_MRH_HMM;
    i2c_write_blocking(_i2c, SI7021_ADR, &dataRegister, 1, true);
    i2c_read_blocking(_i2c, SI7021_ADR, returnData, 3, true);
    uint16_t combined = returnData[0];
    combined = (combined<<8) | returnData[1];
    rh = ((125*(float)combined)/65536) - 6;
    if(rh < 0) rh = 0;
    if(rh > 100) rh = 100;
    _humidity = rh;
    return rh;
}

/// @brief Relative humidity accessor.
/// @return Relative humidity.
float Si7021::Si7021::getHumidity() const {
    return _humidity;
}
#pragma endregion
#pragma region Temperature
/// @brief Ask Si7021 module for current temperature.
/// @return Temperature.
float Si7021::Si7021::askForTemperature() {
    float temp = 0;
    uint8_t returnData[2] = {0,0};
    uint8_t dataRegister = SI7021_MT_HMM;
    i2c_write_blocking(_i2c, SI7021_ADR, &dataRegister, 1, true);
    i2c_read_blocking(_i2c, SI7021_ADR, returnData, 2, true);
    uint16_t combined = returnData[0];
    combined = (combined<<8) | returnData[1];
    temp = ((175.72f*(float)combined)/65536) - 46.85f;
    _temperature = temp;
    return temp;
}

/// @brief Temperature accessor.
/// @return Temperature.
float Si7021::Si7021::getTemperature() const {
    return _temperature;
}

/// @brief Ask Si7021 module for temperature from last humidity measurement.
/// @return Temperature.
float Si7021::Si7021::askForTemperaturePrevRH() {
    float temp = 0;
    uint8_t returnData[2] = {0,0};
    uint8_t dataRegister = SI7021_RTVFPRHM;
    i2c_write_blocking(_i2c, SI7021_ADR, &dataRegister, 1, true);
    i2c_read_blocking(_i2c, SI7021_ADR, returnData, 2, true);
    uint16_t combined = returnData[0];
    combined = (combined<<8) | returnData[1];
    temp = ((175.72f*(float)combined)/65536) - 46.85f;
    _temperature = temp;
    return temp;
}
#pragma endregion
#pragma region Device
//TODO
bool Si7021::Si7021::calculateCRC(uint8_t value){
    return true;
}

//TODO
void Si7021::Si7021::resetDevice(){

}
#pragma endregion
#pragma region Register
/// @brief Ask Si7021 module for data from provided register.
/// @param Register Register which data will be accessed. 
/// @return Register data.
uint8_t Si7021::Si7021::askForRegisterData(Register Register){
    uint8_t m_register = 0;
    uint8_t m_returnData = 0;
    switch(Register){
        case Heater:
        m_register = SI7021_RHCR;
        break;
        case User:
        m_register = SI7021_RRHTUR1;
        break;
    }  
    i2c_write_blocking(_i2c, SI7021_ADR, &m_register, 1, true);
    i2c_read_blocking(_i2c, SI7021_ADR, &m_returnData, 1, true);
    return m_returnData;
}

/// @brief Reset value of the register.
/// @param Register Register whose data will be reseted.
/// @return True if operation was successful, false if not.
bool Si7021::Si7021::resetRegister(Register Register){
    uint8_t returnRegister = 0;
    if(Register == Register::Heater){
        bool returnValue = this->setRegister(0b00000000, Register::Heater);
        return returnValue;
    }
    else{
        // Register::User then...
        bool returnValue = this->setRegister(0b00111010, Register::User);
        return returnValue;
    }
}

/// @brief Set new value of the register.
/// @param newRegisterByte Byte to write into register.
/// @param Register Type of register.
/// @return True if operation was successful, false if not.
bool Si7021::Si7021::setRegister(uint8_t newRegisterByte, Register Register){
    uint8_t m_register[2] = {0, newRegisterByte};
    switch(Register){
        case Heater:
        m_register[0] = SI7021_WHCR;
        break;
        case User:
        m_register[0] = SI7021_WRHTUR1;
        break;
    }
    i2c_write_blocking(_i2c, SI7021_ADR, m_register, 2, true);
    uint8_t returnRegister = this->askForRegisterData(Register);
    if(returnRegister != newRegisterByte){
        return false;
    }
    return true;
}  
#pragma endregion
#pragma region Resolution
/// @brief Current resolution accessor.
/// @return Resolution.
Si7021::Resolution Si7021::Si7021::getResolution() const{
    return _resolution;
}

/// @brief Ask Si7021 module for current resolution.
/// @return Resolution.
Si7021::Resolution Si7021::Si7021::askForResolution(){
    uint8_t currentRegister = this->askForRegisterData(Register::User);
    Resolution resolution;
    int first, second;
    if(currentRegister & 0b10000000 == 0b10000000){
        first = 1;
    } else{
        first = 0;
    } 
    if(currentRegister & 0b00000001 == 0b00000001){
        second = 1;
    } else {
        second = 0;
    }
    if(first == 1 && second == 1) {
        resolution = Resolution::RH11T11;
    }
    else if(first == 1 && second == 0){
        resolution = Resolution::RH10T13;
    }
    else if(first == 0 && second == 1){
        resolution = Resolution::RH08T12;
    }
    else {
        resolution = Resolution::RH12T14;
    }
    _resolution = resolution;
    return resolution;
}

/// @brief Change current type of resolution.
/// @param res Resolution type.
/// @return True if operation was successful, false if not.
bool Si7021::Si7021::setResolution(Resolution res){
    uint8_t currentConfig = this->askForRegisterData(Register::User);
    uint8_t newConfig = 0;
    switch(res){
        case RH12T14:
        newConfig = 0b00000000;
        break;
        case RH10T13:
        newConfig = 0b00000001;
        break;
        case RH08T12:
        newConfig = 0b10000000;
        break;
        case RH11T11:
        newConfig = 0b10000001;
        break;
    }
    newConfig |= currentConfig;
    _resolution = res;
    bool returnValue = this->setRegister(newConfig, Register::User);
    return returnValue;
}
#pragma endregion
#pragma region Heater
/// @brief Ask Si7021 for current heater status.
/// @return True if heater is turned on, false if heater is turned off.
bool Si7021::Si7021::askForHeater(){
    uint8_t currentRegister = this->askForRegisterData(Register::User);
    if(currentRegister & 0b00000100 == 0b00000100){
        _heaterEnabled = true;
        return true;
    }
    _heaterEnabled = false;
    return false;
}

/// @brief Heater status accessor.
/// @return Heater status.
bool Si7021::Si7021::getHeater() const{
    return _heaterEnabled;
}

/// @brief Change current heater status.
/// @param status True to turn on, False to turn off.
/// @return True if operation was successful, false if not.
bool Si7021::Si7021::setHeater(bool status){
    if(status != _heaterEnabled){
        uint8_t currentRegister = this->askForRegisterData(Register::User);
        uint8_t newRegister = 0b00000100;
        newRegister |= currentRegister;
        bool returnValue = this->setRegister(newRegister, Register::User);
        _heaterEnabled = status;
        return returnValue;
    }
    return false;
}

/// @brief Set current heater configuration.
/// @param heater Heater configuration.
/// @return True if operation was successful, false if not.
bool Si7021::Si7021::setHeaterConfig(HeaterConfig heater){
    return true;
    //TODO
}

/// @brief Ask Si7021 for current heater configuration.
/// @return Heater configuration.
Si7021::HeaterConfig Si7021::Si7021::askForHeaterConfig(){
    return HeaterConfig::hc0000;
    //TODO
}

/// @brief Heater configuration accessor.
/// @return Heater configuration.
Si7021::HeaterConfig Si7021::Si7021::getHeaterConfig() const{
    return _heaterConfiguration;
    //TODO
}
#pragma endregion