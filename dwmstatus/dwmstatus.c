/* Compile with -lX11 and -lasound flags */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <alsa/asoundlib.h>
#include <alsa/control.h>
#include <sys/sysinfo.h>
#include <sys/statvfs.h>
#include <X11/Xlib.h>

#include "dwmstatus.h"
#include "io.h"

/* Macros */
#define CPU_LOAD_INFO_COLUMNS 7
#define CPU_LOAD_IDLE_COLUMN 3
#define MAX_SONG_TIME_OUTPUT 6
#define MAX_BUFFER_SIZE 1024
#define MAX_STATUS_OUTPUT 512
#define MAX_ARTIST_OUTPUT 64
#define MAX_TITLE_OUTPUT 64
#define MAX_SONG_OUTPUT 64
#define MAX_MEMORY_OUTPUT 8
#define MAX_VOLUME_OUTPUT 8
#define MAX_DATE_OUTPUT 32
#define MAX_TIME_OUTPUT 16
#define MAX_WIFI_OUTPUT 16
#define MAX_DISK_OUTPUT 16
#define MAX_CPU_OUTPUT 16
#define LENGTH_OF(x) (int)(sizeof(x) / sizeof((x)[0]))

/* Typedefs */
typedef struct {
    int active;
    float strength;
    const char *icon;
} dwmWifi;

typedef struct {
   int active;
   float percent;
   unsigned long currentWh;
   unsigned long capacityWh;
   const char *icon;
} dwmBattery;

typedef struct {
   float utilization;
   unsigned long idleJiffies;
   unsigned long loadJiffies;
   const char *icon;
} dwmCPU;

typedef struct {
    char output[MAX_DATE_OUTPUT];
    const char *format;
    const char *icon;
} dwmDate, dwmTime;

typedef struct {
    unsigned long usedBytes;
    unsigned long totalBytes;
    const char *icon;
} dwmDisk;

typedef struct {
    unsigned long usedBytes;
    unsigned long totalBytes;
    const char *icon;
} dwmMemory;

typedef struct {
    float percent;
    long current;
    long min;
    long max;
    int muted;
    const char *icon;
} dwmVolume;

typedef struct {
   Display *display;
   dwmBattery battery;
   dwmMemory memory;
   dwmVolume volume;
   dwmDate date;
   dwmTime time;
   dwmDisk disk;
   dwmSong song;
   dwmWifi wifi;
   dwmCPU cpu;
   char output[MAX_STATUS_OUTPUT];
} dwmStatus;

/* Constants */
static const struct timespec SleepTime = { .tv_sec = 1, .tv_nsec = 000000000 };
static const float           Gigabyte = 1024 * 1024 * 1024;
static const char *const     PauseIcon = "";
static const char *const     PlayIcon = "";
static const char *const     StopIcon = "";
static const char *const     LikedIcon = "";
static const char *const     DislikedIcon = "";
static const char *const     WifiIcon = "";
static const char *const     MemoryIcon = "";
static const char *const     DiskIcon = "";
static const char *const     CPUIcon = "";
static const char *const     SpeakerIconUnmuted = "";
static const char *const     SpeakerIconMuted = "";
static const char *const     DateIcon = "";
static const char *const     TimeIcon = "";
static const char *const     TimeIconWithSpace = " ";
static const char *const     BatteryIcon100 = "";
static const char *const     BatteryIcon75 = "";
static const char *const     BatteryIcon50 = "";
static const char *const     BatteryIcon25 = "";
static const char *const     BatteryIcon0 = "";
static const char *const     TimeFormat = "%l:%M %p";
static const char *const     DateFormat = "%a %b/%d/%Y";
static const char *const     WirelessInterface = "wlp4s0";
static const char *const     CurrentBatteryFiles[] =
{
   "/sys/class/power_supply/BAT0/energy_now",
   "/sys/class/power_supply/BAT1/energy_now",
};
static const char *const     CapacityBatteryFiles[] =
{
   "/sys/class/power_supply/BAT0/energy_full",
   "/sys/class/power_supply/BAT1/energy_full",
};

