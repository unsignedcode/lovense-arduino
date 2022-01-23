/*
  Lovense Max 2 Arduino BLE Example.
  Reads a vibration level (0-20) via a potentiometer on A7 then sclaes it and sends via BLE.
*/

#include <ArduinoBLE.h>

/*
 * The local name and vibe characteristic can be found using the NRF Connect for Mobile App
 * See https://stpihkal.docs.buttplug.io/protocols/lovense.html#bluetooth-details for more details.
 */
String lovenseLocalName = "LVS-Max213";
const char* vibeCharacteristicUuid = "42300002-0023-4bd4-bbd5-a6920e4c5653";

// Analog control vars
float maxSerial = 100;
byte analogPin = A7;

void setup() {
  Serial.begin(9600);
  while (!Serial);

  // begin initialization
  if (!BLE.begin()) {
    Serial.println("starting BLE failed!");

    while (1);
  }

  Serial.println("Lovense BLE - Serial Vibrate Controller");

  // start scanning for peripherals
  BLE.scan();
}

void loop() {
  // check if a peripheral has been discovered
  BLEDevice peripheral = BLE.available();

  if (peripheral) {
    // discovered a peripheral, print out address, local name, and advertised service
    Serial.print("Found ");
    Serial.print(peripheral.address());
    Serial.print(" '");
    Serial.print(peripheral.localName());
    Serial.print("' ");
    Serial.print(peripheral.advertisedServiceUuid());
    Serial.println();

    // see if peripheral is a Lovense Max 2
    if (peripheral.localName() == lovenseLocalName) {
      // stop scanning
      BLE.stopScan();

      controlVibe(peripheral);

      // peripheral disconnected, we are done
      // start scanning for peripherals
      BLE.scan();
    }
  }
}

void controlVibe(BLEDevice peripheral) {
  // connect to the peripheral
  Serial.println("Connecting ...");

  if (peripheral.connect()) {
    Serial.println("Connected");
  } else {
    Serial.println("Failed to connect!");
    return;
  }

  // discover peripheral attributes
  Serial.println("Discovering attributes ...");
  if (peripheral.discoverAttributes()) {
    Serial.println("Attributes discovered");
  } else {
    Serial.println("Attribute discovery failed!");
    peripheral.disconnect();
    return;
  }

  // retrieve the vibration characteristic
  BLECharacteristic vibeCharacteristic = peripheral.characteristic(vibeCharacteristicUuid);

  if (!vibeCharacteristic) {
    Serial.println("Peripheral does not have Vibe characteristic!");
    peripheral.disconnect();
    return;
  } else if (!vibeCharacteristic.canWrite()) {
    Serial.println("Peripheral does not have a writable Vibe characteristic!");
    peripheral.disconnect();
    return;
  }
  
  Serial.println("Enter a strength between 0 and 20.");
  while (peripheral.connected()) {
    // Read the input on analog pin:
    int sensorValue = analogRead(analogPin);
    // Check the max value
    if (sensorValue > maxSerial){
      maxSerial = sensorValue;
    }
    // Scale it
    float scaled = (sensorValue / maxSerial) * 20;
    int strength = round(scaled);
    delay(1);        // delay in between reads for stability

    // Convert strength to char[]
    char cstr[2];
    memset(cstr, 0, sizeof cstr);
    itoa(strength, cstr, 10);
    // Make a command string
    char charArr[20];
    String cmd = "Vibrate:";
    String term = ";";
    strcpy(charArr, cmd.c_str());
    strcat(charArr, cstr);
    strcat(charArr, term.c_str());
    // Write the strength to BLE
    Serial.print("Sending command: ");
    Serial.println(charArr);
    
    if (!vibeCharacteristic.writeValue(charArr)){
      Serial.println("Error when writing characteristic.");  
    }
    
  }

  Serial.println("Peripheral disconnected");
}
