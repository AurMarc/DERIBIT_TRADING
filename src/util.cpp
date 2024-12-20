#include <iostream>
#include <sstream>
#include <iomanip>
#include "util.hpp"
#include <thread>
#include <chrono>
#include <time.h>
#include <openssl/hmac.h>
#include <openssl/evp.h>
#include "json.hpp"

#ifdef _WIN32
#include <conio.h> 
#else
#include <termios.h>
#include <unistd.h>
#endif
#include <fcntl.h>

using namespace std;
using json = nlohmann::json;

void utils::clear_console() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

int utils::getTerminalWidth() {
    struct winsize w;
    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &w) == 0) {
        return w.ws_col;
    }
    return 80; // Default width
}

void utils::printHeader() {
    int terminal_width = utils::getTerminalWidth();
    string title = "DERIBIT Backend";
    string border(terminal_width, '-');

    fmt::print(fg(fmt::color::crimson) | fmt::emphasis::bold, "{}\n", border);
    fmt::print(fg(fmt::color::white) | fmt::emphasis::bold, "{:^{}}\n", title, terminal_width);
    fmt::print(fg(fmt::color::crimson) | fmt::emphasis::bold, "{}\n", border);
    fmt::print("\n");
}

void utils::printcmd(string const &str) {
    fmt::print(fg(fmt::rgb(219, 186, 221)), str);
}

void utils::printcmd(string const &str, int r, int g, int b) {
    fmt::print(fg(fmt::rgb(r, g, b)), str);
}

void utils::printerr(string const &str) {
    fmt::print(fg(fmt::rgb(255, 83, 29)) | fmt::emphasis::bold, str);
}

long long utils::time_now() {
    auto now = chrono::system_clock::now();
    auto now_ms = chrono::time_point_cast<chrono::milliseconds>(now);
    auto epoch = now_ms.time_since_epoch();
    return epoch.count();
}

string utils::gen_random(const int len) {
    static const char alphanum[] =
        "0123456789"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz";
    string tmp_s;
    tmp_s.reserve(len);

    for (int i = 0; i < len; ++i) {
        tmp_s += alphanum[rand() % (sizeof(alphanum) - 1)];
    }
    return tmp_s;
}

string utils::to_hex_string(const unsigned char* data, unsigned int length) {
    ostringstream hex_stream;
    hex_stream << hex << uppercase << setfill('0');
    for (unsigned int i = 0; i < length; ++i) {
        hex_stream << setw(2) << static_cast<int>(data[i]);
    }
    return hex_stream.str();
}

string utils::hmac_sha256(const string& key, const string& data) {
    unsigned char result[EVP_MAX_MD_SIZE];
    unsigned int result_length = 0;

    HMAC(EVP_sha256(), key.c_str(), key.length(),
         reinterpret_cast<const unsigned char*>(data.c_str()), data.length(),
         result, &result_length);

    return utils::to_hex_string(result, result_length);
}

string utils::get_signature(long long timestamp, string nonce, string data, string clientsecret) {
    string string_to_code = to_string(timestamp) + "\n" + nonce + "\n" + data;
    return utils::hmac_sha256(clientsecret, string_to_code);
}

string utils::pretty(string j) {
    json serialised = json::parse(j);
    return serialised.dump(4);
}

string utils::printmap(map<string, string> mpp) {
    ostringstream os;
    for (const auto& pair : mpp) {
        os << pair.first << " : " << pair.second << '\n';
    }
    return os.str();
}

string utils::getPassword() {
    string password;
    char ch;

    cout << "Enter client secret: "; // Updated prompt

#ifdef _WIN32
    while ((ch = _getch()) != '\r') {
        if (ch == '\b') {
            if (!password.empty()) {
                password.pop_back();
                cout << "\b \b";
            }
        } else {
            password += ch;
            cout << '*';
        }
    }
    cout << endl;
#else
    struct termios oldt, newt;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~ECHO;
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);

    getline(cin, password);

    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    cout << endl;
#endif

    return password;
}

bool utils::is_key_pressed(char key) {
    struct termios oldt, newt;
    int ch;
    int oldf;
    
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    
    oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);

    ch = getchar();

    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    fcntl(STDIN_FILENO, F_SETFL, oldf);

    if (ch == key) {
        while (getchar() != EOF); // Clear input buffer
        return true;
    }
    return false;
}

void utils::printHelp() {
    int terminal_width = utils::getTerminalWidth();
    string separator(terminal_width, '=');

    cout << "\n" << separator << "\n"
         << fmt::format("{:^{}}\n", "COMMAND LIST", terminal_width)
         << separator << "\n\n";

    // Main Menu Options
    cout << "MAIN MENU OPTIONS:\n"
         << fmt::format("  {:<30} : {}\n", "1. Connect to Exchange", "Connect to Deribit TESTNET and authorize")
         << fmt::format("  {:<30} : {}\n", "2. Order Management", "Access order-related functions")
         << fmt::format("  {:<30} : {}\n", "3. Market Coverage", "Access market data and streaming")
         << fmt::format("  {:<30} : {}\n", "4. Performance Metrics", "View system latency statistics")
         << fmt::format("  {:<30} : {}\n", "5. Exit", "Close the application")
         << "\n";

    // Order Management Options
    cout << "ORDER MANAGEMENT OPTIONS:\n"
         << fmt::format("  {:<30} : {}\n", "Place Order", "Create new buy/sell orders")
         << fmt::format("  {:<30} : {}\n", "Cancel Order", "Cancel existing orders")
         << fmt::format("  {:<30} : {}\n", "Modify Order", "Edit existing orders")
         << fmt::format("  {:<30} : {}\n", "View Open Orders", "List all active orders")
         << fmt::format("  {:<30} : {}\n", "View Positions", "Show current positions")
         << "\n";

    // Market Coverage Options
    cout << "MARKET COVERAGE OPTIONS:\n"
         << fmt::format("  {:<30} : {}\n", "View Orderbook", "See current market depth")
         << fmt::format("  {:<30} : {}\n", "Subscribe to Symbol", "Add symbol to streaming")
         << fmt::format("  {:<30} : {}\n", "Unsubscribe from Symbol", "Remove symbol from streaming")
         << fmt::format("  {:<30} : {}\n", "View Current Subscriptions", "List active subscriptions")
         << fmt::format("  {:<30} : {}\n", "View Live Stream", "Watch real-time market data")
         << "\n";

    cout << separator << "\n\n";
}

std::string utils::format_time(const std::chrono::system_clock::time_point& time) {
    auto in_time_t = std::chrono::system_clock::to_time_t(time);
    std::stringstream ss;
    ss << std::put_time(std::localtime(&in_time_t), "%H:%M:%S");
    return ss.str();
}