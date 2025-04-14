#include <cstdlib>
#include <string>

class LoggerSQL {
private:
public:
    static void startMySQL() {
        system("start \"\" cmd /C \"C:\\xampp\\mysql\\mysql_start.bat\"");
    }

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