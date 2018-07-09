

//#define MANUAL_CALIBRATION 1
//#define LOGGING 1
#define JOYSTICK 1
//#define LED_DEBUG 800

#ifdef JOYSTICK
#include <Joystick.h>

// Create the Joystick
Joystick_ Joystick;
#else
#include <Keyboard.h>
#endif


#ifdef SAMD21
const int RX_LED = PIN_LED_RXL;
const int TX_LED = PIN_LED_TXL;
#else
const int RX_LED = LED_BUILTIN_RX;
const int TX_LED = LED_BUILTIN_TX;
#endif

bool ledState = LOW;

const int NUM_PADS = 10;
#ifdef JOYSTICK
float thresh_low[NUM_PADS] =  {150, 210, 200, 150, 180,
								95, 130, 85, 110, 160};
float thresh_high[NUM_PADS] = {thresh_low[0] + 5,
							   thresh_low[1] + 5,
							   thresh_low[2] + 5,
							   thresh_low[3] + 5,
							   thresh_low[4] + 5,
							   thresh_low[5] + 5,
							   thresh_low[6] + 5,
							   thresh_low[7] + 5,
							   thresh_low[8] + 5,
							   thresh_low[9] + 5};
#else
float thresh_high[NUM_PADS] = {250, 600, 300, 400, 400, 1, 1, 1, 1, 1};
float thresh_low[NUM_PADS] =  {200, 500, 250, 300, 350, 1, 1, 1, 1, 1};
#endif
uint8_t port_num[NUM_PADS] = {A3, A4, A1, A0, A2, A11, A9, A8, A7, A6};
int data[NUM_PADS];
char *port_names[NUM_PADS] = {"L_TL", "L_TR", "L_C", "L_BL", "L_BR", "R_TL", "R_TR", "R_C", "R_BL", "R_BR"};
#ifdef JOYSTICK
char button[NUM_PADS] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
#else
char key[NUM_PADS] = {'q', 'e', 's', 'z', 'c', 'u', 'o', 'k', 'm', '.'};
#endif
bool state[NUM_PADS] = {false, false, false, false, false, false, false, false, false, false};

void setup()
{
	pinMode(RX_LED, OUTPUT);
	pinMode(TX_LED, OUTPUT);
	digitalWrite(RX_LED, HIGH);
	digitalWrite(TX_LED, HIGH);

#ifdef SAMD21
	analogReadResolution(12);
#endif

	SerialUSB.begin(115200);
#ifdef JOYSTICK
	Joystick.begin();
#else
	Keyboard.begin();
#endif
}

void loop()
{
#ifdef LED_DEBUG
	static int led_debug = 0;
	static bool led_state = HIGH;
	led_debug++;
	if (led_debug > LED_DEBUG) {
		led_debug = 0;
		led_state = !led_state;
		digitalWrite(RX_LED, led_state);
	}
#endif

	for (int i = 0; i < NUM_PADS; i++) {
		data[i] = analogRead(port_num[i]);

#ifdef LOGGING
		/* Print debugging info */
		SerialUSB.print(port_names[i]);
		SerialUSB.print(",");
		SerialUSB.print(data[i]);
		SerialUSB.print(",");
		SerialUSB.print((int)state[i]);
		if (data[i] < thresh_low[i]) {
			SerialUSB.print("L");
		} else if (data[i] > thresh_high[i]) {
			SerialUSB.print("H");
		} else {
			SerialUSB.print("M");
		}
		SerialUSB.print(",");
		SerialUSB.println((int)state[i]);
#endif

		/* Press keys */
		if (data[i] < thresh_low[i] && state[i] == false) {
			state[i] = true;
#ifdef JOYSTICK
			Joystick.setButton(button[i], state[i]);
#else
			Keyboard.press(key[i]);
#endif
		} else if (data[i] > thresh_high[i] && state[i] == true) {
			state[i] = false;
#ifdef JOYSTICK
			Joystick.setButton(button[i], state[i]);
#else
			Keyboard.release(key[i]);
#endif
		}
	}

#ifdef MANUAL_CALIBRATION
	delay(200);
#endif
}
