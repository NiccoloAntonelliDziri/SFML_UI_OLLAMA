#include "app.hpp"

#include "constants.hpp"

#include <typeinfo>

int main() {

    App app(cst.get<int>("screenInitialWidth"),
            cst.get<int>("screenInitialHeight"), cst["titlebar"]);

    return 0;
}
