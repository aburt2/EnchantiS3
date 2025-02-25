#include <cmath>
#include <Arduino.h>
#include <iostream>
#include <unistd.h>
#include "esp_timer.h"

#include "icm42670_mmc5633/icm42670_mmc5633_imu.h"
#define IMU_SPI SPI
#define IMU_WIRE Wire1
icm42670_mmc5633_config motion_config(IMU_SPI, IMU_WIRE);
ICM42670_MMC5633_IMU imu;
#define USEC_TO_MSEC 0.001
int i;

#include "imu_orientation.h"

// put function declarations here:
void readIMU();
void benchmarkSensorFusion(int num_loops);

// Sensor callbacks
void readIMU() {
    imu.getData();

    // // Save data to puara gestures
    // sensor_fusion.setAccelerometerValues(imu.accl[0], imu.accl[1],imu.accl[2]);
    // sensor_fusion.setGyroscopeRadianValues(imu.gyro[0], imu.gyro[1],imu.gyro[2]);
    // sensor_fusion.setMagnetometerValues(imu.magn[0],imu.magn[1],imu.magn[2]);
}

void benchmarkSensorFusion(int num_loops) {
  IMU_Orientation sensor_fusion;
  int count = num_loops;
  long start, end, elapsed_time;
  float time_per_loop;
  start = esp_timer_get_time();
  while (count--) {
      sensor_fusion.setAccelerometerValues(0, 1, 0);
      sensor_fusion.setGyroscopeRadianValues(0, 0, 0, 0.001);
      sensor_fusion.setMagnetometerValues(0, 1, 2);
      sensor_fusion.update();
  }
  end = esp_timer_get_time();
  elapsed_time = end-start;
  time_per_loop = float(elapsed_time) / float(num_loops);
  elapsed_time = elapsed_time * USEC_TO_MSEC;


  // Print time
  std::cout << "Iterations: " << num_loops << " total_time: " << elapsed_time << "ms"<< " unit_time: " << time_per_loop << "us"<< std::endl;
}

void setup() {
  // Benchmark the code
  std::cout << "Benchmark ESP32S3 Sensor Fusion Performance " << std::endl;
  delay(500);
}

void loop() {
  std::cout << "Start Test " << i << std::endl;
  i++;
  // put your main code here, to run repeatedly:
  benchmarkSensorFusion(100000);
  delay(5000);
}
