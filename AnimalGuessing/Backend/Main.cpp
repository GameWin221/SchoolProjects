#include <iostream>
#include <string>
#include <limits>
#include <fstream>
#include <sstream>

#include "NetworkHandler.hpp"

struct Node
{
	std::string question;

	Node* left;
	Node* right;
};

// Write a node along its children recursively to a file (output stream)
void NodeToStream(const Node* node, std::ofstream& file)
{
	if (!node)
	{
		file << "null\n";
		return;
	}

	file << node->question << '\n';

	NodeToStream(node->left, file);
	NodeToStream(node->right, file);
}

// Save the tree (starting from the first node) to a file
void SaveTree(const Node* firstNode, const std::string&& fileName)
{
	std::ofstream file(fileName);

	if (file.is_open())
	{
		NodeToStream(firstNode, file);

		std::cout << "Zapisano strukture drzewa do \"" + fileName + "\"!\n";
	}
	else
		throw std::runtime_error("void SaveTree() - Nie udalo sie zapisac struktury drzewa do \"" + fileName + "\"");
}

// Recursively read node data from the file (input stream)
Node* NodeFromStream(std::ifstream& file)
{
	std::string question{};

	std::getline(file, question);

	if (question == "null" || question.length() <= 0)
		return nullptr;

	return new Node{
		question,
		NodeFromStream(file),
		NodeFromStream(file),
	};
}

// Return the first node of a tree
Node* LoadTree(const std::string&& fileName)
{
	std::ifstream file(fileName);

	Node* node = nullptr;

	if (file.is_open())
	{
		node = NodeFromStream(file);

		if (node != nullptr)
			std::cout << "Wczytano strukture drzewa z \"" + fileName + "\"!\n";
		else
		{
			std::cout << "Struktura drzewa w \"" << fileName << "\" jest pusta!\n";

			node = new Node{
				"Czy to kot?",

				nullptr,
				nullptr
			};
		}
	}
	else
	{
		std::cout << "Nie znaleziono pliku \"" << fileName << "\"!\n";
		
		std::ofstream newFile(fileName);

		node = new Node{
			"Czy to kot?",

			nullptr,
			nullptr
		};
	}

	return node;
}

int main()
{
	Node* firstNode = LoadTree("Saved.tree");

	Node* currentNode = firstNode;
	bool lastQuestion = false;

	std::string enteredAnimal{};

	NetworkHandler networkHandler([&currentNode, &lastQuestion, &enteredAnimal, firstNode](std::string operation, std::string text) {
		std::string expectation{};
		std::string response{};

		for (auto& c : text)
			if (c == '_')
				c = ' ';

		if (operation == "answer") // If the program is still guessing
		{
			bool yes = text == "yes";

			if (lastQuestion)
			{
				if (yes) // Restart if the program guessed the animal
				{
					expectation = "restart";
					response = "Zgadlem :)";

					SaveTree(firstNode, "Saved.tree");

					currentNode = firstNode;
					lastQuestion = false;
					enteredAnimal.clear();
				}
				else // If the program has failed to guess the animal, ask the user about what animal they were thinking
				{
					expectation = "animal";
					response = "O jakim zwierzeciu myslales? (podaj tylko nazwe)";
				}
			}
			else
			{
				if (yes) // Go right
				{
					currentNode = currentNode->right;

					expectation = "answer";
					response = currentNode->question;
				}
				else // Go left
				{
					currentNode = currentNode->left;

					expectation = "answer";
					response = currentNode->question;
				}

				if (!currentNode->left || !currentNode->right)
					lastQuestion = true;
			}
		}
		else if (operation == "animal") // If the program has failed to guess the animal and the user typed in the correct animal
		{
			if (lastQuestion)
			{
				enteredAnimal = text;

				std::string lastAnimal = currentNode->question.substr(
					currentNode->question.find_last_of(' ') + 1,
					currentNode->question.length() - currentNode->question.find_last_of(' ') - 2
				);

				expectation = "question";
				response = "Jakim pytaniem mozna odroznic " + text + " od " + lastAnimal + "?";

				// After the user types in the animal that they were thinking of, ask them how to differentiate it from the animal that the program has guessed 
			}
			else
				std::cout << "The user entered animal when \'lastQuestion\' was false!\n";
		}
		else if (operation == "question") // If the program has failed to guess the animal and the user typed in the question that differentiates it from the guessed one
		{
			if (lastQuestion && !enteredAnimal.empty())
			{
				expectation = "restart";
				response = "Kolejna proba";

				currentNode->right = new Node{
					std::string{"Czy to "} + enteredAnimal + std::string{"?"},
					nullptr,
					nullptr,
				};

				currentNode->left = new Node{
					currentNode->question,
					nullptr,
					nullptr
				};

				text[0] = std::toupper(text[0]);
				text.push_back('?');

				currentNode->question = text;

				SaveTree(firstNode, "Saved.tree");

				currentNode = firstNode;
				lastQuestion = false;
				enteredAnimal.clear();
			}
			else
				std::cout << "The user entered question when \'lastQuestion\' was false!\n";
		}
		else if (operation == "start") // Starting the game
		{
			expectation = "answer";
			response = currentNode->question; // Ask the question
		}
		else
			std::cout << "[NetworkHandler::Respond] - Wrong operation!\n";

		std::cout << "[IN]:  [" << operation << "]: \"" << text << "\"\n";
		std::cout << "[OUT]: [" << expectation << "]: " << response << "\"\n";

		return make_tuple(expectation, response);
	});
}