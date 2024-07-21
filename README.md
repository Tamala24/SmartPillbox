# SmartPillbox


## Contents
  - [Description](#description)
  - [Setting](#Setting)
  - [Libraries](#libraries)
  - [App](#app)
  - [Circuit Schematic](#circuit-schematic)
  - [Prototype](#prototype)
  - [Installation and Integration Instructions](#installation-and-integration-instructions)

---

## Description

Smart Pillbox manages to help senior citizens and chronic sufferer with pill dosage schedule by combining the usage of a microcontroller and a mobile app to adjust the doses they need to take associating a color, a tone(optional), and a time for each tablet intake.

This allows to include people who may have visual disabilities (due to the large letters and the music) and hearing disabilities (the color). 
The use of MQTT communication allows to easily send and receive information in a bidirectional way. (The information about the pill is sent from the app to the board and the humidity and temperature from the board to the app. We also send to the app the name of the pill to be taken to remember it)

Once the information has been added, we press the add pill button and the information would be sent via MQTT to the board and would be saved in a list of pills that would be shown in the table below. If we want to delete any pills we have to select them and press the remove pill button, finally if we want to update any data on any pill we would select the data to change and press update pill.

The temperature and humidity is measured to control the state of the pills in case they need specific conditions And in case someone needs to know the conditions of their environment for personal needs.

The servo motor allows to close the box so that it cannot be opened when it is not time to take the pill, thus avoiding taking more pills than necessary. 

When the time comes, the music starts to play, the RGB lights up and the box opens. When the pills have been taken, a button has to be pressed and everything stops and after 10 seconds the servo closes. 

There are different tasks: one to check the current time with the timer in case an alarm needs to be activated, another to activate the alarms, another for the humidity and temperature, and another for the LED so that they can work simultaneously, in addition to protected variables (to avoid race conditions). 

In addition, if we add different pills at the same time, they would be added to a queue and would sound one after another, thus avoiding skipping a pill.

What's more, this prototype is sustainable as the material is recycled cardboard and can be expanded easily without buying more electronic components.

<br>

---

## Setting

The code has been developed in the Arduino integrated development environment, [Arduino IDE](https://www.arduino.cc/en/software), and the app using [MIT App Inventor](https://appinventor.mit.edu). To establish the MQTT communication, the [MQTTX](https://mqttx.app/downloads) program is recommended, although any other program can be used.

<br>

<p align="center">
		<img height=100 width=100 align="center" src="./images/arduino_logo.jpg" />
		<img height=100 width=100 align="center" src="./images/appinventor_logo.png" />
		<img height=100 width=100 align="center" src="./images/mqttx_logo.png" />
</p>

<br>

---

## Libraries

Once the programs are installed, download the following libraries:
* `esp32` by Espressif Systems (Boards Manager): [descarga](https://docs.espressif.com/projects/arduino-esp32/en/latest/installing.html)
* `Adafruit BusIO` by Adafruit (Library Manager): [descarga](https://github.com/adafruit/Adafruit_BusIO)
* `Adafruit Unified Sensor` by Adafruit (Library Manager): [descarga](https://github.com/adafruit/Adafruit_Sensor)
* `DHT sensor library` by Adafruit (Library Manager): [descarga](https://github.com/adafruit/DHT-sensor-library)
* `RTClib` by Adafruit (Library Manager): [descarga](https://github.com/adafruit/RTClib)
* `ESP32Servo` by Kevin Harrington,John K. Bennett (Library Manager): [descarga](https://github.com/madhephaestus/ESP32Servo)
* `PubSubClient` by Nick O'Leary (Library Manager): [descarga](https://github.com/knolleary/pubsubclient/releases/tag/v2.8)
* `pitches.h` by Nick O'Leary (Library Manager): [descarga](https://github.com/arduino/arduino-examples/blob/main/examples/02.Digital/toneMelody/pitches.h)

<br>

---

## App

To use the app, [download](./app/PillBox.apk) in your mobile phone and click on the downloadedfile. To modify the app, download this [code](./app/PillBox.aia) in the desktop version of MIT App Inventor.

<br>

<p align="center">
		<img height=500 width=850 align="center" src="./images/app.jpeg" />
</p>

<br>

<p align="center">
		<img height=500 width=850 align="center" src="./images/app_code.png" />
</p>

<br>

<p align="center">
		<img height=500 width=850 align="center" src="./images/app_designer.png" />
</p>

<br>


---

## Circuit Schematic

Below is an image of the circuit assembly schematic: 

<br>

<p align="center">
		<img height=500 width=850 align="center" src="./images/esq_circ.png" />
</p>

<br>

---

## Prototype

The above assembly is placed next to/inside the box as follows:

<br>

<p align="center">
		<img height=500 width=1000 align="center" src="./images/prototype_image.jpeg" />
</p>

<br>

<p align="center">
		<img height=500 width=1000 align="center" src="./images/prototype_video.mp4" />
</p>

<br>

---

## Installation and Integration Instructions

  ### 1. Instalar y configurar el entorno [Arduino](#entorno) con las [librerías](#librerías) especificadas, además de instalar el programa para la comunicación [MQTT](#entorno).
  
 <br>
 
 Para descargar las librerías también se puede hacer desde el _library manager_ y _board manager_ de Arduino IDE

 <br>

 <p align="center">
		<img height=400 width=700 align="center" src="./images/library_manager.png" />
 </p>

 <br>

 <p align="center">
		<img height=400 width=700 align="center" src="./images/boards_manager.png" />
 </p>

 <br>

 Una vez descargadas las librerías necesarías para compilar el proyecto, seleccionar la placa en el cuadro de selección de placa

 <br>

 <p align="center">
		<img height=400 width=700 align="center" src="./images/selecc_placa_arduino.png" />
 </p>

 <br>
  
  ### 2. Instalar el código fuente descargando la carpeta zip de este repositorio o usando git clone.
  
  <br>
  
  #### Opción 1 
  Click en el boton _Download ZIP_ 

  <p align="center">
		<img height=400 width=700 align="center" src="./images/down_zip.png" />
  </p>

  <br>

  #### Opción 2 
  Clonar el repositorio desde el terminal

  <br>

  ```sh
  git clone https://github.com/Tamala24/ROBOTRONIC.git
  ```

  <br>
  
  ### 3. Modificar valores del código por los de la empresa.

  Para conectar la ESP32-S3 al wifi y al broker MQTT de la emmpresa, modificar las constantes NET_SSID, NET_PASSWD, MQTT_SERVER_IP, MQTT_USERNAME y MQTT_PASSWORD del archivo `Config.h` 
  
		/**
		 * @file  Config.h
		 * @brief Archivo de configuración del sistema
		 */
		#ifndef CONFIG_H
		#define CONFIG_H
	
  		...

      		// WIFI
		#define NET_SSID                  "UPV-PSK"
		#define NET_PASSWD                "giirob-pr2-2023"
		
		// MQTT
		#define MQTT_SERVER_IP            "mqtt.dsic.upv.es"
		#define MQTT_SERVER_PORT          1883
		#define MQTT_USERNAME             "giirob"    // Descomentar esta línea (y la siguiente) para que se conecte al broker MQTT usando usuario y contraseña
		#define MQTT_PASSWORD             "UPV2024"

  		...

    		#endif // CONFIG_H

<br> Si se desea modificar los topics por unos más acordes para su integración en la flota de la empresa modificar los valores de HELLO_TOPIC, TOPIC_PRESENCIA, TOPIC_COBOT y TOPIC_PARADA_EMERGENCIA del archivo `Config.h` 
  
		/**
		 * @file  Config.h
		 * @brief Archivo de configuración del sistema
		 */
		#ifndef CONFIG_H
		#define CONFIG_H
	
  		...

      		// TOPICS
		/**** HELLO_TOPIC ****/
		#define HELLO_TOPIC               "A1"
		/**** TOPIC_PRESENCIA : manda si detecta objeto "detect", si no, "libre" ****/
		#define TOPIC_PRESENCIA           "A1/sensor/presencia/cinta/cajas/final"
		/**** TOPIC_COBOT : recibe "operando" si el cobot está operando y "inactivo" si no ****/
		#define TOPIC_COBOT               "A1/actuador/led/cinta/cajas/final"
		/**** TOPIC_PARADA_EMERGENCIA : manda o recibe "PARAR" si se ha pulsado el botón de emergencia ****/
		#define TOPIC_PARADA_EMERGENCIA   "A1/sensor/boton/emergencia/cinta/cajas"

  		...

    		#endif // CONFIG_H
      		
  <br>
 
  ### 4. Montar el circuito como se indica en la [figura](#esquema-del-circuito).
  
  <br>
  
  ### 5. Conectar la placa al ordenador usando el puerto UART, compilar el proyecto y subir a la ESP32-S3.
  
 <br>

 Después de conectar la placa, seleccionar el puerto que aparezca en el seleccionador de puertos en Tools + Port  

 <br>

 <p align="center">
		<img height=400 width=700 align="center" src="./images/puerto.png" />
 </p>

 <br>

 Compilar primero el código y, si no aparece ningún error, ejecutar en la placa

 <br>

 <p align="center">
		<img height=400 width=700 align="center" src="./images/comp_ejec.png" />
 </p>

 <br>
  
  ### 6. Desconectar la placa y situarla en su posición en [planta](#disposición-en-planta) conectada a una fuente de alimentación externa.
  
 <br>

