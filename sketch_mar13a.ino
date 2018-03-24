#include <Keyboard.h>
const int BLUE_LED = 13; // Blue "stat" LED on pin 13
const int RX_LED = PIN_LED_RXL; // RX LED on pin 25, we use the predefined PIN_LED_RXL to make sure
const int TX_LED = PIN_LED_TXL; // TX LED on pin 26, we use the predefined PIN_LED_TXL to make sure

bool ledState = LOW;

const int NUM_PADS = 5;
float thresh_high[NUM_PADS] = {250, 600, 300, 400, 400};
float thresh_low[NUM_PADS] =  {200, 500, 250, 300, 350};
uint8_t port_num[NUM_PADS] = {A3, A4, A1, A0, A2};
int data[NUM_PADS];
char *port_names[NUM_PADS] = {"TL", "TR", "C", "BL", "BR"};
char key[NUM_PADS] = {'q', 'e', 's', 'z', 'c'};
bool state[NUM_PADS] = {false, false, false, false, false};

void setup()
{
	pinMode(BLUE_LED, OUTPUT);
	pinMode(RX_LED, OUTPUT);
	pinMode(TX_LED, OUTPUT);
	digitalWrite(RX_LED, HIGH);
	digitalWrite(TX_LED, HIGH);
	digitalWrite(BLUE_LED, LOW);
	analogReadResolution(12);

	SerialUSB.begin(115200);
	Keyboard.begin();
}

void loop()
{
	for (int i = 0; i < NUM_PADS; i++) {
		data[i] = analogRead(port_num[i]);

#if LOGGING
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
			Keyboard.press(key[i]);
			state[i] = true;
		} else if (data[i] > thresh_high[i] && state[i] == true) {
			Keyboard.release(key[i]);
			state[i] = false;
		}
	}
	/*delay(20);*/
}
