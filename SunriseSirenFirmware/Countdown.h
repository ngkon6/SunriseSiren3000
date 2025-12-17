class SunriseSiren3000Countdown {
  private:
    // milliseconds
    unsigned long now = 0;
    unsigned long lastNow = 0;
    long elapsed = 0;

  public:
    bool started = false;
    bool paused = false;
    bool ended = false;
    bool pauseable = false;
    bool secondsOnly = false;
    bool activity = false;

    // seconds
    int currentTime = 0;

    void start(int seconds) {
      this->currentTime = seconds;
      this->elapsed = 0;
      this->started = true;
      this->paused = false;
      this->ended = false;
      this->activity = false;
    }

    void togglePause() {
      if (!this->pauseable) return;

      this->paused = !this->paused;
    }

    void stop() {
      this->started = false;
      this->paused = false;
      this->ended = false;
      this->activity = false;
    }

    void update(int activityDutyCycle) {
      this->now = millis();

      if (!this->started) {
        this->lastNow = millis();
        return;
      }

      long delta = this->now - this->lastNow;
      if (!this->paused) this->elapsed += delta;

      if (this->elapsed >= 1000) {
        this->elapsed = 0;
        this->currentTime--;
      }

      if (this->currentTime < 0) {
        this->ended = true;
        this->currentTime = 0;
      }
      this->activity = (this->ended && this->elapsed < (activityDutyCycle / 100.0 * 1000));

      this->lastNow = this->now;
    }
};
