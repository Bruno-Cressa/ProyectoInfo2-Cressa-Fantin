#include <SPI.h>
#include <SD.h>

// Declaración de las variables booleanas
bool boton1Estado = false;
bool boton2Estado = false;
bool boton3Estado = false;
bool boton4Estado = false;
bool boton5Estado = false;
bool boton6Estado = false;

// Declaración de los pines
const int boton1Pin = 2;
const int boton2Pin = 3;
const int boton3Pin = 4;
const int boton4Pin = 5;
const int boton5Pin = 6;
const int boton6Pin = 7;
const int sensorTempPin = A0;
const int chipSelect = 10;

// Tiempo para guardar datos (5 minutos)
unsigned long intervalo = 300000; // 5 minutos en milisegundos
unsigned long tiempoAnterior = 0;

void setup() {
    // Inicialización del puerto serial
    Serial.begin(9600);

    // Configuración de los pines de los botones como entradas
    pinMode(boton1Pin, INPUT);
    pinMode(boton2Pin, INPUT);
    pinMode(boton3Pin, INPUT);
    pinMode(boton4Pin, INPUT);
    pinMode(boton5Pin, INPUT);
    pinMode(boton6Pin, INPUT);

    // Configuración del pin del sensor de temperatura como entrada
    pinMode(sensorTempPin, INPUT);

    // Inicialización de la tarjeta SD
    if (!SD.begin(chipSelect)) {
        Serial.println("Error al inicializar la tarjeta SD.");
        return;
    }
    Serial.println("Tarjeta SD inicializada.");

    // Mensaje inicial en el monitor serial
    Serial.println("Sistema iniciado. Esperando interacciones...");
}

void loop() {
    // Comprobar y alternar el estado de cada botón
    comprobarBoton(boton1Pin, boton1Estado, "Botón 1");
    comprobarBoton(boton2Pin, boton2Estado, "Botón 2");
    comprobarBoton(boton3Pin, boton3Estado, "Botón 3");
    comprobarBoton(boton4Pin, boton4Estado, "Botón 4");
    comprobarBoton(boton5Pin, boton5Estado, "Botón 5");
    comprobarBoton(boton6Pin, boton6Estado, "Botón 6");

    // Lectura y muestra de la temperatura
    float temperatura = leerTemperatura();
    Serial.print("Temperatura: ");
    Serial.print(temperatura);
    Serial.println(" °C");

    // Guardar datos en la tarjeta SD cada 5 minutos
    unsigned long tiempoActual = millis();
    if (tiempoActual - tiempoAnterior >= intervalo) {
        guardarDatos(temperatura);
        tiempoAnterior = tiempoActual;
    }

    delay(1000); // Leer la temperatura cada segundo
}

void comprobarBoton(int pin, bool &estado, const char* nombreBoton) {
    if (digitalRead(pin) == HIGH) {
        estado = !estado;
        Serial.print(nombreBoton);
        Serial.println(estado ? " Activado" : " Desactivado");
        delay(200); // Antirrebote
    }
}

float leerTemperatura() {
    // Lectura del pin analógico del sensor de temperatura
    int valorADC = analogRead(sensorTempPin); // Lectura del pin analógico A0
    float voltaje = valorADC * (5.0 / 1023.0); // Conversión a voltaje
    float temperatura = (voltaje - 0.5) * 100; // Conversión a temperatura (ejemplo para sensor TMP36)
    return temperatura;
}

void guardarDatos(float temperatura) {
    // Abrir el archivo en modo de añadir
    File archivo = SD.open("datos.txt", FILE_WRITE);

    // Si el archivo se abrió correctamente, escribir los datos
    if (archivo) {
        archivo.print("Temperatura: ");
        archivo.print(temperatura);
        archivo.print(" °C, Botón 1: ");
        archivo.print(boton1Estado ? "Activado" : "Desactivado");
        archivo.print(", Botón 2: ");
        archivo.print(boton2Estado ? "Activado" : "Desactivado");
        archivo.print(", Botón 3: ");
        archivo.print(boton3Estado ? "Activado" : "Desactivado");
        archivo.print(", Botón 4: ");
        archivo.print(boton4Estado ? "Activado" : "Desactivado");
        archivo.print(", Botón 5: ");
        archivo.print(boton5Estado ? "Activado" : "Desactivado");
        archivo.print(", Botón 6: ");
        archivo.println(boton6Estado ? "Activado" : "Desactivado");
        archivo.close();
        Serial.println("Datos guardados en la tarjeta SD.");
    } else {
        Serial.println("Error al abrir el archivo en la tarjeta SD.");
    }
}
