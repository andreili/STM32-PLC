#include "stl_commands.h"
#ifndef STLC_DEF
#include "plc_fw.h"
#include "plc_hw.h"
#endif
#include <cmath>
#include <stdio.h>

#define NO_ARGS() *((ESTLCommand*)&prog_data[size - sizeof(STL_CMD_TYPE)]) == ESTLCommand::ARG_END

STL_CMD_t STL_CMDs[STL_CMD_COUNT] = {
    {ESTLCommand::Nesting_closed, ")", ESTLCategory::Bit_Logic},    //0

    {ESTLCommand::Assign, "=", ESTLCategory::Bit_Logic},
    {ESTLCommand::CLR, "CLR", ESTLCategory::Bit_Logic},
    {ESTLCommand::FN, "FN", ESTLCategory::Bit_Logic},
    {ESTLCommand::FP, "FP", ESTLCategory::Bit_Logic},
    {ESTLCommand::NOT, "NOT", ESTLCategory::Bit_Logic},
    {ESTLCommand::O, "O", ESTLCategory::Bit_Logic},
    {ESTLCommand::O_net, "O(", ESTLCategory::Bit_Logic},
    {ESTLCommand::ON, "ON", ESTLCategory::Bit_Logic},
    {ESTLCommand::ON_net, "ON(", ESTLCategory::Bit_Logic},
    {ESTLCommand::R, "R", ESTLCategory::Bit_Logic},                 //10
    {ESTLCommand::S, "S", ESTLCategory::Bit_Logic},
    {ESTLCommand::SAVE, "SAVE", ESTLCategory::Bit_Logic},
    {ESTLCommand::SET, "SET", ESTLCategory::Bit_Logic},
    {ESTLCommand::A, "A", ESTLCategory::Bit_Logic},
    {ESTLCommand::A_net, "A(", ESTLCategory::Bit_Logic},
    {ESTLCommand::AN, "AN", ESTLCategory::Bit_Logic},
    {ESTLCommand::AN_net, "AN(", ESTLCategory::Bit_Logic},
    {ESTLCommand::X, "X", ESTLCategory::Bit_Logic},
    {ESTLCommand::X_net, "X(", ESTLCategory::Bit_Logic},
    {ESTLCommand::XN, "XN", ESTLCategory::Bit_Logic},               //20
    {ESTLCommand::XN_net, "XN(", ESTLCategory::Bit_Logic},

    {ESTLCommand::AD, "AD", ESTLCategory::World_Logic},
    {ESTLCommand::AW, "AW", ESTLCategory::World_Logic},
    {ESTLCommand::OD, "OD", ESTLCategory::World_Logic},
    {ESTLCommand::OW, "OW", ESTLCategory::World_Logic},
    {ESTLCommand::XOD, "XOD", ESTLCategory::World_Logic},
    {ESTLCommand::XOW, "XOW", ESTLCategory::World_Logic},

    {ESTLCommand::AR1_Add, "+AR1", ESTLCategory::Accumulator},
    {ESTLCommand::AR2_Add, "+AR2", ESTLCategory::Accumulator},
    {ESTLCommand::DEC, "DEC", ESTLCategory::Accumulator},
    {ESTLCommand::ENT, "ENT", ESTLCategory::Accumulator},
    {ESTLCommand::INC, "INC", ESTLCategory::Accumulator},
    {ESTLCommand::LEAVE, "LEAVE", ESTLCategory::Accumulator},
    {ESTLCommand::NOP, "NOP", ESTLCategory::Accumulator},
    {ESTLCommand::POP, "POP", ESTLCategory::Accumulator},
    {ESTLCommand::PUSH, "PUSH", ESTLCategory::Accumulator},
    {ESTLCommand::TAK, "TAK", ESTLCategory::Accumulator},

    {ESTLCommand::Add, "+", ESTLCategory::Integer_Match},
    {ESTLCommand::AR12_D_Add, "+D", ESTLCategory::Integer_Match},
    {ESTLCommand::AR12_D_Sub, "-D", ESTLCategory::Integer_Match},
    {ESTLCommand::AR12_D_Mul, "*D", ESTLCategory::Integer_Match},
    {ESTLCommand::AR12_D_Div, "/D", ESTLCategory::Integer_Match},
    {ESTLCommand::AR12_I_Add, "+I", ESTLCategory::Integer_Match},
    {ESTLCommand::AR12_I_Sub, "-I", ESTLCategory::Integer_Match},
    {ESTLCommand::AR12_I_Mul, "*I", ESTLCategory::Integer_Match},
    {ESTLCommand::AR12_I_Div, "/I", ESTLCategory::Integer_Match},
    {ESTLCommand::MOD, "MOD", ESTLCategory::Integer_Match},

    {ESTLCommand::AR12_R_Add, "+R", ESTLCategory::Floating_point},
    {ESTLCommand::AR12_R_Sub, "-R", ESTLCategory::Floating_point},
    {ESTLCommand::AR12_R_Mul, "*R", ESTLCategory::Floating_point},
    {ESTLCommand::AR12_R_Div, "/R", ESTLCategory::Floating_point},
    {ESTLCommand::ABS, "ABS", ESTLCategory::Floating_point},
    {ESTLCommand::ACOS, "ACOS", ESTLCategory::Floating_point},
    {ESTLCommand::ASIN, "ASIN", ESTLCategory::Floating_point},
    {ESTLCommand::ATAN, "ATAN", ESTLCategory::Floating_point},
    {ESTLCommand::COS, "COS", ESTLCategory::Floating_point},
    {ESTLCommand::EXP, "EXP", ESTLCategory::Floating_point},
    {ESTLCommand::LN, "LN", ESTLCategory::Floating_point},
    {ESTLCommand::SIN, "SIN", ESTLCategory::Floating_point},
    {ESTLCommand::SQR, "SQR", ESTLCategory::Floating_point},
    {ESTLCommand::SQRT, "SQRT", ESTLCategory::Floating_point},
    {ESTLCommand::TAN, "TAN", ESTLCategory::Floating_point},

    {ESTLCommand::AR12_D_Eq, "==D", ESTLCategory::Compare},
    {ESTLCommand::AR12_D_Ne, "<>D", ESTLCategory::Compare},
    {ESTLCommand::AR12_D_Gt, ">D",  ESTLCategory::Compare},
    {ESTLCommand::AR12_D_Lt, "<D",  ESTLCategory::Compare},
    {ESTLCommand::AR12_D_Ge, ">=D", ESTLCategory::Compare},
    {ESTLCommand::AR12_D_Le, "<=D", ESTLCategory::Compare},
    {ESTLCommand::AR12_I_Eq, "==I", ESTLCategory::Compare},
    {ESTLCommand::AR12_I_Ne, "<>I", ESTLCategory::Compare},
    {ESTLCommand::AR12_I_Gt, ">I",  ESTLCategory::Compare},
    {ESTLCommand::AR12_I_Lt, "<I",  ESTLCategory::Compare},
    {ESTLCommand::AR12_I_Ge, ">=I", ESTLCategory::Compare},
    {ESTLCommand::AR12_I_Le, "<=I", ESTLCategory::Compare},
    {ESTLCommand::AR12_R_Eq, "==R", ESTLCategory::Compare},
    {ESTLCommand::AR12_R_Ne, "<>R", ESTLCategory::Compare},
    {ESTLCommand::AR12_R_Gt, ">R",  ESTLCategory::Compare},
    {ESTLCommand::AR12_R_Lt, "<R",  ESTLCategory::Compare},
    {ESTLCommand::AR12_R_Ge, ">=R", ESTLCategory::Compare},
    {ESTLCommand::AR12_R_Le, "<=R", ESTLCategory::Compare},

    {ESTLCommand::BTD, "BTD", ESTLCategory::Convert},
    {ESTLCommand::BTI, "BTI", ESTLCategory::Convert},
    {ESTLCommand::DTB, "DTB", ESTLCategory::Convert},
    {ESTLCommand::DTR, "DTR", ESTLCategory::Convert},
    {ESTLCommand::INVD, "INVD", ESTLCategory::Convert},
    {ESTLCommand::INVI, "INVI", ESTLCategory::Convert},
    {ESTLCommand::ITB, "ITB", ESTLCategory::Convert},
    {ESTLCommand::ITD, "ITD", ESTLCategory::Convert},
    {ESTLCommand::NEGD, "NEGD", ESTLCategory::Convert},
    {ESTLCommand::NEGI, "NEGI", ESTLCategory::Convert},
    {ESTLCommand::NEGR, "NEGR", ESTLCategory::Convert},
    {ESTLCommand::RND, "RND", ESTLCategory::Convert},
    {ESTLCommand::RND_Add, "RND+", ESTLCategory::Convert},
    {ESTLCommand::RND_Sub, "RND-", ESTLCategory::Convert},
    {ESTLCommand::CAD, "CAD", ESTLCategory::Convert},
    {ESTLCommand::CAW, "CAW", ESTLCategory::Convert},
    {ESTLCommand::CDB, "CDB", ESTLCategory::Convert},
    {ESTLCommand::TRUNC, "TRUNC", ESTLCategory::Convert},

    {ESTLCommand::FR, "FR", ESTLCategory::Counters},
    {ESTLCommand::LC_C, "LC C", ESTLCategory::Counters},
    {ESTLCommand::R_C, "R_C", ESTLCategory::Counters},
    {ESTLCommand::S_C, "S C", ESTLCategory::Counters},
    {ESTLCommand::CD, "CD", ESTLCategory::Counters},
    {ESTLCommand::CU, "CU", ESTLCategory::Counters},

    {ESTLCommand::L, "L", ESTLCategory::Load_Transfer},
    {ESTLCommand::L_T, "L T", ESTLCategory::Load_Transfer},
    {ESTLCommand::L_C, "L C", ESTLCategory::Load_Transfer},
    {ESTLCommand::L_DBLG, "L DBLG", ESTLCategory::Load_Transfer},
    {ESTLCommand::L_DBNO, "L DBNO", ESTLCategory::Load_Transfer},
    {ESTLCommand::L_DILG, "L DILG", ESTLCategory::Load_Transfer},
    {ESTLCommand::L_DINO, "L DINO", ESTLCategory::Load_Transfer},
    {ESTLCommand::L_STW, "L STW", ESTLCategory::Load_Transfer},
    {ESTLCommand::LAR1, "LAR1", ESTLCategory::Load_Transfer},
    {ESTLCommand::LAR1_AR2, "LAR1 AR2", ESTLCategory::Load_Transfer},
    {ESTLCommand::LAR2, "LAR2", ESTLCategory::Load_Transfer},
    {ESTLCommand::T, "T", ESTLCategory::Load_Transfer},
    {ESTLCommand::T_STW, "T STW", ESTLCategory::Load_Transfer},
    {ESTLCommand::CAR, "CAR", ESTLCategory::Load_Transfer},
    {ESTLCommand::TAR1, "TAR1", ESTLCategory::Load_Transfer},
    {ESTLCommand::TAR1_AR2, "TAR1 AR2", ESTLCategory::Load_Transfer},
    {ESTLCommand::TAR2, "TAR2", ESTLCategory::Load_Transfer},

    {ESTLCommand::BE, "BE", ESTLCategory::Program_Control},
    {ESTLCommand::BEU, "BEU", ESTLCategory::Program_Control},
    {ESTLCommand::BEC, "BEC", ESTLCategory::Program_Control},
    {ESTLCommand::BLD, "BLD", ESTLCategory::Program_Control},
    {ESTLCommand::CALL, "CALL", ESTLCategory::Program_Control},
    {ESTLCommand::CC, "CC", ESTLCategory::Program_Control},
    {ESTLCommand::MCR_st, "MCR(", ESTLCategory::Program_Control},
    {ESTLCommand::MCR_en, ")MCR", ESTLCategory::Program_Control},
    {ESTLCommand::MCRA, "MCRA", ESTLCategory::Program_Control},
    {ESTLCommand::MCRD, "MCRD", ESTLCategory::Program_Control},
    {ESTLCommand::UC, "UC", ESTLCategory::Program_Control},

    {ESTLCommand::RLD, "RLD", ESTLCategory::Shift_Rotate},
    {ESTLCommand::RLDA, "RLDA", ESTLCategory::Shift_Rotate},
    {ESTLCommand::RRD, "RRD", ESTLCategory::Shift_Rotate},
    {ESTLCommand::RRDA, "RRDA", ESTLCategory::Shift_Rotate},
    {ESTLCommand::SLD, "SLD", ESTLCategory::Shift_Rotate},
    {ESTLCommand::SLW, "SLW", ESTLCategory::Shift_Rotate},
    {ESTLCommand::SRD, "SRD", ESTLCategory::Shift_Rotate},
    {ESTLCommand::SRW, "SRW", ESTLCategory::Shift_Rotate},
    {ESTLCommand::SSD, "SSD", ESTLCategory::Shift_Rotate},
    {ESTLCommand::SSI, "SSI", ESTLCategory::Shift_Rotate},

    {ESTLCommand::LC_T, "LC T", ESTLCategory::Timers},
    {ESTLCommand::R_T, "R_T", ESTLCategory::Timers},
    {ESTLCommand::SF, "SF", ESTLCategory::Timers},
    {ESTLCommand::SD, "SD", ESTLCategory::Timers},
    {ESTLCommand::SP, "SP", ESTLCategory::Timers},
    {ESTLCommand::SS, "SS", ESTLCategory::Timers},
    {ESTLCommand::SE, "SE", ESTLCategory::Timers},

    {ESTLCommand::LOOP, "LOOP", ESTLCategory::Jumps},
    {ESTLCommand::JU, "JU", ESTLCategory::Jumps},
    {ESTLCommand::JC, "JC", ESTLCategory::Jumps},
    {ESTLCommand::JN, "JN", ESTLCategory::Jumps},
    {ESTLCommand::JL, "JL", ESTLCategory::Jumps},
    {ESTLCommand::JM, "JM", ESTLCategory::Jumps},
    {ESTLCommand::JO, "JO", ESTLCategory::Jumps},
    {ESTLCommand::JP, "JP", ESTLCategory::Jumps},
    {ESTLCommand::JZ, "JZ", ESTLCategory::Jumps},
    {ESTLCommand::JCB, "JCB", ESTLCategory::Jumps},
    {ESTLCommand::JBI, "JBI", ESTLCategory::Jumps},
    {ESTLCommand::JCN, "JCN", ESTLCategory::Jumps},
    {ESTLCommand::JNB, "JNB", ESTLCategory::Jumps},
    {ESTLCommand::JMZ, "JMZ", ESTLCategory::Jumps},
    {ESTLCommand::JPZ, "JPZ", ESTLCategory::Jumps},
    {ESTLCommand::JOS, "JOS", ESTLCategory::Jumps},
    {ESTLCommand::JUO, "JUO", ESTLCategory::Jumps},
    {ESTLCommand::JNBI, "JNBI", ESTLCategory::Jumps},

    {ESTLCommand::OPN, "OPN", ESTLCategory::DB_Call},

    {ESTLCommand::ARG_START, "ARG_START", ESTLCategory::Program_Control},
    {ESTLCommand::ARG_END, "ARG_END", ESTLCategory::Program_Control},
    {ESTLCommand::NETWORK_END, "NETWORK_END", ESTLCategory::Program_Control},
    {ESTLCommand::RET, "RET", ESTLCategory::Program_Control},
};

