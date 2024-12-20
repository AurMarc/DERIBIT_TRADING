#pragma once

#include <iostream>
#include <fmt/color.h>
#include <fmt/core.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <map>
#include <mutex>
#include <chrono>


namespace utils {
    long long time_now();
    std::string gen_random(const int len);
    std::string get_signature(long long timestamp, std::string nonce, 
                            std::string data, std::string clientsecret);
    std::string to_hex_string(const unsigned char* data, unsigned int length);
    std::string hmac_sha256(const std::string& key, const std::string& data);
    std::string pretty(std::string j);
    std::string printmap(std::map<std::string, std::string> mpp);
    std::string getPassword();
    void printcmd(std::string const &str);
    void printcmd(std::string const &str, int r, int g, int b);
    void printerr(std::string const &str);
    int getTerminalWidth();
    void printHeader();
    void printHelp();
    void clear_console();
    bool is_key_pressed(char key);
    bool check_key_pressed(char key);
    std::string format_time(const std::chrono::system_clock::time_point& time);
}