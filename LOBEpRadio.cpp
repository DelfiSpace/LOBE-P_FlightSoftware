#include "LOBEpRadio.h"

#ifndef MODFUNC
#define MODFUNC
int mod(int a, int b)
{ return a<0 ? (a%b+b)%b : a%b; }
#endif

LOBEpRadio* radioStub;
uint8_t onTransmitWrapper(){
    //serial.println("transmitter stub!");
    return radioStub->onTransmit();
};
void onReceiveWrapper(uint8_t data){
    //serial.println("receiver stub!");
    radioStub->onReceive(data);
};

void taskWrapper(){
    radioStub->runTask();
}

LOBEpRadio::LOBEpRadio(DSPI &bitModeSPI_tx, DSPI &bitModeSPI_rx, DSPI &packetModeSPI, SX1276 &txRad, SX1276 &rxRad):
       Task(taskWrapper), bitSPI_tx(&bitModeSPI_tx), bitSPI_rx(&bitModeSPI_rx), packetSPI(&packetModeSPI), txRadio(&txRad), rxRadio(&rxRad)
{
    radioStub = this;
};

bool LOBEpRadio::notified(){
    return false; //todo? Automatically make / store measurements
}


void LOBEpRadio::runTask(){

    //Make / Store Measurements?
    return;

}

uint8_t LOBEpRadio::onTransmit(){
    //NOTE, SPI Bus is configured to MSB_first, meaning that the bit transmitted first should be the MSB.
    //just return 0 in order to have a idle carrier
    return 0;
};

void LOBEpRadio::onReceive(uint8_t data)
{
    //Note: SPI Bus is configured MSB First, meaning that from the received Byte, the MSB was the first received Bit
    //Store RSSI? would be nice?
    return;
};

void LOBEpRadio::init(){
    Console::log("LOBEp Radio Object Starting...");

    Console::log("Initialize TX and RX");
    this->initTX();
    this->initRX();

    Console::log("Configure DB Pins");
        MAP_GPIO_setOutputLowOnPin(DB_PORT, DB_LNA_PIN);
        MAP_GPIO_setAsOutputPin(DB_PORT, DB_LNA_PIN);
        MAP_GPIO_setOutputLowOnPin(DB_PORT, DB_CAL_PIN);
        MAP_GPIO_setAsOutputPin(DB_PORT, DB_CAL_PIN);
};

void LOBEpRadio::initTX(){
    // Initialise TX values
    // Modem set to FSK, deviation set to 1/2 datarate, gaussian filter enabled
    txRadio->init();

    if(txRadio->ping()){

        txConfig.modem = MODEM_FSK;
        txConfig.filtertype = BT_0_5;
        txConfig.bandwidth = 0; //not necessary for tx
        txConfig.fdev = 0; //nothing for pure carrier
        txConfig.datarate = 1200; //placeholder value for how often function is called
        txConfig.power = powerByte;

        txRadio->setFrequency(txFrequency);
        txRadio->enableBitMode(*bitSPI_tx, 0, onTransmitWrapper);
        txRadio->setTxConfig(&txConfig);

        Console::log("TX Radio Settings Set");
    }
    else{
        Console::log("TX Radio not Found");
    }
};

void LOBEpRadio::initRX(){
    // Initialise RX values
    // GMSK:
    // Modem set to FSK, deviation set to 1/2 data-rate, Gaussian filter enabled
    if(rxRadio->ping()){
        rxConfig.modem = MODEM_FSK;
        rxConfig.filtertype = BT_0_5;
        rxConfig.bandwidth = 10000;
        rxConfig.bandwidthAfc = 83333;
        rxConfig.fdev = 0;
        rxConfig.datarate = 1200;

    //        Console::log("LNA_FREQ");
        rxRadio->setFrequency(rxFrequency);

        rxRadio->RxChainCalibration();
        rxRadio->enableBitMode(*bitSPI_rx, onReceiveWrapper, 0);

        rxRadio->setRxConfig(&rxConfig);
        rxRadio->startReceiver();

        Console::log("RX Radio Settings Set");
    }else{
        Console::log("RX Radio not Found");
    }
};

void LOBEpRadio::enableTransmit(){
    Console::log("Enabling Transmitter!");
    txEnabled = true;
    txRadio->setIdleMode(true);
    // else the radio is already on.

    //rxPrint = true;
    //rxReady = false;
}

void LOBEpRadio::disableTransmit(){
    Console::log("TRANSMIT DISABLED");
    txRadio->setIdleMode(false);
    txEnabled = false;
}

signed short LOBEpRadio::getRXRSSI(){
    return rxRadio->GetRssi(ModemType::MODEM_FSK);
}

void LOBEpRadio::enableLNA(){
    Console::log("LOBEpRadio - Enabling LNA");
    MAP_GPIO_setOutputHighOnPin(DB_PORT, DB_LNA_PIN);
}

void LOBEpRadio::disableLNA(){
    Console::log("LOBEpRadio - Disabling LNA");
    MAP_GPIO_setOutputLowOnPin(DB_PORT, DB_LNA_PIN);
}

void LOBEpRadio::enableCalibration(){
    Console::log("LOBEpRadio - Enabling Calibration");
    MAP_GPIO_setOutputHighOnPin(DB_PORT, DB_CAL_PIN);
}

void LOBEpRadio::disableCalibration(){
    Console::log("LOBEpRadio - Disabling Calibration");
    MAP_GPIO_setOutputLowOnPin(DB_PORT, DB_CAL_PIN);
}
