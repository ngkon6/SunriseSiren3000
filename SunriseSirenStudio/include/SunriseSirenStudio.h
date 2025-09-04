#ifndef _sunrise_siren_studio_h
#define _sunrise_siren_studio_h

#define STUDIO_VERSION "v2.0.0"
#define MINIMAL_FIRMWARE_VERSION 10

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
json_object *clock_sensors;

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
GtkStackSwitcher *TestingStackSwitcher;

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

// Countdown
GtkSpinButton *CountdownValue;
GtkSwitch *CountdownPauseable;
GtkSwitch *CountdownSecondsOnly;

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
GtkSpinButton *SnoozeInterval;
GtkSpinButton *ClockReturn;
GtkSpinButton *LDRMin;
GtkSpinButton *LDRMax;

GtkButton *Reconfigure;

// Information
GtkLabel *FirmwareVersionReading;
GtkLabel *LDRReading;
GtkLabel *SHT21TemperatureReading;
GtkLabel *SHT21HumidityReading;
GtkListBoxRow *SensorRefreshRow;
GtkButton *SensorRefresh;

GtkButton *AboutProgram;

// ConnectionWindow
GtkWindow *ConnectionWindow;
GtkEntry *ConnectHostname;
GtkEntry *ConnectUsername;
GtkEntry *ConnectPassword;
GtkButton *ConnectConfirm;
GtkLabel *ConnectVersion;

// ErrorWindow
GtkWindow *ErrorWindow;
GtkButton *CloseAfterError;
GtkButton *RetryAfterError;
GtkButton *ReconfigureAfterError;

enum TargetWindow;

void* thread_function();
void do_absolutely_nothing() {};

static void onActivate(GtkApplication *app, gpointer user_data);
int main(int argc, char* argv[]);

#endif
