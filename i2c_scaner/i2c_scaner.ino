/*
 Name:		i2c_scaner.ino
 Created:	2021/1/8 1:27:04
 Author:	Yazii
*/

String inputString = "";     // a String to hold incoming data
bool stringComplete = false; // whether the string is complete
bool linked = false;
String runtime = "";

void setup() {
	Serial.begin(115200);
	Serial.println("ArduEx 0.1 Boot!\nPlease input command with Ansi code:");
}

void loop()
{
	ReadSerial();
	if (stringComplete)
	{
//	words.set(inputString, " \t");

		Serial.println(inputString + "/ input");
		//Serial.print(" words.size=");
		//Serial.println(words.size);


		inputString = "";
		stringComplete = false;
	}
}

void ReadSerial()
{
	while (Serial.available())
	{
		char inChar = (char)Serial.read();
		inputString += inChar;
		if (inChar == '\n' || inChar == '.')
		{
			stringComplete = true;
			inputString.trim();
			if (inputString.length() > 0)
				return;
		}
	}
}

