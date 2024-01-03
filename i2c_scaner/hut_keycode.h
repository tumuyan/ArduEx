/* 
 Name:		hut_keycode.h
 Created:	2024-01-03 19:09
 Author:	tumuyan

keycode for esp32 usbhidkeyboard define offset 0x88
keycode for CONSUMER CONTROL should offset 1000
HID Usage Tables
https://www.usb.org/sites/default/files/hut1_4.pdf
*/

#define KEY_PRINTSCREEN	           0x46 + 0x88
#define KEY_SCROLL_LOCK	           0x47 + 0x88
#define KEY_PAUSE                  0x48 + 0x88
#define KEY_APPLICATION        0x65 + 0x88
#define KEY_POWER	           0x66 + 0x88
#define KEY_EXECUTE	           0x74 + 0x88
#define KEY_HELP	           0x75 + 0x88
#define KEY_MENU	           0x76 + 0x88

// 以下无法正常使用
#define KEY_SELECT	           0x77 + 0x88
#define KEY_STOP	           0x78 + 0x88
#define KEY_AGAIN	           0x79 + 0x88
#define KEY_UNDO	           0x7A + 0x88
#define KEY_CUT		           0x7B + 0x88
#define KEY_COPY	           0x7C + 0x88
#define KEY_PASTE	           0x7D + 0x88
#define KEY_FIND	           0x7E + 0x88
#define KEY_MUTE	           0x7F + 0x88
#define KEY_VOLUME_UP          0x80 + 0x88
#define KEY_VOLUME_DOWN        0x81 + 0x88
#define KEY_ALTERNATE_ERASE    0x99 + 0x88
#define KEY_SYSREQ	           0x9A + 0x88
#define KEY_CANNEL	           0x9B + 0x88
#define KEY_CLEAR	           0x9C + 0x88
#define KEY_PRIOR	           0x9D + 0x88
// #define KEY_RETURN	           0x9E + 0x88
#define KEY_SEPARATOR          0x9F + 0x88
#define KEY_OUT		           0xA0 + 0x88
#define KEY_OPER	           0xA1 + 0x88
// #define KEY_CLEAR_AGAIN	           0xA2 + 0x88
#define KEY_CRSEL	                 0xA3 + 0x88
#define KEY_EXSEL	                 0xA4 + 0x88
#define KEY_KEYPAD_00	             0xB0 + 0x88
#define KEY_KEYPAD_000	           0xB1 + 0x88
