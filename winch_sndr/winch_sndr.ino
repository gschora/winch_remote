/* JN_SimpleSend, transmitter side. Test transmission by sending some data
 *  Sketch to send some arbitrary data to test JeeNode transmission
 *  Put this sketch on a JeeNode with a portable (battery) power supply
 *  updated 6-4-2012 by Paul Badger from code by <jcw@equi4.com>
 *  http://opensource.org/licenses/mit-license.php
 *
 */

#include <JeeLib.h>

ISR(WDT_vect) {
        Sleepy::watchdogEvent();
}

byte seqnum;

static unsigned int btnPinUp = 5;
static unsigned int btnPinDown = 7;
static unsigned int irqPin = 3;
// the variables below are just "hardwired" data to send
// later we will read a sensor and put some real data into the variables

int bufUP[2];
int bufDOWN[2];

boolean debug = true; //sets debug mode for auto sending different up/down packets, simulating button-press


void setup() {
        Serial.begin(57600);
        pinMode(btnPinUp, INPUT);
        pinMode(btnPinDown, INPUT);

        rf12_initialize(21, RF12_868MHZ, 43); // init the radio

        rf12_sleep(RF12_SLEEP);

        // params are byte nodeId, byte freqBand, byte netGroup
        // freqBands  should be RF12_915MHZ, or RF12_433MHZ
        // nodeId parameter should be in range of to 1-26
        // netGroup parameter should be in range of to 1-212
        // red dots on JeeNode radios are 915Mhz, green dots are 434 Mhz

        bufUP[0] = 33;
        bufUP[1] = 66;
        bufDOWN[0] = 22;
        bufDOWN[1] = 44;
}

void loop() {
        chkBtn();

        if (debug) {
                testsnd();
        }

        Sleepy::loseSomeTime(300);
        // delay(300);

}

void chkBtn(){
        if (digitalRead(btnPinUp)) {
                Serial.println("up");
                sendUP();
        } else if (digitalRead(btnPinDown)) {
                Serial.println("down");
                sendDOWN();
        } else {
                Serial.println("nothing");
                rf12_sleep(RF12_SLEEP);
        }
}
void btnWake(){
        Serial.println("wake");
}

void sendUP(){
        rf12_sleep(RF12_WAKEUP);

        while (!rf12_canSend()) {
                rf12_recvDone(); // processes any incoming data
        }// wait until sending is allowed

        // rf12_recvDone(); // processes any incoming data
        // it all happens "in the background"

        rf12_sendStart(0, bufUP, sizeof bufUP); // sends the data out
        rf12_sendWait(2);
        rf12_sleep(RF12_SLEEP);
}
void sendDOWN(){
        rf12_sleep(RF12_WAKEUP);

        while (!rf12_canSend()) {
                rf12_recvDone(); // processes any incoming data
        } // wait until sending is allowed
          // rf12_recvDone(); // processes any incoming data
          // it all happens "in the background"

        rf12_sendStart(0, bufDOWN, sizeof bufDOWN); // sends the data out
        rf12_sendWait(2);
        rf12_sleep(RF12_SLEEP);
}

int sndswitch = 0;
int sndcnt = 0;
int rndcnt = 0;

void testsnd(){
        switch(sndswitch) {
        case 0:
                sendUP();
                sndcnt++;
                break;
        case 1:
                sendDOWN();
                sndcnt++;
                break;
        case 2:
                sndcnt++;
                break;
        }
        if(sndcnt >= rndcnt) resetRndCnt();
}
void resetRndCnt(){
        rndcnt = random(1,11);
        sndcnt = 0;
        sndswitch=random(0,3);
}
