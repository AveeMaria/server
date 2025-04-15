#include <cstdlib>
#include <string>

class LoggerSQL {
private:
	static std::string url;
public:
    static void logGameSafe(const char* attacker, const char* defender) {
        std::thread([attacker, defender]() {

            std::string command = "curl \"" + url;
            command += "?attacker=";
            command += attacker;
            command += "&defender=";
            command += defender;
            command += "\"";

            //std::cout << "command: " << command.c_str() << "\n";
        
            int result = system(command.c_str());
            }).detach();
    }

    static void getUrlFromFile() {
		std::ifstream file("../../../logger_url.txt");
		if (file.is_open()) {
			std::getline(file, url);
			file.close();
		}
		else {
			std::cerr << "Unable to open file\n";
		}
    }
};