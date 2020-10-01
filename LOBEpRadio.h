#include "SX1276.h"
#include "DSPI.h"
#include "sx1276Enums.h"
#include "Task.h"
#include "Console.h"

#ifndef LOBEPRADIO_H_
#define LOBEPRADIO_H_

#define DB_PORT GPIO_PORT_P2
#define DB_CAL_PIN GPIO_PIN0
#define DB_LNA_PIN GPIO_PIN7

#define MAX_NR_OF_MEASUREMENTS 200

class LOBEpRadio : public Task
{
protected:
    DSPI *bitSPI_tx;
    DSPI *bitSPI_rx;
    DSPI *packetSPI;


    //txRadioSettings:
    uint8_t powerByte = 0x0D-2; //should be 7dBm



public:
    LOBEpRadio(DSPI &bitModeSPI_tx, DSPI &bitModeSPI_rx, DSPI &packetModeSPI, SX1276 &txRad, SX1276 &rxRad);

    volatile bool txEnabled = false;


    bool notified( void );

    SX1276 *txRadio;
    SX1276 *rxRadio;
    TxConfig_t txConfig;
    RxConfig_t rxConfig;

    volatile bool isMeasuring;
    volatile bool completeMeasurement;

    uint8_t measurements[MAX_NR_OF_MEASUREMENTS];

    unsigned long startFreq;
    unsigned long stopFreq;
    unsigned int measurementIndex;
    unsigned int nrOfMeasurements;

    unsigned long txFrequency = 429000000;
    unsigned long rxFrequency = 430000000;

    void runTask();
    void init();
    void initTX();
    void initRX();

    void onReceive(uint8_t data);
    uint8_t onTransmit();

    void enableTransmit();
    void disableTransmit();

    signed short getRXRSSI();

    void enableLNA();
    void disableLNA();

    void enableCalibration();
    void disableCalibration();

    signed short lastRSSI;
};

#endif
