#include "model.hxx"

using namespace ge211;

Model::Model(int size)
        : Model(size, size)
{ }

Model::Model(int width, int height)
        : board_({width, height})
{
    compute_next_moves_();
}

Model::Rectangle
Model::board() const
{
    return board_.all_positions();
}

Player
Model::operator[](Position pos) const
{
    return board_[pos];
}

const Move*
Model::find_move(Position pos) const
{
    auto i = next_moves_.find(pos);

    if (i == next_moves_.end()) {
        // Nothing was found, so return NULL (nullptr in C++)
        return nullptr;
    } else {
        // Dereferences the iterator to get the value then returns a pointer
        // to that value. This is safe as the pointer is to a value living
        // within the `next_moves_` structure.
        return &(*i);
    }
}

void
Model::play_move(Position pos)
{
    if (is_game_over()) {
        throw Client_logic_error("Model::play_move: game over");
    }

    const Move* movep = find_move(pos);
    if (movep == nullptr) { // check if there was no such move
        throw Client_logic_error("Model::play_move: no such move");
    }

    really_play_move_(*movep);


}

//
// BELOW ARE HELPER FUNCTIONS
// Our tests will run on the public functions, not the helper functions
// However, these will be extremely useful in writing the above functions
//

Position_set
Model::find_flips_(Position current, Dimensions dir) const
{
    Position_set move_set = Position_set();
    Position_set empty_set = Position_set();

    Position next = current + dir;

    while(board_.good_position(next))
    {
        if(board_[next] == turn())
        {

            return move_set;
        }

        if(board_[next] == Player::neither)
        {
            return empty_set;
        }
        if(board_[next] == other_player(turn_))
        {
            move_set[{next}] = true;
        }

        next += dir;

    }
    return empty_set;
}

Position_set
Model::evaluate_position_(Position pos) const
{
    Position_set all_set = Position_set();
    Position_set current_set = Position_set();

    for(auto dim : board_.all_directions())
    {
        current_set = find_flips_(pos, dim);
        if (!current_set.empty())
        {

            all_set |= current_set ;

        }

    }
    if(!all_set.empty())
    {
        all_set[{pos}] = true;
    }

    return all_set;
}

void
Model::compute_next_moves_()
{
    next_moves_.clear();

    // Center phase check
    for(auto pos : board_.center_positions())
    {
        if(board_[pos] == Player::neither)
        {
            next_moves_[pos] = {pos};

        }
    }
    // Rest positions check
    if(next_moves_.empty())
    {
        for(auto pos : board_.all_positions())
        {
            if(board_[pos] == Player::neither)
            {
                Position_set eval = evaluate_position_(pos);
                if (!eval.empty())
                {

                    next_moves_[pos] = eval;


                }
            }

        }
    }
}

bool
Model::advance_turn_()
{
    turn_ = other_player(turn());

    compute_next_moves_();

    return !next_moves_.empty();

}

void
Model::set_game_over_()
{
    turn_ = Player::neither;

    size_t light_count = board_.count_player(Player::light);
    size_t dark_count = board_.count_player(Player::dark);

    if(light_count > dark_count)
    {
        winner_ = Player::light;
    }
    if(dark_count > light_count)
    {
        winner_ = Player::dark;
    }
}
void
Model::really_play_move_(Move move)
{

    for(Position pos : move.second)
    {
        board_[pos] = turn();

    }

    if(advance_turn_())
    {
        return;
    }
    else
    {
        if(advance_turn_())
        {
            return;
        }
        else
        {
            set_game_over_();
        }
    }

}
