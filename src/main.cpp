#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <fmt/color.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "websocket.hpp"
#include "api.hpp"
#include "util.hpp"

#include "tracker.hpp"

using namespace std;

enum class MenuOption {
    CONNECT,
    ORDER_MANAGEMENT,
    MARKET_COVERAGE,
    HELP,
    PERFORMANCE_METRICS,
    EXIT
};

enum class OrderManagementOption {
    PLACE_ORDER,
    CANCEL_ORDER,
    MODIFY_ORDER,
    VIEW_ORDERS,
    VIEW_POSITIONS,
    BACK
};

enum class MarketCoverageOption {
    VIEW_ORDERBOOK,
    SUBSCRIBE_SYMBOL,
    UNSUBSCRIBE_SYMBOL,
    VIEW_SUBSCRIPTIONS,
    VIEW_STREAM,
    BACK
};
void displayMainMenu();
void displayOrderManagementMenu();
void displayMarketCoverageMenu();

void handleOrderManagement(websocket_endpoint& endpoint, int connection_id);
void handleMarketCoverage(websocket_endpoint& endpoint, int connection_id);


void displayMainMenu() {
    utils::clear_console();
    fmt::print(fg(fmt::color::cyan) | fmt::emphasis::bold,
        "\nMain Menu:\n"
        "1. Connect to Exchange\n"
        "2. Order Management\n"
        "3. Market Coverage\n"
        "4. Help\n"
        "5. Performance Metrics\n"            
        "6. Exit\n\n"
        "Select an option (or press 'h' for help): ");
}

void displayOrderManagementMenu() {
    utils::clear_console();
    fmt::print(fg(fmt::color::green) | fmt::emphasis::bold,
        "\nOrder Management:\n"
        "1. Place Order (Buy/Sell)\n"
        "2. Cancel Order\n"
        "3. Modify Order\n"
        "4. View Open Orders\n"
        "5. View Positions\n"
        "6. Back to Main Menu\n\n"
        "Select an option: ");
}

void displayMarketCoverageMenu() {
    utils::clear_console();
    fmt::print(fg(fmt::color::yellow) | fmt::emphasis::bold,
        "\nMarket Coverage:\n"
        "1. View Orderbook\n"
        "2. Subscribe to Symbol\n"
        "3. Unsubscribe from Symbol\n"
        "4. View Current Subscriptions\n"
        "5. View Live Stream\n"
        "6. Back to Main Menu\n\n"
        "Select an option: ");
}

void handleOrderManagement(websocket_endpoint& endpoint, int connection_id) {
    bool back_to_main = false;
    while (!back_to_main) {
        displayOrderManagementMenu();
        int order_choice;
        cin >> order_choice;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        OrderManagementOption order_option = static_cast<OrderManagementOption>(order_choice - 1);

        switch (order_option) {
            case OrderManagementOption::PLACE_ORDER: {
                utils::clear_console();
                fmt::print(fg(fmt::color::green) | fmt::emphasis::bold,
                    "Order Type:\n"
                    "1. Buy\n"
                    "2. Sell\n"
                    "Select: ");
                
                int type_choice;
                cin >> type_choice;
                cin.ignore(numeric_limits<streamsize>::max(), '\n');

                fmt::print(fg(fmt::color::cyan), "\nEnter instrument name (e.g., BTC-PERPETUAL): ");
                string instrument;
                getline(cin, instrument);

                fmt::print(fg(fmt::color::cyan), "Enter label for the order: ");
                string label;
                getline(cin, label);

                string command = "Deribit " + to_string(connection_id) + " " +
                               (type_choice == 1 ? "buy " : "sell ") +
                               instrument + " " + label;

                string msg = api::process(command);
                if (!msg.empty()) {
                    endpoint.send(connection_id, msg);
                }
                break;
            }

            case OrderManagementOption::CANCEL_ORDER: {
                utils::clear_console();
                fmt::print(fg(fmt::color::cyan), "Enter order ID to cancel: ");
                string order_id;
                getline(cin, order_id);

                string command = "Deribit " + to_string(connection_id) + " cancel " + order_id;
                string msg = api::process(command);
                if (!msg.empty()) {
                    endpoint.send(connection_id, msg);
                }
                break;
            }

            case OrderManagementOption::MODIFY_ORDER: {
                utils::clear_console();
                fmt::print(fg(fmt::color::cyan), "Enter order ID to modify: ");
                string order_id;
                getline(cin, order_id);

                string command = "Deribit " + to_string(connection_id) + " modify " + order_id;
                string msg = api::process(command);
                if (!msg.empty()) {
                    endpoint.send(connection_id, msg);
                }
                break;
            }

            case OrderManagementOption::VIEW_ORDERS: {
                utils::clear_console();
                fmt::print(fg(fmt::color::cyan),
                    "View Options:\n"
                    "1. All Open Orders\n"
                    "2. Orders by Instrument\n"
                    "3. Orders by Currency\n"
                    "Select: ");

                int view_choice;
                cin >> view_choice;
                cin.ignore(numeric_limits<streamsize>::max(), '\n');

                string command = "Deribit " + to_string(connection_id) + " get_open_orders";
                
                if (view_choice == 2) {
                    fmt::print(fg(fmt::color::cyan), "Enter instrument: ");
                    string instrument;
                    getline(cin, instrument);
                    command += " " + instrument;
                }
                else if (view_choice == 3) {
                    fmt::print(fg(fmt::color::cyan), "Enter currency: ");
                    string currency;
                    getline(cin, currency);
                    command += " " + currency;
                }

                string msg = api::process(command);
                if (!msg.empty()) {
                    endpoint.send(connection_id, msg);
                }
                break;
            }

            case OrderManagementOption::VIEW_POSITIONS: {
                utils::clear_console();
                string command = "Deribit " + to_string(connection_id) + " positions";
                string msg = api::process(command);
                if (!msg.empty()) {
                    endpoint.send(connection_id, msg);
                }
                break;
            }

            case OrderManagementOption::BACK:
                back_to_main = true;
                break;

            default:
                fmt::print(fg(fmt::color::red) | fmt::emphasis::bold,
                    "> Invalid option! Please try again.\n");
                break;
        }

        if (!back_to_main) {
            utils::printcmd("\nPress Enter to continue...");
            cin.get();
        }
    }
}

