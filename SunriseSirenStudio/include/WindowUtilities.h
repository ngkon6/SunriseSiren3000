#ifndef _window_utilities_h
#define _window_utilities_h

#define VALIDATION_STRING "Yes, a Sunrise Siren 3000 is here!"

// general
void quit() {
    exit(0);
}

void reboot_program() {
    execl("/proc/self/exe", argv0, NULL);

    perror("An error occurred while rebooting Sunrise Siren Studio.");
    exit(2);
}

void sleep_clock() {
    gchar *sleep_url[PATH_MAX];
    sprintf(sleep_url, "http://%s/sleep", hostname);

    gchar *zzz = request("PATCH", sleep_url, username, password, "");

    if (zzz && request_last_status_code == 200) {
        // sleep mode performed successful
        show_message_dialog(MainWindow, GTK_MESSAGE_INFO, GTK_BUTTONS_OK, "Success!", "Your Sunrise Siren 3000 falls asleep now... zzz");
    } else if (zzz && request_last_status_code == 418) {
        // device is already in sleep mode
        show_message_dialog(MainWindow, GTK_MESSAGE_WARNING, GTK_BUTTONS_OK, "zzz", "Your Sunrise Siren 3000 is already sleeping.\nTo wake it up again, push the button on top of the device.");
    } else if (zzz && request_last_status_code == 400) {
        // device cannot sleep in current state
        show_message_dialog(MainWindow, GTK_MESSAGE_WARNING, GTK_BUTTONS_OK, "Sleep mode failed", "Your Sunrise Siren 3000 cannot go to sleep mode in its current state.");
    } else {
        // error while trying to perform sleep mode
        show_message_dialog(MainWindow, GTK_MESSAGE_ERROR, GTK_BUTTONS_OK, "Sleep mode failed", "An error occurred while setting your Sunrise Siren 3000 in sleep mode. Please try again later.");
    }
}

void reboot_clock() {
    gint prompt = show_message_dialog(MainWindow, GTK_MESSAGE_WARNING, GTK_BUTTONS_YES_NO, "Reboot?", "Are you sure you want to reboot your Sunrise Siren 3000?");

    if (prompt == GTK_RESPONSE_YES) {
        gchar *reboot_url[PATH_MAX];
        sprintf(reboot_url, "http://%s/reboot", hostname);

        gchar *reboot = request("PATCH", reboot_url, username, password, "");

        if (reboot && strstr(reboot, "Initiating reboot.")) {
            // reboot performed successful
            show_message_dialog(MainWindow, GTK_MESSAGE_INFO, GTK_BUTTONS_OK, "Success!", "Your Sunrise Siren 3000 will reboot any moment now.");
        } else {
            // error while trying to perform reboot
            show_message_dialog(MainWindow, GTK_MESSAGE_ERROR, GTK_BUTTONS_OK, "Failed to reboot", "An error occurred while rebooting your Sunrise Siren 3000. Please try again later.");
        }
    }
}

static void login_change_dialog(GtkWidget *widget, gpointer state) {
    if (state) gtk_widget_show(LoginChangeWindow);
    else gtk_widget_hide(LoginChangeWindow);
}

static void reconfigure_studio(GtkWidget *widget, gpointer do_reset) {
    if (do_reset) {
        gint prompt = show_message_dialog(MainWindow, GTK_MESSAGE_WARNING, GTK_BUTTONS_OK_CANCEL, "Reset credentials?", "Note that you will have to sign in again!");
        if (prompt == GTK_RESPONSE_OK) credentials_reset();
        else return;
    } else {
        set_connect_immediately(FALSE);
    }
    reboot_program();
}

// MainWindow
// stack switcher
static void go_to(GtkWidget *widget, gpointer target) {
    gtk_stack_set_visible_child_name(MainStack, target);

    gtk_widget_set_sensitive(ReturnToHome, (target != "MainMenu"));
    gtk_widget_set_sensitive(UniversalConfirm, (target != "MainMenu" && target != "Information"));
}

// alarm settings
static void validate_alarm_time_sensitivity(GtkWidget *widget, gboolean state, gpointer user_data) {
    gint index = user_data;

    gtk_widget_set_sensitive(AlarmHour[index], state);
    gtk_widget_set_sensitive(AlarmMinute[index], state);
}

