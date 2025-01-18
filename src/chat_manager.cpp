#include "chat_manager.hpp"

bool ChatManager::chatExists(const std::string name) {
    return this->chats.contains(name) && this->messages.contains(name);
}

void ChatManager::deleteChat(const std::string name) {
    if (this->chatExists(name)) {
        this->chats[name] = ChatBox();
        this->messages[name] = ollama::messages();
        return;
    }
    std::cerr << "Chat: " << name << " does not exist" << std::endl;
    exit(1);
}
ChatBox &ChatManager::getActiveChatBox() {
    if (this->chatExists(this->activeChat)) {
        return this->chats[this->activeChat];
    }
    std::cerr << "Chat: " << this->activeChat << " does not exist" << std::endl;
    exit(1);
}
ollama::messages &ChatManager::getActiveMessages() {
    if (this->chatExists(this->activeChat)) {
        return this->messages[this->activeChat];
    }
    std::cerr << "Chat: " << this->activeChat << " does not exist" << std::endl;
    exit(1);
}
void ChatManager::setActiveChat(const std::string name) {
    if (this->chatExists(name)) {
        this->activeChat = name;
        return;
    }
    std::cerr << "Chat: " << name << " does not exist" << std::endl;
    exit(1);
}
void ChatManager::addChat(const std::string name) {
    if (!this->chatExists(name)) {
        this->chats[name] = ChatBox();
        this->messages[name] = ollama::messages();
        return;
    }
    std::cerr << "Chat: " << name << " already exists" << std::endl;
    exit(1);
}
std::vector<std::string> ChatManager::getChatNames() {
    std::vector<std::string> names;
    for (auto &chat : this->chats) {
        names.push_back(chat.first);
    }
    return names;
}
