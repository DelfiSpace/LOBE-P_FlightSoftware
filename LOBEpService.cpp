#include <LOBEpService.h>

LOBEpService::LOBEpService(LOBEpRadio &LOBEp_in):
    LOBEp(&LOBEp_in){

};

bool LOBEpService::process(DataMessage &command, DataMessage &workingBuffer)
{
    if (command.getService() == LOBEp_SERVICE)
    {
//        Console::log("LOBEpService: Service started");
        workingBuffer.setService(LOBEp_SERVICE);
        workingBuffer.setMessageType(SERVICE_RESPONSE_REPLY);

        switch(command.getDataPayload()[0])
        {
        case LOBEp_CMD_INIT_TX:
            Console::log("LOBEpService: Init TX");
            LOBEp->initTX();
            workingBuffer.setPayloadSize(2);
            workingBuffer.getDataPayload()[0] = LOBEp_CMD_INIT_RX;
            workingBuffer.getDataPayload()[1] = LOBEp_CMD_NO_ERROR;
            break;
        case LOBEp_CMD_INIT_RX:
            Console::log("LOBEpService: Init TX");
            LOBEp->initRX();
            workingBuffer.setPayloadSize(2);
            workingBuffer.getDataPayload()[0] = LOBEp_CMD_INIT_RX;
            workingBuffer.getDataPayload()[1] = LOBEp_CMD_NO_ERROR;
            break;
        case LOBEp_CMD_SET_TX_IDLE:
            Console::log("LOBEpService: set TX idle mode:");
            if(command.getPayloadSize() == 2)
            {
                workingBuffer.setPayloadSize(2);
                workingBuffer.getDataPayload()[0] = LOBEp_CMD_SET_TX_IDLE;
                workingBuffer.getDataPayload()[1] = LOBEp_CMD_NO_ERROR;
                if(command.getDataPayload()[1] == 1){
                    LOBEp->enableTransmit();
                }else{
                    LOBEp->disableTransmit();
                }
            }
            else
            {
                workingBuffer.setPayloadSize(2);
                workingBuffer.getDataPayload()[0] = LOBEp_CMD_SET_TX_IDLE;
                workingBuffer.getDataPayload()[1] = LOBEp_CMD_INVALID_VALUE;
            }
            break;
        case LOBEp_CMD_SET_LNA_STATUS:
            Console::log("LOBEpService: set LNA state:");
            if(command.getPayloadSize() == 2)
            {
                workingBuffer.setPayloadSize(2);
                workingBuffer.getDataPayload()[0] = LOBEp_CMD_SET_LNA_STATUS;
                workingBuffer.getDataPayload()[1] = LOBEp_CMD_NO_ERROR;
                if(command.getDataPayload()[1] == 1){
                LOBEp->enableLNA();
                }else{
                    LOBEp->disableLNA();
                }
            }
            else
            {
                workingBuffer.setPayloadSize(2);
                workingBuffer.getDataPayload()[0] = LOBEp_CMD_SET_LNA_STATUS;
                workingBuffer.getDataPayload()[1] = LOBEp_CMD_INVALID_VALUE;
            }
            break;
        case LOBEp_CMD_SET_CAL_STATUS:
            Console::log("LOBEpService: set CAL state:");
            if(command.getPayloadSize() == 2)
            {
                workingBuffer.setPayloadSize(2);
                workingBuffer.getDataPayload()[0] = LOBEp_CMD_SET_CAL_STATUS;
                workingBuffer.getDataPayload()[1] = LOBEp_CMD_NO_ERROR;
                if(command.getDataPayload()[1] == 1){
                LOBEp->enableCalibration();
                }else{
                    LOBEp->disableCalibration();
                }
            }
            else
            {
                workingBuffer.setPayloadSize(2);
                workingBuffer.getDataPayload()[0] = LOBEp_CMD_SET_CAL_STATUS;
                workingBuffer.getDataPayload()[1] = LOBEp_CMD_INVALID_VALUE;
            }
            break;
        case LOBEp_CMD_SET_TX_FREQ:
            Console::log("LOBEpService: set TX freq (mixer):");
            if(command.getPayloadSize() == 5)
            {
                workingBuffer.setPayloadSize(2);
                workingBuffer.getDataPayload()[0] = LOBEp_CMD_SET_TX_FREQ;
                workingBuffer.getDataPayload()[1] = LOBEp_CMD_NO_ERROR;
                unsigned long ulong;
                ((unsigned char *)&ulong)[3] = command.getDataPayload()[1];
                ((unsigned char *)&ulong)[2] = command.getDataPayload()[2];
                ((unsigned char *)&ulong)[1] = command.getDataPayload()[3];
                ((unsigned char *)&ulong)[0] = command.getDataPayload()[4];
                Console::log("NEW FREQUENCY: %d", ulong);
                LOBEp->txFrequency = ulong;
                LOBEp->initTX();
                if(LOBEp->txEnabled){
                    LOBEp->enableTransmit();
                }
            }
            else
            {
                workingBuffer.setPayloadSize(2);
                workingBuffer.getDataPayload()[0] = LOBEp_CMD_SET_TX_FREQ;
                workingBuffer.getDataPayload()[1] = LOBEp_CMD_INVALID_VALUE;
            }
            break;
        case LOBEp_CMD_SWEEP_FREQ:
            Console::log("LOBEpService: SWEEP FREQUENCIES");
            if(command.getPayloadSize() == 1+4+4+2)
            {
                unsigned long startFreq;
                ((unsigned char *)&startFreq)[3] = command.getDataPayload()[1];
                ((unsigned char *)&startFreq)[2] = command.getDataPayload()[2];
                ((unsigned char *)&startFreq)[1] = command.getDataPayload()[3];
                ((unsigned char *)&startFreq)[0] = command.getDataPayload()[4];
                unsigned long stopFreq;
                ((unsigned char *)&stopFreq)[3] = command.getDataPayload()[5];
                ((unsigned char *)&stopFreq)[2] = command.getDataPayload()[6];
                ((unsigned char *)&stopFreq)[1] = command.getDataPayload()[7];
                ((unsigned char *)&stopFreq)[0] = command.getDataPayload()[8];
                unsigned short nrOfMeasurements;
                ((unsigned char *)&nrOfMeasurements)[1] = command.getDataPayload()[9];
                ((unsigned char *)&nrOfMeasurements)[0] = command.getDataPayload()[10];
                if(nrOfMeasurements <= MAX_NR_OF_MEASUREMENTS){
                    workingBuffer.setPayloadSize(2);
                    workingBuffer.getDataPayload()[0] = LOBEp_CMD_SWEEP_FREQ;
                    workingBuffer.getDataPayload()[1] = LOBEp_CMD_NO_ERROR;
                    LOBEp->startFreq = startFreq;
                    LOBEp->stopFreq = stopFreq;
                    LOBEp->nrOfMeasurements = nrOfMeasurements;
                    LOBEp->measurementIndex = 0;
                    LOBEp->enableLNA();
                    LOBEp->isMeasuring = true;
                }else{
                    workingBuffer.setPayloadSize(2);
                    workingBuffer.getDataPayload()[0] = LOBEp_CMD_SWEEP_FREQ;
                    workingBuffer.getDataPayload()[1] = LOBEp_CMD_INVALID_VALUE;
                }
            }
            else
            {
                workingBuffer.setPayloadSize(2);
                workingBuffer.getDataPayload()[0] = LOBEp_CMD_SWEEP_FREQ;
                workingBuffer.getDataPayload()[1] = LOBEp_CMD_INVALID_VALUE;
            }
            break;
        case LOBEp_CMD_SWEEP_RESULT:
            Console::log("LOBEpService: SWEEP RESULT");
            if(command.getPayloadSize() == 1)
            {
                if(!LOBEp->isMeasuring && LOBEp->completeMeasurement){
                    workingBuffer.setPayloadSize(2+LOBEp->nrOfMeasurements);
                    workingBuffer.getDataPayload()[0] = LOBEp_CMD_SWEEP_RESULT;
                    workingBuffer.getDataPayload()[1] = LOBEp_CMD_NO_ERROR;
                    for(int i = 0; i < LOBEp->nrOfMeasurements; i++){
                        workingBuffer.getDataPayload()[2+i] = LOBEp->measurements[i];
                    }
                }else{
                    workingBuffer.setPayloadSize(2);
                    workingBuffer.getDataPayload()[0] = LOBEp_CMD_SWEEP_RESULT;
                    workingBuffer.getDataPayload()[1] = LOBEp_CMD_NOT_DONE;
                }
            }
            else
            {
                workingBuffer.setPayloadSize(2);
                workingBuffer.getDataPayload()[0] = LOBEp_CMD_SWEEP_RESULT;
                workingBuffer.getDataPayload()[1] = LOBEp_CMD_INVALID_VALUE;
            }
            break;
        default:
            Console::log("LOBEpService: Unknown command!");
            workingBuffer.setPayloadSize(1);
            workingBuffer.getDataPayload()[0] = command.getDataPayload()[0];
            workingBuffer.getDataPayload()[0] = LOBEp_CMD_UNKNOWN_COMMAND;
            break;
        }

        return true;
    }
    else
    {
        // this command is related to another service,
        // report the command was not processed
        return false;
    }
}
