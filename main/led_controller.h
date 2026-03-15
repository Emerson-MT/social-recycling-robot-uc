#ifndef LED_CONTROLLER_H
#define LED_CONTROLLER_H

#include <FastLED.h>
#include <Arduino.h>

// ==== DEFINICIÓN DE GRUPOS ====
#define GRUPO1_START  0
#define GRUPO1_END    29
#define GRUPO2_START  30
#define GRUPO2_END    49
#define GRUPO3_START  50
#define GRUPO3_END    79

// ==== COLORES DE RESIDUO (RGB) ====
#define COLOR_CARTON    CRGB(139, 69, 19)    // Marrón (Cartón)
#define COLOR_PAPEL     CRGB(0, 70, 250)     // Azul Medio (Papel)
#define COLOR_PLASTICO  CRGB(255, 200, 250)  // Blanco (Plástico)
#define COLOR_GENERAL   CRGB(70, 70, 70)     // Gris Neutro (General)

// Colores adicionales disponibles (no usados actualmente)
#define COLOR_PILAS     CRGB(255, 25, 20)    // Rojo Oscuro
#define COLOR_VIDRIO    CRGB(38, 250, 45)    // Verde Oscuro

// ==== MODOS LED ====
enum LedMode {
    LED_OFF = 0,              // Apagado
    LED_DIMMING = 1,          // Fade in/out suave (despertar)
    LED_FULL = 2,             // Encendido completo (blanco)
    LED_BLINK_FAST = 3,       // Parpadeo rápido
    LED_BYEBYE = 4,           // Patrón de despedida
    
    // MODOS DE ANIMACIÓN POR TIPO DE RESIDUO
    LED_ANIM_PLASTICO = 6,    // Animación para plástico (barrido)
    LED_ANIM_PAPEL = 7,       // Animación para papel (fade suave)
    LED_ANIM_GENERAL = 8,     // Animación para residuo general (destellos)
    LED_ANIM_CARTON = 10      // Animación para cartón (relleno progresivo marrón)
};

class LEDController {
public:
    LEDController(int pin, int numLeds, int brightness);
    ~LEDController();
    
    void init();
    void setMode(int mode);
    void update();  // Llamar en loop() para animaciones no bloqueantes
    
    // Métodos de conveniencia
    void setColor(CRGB color);
    void clear();

private:
    int ledPin;
    int numLeds;
    int bright;
    CRGB* leds;
    
    // Control de modo actual
    int currentMode;
    unsigned long lastUpdate;
    int animationStep;
    bool animationActive;
    
    // Funciones auxiliares
    void fillRange(int start, int end, CRGB color);
    void clearRange(int start, int end);
    CRGB scaleColor(CRGB color, float factor);
    
    // Modos básicos (bloqueantes)
    void modeOff();
    void modeFull();
    void modeBlinkFast();
    void modeByeBye();
    
    // Modo dimming (no bloqueante)
    void modeDimming();
    
    // Animaciones (bloqueantes - se ejecutan una vez)
    void animCarton();
    void animPlastico();
    void animPapel();
    void animGeneral();
};

#endif
