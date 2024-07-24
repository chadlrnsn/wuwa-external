#include <pch.h>
#include <Helper/Utils.h> // wwlib
#include <Logger/Logger.h> // wwlib

int main() {

#ifdef _DEBUG
    LOG_DEBUG("Due to debug build all logs will be saved to " + Utils::generateLogDirectory());
#endif // _DEBUG
    
    /*
    * Check if the game is running
	* If not, start the game
    */

	if (!Utils::isProcessRunning(L"Client-Win64-Shipping.exe")) {
		//Utils::startGame();
        LOG_DEBUG("Game started %s", "!");
	}


    std::cin.get();
	return 0;
}