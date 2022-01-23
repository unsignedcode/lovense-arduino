# Lovense-Arduino  
Using ArduinoBLE to control Lovense devices.  

Learn more about the Lovense protocol from STPIHKAL [here](https://stpihkal.docs.buttplug.io/protocols/lovense.html#protocol).  
STPIHKAL includes different commands supported by other devices and can be used to easily extend the examples here.

### Examples
**serialControl**

* Reads a vibration strength from serial and sends it to a Lovense Max 2

**analogControl**

* Reads a vibration strength from a potentiometer and sends it to a Lovense device, creating a vibration remote and allowing you to use Lovense devices without a phone. 

### What about my device?

You will need to provide your own local name and characteristic uuids for other devices, these can be found using the nRF Connect for Mobile App.

