#include "AStar.hpp"
#include <algorithm>

std::ostream	&operator<<(std::ostream &o, const Result &result)
{
	o << "Hueristic:\t\t";
	switch (result.heuristic)
	{
	case Node::MANHATTAN:
		o << "Manhattan";
		break;
	case Node::HAMMING:
		o << "Hamming";
		break;
	case Node::LINEAR:
		o << "Linear Conflict";
		break;
	default:
		break;
	}
	o << std::endl;
	o << "Type:\t\t\t";
	switch (result.type)
	{
	case Node::ASTAR:
		o << "Default";
		break;
	case Node::GREEDY:
		o << "Greedy";
		break;
	case Node::UNIFORM:
		o << "Uniform";
		break;
	default:
		break;
	}
	o << std::endl;
	o << "Moves required:\t\t" << result.solution.size() << std::endl;
	o << "solution found in:\t" << result.time_elapsed << " ms" << std::endl;
	o << "Time complexity:\t" << result.time_complexity << std::endl;
	o << "Size complexity:\t" << result.size_complexity << std::endl;
	return (o);
}

Result AStar::solve(Square init, Node::Heuristic heuristic, Node::Type type)
{
    std::priority_queue<std::shared_ptr<Node>,
                        std::vector<std::shared_ptr<Node>>,
                        NodePtr> open_set;
    std::unordered_map<std::vector<uint>, bool, SquareHash> closed_set;

    std::unordered_map<std::shared_ptr<Node>, e_move> move_map;
	Result	result = {std::vector<e_move>(), 0.0, 0, 0, heuristic, type};

    std::shared_ptr<Node> start_node = std::make_shared<Node>(nullptr, init, heuristic, type);
    open_set.push(start_node);
    while (!open_set.empty())
    {
        auto current = open_set.top();
        open_set.pop();
		result.time_complexity++;

        if (current->_sq.check_board())
        {
			result.size_complexity = open_set.size() + closed_set.size() + 1; // + 1 for popped node

            std::shared_ptr<Node> node = current;
            while (node != start_node)
            {
                result.solution.push_back(move_map[node]);
                node = node->_parent;
            }

            std::reverse(result.solution.begin(), result.solution.end());
            return result;
        }

        closed_set[current->_sq._board] = true;

        for (int move = MOVE_UP; move <= MOVE_RIGHT; ++move)
        {
            Square next_board = current->_sq;
            if (next_board.make_move(static_cast<e_move>(move)))
            {
                if (closed_set.find(next_board._board) != closed_set.end())
                {
                    continue;
                }
                auto next_node = std::make_shared<Node>(current, next_board, heuristic, type);
                move_map[next_node] = static_cast<e_move>(move);
                open_set.push(next_node);
            }
        }
    }
	throw std::runtime_error("Puzzle is not solvable!");
    return {};
}