void handleMarketCoverage(websocket_endpoint& endpoint, int connection_id) {
    bool back_to_main = false;
    while (!back_to_main) {
        displayMarketCoverageMenu();
        int market_choice;
        cin >> market_choice;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        MarketCoverageOption market_option = static_cast<MarketCoverageOption>(market_choice - 1);

        switch (market_option) {
            case MarketCoverageOption::VIEW_ORDERBOOK: {
                utils::clear_console();
                fmt::print(fg(fmt::color::cyan), "Enter instrument name: ");
                string instrument;
                getline(cin, instrument);

                string command = "Deribit " + to_string(connection_id) + " orderbook " + instrument;
                string msg = api::process(command);
                if (!msg.empty()) {
                    endpoint.send(connection_id, msg);
                }
                break;
            }

            case MarketCoverageOption::SUBSCRIBE_SYMBOL: {
                utils::clear_console();
                fmt::print(fg(fmt::color::cyan), "Enter symbol to subscribe (e.g., BTC_USD): ");
                string symbol;
                getline(cin, symbol);

                string command = "Deribit " + to_string(connection_id) + " subscribe " + symbol;
                string msg = api::process(command);
                if (!msg.empty()) {
                    endpoint.send(connection_id, msg);
                }
                break;
            }

            case MarketCoverageOption::UNSUBSCRIBE_SYMBOL: {
                utils::clear_console();
                fmt::print(fg(fmt::color::cyan), "Enter symbol to unsubscribe: ");
                string symbol;
                getline(cin, symbol);

                string command = "Deribit " + to_string(connection_id) + " unsubscribe " + symbol;
                string msg = api::process(command);
                if (!msg.empty()) {
                    endpoint.send(connection_id, msg);
                }
                break;
            }

            case MarketCoverageOption::VIEW_SUBSCRIPTIONS: {
                utils::clear_console();
                vector<string> connections = api::getSubscription();
                if (!connections.empty()) {
                    fmt::print(fg(fmt::color::green) | fmt::emphasis::bold, 
                        "> Current Subscriptions:\n");
                    for (const auto& connection : connections) {
                        size_t prefix_pos = connection.find("deribit_price_index.");
                        if (prefix_pos != string::npos) {
                            string index_name = connection.substr(prefix_pos + strlen("deribit_price_index."));
                            fmt::print(fg(fmt::color::green), " - {}\n", index_name);
                        }
                    }
                } else {
                    fmt::print(fg(fmt::color::yellow), 
                        "> No active subscriptions\n");
                }
                break;
            }

            case MarketCoverageOption::VIEW_STREAM: {
                utils::clear_console();
                vector<string> connections = api::getSubscription();
                if (!connections.empty()) {
                    endpoint.streamSubscriptions(connections);
                } else {
                    fmt::print(fg(fmt::color::red) | fmt::emphasis::bold,
                        "> No subscriptions to stream\n");
                }
                break;
            }

            case MarketCoverageOption::BACK:
                back_to_main = true;
                break;

            default:
                fmt::print(fg(fmt::color::red) | fmt::emphasis::bold,
                    "> Invalid option! Please try again.\n");
                break;
        }

        if (!back_to_main) {
            utils::printcmd("\nPress Enter to continue...");
            cin.get();
        }
    }
}

