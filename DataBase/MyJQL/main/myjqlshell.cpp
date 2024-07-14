#define _CRT_SECURE_NO_WARNINGS

#include "myjqlclient.h"

#include <cstdio>
#include <cstring>

#define INPUT_BUFFER_SIZE 1024

struct {
  char buffer[INPUT_BUFFER_SIZE + 1];
  size_t length;
} input_buffer;

char command[INPUT_BUFFER_SIZE + 1];
char key[INPUT_BUFFER_SIZE + 1];
char value[INPUT_BUFFER_SIZE + 1];

typedef enum {
  INPUT_SUCCESS,
  INPUT_TOO_LONG
} InputResult;

void print_prompt() { printf("myjql> "); }

InputResult read_input() {
    /* read the entire line as the input */
    input_buffer.length = 0;
    while (input_buffer.length <= INPUT_BUFFER_SIZE
        && (input_buffer.buffer[input_buffer.length++] = getchar()) != '\n'
        && input_buffer.buffer[input_buffer.length - 1] != EOF);
    if (input_buffer.buffer[input_buffer.length - 1] == EOF)
        exit(EXIT_SUCCESS);
    input_buffer.length--;
    /* if the last character is not new-line, the input is considered too long,
        the remaining characters are discarded */
    if (input_buffer.length == INPUT_BUFFER_SIZE
        && input_buffer.buffer[input_buffer.length] != '\n') {
        while (getchar() != '\n');
        return INPUT_TOO_LONG;
    }
    input_buffer.buffer[input_buffer.length] = 0;
    return INPUT_SUCCESS;
}

int main(void)
{
    MyJQL::client client;

    for (; ; ) {
        print_prompt();

        if (read_input() == INPUT_TOO_LONG) {
            printf("Input is too long.\n");
            continue;
        }

        if (strcmp(input_buffer.buffer, "exit") == 0) {
            printf("bye~\n");
            break;
        }

        if (sscanf(input_buffer.buffer, "%s", command) != 1) {
            printf("Invalid input.\n");
            continue;
        }

        if (strcmp(command, "get") == 0) {
            // get key
            if (sscanf(input_buffer.buffer, "%*s %s", key) != 1) {
                printf("Invalid input.\n");
                continue;
            }
            // printf("get %s\n", key);
            try {
                auto res = client.get(key);
                if (res.has_value()) {
                    printf("%s\n", res.value().data());
                } else {
                    printf("(not found)\n");
                }
            } catch (std::exception& e) {
                printf("Error: %s\n", e.what());
            }
        } else if (strcmp(command, "set") == 0) {
            // set key value
            if (sscanf(input_buffer.buffer, "%*s %s %s", key, value) != 2) {
                printf("Invalid input.\n");
                continue;
            }
            // printf("set %s %s\n", key, value);
            try {
                client.set(key, value);
                printf("OK\n");
            } catch (std::exception& e) {
                printf("Error: %s\n", e.what());
            }
        } else if (strcmp(command, "del") == 0) {
            // del key
            if (sscanf(input_buffer.buffer, "%*s %s", key) != 1) {
                printf("Invalid input.\n");
                continue;
            }
            // printf("del %s\n", key);
            try {
                client.del(key);
                printf("OK\n");
            } catch (std::exception& e) {
                printf("Error: %s\n", e.what());
            }
        } else {
            printf("Invalid command.\n");
        }
    }

    return 0;
}