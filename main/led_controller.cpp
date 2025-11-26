#include "led_controller.h"

LEDController::LEDController(int pin, int numLeds, int brightness)
    : ledPin(pin), numLeds(numLeds), bright(brightness) {
    leds = new CRGB[numLeds];
    currentMode = LED_OFF;
    lastUpdate = 0;
    animationStep = 0;
    animationActive = false;
}

LEDController::~LEDController() {
    delete[] leds;
}

void LEDController::init() {
    FastLED.addLeds<WS2812B, ledPin, GRB>(leds, numLeds);
    FastLED.setBrightness(bright);
    FastLED.clear(true);
    Serial.println("✅ LEDController inicializado (WS2812B)");
}

void LEDController::setMode(int mode) {
    currentMode = mode;
    animationActive = false;
    animationStep = 0;
    
    // Ejecutar modos inmediatos (bloqueantes)
    switch(mode) {
        case LED_OFF:
            modeOff();
            break;
            
        case LED_FULL:
            modeFull();
            break;
            
        case LED_BLINK_FAST:
            // Controlado en update()
            break;
            
        case LED_BYEBYE:
            modeByeBye();
            break;
            
        case LED_DIMMING:
            // Controlado en update()
            break;
            
        // Animaciones (bloqueantes - se ejecutan una vez)
        case LED_ANIM_CARTON:
            animCarton();
            break;
            
        case LED_ANIM_PLASTICO:
            animPlastico();
            break;
            
        case LED_ANIM_PAPEL:
            animPapel();
            break;
            
        case LED_ANIM_GENERAL:
            animGeneral();
            break;
            
        default:
            modeOff();
            break;
    }
}

void LEDController::update() {
    // Solo modos no bloqueantes se actualizan aquí
    switch(currentMode) {
        case LED_DIMMING:
            modeDimming();
            break;
            
        case LED_BLINK_FAST:
            modeBlinkFast();
            break;
            
        default:
            // Otros modos no requieren actualización continua
            break;
    }
}

// ========== MÉTODOS DE CONVENIENCIA ==========

void LEDController::setColor(CRGB color) {
    fill_solid(leds, numLeds, color);
    FastLED.show();
}

void LEDController::clear() {
    fill_solid(leds, numLeds, CRGB::Black);
    FastLED.show();
}

// ========== FUNCIONES AUXILIARES ==========

void LEDController::fillRange(int start, int end, CRGB color) {
    for (int i = start; i <= end && i < numLeds; i++) {
        leds[i] = color;
    }
}

void LEDController::clearRange(int start, int end) {
    fillRange(start, end, CRGB::Black);
}

CRGB LEDController::scaleColor(CRGB color, float factor) {
    return CRGB(
        (uint8_t)(color.r * factor),
        (uint8_t)(color.g * factor),
        (uint8_t)(color.b * factor)
    );
}

// ========== MODOS BÁSICOS ==========

void LEDController::modeOff() {
    clear();
}

void LEDController::modeFull() {
    fill_solid(leds, numLeds, CRGB::White);
    FastLED.show();
}

void LEDController::modeBlinkFast() {
    static unsigned long lastToggle = 0;
    static bool isOn = false;
    
    if (millis() - lastToggle > 200) {  // 200ms = parpadeo rápido
        lastToggle = millis();
        isOn = !isOn;
        
        if (isOn) {
            fill_solid(leds, numLeds, CRGB::White);
        } else {
            fill_solid(leds, numLeds, CRGB::Black);
        }
        FastLED.show();
    }
}

void LEDController::modeByeBye() {
    // Patrón de despedida: 3 destellos
    for (int i = 0; i < 3; i++) {
        fill_solid(leds, numLeds, CRGB::Green);
        FastLED.show();
        delay(150);
        
        fill_solid(leds, numLeds, CRGB::Black);
        FastLED.show();
        delay(150);
    }
}

void LEDController::modeDimming() {
    // Fade in/out suave (respiración)
    static unsigned long lastFade = 0;
    static int brightness = 0;
    static int fadeDirection = 1;  // 1 = subiendo, -1 = bajando
    
    if (millis() - lastFade > 20) {  // Actualizar cada 20ms
        lastFade = millis();
        
        brightness += fadeDirection * 5;
        
        if (brightness >= 255) {
            brightness = 255;
            fadeDirection = -1;
        } else if (brightness <= 0) {
            brightness = 0;
            fadeDirection = 1;
        }
        
        CRGB color = scaleColor(CRGB::White, brightness / 255.0);
        fill_solid(leds, numLeds, color);
        FastLED.show();
    }
}

// ========== ANIMACIONES (basadas en 1SecuenciaTirasLED.ino) ==========

void LEDController::animCarton() {
    // Relleno progresivo de G1 → G3 (Marrón para cartón)
    clear();
    
    // Grupo 1
    for (int i = GRUPO1_START; i <= GRUPO1_END; i++) {
        leds[i] = COLOR_CARTON;
        FastLED.show();
        delay(25);
    }
    delay(200);
    
    // Grupo 3
    for (int i = GRUPO3_START; i <= GRUPO3_END; i++) {
        leds[i] = COLOR_CARTON;
        FastLED.show();
        delay(25);
    }
    delay(800);
    
    clearRange(GRUPO1_START, GRUPO1_END);
    clearRange(GRUPO3_START, GRUPO3_END);
    FastLED.show();
}

void LEDController::animPlastico() {
    // Barrido circular simulado G1 → G3 (Blanco)
    clear();
    
    for (int j = GRUPO1_START; j <= GRUPO1_END; j++) {
        fill_solid(leds, numLeds, CRGB::Black);
        leds[j] = COLOR_PLASTICO;
        FastLED.show();
        delay(25);
    }
    
    for (int j = GRUPO3_START; j <= GRUPO3_END; j++) {
        fill_solid(leds, numLeds, CRGB::Black);
        leds[j] = COLOR_PLASTICO;
        FastLED.show();
        delay(25);
    }
    
    clear();
}

void LEDController::animPapel() {
    // Fade in/out suave todos los grupos (Azul Medio)
    
    // Fade in
    for (int b = 0; b <= 255; b += 5) {
        float factor = b / 255.0;
        fillRange(GRUPO1_START, GRUPO1_END, scaleColor(COLOR_PAPEL, factor));
        fillRange(GRUPO2_START, GRUPO2_END, scaleColor(COLOR_PAPEL, factor));
        fillRange(GRUPO3_START, GRUPO3_END, scaleColor(COLOR_PAPEL, factor));
        FastLED.show();
        delay(10);
    }
    
    // Fade out
    for (int b = 255; b >= 0; b -= 5) {
        float factor = b / 255.0;
        fillRange(GRUPO1_START, GRUPO1_END, scaleColor(COLOR_PAPEL, factor));
        fillRange(GRUPO2_START, GRUPO2_END, scaleColor(COLOR_PAPEL, factor));
        fillRange(GRUPO3_START, GRUPO3_END, scaleColor(COLOR_PAPEL, factor));
        FastLED.show();
        delay(10);
    }
    
    clear();
}

void LEDController::animGeneral() {
    // Destellos metálicos G1 + G2 + G3 (Gris Neutro)
    for (int i = 0; i < 5; i++) {
        fillRange(GRUPO1_START, GRUPO1_END, COLOR_GENERAL);
        fillRange(GRUPO2_START, GRUPO2_END, COLOR_GENERAL);
        fillRange(GRUPO3_START, GRUPO3_END, COLOR_GENERAL);
        FastLED.show();
        delay(100);
        
        clear();
        delay(50);
    }
}
