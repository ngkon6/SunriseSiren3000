#ifndef _requests_h
#define _requests_h

gint request_last_status_code = 0;
gint request_last_firmware_version = -1;

size_t write_memory_callback(void *ptr, size_t size, size_t nmemb, void *data) {
    memcpy(data, ptr, size * nmemb);
    return size * nmemb;
}

gchar* str_escape(gchar* input, gchar to_escape) {
    gint len = strlen(input);
    gchar* escaped = (gchar*)g_malloc(2 * len + 1);
    gint j = 0;

    for (gint i=0; i<len; i++) {
        if (input[i] == to_escape) {
            escaped[j++] = '\\';
        }
        escaped[j++] = input[i];
    }

    escaped[j] = '\0';
    return escaped;
}

gchar* request(gchar* method, gchar* url, gchar* username, gchar* password, gchar* post_data) {
    CURL *curl = curl_easy_init();
    struct curl_header *fwversion_header;
    if (!curl) return "Error while initializing CURL";

    char* ua[24];
    sprintf(ua, "sunrise-siren-studio/%s", STUDIO_VERSION);

    gchar *userpwd[strlen(username) + strlen(password) + 32];
    sprintf(userpwd, "%s:%s", str_escape(username, ':'), str_escape(password, ':'));

    gchar *response_buffer = g_malloc(2048 * sizeof(gchar));
    if (!response_buffer) return NULL;

    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_USERPWD, userpwd);
    curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, method);
    curl_easy_setopt(curl, CURLOPT_USERAGENT, ua);
    if (method == "POST") {
        curl_easy_setopt(curl, CURLOPT_POST, 1L);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, post_data);
    } else curl_easy_setopt(curl, CURLOPT_HTTPGET, 1L);

    curl_easy_setopt(curl, CURLOPT_TIMEOUT, 12L);
    curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 8L);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_memory_callback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, response_buffer);
    CURLcode response = curl_easy_perform(curl);

    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &request_last_status_code);
    curl_easy_header(curl, "Firmware-Version", 0, CURLH_HEADER, -1, &fwversion_header);
    if (fwversion_header) request_last_firmware_version = atoi(fwversion_header->value);

    if (response == CURLE_OK) return response_buffer;

    curl_global_cleanup();
    g_free(response_buffer);
    return NULL;
}

#endif
