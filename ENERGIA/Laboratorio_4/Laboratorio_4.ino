/*

  José Santizo Olivet, Carné 20185
  Electrónica digital - Sección 10

 */
#include <SPI.h>
#include <SD.h>

File Tarjeta;
File myFile1;
File myFile2;
File myFile3;

//------------------Declaración de variables-------------------------
byte Menu = 0;
int Bandera_fin = 0;
int Bandera_inicio = 0;

void setup()
{
  // Inicializar el puerto serial y SPI 0
  Serial.begin(115200);
  SPI.setModule(0);

  // Se indica que se comienza la inicialización de la tarjeta SD
  Serial.print("Inicializando la tarjeta SD...");
  
  /*
   * En este caso, el pin PA_3 se deja como output, pues aunque no sea necesario el CS de SPI,
   * el pin PA_3 debe de dejarse listo para funcionar de esta forma por la propia librería de la
   * SD
   */
  pinMode(PA_3, OUTPUT);                                                              // Pin PB_5 como salida de CS

  if (!SD.begin(PA_3)) {
    Serial.println("Inicialización fallida :(");                                      // Si no se puede iniciar la SD, indicar que esta misma falló
    return;
  }
  Serial.println("Inicialización finalizada");                                        // Si no hay errores, indicar que se finalizó la inicialización

  Tarjeta = SD.open("/");                                                             // Abrir la SD para revisión de archivos

  printDirectory(Tarjeta, 0);                                                         // Listar los archivos en la tarjeta SD                                   

  Serial.println("Listo!");
  Serial.println(" ");                                                                // Realizar 3 espacios para iniciar el menú
  Serial.println(" ");
  Serial.println(" ");
      
  Serial.println("¿De qué archivo desea observar su contenido? ¿1, 2 o 3?");          // Preguntar al usuario que archivo le gustaría observar
  Serial.print("Indique solamente el número");
}

void loop()
{
  
  if(Serial.available() > 0){
    //-----------------Chequeo de valor ingresado a Menú---------------------------
    Menu = Serial.read();

    //-----------------------Menú principal----------------------------------------
    if(Bandera_inicio == 0){
      Serial.println(" ");                                                            // Realizar 3 espacios para iniciar el menú
      Serial.println(" ");
      Serial.println(" ");
      
      Serial.println("¿De qué archivo desea observar su contenido? ¿1, 2 o 3?");      // Preguntar al usuario que archivo le gustaría observar
      Serial.println("Indique solamente el número");                                    
      Bandera_inicio = 1;                                                             // Colocar bandera de inicio en 1 para solo mostrar una vez el menú
    }

  
    //-----------------Acción a realizar si se ingresa un 1------------------------
    if(Menu == '1' && Bandera_inicio == 1){                                           // Si número es "1"
      readSD1();                                                                      // Leer archivo 1 de la SD

      // Desplegar menú para dar opción al usuario de escoger otro archivo
      Serial.println(" ");
      Serial.println(" ");
      Serial.println("¿Desea elegir otro archivo o desea finalizar? Si desea finalizar, presione la tecla F.");
      Serial.println("Si desea elegir otro archivo, presione la tecla O.");    
      delay(250);
    }
  
    //-----------------Acción a realizar si se ingresa un 2------------------------
    if(Menu == '2' && Bandera_inicio == 1){                                           // Si número es "2"
      readSD2();                                                                      // Leer archivo 2 de la SD

      // Desplegar menú para dar opción al usuario de escoger otro archivo
      Serial.println(" ");
      Serial.println(" ");
      Serial.println("¿Desea elegir otro archivo o desea finalizar? Si desea finalizar, presione la tecla F.");
      Serial.println("Si desea elegir otro archivo, presione la tecla O.");   
      delay(250);
    }
  
    //-----------------Acción a realizar si se ingresa un 3------------------------
    if(Menu == '3' && Bandera_inicio == 1){                                           // Si número es "3"
      readSD3();                                                                      // Leer archivo 3 de la SD

      // Desplegar menú para dar opción al usuario de escoger otro archivo
      Serial.println(" ");
      Serial.println(" ");
      Serial.println("¿Desea elegir otro archivo o desea finalizar? Si desea finalizar, presione la tecla F.");
      Serial.println("Si desea elegir otro archivo, presione la tecla O.");   
      delay(250);
    }
  
    //-----------Acción a realizar si se presiona la tecla O (Otro)----------------
    if((Menu == 'O' || Menu == 'o') && Bandera_inicio == 1){                          // Si se presiona la tecla O u o minúscula, entonces:
      Bandera_inicio = 0;                                                             // Colocar bandera en 0 para volver a imprimir el menú de inicio
    }
  
    //-----------Acción a realizar si se presiona la tecla F (Finalizar)-----------
    if((Menu == 'F' || Menu == 'f') && Bandera_inicio == 1){                          // Si se presiona la tecla F o f minúscula, entonces:
      Serial.println("Adios amigo :)");                                               // Imprimir mensaje "Adios amigo :)"
      Serial.print("Que la fuerza te acompañe");
      Bandera_inicio = 5;                                                             // Colocar bandera de inicio en 5 para ya no poder realizar otra acción dentro del menú principal
    }
  }
}

