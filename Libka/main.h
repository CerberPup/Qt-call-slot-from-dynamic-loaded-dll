#include <string>

extern "C" {
    void list_widgets();
    void call_slot(std::string _name, std::string _slot);
}