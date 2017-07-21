#ifndef PLC_CONTROL_H
#define PLC_CONTROL_H

#include <stdint.h>

typedef struct
{
    uint8_t run: 1;
    uint8_t stop: 1;
    uint8_t fault: 1;
    union
    {
        struct
        {
            uint32_t bank_start;
            uint32_t bank_end;
            uint32_t test_size;
            uint32_t cur_start;
        } mem_test;
    } data;
} plc_state_t;

class PLC_CONTROL
{
public:
    static void init();

    static inline uint8_t in_run() { return m_state.run; }
    static inline uint8_t in_stop() { return m_state.stop; }
    static inline uint8_t in_fault() { return m_state.fault; }

    static inline void set_run(uint8_t val) { m_state.run = val; }
    static inline void set_stop(uint8_t val) { m_state.stop = val; }
    static inline void set_fault(uint8_t val) { m_state.fault = val; }
private:
    static plc_state_t m_state;
};

#endif // PLC_CONTROL_H
