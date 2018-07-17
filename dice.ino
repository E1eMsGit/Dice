/*********************************************************************
 * Игральный кубик (кости).
 * Платформа: Arduino Nano
 * Подключенные модули: 
 *                      Кнопка броска - пин: 4
 *                      Сдвиговый регистр - пин: 8 (data), 9 (latch), 10 (clk)
 *                      7 светодиодов подключенных к сдвиговому регистру от 1 до 7 выхода.
 *                     
 *********************************************************************/
const uint8_t throwButton = 4;
const uint8_t dataPin = 8;
const uint8_t latchPin = 9;
const uint8_t clkPin = 10;

const uint8_t numbers[6] = {B00001000, B01000001, B01001001, B01010101, B01011101, B01110111};
const uint8_t progressBar[3] = {B01000100, B00101010, B00010001};

uint8_t randNumber = 0;
boolean lastStateThrowButton= LOW; // Переменная для сохранения предыдущего состояния кнопки броска кубика. 
boolean currentStateThrowButton = LOW; // Переменная для сохранения текущего состояния кнопки броска кубика.
                           
void setup() {
    Serial.begin(9600);
    randomSeed(analogRead(0));
    
    pinMode(throwButton, INPUT);       
    pinMode(dataPin, OUTPUT); 
    pinMode(latchPin, OUTPUT); 
    pinMode(clkPin, OUTPUT);  
    digitalWrite(latchPin, HIGH); 

    digitalWrite(latchPin, LOW);  
    shiftOut(dataPin, clkPin, LSBFIRST, B00000000);
    digitalWrite(latchPin, HIGH); 
}

void loop() {  
    currentStateThrowButton = debounce(lastStateThrowButton, throwButton);   
    if (lastStateThrowButton == LOW && currentStateThrowButton == HIGH) {
        getDiceValue();
    }     
    lastStateThrowButton = currentStateThrowButton;
}    

boolean debounce(boolean last, int buttonPin) { 
    boolean current = digitalRead(buttonPin); // Считать состояние кнопки 
   
    // Если изменилось.
    if (last != current) { 
        delay(5);
        current = digitalRead(buttonPin); // Считываем состояние кнопки 
        return current;  // Возвращаем состояние кнопки 
    }
}

void getDiceValue() {
    for (int i = 0, j = 0; i <= 30; i++, j++) {
        randNumber = random(1, 7);
        if (j > 2) j = 0;
        
        digitalWrite(latchPin, LOW);  
        shiftOut(dataPin, clkPin, LSBFIRST, progressBar[j]);
        digitalWrite(latchPin, HIGH); 
        delay(50);
    }
           
    digitalWrite(latchPin, LOW);  
    shiftOut(dataPin, clkPin, LSBFIRST, numbers[randNumber - 1]);
    digitalWrite(latchPin, HIGH);     
}

