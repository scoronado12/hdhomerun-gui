#ifndef CHANNEL_H
#define CHANNEL_H

#include <string>

class Channel
{
public:
    Channel(std::string stationId, double number, std::string url);
    std::string getChannelName();
    double getNumber();
    std::string getURL();
private:
    std::string stationId;
    double number;
    std::string url;
};

#endif // CHANNEL_H
