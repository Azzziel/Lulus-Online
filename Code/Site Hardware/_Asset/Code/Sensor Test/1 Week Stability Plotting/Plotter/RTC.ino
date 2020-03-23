void RTCcheck() {
  DateTime now = rtc.now();
  timeNow = "";
  timeNow += now.day();
  timeNow += "|";
  timeNow += now.hour();
  timeNow += "|";
  timeNow += now.minute();

}
