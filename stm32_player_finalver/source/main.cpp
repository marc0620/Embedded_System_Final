#include "BufferedSerial.h"
#include "CellularLog.h"
#include "Mutex.h"
#include "ThisThread.h"
#include "Thread.h"
#include "UDPSocket.h"
#include "integer.h"
#include "mac_timer.h"
#include "mbed.h"
#include "nsapi_types.h"
#include "wifi_helper.h"
#include "mbed-trace/mbed_trace.h"
#include <chrono>
#include <cstdint>
#include <ratio>
#include "Timer.h"
#define BUFFER_SIZE 8192
#define SampleRate 4410//numbers/s

static constexpr size_t REMOTE_PORT = 81;
Mutex buffer0Lock,buffer1Lock,recvlock;
Thread ReceivingData,PlayData;
NetworkInterface *_net;
TCPSocket socketh;
AnalogOut  aout(PA_4);
DAC_HandleTypeDef DAC1_Handler;
DAC_ChannelConfTypeDef DACCH1_Config;
bool sockinit=false,dacinit=false;




char recvbuffer[BUFFER_SIZE]={};
char playbuffer[BUFFER_SIZE]={};
void Socket_initialize(){
    _net=NetworkInterface::get_default_instance();
    if (!_net) {
            printf("Error! No network interface found.\r\n");
            return;
        }
        nsapi_size_or_error_t result = _net->connect();
        if (result != 0) {
            printf("Error! _net->connect() returned: %d\r\n", result);
            return;
        }
        result = socketh.open(_net);
        SocketAddress address;
        const char hostname[] = MBED_CONF_APP_HOSTNAME;
        result = _net->gethostbyname(hostname, &address);

        address.set_port(REMOTE_PORT);
        printf("Opening connection to remote port %d\r\n",REMOTE_PORT);
        result = socketh.connect(address);
        if (result != 0) {
            printf("Error! socketh.connect() returned: %d\r\n", result);
            return;
        }
    sockinit=true;
}

void DAC_initailize(){
    DAC1_Handler.Instance=DAC;
    HAL_DAC_Init(&DAC1_Handler); //初始化 DAC
    DACCH1_Config.DAC_Trigger=DAC_TRIGGER_NONE; //不使用触发功能
    DACCH1_Config.DAC_OutputBuffer=DAC_OUTPUTBUFFER_DISABLE;
    HAL_DAC_ConfigChannel(&DAC1_Handler,&DACCH1_Config,DAC_CHANNEL_1);
    HAL_DAC_Start(&DAC1_Handler, DAC_CHANNEL_1);
    dacinit=true;
}

volatile int playindex=2,recvindex=0;
volatile long long tick_counter=0;
void ticker_level(){
    

}

int main() {
    char a=255;
    DAC_initailize();
    Socket_initialize();
    Timer t;
    t.start();

    while(1){
        
        if(duration_cast<std::chrono::microseconds>(t.elapsed_time()).count()>=tick_counter*(1000000/SampleRate)){
            tick_counter=(duration_cast<std::chrono::microseconds>(t.elapsed_time()).count()/(1000000/SampleRate))+1;
            playindex+=2;
            recvindex+=2;
            if(playindex>=BUFFER_SIZE)playindex=0;
            if(recvindex>=BUFFER_SIZE)recvindex=0;
            HAL_DAC_SetValue(&DAC1_Handler, DAC_CHANNEL_1, DAC_ALIGN_12B_L, ((int16_t(playbuffer[playindex])<<8)+(uint8_t(playbuffer[playindex+1]))));
            playbuffer[playindex]=0;
            playbuffer[playindex+1]=0;
            socketh.recv(&playbuffer[recvindex],2);
        }
        
    }
    printf("end\r\n");


    while(1);
    return 0;
}
