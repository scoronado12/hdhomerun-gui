#include "channel.h"
#include <string.h>



Channel::Channel(std::string stationId, double number, std::string url)
{
    this->stationId = stationId;
    this->number = number;
    this->url = url;
}

std::string Channel::getChannelName() const{
    return stationId;
}



std::string Channel::getURL() const{
    return url;
}

double Channel::getNumber() const{
    return number;
}

