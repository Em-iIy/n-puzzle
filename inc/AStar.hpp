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

        struct SquareHash
        {
            size_t operator()(const Square &sq) const
            {
                size_t hash = 0;
                for (uint n : sq._board)
                    hash *= 31 + n;
                return hash;
            }
        };

};