// custom mode
gint custom_combo_box_get_corresponding_number(GtkComboBox *combo_box) {
    GtkTreeIter iter;
    gint corresponding_number = 0;

    if (gtk_combo_box_get_active_iter(combo_box, &iter)) {
        GtkTreeModel *tree = gtk_combo_box_get_model(combo_box);
        gtk_tree_model_get(tree, &iter, 1, &corresponding_number, -1);
    } else exit(3);

    return corresponding_number;
}

static void validate_custom_entry_sensitive(GtkWidget *widget, gpointer user_data) {
    gint corresponding_number = custom_combo_box_get_corresponding_number(widget);
    gtk_widget_set_sensitive(user_data, (corresponding_number == 0));
}

static void set_all_custom_pixels(GtkWidget *widget, gpointer user_data) {
    for (int i=0; i<21; i++)
        gtk_toggle_button_set_active(CustomSegmentNumber[i], user_data);
}

static void invert_all_custom_pixels(GtkWidget *widget, gpointer user_data) {
    ctrl_pressed = TRUE;

    for (int i=0; i<21; i++)
        gtk_toggle_button_set_active(CustomSegmentNumber[i], !gtk_toggle_button_get_active(CustomSegmentNumber[i]));

    ctrl_pressed = FALSE;
}

static void set_custom_digit(GtkWidget *widget, gpointer user_data) {
    gint target_index = user_data ? user_data - 1 : gtk_spin_button_get_value_as_int(CustomDigitApplyIndex) - 1;
    gint custom_number = 0;
    gchar *entry_text[12];

    for (int i=0; i<21; i++) {
        if (gtk_toggle_button_get_active(CustomSegmentNumber[i])) custom_number += (int) pow(2, i);
    }

    sprintf(entry_text, "%i", custom_number);
    gtk_entry_set_text(CustomDigitEntry[target_index], entry_text);

    gint combo_box_item_count = gtk_tree_model_iter_n_children(gtk_combo_box_get_model(CustomDigit[target_index]), NULL);
    gtk_combo_box_set_active(CustomDigit[target_index], combo_box_item_count - 1);

    if (gtk_toggle_button_get_active(CustomAutoIncrement))
        gtk_spin_button_set_value(CustomDigitApplyIndex, gtk_spin_button_get_value(CustomDigitApplyIndex) + 1);
}

static void set_all_custom_digits(GtkWidget *widget, gpointer user_data) {
    for (int i=0; i<4; i++) set_custom_digit(widget, i + 1);
}

static void check_custom_pixels(GtkWidget *widget, gpointer button_index) {
    if (ctrl_pressed) return;

    gint segmentStartIndex = (gint) button_index / 3;
    for (int i=segmentStartIndex * 3; i<segmentStartIndex * 3 + 3; i++)
        gtk_toggle_button_set_active(CustomSegmentNumber[i], gtk_toggle_button_get_active(widget));
}

// countdown
static void apply_countdown(GtkWidget *widget, gpointer user_data) {
    gint countdown_total = gtk_spin_button_get_value_as_int(CountdownValue);
    gint countdown_pauseable = gtk_switch_get_active(CountdownPauseable);
    gint countdown_seconds_only = gtk_switch_get_active(CountdownSecondsOnly);

    if (countdown_total < 5) {
        show_message_dialog(MainWindow, GTK_MESSAGE_ERROR, GTK_BUTTONS_OK, "Cannot start countdown", "The countdown must be at least 5 seconds!");
        return;
    }
    if (countdown_total > 5999 && !countdown_seconds_only) {
        show_message_dialog(MainWindow, GTK_MESSAGE_ERROR, GTK_BUTTONS_OK, "Cannot start countdown", "Activate the option 'Display countdown with seconds only' to exceed 5999 seconds.");
        return;
    }

    gchar *post_url[PATH_MAX];
    gchar *post_string[64];

    sprintf(post_url, "http://%s/countdown", hostname);
    sprintf(post_string, "t=%i&pauseable=%i&secondsonly=%i", countdown_total, countdown_pauseable, countdown_seconds_only);

    gchar *result = request("POST", post_url, username, password, post_string);
    if (result && strstr(result, "Done!")) {
        // countdown successfully started
        show_message_dialog(MainWindow, GTK_MESSAGE_INFO, GTK_BUTTONS_OK, "Success!", "Countdown is running now!");
    } else {
        // error while trying to start countdown
        show_message_dialog(MainWindow, GTK_MESSAGE_ERROR, GTK_BUTTONS_OK, "Failed to start countdown", "An error occurred while starting the countdown. Please try again later.");
    }
}

