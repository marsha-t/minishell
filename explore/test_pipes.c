#include <stdio.h>
#include <unistd.h>

int main() {
    int pipefd1[2]; // Pipe for communication from child1 to child2
    int pipefd2[2]; // Pipe for communication from child2 to child1

    // Create the first pipe
    if (pipe(pipefd1) == -1) {
        perror("pipe1");
        return 1;
    }

    // Create the second pipe
    if (pipe(pipefd2) == -1) {
        perror("pipe2");
        return 1;
    }

    // Fork the first child process
    pid_t pid1 = fork();
    if (pid1 == -1) {
        perror("fork1");
        return 1;
    }

    if (pid1 == 0) { // Child 1 process
        close(pipefd1[0]); // Close the read end of pipe1 in child1
        close(pipefd2[1]); // Close the write end of pipe2 in child1

        const char *message1 = "Message from child1 to child2";
        write(pipefd1[1], message1, sizeof(message1));

        char buffer[100];
        read(pipefd2[0], buffer, sizeof(buffer));
        printf("Child1 received: %s\n", buffer);

        close(pipefd1[1]);
        close(pipefd2[0]);
    } else { // Parent process
        // Fork the second child process
        pid_t pid2 = fork();
        if (pid2 == -1) {
            perror("fork2");
            return 1;
        }

        if (pid2 == 0) { // Child 2 process
            close(pipefd1[1]); // Close the write end of pipe1 in child2
            close(pipefd2[0]); // Close the read end of pipe2 in child2

            char buffer[100];
            read(pipefd1[0], buffer, sizeof(buffer));
            printf("Child2 received: %s\n", buffer);

            const char *message2 = "Message from child2 to child1";
            write(pipefd2[1], message2, sizeof(message2));

            close(pipefd1[0]);
            close(pipefd2[1]);
        } else { // Parent process
            close(pipefd1[0]); // Close read end of pipe1 in parent
            close(pipefd1[1]); // Close write end of pipe1 in parent
            close(pipefd2[0]); // Close read end of pipe2 in parent
            close(pipefd2[1]); // Close write end of pipe2 in parent

            // Wait for both child processes to finish
            wait(NULL);
            wait(NULL);
        }
    }

    return 0;
}
