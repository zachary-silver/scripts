/* Global macros */
#define MAX_DATE_OUTPUT 64
#define MAX_SONG_OUTPUT 64
#define MAX_JSON_VALUE_OUTPUT 64
#define LENGTHOF(x) (int)(sizeof(x) / sizeof((x)[0]))

/* Global constants */
static const char   *PAUSE_ICON = "";
static const char   *PLAY_ICON = "";
static const char   *STOP_ICON = "";
static const char   *LIKED_ICON = "";
static const char   *WIFI_ICON = "";
static const char   *MEMORY_ICON = "";
static const char   *DISK_ICON = "";
static const char   *CPU_ICON = "";
static const char   *SPEAKER_ICON_UNMUTED = "";
static const char   *SPEAKER_ICON_MUTED = "";
static const char   *DATE_ICON = "";
static const char   *TIME_ICON = "";
static const char   *TIME_ICON_WITH_SPACE = " ";
static const char   *BATTERY_ICON_FULL = "";
static const char   *BATTERY_ICON_THREE_QUARTERS = "";
static const char   *BATTERY_ICON_HALF = "";
static const char   *BATTERY_ICON_QUARTER = "";
static const char   *BATTERY_ICON_EMPTY = "";
static const char   *TIME_FORMAT = "%l:%M %p";
static const char   *DATE_FORMAT = "%a %b/%d/%Y";
static const char   *ENERGY_NOW_FILE_BAT0 = "/sys/class/power_supply/BAT0/energy_now";
static const char   *ENERGY_NOW_FILE_BAT1 = "/sys/class/power_supply/BAT1/energy_now";
static const char   *ENERGY_FULL_FILE_BAT0 = "/sys/class/power_supply/BAT0/energy_full";
static const char   *ENERGY_FULL_FILE_BAT1 = "/sys/class/power_supply/BAT1/energy_full";
static const struct timespec SLEEP_TIME = { .tv_sec = 1, .tv_nsec = 000000000 };
static const float  GIGABYTE = 1024 * 1024 * 1024;

/* Function declarations */
float get_batteries(void);
void set_memory_output(void);
void set_status(void);
void set_time_icon(void);
void set_volume_and_icon(void);
void set_wifi_output(void);
void set_cpu_output(void);
void set_disk_space_output(void);
void set_battery_icon(void);
void set_song_output(char song_output[MAX_SONG_OUTPUT]);
void set_date(const char *date_format, char date_output[MAX_DATE_OUTPUT]);
float get_battery(const char *energy_now_file, const char *energy_full_file);
void set_json_value(char json_value[MAX_JSON_VALUE_OUTPUT], const char *json);
int concat_strings(char first[MAX_SONG_OUTPUT], const char *second, int start);
