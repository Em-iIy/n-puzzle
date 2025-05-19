#pragma once

#include <queue>
#include <unordered_map>
#include <vector>
#include "Node.hpp"
#include "Square.hpp"

class AStar
{
    public:
        static std::vector<e_move> solve(Square ini);

    private:
        struct NodePtr
        {
            bool operator()(const std::shared_ptr<Node> &a, const std::shared_ptr<Node> &b) const
            {
                return *a > *b;
            }
        };

    struct SquareHash {
        size_t operator()(const std::vector<uint> &sq) const
        {
            std::size_t hash = 0;
            for (uint val : sq)
            {
                hash ^= std::hash<uint>{}(val) + 0x9e3779b9 + (hash << 6) + (hash >> 2);
            }
            return hash;
        }
    };

};