/* Function declarations */
unsigned long getBatteryValue(const char *fileName);
int getQualityLinkValue(char *wirelessInterfaceInfo);
void output(dwmStatus *status);
void setOutput(dwmStatus *status);
void setBattery(dwmBattery *battery);
void setBatteryIcon(dwmBattery *battery);
void setMemory(dwmMemory *memory);
void setVolume(dwmVolume *volume);
void setVolumeIcon(dwmVolume *volume);
void setWifi(dwmWifi *wifi);
void setCPU(dwmCPU *cpu);
void setDisk(dwmDisk *disk);
void setSongIcons(dwmSong *song);
void setSongOutput(dwmSong *song);
void setDate(dwmDate *date);
void setTime(dwmTime *time);
void setTimeIcon(dwmTime *time);

int main(void)
{
    dwmStatus status = { 0 };
    status.display = XOpenDisplay(NULL);
    status.date.icon = DateIcon;
    status.wifi.icon = WifiIcon;
    status.disk.icon = DiskIcon;
    status.memory.icon = MemoryIcon;
    status.cpu.icon = CPUIcon;
    status.date.format = DateFormat;
    status.time.format = TimeFormat;

loop:
    setDate(&status.date);
    setTime(&status.time);
    setTimeIcon(&status.time);
    setBattery(&status.battery);
    setBatteryIcon(&status.battery);
    setVolume(&status.volume);
    setVolumeIcon(&status.volume);
    setMemory(&status.memory);
    setDisk(&status.disk);
    setCPU(&status.cpu);
    setWifi(&status.wifi);
    setSong(&status.song);
    setSongIcons(&status.song);
    setSongOutput(&status.song);
    setOutput(&status);

    output(&status);

    nanosleep(&SleepTime, NULL);
    goto loop;
}

void setOutput(dwmStatus *status)
{
    int volume, usedMemory, totalMemory, usedDisk, totalDisk;
    volume = status->volume.percent + 0.5;
    usedMemory = status->memory.usedBytes / Gigabyte + 0.5;
    totalMemory = status->memory.totalBytes / Gigabyte + 0.5;
    usedDisk = status->disk.usedBytes / Gigabyte + 0.5;
    totalDisk = status->disk.totalBytes / Gigabyte + 0.5;

    snprintf(status->output, MAX_STATUS_OUTPUT,
             "%s  %s %.1f%%  %s %.1f%%  %s %d/%dG  %s %d/%dG  ",
             status->song.output,
             status->wifi.icon, status->wifi.strength,
             status->cpu.icon, status->cpu.utilization,
             status->memory.icon, usedMemory, totalMemory,
             status->disk.icon, usedDisk, totalDisk);

    snprintf(status->output + strlen(status->output), MAX_STATUS_OUTPUT,
             "%s %d%%  %s %.0f%%  %s %s  %s%s",
             status->volume.icon, volume,
             status->battery.icon, status->battery.percent,
             status->date.icon, status->date.output,
             status->time.icon, status->time.output);
}

/*
 * In conditions where values can't be read from files
 * and added to appropriate variables,
 * battery active will be set to false;
 */
void setBattery(dwmBattery *battery)
{
    int i, batteries = LENGTH_OF(CurrentBatteryFiles);

    battery->currentWh = 0;
    battery->capacityWh = 1;

    for (i = 0; i < batteries; i++) {
        battery->currentWh += getBatteryValue(CurrentBatteryFiles[i]);
        battery->capacityWh += getBatteryValue(CapacityBatteryFiles[i]);
    }

    battery->active = battery->capacityWh > 0;
    battery->percent = ((double)battery->currentWh / battery->capacityWh) * 100;
}

