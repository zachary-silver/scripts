#include <string.h>

#include "dwm_status.h"

#define NULL_JSON_FILE_SIZE 390
#define MAX_JSON_FILE_SIZE 8192

static const char *GPMDP_JSON_FILE = "/home/zack/.config/Google Play Music Desktop Player/json_store/playback.json";

static char json_data[MAX_JSON_FILE_SIZE], *json_data_ptr,
            song_playing[MAX_JSON_VALUE_OUTPUT],
            song_title[MAX_JSON_VALUE_OUTPUT],
            song_artist[MAX_JSON_VALUE_OUTPUT],
            song_liked[MAX_JSON_VALUE_OUTPUT],
            song_current_time[MAX_JSON_VALUE_OUTPUT],
            song_total_time[MAX_JSON_VALUE_OUTPUT],
            song_time_left[MAX_TIME_OUTPUT];

static char *song_metadata[] =
{
    song_playing,
    song_title,
    song_artist,
    song_liked,
    song_current_time,
    song_total_time
};

static const char *song_metadata_queries[] =
{
    "playing",
    "title",
    "artist",
    "liked",
    "current",
    "total"
};

void set_song_output(char song_output[MAX_SONG_OUTPUT])
{
    static const int max_file_size = MAX_JSON_FILE_SIZE - 1;
    int total_seconds_left = 0, minutes_left = 0, seconds_left = 0,
        i = 0, start = 0;
    char character;
    FILE *fd;

    json_data_ptr = json_data;

    fd = fopen(GPMDP_JSON_FILE, "r");
    if (fd == NULL) {
        return;
    }

    while ((character = getc(fd)) != EOF && i < max_file_size) {
        json_data[i++] = character;
    }
    json_data[i] = '\0';

    fclose(fd);

    if (i <= NULL_JSON_FILE_SIZE) {
        return;
    }

    for (i = 0; i < LENGTH_OF(song_metadata); i++) {
        json_data_ptr = strstr(json_data_ptr, song_metadata_queries[i]);
        set_json_value(song_metadata[i], json_data_ptr);
    }

    total_seconds_left = (atoi(song_total_time) - atoi(song_current_time)) / 1000;
    minutes_left = total_seconds_left / 60;
    seconds_left = total_seconds_left % 60;

    sprintf(song_time_left, "%i:%02i", minutes_left, seconds_left);

    song_output[0] = '\0';

    if (song_playing[0] == 't') {
        start = concat_strings(song_output, PLAY_ICON, start);
    } else {
        start = concat_strings(song_output, PAUSE_ICON, start);
    }

    if (song_liked[0] == 't') {
        start = concat_strings(song_output, LIKED_ICON, start);
    }

    start = concat_strings(song_output, song_title, start);
    start = concat_strings(song_output, "-", start);
    start = concat_strings(song_output, song_artist, start);

    if (start > MAX_SONG_OUTPUT - 8) {
        for (i = MAX_SONG_OUTPUT - 10; i < MAX_SONG_OUTPUT - 7; i++) {
            song_output[i] = '.';
        }
        song_output[i] = '\0';
        start = i;
    }

    concat_strings(song_output, song_time_left, start);
}

void set_json_value(char json_value[MAX_JSON_VALUE_OUTPUT], const char *json_data)
{
    if (json_data_ptr == NULL) {
        return;
    }
    int i = 0, j = 0;

    while (json_data[i] != ':') {
        i++;
    }

    do {
        i++;
    } while (json_data[i] == ' ');

    if (json_data[i] == '"') {
        i++;
    }

    while (json_data[i] != '\n') {
        json_value[j++] = json_data[i++];
    }

    do {
        j--;
    } while (json_value[j] == ',' || json_value[j] == '"');

    json_value[j + 1] = '\0';
}

/*
 * Concatenates two strings with a space between and returns the ending
 * index to save progress for future calls.
 */
int concat_strings(char first[MAX_SONG_OUTPUT], const char *second, int start)
{
    static const int max_length = MAX_SONG_OUTPUT - 1;
    int i = start, j = 0;

    if (start < 0 || start > max_length) {
        return MAX_SONG_OUTPUT;
    }

    while (first[i] != '\0') {
        i++;
    }

    first[i++] = ' ';

    while (second[j] != '\0' && i < max_length) {
        first[i++] = second[j++];
    }

    first[i] = '\0';

    return i;
}
