String receiveFromMesh()
{
  String response = "NULL";
  uint8_t len = sizeof(buf);
  uint8_t from;
  if (manager->recvfromAck(buf, &len, &from))
  {
    Serial.print("receiveFromMesh() : 0x");
    Serial.print(from, HEX);
    Serial.print(": ");
    Serial.println((char*)buf);
    response = String((char*)buf);
  }
  return response;
}
