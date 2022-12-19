#include <iostream>
#include <fstream>
#include <algorithm>
#include <string>
#include <filesystem>
namespace fs = std::filesystem;

std::string ReadFile(const std::string&& path)
{
	std::ifstream file(path);

    if (!file.is_open())
        return std::string{"FAILED TO READ FILE!"};

    std::string buffer{};

    std::string line{};
    while (std::getline(file, line))
        buffer.append(line+'\n');

    if (buffer.empty())
       return "EMPTY BUFFER!";

    return buffer;
}

double CalculateValue(const std::string& sourceCode)
{
    if (sourceCode.length() <= 0)
        return -1.0;
 
    int64_t newLineCount{ std::count(sourceCode.begin(), sourceCode.end(), '\n') };

    double v{};
    for (int64_t i{}; const char& c : sourceCode)
        v += c + (i++ % 128) * newLineCount;

    return v / sourceCode.length();
}

bool SimplestTest(const double& val/*, const std::vector<double>& sourceCodeValues*/)
{
    return (val == /*any other value in sourceCodeValues*/ 0.0);
}

bool ComplexTest(const std::vector<std::string>& tokensA, const std::vector<std::string>& tokensB)
{
    uint64_t maxTokens{ std::max(tokensA.size(), tokensB.size()) };

    uint32_t matchingTokens{};

    for (uint32_t i{}; i < maxTokens; ++i)
    {
        const std::string a = tokensA[i];
        const std::string b = tokensB[i];

        if (a == b) // I need a better way to determine similarity
            matchingTokens++;
    }

    return ((maxTokens - matchingTokens) / maxTokens > /*someThreshold*/ 0.5);
}

std::vector<std::string> ToTokens(const std::string& sourceCode)
{
    std::vector<std::string> tokens{};
    std::string word{""};

    bool isQuoting = false;

    for (int i = 0; i < sourceCode.length(); ++i)
    {
        char c = sourceCode[i];

        // If 'c' is the opening/closing of quotation
        if (c == '\"' || c == '\'')
        {
            isQuoting = !isQuoting;

            if(!isQuoting)
                word += c;

            if (i < sourceCode.length() && word.length() > 0)
            {
                tokens.emplace_back(word);
                word.clear();
            }

            if (isQuoting)
                word += c;
        }
        else if (isQuoting)
        {
            word += c;
        }
        // If 'c' is a whitespace
        else if (c == ' ' || c == '\n')
        {
            if (i < sourceCode.length() && word.length() > 0)
            {
                tokens.emplace_back(word);
                word.clear();
            }
        }
        // If 'c' is an operator or is quoting
        else if (c == '!'|| (c >= 35 && c <= 47) || (c >= 58 && c <= 64) || (c >= 91 && c <= 96) || (c >= 123 && c <= 125))
        {
            if (i < sourceCode.length() && word.length() > 0)
            {
                tokens.emplace_back(word);
                word.clear();
            }

            tokens.emplace_back(std::string{c});
        }
        // If it is an ordinary character (a-z, A-Z, 0-9)
        else
            word += c;
    }

    return tokens;
}

int main()
{
    auto tokens = ToTokens(ReadFile("Files/D10.txt"));

    for (const auto& tok : tokens)
        std::cout << '|' << tok << '|' << '\n';

    //for (const auto& entry : fs::directory_iterator("Files"))
        //std::cout << entry.path().string() << ": " << CalculateValue(ReadFile(entry.path().string())) << '\n';
}