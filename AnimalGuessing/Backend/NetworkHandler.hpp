#pragma once

#include <memory>
#include <functional>
#include <string>
#include <restbed>
#include <resource.hpp>
#include <settings.hpp>
#include <service.hpp>

class NetworkHandler
{
public:
    NetworkHandler(std::function<std::tuple<std::string, std::string>(std::string, std::string)> respondMethod);

private:
    std::tuple<std::string, std::string> GetPathParameters(const std::shared_ptr<restbed::Session> session) const;

    std::string ToJson(std::string expectation, std::string response);

    void GetHandler(const std::shared_ptr<restbed::Session> session);

    std::function<std::tuple<std::string, std::string>(std::string, std::string)> RespondMethod;

    std::shared_ptr<restbed::Resource> m_Resource;
    std::shared_ptr<restbed::Settings> m_Settings;
    restbed::Service m_Service;
};