unsigned long getBatteryValue(const char *fileName)
{
    unsigned long value = 0;
    FILE *fd;

    if ((fd = fopen(fileName, "r")) != NULL) {
        fscanf(fd, "%lu", &value);
        fclose(fd);
    }

    return value;
}

void setBatteryIcon(dwmBattery *battery)
{
    switch ((int)battery->percent) {
    case 90 ... 100:
        battery->icon = BatteryIcon100;
        break;
    case 60 ... 89:
        battery->icon = BatteryIcon75;
        break;
    case 30 ... 59:
        battery->icon = BatteryIcon50;
        break;
    case 10 ... 29:
        battery->icon = BatteryIcon25;
        break;
    default:
        battery->icon = BatteryIcon0;
        break;
    }
}

/*
 * Ignores the first column of /proc/stat's output which only
 * contains the string 'cpu' and reads the next 7 columns
 * which contains all the appropriate cpu load information needed
 * to calculate total cpu percent utilization.
 */
void setCPU(dwmCPU *cpu)
{
    unsigned long loadInfo[CPU_LOAD_INFO_COLUMNS],
                  loadJiffies, idleJiffies,
                  loadDelta, idleDelta;
    FILE *fd;
    int i;

    if ((fd = fopen("/proc/stat","r")) == NULL) {
        return;
    }
    fscanf(fd, "%*s %lu %lu %lu %lu %lu %lu %lu",
           &loadInfo[0], &loadInfo[1], &loadInfo[2], &loadInfo[3],
           &loadInfo[4], &loadInfo[5], &loadInfo[6]);
    fclose(fd);

    loadJiffies = 0;
    idleJiffies = loadInfo[CPU_LOAD_IDLE_COLUMN];
    for (i = 0; i < CPU_LOAD_INFO_COLUMNS; i++) {
        loadJiffies += loadInfo[i];
    }

    loadDelta = cpu->loadJiffies < loadJiffies ? loadJiffies - cpu->loadJiffies
                                               : cpu->loadJiffies - loadJiffies;
    idleDelta = cpu->idleJiffies < idleJiffies ? idleJiffies - cpu->idleJiffies
                                               : cpu->idleJiffies - idleJiffies;

    cpu->loadJiffies = loadJiffies;
    cpu->idleJiffies = idleJiffies;
    cpu->utilization = 100 * (loadDelta - idleDelta) / (float)loadDelta;
}

void setTime(dwmTime *time)
{
   setDate(time);
}

void setDate(dwmDate *date)
{
    time_t currentTime = time(NULL);

    strftime(date->output, MAX_DATE_OUTPUT,
             date->format, localtime(&currentTime));
}

/*
 * Provides consistent spacing between the time icon and time value
 * for both single and double digit hours throughout the day.
 * All double digit hours are prefixed with an empty space.
 */
void setTimeIcon(dwmTime *time)
{
    time->icon = time->output[0] != ' ' ? TimeIconWithSpace : TimeIcon;
}

void setDisk(dwmDisk *disk)
{
    struct statvfs diskInfo;

    if (statvfs("/", &diskInfo) < 0) {
        return;
    }

    disk->totalBytes = diskInfo.f_blocks * diskInfo.f_bsize;
    disk->usedBytes = disk->totalBytes - (diskInfo.f_bfree * diskInfo.f_bsize);
}

void setMemory(dwmMemory *memory)
{
    struct sysinfo sysInfo;

    if (sysinfo(&sysInfo) < 0) {
        return;
    }

    memory->usedBytes = (sysInfo.totalram - sysInfo.freeram);
    memory->totalBytes = sysInfo.totalram;
}

/*
 * For more detail and documentation on what's involved here visit:
 * https://www.alsa-project.org/alsa-doc/alsa-lib/group___simple_mixer.html
 */
