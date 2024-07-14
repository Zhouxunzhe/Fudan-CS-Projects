#ifndef _MYJQLCLIENT_H
#define _MYJQLCLIENT_H

/*
 * myjql-client supports windows & linux (ubuntu)
 * Windows Socket:
 * https://docs.microsoft.com/en-us/windows/win32/winsock/complete-client-code
 * 
 * Linux Socket:
 * https://www.tutorialspoint.com/unix_sockets/socket_client_example.htm
 */

#ifdef _WIN32

#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>

#endif

#include <exception>
#include <string_view>
#include <optional>

#ifdef _WIN32

// Need to link with Ws2_32.lib, Mswsock.lib, and Advapi32.lib
#pragma comment(lib, "Ws2_32.lib")
#pragma comment(lib, "Mswsock.lib")
#pragma comment(lib, "AdvApi32.lib")

#endif

#define DEFAULT_HOST "localhost"

#ifdef _WIN32
#define DEFAULT_PORT "27027"
#else
#define DEFAULT_PORT 27027
#endif

namespace MyJQL {

    struct system_error : std::exception {
        std::string_view msg_;
        int error_code_;
        system_error(std::string_view msg, int error_code)
            : msg_{msg}, error_code_{error_code} {}
        const char* what() const noexcept { return msg_.data(); }
        int code() const { return error_code_; }
    };

    struct key_too_long : std::exception {
        const char* what() const noexcept { return "key is too long"; }
    };

    struct value_too_long : std::exception {
        const char* what() const noexcept { return "value is too long"; }
    };

    class client {
    private:
#ifdef _WIN32
        WSADATA wsaData;
        SOCKET ConnectSocket;
#else
        int ConnectSocket;
#endif
    public:
        client();
        ~client();
        std::optional<std::string> get(std::string_view key);
        void set(std::string_view key, std::string_view value);
        void del(std::string_view key);
    };

}  // MyJQL

#endif  /* _MYJQLCLIENT_H */