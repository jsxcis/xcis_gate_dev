void initaliseScanList()
{

  for (int i = 0; i < configuration.scanListLength; i++)
  {
    #ifdef debug
    Serial.println("Adding sensor:");
    Serial.print(configuration.scanList[i].loraID);
    Serial.print(",");
    Serial.print(configuration.scanList[i].deviceType);
    Serial.print(":");
    #endif
    sensors.addSensor(i, configuration.scanList[i].loraID, configuration.scanList[i].deviceType);
  }
}
