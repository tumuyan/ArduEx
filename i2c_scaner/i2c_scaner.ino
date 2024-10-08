#include <Adafruit_Keypad.h>
#include <Adafruit_Keypad_Ringbuffer.h>

#include <Wire.h>
#include "i2c_oled.h"

#include <Key.h>
#include "USB.h"
#include "USBHIDKeyboard.h"
#include "USBHIDConsumerControl.h"
#include "hut_keycode.h"


/*
 * // #include "esp32_i2s.h"

 Name:		i2c_scaner.ino
 Created:	2021/1/8 1:27:04
 Author:	Yazii

 待完成：
loop间隔时间
loop次数
i2c速率

*/

#define TITLE_KEYPAD "USB KeyPad"
#define TITLE_LAYER "Set Layer"
// cmd
uint8_t ID = 0x68;

String inputString = "";      // a String to hold incoming data
bool stringComplete = false;  // whether the string is complete
bool inputSpacer = false;     // flag for last char=\s
bool linked = false;          // flag for select i2c derive
String runtime = "";          //

bool looping = false;    // flag for repeat last commant
String loopingCmd = "";  // cache last command

uint8_t pinState[60];
uint8_t outputPins[60];

I2C_OLED i2c_oled;


//  keypad
String keyInString = "";

const short layerKey = 9;
const short layerNum = 9;
String layerHint = "1 a k u + \nNav F0 F1\n>";
short keySets[layerNum][10] = {
  { '1', '2', '3', '4', '5', '6', '7', '8', '9', '0' },
  { 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j' },
  { 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't' },
  { 'u', 'v', 'w', ',', KEY_BACKSPACE, 'x', 'y', 'z', '.', KEY_RETURN },
  { '+', '-', '*', '/', '\\', ' ', '.', ';', '\'', '=' },
  { KEY_PAGE_UP, KEY_UP_ARROW, KEY_PAGE_DOWN, KEY_TAB, KEY_ESC, KEY_LEFT_ARROW, KEY_DOWN_ARROW, KEY_RIGHT_ARROW, KEY_RETURN, KEY_HOME },
  { KEY_F1, KEY_F2, KEY_F3, KEY_F4, KEY_F5, KEY_F6, KEY_F7, KEY_F8, KEY_F9, KEY_F10 },
  { KEY_F11, KEY_F12, KEY_F13, KEY_F14, KEY_F15, KEY_F16, KEY_F17, KEY_F18, KEY_F19, KEY_F20 },
  // {KEY_APPLICATION, KEY_MENU, KEY_HELP, KEY_SELECT,KEY_STOP,KEY_CANNEL, KEY_CLEAR,KEY_RETURN, KEY_PRINTSCREEN,KEY_PAUSE}
  { KEY_APPLICATION, CONSUMER_CONTROL_HOME + 1000, CONSUMER_CONTROL_BACK + 1000, KEY_PRINTSCREEN, '#',
    CONSUMER_CONTROL_SEARCH + 1000, KEY_BACKSPACE, KEY_RETURN, KEY_TAB, '3' }

};

String keyHints[layerNum] = {
  "1 2 3 4 5\n6 7 8 9 0\n> ",
  "a b c d e\nf g h i j\n> ",
  "k l m n o\np q r s t\n> ",
  "u v w , \\b\nx y z . \\n\n> ",
  "+ - * / \\\n  . ; ' =\n> ",
  "U w D\\t Es\n asd \\n Ho\n> ",
  "F1 F2 F3 .\nF6 F7 F8 .\n> ",
  "F11 F12 ..\nF16 F17 ..\n> ",
  "Ap Ho Bk P\nSe \\b \\n\\t\n> "
};
int layer = 0;        // 0: USB keyboard 0-9
const byte ROWS = 5;  // rows
const byte COLS = 2;  // columns
char keyName[ROWS * COLS] = { '1', '2', '3', '4', '5', '6', '7', '8', '9', '0' };
//define the symbols on the buttons of the keypads
char keys[ROWS][COLS] = {
  { '1', '6' },
  { '2', '7' },
  { '3', '8' },
  { '4', '9' },
  { '5', '0' }
};
byte rowPins[ROWS] = { 21, 39, 40, 41, 42 };  //connect to the row pinouts of the keypad
byte colPins[COLS] = { 16, 45 };              //connect to the column pinouts of the keypad

//initialize an instance of class NewKeypad
Adafruit_Keypad customKeypad = Adafruit_Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

USBHIDKeyboard Keyboard;

USBHIDConsumerControl ConsumerControl;

unsigned long wake_time = 100000;
unsigned long wake = 0;
bool oled_off = false;

void setup() {
  Keyboard.begin();
  ConsumerControl.begin();
  USB.begin();
  customKeypad.begin();

  Wire.begin();
  Serial.begin(115200);
  Serial.println("\nArduEx 0.1 Boot!\nPlease input command with Ansi code:");
  // i2c_oled.keyin(true, "ArduEx", "\n> ver 0.2");
  i2c_oled.keyin(true, TITLE_KEYPAD, keyHints[layer]);
  wake = millis() + wake_time;
}

bool layerChange = false;
bool layerKeyPressed = false;

void loop() {
  customKeypad.tick();

  keyInString = "";


  while (customKeypad.available()) {
    if (oled_off && millis() > wake) {
      oled_off = false;
      i2c_oled.power(true);
    }
    wake = millis() + wake_time;

    keypadEvent e = customKeypad.read();
    char c = (char)e.bit.KEY;
    int i = e.bit.ROW + e.bit.COL * ROWS;
    Serial.print(c);
    if (e.bit.EVENT == KEY_JUST_PRESSED) {
      Serial.println(" o");

      if (i != layerKey) {
        if (layerKeyPressed) {

          if (i >= layerNum)
            layer = 0;
          else
            layer = i;
          layerChange = true;
          i2c_oled.keyin(true, TITLE_KEYPAD, keyHints[layer]);

        } else {
          short k = keySets[layer][i];
          process_key(k, true, false);
          // if (k < 256)
          //   Keyboard.press(k);
          // else
          //   Keyboard.pressRaw(k - 0x88);
          keyInString += c;
          keyInString += "o ";
          i2c_oled.keyin(true, TITLE_KEYPAD, keyHints[layer] + keyInString);
        }

      } else {
        layerKeyPressed = true;
        i2c_oled.keyin(true, TITLE_LAYER, layerHint);
      }

    } else if (e.bit.EVENT == KEY_JUST_RELEASED) {
      Serial.println(" x");

      if (i != layerKey) {
        if (layerKeyPressed) {

          i2c_oled.keyin(true, TITLE_LAYER, layerHint + keyName[i]);

        } else {

          short k = keySets[layer][i];
          process_key(k, false, true);
          // if (k < 256)
          //   Keyboard.release(k);
          // else
          //   Keyboard.releaseRaw(k - 0x88);
          keyInString += c;
          keyInString += "x ";
          if (keyInString.length() > 30) {
            keyInString = keyInString.substring(10);
          }
          i2c_oled.keyin(true, TITLE_KEYPAD, keyHints[layer] + keyInString);
        }

      } else {
        layerKeyPressed = false;
        if (layerChange) {
          layerChange = false;
        } else {
          // 发送 layerkey 的键盘事件
          short k = keySets[layer][i];
          process_key(k, true, true);

          // if (k < 256)
          //   Keyboard.write(k);
          // else {
          //   Keyboard.pressRaw(k - 0x88);
          //   Keyboard.releaseRaw(k - 0x88);
          // }
          keyInString += c;
          keyInString += "x ";
        }
        i2c_oled.keyin(true, TITLE_KEYPAD, keyHints[layer] + keyInString);
      }
    }
  }

  if (!oled_off && millis() > wake) {
    oled_off = true;
    i2c_oled.power(false);
  }

  delay(1);


  ReadSerial();
  if (stringComplete) {
    if (inputString == "loop") {
      looping = true;
    } else if (inputString == "q" && looping) {
      looping = false;
    } else {
      looping = false;
      if (matchCmd(inputString)) {
        loopingCmd = inputString;
      } else {
        Serial.println("[input]" + inputString);
      }
    }

    inputString = "";
    stringComplete = false;
  }

  if (looping) {
    matchCmd(loopingCmd);
  }
}

// 读取一行串口内容（一个或多个tab和空格转为一个空格,跳过空行）
void ReadSerial() {
  while (Serial.available()) {
    char inChar = (char)Serial.read();
    if (inChar == '\t' || inChar == ' ') {
      if (inputSpacer)
        continue;
      inputSpacer = true;
      inputString += ' ';
    } else {
      inputSpacer = false;
      if (inChar == '\n' || inChar == '\r' || inChar == '.') {
        inputString.trim();
        if (inputString.length() > 0) {
          stringComplete = true;
          return;
        }
      } else
        inputString += inChar;
    }
  }
}


void process_key(short k, bool press, bool release) {
  if (press) {
    if (k < 256)
      Keyboard.press(k);
    else if (k < 1000) {
      Keyboard.pressRaw(k - 0x88);
    } else {
      ConsumerControl.press(k - 1000);
    }
  }

  if (release) {
    if (k < 256)
      Keyboard.release(k);
    else if (k < 1000) {
      Keyboard.releaseRaw(k - 0x88);
    } else {
      ConsumerControl.release();
    }
  }
}



boolean matchCmd(String input) {
  if (input.length() < 2)
    return false;

  String input_str = input;
  input_str.toLowerCase();
  input_str.trim();

  //Serial.println("[debug]matchCmd 1=" + input + ";");
  //Serial.println("[debug]matchCmd 2=" + input_str + ";");

  if (input_str == "help") {
    Serial.println("[Done]help:\ni2c [help]\npin [help]\nloop: repeat the last command. input q exit repeat mode");
    return true;
  }



  bool cmd_rt = true;

  if (input_str == "runtime") {

  } else if (input_str == "i2c" || input_str == "iic") {
    runtime = "i2c";
  } else if (input_str == "quit" || input_str == "exit") {
    runtime = "";
  } else if (input_str == "i2s") {
    runtime = "i2s";
  } else if (input_str == "pin") {
    runtime = "pin";
  } else {
    cmd_rt = false;
  }

  if (cmd_rt) {
    Serial.println("[Done]runtime=\"" + runtime + "\"");
    return true;
  }


  if (input_str.startsWith("i2c ") || input_str.startsWith("iic ")) {
    run_i2c(input_str.substring(4));
    return true;
  } else if (input_str.startsWith("pin ")) {
    run_pin(input_str.substring(4));
    return true;
  } else {

    if (runtime == "i2c") {
      run_i2c(input_str);
      return true;
    } else if (runtime == "pin") {
      run_pin(input_str);
      return true;
    }

    else
      return false;
  }

  return false;
}


void run_pin(String cmd) {
  //Serial.println("[PIN CMD]" + cmd);

  int s0 = getWordStart(cmd, 0);
  int e0 = getWordEnd(cmd, s0);
  String w0 = cmd.substring(s0, e0);

  // debug
  //Serial.println("[w0]\""+w0+"\"");

  int size = CountWord(cmd);

  if (w0 == "help") {
    Serial.println("[Help]pin\n set/w/write pin_number(h/l): write data to pin output\nget/r/read pin_number: read pin input");
  } else if (w0 == "w" || w0 == "write" || w0 == "set") {

    if (size < 2) {
      Serial.println("[Err]pin_rw, Parameter size error.");
      return;
    }

    int s1;
    int e1 = e0;

    for (int i = 1; i < size; i++) {
      s1 = getWordStart(cmd, e1);
      e1 = getWordEnd(cmd, s1);
      int p = str2int(cmd.substring(s1, e1 - 1));
      if (p >= 0 && p < 60) {
        String s = cmd.substring(e1 - 1, e1);
        pinState[p] = (s == "h" || s == "t");
        if (!outputPins[p]) {
          pinMode(p, OUTPUT);
          outputPins[p] = true;
        }
        digitalWrite(p, pinState[p]);
        Serial.print(" pin_white, pin=" + cmd.substring(s1, e1 - 1) + ", ");
        Serial.println(pinState[p]);
      } else {
        Serial.println("[Err]pin_rw, pin error." + cmd.substring(s1, e1));
      }
    }

    Serial.println("[Done]pin_write");
  } else if (w0.startsWith("r") || w0 == "read" || w0.startsWith("get")) {
    if (size < 2) {
      Serial.println("[Err]pin_rw, parameter siez error.");
      return;
    }

    int s1;
    int e1 = e0;

    for (int i = 1; i < size; i++) {
      s1 = getWordStart(cmd, e1);
      e1 = getWordEnd(cmd, s1);
      int p = str2int(cmd.substring(s1, e1));
      if (p >= 0 && p < 60) {
        if (outputPins[p]) {
          pinMode(p, INPUT);
          outputPins[p] = false;
        }

        bool s = digitalRead(p);
        pinState[p] = s;
        Serial.print(" pin_read, pin=" + cmd.substring(s1, e1) + ", ");
        Serial.println(pinState[p]);
      } else {
        Serial.println("[Err]pin_rw, pin error." + cmd.substring(s1, e1));
      }
    }

    Serial.println("[Done]pin_read");
  } else {
    Serial.println("[Err]pin, Unknow Parameter.");
    return;
  }
}

uint8_t i2c_scan(uint8_t scan_from, uint8_t scan_to) {
  uint8_t error;
  int nDevices = 0;

  if (scan_from < 0) {
    Serial.println("[Err]i2c_scan scan_from<0");
    scan_from = 1;
  }
  if (scan_to > 127) {
    Serial.println("[Err]i2c_scan scan_to>127\n");
    scan_to = 127;
  }
  if (scan_from < scan_to) {
    Serial.print("[Wait]i2c_scan ");
    Serial.print(scan_from);
    Serial.print(" to ");
    Serial.println(scan_to);
  } else if (scan_from == scan_to) {
    Serial.print("[Wait]i2c_scan ");
    Serial.println(scan_from);
  } else {
    Serial.print("[Err]i2c_scan scan_to=");
    Serial.print(scan_to);
    Serial.print("<scan_from=");
    Serial.println(scan_from);
    return 0;
  }

  for (uint8_t address = scan_from; address <= scan_to; address++) {
    Wire.beginTransmission(address);
    error = Wire.endTransmission();
    if (error == 0) {
      Serial.print("I2C device found at address 0x");
      if (address < 16)
        Serial.print("0");
      Serial.println(address, HEX);
      if (nDevices < 1)
        ID = address;
      nDevices++;
    } else if (error == 4) {
      Serial.print("Unknown error at address 0x");
      if (address < 16)
        Serial.print("0");
      Serial.println(address, HEX);
    }
  }

  if (nDevices == 0)
    Serial.println("[Finish] No I2C devices found\n");
  else {
    Serial.print("[Finish] ");
    Serial.print(nDevices);
    Serial.println(" I2C devices found\n");
  }

  return nDevices;
}

void run_i2c(String cmd) {
  //Serial.println("[I2C CMD]" + cmd);

  int s0 = getWordStart(cmd, 0);
  int e0 = getWordEnd(cmd, s0);
  String w0 = cmd.substring(s0, e0);

  // debug
  //Serial.println("[w0]\""+w0+"\"");

  int size = CountWord(cmd);

  if (w0 == "select") {
    //Serial.println("[w0] = select");
    if (size < 2) {
      Serial.println("[Err]i2c_select, ID=null");
      return;
    }
    int s1 = getWordStart(cmd, e0);
    int e1 = getWordEnd(cmd, s1);

    //Serial.println("[debug]w1.str=" + cmd.substring(s1, e1));
    ID = str2int(cmd.substring(s1, e1));
    //Serial.print("[debug]w1.int=");
    //Serial.println(ID);

    Wire.beginTransmission(ID);
    uint8_t error = Wire.endTransmission();

    if (error == 0) {
      linked = true;
      Serial.print("[Done]i2c_select, ID=");
      Serial.print(ID);
    } else {
      linked = false;
      Serial.print("[Err]i2c_select, ID=");
      Serial.print(ID);
      Serial.print(", error code=");
      Serial.print(error);
    }

    if (ID < 16)
      Serial.print(" (ID=0x0");
    else
      Serial.print(" (ID=0x");
    Serial.print(ID, HEX);
    Serial.println(")");

  } else if (w0 == "scan") {
    int start = 0, end = 0x7f;
    if (size > 1) {
      int s1 = getWordStart(cmd, e0);
      int e1 = getWordEnd(cmd, s1);
      start = str2int(cmd.substring(s1, e1));

      if (size > 2) {
        int s2 = getWordStart(cmd, e1);
        int e2 = getWordEnd(cmd, s2);
        end = str2int(cmd.substring(s2, e2));
      } else {
        end = start;
      }
    }
    i2c_scan(start, end);
  } else if (w0 == "help") {
    Serial.println("[Help]i2c\n scan [start address] [end address]: scan i2c derives\n select address: select a derive (for write or read command)\n set/w/write regiser_address data_bytes: write data to regiser\nget/r/read[n][dec] regiser_address [read_bytes]: read regiser frome adress. n could set print how many data per line. Dec could print data as decimal(default print hex)");
  } else if (w0 == "w" || w0 == "write" || w0 == "set") {

    if (size < 3) {
      Serial.println("[Err]i2c_rw, Parameter size error.");
      return;
    }

    if (!linked) {
      Serial.println("[Err]i2c_rw, no derive selected.");
      return;
    }

    Wire.beginTransmission(ID);

    String Data = cmd.substring(e0);

    int s1;
    int e1 = e0;

    for (int i = 1; i < size; i++) {
      s1 = getWordStart(cmd, e1);
      e1 = getWordEnd(cmd, s1);
      Wire.write(str2int(cmd.substring(s1, e1)));
    }

    int error = Wire.endTransmission();
    if (error == 0)
      Serial.println("[Done]i2c_write");
    else {
      Serial.println("[Err]i2c_write");
      Serial.print(", error code=");
      Serial.print(error);
    }
  } else if (w0.startsWith("r") || w0 == "read" || w0.startsWith("get")) {
    if (size < 2) {
      Serial.println("[Err]i2c_rw, regiser lost .");
      return;
    }

    if (!linked) {
      Serial.println("[Err]i2c_rw, no derive selected.");
      return;
    }



    int s1 = getWordStart(cmd, e0);
    int e1 = getWordEnd(cmd, s1);
    uint8_t register_address = (uint8_t)str2int(cmd.substring(s1, e1));

    uint8_t read_length = 1;
    if (size > 2) {

      int s2 = getWordStart(cmd, e1);
      int e2 = getWordEnd(cmd, s2);
      //Serial.println("[debug]length.str=" + cmd.substring(s2, e2));
      read_length = (uint8_t)str2int(cmd.substring(s2, e2));
    }

    Wire.beginTransmission(ID);
    Wire.write(register_address);

    if (Wire.endTransmission() != 0) {
      Serial.println("[Err]i2c_read, read address error.");
      return;
    }
    Wire.requestFrom(ID, read_length);
    int line = 0;

    int row = str2int(w0) - 1;

    bool printDec = (w0.endsWith("dec"));


    //Serial.print("[debug]register_address=");
    //Serial.print(register_address);
    //Serial.print(" read_length=");
    //Serial.print(read_length);
    //Serial.print(" wire_avaiable=");
    //Serial.print(Wire.available());
    //Serial.print(" row=");
    //Serial.println(row);

    while (Wire.available()) {
      byte c = Wire.read();
      if (printDec)
        Serial.print(c);
      else
        Serial.print(c, HEX);

      if (row > 0) {
        line++;
        if (row < line) {
          Serial.print('\n');
          line = 0;
        } else {
          Serial.print('\t');
        }
      } else {
        Serial.print('\t');
      }
    }

    if (line > 0 || row < 0)
      Serial.print('\n');

    if (!looping) {
      if (printDec)
        Serial.println("[Done]i2c_read, show as Dec");
      else
        Serial.println("[Done]i2c_read, show as HEX");
    }
  } else if (w0 == "clk") {
    if (size > 1) {
      int s1 = getWordStart(cmd, e0);
      int e1 = getWordEnd(cmd, s1);
      String w1 = cmd.substring(s1, e1);
      int clk = (uint8_t)str2int(w1);
      if (w1.endsWith("m")) {
        clk = clk * 1000000;
      } else if (w1.endsWith("k")) {
        clk = clk * 1000;
      }
      if (clk > 0 && clk < 100000000) {
        Wire.setClock(clk);
      } else {
        Serial.print("[Err]i2c_clk, set clock = ");
        Serial.println(clk);
        return;
      }
    }

    Serial.print("[Done]i2c_clk = ");
    Serial.println(Wire.getClock());
    return;

  } else if (w0 == "demo") {
    int reading = 0;
    Wire.requestFrom(0x39, 2);  // request 2 bytes from slave device #112

    // step 5: receive reading from sensor

    if (2 <= Wire.available()) {  // if two bytes were received

      reading = Wire.read();  // receive high byte (overwrites previous reading)

      reading = reading << 8;  // shift high byte to be high 8 bits

      reading |= Wire.read();  // receive low byte as lower 8 bits

      Serial.println(reading);  // print the reading
    }
  } else if (w0 == "oled_demo") {
    String demo_name = "";
    if (size > 1) {
      int s1 = getWordStart(cmd, e0);
      int e1 = getWordEnd(cmd, s1);
      demo_name = cmd.substring(s1, e1);
      demo_name.trim();
    }

    Serial.println(demo_name);
    i2c_oled.demo(demo_name, true);
  } else if (w0 == "keyin") {
    if (size > 1) {
      int s1 = getWordStart(cmd, e0);
      int e1 = getWordEnd(cmd, s1);
      String text = cmd.substring(s1, e1);
      Serial.println(text);
      i2c_oled.keyin(true, "Key In>", text);
    }
  } else {
    Serial.println("[Err]i2c, Unknow Parameter.");
    return;
  }
}

int CountChar(String str, char c) {
  int count = 0;
  for (int i = 0; i < str.length(); i++) {
    if (str[i] == c)
      count++;
  }
  return count;
}

int CountWord(String str) {
  str.trim();
  int count = 0;
  bool s = true;
  for (int i = 0; i < str.length(); i++) {
    if (str[i] == '\t' || str[i] == ' ') {
      if (s)
        continue;
      s = true;
    } else if (s) {
      s = false;
      count++;
    }
  }
  return count;
}

int getWordStart(String string, int start) {
  return getWordStart(string, 0, start);
}

/// <summary>
/// 匹配字符串中word的开始位置
/// </summary>
/// <param name="string">字符串</param>
/// <param name="n">第几个（从0开始计算）</param>
/// <param name="start">匹配起始位置</param>
/// <returns>匹配到的起始位置</returns>
int getWordStart(String string, int n, int start) {
  String str = string.substring(start);
  int count = 0;
  bool s = true;
  for (int i = 0; i < str.length(); i++) {
    if (str[i] == '\t' || str[i] == ' ') {
      if (count == n)
        break;
      if (s)
        continue;
      count++;
      s = true;
    } else {
      if (s) {
        s = false;
        if (count == n) {
          return i;
        }
      }
    }
  }
  return start;
}

/// <summary>
/// 匹配字符串中word的结束位置
/// </summary>
/// <param name="string">字符串</param>
/// <param name="start">匹配起始位置</param>
/// <returns>匹配到的结束位置</returns>
int getWordEnd(String str, int start) {
  bool s = true;
  for (int i = start; i < str.length(); i++) {
    if (str[i] == '\t' || str[i] == ' ') {
      if (s)
        continue;
      return i;
    } else if (s) {
      s = false;
    }
  }
  return str.length();
}

// 字符串转int。
// 当字符串0x开头时，转换十六进制为十进制。当字符串以其他字符开头时，判读为十进制。
// 即使字符串有空格和其他字符，也会从中间读出第一段连续的数字。
int str2int(String str) {
  str.trim();
  if (str.startsWith("0x") || str.startsWith("0X")) {
    int value = 0;
    for (int i = 2; i < str.length(); i++) {
      int j = str[i] - 48;
      if (j >= 0 && j <= 9) {
        value = value * 16 + j;
      } else if (j >= 49 && j <= 54) {
        value = value * 16 + j - 39;
      } else {
        return value;
      }
    }
    return value;
  } else {
    int value = 0;

    int i = 0;
    int j = str[i] - 48;
    while (j < 0 || j > 9) {
      i++;
      if (i >= str.length())
        return 0;
      j = str[i] - 48;
    }

    while (i < str.length()) {
      j = str[i] - 48;

      if (j >= 0 && j <= 9) {
        value = value * 10 + j;
      } else {
        return value;
      }
      i++;
    }
    return value;
  }
}
