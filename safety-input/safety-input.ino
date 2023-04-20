#include <SoftwareSerial.h>
#include "Seeed_vl53l0x.h"
Seeed_vl53l0x VL53L0X;

SoftwareSerial mySerial(2,3);
char distance[10];

void setup() {
  VL53L0X_Error Status = VL53L0X_ERROR_NONE;
  Serial.begin(9600);
  Status = VL53L0X.VL53L0X_common_init();
  if (VL53L0X_ERROR_NONE != Status) {
      Serial.println("start vl53l0x mesurement failed!");
      VL53L0X.print_pal_error(Status);
      while (1);
  }

  VL53L0X.VL53L0X_long_distance_ranging_init();

  if (VL53L0X_ERROR_NONE != Status) {
      Serial.println("start vl53l0x mesurement failed!");
      VL53L0X.print_pal_error(Status);
      while (1);
  }
  mySerial.begin(9600);
  Serial.println("Test begin");
  VL53L0X.VL53L0X_long_distance_ranging_init();
}

void loop() {
  VL53L0X_RangingMeasurementData_t RangingMeasurementData;
  VL53L0X_Error Status = VL53L0X_ERROR_NONE;

  memset(&RangingMeasurementData, 0, sizeof(VL53L0X_RangingMeasurementData_t));
  Status = VL53L0X.PerformSingleRangingMeasurement(&RangingMeasurementData);
  if (VL53L0X_ERROR_NONE == Status) {
      Serial.print("Measured distance:");
      Serial.print(RangingMeasurementData.RangeMilliMeter);
      Serial.println(" mm");
      sprintf(distance,"%i\r",RangingMeasurementData.RangeMilliMeter);
      mySerial.write(distance);
      mySerial.write("\n");
  } else {
      Serial.print("mesurement failed !! Status code =");
      Serial.println(Status);
  }
  
  delay(500);
}