static void stop_countdown(GtkWidget *widget, gpointer user_data) {
    gchar *delete_url[PATH_MAX];
    sprintf(delete_url, "http://%s/countdown", hostname);

    gchar *stop = request("DELETE", delete_url, username, password, "");

    if (stop && request_last_status_code == 200) {
        // countdown stopped successfully
        show_message_dialog(MainWindow, GTK_MESSAGE_INFO, GTK_BUTTONS_OK, "Success!", "The countdown has been stopped!");
    } else if (stop && request_last_status_code == 400) {
        // no countdown active
        show_message_dialog(MainWindow, GTK_MESSAGE_WARNING, GTK_BUTTONS_OK, "Failed to stop countdown", "There is no countdown active right now.");
    } else {
        // error while trying to stop countdown
        show_message_dialog(MainWindow, GTK_MESSAGE_ERROR, GTK_BUTTONS_OK, "Failed to stop countdown", "An error occurred while trying to stop the countdown. Please try again later.");
    }
}

// Sunrise Siren 3000 information
void fetch_information() {
    gchar* information_url[PATH_MAX];
    sprintf(information_url, "http://%s/information", hostname);
    gchar *information_response = request("GET", information_url, username, password, "");

    if (information_response) {
        if (clock_information = json_tokener_parse(information_response)) {
            gchar *fw_version_label[8];
            gchar *brightness_label[3];
            gchar *ldr_label[4];
            gchar *temperature_label[20];
            gchar *humidity_label[20];

            sprintf(fw_version_label, "%i.%i", request_last_firmware_version, json_object_get_int(json_object_object_get(clock_information, "subversion")));
            sprintf(brightness_label, "%d", json_object_get_int(json_object_object_get(clock_information, "brightness")));
            sprintf(ldr_label, "%d", json_object_get_int(json_object_object_get(clock_information, "ldr")));
            sprintf(
                temperature_label,
                "%i → %.2f °C",
                json_object_get_int(json_object_object_get(json_object_object_get(clock_information, "temperature"), "raw")),
                json_object_get_double(json_object_object_get(json_object_object_get(clock_information, "temperature"), "translated"))
            );
            sprintf(
                humidity_label,
                "%i → %.2f%%",
                json_object_get_int(json_object_object_get(json_object_object_get(clock_information, "humidity"), "raw")),
                json_object_get_double(json_object_object_get(json_object_object_get(clock_information, "humidity"), "translated"))
            );

            gtk_label_set_label(FirmwareVersionReading, fw_version_label);
            gtk_label_set_label(NeoPixelBrightnessReading, brightness_label);
            gtk_label_set_label(LDRReading, ldr_label);
            gtk_label_set_label(SHT21TemperatureReading, temperature_label);
            gtk_label_set_label(SHT21HumidityReading, humidity_label);
        }
    }
}

static void goto_github(GtkWidget *widget, gpointer pathname) {
    gchar* cmd[75];
    sprintf(cmd, "xdg-open https://github.com/ngkon6/SunriseSiren3000%s", pathname);

    system(cmd);
}

