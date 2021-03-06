// Code to run and read data from science module instruments, including carousel, spectrometer, thermocouple, and moisture sensor

#include "Robocluster.h"
#include "carousel.h"
#include "thermocouple.h"
#include "moisture_sensor.h"
#include "spectrometer.h"

// Define Arduino Pins
#define IR_SENSOR  A1
#define MOIST_SENSOR  A2
#define CAROUSEL_DIRECTION  6
#define CAROUSEL_MOTOR 4
#define CAROUSEL_STEP  7
#define DETECTOR_FEED  8
#define EMITTER  5
#define MOTOR_POWER 4

void move_carousel(char *data)  {
    int steps = atoi(data);
    int direction = 1;
    if (steps > 0) {
        direction = 1;
    } else {
        direction = 0;
        steps *= -1;
    }
    motor_on(MOTOR_POWER);
    delay(10);
    for(int i=0; i<steps; i++) {
        step_motor(direction, CAROUSEL_DIRECTION, CAROUSEL_STEP);
    }
    delay(10);
    motor_off(MOTOR_POWER);
}

void run_thermocouple(char *data)   {
    read_temperature();
}

void run_moisture_probe(char *data)  {
    read_moisture(MOIST_SENSOR);
}

void run_emitter(char *data)  {
    int d = atoi(data);
    if(d == 1) {
        emitter_on(EMITTER);
        digitalWrite(13, HIGH);
    }
    else if(d == 0)  {
        emitter_off(EMITTER);
    }
}

void take_reading(char *data) {
    analyse_sample();
}

void toggle_carousel(char *data) {
  int d = atoi(data);
  if (d == 1) {
    digitalWrite(CAROUSEL_MOTOR, LOW);
  } else if (d == 0) {
    digitalWrite(CAROUSEL_MOTOR, HIGH);
  }
}


void setup()    {
    Serial.begin(115200);
    a_max.begin();

    // Set pin I/O
    pinMode(EMITTER, OUTPUT);
    pinMode(CAROUSEL_STEP, OUTPUT);
    pinMode(CAROUSEL_DIRECTION, OUTPUT);
    pinMode(13, OUTPUT);

    // Initialize send pins and ensure that nothing happens during initialization
    digitalWrite(CAROUSEL_STEP, LOW);
    digitalWrite(CAROUSEL_DIRECTION, LOW);
    digitalWrite(EMITTER,HIGH);

    // Set up message handling
    set_name("ScienceArduino");
    int num_msgs = 5;
    set_messages(num_msgs, "move_carousel", "run_thermocouple", "run_moisture_probe", "run_emitter", "take_reading");
    set_callbacks(num_msgs, move_carousel, run_thermocouple,
    run_moisture_probe, run_emitter, take_reading);

    char buffer[BUFF_SIZE];
    sprintf(buffer, "{\"science_ready\":%i}", 1);
    Publish(buffer);
}

void loop() { 
    delay(1000);
    digitalWrite(13, LOW);
}






