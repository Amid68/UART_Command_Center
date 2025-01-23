#include <zephyr/drivers/adc.h>
#include "sensor_readings.h"

static const struct device *adc_dev = DEVICE_DT_GET(DT_NODELABEL(adc1));
static struct adc_channel_cfg channel_cfg = {
    .gain = ADC_GAIN_1_6,
    .reference = ADC_REF_INTERNAL,
    .channel_id = 0, 
};

int sensor_readings_get_temperature(void) {
    int16_t buf;
    struct adc_sequence seq = {
        .channels = BIT(0),
        .buffer = &buf,
        .buffer_size = sizeof(buf),
        .resolution = 12,
    };

    if (!device_is_ready(adc_dev)) {
        return -ENODEV;
    }

    adc_channel_setup(adc_dev, &channel_cfg);
    adc_read(adc_dev, &seq);
    return (buf * 3300) / 4096; // Convert to millivolts, then to temperature
}