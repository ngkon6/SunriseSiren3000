#define DEFAULT_USERNAME "clock"
#define DEFAULT_PASSWORD "beepbeepbeep" // 8+ characters, is used for AP

class SunriseSiren3000Auth {
  private:
    String username;
    String password;

  public:
    void fetch() {
      pref.begin("SS3000-Auth", true);
      this->username = pref.getString("username", "clock");
      this->password = pref.getString("password", "beepbeepbeep");
      pref.end();
    }

    void place(String username, String password) {
      pref.begin("SS3000-Auth", false);
      pref.putString("username", username);
      pref.putString("password", password);
      pref.end();

      this->username = username;
      this->password = password;
    }

    const char* getUsername() {
      return this->username.c_str();
    }
    const char* getPassword() {
      return this->password.c_str();
    }
};
