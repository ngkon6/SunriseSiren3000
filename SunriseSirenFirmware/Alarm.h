#define ALARM_ACTIVITY_PERIOD 1000 // in ms
#define ALARM_TIMEOUT 60000 // in ms

class SunriseSiren3000Alarm {
  private:
    unsigned long snoozedAt;
    unsigned long trippingStartedAt; // applies to both first time and snoozes after
    bool timePassed; // when the alarm time has passed, not affected by snoozing
    bool stopped; // false by default, true once button has been pressed

  public:
    String time;
    bool enabled; // whether the alarm is *supposed* to trip
    bool snoozed; // whether the alarm is snoozed
    bool tripping; // whether the alarm is active/beeping, caused by starting and snoozing
    bool activity; // whether the LEDs and buzzer should flash/beep, determined by `dutyCycle`
    bool quiet; // silence buzzer, but retain flashing LEDs

    bool update(String now, unsigned int snoozeInterval, float dutyCycle) {
      if (!this->enabled) return false;

      bool justTripped = false;
      int mappedDutyCycle = dutyCycle * ALARM_ACTIVITY_PERIOD;

      if (now == this->time && !this->timePassed) {
        this->timePassed = true;
        this->trippingStartedAt = millis();
        justTripped = true;
      } else if (this->snoozed && millis() - this->snoozedAt >= snoozeInterval)  {
        this->snoozed = false;
        this->trippingStartedAt = millis();
        justTripped = true;
      }

      unsigned long trippingSince = millis() - this->trippingStartedAt;
      this->tripping = (this->timePassed && !this->stopped && !this->snoozed);
      this->activity = (this->tripping && trippingSince % ALARM_ACTIVITY_PERIOD < mappedDutyCycle);

      if (millis() - this->trippingStartedAt >= ALARM_TIMEOUT && this->tripping) this->quiet = true;

      return justTripped;
    }

    void snooze() {
      this->snoozed = true;
      this->snoozedAt = millis();
      this->quiet = false;
    }

    void stop() {
      this->stopped = true;
      this->snoozed = false;
      this->quiet = false;
    }

    void reset() {
      this->timePassed = false;
      this->tripping = false;
      this->activity = false;
      this->stopped = false;
      this->snoozed = false;
      this->quiet = false;
    }
};
