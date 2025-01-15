#pragma once

#include "../ollama/ollama.hpp"
#include "multiline_text.hpp"
#include <string>

class ChatManager {
    public:
    ChatManager() = default;
    ~ChatManager() = default;

    void addChat(const std::string name);
    void deleteChat(const std::string name);

    ChatBox &getActiveChatBox();
    ollama::messages &getActiveMessages();

    void setActiveChat(const std::string name);
    inline std::string getActiveChatName() { return this->activeChat; }
    inline int getNumberOfChats() { return this->chats.size(); }

    inline std::map<std::string, ChatBox> &getChats() { return this->chats; }
    inline std::map<std::string, ollama::messages> &getMessages() {
        return this->messages;
    }

    std::vector<std::string> getChatNames();

    bool chatExists(const std::string name);

    private:
    std::map<std::string, ChatBox> chats;
    std::map<std::string, ollama::messages> messages;

    std::string activeChat;
};
