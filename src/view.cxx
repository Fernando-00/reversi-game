#include "view.hxx"

// Convenient type aliases:
using Color = ge211::Color;
using Sprite_set = ge211::Sprite_set;

// You can change this or even determine it some other way:
static int const grid_size = 36;
static int const border_size = 2;
static Color const light_color {255, 255, 255};
static Color const dark_color {0, 0,0};
static Color const valid_color {0, 139, 139};
static Color const board_color {0, 100, 0};
static Color const invalid_color {100, 100, 100};
View::View(Model const& model)
        : model_(model),
          light_piece_sprite(grid_size/2 - border_size, light_color),
          dark_piece_sprite(grid_size/2- border_size, dark_color),
          board_sprite({grid_size - border_size, grid_size - border_size},
                       board_color),
          valid_sprite({grid_size - border_size, grid_size - border_size},
                       valid_color),
          invalid_light_sprite(grid_size/3 - border_size*2, light_color),
          invalid_dark_sprite(grid_size/3 - border_size*2, dark_color),
          loser_sprite(grid_size/2 - border_size, invalid_color)

{ }

View :: Position
View::board_to_screen (Model::Position logical) const
{
    return Position(logical.x*grid_size + border_size,
                    logical.y*grid_size + border_size);
}

Model :: Position
View ::screen_to_board (View :: Position physical ) const
{
    return Position(physical.x/grid_size, physical.y/grid_size);
}

void
View :: add_player_sprite_ (ge211 :: Sprite_set & sprites ,
                                    Player which ,
                                    ge211 :: Posn <int > physical ,
                                    int z_layer )
                                    const
{
    const Move* moves = model_.find_move(physical);

    Player player_type = model_.operator[](physical);

    physical = board_to_screen(physical);

    if(!model_.is_game_over())
    {
        if(player_type == Player::neither)
        {
            if(moves == nullptr)
            {
                if(which == Player::light)
                {
                    sprites.add_sprite(invalid_light_sprite,
                                       {physical.x + grid_size/4 - border_size,
                                        physical.y +grid_size/4 - border_size},
                                       z_layer);
                }
                if(which == Player::dark)
                {
                    sprites.add_sprite(invalid_dark_sprite,
                                       {physical.x + grid_size/4 - border_size,
                                        physical.y +grid_size/4 - border_size},
                                       z_layer);
                }
            }
            else
            {
                if(which == Player::light)
                {
                    sprites.add_sprite(light_piece_sprite,
                                       physical,
                                       z_layer);

                    sprites.add_sprite(valid_sprite,
                                       physical,
                                       1);

                }
                if(which == Player::dark)
                {
                    sprites.add_sprite(dark_piece_sprite,
                                       physical,
                                       z_layer);
                    sprites.add_sprite(valid_sprite,
                                       physical,
                                       1);
                }
            }
        }
        else
        {
            if(which == Player::light)
            {
                sprites.add_sprite(light_piece_sprite,
                                   physical,
                                   z_layer);

            }
            if(which == Player::dark)
            {
                sprites.add_sprite(dark_piece_sprite,
                                   physical,
                                   z_layer);
            }
        }
    }
    else
    {
        physical = screen_to_board(physical);
        if(model_.winner() == Player::neither)
        {
            physical = board_to_screen(physical);
            sprites.add_sprite(loser_sprite,
                               physical,
                               z_layer);
        }
        else
        {
            if(model_.operator[](physical) == other_player(model_.winner()))
            {
                physical = board_to_screen(physical);
                sprites.add_sprite(loser_sprite,
                                   physical,
                                   z_layer);
            }
            else
            {
                physical = board_to_screen(physical);
                if(model_.winner() == Player::light)
                {
                    sprites.add_sprite(light_piece_sprite,
                                       physical,
                                       z_layer);
                }
                if(model_.winner() == Player::dark)
                {
                    sprites.add_sprite(dark_piece_sprite,
                                       physical,
                                       z_layer);
                }

            }
        }



    }






}

void View::draw(Sprite_set& set, ge211::Posn<int> mouse_posn)
{

    for( Position piece_pos: model_.board())
    {

        set.add_sprite(board_sprite,
                      Position(piece_pos.x*grid_size,piece_pos.y*grid_size) +
                      Dimensions(1, 1), 0);


        if(piece_pos == mouse_posn)
        {

            if(model_.operator[](piece_pos) == Player::neither)
            {
                add_player_sprite_(set, model_.turn(),
                                       piece_pos, 2);
            }

        }

        if(model_.operator[](piece_pos) == Player::light)
        {
            add_player_sprite_(set, Player::light,
                                   piece_pos, 2);
        }
        if(model_.operator[](piece_pos) == Player::dark)
        {
            add_player_sprite_(set, Player::dark,
                                   piece_pos, 2);
        }

    }



}

View::Dimensions
View::initial_window_dimensions() const
{
    // You can change this if you want:
    return grid_size * model_.board().dimensions();
}

std::string
View::initial_window_title() const
{
    // You can change this if you want:
    return "Reversi";
}