STLCommands::STLCommands()
{
}

int32_t STLCommands::exec_command(STL_CMD_TYPE *prog_data)
{
#ifndef STLC_DEF
    ESTLCommand cmd = (ESTLCommand)*((STL_CMD_TYPE*)prog_data);
    int32_t size = sizeof(STL_CMD_TYPE);
	
		#ifdef PLC_DEBUG
        printf("Execute comand %s\n", STL_CMDs[(int)cmd].mnemonic);
		#endif

    size += sizeof(STL_CMD_TYPE);

    switch (cmd)
    {
    case ESTLCommand::NETWORK_END:
        return 0;

    /*
     * Bit Logic
     * */
    case ESTLCommand::Nesting_closed:
        {
            nested_t nested = plc_hw.nested_pop();
            switch(nested.cmd)
            {
            case ESTLCommand::A_net:
                plc_hw.set_reset_status_bits(PLC_STATUS_OR_MASK, nested.status);
                plc_hw.set_reset_status_bits(PLC_STATUS_RLO_MASK, nested.status & plc_hw.get_status_bit(PLC_STATUS_RLO_MASK));
                break;
            case ESTLCommand::AN_net:
                plc_hw.set_reset_status_bits(PLC_STATUS_OR_MASK, nested.status);
                plc_hw.set_reset_status_bits(PLC_STATUS_RLO_MASK, nested.status & (!plc_hw.get_status_bit(PLC_STATUS_RLO_MASK)));
                break;
            case ESTLCommand::O_net:
                //TODO
                break;
            case ESTLCommand::ON_net:
                //TODO
                break;
            case ESTLCommand::X_net:
                //TODO
                break;
            case ESTLCommand::XN_net:
                //TODO
                break;
            default:
                //TODO: exception
                break;
            }
            plc_hw.set_status_bits(PLC_STATUS_STA_MASK |
                                   PLC_STATUS_NOT_FC_MASK);
        }
        break;

    case ESTLCommand::Assign:   // (param) = RLO
        {
            parameter_t *param = (parameter_t*)&prog_data[size];
            if (plc_hw.get_MCR())
                param->write_bool(plc_hw.get_status_bit(PLC_STATUS_RLO_MASK));
            else
                param->write_bool(false);

            plc_hw.clear_status_bits(PLC_STATUS_OR_MASK |
                                     PLC_STATUS_NOT_FC_MASK);
            size += sizeof(parameter_t) + sizeof(STL_CMD_TYPE);
        }
        break;
    case ESTLCommand::CLR:      // RLO = 0
        plc_hw.clear_status_bits(PLC_STATUS_OR_MASK |
                                 PLC_STATUS_STA_MASK |
                                 PLC_STATUS_RLO_MASK |
                                 PLC_STATUS_NOT_FC_MASK);
        break;
    case ESTLCommand::FN:       // Negative edge
        {
            bool val = plc_hw.get_status_bit(PLC_STATUS_RLO_MASK);
            parameter_t *param = (parameter_t*)&prog_data[size];
            bool prev_val = param->read_bool();
            param->write_bool(val);
            val = ((prev_val) && (!val));

            plc_hw.set_reset_status_bits(PLC_STATUS_RLO_MASK | PLC_STATUS_NOT_FC_MASK | PLC_STATUS_OR_MASK,
                                         (val << PLC_STATUS_RLO_BIT) | PLC_STATUS_NOT_FC_MASK);
            size += sizeof(parameter_t) + sizeof(STL_CMD_TYPE);
        }
        break;
    case ESTLCommand::FP:       // Positive edge
        {
            bool val = plc_hw.get_status_bit(PLC_STATUS_RLO_MASK);
            parameter_t *param = (parameter_t*)&prog_data[size];
            bool prev_val = param->read_bool();
            param->write_bool(val);
            val = ((!prev_val) && (val));

            plc_hw.set_reset_status_bits(PLC_STATUS_RLO_MASK | PLC_STATUS_NOT_FC_MASK | PLC_STATUS_OR_MASK,
                                         (val << PLC_STATUS_RLO_BIT) | PLC_STATUS_NOT_FC_MASK);
            size += sizeof(parameter_t) + sizeof(STL_CMD_TYPE);
        }
        break;
    case ESTLCommand::NOT:      // STA = !STA
        {
            plc_hw.set_reset_status_bits(PLC_STATUS_RLO_MASK | PLC_STATUS_STA_MASK,
                                         plc_hw.get_status_bit(PLC_STATUS_RLO_MASK));
            size += sizeof(STL_CMD_TYPE);
        }
        break;
    case ESTLCommand::O:        // RLO || (param)
        {
            ESTLCommand arg_marker = (ESTLCommand)*((STL_CMD_TYPE*)&prog_data[size - sizeof(STL_CMD_TYPE)]);
            if (arg_marker == ESTLCommand::ARG_END)
            {
                plc_hw.set_status_bits(PLC_STATUS_STA_MASK);
                if (plc_hw.get_status_bit(PLC_STATUS_RLO_MASK))
                {
                    plc_hw.set_status_bits(PLC_STATUS_OR_MASK | PLC_STATUS_NOT_FC_MASK);
                }
                else
                {
                    plc_hw.clear_status_bits(PLC_STATUS_OR_MASK | PLC_STATUS_NOT_FC_MASK);
                }
                break;
            }

            parameter_t *param = (parameter_t*)&prog_data[size];
            bool val = param->read_bool();
            if (plc_hw.get_status_bit(PLC_STATUS_NOT_FC_MASK))
                // not first check, OR with RLO
                val = plc_hw.get_status_bit(PLC_STATUS_RLO_MASK) || val;
            plc_hw.set_reset_status_bits(PLC_STATUS_RLO_MASK | PLC_STATUS_OR_MASK | PLC_STATUS_NOT_FC_MASK,
                                         (val << PLC_STATUS_RLO_BIT) | PLC_STATUS_NOT_FC_MASK);
            size += sizeof(parameter_t) + sizeof(STL_CMD_TYPE);
        }
        break;
    case ESTLCommand::O_net:
        plc_hw.nested_push(ESTLCommand::O_net);
        plc_hw.set_reset_status_bits(PLC_STATUS_STA_MASK | PLC_STATUS_OR_MASK | PLC_STATUS_NOT_FC_MASK,
                                     PLC_STATUS_STA_MASK);
        break;
    case ESTLCommand::ON:       // RLO || (!param)
    {
        parameter_t *param = (parameter_t*)&prog_data[size];
        bool val = !param->read_bool();
        if (plc_hw.get_status_bit(PLC_STATUS_NOT_FC_MASK))
            // not first check, OR with RLO
            val = plc_hw.get_status_bit(PLC_STATUS_RLO_MASK) || val;
        plc_hw.set_reset_status_bits(PLC_STATUS_RLO_MASK | PLC_STATUS_NOT_FC_MASK | PLC_STATUS_OR_MASK,
                                     (val << PLC_STATUS_RLO_BIT) | PLC_STATUS_NOT_FC_MASK);
        size += sizeof(parameter_t) + sizeof(STL_CMD_TYPE);
    }
    break;
        break;
    case ESTLCommand::ON_net:
        plc_hw.nested_push(ESTLCommand::ON_net);
        plc_hw.set_reset_status_bits(PLC_STATUS_STA_MASK | PLC_STATUS_OR_MASK | PLC_STATUS_NOT_FC_MASK,
                                     PLC_STATUS_STA_MASK);
        break;
    case ESTLCommand::R:        // if (RLO) param=0
        if (plc_hw.get_status_bit(PLC_STATUS_RLO_MASK))
            ((parameter_t*)&prog_data[size])->write_bool(false);
        else plc_hw.set_reset_status_bits(PLC_STATUS_STA_MASK,
                                          ((parameter_t*)&prog_data[size])->read_bool() << PLC_STATUS_STA_BIT);

        plc_hw.clear_status_bits(PLC_STATUS_OR_MASK |
                                 PLC_STATUS_NOT_FC_MASK);
        size += sizeof(parameter_t) + sizeof(STL_CMD_TYPE);
        break;
    case ESTLCommand::S:        // if (RLO) param=1
        if (plc_hw.get_status_bit(PLC_STATUS_RLO_MASK))
            ((parameter_t*)&prog_data[size])->write_bool(true);
        else plc_hw.set_reset_status_bits(PLC_STATUS_STA_MASK,
                                          ((parameter_t*)&prog_data[size])->read_bool() << PLC_STATUS_STA_BIT);

        plc_hw.clear_status_bits(PLC_STATUS_OR_MASK |
                                 PLC_STATUS_NOT_FC_MASK);
        size += sizeof(parameter_t) + sizeof(STL_CMD_TYPE);
        break;
    case ESTLCommand::SAVE:     // BR = RLO
        if (plc_hw.get_status_bit(PLC_STATUS_RLO_MASK))
            plc_hw.set_status_bits(PLC_STATUS_BR_MASK);
        else
            plc_hw.clear_status_bits(PLC_STATUS_BR_MASK);
        break;
    case ESTLCommand::SET:      // RLO = 1
        plc_hw.set_reset_status_bits(PLC_STATUS_STA_MASK | PLC_STATUS_RLO_MASK | PLC_STATUS_OR_MASK | PLC_STATUS_NOT_FC_MASK,
                                     PLC_STATUS_STA_MASK | PLC_STATUS_RLO_MASK);
        break;
    case ESTLCommand::A:        // RLO && (param)
        {
            parameter_t *param = (parameter_t*)&prog_data[size];
            bool val = param->read_bool();
            if (plc_hw.get_status_bit(PLC_STATUS_NOT_FC_MASK))
                // not first check, AND with RLO
                val = plc_hw.get_status_bit(PLC_STATUS_RLO_MASK) && val;

            if (plc_hw.get_status_bit(PLC_STATUS_OR_MASK))
                plc_hw.set_reset_status_bits(PLC_STATUS_RLO_MASK | PLC_STATUS_NOT_FC_MASK,
                                             PLC_STATUS_RLO_MASK | PLC_STATUS_NOT_FC_MASK);
            else
                plc_hw.set_reset_status_bits(PLC_STATUS_RLO_MASK | PLC_STATUS_NOT_FC_MASK,
                                             (val << PLC_STATUS_RLO_BIT) | PLC_STATUS_NOT_FC_MASK);

            size += sizeof(parameter_t) + sizeof(STL_CMD_TYPE);
        }
        break;
    case ESTLCommand::A_net:
        plc_hw.nested_push(ESTLCommand::A_net);
        plc_hw.set_reset_status_bits(PLC_STATUS_STA_MASK | PLC_STATUS_OR_MASK | PLC_STATUS_NOT_FC_MASK,
                                     PLC_STATUS_STA_MASK);
        break;
    case ESTLCommand::AN:       // RLO && (!param)
        {
            parameter_t *param = (parameter_t*)&prog_data[size];
            bool val = !param->read_bool();
            if (plc_hw.get_status_bit(PLC_STATUS_NOT_FC_MASK))
                // not first check, AND with RLO
                val = plc_hw.get_status_bit(PLC_STATUS_RLO_MASK) && val;
            if (plc_hw.get_status_bit(PLC_STATUS_OR_MASK))
                plc_hw.set_status_bits(PLC_STATUS_RLO_MASK);

            if (plc_hw.get_status_bit(PLC_STATUS_OR_MASK))
                plc_hw.set_reset_status_bits(PLC_STATUS_RLO_MASK | PLC_STATUS_NOT_FC_MASK,
                                             PLC_STATUS_RLO_MASK | PLC_STATUS_NOT_FC_MASK);
            else
                plc_hw.set_reset_status_bits(PLC_STATUS_RLO_MASK | PLC_STATUS_NOT_FC_MASK,
                                             (val << PLC_STATUS_RLO_BIT) | PLC_STATUS_NOT_FC_MASK);
            size += sizeof(parameter_t) + sizeof(STL_CMD_TYPE);
        }
        break;
    case ESTLCommand::AN_net:
        plc_hw.nested_push(ESTLCommand::AN_net);
        plc_hw.set_reset_status_bits(PLC_STATUS_STA_MASK | PLC_STATUS_OR_MASK | PLC_STATUS_NOT_FC_MASK,
                                     PLC_STATUS_STA_MASK);
        break;
    case ESTLCommand::X:        // RLO ^ (param)
        {
            parameter_t *param = (parameter_t*)&prog_data[size];
            bool val = param->read_bool();
            if (plc_hw.get_status_bit(PLC_STATUS_NOT_FC_MASK))
                // not first check, XOR with RLO
                val = plc_hw.get_status_bit(PLC_STATUS_RLO_MASK) ^ val;
            plc_hw.set_reset_status_bits(PLC_STATUS_RLO_MASK | PLC_STATUS_NOT_FC_MASK | PLC_STATUS_OR_MASK,
                                         (val << PLC_STATUS_RLO_BIT) | PLC_STATUS_NOT_FC_MASK);
            size += sizeof(parameter_t) + sizeof(STL_CMD_TYPE);
        }
        break;
    case ESTLCommand::X_net:
        plc_hw.nested_push(ESTLCommand::X_net);
        plc_hw.set_reset_status_bits(PLC_STATUS_STA_MASK | PLC_STATUS_OR_MASK | PLC_STATUS_NOT_FC_MASK,
                                     PLC_STATUS_STA_MASK);
        break;
    case ESTLCommand::XN:       // RLO ^ (!param)
        {
            parameter_t *param = (parameter_t*)&prog_data[size];
            bool val = !param->read_bool();
            if (plc_hw.get_status_bit(PLC_STATUS_NOT_FC_MASK))
                // not first check, XOR with RLO
                val = plc_hw.get_status_bit(PLC_STATUS_RLO_MASK) ^ val;
            plc_hw.set_reset_status_bits(PLC_STATUS_RLO_MASK | PLC_STATUS_NOT_FC_MASK | PLC_STATUS_OR_MASK,
                                         (val << PLC_STATUS_RLO_BIT) | PLC_STATUS_NOT_FC_MASK);
            size += sizeof(parameter_t) + sizeof(STL_CMD_TYPE);
        }
        break;
    case ESTLCommand::XN_net:
        plc_hw.nested_push(ESTLCommand::XN_net);
        plc_hw.set_reset_status_bits(PLC_STATUS_STA_MASK | PLC_STATUS_OR_MASK | PLC_STATUS_NOT_FC_MASK,
                                     PLC_STATUS_STA_MASK);
        break;

    /*
     * Word Logic Instructions
     * */
    case ESTLCommand::AD:   // AND Double Word (32-Bit)
        break;
    case ESTLCommand::AW:   // AND Word (16-Bit)
        {
            uint16_t arg;
            if (NO_ARGS())
                arg = plc_hw.get_ACCU2()->get_lo();
            else
            {
                parameter_t *param = (parameter_t*)&prog_data[size];
                arg = param->read_word();
                size += sizeof(parameter_t) + sizeof(STL_CMD_TYPE);
            }

            arg &= plc_hw.get_ACCU1()->get_lo();
            plc_hw.get_ACCU1()->set_lo(arg);
            bool val = (arg != 0);
            plc_hw.set_reset_status_bits(PLC_STATUS_CC1_MASK | PLC_STATUS_CC0_MASK | PLC_STATUS_OV_MASK,
                                         (val << PLC_STATUS_CC1_BIT));
        }
        break;
    case ESTLCommand::OD:   // OR Double Word (32-Bit)
        {
            uint32_t arg;
            if (NO_ARGS())
                arg = plc_hw.get_ACCU2()->get();
            else
            {
                parameter_t *param = (parameter_t*)&prog_data[size];
                arg = param->read_dword();
                size += sizeof(parameter_t) + sizeof(STL_CMD_TYPE);
            }

            arg |= plc_hw.get_ACCU1()->get();
            plc_hw.get_ACCU1()->set(arg);
            bool val = (arg != 0);
            plc_hw.set_reset_status_bits(PLC_STATUS_CC1_MASK | PLC_STATUS_CC0_MASK | PLC_STATUS_OV_MASK,
                                         (val << PLC_STATUS_CC1_BIT));
        }
        break;
    case ESTLCommand::OW:   // OR Word (16-Bit)
        {
            uint16_t arg;
            if (NO_ARGS())
                arg = plc_hw.get_ACCU2()->get_lo();
            else
            {
                parameter_t *param = (parameter_t*)&prog_data[size];
                arg = param->read_word();
                size += sizeof(parameter_t) + sizeof(STL_CMD_TYPE);
            }

            arg |= plc_hw.get_ACCU1()->get_lo();
            plc_hw.get_ACCU1()->set_lo(arg);
            bool val = (arg != 0);
            plc_hw.set_reset_status_bits(PLC_STATUS_CC1_MASK | PLC_STATUS_CC0_MASK | PLC_STATUS_OV_MASK,
                                         (val << PLC_STATUS_CC1_BIT));
        }
        break;
    case ESTLCommand::XOD:  // Exclusive OR Double Word (32-Bit)
        {
            uint32_t arg;
            if (NO_ARGS())
                arg = plc_hw.get_ACCU2()->get();
            else
            {
                parameter_t *param = (parameter_t*)&prog_data[size];
                arg = param->read_dword();
                size += sizeof(parameter_t) + sizeof(STL_CMD_TYPE);
            }

            arg ^= plc_hw.get_ACCU1()->get();
            plc_hw.get_ACCU1()->set(arg);
            bool val = (arg != 0);
            plc_hw.set_reset_status_bits(PLC_STATUS_CC1_MASK | PLC_STATUS_CC0_MASK | PLC_STATUS_OV_MASK,
                                         (val << PLC_STATUS_CC1_BIT));
        }
        break;
    case ESTLCommand::XOW:  // Exclusive OR Word (16-Bit)
        {
            uint16_t arg;
            if (NO_ARGS())
                arg = plc_hw.get_ACCU2()->get_lo();
            else
            {
                parameter_t *param = (parameter_t*)&prog_data[size];
                arg = param->read_word();
                size += sizeof(parameter_t) + sizeof(STL_CMD_TYPE);
            }

            arg ^= plc_hw.get_ACCU1()->get_lo();
            plc_hw.get_ACCU1()->set_lo(arg);
            bool val = (arg != 0);
            plc_hw.set_reset_status_bits(PLC_STATUS_CC1_MASK | PLC_STATUS_CC0_MASK | PLC_STATUS_OV_MASK,
                                         (val << PLC_STATUS_CC1_BIT));
        }
        break;

    /*
     * Accumulator and Address Register Instructions
     * */
    case ESTLCommand::AR1_Add:  // Add ACCU 1 to Address Register 1
        {
            int32_t addr = plc_hw.get_AR1()->get();
            if (NO_ARGS())
                addr += plc_hw.get_ACCU1()->get_lo();
            else
            {
                //TODO: pointer operation
                parameter_t *param = (parameter_t*)&prog_data[size];
                addr += param->read_word();
                size += sizeof(parameter_t) + sizeof(STL_CMD_TYPE);
            }

            plc_hw.get_AR1()->set(addr);
        }
        break;
    case ESTLCommand::AR2_Add:  // Add ACCU 1 to Address Register 2
        {
            int32_t addr = plc_hw.get_AR2()->get();
            if (NO_ARGS())
                addr += plc_hw.get_ACCU1()->get_lo();
            else
            {
                //TODO: pointer operation
                parameter_t *param = (parameter_t*)&prog_data[size];
                addr += param->read_word();
                size += sizeof(parameter_t) + sizeof(STL_CMD_TYPE);
            }

            plc_hw.get_AR2()->set(addr);
        }
        break;
    case ESTLCommand::DEC:      // Decrement ACCU 1-L-L
        {
            parameter_t *param = (parameter_t*)&prog_data[size];
            plc_hw.get_ACCU1()->set_lolo(plc_hw.get_ACCU1()->get_lolo() - param->read_byte());
            size += sizeof(parameter_t) + sizeof(STL_CMD_TYPE);
        }
        break;
    case ESTLCommand::ENT:      // Enter ACCU Stack
        plc_hw.push_ACCU(0);
        break;
    case ESTLCommand::INC:      // Increment ACCU 1-L-L
        {
            parameter_t *param = (parameter_t*)&prog_data[size];
            plc_hw.get_ACCU1()->set_lolo(plc_hw.get_ACCU1()->get_lolo() + param->read_byte());
            size += sizeof(parameter_t) + sizeof(STL_CMD_TYPE);
        }
        break;
    case ESTLCommand::LEAVE:    //  Leave ACCU Stack
        plc_hw.get_ACCU1()->set(plc_hw.pop_ACCU());
        break;
    case ESTLCommand::NOP:
        break;
    case ESTLCommand::POP:
        plc_hw.pop_ACCU();
        break;
    case ESTLCommand::PUSH:
        plc_hw.push_ACCU(plc_hw.get_ACCU1()->get());
        break;
    case ESTLCommand::TAK:      // Toggle ACCU 1 with ACCU 2
        {
            int32_t val = plc_hw.get_ACCU1()->get();
            plc_hw.get_ACCU1()->set(plc_hw.get_ACCU2()->get());
            plc_hw.get_ACCU2()->set(val);
        }
        break;

    /*
     * Integer Math Instructions
     * */
    case ESTLCommand::Add:          // Add Integer Constant (16, 32-Bit)
        {
            parameter_t *param = (parameter_t*)&prog_data[size];
            if (param->get_size() == EDataSize::WORD)
                plc_hw.get_ACCU1()->set_lo(plc_hw.get_ACCU1()->get_lo() + param->read_word());
            else
                plc_hw.get_ACCU1()->set(plc_hw.get_ACCU1()->get() + param->read_dword());
            size += sizeof(parameter_t) + sizeof(STL_CMD_TYPE);
        }
        break;
    case ESTLCommand::AR12_D_Add:   // Add ACCU 1 and ACCU 2 as Double Integer (32-Bit)
        {
            int64_t res = plc_hw.pop_ACCU();
            res = plc_hw.get_ACCU1()->get() + res;
            plc_hw.get_ACCU1()->set(res);
            proceed_int32_op(res);
        }
        break;
    case ESTLCommand::AR12_D_Sub:   // Subtract ACCU 1 from ACCU 2 as Double Integer (32-Bit)
        {
            int64_t res = plc_hw.pop_ACCU();
            res = plc_hw.get_ACCU1()->get() - res;
            plc_hw.get_ACCU1()->set(res);
            proceed_int32_op(res);
        }
        break;
    case ESTLCommand::AR12_D_Mul:   // Multiply ACCU 1 and ACCU 2 as Double Integer (32-Bit)
        {
            int64_t res = plc_hw.pop_ACCU();
            res = plc_hw.get_ACCU1()->get() * res;
            plc_hw.get_ACCU1()->set(res);
            proceed_int32_op(res);
        }
        break;
    case ESTLCommand::AR12_D_Div:   // Divide ACCU 2 by ACCU 1 as Double Integer (32-Bit)
        {
            int64_t res = plc_hw.pop_ACCU();
            res = plc_hw.get_ACCU1()->get() / res;
            plc_hw.get_ACCU1()->set(res);
            proceed_int32_op(res);
        }
        break;
    case ESTLCommand::AR12_I_Add:   // Add ACCU 1 and ACCU 2 as Integer (16-Bit)
        {
            int32_t res = plc_hw.pop_ACCU() & 0xffff;
            res = plc_hw.get_ACCU1()->get_lo() + res;
            plc_hw.get_ACCU1()->set(res);
            proceed_int16_op(res);
        }
        break;
    case ESTLCommand::AR12_I_Sub:   // Subtract ACCU 1 from ACCU 2 as Integer (16-Bit)
        {
            int32_t res = plc_hw.pop_ACCU() & 0xffff;
            res = plc_hw.get_ACCU1()->get_lo() - res;
            plc_hw.get_ACCU1()->set(res);
            proceed_int16_op(res);
        }
        break;
    case ESTLCommand::AR12_I_Mul:   // Multiply ACCU 1 and ACCU 2 as Integer (16-Bit)
        {
            int32_t res = plc_hw.pop_ACCU() & 0xffff;
            res = plc_hw.get_ACCU1()->get_lo() * res;
            plc_hw.get_ACCU1()->set(res);
            proceed_int16_op(res);
        }
        break;
    case ESTLCommand::AR12_I_Div:   // Divide ACCU 2 by ACCU 1 as Integer (16-Bit)
        {
            int32_t res = plc_hw.pop_ACCU() & 0xffff;
            res = plc_hw.get_ACCU1()->get_lo() / res;
            plc_hw.get_ACCU1()->set(res);
            proceed_int16_op(res);
        }
        break;
    case ESTLCommand::MOD:          // Division Remainder Double Integer (32-Bit)
        {
            int64_t res = plc_hw.pop_ACCU();
            res = plc_hw.get_ACCU1()->get() % res;
            plc_hw.get_ACCU1()->set(res);
            proceed_int32_op(res);
        }
        break;

    /*
     * Floating-Point Math Instructions
     * */
    case ESTLCommand::AR12_R_Add:   // Add ACCU 1 and ACCU 2 as a Floating-Point Number (32-Bit IEEE 754)
        {
            int32_t accu1 = plc_hw.pop_ACCU();
            float res = plc_hw.get_ACCU1()->get_real() + *((float*)&accu1);
            proceed_real_op(res);
            plc_hw.get_ACCU1()->set_real(res);
        }
        break;
    case ESTLCommand::AR12_R_Sub:   // Subtract ACCU 1 from ACCU 2 as a Floating-Point Number (32-Bit IEEE 754)
        {
        int32_t accu1 = plc_hw.pop_ACCU();
        float res = plc_hw.get_ACCU1()->get_real() - *((float*)&accu1);
            proceed_real_op(res);
            plc_hw.get_ACCU1()->set_real(res);
        }
        break;
    case ESTLCommand::AR12_R_Mul:   // Multiply ACCU 1 and ACCU 2 as Floating-Point Numbers (32-Bit IEEE 754)
        {
        int32_t accu1 = plc_hw.pop_ACCU();
        float res = plc_hw.get_ACCU1()->get_real() * *((float*)&accu1);
            proceed_real_op(res);
            plc_hw.get_ACCU1()->set_real(res);
        }
        break;
    case ESTLCommand::AR12_R_Div:   // Divide ACCU 2 by ACCU 1 as a Floating-Point Number (32-Bit IEEE 754)
        {
        int32_t accu1 = plc_hw.pop_ACCU();
        float res = plc_hw.get_ACCU1()->get_real() / *((float*)&accu1);
            proceed_real_op(res);
            plc_hw.get_ACCU1()->set_real(res);
        }
        break;
    case ESTLCommand::ABS:          // Absolute Value of a Floating-Point Number (32-Bit IEEE 754)
        plc_hw.get_ACCU1()->set_real(std::abs(plc_hw.get_ACCU1()->get_real()));
        break;
    case ESTLCommand::ACOS:         // Generate the Arc Cosine of a Floating-Point Number (32-Bit)
        {
            float res = std::acos(plc_hw.get_ACCU1()->get_real());
            plc_hw.get_ACCU1()->set_real(res);
            proceed_real_op(res);
        }
        break;
    case ESTLCommand::ASIN:         // Generate the Arc Sine of a Floating-Point Number (32-Bit)
        {
            float res = std::asin(plc_hw.get_ACCU1()->get_real());
            plc_hw.get_ACCU1()->set_real(res);
            proceed_real_op(res);
        }
        break;
    case ESTLCommand::ATAN:         // Generate the Arc Tangent of a Floating-Point Number (32-Bit)
        {
            float res = std::atan(plc_hw.get_ACCU1()->get_real());
            plc_hw.get_ACCU1()->set_real(res);
            proceed_real_op(res);
        }
        break;
    case ESTLCommand::COS:          // Generate the Cosine of Angles as Floating-Point Numbers (32-Bit)
        {
            float res = std::cos(plc_hw.get_ACCU1()->get_real());
            plc_hw.get_ACCU1()->set_real(res);
            proceed_real_op(res);
        }
        break;
    case ESTLCommand::EXP:          // Generate the Exponential Value of a Floating-Point Number (32-Bit)
        {
            float res = std::exp(plc_hw.get_ACCU1()->get_real());
            plc_hw.get_ACCU1()->set_real(res);
            proceed_real_op(res);
        }
        break;
    case ESTLCommand::LN:           // Generate the Natural Logarithm of a Floating-Point Number (32-Bit)
        {
            float res = std::log(plc_hw.get_ACCU1()->get_real());
            plc_hw.get_ACCU1()->set_real(res);
            proceed_real_op(res);
        }
        break;
    case ESTLCommand::SIN:          // Generate the Sine of Angles as Floating-Point Numbers (32-Bit)
        {
            float res = std::sin(plc_hw.get_ACCU1()->get_real());
            plc_hw.get_ACCU1()->set_real(res);
            proceed_real_op(res);
        }
        break;
    case ESTLCommand::SQR:          // Generate the Square of a Floating-Point Number (32-Bit)
        {
            float res = std::pow(plc_hw.get_ACCU1()->get_real(), 2.f);
            plc_hw.get_ACCU1()->set_real(res);
            proceed_real_op(res);
        }
        break;
    case ESTLCommand::SQRT:         // Generate the Square Root of a Floating-Point Number (32-Bit)
        {
            float res = std::sqrt(plc_hw.get_ACCU1()->get_real());
            plc_hw.get_ACCU1()->set_real(res);
            proceed_real_op(res);
        }
        break;
    case ESTLCommand::TAN:          // Generate the Tangent of Angles as Floating-Point Numbers (32-Bit)
        {
            float res = std::tan(plc_hw.get_ACCU1()->get_real());
            plc_hw.get_ACCU1()->set_real(res);
            proceed_real_op(res);
        }
        break;

    /*
     * Comparasion
     * */
    case ESTLCommand::AR12_D_Eq:
    case ESTLCommand::AR12_D_Ne:
    case ESTLCommand::AR12_D_Gt:
    case ESTLCommand::AR12_D_Lt:
    case ESTLCommand::AR12_D_Ge:
    case ESTLCommand::AR12_D_Le:
        {
            int32_t accu1 = plc_hw.get_ACCU1()->get();
            int32_t accu2 = plc_hw.get_ACCU2()->get();
            if (accu1 == accu2)
                plc_hw.set_reset_status_bits(PLC_STATUS_CC0_MASK | PLC_STATUS_CC1_MASK,
                                             0);
            else if (accu1 > accu2)
                plc_hw.set_reset_status_bits(PLC_STATUS_CC0_MASK | PLC_STATUS_CC1_MASK,
                                             PLC_STATUS_CC0_MASK);
            else if (accu1 < accu2)
                plc_hw.set_reset_status_bits(PLC_STATUS_CC0_MASK | PLC_STATUS_CC1_MASK,
                                             PLC_STATUS_CC1_MASK);
            else
                plc_hw.set_reset_status_bits(PLC_STATUS_CC0_MASK | PLC_STATUS_CC1_MASK,
                                             PLC_STATUS_CC0_MASK | PLC_STATUS_CC1_MASK);

            bool rlo = false;
            switch (cmd)
            {
            case ESTLCommand::AR12_D_Eq:
                rlo = (accu1 == accu2);
                break;
            case ESTLCommand::AR12_D_Ne:
                rlo = (accu1 != accu2);
                break;
            case ESTLCommand::AR12_D_Gt:
                rlo = (accu1 < accu2);
                break;
            case ESTLCommand::AR12_D_Lt:
                rlo = (accu1 > accu2);
                break;
            case ESTLCommand::AR12_D_Ge:
                rlo = (accu1 <= accu2);
                break;
            case ESTLCommand::AR12_D_Le:
                rlo = (accu1 >= accu2);
                break;
            default:
                break;
            }

            plc_hw.set_reset_status_bits(PLC_STATUS_RLO_MASK | PLC_STATUS_OV_MASK | PLC_STATUS_OR_MASK | PLC_STATUS_NOT_FC_MASK,
                                         (rlo << PLC_STATUS_OV_BIT) | PLC_STATUS_NOT_FC_MASK);
        }
        break;
    case ESTLCommand::AR12_I_Eq:
    case ESTLCommand::AR12_I_Ne:
    case ESTLCommand::AR12_I_Gt:
    case ESTLCommand::AR12_I_Lt:
    case ESTLCommand::AR12_I_Ge:
    case ESTLCommand::AR12_I_Le:
        {
            int16_t accu1 = plc_hw.get_ACCU1()->get_lo();
            int16_t accu2 = plc_hw.get_ACCU2()->get_lo();
            if (accu1 == accu2)
                plc_hw.set_reset_status_bits(PLC_STATUS_CC0_MASK | PLC_STATUS_CC1_MASK,
                                             0);
            else if (accu1 > accu2)
                plc_hw.set_reset_status_bits(PLC_STATUS_CC0_MASK | PLC_STATUS_CC1_MASK,
                                             PLC_STATUS_CC0_MASK);
            else if (accu1 < accu2)
                plc_hw.set_reset_status_bits(PLC_STATUS_CC0_MASK | PLC_STATUS_CC1_MASK,
                                             PLC_STATUS_CC1_MASK);
            else
                plc_hw.set_reset_status_bits(PLC_STATUS_CC0_MASK | PLC_STATUS_CC1_MASK,
                                             PLC_STATUS_CC0_MASK | PLC_STATUS_CC1_MASK);

            bool rlo = false;
            switch (cmd)
            {
            case ESTLCommand::AR12_I_Eq:
                rlo = (accu1 == accu2);
                break;
            case ESTLCommand::AR12_I_Ne:
                rlo = (accu1 != accu2);
                break;
            case ESTLCommand::AR12_I_Gt:
                rlo = (accu1 < accu2);
                break;
            case ESTLCommand::AR12_I_Lt:
                rlo = (accu1 > accu2);
                break;
            case ESTLCommand::AR12_I_Ge:
                rlo = (accu1 <= accu2);
                break;
            case ESTLCommand::AR12_I_Le:
                rlo = (accu1 >= accu2);
                break;
            default:
                break;
            }

            plc_hw.set_reset_status_bits(PLC_STATUS_RLO_MASK | PLC_STATUS_OV_MASK | PLC_STATUS_OR_MASK | PLC_STATUS_NOT_FC_MASK,
                                         (rlo << PLC_STATUS_OV_BIT) | PLC_STATUS_NOT_FC_MASK);
        }
        break;
    case ESTLCommand::AR12_R_Eq:
    case ESTLCommand::AR12_R_Ne:
    case ESTLCommand::AR12_R_Gt:
    case ESTLCommand::AR12_R_Lt:
    case ESTLCommand::AR12_R_Ge:
    case ESTLCommand::AR12_R_Le:
        {
            float accu1 = (float)plc_hw.get_ACCU1()->get();
            float accu2 = (float)plc_hw.get_ACCU2()->get();
            if (accu1 == accu2)
                plc_hw.set_reset_status_bits(PLC_STATUS_CC0_MASK | PLC_STATUS_CC1_MASK,
                                             0);
            else if (accu1 > accu2)
                plc_hw.set_reset_status_bits(PLC_STATUS_CC0_MASK | PLC_STATUS_CC1_MASK,
                                             PLC_STATUS_CC0_MASK);
            else if (accu1 < accu2)
                plc_hw.set_reset_status_bits(PLC_STATUS_CC0_MASK | PLC_STATUS_CC1_MASK,
                                             PLC_STATUS_CC1_MASK);
            else
                plc_hw.set_reset_status_bits(PLC_STATUS_CC0_MASK | PLC_STATUS_CC1_MASK,
                                             PLC_STATUS_CC0_MASK | PLC_STATUS_CC1_MASK);

            bool rlo = false;
            switch (cmd)
            {
            case ESTLCommand::AR12_R_Eq:
                rlo = (accu1 == accu2);
                break;
            case ESTLCommand::AR12_R_Ne:
                rlo = (accu1 != accu2);
                break;
            case ESTLCommand::AR12_R_Gt:
                rlo = (accu1 < accu2);
                break;
            case ESTLCommand::AR12_R_Lt:
                rlo = (accu1 > accu2);
                break;
            case ESTLCommand::AR12_R_Ge:
                rlo = (accu1 <= accu2);
                break;
            case ESTLCommand::AR12_R_Le:
                rlo = (accu1 >= accu2);
                break;
            default:
                break;
            }

            //TODO: OV, OS
            plc_hw.set_reset_status_bits(PLC_STATUS_RLO_MASK | PLC_STATUS_OV_MASK | PLC_STATUS_OR_MASK | PLC_STATUS_NOT_FC_MASK,
                                         (rlo << PLC_STATUS_OV_BIT) | PLC_STATUS_NOT_FC_MASK);
        }
        break;

    /*
     * Conversion Instructions
     * */
    case ESTLCommand::BTD:      // BCD to Integer (32-Bit)
        ///plc_hw.get_ACCU1()->set(PLCUtils::bcd_to_int32(plc_hw.get_ACCU1()->get()));
        break;
    case ESTLCommand::BTI:      // BCD to Integer (16-Bit)
        ///plc_hw.get_ACCU1()->set_lo(PLCUtils::bcd_to_int16(plc_hw.get_ACCU1()->get()));
        break;
    case ESTLCommand::DTB:      // Double Integer (32-Bit) to BCD
        ///plc_hw.get_ACCU1()->set(PLCUtils::int32_to_bcd(plc_hw.get_ACCU1()->get()));
        break;
    case ESTLCommand::DTR:      // Double Integer (32-Bit) to Floating-Point (32-Bit IEEE 754)
        plc_hw.get_ACCU1()->set_real(1.f * plc_hw.get_ACCU1()->get());
        break;
    case ESTLCommand::INVD:     // Ones Complement Double Integer (32-Bit)
        plc_hw.get_ACCU1()->set(~plc_hw.get_ACCU1()->get());
        break;
    case ESTLCommand::INVI:     // Ones Complement Integer (16-Bit)
        plc_hw.get_ACCU1()->set_lo(~plc_hw.get_ACCU1()->get_lo());
        break;
    case ESTLCommand::ITB:      // Integer (16-Bit) to BCD
        //plc_hw.get_ACCU1()->set_lo(PLCUtils::int16_to_bcd(plc_hw.get_ACCU1()->get_lo()));
        break;
    case ESTLCommand::ITD:      // Integer (16 Bit) to Double Integer (32-Bit)
        plc_hw.get_ACCU1()->set(plc_hw.get_ACCU1()->get_lo());
        break;
    case ESTLCommand::NEGD:     // Twos Complement Double Integer (32-Bit)
        plc_hw.get_ACCU1()->set(-1 * plc_hw.get_ACCU1()->get());
        break;
    case ESTLCommand::NEGI:     // Twos Complement Integer (16-Bit)
        plc_hw.get_ACCU1()->set_lo(-1 * plc_hw.get_ACCU1()->get_lo());
        break;
    case ESTLCommand::NEGR:     // Negate Floating-Point Number (32-Bit, IEEE 754)
        plc_hw.get_ACCU1()->set_real(-1.f * plc_hw.get_ACCU1()->get_real());
        break;
    case ESTLCommand::RND:      // Round
        plc_hw.get_ACCU1()->set(std::round(plc_hw.get_ACCU1()->get_real()));
        break;
    case ESTLCommand::RND_Add:  // Round to Upper Double Integer
        plc_hw.get_ACCU1()->set(std::ceil(plc_hw.get_ACCU1()->get_real()));
        break;
    case ESTLCommand::RND_Sub:  // Round to Lower Double Integer
        plc_hw.get_ACCU1()->set(std::floor(plc_hw.get_ACCU1()->get_real()));
        break;
    case ESTLCommand::CAD:      // Change Byte Sequence in ACCU 1 (32-Bit)
        plc_hw.get_ACCU1()->set(BO_DWORD(plc_hw.get_ACCU1()->get()));
        break;
    case ESTLCommand::CAW:      // Change Byte Sequence in ACCU 1-L (16-Bit)
        plc_hw.get_ACCU1()->set_lo(BO_WORD(plc_hw.get_ACCU1()->get_lo()));
        break;
    case ESTLCommand::CDB:      // Exchange Shared DB and Instance DB
        //TODO
        break;
    case ESTLCommand::TRUNC:    // Truncate
        plc_hw.get_ACCU1()->set(std::trunc(plc_hw.get_ACCU1()->get_real()));
        break;

    /*
     * Load and Transfer Instructions
     * */
    case ESTLCommand::L:
        {
            parameter_t *param = (parameter_t*)&prog_data[size];
            plc_hw.push_ACCU(param);
            size += sizeof(parameter_t) + sizeof(STL_CMD_TYPE);
        }
        break;
    case ESTLCommand::L_T:
        //TODO
        break;
    case ESTLCommand::L_C:
        //TODO
        break;
    case ESTLCommand::L_DBLG:   // Load Length of Shared DB in ACCU 1
        //TODO
        break;
    case ESTLCommand::L_DBNO:   // Load Number of Shared DB in ACCU 1
        //TODO
        break;
    case ESTLCommand::L_DILG:   // Load Length of Instance DB in ACCU 1
        plc_hw.push_ACCU(0);
        plc_hw.get_ACCU1()->set_lo(plc_fw.get_cur_db_info()->size_bytes);
        break;
    case ESTLCommand::L_DINO:   // Load Number of Instance DB in ACCU 1
        //TODO
        break;
    case ESTLCommand::L_STW:    // Load Status Word into ACCU 1
        {
            uint32_t val = plc_hw.get_status_word()
            #ifdef PLC_STW_S7_300
                    & ~(PLC_STATUS_NOT_FC_MASK |
                        PLC_STATUS_STA_MASK |
                        PLC_STATUS_OR_MASK);
            #else
                    ;
            #endif
            plc_hw.get_ACCU1()->set(val);
        }
        break;
    case ESTLCommand::LAR1:     // Load Address Register 1
        {
            if (NO_ARGS())
                plc_hw.get_AR1()->set(plc_hw.get_ACCU1()->get());
            else
            {
                parameter_t *param = (parameter_t*)&prog_data[size];
                plc_hw.get_AR1()->set(param->read_dword());
                size += sizeof(parameter_t) + sizeof(STL_CMD_TYPE);
            }
        }
        break;
    case ESTLCommand::LAR1_AR2: // Load Address Register 1 from Address Register 2
        plc_hw.get_AR1()->set(plc_hw.get_AR2()->get());
        break;
    case ESTLCommand::LAR2:     // Load Address Register 2
        {
            if (NO_ARGS())
                plc_hw.get_AR2()->set(plc_hw.get_ACCU1()->get());
            else
            {
                parameter_t *param = (parameter_t*)&prog_data[size];
                plc_hw.get_AR2()->set(param->read_dword());
                size += sizeof(parameter_t) + sizeof(STL_CMD_TYPE);
            }
        }
        break;
    case ESTLCommand::T:
        {
            parameter_t *param = (parameter_t*)&prog_data[size];
            plc_hw.get_ACCU1()->load_to_param(param);
            size += sizeof(parameter_t) + sizeof(STL_CMD_TYPE);
        }
        break;
    case ESTLCommand::T_STW:    // Transfer ACCU 1 into Status Word
        {
            uint32_t val = plc_hw.get_status_word();
            #ifdef PLC_STW_S7_300
            uint32_t mask = PLC_STATUS_RLO_MASK |
                            PLC_STATUS_OS_MASK |
                            PLC_STATUS_OV_MASK |
                            PLC_STATUS_CC0_MASK |
                            PLC_STATUS_CC1_MASK |
                            PLC_STATUS_BR_MASK;
            #else
            uint32_t mask = PLC_STATUS_NOT_FC_MASK |
                            PLC_STATUS_RLO_MASK |
                            PLC_STATUS_STA_MASK |
                            PLC_STATUS_OR_MASK |
                            PLC_STATUS_OS_MASK |
                            PLC_STATUS_OV_MASK |
                            PLC_STATUS_CC0_MASK |
                            PLC_STATUS_CC1_MASK |
                            PLC_STATUS_BR_MASK;
            #endif
            plc_hw.set_reset_status_bits(mask, val);
        }
        break;
    case ESTLCommand::CAR:      // Exchange Address Register 1 with Address Register 2
        {
            int32_t tmp = plc_hw.get_AR1()->get();
            plc_hw.get_AR1()->set(plc_hw.get_AR2()->get());
            plc_hw.get_AR2()->set(tmp);
        }
        break;
    case ESTLCommand::TAR1:     // Transfer Address Register 1
        {
            if (NO_ARGS())
                plc_hw.push_ACCU(plc_hw.get_AR1()->get());
            else
            {
                parameter_t *param = (parameter_t*)&prog_data[size];
                param->write_dword(plc_hw.get_AR1()->get());
                size += sizeof(parameter_t) + sizeof(STL_CMD_TYPE);
            }
        }
        break;
    case ESTLCommand::TAR1_AR2: // Transfer Address Register 1 to Address Register 2
        plc_hw.get_AR2()->set(plc_hw.get_AR1()->get());
        break;
    case ESTLCommand::TAR2:     // Transfer Address Register 2
        {
            if (NO_ARGS())
                plc_hw.push_ACCU(plc_hw.get_AR2()->get());
            else
            {
                parameter_t *param = (parameter_t*)&prog_data[size];
                param->write_dword(plc_hw.get_AR2()->get());
                size += sizeof(parameter_t) + sizeof(STL_CMD_TYPE);
            }
        }
        break;


    case ESTLCommand::BLD:
        size += sizeof(parameter_t) + sizeof(STL_CMD_TYPE);
        break;
    case ESTLCommand::CALL:
        {
            parameter_t *fb_addr = (parameter_t*)&prog_data[size];
            size += sizeof(parameter_t);
            parameter_t *db_addr = (parameter_t*)&prog_data[size];
            size += sizeof(parameter_t);
            parameter_t *call_args_count = (parameter_t*)&prog_data[size];
            size += sizeof(parameter_t);

            //plc_fw.set_current_db(db_addr->data.DWORD);
            plc_data_t locals_addr[PLC_LOCAL_SIZE];
            parameter_t* params[PLC_LOCAL_SIZE];
            int32_t local_bit = -1;
            int32_t local_addr = 0;

            PLCDataInterface *io_area;
            uint32_t local_top = plc_hw.get_local_top();
            switch (fb_addr->location)
            {
            case EParameterLocation::FB:
                io_area = plc_hw.get_dbs_obj();
                local_addr = 0;
                break;
            case EParameterLocation::FC:
                io_area = plc_hw.get_local_obj();
                local_addr = plc_hw.get_local_top();
                break;
            case EParameterLocation::SFB:
                io_area = plc_hw.get_dbs_obj();      //TODO
                local_addr = 0;
                break;
            case EParameterLocation::SFC:
                io_area = plc_hw.get_local_obj();    //TODO
                local_addr = plc_hw.get_local_top();
                local_addr = 0;
                break;
            default:
                while (1) {} //TODO
                break;
            }

            int32_t db_offset = plc_fw.get_db_offset(db_addr->data.DWORD);
            EParameterLocation loc = ((parameter_t*)&prog_data[size])->location;
            for (int32_t i=0 ; i<call_args_count->data.DWORD ; ++i)
            {
                parameter_t *param = (parameter_t*)&prog_data[size];
                params[i] = param;
                size += sizeof(parameter_t);
                if (loc != param->location)
                {
                    if (local_bit != 0xff)
                        ++local_addr;
                    local_bit = -1;
                    loc = param->location;
                }
                param->data.ADDR_DB.db_idx = db_addr->data.DWORD;

                switch (param->get_size())
                {
                case EDataSize::BIT:
                    {
                        if (local_bit == 7)
                        {
                            local_bit = -1;
                            ++local_addr;
                        }
                        bool val = param->read_bool();
                        locals_addr[i].BOOL.addr = db_offset + local_addr;
                        locals_addr[i].BOOL.bit = ++local_bit;
                        io_area->set_bit(locals_addr[i].BOOL.addr,
                                    locals_addr[i].BOOL.bit,
                                    val);
                    }
                    break;
                case EDataSize::BYTE:
                    if (local_bit != -1)
                    {
                        local_bit = -1;
                        ++local_addr;
                    }
                    locals_addr[i].ADDR = local_addr;
                    io_area->write_byte(db_offset + local_addr, param->read_byte());
                    ++local_addr;
                    break;
                case EDataSize::WORD:
                    if (local_bit != -1)
                    {
                        local_bit = -1;
                        ++local_addr;
                    }
                    locals_addr[i].ADDR = local_addr;
                    io_area->write_word(db_offset + local_addr, param->read_word());
                    local_addr += 2;
                    break;
                case EDataSize::DWORD:
                    if (local_bit != -1)
                    {
                        local_bit = -1;
                        ++local_addr;
                    }
                    locals_addr[i].ADDR = local_addr;
                    io_area->write_dword(db_offset + local_addr, param->read_dword());
                    local_addr += 4;
                    break;
                default:
                    break;
                }
            }
            size += sizeof(STL_CMD_TYPE);
            plc_hw.set_local_top(local_top);

            plc_hw.set_reset_status_bits(PLC_STATUS_OS_MASK | PLC_STATUS_OR_MASK | PLC_STATUS_STA_MASK | PLC_STATUS_NOT_FC_MASK,
                                         PLC_STATUS_STA_MASK);

            plc_fw.exec_block(fb_addr->data.DWORD, db_addr->data.DWORD);

            plc_hw.set_reset_status_bits(PLC_STATUS_OS_MASK | PLC_STATUS_OR_MASK | PLC_STATUS_STA_MASK | PLC_STATUS_NOT_FC_MASK,
                                         PLC_STATUS_STA_MASK);

            for (int32_t i=0 ; i<call_args_count->data.DWORD ; ++i)
            {
                parameter_t *param = params[i];
                //if (param->location != EParameterLocation::BLOCK_OUTPUT)
                //    continue;

                switch (param->get_size())
                {
                case EDataSize::BIT:
                    param->write_bool(io_area->read_bit(locals_addr[i].BOOL.addr,
                                                   locals_addr[i].BOOL.bit));
                    break;
                case EDataSize::BYTE:
                    param->write_byte(io_area->read_byte(locals_addr[i].ADDR));
                    break;
                case EDataSize::WORD:
                    param->write_word(io_area->read_word(locals_addr[i].ADDR));
                    break;
                case EDataSize::DWORD:
                    param->write_dword(io_area->read_dword(locals_addr[i].ADDR));
                    break;
                default:
                    break;
                }
            }
            plc_hw.set_local_top(local_top);
        }
        break;


    // Logic control instructions
    /*case ESTLCommand::LOOP:
        //TODO
        break;
    case ESTLCommand::JU:
        //TODO
        break;
    case ESTLCommand::JC:
        //TODO
        break;
    case ESTLCommand::JCB:
        //TODO
        break;
    case ESTLCommand::JBI:
        //TODO
        break;
    case ESTLCommand::JNBI:
        //TODO
        break;
    case ESTLCommand::JCN:
        //TODO
        break;*/
    case ESTLCommand::JNB:
        {
            parameter_t *param = (parameter_t*)&prog_data[size];
            bool rlo = plc_hw.get_status_bit(PLC_STATUS_RLO_MASK);
            if (!rlo)
                size = (-1) * (int16_t)param->data.addr;
            else
                size += sizeof(parameter_t) + sizeof(STL_CMD_TYPE);

            plc_hw.set_reset_status_bits(PLC_STATUS_OR_MASK | PLC_STATUS_STA_MASK | PLC_STATUS_RLO_MASK | PLC_STATUS_NOT_FC_MASK,
                                         PLC_STATUS_STA_MASK | PLC_STATUS_RLO_MASK);
        }
        break;
    /*case ESTLCommand::JL:
        //TODO
        break;
    case ESTLCommand::JM:
        //TODO
        break;
    case ESTLCommand::JMZ:
        //TODO
        break;
    case ESTLCommand::JN:
        //TODO
        break;
    case ESTLCommand::JO:
        //TODO
        break;
    case ESTLCommand::JP:
        //TODO
        break;
    case ESTLCommand::JPZ:
        //TODO
        break;
    case ESTLCommand::JOS:
        //TODO
        break;
    case ESTLCommand::JUO:
        //TODO
        break;
    case ESTLCommand::JZ:
        //TODO
        break;*/

    default:
        {
            ESTLCommand* mark = (ESTLCommand*)&prog_data[size];
            while (*mark != ESTLCommand::ARG_END)
            {
                size += sizeof(STL_CMD_TYPE);
                mark += sizeof(STL_CMD_TYPE);
            }
        }
        break;
    /*

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

    FR,
    LC_C,
    R_C,
    S_C,
    CD,
    CU,

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

    OPN,

    ARG_START,
    ARG_END,
    */
    }

    return size;
#else
    (void)(prog_data);
    return 0;
#endif
}

