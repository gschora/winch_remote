
// JN_SimpleReceive.pde   Illustrates receiving data from JN_SimpleSend.pde
// updated 6-4-2012 by paul badger from code by <jcw@equi4.com>
// http://opensource.org/licenses/mit-license.php
// Sketch to receive data from SimpleSend

#include <JeeLib.h>


int pinUp = 6;
int pinDown = 4;
int waitTime = 300; //time to wait before shutting off
int timecnt = 0;
int tm = 0;

void setup() {
        // Serial.begin(57600);
        rf12_initialize(20, RF12_868MHZ, 43); // params are byte nodeId, byte freqBand, byte netGroup
                                              // freqBands  should be RF12_915MHZ, or RF12_433MHZ
                                              // nodeId parameter should be in range of to 1-26
                                              // netGroup parameter should be in range of to 1-212

        // red dots on radios are 915Mhz, green dots are 434 Mhz, yellow dots are 868Mhz

        pinMode(pinUp, OUTPUT);
        pinMode(pinDown, OUTPUT);
}

void loop() {
        // rf12_recvDone() is true if new information has been received.
        // re12_crc == 0 indicates good transmission, checks validity of data

        if (rf12_recvDone() && rf12_crc == 0) {  // received good data if true
                int* buf = (int*) rf12_data;   // buf is a pointer to our data
                                               // the same as an array name

                if (buf[0]==22 && buf[1]==44) {
                        // Serial.println("down");
                        down();
                }else if(buf[0]==33 && buf[1]==66) {
                        // Serial.println("up");
                        up();
                }
        }
        delay(100);
        tm = millis();
        // Serial.println(tm-timecnt);
        if ((tm - timecnt) >= waitTime) {
                off();
                timecnt = tm;
                // Serial.println("off");
        }
}

void up(){
        digitalWrite(pinDown, LOW);
        digitalWrite(pinUp, HIGH);
        timecnt = millis();

}
void down(){
        digitalWrite(pinUp, LOW);
        digitalWrite(pinDown, HIGH);
        timecnt = millis();
}
void off(){
        digitalWrite(pinUp, LOW);
        digitalWrite(pinDown, LOW);
}