void printDirectory(File dir, int numTabs) {
   while(true) {
     
     File entry =  dir.openNextFile();                                              // Abrir archivo entry
     if (! entry) {                                                                 // Si entry no contiene archivos, salir de la subrutina
       // no more files
       break;
     }
     for (uint8_t i=0; i<numTabs; i++) {                                            
       Serial.print('\t');
     }
     Serial.print(entry.name());                                                    // Imprimir el nombre de los archivos en SD
     if (entry.isDirectory()) {
       Serial.println("/");
       printDirectory(entry, numTabs+1);                                            // Si entry es directorio, entonces imprimir los nombres y su tamaño
     } else {
       // files have sizes, directories do not
       Serial.print("\t\t");
       Serial.println(entry.size(), DEC);                                           // Si no es directorio, entonces imprimir tabulaciones y el tamaño del directorio
     }
     entry.close();                                                 
   }
}

void readSD1(void){
  myFile1 = SD.open("Imagen~1.txt");                                                // Abrir archivo "Imagen 1.txt" en SD 
  if (myFile1) {                                                                    // Si existen el archivo "Imagen 1.txt", entonces:
    Serial.println("Imagen 1.txt:");                                                  

    while (myFile1.available()) {                                                   // Leer todo el contenido del archivo hasta que el mismo se acabe
      Serial.write(myFile1.read());
    }

    myFile1.close();                                                                // Cerrar el archivo "Imagen 1.txt"
   }
  else{
    Serial.println("error abriendo Imagen 1.txt");                                  // De lo contrario, escribir que hubo un error
  } 
}

void readSD2(void){
  myFile2 = SD.open("Imagen~2.txt");                                                // Abrir archivo "Imagen 2.txt" en SD 
  if (myFile2) {                                                                    // Si existen el archivo "Imagen 2.txt", entonces:
    Serial.println("Imagen 2.txt:");                                                  

    while (myFile2.available()) {                                                   // Leer todo el contenido del archivo hasta que el mismo se acabe
      Serial.write(myFile2.read());
    }

    myFile2.close();                                                                // Cerrar el archivo "Imagen 2.txt"
   }
  else{
    Serial.println("error abriendo Imagen 2.txt");                                  // De lo contrario, escribir que hubo un error
  }
}

void readSD3(void){
  myFile3 = SD.open("Imagen~3.txt");                                                // Abrir archivo "Imagen 3.txt" en SD 
  if (myFile3) {                                                                    // Si existen el archivo "Imagen 3.txt", entonces:
    Serial.println("Imagen 3.txt:");                                                  

    while (myFile3.available()) {                                                   // Leer todo el contenido del archivo hasta que el mismo se acabe
      Serial.write(myFile3.read());
    }

    myFile3.close();                                                                // Cerrar el archivo "Imagen 3.txt"
   }
  else{
    Serial.println("error abriendo Imagen 3.txt");                                  // De lo contrario, escribir que hubo un error
  }
}
