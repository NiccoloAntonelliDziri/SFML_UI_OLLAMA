#include "app.hpp"

#include "constants.hpp"
#include <iostream>

#include "thread_manager.hpp"

#include <typeinfo>

// float test(int a, int b) {
//     std::cout << "In function" << std::endl;
//     return (float)a + b;
// }
//
// void test2(int a) {
//     std::cout << "In function 2 with argument: " << a << std::endl;
// }
//
bool generate(std::string model, std::string prompt,
              std::function<void(const ollama::response &)> callback) {
    std::cout << "Generating" << std::endl;
    return ollama::generate(model, prompt, callback);
}

void on(const ollama::response &response) {
    // Print the token received
    std::cout << response << std::flush;

    // The server will set "done" to true for the last response
    if (response.as_json()["done"] == true)
        std::cout << std::endl;
}

int main() {
    // App app(cst.get<int>("screenInitialWidth"),
    //         cst.get<int>("screenInitialHeight"), cst["titlebar"]);

    ThreadManager<void> tm;
    tm.start(generate, "granite3-moe",
             "The quick brown fox jumps over the lazy dog", on);
    tm.getResult();

    return 0;
}