// save actions
static void apply_clock_settings(GtkWidget *widget, gpointer user_data) {
    // step 1: collect all settings
    GdkRGBA *default_c = g_new(GdkRGBA, 1);
    GdkRGBA *highlight_c = g_new(GdkRGBA, 1);
    gtk_color_chooser_get_rgba(DefaultColor, &*default_c);
    gtk_color_chooser_get_rgba(HighlightColor, &*highlight_c);

    glong default_c_number = (int) (default_c->red * 255) * (int) pow(256, 2)
        + (int) (default_c->green * 255) * (int) pow(256, 1)
        + (int) (default_c->blue * 255) * (int) pow(256, 0);
    glong highlight_c_number = (int) (highlight_c->red * 255) * (int) pow(256, 2)
        + (int) (highlight_c->green * 255) * (int) pow(256, 1)
        + (int) (highlight_c->blue * 255) * (int) pow(256, 0);

    gint alarms_enabled = 0;
    gint alarm_hours[7];
    gint alarm_minutes[7];

    for (int i=0; i<7; i++) {
        if (gtk_switch_get_active(AlarmEnable[i])) alarms_enabled += (int) pow(2, i);

        alarm_hours[i] = gtk_spin_button_get_value_as_int(AlarmHour[i]);
        alarm_minutes[i] = gtk_spin_button_get_value_as_int(AlarmMinute[i]);
    }

    gchar* alarm_times[28];
    sprintf(alarm_times,
        "%02i%02i%02i%02i%02i%02i%02i%02i%02i%02i%02i%02i%02i%02i",
        alarm_hours[0], alarm_minutes[0],
        alarm_hours[1], alarm_minutes[1],
        alarm_hours[2], alarm_minutes[2],
        alarm_hours[3], alarm_minutes[3],
        alarm_hours[4], alarm_minutes[4],
        alarm_hours[5], alarm_minutes[5],
        alarm_hours[6], alarm_minutes[6]
    );

    gint leading_zero = gtk_switch_get_active(EnableLeadingZero);
    gint dst = gtk_switch_get_active(EnableDST);
    gint snooze_t = gtk_spin_button_get_value_as_int(SnoozeInterval) * 1000;
    gint return_after = gtk_spin_button_get_value_as_int(ClockReturn) * 1000;
    gint ldr_min = gtk_spin_button_get_value_as_int(LDRMin);
    gint ldr_max = gtk_spin_button_get_value_as_int(LDRMax);

    // step 2: make a request string
    gchar *post_url[PATH_MAX];
    gchar *post_string[512];

    sprintf(post_url, "http://%s/update", hostname);
    sprintf(
        post_string,
        "default-c=%ld&highlight-c=%ld&alarms-enabled=%i&alarm-times=%s&leading-zero=%i&enable-dst=%i&snooze-t=%i&clock-return=%i&ldr-min=%i&ldr-max=%i",
        default_c_number, highlight_c_number, alarms_enabled, alarm_times, leading_zero, dst, snooze_t, return_after, ldr_min, ldr_max
    );

    // step 3: yeet a request
    gchar *result = request("POST", post_url, username, password, post_string);
    if (result && strstr(result, "Done!")) {
        // settings successfully applied
        show_message_dialog(MainWindow, GTK_MESSAGE_INFO, GTK_BUTTONS_OK, "Success!", "Changes have been saved and applied!");
    } else {
        // error while trying to apply settings
        show_message_dialog(MainWindow, GTK_MESSAGE_ERROR, GTK_BUTTONS_OK, "Failed to save settings", "An error occurred while trying to save and apply the settings. Please try again later.");
    }

    gtk_list_box_unselect_all(AlarmListBox);
    gtk_widget_hide(HighlightInfoLabel);
}

