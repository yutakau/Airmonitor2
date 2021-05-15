//-------------------------------------------------------------------------------------------
// AQM0802.cpp - ESP32 LCDC library for AQM0802
// Yutaka Usui,  
//
// inspired by LCD_ST7032 Arduino library for ST7032 LCD controller with i2c interface.
//-------------------------------------------------------------------------------------------

#include <Arduino.h>
#include "AQM0802.h"

AQM0802::AQM0802() 
{ 
} 

bool AQM0802::begin(TwoWire &wirePort) //
{
  bool status;
  status = wirePort.begin();
  _i2cPort = &wirePort;
  
  delay(100);
  Write_Instruction(FUNCTION_SET | FUNCTION_SET_DL | FUNCTION_SET_N | FUNCTION_SET_IS);
  Write_Instruction(INTERNAL_OSC_FREQ | INTERNAL_OSC_FREQ_BS | INTERNAL_OSC_FREQ_F2);
  Write_Instruction(POWER_ICON_BOST_CONTR | POWER_ICON_BOST_CONTR_Ion);
  setcontrast(contrast);
  Write_Instruction(FOLLOWER_CONTROL | FOLLOWER_CONTROL_Fon | FOLLOWER_CONTROL_Rab2);
  delay(300);
  Write_Instruction(displayOnOffSetting);
  Write_Instruction(ENTRY_MODE_SET | ENTRY_MODE_SET_ID); 
  this->clear(); 
  this->home();
  return(status);
} 

void AQM0802::Write_Instruction(uint8_t cmd)
{
  _i2cPort->beginTransmission(Write_Address);
  _i2cPort->write(CNTRBIT_CO);  
  _i2cPort->write(cmd);
  _i2cPort->endTransmission();
  delayMicroseconds(WRITE_DELAY_MS);
}

void AQM0802::Write_Data(uint8_t data)
{
  _i2cPort->beginTransmission(Write_Address);
  _i2cPort->write(CNTRBIT_RS); 
  _i2cPort->write(data);
  _i2cPort->endTransmission();
  delayMicroseconds(WRITE_DELAY_MS);
}

size_t AQM0802::write(uint8_t chr) 
{
  this->Write_Data(chr);
  return 1;
}

void AQM0802::clear() { //clear display
  this->Write_Instruction(CLEAR_DISPLAY);
  delayMicroseconds(HOME_CLEAR_DELAY_MS);
}

void AQM0802::home() { //return to first line address 0
  this->Write_Instruction(RETURN_HOME);
  delayMicroseconds(HOME_CLEAR_DELAY_MS);
}

void AQM0802::setCursor(uint8_t line, uint8_t pos) 
{
  uint8_t p;
  if(pos > 15) pos = 0;
  if(line == 0) p = LINE_1_ADR + pos;
  else p = LINE_2_ADR + pos;
  Write_Instruction(SET_DDRAM_ADDRESS | p);
}

void AQM0802::display() //turn on display
{ 
  displayOnOffSetting |= DISPLAY_ON_OFF_D;
  Write_Instruction(displayOnOffSetting);
}

void AQM0802::noDisplay() //turn off display
{ 
  displayOnOffSetting &= ~DISPLAY_ON_OFF_D;
  Write_Instruction(displayOnOffSetting);
}

void AQM0802::cursor() //display underline cursor
{ 
  displayOnOffSetting |= DISPLAY_ON_OFF_C;
  Write_Instruction(displayOnOffSetting);
}

void AQM0802::noCursor() //stop display underline cursor
{ 
  displayOnOffSetting &= ~DISPLAY_ON_OFF_C;
  Write_Instruction(displayOnOffSetting);
}

void AQM0802::blink() //cursor block blink
{ 
  displayOnOffSetting |= DISPLAY_ON_OFF_B;
  Write_Instruction(displayOnOffSetting);
}

void AQM0802::noBlink() //stop cursor block blink
{ 
  displayOnOffSetting &= ~DISPLAY_ON_OFF_B;
  Write_Instruction(displayOnOffSetting);
}

void AQM0802::setcontrast(int val) 
{
  if (val > CONTRAST_MAX) val = CONTRAST_MIN;
  else if (val < CONTRAST_MIN) val = CONTRAST_MAX;
  Write_Instruction(CONTRAST_SET | (val & B00001111));
  Write_Instruction((val >> 4) | POWER_ICON_BOST_CONTR | POWER_ICON_BOST_CONTR_Bon);
  contrast = val;
}

void AQM0802::adjcontrast(int val) 
{
  setcontrast(val + contrast);
}

uint8_t AQM0802::getcontrast() 
{
  return contrast;
}
