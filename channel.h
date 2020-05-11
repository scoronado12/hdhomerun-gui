#ifndef CHANNEL_H
#define CHANNEL_H

#include <string>

class Channel
{
public:
    Channel(std::string stationId, std::string number, std::string url);
    std::string getChannelName();
    std::string getNumber();
    std::string getURL();
private:
    std::string stationId;
    std::string number;
    std::string url;
};

#endif // CHANNEL_H