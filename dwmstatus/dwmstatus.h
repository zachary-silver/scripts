#ifndef DWMSTATUS_H
#define DWMSTATUS_H

/* Macros */
#define TRUE 1
#define FALSE 0
#define MAX_JSON_FILE_SIZE 8192

/* Typedefs */
typedef struct {
    char output[MAX_JSON_FILE_SIZE];
    float currentSeconds;
    float totalSeconds;
    int active;
    int playing;
    int liked;
    int disliked;
    char *title;
    char *artist;
    const char *likeIcon;
    const char *playIcon;
} dwmSong;

/* Function definitions */
void setSong(dwmSong *song);

#endif
