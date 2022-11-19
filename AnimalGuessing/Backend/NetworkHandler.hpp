#pragma once

#include <memory>
#include <string>
#include <restbed>
#include <resource.hpp>
#include <settings.hpp>
#include <service.hpp>

class NetworkHandler
{
public:
    NetworkHandler();
    
    void Start();

private:
    std::tuple<std::string, std::string> Respond(std::string operation, std::string text);
    std::tuple<std::string, std::string> GetPathParameters(const std::shared_ptr<restbed::Session> session) const;

    std::string ToJson(std::string expectation, std::string response);

    void GetHandler(const std::shared_ptr<restbed::Session> session);

    std::shared_ptr<restbed::Resource> m_Resource;
    std::shared_ptr<restbed::Settings> m_Settings;
    restbed::Service m_Service;
};