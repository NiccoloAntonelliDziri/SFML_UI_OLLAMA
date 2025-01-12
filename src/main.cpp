#include "app.hpp"

#include "constants.hpp"
#include <iostream>
#include <typeinfo>

int main() {
    App app(cst.get<int>("screenInitialWidth"),
            cst.get<int>("screenInitialHeight"), cst["titlebar"]);

    return 0;
}
