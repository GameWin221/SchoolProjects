#include "NetworkHandler.hpp"
#include "json.hpp"
#include <iostream>

NetworkHandler::NetworkHandler(std::function<std::tuple<std::string, std::string>(std::string, std::string)> respondMethod)
    : RespondMethod(respondMethod)
{
    m_Settings = std::make_shared<restbed::Settings>();
    m_Settings->set_port(8080);
    m_Settings->set_default_header("Connection", "close");
    m_Settings->set_default_header("Access-Control-Allow-Origin", "*");

    m_Resource = std::make_shared<restbed::Resource>();
    m_Resource->set_path(
        "/{operation: answer|question|animal|start}"
        "/{text:[a-z_]+}"
    );

    m_Resource->set_method_handler("GET", 
        [&](const auto session) {
            GetHandler(session);
        });

    m_Service.publish(m_Resource);
    m_Service.start(m_Settings);
}

std::tuple<std::string, std::string> NetworkHandler::GetPathParameters(const std::shared_ptr<restbed::Session> session) const
{
    const auto& request = session->get_request();

    const auto operation = request->get_path_parameter("operation");
    const auto text = request->get_path_parameter("text");

    return make_tuple(operation, text);
}
std::string NetworkHandler::ToJson(std::string expectation, std::string response)
{
    nlohmann::json jsonResult {
        {"expectation", expectation},
        {"response", response}
    };

    return jsonResult.dump();
}

void NetworkHandler::GetHandler(const std::shared_ptr<restbed::Session> session)
{
    const auto [operation, text] = GetPathParameters(session);

    const auto [expectation, response] = RespondMethod(operation, text);
    const std::string content = ToJson(expectation, response);

    session->close(restbed::OK, content, {{"Content-Length", std::to_string(content.size())}});
}