static void apply_custom_settings(GtkWidget *widget, gpointer user_data) {
    // step 1: collect all settings
    GdkRGBA *custom_colors[5]; // 0-3 are digits, 4 is colon
    glong color_numbers[4];
    gint custom_digits[4];

    for (int i=0; i<4; i++) {
        custom_colors[i] = g_new(GdkRGBA, 1);
        gtk_color_chooser_get_rgba(CustomColor[i], &*custom_colors[i]);

        color_numbers[i] = (int) (custom_colors[i]->red * 255) * (int) pow(256, 2)
            + (int) (custom_colors[i]->green * 255) * (int) pow(256, 1)
            + (int) (custom_colors[i]->blue * 255) * (int) pow(256, 0);

        gint combo_box_number = custom_combo_box_get_corresponding_number(CustomDigit[i]);
        custom_digits[i] = (combo_box_number == 0) ? atoi(gtk_entry_get_text(CustomDigitEntry[i])) : combo_box_number;
    }

    custom_colors[4] = g_new(GdkRGBA, 1);
    gtk_color_chooser_get_rgba(CustomColor_Colon, &*custom_colors[4]);
    glong color_colon = (int) (custom_colors[4]->red * 255) * (int) pow(256, 2)
        + (int) (custom_colors[4]->green * 255) * (int) pow(256, 1)
        + (int) (custom_colors[4]->blue * 255) * (int) pow(256, 0);

    // step 2: make a request string
    gchar *segment_strings[4][64];
    gchar *post_url[PATH_MAX];
    gchar *post_string[512];

    sprintf(post_url, "http://%s/custom", hostname);
    for (int i=0; i<4; i++) sprintf(
        segment_strings[i],
        "segment-%i=%i&color-%i=%ld",
        i, custom_digits[i], i, color_numbers[i]
    );
    sprintf(
        post_string,
        "%s&%s&%s&%s&color-colon=%ld",
        segment_strings[0],
        segment_strings[1],
        segment_strings[2],
        segment_strings[3],
        color_colon
    );

    // step 3: yeet a request
    gchar *result = request("POST", post_url, username, password, post_string);
    if (result && strstr(result, "Done!")) {
        // custom config successfully set
        show_message_dialog(MainWindow, GTK_MESSAGE_INFO, GTK_BUTTONS_OK, "Success!", "Custom configuration has been saved and is visible now!");
    } else {
        // failed to set custom config
        show_message_dialog(MainWindow, GTK_MESSAGE_ERROR, GTK_BUTTONS_OK, "Failed to show custom mode", "An error occurred while trying to save and show the custom mode. Please try again later.");
    }
}

// save actions caller: the universal confirm!
static void universal_confirm(GtkWidget *widget, gpointer user_data) {
    gchar* current_page = gtk_stack_get_visible_child_name(MainStack);

    if (strstr(current_page, "Colors") || strstr(current_page, "Alarms") || strstr(current_page, "Settings"))
        apply_clock_settings(widget, user_data);
    else if (strstr(current_page, "CustomMode"))
        apply_custom_settings(widget, user_data);
    else if (strstr(current_page, "Countdown"))
        apply_countdown(widget, user_data);
    else
        show_message_dialog(MainWindow, GTK_MESSAGE_ERROR, GTK_BUTTONS_OK, "Unable to save", "Navigate to a menu to save/apply its contents.");
}

// ConnectionWindow
static void create_connection(GtkWidget *widget, gpointer user_data) {
    gchar* hostname = gtk_entry_get_text(ConnectHostname);
    gchar* username = gtk_entry_get_text(ConnectUsername);
    gchar* password = gtk_entry_get_text(ConnectPassword);

    if (!strlen(hostname) || !strlen(username) || !strlen(password)) {
        show_message_dialog(ConnectionWindow, GTK_MESSAGE_WARNING, GTK_BUTTONS_OK, "Unable to connect", "Please enter all the fields!");
        return;
    }

    gchar* url[PATH_MAX];
    sprintf(url, "http://%s/connect", hostname);

    gchar* req = request("GET", url, username, password, "");

    if (req && strstr(req, VALIDATION_STRING)) {
        // authentication successful, save and continue

        credentials_write(hostname, username, password);
        set_connect_immediately(TRUE);
        reboot_program();
    } else if (req && request_last_status_code == 401) {
        // incorrect username or password
        show_message_dialog(ConnectionWindow, GTK_MESSAGE_WARNING, GTK_BUTTONS_OK, "Unable to connect", "Incorrect username or password!");
    } else if (req) {
        // failed to validate
        show_message_dialog(ConnectionWindow, GTK_MESSAGE_ERROR, GTK_BUTTONS_OK, "Unable to connect", "Failed to validate the connection.");
    } else {
        // request failed, nothing found at all
        show_message_dialog(ConnectionWindow, GTK_MESSAGE_ERROR, GTK_BUTTONS_OK, "Unable to connect", "Failed to connect to your Sunrise Siren 3000.");
    }
}

