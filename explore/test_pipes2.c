#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

// Structure for the command node in the AST
typedef struct CommandNode {
    char *command; // Command to execute
    struct CommandNode *left; // Left child (command or pipe)
    struct CommandNode *right; // Right child (command or pipe)
} CommandNode;

// Function to execute a single command
void execute_command(char *command) {
    // Execute the command using execvp() or another exec() function
    // Handle errors if the command execution fails
    system(command);
}

// Function to execute the AST
void execute_ast(CommandNode *root) {
    if (root == NULL) {
        return;
    }

    // If the root is a command, execute it
    if (root->left == NULL && root->right == NULL) {
        execute_command(root->command);
    } else if (root->left != NULL && root->right != NULL) { // If the root is a pipe node
        int pipefd[2];
        if (pipe(pipefd) == -1) {
            perror("pipe");
            exit(EXIT_FAILURE);
        }

        pid_t pid_left, pid_right;
        pid_left = fork();

        if (pid_left == -1) {
            perror("fork");
            exit(EXIT_FAILURE);
        } else if (pid_left == 0) { // Child process for the left command
            close(pipefd[0]); // Close unused read end of pipe

            // Redirect standard output to the write end of the pipe
            if (dup2(pipefd[1], STDOUT_FILENO) == -1) {
                perror("dup2");
                exit(EXIT_FAILURE);
            }

            // Close the write end of the pipe
            close(pipefd[1]);

            // Execute the left command
            execute_ast(root->left);
            exit(EXIT_SUCCESS);
        } else { // Parent process
            close(pipefd[1]); // Close unused write end of pipe

            pid_right = fork();

            if (pid_right == -1) {
                perror("fork");
                exit(EXIT_FAILURE);
            } else if (pid_right == 0) { // Child process for the right command
                // Redirect standard input to the read end of the pipe
                if (dup2(pipefd[0], STDIN_FILENO) == -1) {
                    perror("dup2");
                    exit(EXIT_FAILURE);
                }

                // Close the read end of the pipe
                close(pipefd[0]);

                // Execute the right command
                execute_ast(root->right);
                exit(EXIT_SUCCESS);
            } else { // Parent process
                // Close both ends of the pipe
                close(pipefd[0]);
                close(pipefd[1]);

                // Wait for both child processes to finish
                waitpid(pid_left, NULL, 0);
                waitpid(pid_right, NULL, 0);
            }
        }
    } else {
        // Invalid AST structure
        fprintf(stderr, "Invalid AST structure\n");
        exit(EXIT_FAILURE);
    }
}

// Function to free the memory allocated for the AST
void free_ast(CommandNode *root) {
    if (root == NULL) {
        return;
    }
    free_ast(root->left);
    free_ast(root->right);
    free(root->command);
    free(root);
}

int main() {
    // Example AST construction (you need to implement your AST parsing logic)
    CommandNode *command1 = malloc(sizeof(CommandNode));
    command1->command = "echo abc";
    
    CommandNode *command2 = malloc(sizeof(CommandNode));
    command2->command = "echo def";
    
    CommandNode *command3 = malloc(sizeof(CommandNode));
    command3->command = "echo ghi";
    
    CommandNode *pipe1 = malloc(sizeof(CommandNode));
    pipe1->command = "|";
    pipe1->left = command1;
    pipe1->right = command2;

    CommandNode *pipe2 = malloc(sizeof(CommandNode));
    pipe2->command = "|";
    pipe2->left = pipe1;
    pipe2->right = command3;

    // Execute the AST
    execute_ast(pipe2);

    // Free the memory allocated for the AST
    free_ast(pipe2);

    return 0;
}
