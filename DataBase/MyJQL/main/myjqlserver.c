/*
 * myjql-server supports windows & linux (ubuntu)
 * 
 * Windows:
 * 
 * Socket:
 * https://docs.microsoft.com/en-us/windows/win32/winsock/complete-server-code
 * 
 * Signal Handler:
 * https://docs.microsoft.com/en-us/windows/console/registering-a-control-handler-function
 * 
 * Linux:
 * 
 * Socket:
 * https://www.tutorialspoint.com/unix_sockets/socket_server_example.htm
 * 
 * Thread:
 * https://man7.org/linux/man-pages/man3/pthread_create.3.html
 * 
 * Mutex:
 * https://www.geeksforgeeks.org/mutex-lock-for-linux-thread-synchronization/
 * 
 * Signal:
 * https://www.thegeekstuff.com/2012/03/catch-signals-sample-c-code
 */

#ifdef _WIN32

#undef UNICODE

#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>

#else

#include <pthread.h>
#include <unistd.h>
#include <netdb.h>
#include <netinet/in.h>
#include <signal.h>

#endif

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "myjql.h"

#ifdef _WIN32

// Need to link with Ws2_32.lib
#pragma comment(lib, "Ws2_32.lib")
// #pragma comment(lib, "Mswsock.lib")

#define DEFAULT_PORT "27027"

SOCKET ListenSocket = INVALID_SOCKET;

CRITICAL_SECTION CriticalSection;

#else

#define DEFAULT_PORT 27027

int ListenSocket;

pthread_mutex_t CriticalSection;

#define SOCKET_ERROR -1
#define SD_SEND SHUT_WR

void closesocket(int fd) { close(fd); }

void WSACleanup() {}

int WSAGetLastError() { return 0; }

void EnterCriticalSection(pthread_mutex_t *mutex) {
    pthread_mutex_lock(mutex);
}

void LeaveCriticalSection(pthread_mutex_t *mutex) {
    pthread_mutex_unlock(mutex);
}

#endif

#define MAX_STR_LEN 1024

#ifdef _WIN32
DWORD WINAPI InstanceThread(LPVOID lpvParam)
#else
int InstanceThread(void *lpvParam);

void *thread_start(void *lpvParam)
{
    InstanceThread(lpvParam);
    return NULL;
}

