#include <iostream>
#include <fstream>
#include <algorithm>
#include <string>
#include <filesystem>
#include <array>
#include <vector>
#include <unordered_map>

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

const inline bool IsOperator(const char c)
{
    return c == '!' || ((c >= 35 && c <= 47) && c != '\'') || (c >= 58 && c <= 64) || (c >= 91 && c <= 96) || (c >= 123 && c <= 126);
}

std::vector<std::string> ToTokens(const std::string& sourceCode)
{
    std::vector<std::string> tokens{};
    std::string word{""};

    bool isQuoting = false;

    for (int i = 0; i < sourceCode.length(); ++i)
    {
        char c = sourceCode[i];

        // If 'c' is a start of a comment
        if (c == '#')
        {
            while (i < sourceCode.length() && sourceCode[++i] != '\n');
        }
        // If 'c' is the opening/closing of quotation
        else if (c == '\"' || c == '\'')
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
        else if (IsOperator(c))
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

std::vector<std::string> OptimizeTokens(const std::vector<std::string>& tokens)
{
    std::vector<std::string> optimizedTokens(tokens.size());

    optimizedTokens = tokens;

    for (uint32_t i = 0U; i < tokens.size(); i++)
    {
        if (tokens[i] == "def" && tokens[i + 2] == "(")
        {
            std::string function = tokens[i + 1];
            std::cout << "Found function: " << function << '\n';

            for (auto& tok : optimizedTokens)
            {
                if (tok == function)
                    tok = "function";
            }
        }
        
        else if (tokens[i] == "=" && tokens[i+1] != "=")
        {
            //char c = tokens[i - 1][0];

            //if (IsOperator(c))
            //{
            //    int j = i - 1;
            //    
            //    while (tokens[j][0] != c - 1 && tokens[j][0] != c - 2)
            //        j--;
            //
            //    std::vector<std::string> variables{};
            //
            //    std::cout << j << ' ' << i << '\n';
            //
            //    for (int k = j; k < i - 1; k++)
            //        if (!IsOperator(tokens[k][0]))
            //            variables.emplace_back(tokens[k]);
            //
            //    for(const auto& var : variables)
            //        std::cout << "Found variable in a tuple: " << var << '\n';
            //}
            //else
            if(!IsOperator(tokens[i-1][0]))
            {
                std::string variable = tokens[i - 1];
                std::cout << "Found variable: " << variable << '\n';

                for (auto& tok : optimizedTokens)
                {
                    if (tok == variable)
                        tok = "var";
                }
            }
        }
        
    }

    return optimizedTokens;
}

int main()
{
    std::srand(time(0));

    auto tokensA = /*OptimizeTokens*/(ToTokens(ReadFile("Files/D10.txt")));
    auto tokensB = /*OptimizeTokens*/(ToTokens(ReadFile("Files/D10Plagiat.txt")));

    std::unordered_map<std::string, uint32_t> tokenCountA{};
    std::unordered_map<std::string, uint32_t> tokenCountB{};


    for (const auto& tok : tokensA)
    {
        if (tokenCountA.count(tok) > 0)
            tokenCountA.at(tok)++;
        else
            tokenCountA[tok] = 0;
   
        //std::cout << '|' << tok << '|' << '\n';
    }
    for (const auto& tok : tokensB)
    {
        if (tokenCountB.count(tok) > 0)
            tokenCountB.at(tok)++;
        else
            tokenCountB[tok] = 0;
        //std::cout << '|' << tok << '|' << '\n';
    }

    std::cout << tokenCountA.size() << '\n';
    std::cout << tokenCountB.size() << '\n';

    uint32_t allTokenCounts = tokenCountA.size();
    uint32_t matchingTokenCounts = 0U;

    for (const auto& [token, count] : tokenCountA)
    {
        if (tokenCountB.count(token) > 0)
            if (tokenCountB.at(token) == count)
                matchingTokenCounts++;
    }

    std::cout << "Matching token counts for tokenCountA: " << matchingTokenCounts << ", this is " << (float)matchingTokenCounts / allTokenCounts << "%!\n";

    allTokenCounts = tokenCountA.size();
    matchingTokenCounts = 0U;

    for (const auto& [token, count] : tokenCountB)
    {
        if (tokenCountA.count(token) > 0)
            if (tokenCountA.at(token) == count)
                matchingTokenCounts++;
    }
    
    std::cout << "Matching token counts for tokenCountB: " << matchingTokenCounts << ", this is " << (float)matchingTokenCounts / allTokenCounts << "%!\n";

    //for (const auto& entry : fs::directory_iterator("Files"))
        //std::cout << entry.path().string() << ": " << CalculateValue(ReadFile(entry.path().string())) << '\n';
}