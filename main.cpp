//******************************************/
// Universidad del Valle de Guatemala
// BE3029 - Electronica Digital 2
// Pablo Mazariegos
// 21/07/2025
// Ejemplo Hola Mundo
// MCU: ESP32 dev kit 1.0
//******************************************/
//******************************************/
// Librerias
//******************************************/
#include <Arduino.h>
#include <stdint.h>
//    Paso 1 incluir librería
#include <LiquidCrystal.h>
//******************************************/
// Definiciones
//******************************************/
//  Paso 2 definir pines de entrada físicos
#define rs 5
#define en 4

#define d4 25
#define d5 26
#define d6 27
#define d7 14

#define btn1 18
#define btn2 19

#define Pot1 2
#define Pot2 21

#define delayBounce 200
//******************************************/
// Prototipos de funciones
//******************************************/
void initLCD(void);
void initbtn(void);

void mostrarContador(uint8_t valor);
void mostrarPot1(void);
void mostrarPot2(void);

void IRAM_ATTR btn1_ISR(void);
//******************************************/
// Variables globales
//******************************************/
uint8_t cont;

volatile bool btn1Pressed;
volatile uint32_t lastISRbtn1 = 0;

volatile bool btn2Pressed;
volatile uint32_t lastISRbtn2 = 0;

//    Paso 3 Definir variables LCD
LiquidCrystal lcd(rs,en,d4,d5,d6,d7);
//******************************************/
// ISRs Rutinas de Interrupcion
//******************************************/
void IRAM_ATTR btn1_ISR(void){
  uint32_t tiempoActual1 = millis();
  if (tiempoActual1 - lastISRbtn1 > delayBounce){
    btn1Pressed = true;
    lastISRbtn1 = tiempoActual1;
  } 
}
void IRAM_ATTR btn2_ISR(void){
  uint32_t tiempoActual2 = millis();
  if (tiempoActual2 - lastISRbtn2 > delayBounce){
    btn2Pressed = true;
    lastISRbtn2 = tiempoActual2;
  } 
}
//******************************************/
// Configuracion
//******************************************/
void setup() {
  initLCD();
  initbtn();
  cont = 0;
  mostrarContador(cont);
}
//******************************************/
// Loop Principal
//******************************************/
void loop() {
  lcd.clear();
  if (btn1Pressed){
    btn1Pressed = false;
    if (cont ==256){
      cont=0;
      lcd.clear();
    }else{
      cont++;
    }
    mostrarContador(cont);
  }
  if (btn2Pressed){
    btn2Pressed = false;
    if (cont ==0){
      cont=255;
      lcd.clear();
    }else{
      cont--;
    }
    mostrarContador(cont);
  }
  mostrarPot1();
  mostrarPot2();
}
//******************************************/
// Otras funciones
//******************************************/
void mostrarPot1(void){
  uint32_t bits = analogRead(Pot1);
  lcd.setCursor(0,0);
  lcd.print("Pot 1");
  lcd.setCursor(0,2);
  lcd.print(bits);
}
void mostrarPot2(void){
  uint32_t bits2 = analogReadMilliVolts(Pot2)*1000;
  lcd.setCursor(6,0);
  lcd.print("Pot 2");
  lcd.setCursor(6,2);
  lcd.print(bits2);
}
void initbtn(void){
  // Configuración entrada
  pinMode(btn1, INPUT_PULLUP);
  pinMode(btn2, INPUT_PULLUP);

  attachInterrupt(btn1, &btn1_ISR, FALLING);
  attachInterrupt(btn2, &btn2_ISR, FALLING);

}
void initLCD (void){
  //    Paso 4 Iniciar LCD
  lcd.begin(16,2);
  //  Limpia la pantalla antes de iniciar
  //lcd.clear();
  //  Define dónde empezar
  lcd.setCursor(0,0);
  lcd.clear();
  //  Imprime la variable
  lcd.print("Todo bien!");
  delay(350);
  lcd.clear();
}
void mostrarContador(uint8_t valor){
  lcd.setCursor(12,0);
  lcd.print("Cont");
  lcd.setCursor(12,2);
  lcd.print(valor);
}