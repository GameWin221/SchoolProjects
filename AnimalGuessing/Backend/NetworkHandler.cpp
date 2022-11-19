#include "NetworkHandler.hpp"
#include "json.hpp"
#include <iostream>

NetworkHandler::NetworkHandler()
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
}

void NetworkHandler::Start()
{
    m_Service.start(m_Settings);
}

std::tuple<std::string, std::string> NetworkHandler::Respond(std::string operation, std::string text)
{
    std::string expectation{};
    std::string response{};

    for (auto& c : text)
        if (c == '_')
            c = ' ';

    if(operation == "answer") // If the program is still guessing
    {
        bool lastQuestion = false;
        bool yes = text == "yes";

        if (lastQuestion)
        {
            if (yes) // Restart if the program guessed the animal
            {
                expectation = "restart";
                response = "restart";
            }
            else // If the program has failed to guess the animal, ask the user about what animal they were thinking
            {
                expectation = "animal";
                response = "O jakim zwierzeciu myslales? (podaj tylko nazwe)";
            }
        }
        else
        {
            if (yes) // Go left
            {
                expectation = "answer";
                response = "Czy to {left}?";
            }
            else // Go right
            {
                expectation = "answer";
                response = "Czy to {right}?";
            }
        }
    }
    else if(operation == "animal") // If the program has failed to guess the animal and the user typed in the correct animal
    {
        expectation = "question";
        response = "Jak odroznic " + text + " od tego o ktory program zgadl?";

        // After the user types in the animal that they were thinking of, ask them how to differentiate it from the animal that the program has guessed 
    }
    else if(operation == "question") // If the program has failed to guess the animal and the user typed in the question that differentiates it from the guessed one
    {
        expectation = "restart";
        response = "restart";
    }
    else if (operation == "start") // Starting the game
    {
        expectation = "answer";
        response = "Czy to ssak?"; // Ask the question
    }
    else
        std::cout << "[NetworkHandler::Respond] - Wrong operation!\n";

    std::cout << "[IN]:  [" << operation << "]: \"" << text << "\"\n";
    std::cout << "[OUT]: [" << expectation << "]: " << response << "\"\n";

    return make_tuple(expectation, response);
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

    const auto [expectation, response] = Respond(operation, text);
    const std::string content = ToJson(expectation, response);

    session->close(restbed::OK, content, {{"Content-Length", std::to_string(content.size())}});
}