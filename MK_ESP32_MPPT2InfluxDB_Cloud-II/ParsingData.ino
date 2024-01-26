/*
  © MK4001 16 Jul 2023
  Write Victron Energy MPPT fields to INfluxDB on Cloud
  based on ESP32 Wroom
*/

// Parsing MPPT serial data and InfluxDB Cloud writing
void parseSerialData(String field, String value) {

  //Serial.print(field);  Serial.println("-"+value); // DEBUG ONLY

  //Main or channel 1 (battery) voltage (mV)
  if (field.equals("V")) {
    float voltage = value.toFloat() / 1000;
    point.addField("Battery_voltage", voltage);
  }

  //Main or channel 1 battery current (mA)
  else if (field.equals("I")) {
    float current = value.toFloat() / 1000;
    point.addField("Battery_current", current);
  }

  //Panel voltage (mV)
  else if (field.equals("VPV")) {
    float vpv = value.toFloat() / 1000;
    point.addField("PV_voltage", vpv);
  }

  //Panel Power (W)
  else if (field.equals("PPV")) {
    float ppv = value.toFloat();
    point.addField("PV_power", ppv);
  }

  //State of Operation
  else if (field.equals("CS")) {
    float state = value.toFloat();
    point.addField("Operation_state", state);
  }

  //Tracker operation mode
  else if (field.equals("MPPT")) {
    float mppt = value.toFloat();
    point.addField("Tracker_mode", mppt);
  }

  //Off reason
  else if (field.equals("OR")) {
    float off = value.toFloat();
    point.addField("Off_reason", off);
  }

  //Error code
  else if (field.equals("ERR")) {
    float err = value.toFloat();
    point.addField("Error_code", err);
  }

  //Load output state (ON/OFF)
  else if (field.equals("LOAD")) {
    if (value == "OFF") {
      point.addField("Load_status", 0);
    } else if (value == "ON") {
      point.addField("Load_status", 1);
    }
  }

  //Load current (mA)
  else if (field.equals("IL")) {
    float il = value.toFloat() / 1000;
    point.addField("Load_current", il);
  }

  //Yield total (0.01 kWh)
  else if (field.equals("H19")) {
    float yeld = value.toFloat() / 1000;
    point.addField("Yeld_total", yeld);
  }

  //Yield today (0.01 kWh)
  else if (field.equals("H20")) {
    float yeld = value.toFloat() / 1000;
    point.addField("Yeld_today", yeld);
  }

  //Maximum power today (W)
  else if (field.equals("H21")) {
    float power = value.toFloat();
    point.addField("Max_pw_today", power);
  }

  //Yield yesterday (0.01 kWh)
  else if (field.equals("H22")) {
    float yeld = value.toFloat() / 1000;
    point.addField("Yeld_yesterday", yeld);
  }

  //Maximum power yesterday (W)
  else if (field.equals("H23")) {
    float power = value.toFloat();
    point.addField("Max_pw_yesterday", power);
  }

  else if (field.equals("HSDS")) {
    // InfluxDB Cloud flush on last field received
    influxDB.writePoint(point);
    Serial.println();
    rec_count++;
    Serial.print("[");
    Serial.print(rec_count);
    Serial.println("]");
  }
}