#pragma once

#include "../ollama/ollama.hpp"
#include "multiline_text.hpp"
#include <string>

class ChatManager {
    public:
    ChatManager() = default;
    ~ChatManager() = default;

    void createChat(const std::string name);
    void deleteChat(const std::string name);

    ChatBox &getChatBox(const std::string name);
    ollama::messages &getMessages(const std::string name);

    private:
    std::map<std::string, ChatBox> chats;
    std::map<std::string, ollama::messages> messages;
};
