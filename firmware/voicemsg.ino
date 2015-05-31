#include "Emic2TTS.h"
#include "SPIUart.h"
#include "Adafruit_FONA.h"

#define FONA_RST                    D1
#define FONA_RI_INTERRUPT           RX

#define FONA_INITIALIZED            0
#define FONA_ERROR_NOT_INITIALIZED  1
#define FONA_ERROR_NO_CALLER_NOTIF  2

#define EVENT_FONA                  "vmsg-event"

#define MAX_ARGS                    64
#define MAX_MESSAGE                 63
#define MAX_PHONE                   13

Adafruit_FONA fona = Adafruit_FONA(FONA_RST);
SPIUart SPIUart;
Emic2TtsModule emic;

int fonaState = FONA_ERROR_NOT_INITIALIZED;

char szMessage[MAX_MESSAGE + 1];
char szNumber[MAX_PHONE + 1];

boolean isMessageSent = false;

int setphnumber(String args){
    if(FONA_INITIALIZED != fonaState){
        return -100;
    }
    
    char szArgs[MAX_ARGS];
    
    args.toCharArray(szNumber, MAX_PHONE);
    
    Serial.println();
    Serial.print("Number: ");
    Serial.print(szArgs);
    Serial.println();
    
    return 1;
}

int setemicvoice(String args){
    EmicVoice ev = PerfectPaul;
    int voice = args.toInt();
    
    Serial.print("Voice: ");
    Serial.println(voice);
    
    switch(voice){
        case 0:     ev = PerfectPaul;       break;
        case 1:     ev = HugeHarry;         break;
        case 2:     ev = BeautifulBetty;    break;
        case 3:     ev = UppityUrsula;      break;
        case 4:     ev = DoctorDennis;      break;
        case 5:     ev = KitTheKid;         break;
        case 6:     ev = FrailFrank;        break;
        case 7:     ev = RoughRita;         break;
        case 8:     ev = WhisperingWendy;   break;
        default:    ev = PerfectPaul;       break;
    }
    
    emic.setVoice(ev);
    
    return 1;
}

int sendvoicemessage(String args){
    if(FONA_INITIALIZED != fonaState){
        return -100;
    }
    
    args.toCharArray(szMessage, MAX_MESSAGE);
    
    fona.callPhone(szNumber);
    
    return 1;
}

int setautoanswer(String args){
    int count = args.toInt();
    
    return fona.setAutoAnswer(count) ? 1 : 0;
}

void Publish(String message){
    Spark.publish(EVENT_FONA, message, 60, PRIVATE);
}

void setup() {
    Serial.begin(9600);
    Serial1.begin(9600);
    
    Spark.function("setnumber", setphnumber);
    Spark.function("setvoice", setemicvoice);
    Spark.function("sendvoice", sendvoicemessage);
    Spark.function("autoans", setautoanswer);
    
    SPIUart.begin();
    
    if(fona.begin(SPIUart)) {
        if(fona.callerIdNotification(true, FONA_RI_INTERRUPT)){
            fonaState = FONA_INITIALIZED;
            
            Serial.println("FONA Initialized!");
            
            Publish("FONA initialized");
        }
        else{
            Serial.println("FONA Caller Identificaiton not enabled");
            
            Publish("FONA Caller ID not Enabled");
            
            fonaState = FONA_ERROR_NO_CALLER_NOTIF;
        }
    }
    else{
        Publish("FONA not initialized");
        Serial.println("FONA not initialized");
        
        fonaState = FONA_ERROR_NOT_INITIALIZED;
    }
    
    
    emic.init(Serial1);
    setemicvoice("3");
    
    Publish("SPIUart and Emic initialized");
    
    strncpy(szMessage, "Welcome to Spark Core IVR Sample.", MAX_MESSAGE);
}

void loop() {
    if(FONA_INITIALIZED == fonaState){
        char phone[64] = {0};
        int status = fona.getStatus(phone);
        
        if(-1 == status){
            isMessageSent = false;
        }
        else{
            switch(status){
                case 0:     Serial.print("Active Call");        break;
                case 2:     Serial.print("Dialing");            break;
                case 3:     Serial.print("Ringing");            break;
                case 4:     Serial.print("Incoming Call");      break;
                case 5:     Serial.print("Waiting");            break;
                default4:   Serial.print("Unknown");            break;
            }
            
            Serial.print(" ");
            Serial.print(phone);
            
            Serial.println();
        }
        
        if(status == 0){
            if(!isMessageSent){
                EmicVoice ev = WhisperingWendy;
                
                emic.setVoice(ev);
                emic.say(szMessage);

                isMessageSent = true;
                
                fona.hangUp();
            }
        }
    }
    else{
        Serial.println("FONA not initialized");
    }
    
    delay(10);
}