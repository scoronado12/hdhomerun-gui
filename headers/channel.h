#include <iostream>
#include <string>

#ifndef CHANNEL_H
#define CHANNEL_H


class Channel
{
public:
    Channel(std::string stationId, double number, std::string url);
    std::string getChannelName() const;
    double getNumber() const;
    std::string getURL() const;
    std::string toString();
private:
    std::string stationId;
    double number;
    std::string url;
};

#endif // CHANNEL_H
