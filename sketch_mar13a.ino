#include <Keyboard.h>
#if 0

void setup()
{

}

void loop()
{
    Keyboard.write('z');  // send a 'z' to the computer via Keyboard HID
    delay(1000);  // delay so there aren't a kajillion z's
}

#if 0
// Connect A0 to A1, then open the Serial Plotter.

float x = 0; // Value to take the sin of
float increment = 0.02;  // Value to increment x by each time
int frequency = 440; // Frequency of sine wave

void setup()
{
  analogWriteResolution(10); // Set analog out resolution to max, 10-bits
  analogReadResolution(12); // Set analog input resolution to max, 12-bits

  SerialUSB.begin(9600);
}

void loop()
{
  // Generate a voltage value between 0 and 1023.
  // Let's scale a sin wave between those values:
  // Offset by 511.5, then multiply sin by 511.5.
  int dacVoltage = (int)(511.5 + 511.5 * sin(x));
  x += increment; // Increase value of x

  // Generate a voltage between 0 and 3.3V.
  // 0= 0V, 1023=3.3V, 512=1.65V, etc.
  analogWrite(A0, dacVoltage);

  // Now read A1 (connected to A0), and convert that
  // 12-bit ADC value to a voltage between 0 and 3.3.
  float voltage = analogRead(A1) * 3.3 / 4096.0;
  SerialUSB.println(voltage); // Print the voltage.
  delay(1); // Delay 1ms
}
#endif
#else
const int BLUE_LED = 13; // Blue "stat" LED on pin 13
const int RX_LED = PIN_LED_RXL; // RX LED on pin 25, we use the predefined PIN_LED_RXL to make sure
const int TX_LED = PIN_LED_TXL; // TX LED on pin 26, we use the predefined PIN_LED_TXL to make sure

bool ledState = LOW;

const int NUM_PADS = 5;
#if 0
float thresh_high[NUM_PADS] = {750, 1400, 1000, 1250, 1500};
float thresh_low[NUM_PADS] =  {600, 1300,  900, 1100, 1400};
#else
float thresh_high[NUM_PADS] = {250, 600, 300, 400, 400};
float thresh_low[NUM_PADS] =  {200, 500, 250, 300, 350};
#endif
uint8_t port_num[NUM_PADS] = {A3, A4, A1, A0, A2};
int data[NUM_PADS];
char *port_names[NUM_PADS] = {"Top Left", "Top Right", "Center", "Bottom Left", "Bottom Right"};
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

#if 0
	for (int i = 0; i < NUM_PADS; i++) {
		pinMode(port_num[i], INPUT_PULLDOWN);
	}
#endif

	SerialUSB.begin(9600);
	Keyboard.begin();
}

void loop()
{
	for (int i = 0; i < NUM_PADS; i++) {
		data[i] = analogRead(port_num[i]);
#if 0
		SerialUSB.print(port_names[i]);
		SerialUSB.print(" - ");
		if (data[i] < thresh_low[i]) {
			SerialUSB.print("___ - ");
		} else if (data[i] > thresh_high[i]) {
			SerialUSB.print("*** - ");
		} else {
			SerialUSB.print("MMM - ");
		}
		SerialUSB.println(data[i]);
	}

	int DELAY = 100;
	digitalWrite(RX_LED, LOW); // RX LED on
	delay(DELAY);
	digitalWrite(RX_LED, HIGH); // RX LED off
	digitalWrite(TX_LED, LOW); // TX LED on
	delay(DELAY);
	digitalWrite(TX_LED, HIGH); // TX LED off
	digitalWrite(BLUE_LED, HIGH); // Blue LED on
	delay(DELAY);
	digitalWrite(BLUE_LED, LOW); // Blue LED off
#else
		if (data[i] < thresh_low[i] && state[i] == false) {
			Keyboard.press(key[i]);
			state[i] = true;
		} else if (data[i] > thresh_high[i] && state[i] == true) {
			Keyboard.release(key[i]);
			state[i] = false;
		}
	}
	delay(20);
#endif
}
#endif
