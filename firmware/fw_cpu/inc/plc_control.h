#ifndef PLC_CONTROL_H
#define PLC_CONTROL_H

#include <stdint.h>

typedef struct
{
    uint8_t run;
    uint8_t stop;
    uint8_t fault;
    uint8_t initialized;
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
    static inline uint8_t is_initialized() { return m_state.initialized; }

    static inline void set_run(uint8_t val) { m_state.run = val; }
    static inline void set_stop(uint8_t val) { m_state.stop = val; }
    static inline void set_fault(uint8_t val) { m_state.fault = val; }
    static inline void set_initialized(uint8_t val) { m_state.initialized = val; }
private:
    static plc_state_t m_state;
};

#endif // PLC_CONTROL_H