void STLCommands::proceed_real_op(float res)
{
#ifndef STLC_DEF
    if (std::isnan(res))
        plc_hw.set_reset_status_bits(PLC_STATUS_CC1_MASK | PLC_STATUS_CC0_MASK | PLC_STATUS_OV_MASK | PLC_STATUS_OS_MASK,
                                     PLC_STATUS_CC1_MASK | PLC_STATUS_CC0_MASK | PLC_STATUS_OV_MASK | PLC_STATUS_OS_MASK);
    else if (std::fpclassify(res) == FP_SUBNORMAL)
        plc_hw.set_reset_status_bits(PLC_STATUS_CC1_MASK | PLC_STATUS_CC0_MASK | PLC_STATUS_OV_MASK | PLC_STATUS_OS_MASK,
                                     PLC_STATUS_OV_MASK | PLC_STATUS_OS_MASK);
    else if (std::fpclassify(res) == FP_ZERO)
        plc_hw.set_reset_status_bits(PLC_STATUS_CC1_MASK | PLC_STATUS_CC0_MASK | PLC_STATUS_OV_MASK,
                                     0);
    else if (std::fpclassify(res) == FP_NORMAL)
    {
        if (std::signbit(res))
            plc_hw.set_reset_status_bits(PLC_STATUS_CC1_MASK | PLC_STATUS_CC0_MASK | PLC_STATUS_OV_MASK,
                                         PLC_STATUS_CC0_MASK);
        else
            plc_hw.set_reset_status_bits(PLC_STATUS_CC1_MASK | PLC_STATUS_CC0_MASK | PLC_STATUS_OV_MASK,
                                         PLC_STATUS_CC1_MASK);
    }
    else if (std::fpclassify(res) == FP_INFINITE)
    {
        if (std::signbit(res))
            plc_hw.set_reset_status_bits(PLC_STATUS_CC1_MASK | PLC_STATUS_CC0_MASK | PLC_STATUS_OV_MASK | PLC_STATUS_OS_MASK,
                                         PLC_STATUS_CC0_MASK | PLC_STATUS_OV_MASK | PLC_STATUS_OS_MASK);
        else
            plc_hw.set_reset_status_bits(PLC_STATUS_CC1_MASK | PLC_STATUS_CC0_MASK | PLC_STATUS_OV_MASK | PLC_STATUS_OS_MASK,
                                         PLC_STATUS_CC1_MASK | PLC_STATUS_OV_MASK | PLC_STATUS_OS_MASK);
    }
#else
    (void)(res);
#endif
}

