#include <cstdlib>
#include <string>

class LoggerSQL {
private:
public:
    //problematicno
    static void startMySQL() {
        system("start \"\" cmd /C \"C:\\xampp\\mysql\\mysql_start.bat\"");
    }

    static void logGameAsync(const char* attacker, const char* defender) {
        std::thread([attacker, defender]() {
            LoggerSQL::logGame(attacker, defender);
            }).detach();
    }
    
    //ta query ne dela ker windows/cmd/xampp
    static void logGameSafe(const char* attacker, const char* defender) {
        std::thread([attacker, defender]() {
            std::string query = "INSERT INTO gamelog (Attacker, Defender, FightDate, FightTime) VALUES('";
            query += attacker;
            query += "', '";
            query += defender;
            query += "', CURDATE(), CURTIME());";
            
            std::string command =
                "start \"\" cmd /C \"C:\\xampp\\mysql\\bin\\mysql.exe -u root -p1234 -D Game -e \\\""
                + query +
                "\\\" & timeout /T 20\"";

            std::cout << "[Logger] Executing command:\n" << command << "\n";

            int result = system(command.c_str());

            if (result != 0) {
                std::cerr << "ERROR: LoggerSQL command failed with code " << result << "\n";
            }
            else {
                std::cout << "OK: LoggerSQL Game logged." << "\n";
            }
            }).detach();
    }
    
    //ta query ne dela ker windows/cmd/xampp
    static void logGame(const char* attacker, const char* defender) {
        std::string command = "start \"\" cmd /C \"C:\\xampp\\mysql\\bin\\mysql.exe -u root -p1234 -D Game -e \"";

        command += "INSERT INTO gamelog (Attacker, Defender, FightDate, FightTime) VALUES('";
        command += attacker;
        command += "', '";
        command += defender;
        command += "', CURDATE(), CURTIME());\"\"";

        system(command.c_str());
        std::cout << "Game Logged.\n";
    }
};