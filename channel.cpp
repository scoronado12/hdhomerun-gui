#include "channel.h"
#include <string.h>



Channel::Channel(std::string stationId, std::string number, std::string url)
{
    this->stationId = stationId;
    this->number = number;
    this->url = url;
}

std::string Channel::getChannelName(){
    return stationId;
}



std::string Channel::getURL(){
    return url;
}

std::string Channel::getNumber(){
    return number;
}