void STLCommands::proceed_int32_op(int64_t res)
{
#ifndef STLC_DEF
    if (res == 0)
        plc_hw.set_reset_status_bits(PLC_STATUS_CC1_MASK | PLC_STATUS_CC0_MASK | PLC_STATUS_OV_MASK,
                                     0);
    else if ((INT32_MIN <= res) && (res < 0))
        plc_hw.set_reset_status_bits(PLC_STATUS_CC1_MASK | PLC_STATUS_CC0_MASK | PLC_STATUS_OV_MASK,
                                     PLC_STATUS_CC0_MASK);
    else if ((INT32_MAX >= res) && (res > 0))
        plc_hw.set_reset_status_bits(PLC_STATUS_CC1_MASK | PLC_STATUS_CC0_MASK | PLC_STATUS_OV_MASK,
                                     PLC_STATUS_CC1_MASK);
    else if (res < -4294967296)
        plc_hw.set_reset_status_bits(PLC_STATUS_CC1_MASK | PLC_STATUS_CC0_MASK | PLC_STATUS_OV_MASK | PLC_STATUS_OS_MASK,
                                     PLC_STATUS_OV_MASK | PLC_STATUS_OS_MASK);
    else if (res > INT32_MAX)
        plc_hw.set_reset_status_bits(PLC_STATUS_CC1_MASK | PLC_STATUS_CC0_MASK | PLC_STATUS_OV_MASK | PLC_STATUS_OS_MASK,
                                     PLC_STATUS_CC0_MASK | PLC_STATUS_OV_MASK | PLC_STATUS_OS_MASK);
    else if (res < INT32_MIN)
        plc_hw.set_reset_status_bits(PLC_STATUS_CC1_MASK | PLC_STATUS_CC0_MASK | PLC_STATUS_OV_MASK | PLC_STATUS_OS_MASK,
                                     PLC_STATUS_CC1_MASK | PLC_STATUS_OV_MASK | PLC_STATUS_OS_MASK);
#else
    (void)(res);
#endif
}

