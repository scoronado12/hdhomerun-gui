#ifndef CHANNEL_H
#define CHANNEL_H

#include <string>

class Channel
{
public:
    Channel(std::string stationId, double number, std::string url);
    std::string getChannelName() const;
    double getNumber() const;
    std::string getURL() const;
private:
    std::string stationId;
    double number;
    std::string url;
};

#endif // CHANNEL_H
