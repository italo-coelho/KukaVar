# KukaVar

A library to read and change the value of global variables in KUKA Robots. The robot controller must be running KUKA VarProxy server (https://github.com/ImtsSrl/KUKAVARPROXY). This library is intended for ESP32 and ESP8266 devices and can be used with WiFi or Ethernet Clients.

### Supports

* Read: get variable from controller as String
* Write: change variable in the controller via String

* JSON-like Access: KukaVar[var_name] or KukaVar[array_name][index] for string, bool, int, float and double variables