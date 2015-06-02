void buttonRead() {

  int reading = digitalRead(buttonPin);

  if (reading != lastButtonState) {
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > debounceDelay) {
    if (reading != buttonState) {
      buttonState = reading;

      if (buttonState == LOW && role == role_pong_back) {
        printf("*** CHANGING TO TRANSMIT ROLE -- PRESS 'R' TO SWITCH BACK\n\r");

        // Become the primary transmitter (ping out)
        role = role_ping_out;
        ledState = HIGH;
        radio.openWritingPipe(pipes[0]);
        radio.openReadingPipe(1, pipes[1]);
      }

      else if (buttonState == LOW && role == role_ping_out) {
        printf("*** CHANGING TO RECEIVE ROLE -- PRESS 'T' TO SWITCH BACK\n\r");

        // Become the primary receiver (pong back)
        role = role_pong_back;
        ledState = LOW;
        radio.openWritingPipe(pipes[1]);
        radio.openReadingPipe(1, pipes[0]);
      }
    }
  }

  digitalWrite(ledPin, ledState);
  lastButtonState = reading;

}
