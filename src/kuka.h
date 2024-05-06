#ifndef KUKA_H
#define KUKA_H

#include <vector>
#include <Client.h>
#include <Arduino.h>
#include <IPAddress.h>

#define KUKA_TIMEOUT 1000 //miliseconds
#define CLIENT_TIMEOUT 10 //miliseconds
#define KUKA_MAX_PACKET 1024 //bytes

class KUKA
{
    private:
        IPAddress _ip;
        int _port;

        bool _connected = false;
        Client* _client = nullptr;

        uint16_t _id = 99;
        std::vector<uint8_t> _request;

        uint8_t buffer[KUKA_MAX_PACKET];

        uint8_t pack(uint16_t value, int index);
        uint16_t unpack(uint8_t byte_0, uint8_t byte_1);

        String get_response(bool verbose = false);

        IPAddress parseIP(const char* address);

    public:
        KUKA(IPAddress ip, int port, Client& client);
        KUKA(const char* ip, int port, Client& client);
        ~KUKA();

        bool connect();
        bool connected();
        void disconnect();

        String read(String var_name, bool verbose = false);
        bool write(String var_name, String value, bool verbose = false);

        KUKA& setClient(Client& client);
};

#endif //KUKA_H