/*
 * @Author: AZDelivery (original), Robert Wloch (adaptions)
 * @Date: 2025-01-19 23:00:00
 * @LastEditTime: 2025-01-19 23:00:00
 * @LastEditors: Robert Wloch
 * @Description: Smart Robot Car V4.0 Mega2560
 * @FilePath: 
 */
#ifndef _DF_PLAYER_MINI_H_
#define _DF_PLAYER_MINI_H_

#if defined(ARDUINO) && ARDUINO >= 100
  #include "Arduino.h"
#else
  #include "WProgram.h"
#endif

#include "SoftwareSerial.h"
#include "DFPlayer_commands.h"

class DFPlayerMini
{
public:
  DFPlayerMini(int rxPin, int txPin);
  /**
   * Sets the state of EQ.
   * @param equalizerPreset An Equalizer_Preset enum literal.
   */
  void set_equalizer(Equalizer_Preset equalizerPreset);
  /**
   * Sets the volume level.
   * @param volume Decibels as HEX byte. You may want to use one of the presents defined in Volume_Preset enum literal.
   */
  void set_volume(uint8_t volume);
  /**
   * Plays an mp3 folder file by its index.
   * @param DATA1_DATA2 Two bytes that define file number in range 1-3000 (0x0BB8).
   */
  void play_mp3(uint16_t DATA1_DATA2);
  /**
   * Query status of module. This will print playback status (0x42), volume, equalizer setting,
   * playback status (0x45), files on SD card and current file played.
   */
  void query_status();
  /**
   * Resets the module and sets the equalizer preset and volume level.
   */
  void module_init();
  /**
   * Establishes the serial connection to the DFPlayer Mini Module and initializes the module.
   */
  void setup_DFPlayer_Mini();

private:
  /**
   * Sends the command to the module.
   * @param CMD The command byte according to specification. You may want to use the defines from DFPlayer_commands.h.
   * @param DATA1 The first data byte.
   * @param DATA2 The second data byte.
   */
  void execute_command(uint8_t CMD, uint8_t DATA1 = 0x00, uint8_t DATA2 = 0x00);
  /**
   * Resets the receive_buffer.
   */
  void reset_receive_buffer();
  /**
   * Reads the received data.
   */
  bool receive();
  /**
   * Attempts to receive serial data from the module. Skips the Acknowledge responses until expected command response or nothing is received.
   * The function also calls necessary delays with the default values or provided values.
   *
   * This function is the fence for print_essential_receive_buffer!
   *
   * @param commandLabel A string describing the command. If empty, only receive() is called.
   * @param expectedCommand One of the Command_* defines in DFPlayer_commands.h. The function looks for a response with that command in the CMD field.
   * @param firstDelay The delay which is called immediately when the function is called. The default is usually fine, but after chip reset you should set 1000.
   * @param furtherDelays The delays that are called before the function returns. They ensure that following commands can receive responses. In rare cases you may need to tweak that default.
   * @return false if the receive_buffer should not be printed because no commandLabel was provided, no data was received or the expected command response was not received, 
   */
  bool smart_receive(const String &commandLabel = "", uint8_t expectCommand = 0, unsigned long firstDelay = 100, unsigned long furtherDelays = 100);
  /**
   * If commandLabel is provided it will print received CMD DATA1 DATA2 bytes in HEX.
   * The function also calls necessary delays with the default values or provided values.
   *
   * Must only be called fenced by smart_receive()!
   *
   * @param commandLabel A string describing the command. It will be printed in the first column before the TAB.
   * @param furtherDelays The delays that are called before the function returns. They ensure that following commands can receive responses. In rare cases you may need to tweak that default.
   */
  void print_essential_receive_buffer(const String &commandLabel = "", unsigned long furtherDelays = 100);
  /**
   * Attempts to receive serial data from the module. If commandLabel is provided it will print received CMD DATA1 DATA2 bytes in HEX. Skips the Acknowledge responses until expected command response or nothing is received.
   * The function also calls necessary delays with the default values or provided values.
   *
   * @param commandLabel A string describing the command. It will be printed in the first column before the TAB.
   * @param expectedCommand One of the Command_* defines in DFPlayer_commands.h. The function looks for a response with that command in the CMD field.
   * @param firstDelay The delay which is called immediately when the function is called. The default is usually fine, but after chip reset you should set 1000.
   * @param furtherDelays The delays that are called before the function returns. They ensure that following commands can receive responses. In rare cases you may need to tweak that default.
   */
  void smart_receive_and_print(const String &commandLabel = "", uint8_t expectCommand = 0, unsigned long firstDelay = 100, unsigned long furtherDelays = 100);
  /**
   * Resets the DFPlayer mini master module.
   */
  void reset_chip();

private:
  SoftwareSerial dfPlayerSerialConnection;
  uint8_t receive_buffer[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
  char data;
  uint8_t volume = 0x00;
  bool mute_state = false;  
};

#endif
