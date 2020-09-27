#include <mqtt.h>
#include <Arduino.h>

/*
Create the helper object
Giving it a unique id
*/
Mqtt_Helper::Mqtt_Helper(String ID="") {

    // check if an id was given or id is ok
    if (ID == "" || ID.length() != 6) { // no id given or bad id
        // generate a random id
        _ID = "ABCDEF";
    } else {
        _ID = ID;
    }
}

/*
Create a connect packet with authentication
Data is written into the byte array at *data
If length exceeds maxlen, no data is written
Make sure to offer a buffer large enough
*/
bool Mqtt_Helper::ConnectAuthenticate(char *data, String user, String password, int maxlen=255) {

    // estimate length of packet
    int packet_length = 2 + 6*2 + 6 + 6 + user.length() + password.length();

    if (packet_length > maxlen) {
        // available length is not sufficient
        return 1;
    }

    // connect header
    *data = 0x10;
    data++;

    // check remaining length with protocol
    packet_length = packet_length - 2;
    if (packet_length > 0xFF) {
        // remaining length does not fit
        return 1;
    }

    // remaining length
    *data = packet_length & 0xFF;
    data++;

    // packet id length
    *data = 0x00;
    data++;
    *data = 0x06;
    data++;

    // packet id
    String packetid = "MQIsdp";
    for (int i = 0; i < packetid.length(); i++) {
        *data = packetid[i];
        data++;
    }

    // level
    *data = 0x03;
    data++;

    // flags
    *data = 0xC2;
    data++;

    // i dont know what this is
    *data = 0x00;
    data++;
    *data = 0x3C;
    data++;

    // unique id length
    // I fixed id length to 6
    *data = 0x00;
    data++;
    *data = 0x06;
    data++;

    // unique id
    for (int i = 0; i < _ID.length(); i++) {
        *data = _ID[i];
        data++;
    }

    // username length
    if (user.length() > 0xFFFF) {
        // username too long
        return 1;
    }

    uint16_t user_length = user.length();
    *data = user_length & 0xFF00;
    data++;
    *data = user_length & 0xFF;
    data++;

    // username
    for (int i = 0; i < user.length(); i++) {
        *data = user[i];
        data++;
    }

    // password length
    if (password.length() > 0xFFFF) {
        // password too long
        return 1;
    }

    uint16_t password_length = password.length();
    *data = password_length & 0xFF00;
    data++;
    *data = password_length & 0xFF;
    data++;

    // password
    for (int i = 0; i < password.length(); i++) {
        *data = password[i];
        data++;
    }

    // no errors
    return 0;
}

/*
Create a publish packet
Data is written into the byte array at *data
If length exceeds maxlen, no data is written
Make sure to offer a buffer large enough
*/
bool Mqtt_Helper::Publish(char *data, String topic, String message, int maxlen=255) {

    // estimate length of packet
    int packet_length = 2 + 1*2 + topic.length() + message.length();

    if (packet_length > maxlen) {
        // available length is not sufficient
        return 1;
    }

    // publish header
    *data = 0x30;
    data++;

    // check remaining length with protocol
    packet_length = packet_length - 2;
    if (packet_length > 0xFF) {
        // remaining length does not fit
        return 1;
    }

    // remaining length
    *data = packet_length & 0xFF;
    data++;

    // topic length
    if (topic.length() > 0xFFFF) {
        // topic too long
        return 1;
    }

    uint16_t topic_length = topic.length();
    *data = topic_length & 0xFF00;
    data++;
    *data = topic_length & 0xFF;
    data++;

    // topic
    for (int i = 0; i < topic.length(); i++) {
        *data = topic[i];
        data++;
    }

    // message
    for (int i = 0; i < message.length(); i++) {
        *data = message[i];
        data++;
    }

    // no errors
    return 0;
}