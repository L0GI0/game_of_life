#include "gameOfLife.h"

int main(int argc, const char *argv[]){

	using namespace boost::program_options;

    try
    {
        options_description desc{ "Options" };
        desc.add_options()
            ("help,h", "Help screen")
            ("pi", value<float>()->default_value(3.14f), "Pi")
            ("age", value<int>()->default_value(23), "Age");

        variables_map vm;
        store(parse_command_line(argc, argv, desc), vm);
        notify(vm);

        if (vm.count("help"))
            std::cout << desc << '\n';
        else if (vm.count("age"))
            std::cout << "Age: " << vm["age"].as<int>() << '\n';
        else if (vm.count("pi"))
            std::cout << "Pi: " << vm["pi"].as<float>() << '\n';
    }
    catch (const error& ex)
    {
        std::cerr << ex.what() << '\n';
    }

	Grid grid(10, 10);

	Blinker blinker = Blinker(5, 5);

	grid.addShape(blinker);

	Glider glider = Glider (5, 5);

	grid.addShape(glider);

	grid.printTheGrid();

	grid.updateGrid();

	grid.printTheGrid();

	GameOfLife game = GameOfLife(grid);

	game.startTheGame();

	return 0;
}