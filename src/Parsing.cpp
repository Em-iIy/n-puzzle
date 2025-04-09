//
// Created by abi on 7-4-25.
//

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <unordered_set>
#include <stdexcept>

std::vector<uint> parse_puzzle(const std::string& filename)
{
    std::ifstream file(filename);
    if (!file.is_open())
    {
        throw std::runtime_error("Unable to open file: " + filename);
    }

    std::vector<std::string> lines;
    std::string line;
    while (std::getline(file, line))
    {
        if (line.empty() || line[0] == '#')
            continue;

        size_t comment_pos = line.find('#');
        if (comment_pos != std::string::npos)
            line = line.substr(0, comment_pos);

        while (!line.empty() && std::isspace(line.back()))
            line.pop_back();
        if (!line.empty())
            lines.push_back(line);
    }
    file.close();
    // for (auto a: lines)
    //     std::cout << "testing: " << a << std::endl;

    if (lines.empty())
        throw std::invalid_argument("Empty file or only comments");

    int N;
    std::istringstream size_stream(lines[0]);
    if (!(size_stream >> N) || N < 2)
        throw std::invalid_argument("Invalid grid size (must be >= 2)");

    if (lines.size() < static_cast<size_t>(N + 1))
        throw std::invalid_argument("Insufficient lines for grid");

    std::vector<uint> grid(N * N);
    std::unordered_set<int> seen;
    int idx = 0;

    for (int i = 0; i < N; ++i)
    {
        std::istringstream iss(lines[i + 1]);
        int j = 0;
        int num;
        
        while (iss >> num)
        {
            if (j >= N)
                throw std::invalid_argument("Too many numbers in row " + std::to_string(i));

            if (num < 0 || num >= N * N)
                throw std::invalid_argument("Number out of range in row " + std::to_string(i));

            if (seen.count(num))
                throw std::invalid_argument("Duplicate number in grid: " + std::to_string(num));

            seen.insert(num);
            grid[idx++] = num;
            ++j;
        }
        // check if the row has the same number to N
        if (j != N)
            throw std::invalid_argument("Too few numbers in row " + std::to_string(i));
    }

    if (seen.count(0) != 1)
        throw std::invalid_argument("Grid must contain exactly one 0");

    return grid;
}