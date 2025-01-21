/*
 * @Author: AZDelivery (original), Robert Wloch (adaptions)
 * @Date: 2025-01-19 23:00:00
 * @LastEditTime: 2025-01-19 23:00:00
 * @LastEditors: Robert Wloch
 * @Description: Smart Robot Car V4.0 Mega2560
 * @FilePath: 
 */
#include "SoftwareSerial.h"
#include "DFPlayer_commands.h"
#include "DFPlayerMini.h"

DFPlayerMini::DFPlayerMini(int rxPin, int txPin) : dfPlayerSerialConnection(rxPin, txPin) {
}

void DFPlayerMini::execute_command(uint8_t CMD, uint8_t DATA1 = 0x00, uint8_t DATA2 = 0x00) {
  //  Calculate the checksum (2 bytes)
  word checksum = -(Version_Byte + Command_Length + CMD + Acknowledge + DATA1 + DATA2);
  
  //  Build the command line
  uint8_t Command_line[10] = { Start_Byte, Version_Byte,
                            Command_Length, CMD, Acknowledge, 
                            DATA1, DATA2, highByte(checksum),
                            lowByte(checksum), End_Byte};

  //  Send the command line to the module
  for(uint8_t k = 0; k < 10; k++) {
    dfPlayerSerialConnection.write(Command_line[k]);
  }
}

void DFPlayerMini::reset_receive_buffer() {
  for(uint8_t i = 0; i < 10; i++) {
    receive_buffer[i] = 0;
  }
}

bool DFPlayerMini::receive() {
  reset_receive_buffer();
  if(dfPlayerSerialConnection.available() < 10) {
    return false;
  }  
  for(uint8_t i = 0; i < 10; i++) {
    short b = dfPlayerSerialConnection.read();
    if(b == -1) {
      return false;
    }
    receive_buffer[i] = b;
  }
  
  // when you reset the module in software,
  // receive_buffer elements are shifted
  // to correct that we do the following:
  short b = receive_buffer[0];
  for(uint8_t i = 0; i < 10; i++) {
    if(i == 9) {
      receive_buffer[i] = b;
    }
    else {
      receive_buffer[i] = receive_buffer[i+1];
    }  
  } //  end correcting receive_buffer
  return true;
}

bool DFPlayerMini::smart_receive(const String &commandLabel = "", uint8_t expectCommand = 0, unsigned long firstDelay = 100, unsigned long furtherDelays = 100) {
  delay(firstDelay);

  if (commandLabel == "") {
    receive();
    delay(furtherDelays);
    return false;
  }

  uint8_t skippedResponses = 0;
  uint8_t hasReceivedCommandResponse = 0;
  for (uint8_t i = 0; i < 10; i++) {
    if (!receive()) {
      Serial.println(commandLabel + "\t" + "recieved nothing");
      delay(furtherDelays);
      return false;
    }
    // break from for if response is not Acknowledge and matches expectCommand
    bool isExpectedCommand = (expectCommand != 0 && receive_buffer[Command_Index] == expectCommand) || expectCommand == 0;
    if (receive_buffer[Command_Index] != Command_Successfully_Executed && isExpectedCommand) {
      hasReceivedCommandResponse = 1;
      if (skippedResponses == 0) {
        Serial.println("recieved something that is not Acknowledge or the expected command");
      }
      break;
    }
    skippedResponses++;
    delay(furtherDelays);
  }
  if (hasReceivedCommandResponse == 0) {
    Serial.println("recieved nothing at all in 10 retries");
    delay(furtherDelays);
    return false;
  }
  return true;
}

void DFPlayerMini::print_essential_receive_buffer(const String &commandLabel = "", unsigned long furtherDelays = 100) {
  if (commandLabel == "") {
    return false;
  }

  Serial.print(commandLabel + "\t");
  // Outputs the returend data from the module
  // to the Serial Monitor
  for(uint8_t i = 3; i < 7; i++) { // only loop from command index to data bytes
    if (i == 4) continue; // skip Acknowledge byte
    Serial.print(receive_buffer[i], HEX);
    Serial.print("\t");
  }
  Serial.println();
  delay(furtherDelays);
}

void DFPlayerMini::smart_receive_and_print(const String &commandLabel = "", uint8_t expectCommand = 0, unsigned long firstDelay = 100, unsigned long furtherDelays = 100) {
  if (smart_receive(commandLabel, expectCommand, firstDelay, furtherDelays)) {
    print_essential_receive_buffer(commandLabel, furtherDelays);
  }
}

void DFPlayerMini::reset_chip() {
  execute_command(Command_Reset_Chip);
  smart_receive_and_print("SDON", Command_Current_Storage_Device, 1000);
}

void DFPlayerMini::set_equalizer(Equalizer_Preset equalizerPreset) {
  execute_command(Command_Set_Equalizer, 0, equalizerPreset);
  smart_receive_and_print();

  execute_command(Command_Get_Equalizer_Status);
  smart_receive_and_print("SETEQ", Command_Get_Equalizer_Status);
}

void DFPlayerMini::set_volume(uint8_t volume) {
  execute_command(Command_Set_Volume, 0, volume);
  smart_receive_and_print();

  execute_command(Command_Get_Volume_Status);
  smart_receive_and_print("SETVOL", Command_Get_Volume_Status);
}

void DFPlayerMini::play_mp3(uint16_t DATA1_DATA2) {
  if (DATA1_DATA2 > 3000) {
    Serial.println("Invalid value for mp3 folder file index. 1 <= DATA1_DATA2 <= 3000.");
    return;
  }
  // DATA1_DATA2=0x1234 -> DATA1=0x12, DATA2=0x34
  // for details see https://stackoverflow.com/a/70407902
  uint8_t DATA1 = DATA1_DATA2 / 0x100 & 0xff;
  uint8_t DATA2 = DATA1_DATA2 & 0xff;

  execute_command(Command_Play_MP3, DATA1, DATA2);
  smart_receive_and_print();

  execute_command(Command_Get_Playback_Status); // Get playback status
  smart_receive_and_print("MP3PLBK", Command_Get_Playback_Status);
}

void DFPlayerMini::query_status() {
  execute_command(Command_Get_Playback_Status_1); // Get status of module
  smart_receive_and_print("STATUS", Command_Get_Playback_Status_1);
  
  execute_command(Command_Get_Volume_Status);
  smart_receive_and_print("VOLUME", Command_Get_Volume_Status);
  
  execute_command(Command_Get_Equalizer_Status);
  smart_receive_and_print("EQ", Command_Get_Equalizer_Status);
  
  execute_command(Command_Get_Playback_Status); // Get playback status
  smart_receive_and_print("PLYBCK", Command_Get_Playback_Status);

  execute_command(Command_Get_Total_Number_Of_Files_On_SDcard);
  smart_receive_and_print("FILES", Command_Get_Total_Number_Of_Files_On_SDcard);

  execute_command(Command_Get_Current_File_Number);
  smart_receive_and_print("CRRTRK", Command_Get_Current_File_Number);
}

void DFPlayerMini::module_init() {
  reset_chip();
  set_equalizer(Equalizer_Preset::Classic);
  set_volume(Volume_Preset::dB_010_Quiet_Forest);
}

void DFPlayerMini::setup_DFPlayer_Mini() {
  dfPlayerSerialConnection.begin(9600);
  delay(1000);
  
  Serial.println("\nDFPlayer Mini Module Initialization");
  module_init();
}
