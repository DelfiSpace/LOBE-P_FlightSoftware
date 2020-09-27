
#include "LOBEpRadio.h"
#include "Service.h"
#include "Console.h"

#ifndef LOBEpSERVICE_H_
#define LOBEpSERVICE_H_

#define LOBEp_SERVICE               35
#define LOBEp_CMD_INIT_TX           1
#define LOBEp_CMD_INIT_RX           2

#define LOBEp_CMD_SET_TX_IDLE       13
#define LOBEp_CMD_SET_LNA_STATUS    14
#define LOBEp_CMD_SET_CAL_STATUS    15
#define LOBEp_CMD_SET_TX_FREQ       16

#define LOBEp_CMD_NO_ERROR          0
#define LOBEp_CMD_UNKNOWN_COMMAND   1
#define LOBEp_CMD_INVALID_VALUE     2


class LOBEpService: public Service
{
protected:
    LOBEpRadio *LOBEp;
public:
    LOBEpService(LOBEpRadio &LOBEp_in);

    virtual bool process( DataMessage &command, DataMessage &workingBbuffer );
};

#endif
