#include "ccc.hpp"
#include "ccc_gui.hpp"
#include "ccc_utils.hpp"

int main(int argc, char *argv[])
{
    bool is_gui_mode = false;
    ccc::CCConfig config = parse_args(argc, argv, &is_gui_mode);

    if (is_gui_mode)
    {
        return execute_gui();
    }
    else
    {
        ccc::CaesarCipher caesar_cipher{&config};
        std::cout << caesar_cipher.get_caesar_text() << std::endl;
    }

    return 0;
}
