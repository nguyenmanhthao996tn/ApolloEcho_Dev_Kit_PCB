void WakeupCallback() {
  Serial.printf("This is Wakeup Callback\r\n");
}

void setup() {
  Serial.begin(115200);
  while (!Serial) {}

  Serial1.begin(115200);
  while (!Serial1) {}

  if (api.system.sleep.registerWakeupCallback(WakeupCallback) == false) {
    Serial.println("Create Wakeup Callback failed.");
  }

  Serial.println("Starting!");
}

void loop() {
  Serial1.println("AT+SEND=1,7,12,0,hello in SF7BW125\r\n");
  api.system.sleep.all(60 * 1000);
  Serial1.println("AT+SEND=1,8,13,0,hello in fast channel\r\n");
  api.system.sleep.all(60 * 1000);
}
