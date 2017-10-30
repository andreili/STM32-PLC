#ifndef STL_COMMANDS_H
#define STL_COMMANDS_H

#include <inttypes.h>

#define STL_CMD_TYPE uint8_t

enum class ESTLCommand
{
    Nesting_closed,

    Assign,
    CLR,    // untested
    FN,
    FP,
    NOT,
    O,
    O_net,
    ON,     // untested
    ON_net, // untested
    R,
    S,
    SAVE,   // untested
    SET,    // untested
    A,
    A_net,
    AN,
    AN_net, // untested
    X,      // untested
    X_net,  // untested
    XN,     // untested
    XN_net, // untested

    AD,
    AW,
    OD,
    OW,
    XOD,
    XOW,

    AR1_Add,
    AR2_Add,
    DEC,
    ENT,
    INC,
    LEAVE,
    NOP,
    POP,
    PUSH,
    TAK,

    Add,
    AR12_D_Add,
    AR12_D_Sub,
    AR12_D_Mul,
    AR12_D_Div,
    AR12_I_Add,
    AR12_I_Sub,
    AR12_I_Mul,
    AR12_I_Div,
    MOD,

    AR12_R_Add,
    AR12_R_Sub,
    AR12_R_Mul,
    AR12_R_Div,
    ABS,
    ACOS,
    ASIN,
    ATAN,
    COS,
    EXP,
    LN,
    SIN,
    SQR,
    SQRT,
    TAN,

    AR12_D_Eq,
    AR12_D_Ne,
    AR12_D_Gt,
    AR12_D_Lt,
    AR12_D_Ge,
    AR12_D_Le,
    AR12_I_Eq,
    AR12_I_Ne,
    AR12_I_Gt,
    AR12_I_Lt,
    AR12_I_Ge,
    AR12_I_Le,
    AR12_R_Eq,
    AR12_R_Ne,
    AR12_R_Gt,
    AR12_R_Lt,
    AR12_R_Ge,
    AR12_R_Le,

    BTD,
    BTI,
    DTB,
    DTR,
    INVD,
    INVI,
    ITB,
    ITD,
    NEGD,
    NEGI,
    NEGR,
    RND,
    RND_Add,
    RND_Sub,
    CAD,
    CAW,
    CDB,
    TRUNC,

    FR,
    LC_C,
    R_C,
    S_C,
    CD,
    CU,

    L,
    L_T,
    L_C,
    L_DBLG,
    L_DBNO,
    L_DILG,
    L_DINO,
    L_STW,
    LAR1,
    LAR1_AR2,
    LAR2,
    T,
    T_STW,
    CAR,
    TAR1,
    TAR1_AR2,
    TAR2,

    BE,
    BEU,
    BEC,
    BLD,
    CALL,
    CC,
    MCR_st,
    MCR_en,
    MCRA,
    MCRD,
    UC,

    RLD,
    RLDA,
    RRD,
    RRDA,
    SLD,
    SLW,
    SRD,
    SRW,
    SSD,
    SSI,

    LC_T,
    R_T,
    SF,
    SD,
    SP,
    SS,
    SE,

    LOOP,
    JU,
    JC,
    JCB,
    JBI,
    JNBI,
    JCN,
    JNB,
    JL,
    JM,
    JMZ,
    JN,
    JO,
    JP,
    JPZ,
    JOS,
    JUO,
    JZ,

    OPN,

    ARG_START,
    ARG_END,
    NETWORK_END,

    RET,
};

enum class ESTLCategory
{
    Integer_Match,
    Bit_Logic,
    Accumulator,
    Compare,
    Floating_point,
    DB_Call,
    Program_Control,
    Convert,
    Timers,
    Load_Transfer,
    Counters,
    Jumps,
    World_Logic,
    Shift_Rotate,
};

typedef struct
{
    ESTLCommand     command;
    const char*			mnemonic;
    ESTLCategory    category;
} STL_CMD_t;


#define STL_CMD_COUNT 173

extern STL_CMD_t STL_CMDs[STL_CMD_COUNT];

class STLCommands
{
public:
    STLCommands();

    static int32_t exec_command(STL_CMD_TYPE *prog_data);
private:
    static void proceed_real_op(float res);
    static void proceed_int32_op(int64_t res);
    static void proceed_int16_op(int32_t res);
};

#endif // STL_COMMANDS_H
