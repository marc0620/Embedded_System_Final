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
#include "Timer.h"
#define BUFFER_SIZE 2048
#define SampleRate 300//numbers/s

static constexpr size_t REMOTE_PORT = 84;
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

volatile int playindex=1024,recvindex=0;
void index_shift(){
    //if(playbuffer[playindex])
    
    playindex+=1;
    recvindex+=1;
    if(playindex>=BUFFER_SIZE)playindex=0;
    if(recvindex>=BUFFER_SIZE)recvindex=0;
    // HAL_DAC_SetValue(&DAC1_Handler, DAC_CHANNEL_1, DAC_ALIGN_12B_R, int(playbuffer[playindex])<<3);
    if(playbuffer[playindex]==0 && playindex!=0){
        HAL_DAC_SetValue(&DAC1_Handler, DAC_CHANNEL_1, DAC_ALIGN_12B_R, int(playbuffer[playindex-1])<<3);
    }
    else{
        HAL_DAC_SetValue(&DAC1_Handler, DAC_CHANNEL_1, DAC_ALIGN_12B_R, int(playbuffer[playindex])<<3);
    }
    playbuffer[playindex]=0;
}


void receiveData(){
    int prev=0;
    while(1){
        if(recvindex != prev){
            prev = recvindex;
            //HAL_DAC_SetValue(&DAC1_Handler, DAC_CHANNEL_1, DAC_ALIGN_12B_R, int(playbuffer[playindex])<<3);
            nsapi_size_or_error_t result;
            result=socketh.recv(&playbuffer[recvindex],1);

        }
    }
    // printf("error");
    // printf("\r\n");
   
        
}
void ticker_level(){
    Ticker t;
    
    t.attach(&index_shift,(1000000us/SampleRate));
    while(1);
}

int main() {
    char a=255;
    DAC_initailize();
    Socket_initialize();
    ThisThread::sleep_for(1000);
    
    ReceivingData.start(callback(&receiveData));
    PlayData.start(callback(&ticker_level));

    while(1);
    return 0;
}
