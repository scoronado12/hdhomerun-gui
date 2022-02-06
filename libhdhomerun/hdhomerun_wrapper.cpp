
#ifndef HDHOMERUN_WRAPPER_CPP
#define HDHOMERUN_WRAPPER_CPP

#include "hdhomerun_wrapper.h"

#define HOW_MANY 1

HDHomeRun_Wrapper::HDHomeRun_Wrapper()
{
    /*Find HD Home Run Device*/ 
    struct hdhomerun_discover_device_t device[1];
    int nDevice = hdhomerun_discover_find_devices_custom_v2(0, 
            HDHOMERUN_DEVICE_TYPE_TUNER,
            HDHOMERUN_DEVICE_ID_WILDCARD, 
            device,
            HOW_MANY);
    try
    {
        if (nDevice < -1 )
            throw "Error Occurred";
        if (nDevice == 0)
            throw "Could Not Find A Device!";

    }
    catch (std::exception &e)
    {
        exit(1);
    }
    /*Decode Attributes of Device */ 
    this->deviceBaseUrl = (std::string) device[0].base_url;
    this->deviceIPAddress =  std::to_string(static_cast<unsigned int>((device[0].ip_addr >> 24)) & 0x0FF) +"."
        + std::to_string((static_cast<unsigned int>((device[0].ip_addr >> 16)) & 0x0FF))+ "." 
        + std::to_string((static_cast<unsigned int>((device[0].ip_addr >> 8)) & 0x0FF))+ "." 
        + std::to_string((static_cast<unsigned int>((device[0].ip_addr >> 0)) & 0x0FF)); 

    this->tunerCount = static_cast<int>(device[0].tuner_count); 
    this->isLegacy = device[0].is_legacy;
    this->deviceAuth = device[0].device_auth; 
    this->deviceType = std::to_string(device[0].device_type); 
    this->deviceId = std::to_string(static_cast<int>(device[0].device_id));

    /* Download Channel Data From Device */
    std::string lineupURL = this->deviceBaseUrl + "/lineup.json";
    const char *cStyleURL = lineupURL.c_str();
    std::string jsonString;
    CURL *curl_handle = curl_easy_init();
    CURLcode response;
    Q_UNUSED(response)

    if(curl_handle){
        curl_easy_setopt(curl_handle, CURLOPT_URL, cStyleURL);
        curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, &jsonString);
        response = curl_easy_perform(curl_handle);

    } else {
        std::cout << "Curl Failed" << "\n";
        exit(1);
    }

   QString json = QString::fromStdString(jsonString);
   QJsonDocument lineup = QJsonDocument::fromJson(json.toUtf8());
   QJsonArray lineupArr = lineup.array();
   /* Log found channels*/
    for (int i = 0 ; i < lineupArr.count(); i++){
        std::cout << "[FOUND CHANNEL]" << lineupArr.at(i)["GuideNumber"].toString().toStdString()
            << lineupArr.at(i)["GuideName"].toString().toStdString() << " " <<
            lineupArr.at(i)["URL"].toString().toStdString() + "\n";
        channels.push_back(Channel(lineupArr.at(i)["GuideName"].toString().toStdString(),
                                    atof(lineupArr.at(i)["GuideNumber"].toString().toStdString().c_str()),
                                    lineupArr.at(i)["URL"].toString().toStdString()));
    }

} 

HDHomeRun_Wrapper::~HDHomeRun_Wrapper()
{
    
}


size_t HDHomeRun_Wrapper::WriteCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

std::vector<Channel> HDHomeRun_Wrapper::getChannels(){
    return channels;
}

std::string HDHomeRun_Wrapper::getDeviceBaseURL()
{
    return deviceBaseUrl;
}

std::string HDHomeRun_Wrapper::getDeviceIPAddress(){
   return deviceIPAddress;
}
std::string HDHomeRun_Wrapper::getDeviceId()
{
    return deviceId;
}
std::string HDHomeRun_Wrapper::getDeviceType()
{
    return deviceType;
}
bool HDHomeRun_Wrapper::getIsLegacy()
{
    return isLegacy;
}
std::string HDHomeRun_Wrapper::getDeviceAuth()
{
    return deviceAuth;
}
std::string HDHomeRun_Wrapper::getBaseUrl()
{
    return baseUrl;
}

std::string HDHomeRun_Wrapper::toString()
{
    return deviceBaseUrl + "|" + 
        deviceIPAddress + "|" +
        deviceId + "|" + 
        deviceType + "|" + 
        std::to_string(isLegacy) + "|" +
        deviceAuth + "|\n";
}

#endif //HDHOMERUN_WRAPPER_CPP
