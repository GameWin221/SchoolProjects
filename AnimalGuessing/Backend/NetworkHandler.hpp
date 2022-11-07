#pragma once

#include <memory>
#include <string>
#include "resource.hpp"
#include "settings.hpp"

class NetworkHandler
{
public:
    NetworkHandler();
    
    std::shared_ptr<restbed::Resource> GetResource() const { return m_Resource; };
    std::shared_ptr<restbed::Settings> GetSettings() const { return m_Settings; };

private:
    float Calculate(float num1, float num2, std::string operation);

    std::tuple<float, float, std::string> GetPathParameters(const std::shared_ptr<restbed::Session> session) const;
    std::string ToJson(float result);

    void GetHandler(const std::shared_ptr<restbed::Session> session);

    std::shared_ptr<restbed::Resource> m_Resource;
    std::shared_ptr<restbed::Settings> m_Settings;
};