void setVolume(dwmVolume *volume)
{
    static const snd_mixer_selem_channel_id_t channel = SND_MIXER_SCHN_MONO;
    static const char *soundCard = "default";
    static const char *channelName = "Master";
    snd_mixer_t *handle;
    snd_mixer_selem_id_t *sid;
    snd_mixer_elem_t *elem;

    snd_mixer_open(&handle, 0);

    snd_mixer_attach(handle, soundCard);
    snd_mixer_selem_register(handle, NULL, NULL);
    snd_mixer_load(handle);

    snd_mixer_selem_id_alloca(&sid);
    snd_mixer_selem_id_set_index(sid, 0);
    snd_mixer_selem_id_set_name(sid, channelName);

    elem = snd_mixer_find_selem(handle, sid);

    snd_mixer_selem_get_playback_volume_range(elem, &volume->min, &volume->max);
    snd_mixer_selem_get_playback_volume(elem, 0, &volume->current);
    snd_mixer_selem_get_playback_switch(elem, channel, &volume->muted);

    snd_mixer_close(handle);

    volume->percent = ((double)volume->current / volume->max) * 100;
}

void setVolumeIcon(dwmVolume *volume)
{
    volume->icon = volume->muted ? SpeakerIconUnmuted : SpeakerIconMuted;
}

/*
 * Searches /proc/net/wireless for specified WirelessInterface's
 * quality link value. If result is null it's likely because there
 * is no active connection. The third column in the wireless interface's
 * output line indicates the end of the quality link value.
 */
void setWifi(dwmWifi *wifi)
{
    char wifiInfo[MAX_BUFFER_SIZE], *interfaceInfo;
    int fd, bytesRead;

    fd = openFile("/proc/net/wireless", 'r');
    bytesRead = read(fd, wifiInfo, MAX_BUFFER_SIZE - 1);
    closeFile(fd);

    if (bytesRead == -1) {
        return;
    }

    wifiInfo[bytesRead] = '\0';
    interfaceInfo = strstr(wifiInfo, WirelessInterface);
    if (interfaceInfo == NULL) {
        wifi->active = FALSE;
        return;
    }

    wifi->active = TRUE;
    wifi->strength = getQualityLinkValue(interfaceInfo) * 100 / 70.0;
}

int getQualityLinkValue(char *interfaceInfo)
{
    char *result;

    result = strtok(interfaceInfo, " .");
    result = strtok(NULL, " .");
    result = strtok(NULL, " .");

    return atoi(result);
}

void setSongIcons(dwmSong *song)
{
    if (song->liked) {
        song->likeIcon = LikedIcon;
    } else if (song->disliked) {
        song->likeIcon = DislikedIcon;
    } else {
        song->likeIcon = "";
    }

    song->playIcon = song->playing ? PlayIcon : PauseIcon;
}

void setSongOutput(dwmSong *song)
{
    unsigned int secondsLeft, maxLength;

    if (!song->active) {
        (song->output)[0] = '\0';
        return;
    }

    snprintf(song->output, MAX_SONG_OUTPUT, "%s%s%s %s - %s",
             song->playIcon, (song->likeIcon)[0] != '\0' ? " " : "",
             song->likeIcon, song->artist, song->title);

    /* -2 to include room for space and null terminator */
    maxLength = MAX_SONG_OUTPUT - MAX_SONG_TIME_OUTPUT - 2;
    if (strlen(song->output) > maxLength) {
        strcpy(song->output + maxLength - 3, "...");
    }

    secondsLeft = song->totalSeconds - song->currentSeconds;
    snprintf(song->output + strlen(song->output), MAX_SONG_TIME_OUTPUT + 2,
             " %d:%02d", secondsLeft / 60, secondsLeft % 60);
}

/*
 * Equivalent to xsetroot -name $(status_output) in bash
 */
void output(dwmStatus *status)
{
    XStoreName(status->display, DefaultRootWindow(status->display),
               status->output);
    XSync(status->display, False);
}
