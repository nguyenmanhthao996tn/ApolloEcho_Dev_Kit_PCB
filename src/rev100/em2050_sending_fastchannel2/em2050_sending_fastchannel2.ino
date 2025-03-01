#define TEST_PWR_MAX 20
#define TEST_PWR_MIN 0
#define TEST_PWR_STEP 5
#define NUM_PKT_EACH_CONFIG 1000

#define LED_USER_1_G P12 // Red on my board
#define LED_USER_2_R P45 // White on my board
#define BTN_USER_1 P36
#define BTN_USER_2 P31

uint32_t counter = 0;
int8_t tx_power = TEST_PWR_MAX;

void WakeupCallback()
{
	Serial.printf("This is Wakeup Callback\r\n");
}

void increase_counter(void)
{
	counter++;

	if (counter >= NUM_PKT_EACH_CONFIG)
	{
		counter = 0;

		tx_power -= TEST_PWR_STEP;
		if (tx_power < TEST_PWR_MIN)
		{
			tx_power = TEST_PWR_MAX;
		}

		Serial1.print("AT+TXPMSS=");
		Serial1.print(tx_power);
		Serial1.print("\r\n");
		delay(5000);
	}
}

void led_blink(int num_of_blink = 2)
{
	while (num_of_blink-- > 0)
	{
		digitalWrite(LED_USER_1_G, HIGH);
		delay(50);
		digitalWrite(LED_USER_1_G, LOW);
		delay(150);
	}
}

void setup()
{
	counter = 0;
	tx_power = TEST_PWR_MAX;

	// LEDs
	pinMode(LED_USER_1_G, OUTPUT);
	digitalWrite(LED_USER_1_G, HIGH);

	Serial.begin(115200);
	while (!Serial)
	{
	}

	Serial1.begin(115200);
	while (!Serial1)
	{
	}

	if (api.system.sleep.registerWakeupCallback(WakeupCallback) == false)
	{
		Serial.println("Create Wakeup Callback failed.");
	}

	Serial1.print("AT+TXPMSS=");
	Serial1.print(tx_power);
	Serial1.print("\r\n");
	delay(5000);

	Serial.println("Starting!");
	digitalWrite(LED_USER_1_G, LOW);
}

void loop()
{
	increase_counter();

	Serial1.print("AT+SEND=1,0,11,0,E0C");
	Serial1.print(counter);
	Serial1.print("P");
	Serial1.print(tx_power);
	Serial1.print("E\r\n");
	led_blink();
	api.system.sleep.all(60 * 1000);

	Serial1.print("AT+SEND=1,7,12,0,E0C");
	Serial1.print(counter);
	Serial1.print("P");
	Serial1.print(tx_power);
	Serial1.print("E\r\n");
	led_blink();
	api.system.sleep.all(60 * 1000);
}