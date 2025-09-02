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
// Clock settings tab
// Colors
GtkColorButton *DefaultColor;
GtkColorButton *HighlightColor;

// Alarms
GtkSwitch *AlarmEnable[7];
GtkSpinButton *AlarmHour[7];
GtkSpinButton *AlarmMinute[7];

// Advanced
GtkSwitch *EnableLeadingZero;
GtkSwitch *EnableDST;
GtkSpinButton *SnoozeInterval;
GtkSpinButton *ClockReturn;
GtkSpinButton *LDRMin;
GtkSpinButton *LDRMax;

GtkButton *ClockUpdate;

// Custom mode tab
GtkListStore *CustomDigits;
// Basic configuration
GtkColorButton *CustomColor[4];
GtkColorButton *CustomColor_Colon;
GtkComboBox *CustomDigit[4];
GtkEntry *CustomDigitEntry[4];
GtkButton *CustomUpdate;

// Custom digit generator
GtkToggleButton *CustomSegmentNumber[21];
GtkEntry *CustomDigitApplyIndex;
GtkButton *CustomDigitApply;
GtkButton *CustomDigitApplyAll;
GtkButton *CustomDigitEnableAll;
GtkButton *CustomDigitDisableAll;
GtkButton *CustomDigitInvertAll;

// Miscellaneous tab
// Countdown
GtkSpinButton *CountdownValue;
GtkSwitch *CountdownPauseable;
GtkButton *CountdownStart;

// Sensor information
GtkLabel *LDRReading;
GtkLabel *SHT21TemperatureReading;
GtkLabel *SHT21HumidityReading;
GtkButton *SensorRefresh;
GtkListBoxRow *SensorRefreshRow;

// Miscellaneous Sunrise Siren Stuff
GtkButton *SleepClock;
GtkButton *RebootClock;
GtkButton *AboutProgram;
GtkButton *Reconfigure;

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
