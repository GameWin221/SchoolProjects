#include "NetworkHandler.hpp"

NetworkHandler::NetworkHandler()
{
    m_Settings = std::make_shared<restbed::Settings>();
    m_Settings->set_port(8080);
    m_Settings->set_default_header("Connection", "close");
    m_Settings->set_default_header( "Access-Control-Allow-Origin", "*");
}

float NetworkHandler::Calculate(float num1, float num2, std::string operation)
{

}

std::tuple<float, float, std::string> NetworkHandler::GetPathParameters(const std::shared_ptr<restbed::Session> session) const
{

}
std::string NetworkHandler::ToJson(float result)
{

}

void NetworkHandler::GetHandler(const std::shared_ptr<restbed::Session> session)
{

}