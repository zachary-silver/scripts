#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "dwm_status.h"

#define MAX_TIME_OUTPUT 6
#define MAX_SONG_OUTPUT 64
#define NULL_JSON_FILE_SIZE 390
#define MAX_JSON_VALUE_OUTPUT 64
#define MAX_JSON_FILE_SIZE 10240

static const char *GPMDP_JSON_FILE = "/home/zack/.config/Google Play Music Desktop Player/json_store/playback.json";

void set_song_output(char song_output[MAX_SONG_OUTPUT])
{
    static char json_data[MAX_JSON_FILE_SIZE], *json_data_ptr,
                song_playing[MAX_JSON_VALUE_OUTPUT],
                song_title[MAX_JSON_VALUE_OUTPUT],
                song_artist[MAX_JSON_VALUE_OUTPUT],
                song_liked[MAX_JSON_VALUE_OUTPUT],
                song_current_time[MAX_JSON_VALUE_OUTPUT],
                song_total_time[MAX_JSON_VALUE_OUTPUT],
                song_time[MAX_TIME_OUTPUT];
    static char *song_metadata[] =
    {
        song_playing,
        song_title,
        song_artist,
        song_liked,
        song_current_time,
        song_total_time
    };
    static char *song_metadata_queries[] =
    {
        "playing",
        "title",
        "artist",
        "liked",
        "current",
        "total"
    };
    static int i, start, time_left, minutes, seconds,
               max_file_size = MAX_JSON_FILE_SIZE - 1;
    static char character;
    static FILE *fd;

    json_data_ptr = json_data;

    fd = fopen(GPMDP_JSON_FILE, "r");
    if (fd == NULL)
    {
        return;
    }
    /* Load the characters from the json file into json_data until */
    /* the end of the file is reached or the maximum buffer size is reached */
    for (i = 0; (character = getc(fd)) != EOF && i < max_file_size; i++)
    {
        json_data[i] = character;
    }
    json_data[i] = '\0';
    fclose(fd);

    /* Greedy method of avoiding printing out null values from the json file */
    /* produced when GPMDP isn't running. It's highly unlikely that there */
    /* will be an active json output file that is shorter than the null one */
    /* produced when the program isn't running */
    if (i <= NULL_JSON_FILE_SIZE)
    {
        song_output[0] = '\0';
        return;
    }

    /* Search for the song metadata json keys specified in */
    /* song_metadata_queries and set the value of the appropriate */
    /* song_metadata array */
    for (i = 0; i < LENGTHOF(song_metadata); i++)
    {
        json_data_ptr = strstr(json_data_ptr, song_metadata_queries[i]);
        if (json_data_ptr == NULL)
        {
            return;
        }
        set_json_value(song_metadata[i], json_data_ptr);
    }

    time_left = (atoi(song_total_time) - atoi(song_current_time)) / 1000;
    minutes = time_left / 60;
    seconds = time_left % 60;
    sprintf(song_time, "%i:%02i", minutes, seconds);

    start = 0;
    /* Logically clear the previous string */
    song_output[0] = '\0';

    if (song_playing[0] == 't')
    {
        start = concat_strings(song_output, PLAY_ICON, start);
    }
    else
    {
        start = concat_strings(song_output, PAUSE_ICON, start);
    }

    if (song_liked[0] == 't')
    {
        start = concat_strings(song_output, LIKED_ICON, start);
    }

    start = concat_strings(song_output, song_title, start);
    start = concat_strings(song_output, "-", start);
    start = concat_strings(song_output, song_artist, start);

    /* If the song output is too long, shorten */
    /* with "..." before concatenating song_time */
    if (start > MAX_SONG_OUTPUT - 8)
    {
        for (i = MAX_SONG_OUTPUT - 10; i < MAX_SONG_OUTPUT - 7; i++)
        {
            song_output[i] = '.';
        }
        song_output[i] = '\0';

        start = i;
    }

    concat_strings(song_output, song_time, start);
}

void set_json_value(char json_value[MAX_JSON_VALUE_OUTPUT], const char *json_data)
{
    static char character, delimeter;
    static int i, j;

    /* Reach the end of the key. Marked by a ':' */
    for (i = 0; json_data[i] != ':'; i++);
    /* Step forward until its value is reached. Marked by non-whitespace */
    for (i++; json_data[i] == ' '; i++);

    /* In this context, the json value will either be a string */
    /* (indicated by a double quote), a boolean, or an integer. */
    character = json_data[i];
    if (character == '"')
    {
        character = json_data[++i];
        delimeter = '"';
    }
    else
    {
        delimeter = ' ';
    }

    /* In this context, the end of the json value will be indicated */
    /* by either the set delimeter or a new line character */
    for (j = 0, i++; character != delimeter && character != '\n'; i++, j++)
    {
        json_value[j] = character;
        character = json_data[i];
    }

    /* If the json value is not the last within a json object, */
    /* there will be a comma that needs to be excluded */
    /* by inserting the null terminator there instead */
    if (json_value[j - 1] == ',')
    {
        json_value[j - 1] = '\0';
    }
    else
    {
        json_value[j] = '\0';
    }
}

int concat_strings(char first[MAX_SONG_OUTPUT], const char *second, int start)
{
    static int max_length = MAX_SONG_OUTPUT - 1, i, j;

    if (start < 0 || start > max_length)
    {
        return -1;
    }

    /* Reach the end of the first string if we're not already there */
    for (i = start; first[i] != '\0'; i++);

    /* Concatenate the strings with a space between */
    first[i++] = ' ';

    /* Append characters from second onto first until we've reached */
    /* the end of either and mark the new end with a null terminator */
    for (j = 0; second[j] != '\0' && i < max_length; i++, j++)
    {
        first[i] = second[j];
    }
    first[i] = '\0';

    /* Return the new ending index to save progress */
    return i;
}
