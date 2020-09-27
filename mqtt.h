#include <Arduino.h>

class Mqtt_Helper {

public:
	// constructor
	Mqtt_Helper(String ID="");

    // methods
    int ConnectAuthenticate(char *data, String user, String password, int maxlen=255);
    int Publish(char *data, String topic, String message, int maxlen=255);

private:
    // a unique ID
    String _ID;
};