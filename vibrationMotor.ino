void playVibration(int duration) {
  digitalWrite(Vibration_PIN, HIGH);
  delay(duration);
  digitalWrite(Vibration_PIN, LOW);
  delay(duration);
}