
#include "channel.h"

#ifndef CHANNEL_CPP
#define CHANNEL_CPP



Channel::Channel(std::string stationId, double number, std::string url)
{
    this->stationId = stationId;
    this->number = number;
    this->url = url;
    this->nowPlaying = "No Information Avaliable"; 
}

std::string Channel::getChannelName() const
{
    return stationId;
}

std::string Channel::getURL() const
{
    return url;
}

std::string Channel::getNowPlaying() const
{
    return nowPlaying;
}

double Channel::getNumber() const
{
    return number;
}

std::string Channel::toString()
{
    return stationId + " - " +
        std::to_string(number) + "-" +
        url + "-" +
       nowPlaying + "\n";
}

#endif //CHANNEL_CPP
