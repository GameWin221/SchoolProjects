#include <iostream>
#include <string>
#include <limits>
#include <fstream>
#include <sstream>

struct Node
{
	std::string question;

	Node* left;
	Node* right;
};

bool YesNoQuestion()
{
	std::string decision{""};

	std::cin >> decision;

	for (auto& c : decision)
		c = std::tolower(c);

	return decision[0] == 'y' || decision[0] == 't';
}

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

	std::cout << "Pomysl o jakims zwierzeciu a ten program sprobuje je zgadnac (sprobuj kilka razy, to sie uczy na bledach):\n";

	int attempt = 0;

	while (1)
	{
		std::cout << "\nProba nr. " << attempt++ << ":\n";

		Node* currentNode = firstNode;

		while (currentNode->right && currentNode->left)
		{
			std::cout << currentNode->question << '\n';

			if (YesNoQuestion())
				currentNode = currentNode->right;
			else
				currentNode = currentNode->left;
		}

		std::cout << currentNode->question << '\n';

		if (YesNoQuestion())
		{
			std::cout << "Zgadlem :))\n";
		}
		else
		{
			std::string animal{ "" };
			std::string question{ "" };

			//std::cout << "Nie zgadlem :((\n";

			std::cout << "O czym myslales? (Podaj tylko nazwe)\n";
			std::cin >> animal;

			for (auto& c : animal)
				c = std::tolower(c);

			std::string lastAnimal = currentNode->question.substr(
				currentNode->question.find_last_of(' ')+1,
				currentNode->question.length() - currentNode->question.find_last_of(' ') - 2
			);

			std::cout << "Jakim pytaniem mozna odroznic " << animal << " od " << lastAnimal << "?\n";

			std::cin.ignore();
			std::getline(std::cin, question);

			currentNode->right = new Node{
				std::string{"Czy to "} + animal + std::string{"?"},
				nullptr,
				nullptr,
			};

			currentNode->left = new Node{
				currentNode->question,
				nullptr,
				nullptr
			};

			currentNode->question = question;

			SaveTree(firstNode, "Saved.tree");
		}
	}
}