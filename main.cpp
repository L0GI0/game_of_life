#include "gameOfLife.h"

int main(int argc, const char *argv[]){

	using namespace boost::program_options;

    int gridHeight = 10;
    int gridWidth = 10;
    float speedOfSimulation = 3000;
    std::string RLFile = "";

    try{
        options_description desc{ "Options" };
        desc.add_options()
            ("help,h", "Help screen")
            ("width,w", value<int>()->default_value(10), "Board width")
            ("height,h", value<int>()->default_value(10), "Board height")
            ("speed,s", value<float>()->default_value(1500), "Speed of simulation in milliseconds")
            ("init_file, initf,", value<std::string>(), "RL File containing starting board")
            ("config,c", value<std::string>(), "Config file");

        variables_map vm;
        store(parse_command_line(argc, argv, desc), vm);

        notify(vm);


        if (vm.count("init_file")) {
            RLFile = vm["init_file"].as<std::string>();
        }
        else {
            if (vm.count("config")) {
                std::ifstream ifs{ vm["config"].as<std::string>().c_str() };
                if (ifs)
                    store(parse_config_file(ifs, desc), vm);
            }

            notify(vm);

            if (vm.count("help"))
                std::cout << desc << '\n';
            if (vm.count("height")) {
                if (gridHeight >= 5 && gridHeight <= 1000) {
                    gridHeight = vm["height"].as<int>();
                    std::cout << "Grid height: " << vm["height"].as<int>() << '\n';
                }
                else
                    std::cout << "Invalid grid height" << '\n';
            }
            if (vm.count("width")) {
                if (gridWidth >= 5 && gridWidth <= 100) {
                    gridWidth = vm["height"].as<int>();
                    std::cout << "Grid width: " << vm["width"].as<int>() << '\n';
                }
                else
                    std::cout << "Invalid grid width" << '\n';

            }
            if (vm.count("speed")) {
                if (speedOfSimulation >= 1000 && speedOfSimulation <= 10000) {
                    speedOfSimulation = vm["speed"].as<float>();
                    std::cout << "Speed of simulation: " << vm["speed"].as<float>() << '\n';
                }
                else
                    std::cout << "Invalid speed of simulation: " << vm["speed"].as<float>() << '\n';
            }
        }
    }catch (const error& ex){
        std::cerr << ex.what() << '\n';
    }

    Grid grid(10, 10);

    GameOfLife game = GameOfLife(grid);

    if (RLFile.compare("") != 0) {
        game.loadFromFile(RLFile);
    }
    else {

        std::cout << "Starting the game with default grid due to lack of init file" << std::endl;

        Blinker blinker = Blinker(2, 0);

        grid.addShape(blinker);

        Glider glider = Glider(5, 5);

        grid.addShape(glider);
    }

    game.startTheGame(speedOfSimulation);


	return 0;
}