#pragma once

#include <queue>
#include <unordered_map>
#include <vector>
#include "Node.hpp"
#include "Square.hpp"

struct Result
{
	std::vector<e_move>	solution;
	double				time_elapsed;
	uint64_t			time_complexity;
	uint64_t			size_complexity;
	Node::Heuristic		heuristic;
	Node::Type			type;
};

std::ostream	&operator<<(std::ostream &o, const Result &result);

class AStar
{
    public:
        static Result solve(Square init, Node::Heuristic heuristic, Node::Type type);

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
