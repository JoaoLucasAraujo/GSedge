

#include "wokwi-api.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

static float period;

typedef struct {
  pin_t pin_out_pulse;AC
  float amplitude_attr;
  float frequency_attr;
  float reference_attr;
} chip_state_t;

static void chip_timer_event(void *user_data);

void chip_init(void) {
  chip_state_t *chip = malloc(sizeof(chip_state_t));

  chip->pin_out_pulse = pin_init("PULSE", OUTPUT_HIGH);
  chip->frequency_attr = attr_init_float("frequency", 1.0f);

  const timer_config_t timer_config = {
    .callback = chip_timer_event,
    .user_data = chip,
  };
  timer_t timer_id = timer_init(&timer_config);
  timer_start(timer_id, 1, true);
}

void chip_timer_event(void *user_data) {
  chip_state_t *chip = (chip_state_t*)user_data;
  float frequency = attr_read_float(chip->frequency_attr);
  period += frequency / 1000000.0f;
  if (period > 1.0f) period = 0.0f;
  float sineValue = sin(period * 2.0f * M_PI);
  if (sineValue >= 0.0f) {
    pin_write(chip->pin_out_pulse, HIGH);
  } else {
    pin_write(chip->pin_out_pulse, LOW);
  }
}