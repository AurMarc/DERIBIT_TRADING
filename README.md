# C++ Deribit Trading System Documentation

## 1. System Overview

### Architecture Diagram
The system is designed with a modular architecture to ensure scalability and maintainability. The architecture includes the following layers:

- **Application Layer:** Handles user interaction and system commands.
- **API Integration Layer:** Manages communication with the Deribit API.
- **Core Logic Layer:** Implements trading strategies, order management, and market data processing.
- **Utility Layer:** Provides logging, error handling, and performance monitoring.

```plaintext
+-------------------+
| Application Layer |
+-------------------+
          |
+-------------------+
| API Integration   |
+-------------------+
          |
+-------------------+
| Core Logic        |
+-------------------+
          |
+-------------------+
| Utility Layer     |
+-------------------+
```

### Key Components
- **WebSocket Manager:** Handles live streaming of market data and order updates.
- **Order Management System:** Processes buy, sell, modify, and cancel orders.
- **Market Coverage Module:** Fetches and displays real-time order books and market statistics.

### Technology Stack
- **Programming Language:** C++ (Standard 17 and above)
- **Build System:** CMake
- **Libraries:** Boost, WebSocket++, nlohmann/json
- **Testing Framework:** Google Test

### Dependencies
- Boost (for threading and utilities)
- WebSocket++ (for WebSocket communication)
- nlohmann/json (for JSON parsing)
- CMake (for building and managing dependencies)

## 2. Installation & Setup

### Prerequisites
- C++ compiler supporting C++17 (e.g., GCC, Clang)
- CMake 3.14 or newer
- Boost library installed
- Network access to `wss://test.deribit.com`

### Build Instructions
1. Clone the repository:
   ```bash
   git clone <https://github.com/AurMarc/DERIBIT_TRADING>
   cd <DERIBIT_TRADING>
   ```
2. Create a build directory:
   ```bash
   mkdir build && cd build
   ```
3. Run CMake and build the project:
   ```bash
   cmake ..
   make
   ```

### Configuration
- The configuration file (`config.json`) should include API credentials and system settings:
  ```json
  {
      "client_id": "<Your_Client_ID>",
      "client_secret": "<Your_Client_Secret>",
      "test_url": "wss://test.deribit.com/ws/api/v2"
  }
  ```

### Environment Setup
Set environment variables for library paths if necessary:
```bash
export LD_LIBRARY_PATH=/path/to/boost/libs:$LD_LIBRARY_PATH
```

### API Credentials Setup
1. Create a Deribit account and generate API credentials from the dashboard.

## 3. Core Components Documentation

### WebSocket Manager
**Purpose:** Manages real-time communication with the Deribit API for live updates.

**Functionality:**
- Connects to the WebSocket endpoint.
- Subscribes to market data streams.
- Handles ping/pong for connection health.

**Key Classes/Functions:**
- `WebSocketClient::connect()` - Establishes connection.
- `WebSocketClient::subscribe()` - Subscribes to a data stream.
- `WebSocketClient::on_message()` - Callback for handling messages.

**Input/Output:**
- **Input:** Subscription requests (e.g., symbols, order types).
- **Output:** JSON-formatted real-time updates.

**Error Handling:**
- Reconnect on disconnection.
- Log errors and notify the user.

**Code Example:**
```cpp
WebSocketClient ws_client;
ws_client.connect("wss://test.deribit.com/ws/api/v2");
ws_client.subscribe("BTC-PERPETUAL");
```

**Best Practices:**
- Use async handlers for scalability.
- Maintain a ping/pong mechanism.

### Order Management System
**Purpose:** Handles order lifecycle operations.

**Functionality:**
- Places, modifies, and cancels orders.
- Monitors active orders and tracks their status.

**Key Classes/Functions:**
- `OrderManager::place_order()` - Sends a new order.
- `OrderManager::modify_order()` - Updates an existing order.
- `OrderManager::cancel_order()` - Cancels an active order.

**Input/Output:**
- **Input:** Order parameters (instrument, size, price, type).
- **Output:** Confirmation or error messages.

**Code Example:**
```cpp
OrderManager om;
om.place_order("BTC-PERPETUAL", "limit", 100, 50000);
```

### Market Coverage Module
**Purpose:** Provides detailed market data.

**Functionality:**
- Displays order books.
- Streams market updates.

**Key Classes/Functions:**
- `MarketCoverage::view_orderbook()` - Fetches the order book.
- `MarketCoverage::stream_updates()` - Streams live updates.

## 4. API Integration

### Authentication Process
1. Send a POST request to the authentication endpoint with client credentials.
2. Receive an access token for further API calls.

**Code Example:**
```cpp
APIClient api_client;
api_client.authenticate("client_id", "client_secret");
```

### Available Endpoints
- `/public/auth`
- `/private/subscribe`
- `/private/buy`
- `/private/sell`

### Request/Response Formats
- **Request:** JSON
  ```json
  {
      "method": "public/auth",
      "params": {
          "grant_type": "client_credentials",
          "client_id": "<ID>",
          "client_secret": "<Secret>"
      }
  }
  ```
- **Response:** JSON
  ```json
  {
      "result": {
          "access_token": "<Token>"
      }
  }
  ```

## 5. Performance Optimization

### Latency Measurements
- Measure order placement time.
- Log WebSocket message delays.

### Memory Management
- Use smart pointers.
- Minimize heap allocations.

### Threading Model
- Use Boost.Asio for concurrency.
- Implement thread pools for high-load scenarios.

### Network Optimization
- Batch API requests when possible.
- Compress WebSocket messages.

## 6. Troubleshooting Guide

### Common Issues
- **Connection Errors:** Check API credentials and network settings.
- **Invalid Orders:** Validate order parameters.

### Debug Procedures
- Enable verbose logging.
- Use mock data for testing.

### Logging System
- Logs stored in `logs/` directory.
- Use `--verbose` flag for detailed output.

### Error Resolution
- Refer to API error codes for details.
- Retry failed requests with exponential backoff.

---

For further assistance, contact the support team or refer to the official Deribit API documentation.