int main() {
    websocket_endpoint endpoint;
    int active_connection_id = -1;
    bool done = false;

    utils::printHeader();

    while (!done) {
        displayMainMenu();
        
        char input = cin.get();
        if (input == 'h' || input == 'H') {
            utils::clear_console();
            utils::printHelp();
            utils::printcmd("\nPress Enter to continue...");
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }
        cin.unget();

        int choice;
        cin >> choice;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        MenuOption option = static_cast<MenuOption>(choice - 1);

        switch (option) {
            case MenuOption::CONNECT: {
            utils::clear_console();
            const string uri = "wss://test.deribit.com/ws/api/v2";
            active_connection_id = endpoint.connect(uri);

            if (active_connection_id != -1) {
                fmt::print(fg(fmt::color::green) | fmt::emphasis::bold,
                    "> Successfully connected to Deribit TESTNET.\n"
                    "> Connection ID: {}\n\n", active_connection_id);  

                fmt::print(fg(fmt::color::cyan), "Would you like to authorize now? (y/n): ");
                char auth_choice;
                cin >> auth_choice;
                cin.ignore(numeric_limits<streamsize>::max(), '\n');

                if (tolower(auth_choice) == 'y') {
                    string client_id, client_secret;
                    
                    fmt::print(fg(fmt::color::cyan), "Enter client ID: ");
                    getline(cin, client_id);

                    client_secret = utils::getPassword();

                    fmt::print(fg(fmt::color::cyan), "\nSave access token for this session? (y/n): ");
                    char save_choice;
                    cin >> save_choice;
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');

                    string command = "Deribit " + to_string(active_connection_id) + 
                                " authorize " + client_id + " " + client_secret;
                    
       
                    if (tolower(save_choice) == 'y') {
                        command += " -s";
                    }

                    string msg = api::process(command);
                    if (!msg.empty()) {
                        endpoint.send(active_connection_id, msg);
                        fmt::print(fg(fmt::color::green) | fmt::emphasis::bold,
                            "> Authorization request sent.\n");
                    }
                }
            } else {
                fmt::print(fg(fmt::color::red) | fmt::emphasis::bold,
                    "> Failed to connect to Deribit TESTNET.\n");
            }
            
            utils::printcmd("Press Enter to continue...");
            cin.get();
            break;
        }

            case MenuOption::ORDER_MANAGEMENT: {
                if (active_connection_id == -1) {
                    fmt::print(fg(fmt::color::red) | fmt::emphasis::bold,
                        "> Please connect to exchange first!\n");
                    utils::printcmd("Press Enter to continue...");
                    cin.get();
                    break;
                }
                handleOrderManagement(endpoint, active_connection_id);
                break;
            }

            case MenuOption::MARKET_COVERAGE: {
                if (active_connection_id == -1) {
                    fmt::print(fg(fmt::color::red) | fmt::emphasis::bold,
                        "> Please connect to exchange first!\n");
                    utils::printcmd("Press Enter to continue...");
                    cin.get();
                    break;
                }
                handleMarketCoverage(endpoint, active_connection_id);
                break;
            }

            case MenuOption::PERFORMANCE_METRICS: {
                utils::clear_console();
                cout << getLatencyTracker().generate_report() << endl;
                utils::printcmd("Press Enter to continue...");
                cin.get();
                break;
            }


            case MenuOption::HELP:
                utils::clear_console();
                utils::printHelp();
                utils::printcmd("Press Enter to continue...");
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                break;

            case MenuOption::EXIT:
                done = true;
                break;

            default:
                fmt::print(fg(fmt::color::red) | fmt::emphasis::bold,
                    "> Invalid option! Please try again.\n");
                utils::printcmd("Press Enter to continue...");
                cin.get();
                break;
        }
    }
    return 0;
}