int InstanceThread(void *lpvParam)
#endif
{
    int iResult;
    int code;

#ifdef _WIN32
    SOCKET ClientSocket = (SOCKET)lpvParam;
    DWORD threadId = GetCurrentThreadId();
#else
    int ClientSocket = *(int*)lpvParam;
    pthread_t threadId = pthread_self();
    free(lpvParam);
#endif

    printf("[%ld] InstanceThread created, receiving and processing messages.\n", threadId);

    for (; ; ) {
        iResult = recv(ClientSocket, (char*)&code, sizeof(code), 0);
        if (iResult == SOCKET_ERROR) {
            printf("[%ld] recv failed with error: %d\n", threadId, WSAGetLastError());
            closesocket(ClientSocket);
            return 1;
        }
        if (code == 0) {  // get key
            // recv: 0 key_len key
            // send: 0 (not found)
            //       1 value_len value
            //       2 (key too long)
            // key
            iResult = recv(ClientSocket, (char*)&code, sizeof(code), 0);
            if (iResult == SOCKET_ERROR) {
                printf("[%ld] recv failed with error: %d\n", threadId, WSAGetLastError());
                closesocket(ClientSocket);
                return 1;
            }
            char *key = malloc(code + 1);
            iResult = recv(ClientSocket, key, code, 0);
            if (iResult == SOCKET_ERROR) {
                printf("[%ld] recv failed with error: %d\n", threadId, WSAGetLastError());
                closesocket(ClientSocket);
                free(key);
                return 1;
            }
            key[code] = 0;
            printf("[%ld] get %s\n", threadId, key);

            if (code > MAX_STR_LEN) {
                // 2
                code = 2;
                iResult = send(ClientSocket, (char*)&code, sizeof(code), 0);
                if (iResult == SOCKET_ERROR) {
                    printf("[%ld] send failed with error: %d\n", threadId, WSAGetLastError());
                    closesocket(ClientSocket);
                    free(key);
                    return 1;
                }
            } else {
                char *value = malloc(MAX_STR_LEN + 1);
                int value_len;

                // Request ownership of the critical section.
                EnterCriticalSection(&CriticalSection);

                // Sleep(5000);
                value_len = (int)myjql_get(key, code, value, MAX_STR_LEN);
                printf("[%ld] myjql_get\n", threadId);

                // Release ownership of the critical section.
                LeaveCriticalSection(&CriticalSection);

                if (value_len == -1) {
                    // 0
                    code = 0;
                    iResult = send(ClientSocket, (char*)&code, sizeof(code), 0);
                    if (iResult == SOCKET_ERROR) {
                        printf("[%ld] send failed with error: %d\n", threadId, WSAGetLastError());
                        closesocket(ClientSocket);
                        free(key);
                        free(value);
                        return 1;
                    }
                } else {
                    // 1
                    code = 1;
                    iResult = send(ClientSocket, (char*)&code, sizeof(code), 0);
                    if (iResult == SOCKET_ERROR) {
                        printf("[%ld] send failed with error: %d\n", threadId, WSAGetLastError());
                        closesocket(ClientSocket);
                        free(key);
                        free(value);
                        return 1;
                    }
                
                    // printf("[%ld] ret %d\n", threadId, code);

                    // value
                    code = value_len;
                    iResult = send(ClientSocket, (char*)&code, sizeof(code), 0);
                    if (iResult == SOCKET_ERROR) {
                        printf("[%ld] send failed with error: %d\n", threadId, WSAGetLastError());
                        closesocket(ClientSocket);
                        free(key);
                        free(value);
                        return 1;
                    }
                    iResult = send(ClientSocket, value, code, 0);
                    if (iResult == SOCKET_ERROR) {
                        printf("[%ld] send failed with error: %d\n", threadId, WSAGetLastError());
                        closesocket(ClientSocket);
                        free(key);
                        free(value);
                        return 1;
                    }
                    value[code] = 0;
                    // printf("[%ld] res %s (%d)\n", threadId, value, code);
                }

                free(value);
            }

            free(key);
        } else if (code == 1) {  // set key value
            // recv: 1 key_len key value_len value
            // send: 1 (ok)
            //       2 (key too long)
            //       3 (value too long)
            // key
            iResult = recv(ClientSocket, (char*)&code, sizeof(code), 0);
            if (iResult == SOCKET_ERROR) {
                printf("[%ld] recv failed with error: %d\n", threadId, WSAGetLastError());
                closesocket(ClientSocket);
                return 1;
            }
            int key_len = code;
            char *key = malloc(code + 1);
            iResult = recv(ClientSocket, key, code, 0);
            if (iResult == SOCKET_ERROR) {
                printf("[%ld] recv failed with error: %d\n", threadId, WSAGetLastError());
                closesocket(ClientSocket);
                free(key);
                return 1;
            }
            key[code] = 0;
            // value
            iResult = recv(ClientSocket, (char*)&code, sizeof(code), 0);
            if (iResult == SOCKET_ERROR) {
                printf("[%ld] recv failed with error: %d\n", threadId, WSAGetLastError());
                closesocket(ClientSocket);
                return 1;
            }
            int value_len = code;
            char *value = malloc(code + 1);
            iResult = recv(ClientSocket, value, code, 0);
            if (iResult == SOCKET_ERROR) {
                printf("[%ld] recv failed with error: %d\n", threadId, WSAGetLastError());
                closesocket(ClientSocket);
                free(key);
                free(value);
                return 1;
            }
            value[code] = 0;
            printf("[%ld] set %s %s\n", threadId, key, value);

            if (key_len > MAX_STR_LEN) {
                // 2
                code = 2;
                iResult = send(ClientSocket, (char*)&code, sizeof(code), 0);
                if (iResult == SOCKET_ERROR) {
                    printf("[%ld] send failed with error: %d\n", threadId, WSAGetLastError());
                    closesocket(ClientSocket);
                    free(key);
                    free(value);
                    return 1;
                }
            } else if (value_len > MAX_STR_LEN) {
                // 3
                code = 3;
                iResult = send(ClientSocket, (char*)&code, sizeof(code), 0);
                if (iResult == SOCKET_ERROR) {
                    printf("[%ld] send failed with error: %d\n", threadId, WSAGetLastError());
                    closesocket(ClientSocket);
                    free(key);
                    free(value);
                    return 1;
                }
            } else {

                // Request ownership of the critical section.
                EnterCriticalSection(&CriticalSection);

                // Sleep(5000);
                myjql_set(key, key_len, value, value_len);
                printf("[%ld] myjql_set\n", threadId);

                // Release ownership of the critical section.
                LeaveCriticalSection(&CriticalSection);

                // 1
                code = 1;
                iResult = send(ClientSocket, (char*)&code, sizeof(code), 0);
                if (iResult == SOCKET_ERROR) {
                    printf("[%ld] send failed with error: %d\n", threadId, WSAGetLastError());
                    closesocket(ClientSocket);
                    free(key);
                    free(value);
                    return 1;
                }
            }

            free(key);
            free(value);
        } else if (code == 2) {  // del key
            // recv: 2 key_len key
            // send: 1 (ok)
            //       2 (key too long)
            // key
            iResult = recv(ClientSocket, (char*)&code, sizeof(code), 0);
            if (iResult == SOCKET_ERROR) {
                printf("[%ld] recv failed with error: %d\n", threadId, WSAGetLastError());
                closesocket(ClientSocket);
                return 1;
            }
            char *key = malloc(code + 1);
            iResult = recv(ClientSocket, key, code, 0);
            if (iResult == SOCKET_ERROR) {
                printf("[%ld] recv failed with error: %d\n", threadId, WSAGetLastError());
                closesocket(ClientSocket);
                free(key);
                return 1;
            }
            key[code] = 0;
            printf("[%ld] del %s\n", threadId, key);

            if (code > MAX_STR_LEN) {
                // 2
                code = 2;
                iResult = send(ClientSocket, (char*)&code, sizeof(code), 0);
                if (iResult == SOCKET_ERROR) {
                    printf("[%ld] send failed with error: %d\n", threadId, WSAGetLastError());
                    closesocket(ClientSocket);
                    free(key);
                    return 1;
                }
            } else {

                // Request ownership of the critical section.
                EnterCriticalSection(&CriticalSection);

                // Sleep(5000);
                myjql_del(key, code);
                printf("[%ld] myjql_del\n", threadId);

                // Release ownership of the critical section.
                LeaveCriticalSection(&CriticalSection);

                // 1
                code = 1;
                iResult = send(ClientSocket, (char*)&code, sizeof(code), 0);
                if (iResult == SOCKET_ERROR) {
                    printf("[%ld] send failed with error: %d\n", threadId, WSAGetLastError());
                    closesocket(ClientSocket);
                    free(key);
                    return 1;
                }
            }

            free(key);
        } else if (code == 3) {  // client exits
            // recv: 3
            // send: 1 (ok)
            code = 1;
            iResult = send(ClientSocket, (char*)&code, sizeof(code), 0);
            if (iResult == SOCKET_ERROR) {
                printf("[%ld] send failed with error: %d\n", threadId, WSAGetLastError());
                closesocket(ClientSocket);
                return 1;
            }
            break;
        }
    }

    // shutdown the connection since we're done
    iResult = shutdown(ClientSocket, SD_SEND);
    if (iResult == SOCKET_ERROR) {
        printf("[%ld] shutdown failed with error: %d\n", threadId, WSAGetLastError());
        closesocket(ClientSocket);
        return 1;
    }

    closesocket(ClientSocket);
    
    printf("[%ld] InstanceThread exiting.\n", threadId);

    return 0;
}

