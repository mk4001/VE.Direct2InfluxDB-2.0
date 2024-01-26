# VE.Direct2InfluxDB-2.0
Victron Energy VE.Direct UART data output capture via MQTT based on ESP32 Victron Energy VE.Direct UART data output to influxDB Cloud using Espressif ESP32 (based on the Arduino platform)

The idea of untying the Victron Energy Smart Solar MPPT 75/15 from the smartphone's BLE APP and being able to send it to an MQTT broker led me to create this project.

The VE.Direct interface, on most Victron Energy products, is in fact a UART that transmits at a record speed of one per second at 19200 baud.

It is necessary to create a simple adapter cable with the help of a JST 2.0 PH 4 connector easily available on both Amazon and Aliexpress.

For the pins of the UART port in question, I only used PINs 1 (GND) and 3 (TX), then connected to Serial2 of a "DEVIT V1 ESP32-WROOM-32 Development Board" (GND and GPIO16 ports).

<img width="1369" alt="Screenshot 2023-07-20 at 09 47 55" src="https://github.com/mk4001/VE.Direct2InfluxDB/assets/50479511/780e0403-754e-42f9-90fa-479fb00701fc">

Data flows from the UART port at a rate of 1 record/sec.

Detailed documentation of the fields, their format and content is collected here:

https://www.victronenergy.com/upload/documents/VE.Direct-Protocol-3.33.pdf

Once the record has been captured on ESP32, we need to parse the fields and collect the data we need before sending it to the MQTT broker

In this new version 2.0 I used the official Victron Energy library "VeDirectFrameHandler" which is able to manage the "Chesum" field very well at the end of each record coming from a Victron device.

The "ParsingData.ino" function takes on, in this new version, a less relevant role and is no longer invoked for each entire record but rather for each field every time an entire and correct record is validated at Checksum level.

When your broker finally receives the data you can decide to collect it (perhaps using InfluxDB TELEGRAF):

https://www.influxdata.com/time-series-platform/telegraf/

and create a wonderful Dashboard on Grafana (always in the cloud, always free)

https://grafana.com/auth/sign-up/create-user?pg=hp&plcmt=hero-btn1&cta=create-free-account

With a little imagination and above all practice, you can also create dashboards like this:

![image1](https://github.com/mk4001/VE.Direct2MQTT-2.0/blob/5b6c22a4f9a01b6e5497d393e531245881a66e8c/Screenshot%202024-01-26%20at%2010.20.00.png)
(Grafana Dashboard file: Victron Energy-1700659129739.json)

the obviously "interesting" thing is that you can consult the data of your Victron Energy devices quickly and easily from anywhere and customize them as you prefer.
