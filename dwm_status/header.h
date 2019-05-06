/* Global constants */
static const char   *WIFI_ICON = "";
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
static const struct timespec sleep_time = { .tv_sec = 0, .tv_nsec = 200000000 };