void STLCommands::proceed_int16_op(int32_t res)
{
#ifndef STLC_DEF
    if (res == 0)
        plc_hw.set_reset_status_bits(PLC_STATUS_CC1_MASK | PLC_STATUS_CC0_MASK | PLC_STATUS_OV_MASK,
                                     0);
    else if ((INT16_MIN <= res) && (res < 0))
        plc_hw.set_reset_status_bits(PLC_STATUS_CC1_MASK | PLC_STATUS_CC0_MASK | PLC_STATUS_OV_MASK,
                                     PLC_STATUS_CC0_MASK);
    else if ((INT16_MAX >= res) && (res > 0))
        plc_hw.set_reset_status_bits(PLC_STATUS_CC1_MASK | PLC_STATUS_CC0_MASK | PLC_STATUS_OV_MASK,
                                     PLC_STATUS_CC1_MASK);
    else if (res < -65536)
        plc_hw.set_reset_status_bits(PLC_STATUS_CC1_MASK | PLC_STATUS_CC0_MASK | PLC_STATUS_OV_MASK | PLC_STATUS_OS_MASK,
                                     PLC_STATUS_OV_MASK | PLC_STATUS_OS_MASK);
    else if (res > INT16_MAX)
        plc_hw.set_reset_status_bits(PLC_STATUS_CC1_MASK | PLC_STATUS_CC0_MASK | PLC_STATUS_OV_MASK | PLC_STATUS_OS_MASK,
                                     PLC_STATUS_CC0_MASK | PLC_STATUS_OV_MASK | PLC_STATUS_OS_MASK);
    else if (res < INT16_MIN)
        plc_hw.set_reset_status_bits(PLC_STATUS_CC1_MASK | PLC_STATUS_CC0_MASK | PLC_STATUS_OV_MASK | PLC_STATUS_OS_MASK,
                                     PLC_STATUS_CC1_MASK | PLC_STATUS_OV_MASK | PLC_STATUS_OS_MASK);
#else
    (void)(res);
#endif
}
