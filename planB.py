import RPi.GPIO as GPIO
import time

GPIO.setmode(GPIO.BCM)

LED_PIN = 17
LED_PIN_2 = 27
GPIO.setup(LED_PIN, GPIO.OUT)
GPIO.setup(LED_PIN_2, GPIO.OUT)

try:
    while True:
        GPIO.output(LED_PIN, GPIO.HIGH)  # Включить светодиод
        GPIO.output(LED_PIN_2, GPIO.LOW)  # Включить светодиод
        time.sleep(1)                    # Пауза в 1 секунду
        GPIO.output(LED_PIN, GPIO.LOW)   # Выключить светодиод
        GPIO.output(LED_PIN_2, GPIO.HIGH)   # Выключить светодиод
        time.sleep(1)
except KeyboardInterrupt:
    GPIO.cleanup()  # Очистить состояние GPIO при выходе