void exit_nicely()
{
    printf("Terminating server.\n");

    // Request ownership of the critical section.
    EnterCriticalSection(&CriticalSection);

    // Sleep(5000);
    myjql_close();
    printf("myjql_close\n");

    // Release ownership of the critical section.
    LeaveCriticalSection(&CriticalSection);

    // cleanup
    closesocket(ListenSocket);
    WSACleanup();

    // Release resources used by the critical section object.
#ifdef _WIN32
    DeleteCriticalSection(&CriticalSection);
#else
    pthread_mutex_destroy(&CriticalSection);
#endif

    exit(0);
}

#ifdef _WIN32

BOOL WINAPI CtrlHandler(DWORD fdwCtrlType)
{
    switch (fdwCtrlType)
    {
        // Handle the CTRL-C signal.
    case CTRL_C_EVENT:
        printf("Ctrl-C event\n\n");
        Beep(750, 300);
        exit_nicely();
        return TRUE;

        // CTRL-CLOSE: confirm that the user wants to exit.
    case CTRL_CLOSE_EVENT:
        Beep(600, 200);
        printf("Ctrl-Close event\n\n");
        exit_nicely();
        return TRUE;

        // Pass other signals to the next handler.
    case CTRL_BREAK_EVENT:
        Beep(900, 200);
        printf("Ctrl-Break event\n\n");
        exit_nicely();
        return FALSE;

    case CTRL_LOGOFF_EVENT:
        Beep(1000, 200);
        printf("Ctrl-Logoff event\n\n");
        exit_nicely();
        return FALSE;

    case CTRL_SHUTDOWN_EVENT:
        Beep(750, 500);
        printf("Ctrl-Shutdown event\n\n");
        exit_nicely();
        return FALSE;

    default:
        return FALSE;
    }
}

#else

void CtrlHandler(int signo)
{
    if (signo == SIGINT) {
        printf("Ctrl-C event\n\n");
        exit_nicely();
    }
}

#endif

