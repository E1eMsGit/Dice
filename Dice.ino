/*********************************************************************
 * Игральный кубик (кости).
 * Платформа: Arduino Nano
 * Подключенные модули: 
 *                      Кнопка просмотра первого значения - пин: 2
 *                      Кнопка просмотра второго значения - пин: 3
 *                      Кнопка броска - пин: 4
 *                      Сдвиговый регистр - пин: 8 (data), 9 (latch), 10 (clk)
 *                      7 светодиодов подключенных к сдвиговому регистру от 1 до 7 выхода.
 * Описание: Нажатие на кнопку на 4м пине вызывает бросок кубика.
 *                   После первого броска значение сохраняется в firstValue и сбрасывает значение в secondValue если оно там есть, 
 *                   после второго броска сохраняет значение  в secondValue. Это нужно для имитации двух кубиков.
 * 
 *                     
 *********************************************************************/
const uint8_t firstValueButton = 2;
const uint8_t secondValueButton = 3;
const uint8_t throwButton = 4;

const uint8_t dataPin = 8;
const uint8_t latchPin = 9;
const uint8_t clkPin = 10;

const uint8_t numbers[7] = {B00000000, B00001000, B01000001, B01001001, B01010101, B01011101, B01110111};
const uint8_t progressBar[3] = {B01000100, B00101010, B00010001};

uint8_t randNumber = 0;
uint8_t diceCounter = 1; // Кол-во брошенных кубиков. По умолчанию 1, чтобы после первого броска сразу записалось в firstValue.
uint8_t currentValueIndex = 0; // Индекс значения которое выпало после броска.
uint8_t firstValueIndex = 0;
uint8_t secondValueIndex = 0;

boolean lastStateThrowButton= LOW; // Переменная для сохранения предыдущего состояния кнопки броска кубика. 
boolean currentStateThrowButton = LOW; // Переменная для сохранения текущего состояния кнопки броска кубика.
boolean lastStateFirstValueButton= LOW; // Переменная для сохранения предыдущего состояния кнопки первого значения. 
boolean currentStateFirstValueButton = LOW; // Переменная для сохранения текущего состояния кнопки первого значения.
boolean lastStateSecondValueButton= LOW; // Переменная для сохранения предыдущего состояния кнопки второго значения. 
boolean currentStateSecondValueButton = LOW; // Переменная для сохранения текущего состояния кнопки второго значения. 
                           
void setup() {
    Serial.begin(9600);
    randomSeed(analogRead(0));
    
    pinMode(throwButton, INPUT);
    pinMode(firstValueButton, INPUT); 
    pinMode(secondValueButton, INPUT);  
          
    pinMode(dataPin, OUTPUT); 
    pinMode(latchPin, OUTPUT); 
    pinMode(clkPin, OUTPUT);  
    digitalWrite(latchPin, HIGH); 

    digitalWrite(latchPin, LOW);  
    shiftOut(dataPin, clkPin, LSBFIRST, B00000000);
    digitalWrite(latchPin, HIGH); 
}

void loop() {
    // Бросок кубика.
    currentStateThrowButton = debounce(lastStateThrowButton, throwButton);   
    if (lastStateThrowButton == LOW && currentStateThrowButton == HIGH) {
        getDiceValue();
        if (diceCounter == 1) {
            firstValueIndex = currentValueIndex;
            secondValueIndex = 0;
        }
        else if (diceCounter == 2) {
            secondValueIndex = currentValueIndex;
            diceCounter = 0;
        }
        diceCounter++;
    }     
    lastStateThrowButton = currentStateThrowButton;

    // Просмотр первого значения.
    currentStateFirstValueButton = debounce(lastStateFirstValueButton, firstValueButton);   
    if (lastStateFirstValueButton == LOW && currentStateFirstValueButton == HIGH) {
        printValue(numbers[firstValueIndex]);
    }     
    lastStateFirstValueButton = currentStateFirstValueButton;

    // Просмотр второго значения.
    currentStateSecondValueButton = debounce(lastStateSecondValueButton, secondValueButton);   
    if (lastStateSecondValueButton == LOW && currentStateSecondValueButton == HIGH) {
        printValue(numbers[secondValueIndex]);
    }     
    lastStateSecondValueButton = currentStateSecondValueButton;
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
        randNumber = random(2, 8);
        if (j > 2) j = 0;       
        printValue(progressBar[j]);
        delay(50);
    }

    currentValueIndex = randNumber - 1;
    printValue(numbers[currentValueIndex]);
}

void printValue(uint8_t value) {
    digitalWrite(latchPin, LOW); 
    shiftOut(dataPin, clkPin, LSBFIRST, value);
    digitalWrite(latchPin, HIGH);   
}

