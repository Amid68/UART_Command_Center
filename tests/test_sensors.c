ZTEST(sensors, test_temperature_read) {
    int temp = sensor_readings_get_temperature();
    zassert_true(temp >= -40 && temp <= 85, "Temperature out of expected range");
}