int main(void)
{
    int iResult;

#ifdef _WIN32

    WSADATA wsaData;

    SOCKET ClientSocket = INVALID_SOCKET;

    struct addrinfo *result = NULL;
    struct addrinfo hints;

    HANDLE hThread = NULL;
    DWORD  dwThreadId = 0;

#else

    int ClientSocket;

    int clilen;
    int *arg;

    struct sockaddr_in serv_addr, cli_addr;

    pthread_t hThread;

#endif

#ifdef _WIN32

    if (!SetConsoleCtrlHandler(CtrlHandler, TRUE)) {
        printf("\nERROR: Could not set control handler");
        return 1;
    }

    // Initialize Winsock
    iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != 0) {
        printf("WSAStartup failed with error: %d\n", iResult);
        return 1;
    }

#else

    if (signal(SIGINT, CtrlHandler) == SIG_ERR) {
        printf("\nERROR: Could not set control handler");
        return 1;
    }

#endif

#ifdef _WIN32

    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    hints.ai_flags = AI_PASSIVE;

    // Resolve the server address and port
    iResult = getaddrinfo(NULL, DEFAULT_PORT, &hints, &result);
    if (iResult != 0) {
        printf("getaddrinfo failed with error: %d\n", iResult);
        WSACleanup();
        return 1;
    }

    // Create a SOCKET for connecting to server
    ListenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
    if (ListenSocket == INVALID_SOCKET) {
        printf("socket failed with error: %ld\n", WSAGetLastError());
        freeaddrinfo(result);
        WSACleanup();
        return 1;
    }

    // Setup the TCP listening socket
    iResult = bind(ListenSocket, result->ai_addr, (int)result->ai_addrlen);
    if (iResult == SOCKET_ERROR) {
        printf("bind failed with error: %d\n", WSAGetLastError());
        freeaddrinfo(result);
        closesocket(ListenSocket);
        WSACleanup();
        return 1;
    }

    freeaddrinfo(result);

    iResult = listen(ListenSocket, SOMAXCONN);
    if (iResult == SOCKET_ERROR) {
        printf("listen failed with error: %d\n", WSAGetLastError());
        closesocket(ListenSocket);
        WSACleanup();
        return 1;
    }

#else

    /* First call to socket() function */
    ListenSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (ListenSocket < 0) {
        printf("socket failed\n");
        return 1;
    }

    /* Initialize socket structure */
    bzero((char*)&serv_addr, sizeof(serv_addr));
    
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(DEFAULT_PORT);
   
   /* Now bind the host address using bind() call.*/
    iResult = bind(ListenSocket, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
    if (iResult < 0) {
        printf("bind failed\n");
        return 1;
    }

    /* Now start listening for the clients, here process will
     * go in sleep mode and will wait for the incoming connection
     */
    
    listen(ListenSocket, 128);

#endif

#ifdef _WIN32

    // Initialize the critical section one time only.
    if (!InitializeCriticalSectionAndSpinCount(&CriticalSection, 0x00000400)) {
        printf("CriticalSection myjql: InitializeCriticalSectionAndSpinCount failed\n");
        closesocket(ListenSocket);
        WSACleanup();
        return 1;
    }

#else

    // Initialize the critical section one time only.
    if (pthread_mutex_init(&CriticalSection, NULL) != 0) {
        printf("CriticalSection myjql: pthread_mutex_init failed\n");
        closesocket(ListenSocket);
        return 1;
    }

#endif

    myjql_init();
    printf("Main thread: myjql_init\n");

    for (; ; ) {
        printf("\nSocket Server: Main thread awaiting client connection\n");

#ifdef _WIN32

        // Accept a client socket
        ClientSocket = accept(ListenSocket, NULL, NULL);
        if (ClientSocket == INVALID_SOCKET) {
            printf("accept failed with error: %d\n", WSAGetLastError());
            continue;
        }

#else

        clilen = sizeof(cli_addr);
        /* Accept actual connection from the client */
        ClientSocket = accept(ListenSocket, (struct sockaddr*)&cli_addr, &clilen);
        if (ClientSocket < 0) {
            printf("accept failed\n");
            continue;
        }

#endif

#ifdef _WIN32

        // Create a thread for this client. 
        hThread = CreateThread( 
            NULL,                   // no security attribute 
            0,                      // default stack size 
            InstanceThread,         // thread proc
            (LPVOID) ClientSocket,  // thread parameter 
            0,                      // not suspended 
            &dwThreadId);           // returns thread ID 

        if (hThread == NULL) {
            printf("CreateThread failed, GLE=%d.\n", GetLastError());
            closesocket(ClientSocket);
            continue;
        } else {
            CloseHandle(hThread);
        }

#else

        arg = (int*)malloc(sizeof(int));
        *arg = ClientSocket;

        // Create a thread for this client.
        iResult = pthread_create(&hThread, NULL,
                                 &thread_start, arg);

        if (iResult != 0) {
            printf("pthread_create failed\n");
            closesocket(ClientSocket);
            free(arg);
            continue;
        }

#endif

    }

    // We won't reach this line.

    return 0;
}