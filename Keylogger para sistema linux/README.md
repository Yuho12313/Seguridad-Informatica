Este proyecto es un keylogger básico desarrollado en lenguaje C que captura las teclas presionadas en un sistema Linux 
y las guarda en un archivo de texto

IMPORTANTE 
Este código debe utilizarse únicamente con fines éticos y legales
En la linea 10 cambiar el Nombre_de_usuario por el tuyo

Requisitos
-Sistema operativo: Linux
-Acceso root o permisos de superusuario
-Compilador GCC

Compilación y Ejecución
Paso 1: Compilar el Código

gcc keylogger.c -o keylogger

Paso 2: Ejecutar el Keylogger

sudo ./keylogger

Funcionamiento del Código
Captura de Teclas: El programa monitorea el dispositivo de entrada del teclado
 y captura los códigos de las teclas presionadas

Conversión de Códigos a Caracteres: Los códigos de las teclas se convierten en caracteres 
legibles mediante un mapeo básico

Registro en Archivo: Los caracteres convertidos se guardan en el archivo de log keylogger.txt

Supuestos del Sistema
El sistema Linux en el que se ejecuta el keylogger tiene un directorio
 /dev/input/ con dispositivos de entrada accesibles

El usuario tiene los permisos necesarios para acceder a estos dispositivos

El archivo de log se guarda en /home/Nombre_de_usuario/, y el usuario tiene permisos de escritura en este directorio

Casos de Uso

Entorno Educativo: El keylogger puede ser utilizado en un laboratorio de seguridad 
informática para entender cómo funcionan los keyloggers y cómo prevenirlos

Investigación de Seguridad: Puede ser empleado para probar sistemas de 
detección de malware y desarrollar contramedidas