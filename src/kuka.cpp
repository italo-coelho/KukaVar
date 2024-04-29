#include "kuka.h"

KUKA::KUKA(IPAddress ip, int port, Client& client)
{
    _ip = ip;
    _port = port;
    _client = &client;
#ifdef ESP32
    _id = esp_random();
#else
    _id = random(0, UINT16_MAX);
#endif //ESP32
    if(_client != nullptr)
        _client->setTimeout(10);
}

KUKA::~KUKA()
{
    _client->stop();
}

bool KUKA::connect()
{
    if(_client == nullptr)
        return false;
    _connected = _client->connect(_ip, _port);
    return _connected;
}

void KUKA::disconnect()
{
    if(_client == nullptr)
        return;
    _client->flush();
    _client->stop();
}

String KUKA::read(String var_name, bool verbose)
{
    if(!connected())
        return "ERROR->Disconnected";

    uint8_t flag = 0;
    uint16_t var_name_len = var_name.length();
    uint16_t req_len = var_name_len + 3;

    _request.clear();
    _request.push_back(pack(_id, 0));
    _request.push_back(pack(_id, 1));
    _request.push_back(pack(req_len, 0));
    _request.push_back(pack(req_len, 1));
    _request.push_back(flag);
    _request.push_back(pack(var_name_len, 0));
    _request.push_back(pack(var_name_len, 1));
    for(int i = 0; i < var_name_len; i++)
        _request.push_back(var_name[i]);

    _client->write(_request.data(), _request.size());
    _client->flush();

    return get_response(verbose);
}

bool KUKA::write(String var_name, String value, bool verbose)
{
    if(!connected())
        return false;

    uint8_t flag = 1;
    uint16_t var_name_len = var_name.length();
    uint16_t value_len = value.length();
    uint16_t req_len = var_name_len + 3 + 2 + value_len;

    _request.clear();
    _request.push_back(pack(_id, 0));
    _request.push_back(pack(_id, 1));
    _request.push_back(pack(req_len, 0));
    _request.push_back(pack(req_len, 1));
    _request.push_back(flag);
    _request.push_back(pack(var_name_len, 0));
    _request.push_back(pack(var_name_len, 1));
    for(int i = 0; i < var_name_len; i++)
        _request.push_back(var_name[i]);
    _request.push_back(pack(value_len, 0));
    _request.push_back(pack(value_len, 1));
    for(int i = 0; i < var_name_len; i++)
        _request.push_back(value[i]);
    
    _client->write(_request.data(), _request.size());
    _client->flush();

    String response = get_response(verbose);
    if(response.indexOf("ERROR->") == -1)
        return true;
    else
        return false;
}

bool KUKA::connected()
{
    if(_client == nullptr)
        _connected = false;
    else
        _connected = _client->connected();
    return _connected;
}

uint8_t KUKA::pack(uint16_t value, int index)
{
    uint8_t array[2];
    array[0] = (value >> 8) & 0xFF;
    array[1] = value & 0xFF;

    return array[index];
}

uint16_t KUKA::unpack(uint8_t byte_0, uint8_t byte_1)
{
    return (byte_0 << 8) + byte_1;
}

String KUKA::get_response(bool verbose)
{
#ifdef ESP32
    TickType_t then = xTaskGetTickCount();
    while(!_client->available())
    {
        if(xTaskGetTickCount() - then >= TIMEOUT)
            break;
    }
#else
    uint32_t then = millis();
    while(!_client->available())
    {
        if(millis() - then >= TIMEOUT)
            break;
        yield();
    }
#endif //ESP32

    if(_client->available())
    {
        size_t bytes_read = _client->readBytes(buffer, MAX_PACKET);

        // (!HHHB) = 7 bytes
        // uint16_t var_value_len = bytes_read - 7 - 3;

        uint16_t id = unpack(buffer[0], buffer[1]);
        uint16_t body_len = unpack(buffer[2], buffer[3]);
        uint8_t flag = buffer[4];
        uint16_t var_value_len = unpack(buffer[5], buffer[6]);
        String value = "";
        for(int i = 0; i < var_value_len; i++)
            value += char(buffer[7+i]);
        char isOK[3];
        for(int i = 0; i < 3; i++)
            isOK[i] = char(buffer[7 + var_value_len + 1]);

        if(verbose)
        {
            Serial.printf("[KUKA Var]: Flag = %d\n", flag);
            Serial.printf("[KUKA Var]: Is Ok = %s\n", isOK);
            Serial.printf("[KUKA Var]: Message ID = %d\n", id);
            Serial.printf("[KUKA Var]: Body Length = %d\n", body_len);
            Serial.printf("[KUKA Var]: Value Length = %d\n", var_value_len);
            Serial.printf("[KUKA Var]: Var Value = %s\n", value);
        }

        // if(id == _id && isOK[2] == 0x01)
        if(id == _id)
        {
            _id += 1;
            return value;
        }
        else
            return "ERROR->Invalid Data";
    }
    else
        return "ERROR->Client Unavailable";
}