// LoginChangeWindow (actually a dialog here too)
static void reconfigure_clock(GtkWidget *widget, gpointer user_data) {
    // step 1: collect all data
    gchar* old_password = gtk_entry_get_text(ChangeOldPassword);
    gchar* new_username = gtk_entry_get_text(ChangeNewUsername);
    gchar* new_password = gtk_entry_get_text(ChangeNewPassword);
    gchar* new_password_2 = gtk_entry_get_text(ChangeNewPasswordRetype);

    if (!strlen(old_password) || !strlen(new_username) || !strlen(new_password) || !strlen(new_password_2)) {
        show_message_dialog(ConnectionWindow, GTK_MESSAGE_WARNING, GTK_BUTTONS_OK, "Unable to reset credentials", "Please enter all the fields!");
        return;
    } else if (!g_str_equal(new_password, new_password_2)) {
        show_message_dialog(ConnectionWindow, GTK_MESSAGE_ERROR, GTK_BUTTONS_OK, "Unable to reset credentials", "The new passwords do not match!");
        return;
    }

    // step 2: make a request string
    gchar* url[PATH_MAX];
    gchar *post_string[512];
    sprintf(url, "http://%s/set-login", hostname);
    sprintf(post_string, "user=%s&passwd=%s", new_username, new_password);

    // step 3: yeet a request
    gchar *req = request("POST", url, username, old_password, post_string);
    if (req && request_last_status_code == 204) {
        // successful: wipe saved credentials and restart
        credentials_reset();
        reboot_program();
    } else if (req && request_last_status_code == 401) {
        show_message_dialog(LoginChangeWindow, GTK_MESSAGE_ERROR, GTK_BUTTONS_OK, "Unable to reset credentials", "The old password is incorrect!");
    } else if (req && strstr(req, "UsernameLength")) {
        show_message_dialog(LoginChangeWindow, GTK_MESSAGE_ERROR, GTK_BUTTONS_OK, "Unable to reset credentials", "Username needs to be at least 2 and at most 32 characters!");
    } else if (req && strstr(req, "PasswordLength")) {
        show_message_dialog(LoginChangeWindow, GTK_MESSAGE_ERROR, GTK_BUTTONS_OK, "Unable to reset credentials", "Password needs to be at least 8 and at most 32 characters!");
    } else if (req && strstr(req, "PasswordDefault")) {
        show_message_dialog(LoginChangeWindow, GTK_MESSAGE_ERROR, GTK_BUTTONS_OK, "Unable to reset credentials", "The new password cannot be the same as the default password.");
    } else if (req) {
        // another error occured
        show_message_dialog(LoginChangeWindow, GTK_MESSAGE_ERROR, GTK_BUTTONS_OK, "Unable to reset credentials", "Failed to process the new credentials.\nPlease try again later.");
    } else {
        // request failed
        show_message_dialog(ConnectionWindow, GTK_MESSAGE_ERROR, GTK_BUTTONS_OK, "Unable to reset credentials", "Failed to connect to your Sunrise Siren 3000.");
    }
}


// key event handlers
ctrl_pressed = FALSE;

static void main_window_key_press(GtkWidget *widget, GdkEventKey *e, gpointer data) {
    if (e->keyval == 65507) ctrl_pressed = TRUE; // Left Ctrl
    else if (e->keyval == 65307) go_to(widget, "MainMenu"); // Esc
    else if (e->keyval == 82 && ctrl_pressed) reboot_program(); // R
    else if (e->keyval == 114 && ctrl_pressed) reboot_clock(); // r
    else if (e->keyval == 119 && ctrl_pressed) quit(); // w
    else if (e->keyval == 115 && ctrl_pressed) { // s
        universal_confirm(widget, data);
        ctrl_pressed = FALSE;
    } else do_absolutely_nothing(); // idk WHY I have to do this, but I have to
}

static void main_window_key_release(GtkWidget *widget, GdkEventKey *e, gpointer data) {
    if (e->keyval == 65507) ctrl_pressed = FALSE;
    else do_absolutely_nothing();
}

static void connection_window_key_press(GtkWidget *widget, GdkEventKey *e, gpointer data) {
    if (e->keyval == 65293) create_connection(widget, data); // Enter
    else do_absolutely_nothing();
}

#endif
