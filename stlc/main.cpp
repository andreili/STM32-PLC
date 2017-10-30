#include "CmdlineParser.h"
#include "stlc.h"

int main(int argc, char *argv[])
{
    bool mode_compile, mode_decompile, show_help, iec;
    std::string in_file, out_file;

    CmdlineParser* cmd = new CmdlineParser(argc, argv);
    cmd->set_name("ST(L) Compilator/Decompilator");
    cmd->add_bool_param("iec", &iec, false, "Compatible with IEC61131 ST (otherwise - Siemens STL)");
    cmd->add_bool_param("compile", &mode_compile, false, "Compile mode");
    cmd->add_bool_param("decompile", &mode_decompile, false, "Decompile mode");
    cmd->add_bool_param("help", &show_help, false, "Show help and exit");
    cmd->add_string_param("in", &in_file, "", "Input file");
    cmd->add_string_param("out", &out_file, "", "Output file");
    cmd->parse();

    if (show_help)
    {
        cmd->show_desc();
        return 0;
    }

    STLC stlc;

    if (mode_compile && (in_file.length()) && (out_file.length()))
    {
        if (stlc.load_plain(in_file, iec) && stlc.compile(out_file))
            printf("Compilation finished: OK.\n");
        else
            printf("Compilation finished: FAILED!\n");
    }
    else
        cmd->show_desc();

    return 0;
}
