from lab1 import command_handler


if __name__ == "__main__":
    while True:
        command = input("Enter a command ('quit' to exit): ").strip()
        if command == "":
            continue
        if command.lower() == 'quit':
            print("Exiting the program.")
            break
        try:
            command_handler.execute(command)
        except Exception as e:
            print(f"An error occurred: {e}")
