#ifndef PLCDATA_H
#define PLCDATA_H

#include "settings.h"
#include "datainterface.h"

extern DataInterface<IO_AREA_SIZE> plc_inputs;
extern DataInterface<IO_AREA_SIZE> plc_outputs;
extern DataInterface<MEM_AREA_SIZE> plc_memory;

#endif // PLCDATA_H
