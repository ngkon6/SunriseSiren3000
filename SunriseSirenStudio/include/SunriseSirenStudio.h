#ifndef _sunrise_siren_studio_h
#define _sunrise_siren_studio_h

#define STUDIO_VERSION "v2.2.0"
#define REQUIRED_FIRMWARE_VERSION 12

#include <stdlib.h>
#include <pthread.h>
#include <math.h>

#include <gtk/gtk.h>
#include <curl/curl.h>
#include <json-c/json.h>

#ifndef PATH_MAX
    #define PATH_MAX 1024 // stfu VSCode
#endif

char* argv0;

gchar* hostname[32];
gchar* username[32];
gchar* password[32];

json_object *clock_status;
json_object *clock_information;

GSettings *credentials;

GtkStack *MainStack;

// MainWindow
GtkWindow *MainWindow;
// Toolbar items
GtkToolButton *ReturnToHome;
GtkLabel *HostnameLabel;
GtkButton *SleepClock;
GtkButton *RebootClock;
GtkButton *UniversalConfirm;
GtkStackSwitcher *TestingStackSwitcher;CustomColor_Colon

// Main menu
GtkButton *GotoColors;
GtkButton *GotoAlarms;
GtkButton *GotoCountdown;
GtkButton *GotoCustomMode;
GtkButton *GotoSettings;
GtkButton *GotoInformation;

// Colors
GtkColorButton *DefaultColor;
GtkColorButton *HighlightColor;

// Alarms
GtkListBox *AlarmListBox;
GtkListBoxRow *AlarmRow[7];
GtkSwitch *AlarmEnable[7];
GtkSpinButton *AlarmHour[7];
GtkSpinButton *AlarmMinute[7];
GtkLabel *HighlightInfoLabel;

// Countdown
GtkSpinButton *CountdownValue;
GtkSwitch *CountdownPauseable;
GtkSwitch *CountdownSecondsOnly;
GtkButton *CountdownStop;

// Custom mode
GtkListStore *CustomDigits;

GtkColorButton *CustomColor[4];
GtkColorButton *CustomColor_Colon;
GtkComboBox *CustomDigit[4];
GtkEntry *CustomDigitEntry[4];

GtkToggleButton *CustomSegmentNumber[21];
GtkEntry *CustomDigitApplyIndex;
GtkButton *CustomDigitApply;
GtkButton *CustomDigitApplyAll;
GtkButton *CustomDigitEnableAll;
GtkButton *CustomDigitDisableAll;
GtkButton *CustomDigitInvertAll;
GtkCheckButton *CustomAutoIncrement;

// Settings
GtkSwitch *EnableLeadingZero;
GtkSwitch *EnableDST;
GtkSpinButton *BuzzerDutyCycle;
GtkSpinButton *SnoozeInterval;
GtkSpinButton *ClockReturn;
GtkSpinButton *LDRMin;
GtkSpinButton *LDRMax;

GtkButton *ReconfigureClock;
GtkButton *ReconfigureStudio;

// Information
GtkLabel *FirmwareVersionReading;
GtkLabel *NeoPixelBrightnessReading;
GtkLabel *LDRReading;
GtkLabel *SHT21TemperatureReading;
GtkLabel *SHT21HumidityReading;
GtkListBoxRow *InformationRefreshRow;
GtkButton *InformationRefresh;

GtkButton *AboutProgram;
GtkButton *OnGitHub;

// ConnectionWindow
GtkWindow *ConnectionWindow;
GtkEntry *ConnectHostname;
GtkEntry *ConnectUsername;
GtkEntry *ConnectPassword;
GtkButton *ConnectConfirm;
GtkLabel *ConnectVersion;

// ConnectErrorWindow
GtkWindow *ConnectErrorWindow;
GtkButton *CloseAfterError;
GtkButton *RetryAfterError;
GtkButton *ReconfigureAfterError;

// IncompatibleFirmwareWindow
GtkWindow *IncompatibleFirmwareWindow;
GtkLabel *IncompatibilityLabel;
GtkButton *CloseAfterIncompatibility;
GtkButton *RetryAfterIncompatibility;
GtkButton *DownloadAfterIncompatibility;

// LoginChangeWindow (actually a dialog)
GtkWindow *LoginChangeWindow;
GtkEntry *ChangeOldPassword;
GtkEntry *ChangeNewUsername;
GtkEntry *ChangeNewPassword;
GtkEntry *ChangeNewPasswordRetype;
GtkButton *ChangeCancel;
GtkButton *ChangeConfirm;

enum TargetWindow;

void* thread_function();
void do_absolutely_nothing() {};

gboolean ctrl_pressed;

static void onActivate(GtkApplication *app, gpointer user_data);
int main(int argc, char* argv[]);

#endif
