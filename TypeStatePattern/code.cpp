#include <vector>
#include <tuple>
#include <iostream>
#include <memory>
#include <string_view>

class Token
{
    friend class ChatService;
    friend struct NamelessChatService;
};

struct Socket
{
    Socket(Token /*token*/) {};
};

struct Endpoint {};
struct Message {};

class ChatService
{
public:
    void setName(std::string_view /*name*/) {}
    void connect(Endpoint) {}
    void sendMessage(std::string_view /*message*/) {}
    void disconnect() {}

private:
    std::unique_ptr<Socket> m_socket;
};

// Typestate pattern!
struct ConnectedChatService
{
    ConnectedChatService(std::unique_ptr<Socket> state) : m_socket(std::move(state)) {}
    void sendMessage(std::string_view /*msg*/) {};

    void disconnect() {}
private:
    std::unique_ptr<Socket> m_socket;
};

struct DisconnectedChatService
{
    DisconnectedChatService(std::unique_ptr<Socket> state) : m_socket(std::move(state)) {}

    [[nodiscard]] ConnectedChatService connect(Endpoint)&& { return ConnectedChatService(std::move(m_socket)); }

private:
    std::unique_ptr<Socket> m_socket;
};

struct NamelessChatService
{
    [[nodiscard]] DisconnectedChatService setName(std::string_view /*name*/)
    {
        return DisconnectedChatService(std::make_unique<Socket>(Token{}));
    };
};


// Lets give this to a tester
int main()
{
    ChatService service;

    service.disconnect();
    service.sendMessage("Hi");
    service.connect(Endpoint{});
    service.setName("Roman");
    service.sendMessage("Goodbye");
    service.disconnect();

    // Now
    NamelessChatService init;

    auto disconnected = init.setName("Roman");
    auto connected = std::move(disconnected).connect(Endpoint{});

    // No warning for use after move? --> Clang tidy might help
    //connected = std::move(disconnected).connect(Endpoint{});
    //auto connected = init.setName("Roman").connect(Endpoint{});

    connected.sendMessage("Jooho");
    connected.disconnect();
}
