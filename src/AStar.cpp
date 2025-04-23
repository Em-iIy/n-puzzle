#include "AStar.hpp"
#include <algorithm>

std::vector<e_move> AStar::solve(Square init)
{
    std::priority_queue<std::shared_ptr<Node>,
                        std::vector<std::shared_ptr<Node>>,
                        NodePtr> open_set;
    std::unordered_map<std::vector<uint>, bool, SquareHash> closed_set;

    auto start_node = std::make_shared<Node>(nullptr, init);
    open_set.push(start_node);
    std::unordered_map<std::shared_ptr<Node>, e_move> move_map;

    while (!open_set.empty())
    {
        auto current = open_set.top();
        open_set.pop();

        if (current->_sq.check_board())
        {
            std::vector<e_move> path;
            std::shared_ptr<Node> node = current;

            while (node != start_node)
            {
                path.push_back(move_map[node]);
                node = node->_parent;
            }

            std::reverse(path.begin(), path.end());
            return path;
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
                auto next_node = std::make_shared<Node>(current, next_board);
                move_map[next_node] = static_cast<e_move>(move);
                open_set.push(next_node);
            }
        }
    }
}