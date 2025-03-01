#define ECHOSTAR_nRST_PIN P38
#define ECHOSTAR_BOOT_PIN P44
#define ECHOSTAR_PWR_ENABLE_PIN P4

#define LED_USER_1_G P12  // Red on my board
#define LED_USER_2_R P45  // White on my board
#define BTN_USER_1 P36
#define BTN_USER_2 P31

typedef enum {
  MODE_IDLE = 0,
  MODE_NORMAL,
  MODE_FIRMWARE_UPLOAD
} workingmode_t;
workingmode_t current_mode;

void setup(void) {
  gpio_init();

  current_mode = MODE_NORMAL;
}

void loop(void) {
  if (digitalRead(BTN_USER_1) == LOW) {
    current_mode = MODE_FIRMWARE_UPLOAD;
    delay(100);
  }

  if (digitalRead(BTN_USER_2) == LOW) {
    current_mode = MODE_NORMAL;
    delay(100);
  }

  switch (current_mode) {

    case MODE_FIRMWARE_UPLOAD:
      current_mode = MODE_IDLE;

      digitalWrite(LED_USER_1_G, HIGH);
      digitalWrite(LED_USER_2_R, LOW);

      EM2050_reset_to_bootloader();

      break;

    case MODE_NORMAL:
      current_mode = MODE_IDLE;

      digitalWrite(LED_USER_1_G, LOW);
      digitalWrite(LED_USER_2_R, HIGH);

      EM2050_reset_to_run();

      break;

    case MODE_IDLE:
    default:
      // Do nothing here
      break;
  }
}

void gpio_init(void) {
  // LEDs
  pinMode(LED_USER_1_G, OUTPUT);
  digitalWrite(LED_USER_1_G, LOW);

  pinMode(LED_USER_2_R, OUTPUT);
  digitalWrite(LED_USER_2_R, LOW);

  // Buttons
  pinMode(BTN_USER_1, INPUT_PULLUP);
  pinMode(BTN_USER_2, INPUT_PULLUP);

  // EM2050 Control Pins
  pinMode(ECHOSTAR_PWR_ENABLE_PIN, OUTPUT);
  digitalWrite(ECHOSTAR_PWR_ENABLE_PIN, HIGH);

  
  pinMode(ECHOSTAR_nRST_PIN, OUTPUT);
  pinMode(ECHOSTAR_BOOT_PIN, OUTPUT);
  EM2050_reset_to_run();
}

void EM2050_reset_to_bootloader(void) {
  digitalWrite(ECHOSTAR_nRST_PIN, LOW);
  digitalWrite(ECHOSTAR_BOOT_PIN, LOW);
  delay(200);
  digitalWrite(ECHOSTAR_nRST_PIN, HIGH);
}

void EM2050_reset_to_run(void) {
  digitalWrite(ECHOSTAR_nRST_PIN, LOW);
  digitalWrite(ECHOSTAR_BOOT_PIN, HIGH);
  delay(200);
  digitalWrite(ECHOSTAR_nRST_PIN, HIGH);
}
