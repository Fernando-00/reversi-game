#include "controller.hxx"

Controller::Controller(int size)
        : Controller(size, size)
{ }

Controller::Controller(int width, int height)
        : model_(width, height),
          view_(model_),
          mouse_posn(3,3)
{ }

void
Controller::on_mouse_move(ge211::Posn<int> mouse)
{

   mouse_posn = view_.screen_to_board(mouse);

}


void
Controller::on_mouse_up(ge211::Mouse_button, ge211::Posn<int> mouse)
{

    const Move* moves = model_.find_move(mouse_posn);

    if(moves != nullptr)
    {
        model_.play_move(mouse_posn);
    }

}

void
Controller::draw(ge211::Sprite_set& sprites)
{

    view_.draw(sprites, mouse_posn);
}

View::Dimensions
Controller::initial_window_dimensions() const
{
    return view_.initial_window_dimensions();
}

std::string
Controller::initial_window_title() const
{
    return view_.initial_window_title();
}

