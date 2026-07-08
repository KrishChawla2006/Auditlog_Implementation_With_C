#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>

#define LOGFILE "audit.log"
#define BUFFER_SIZE 1024

int main(int argc, char *argv[]) {

    if (argc < 2) {
        write(STDOUT_FILENO,
              "Usage:\n./auditlog --add \"message\"\n./auditlog --view\n",
              54);
        return 1;
    }

    // ADD 
    if (strcmp(argv[1], "--add") == 0) {

        if (argc < 3) {
            write(STDOUT_FILENO, "Message missing.\n", 17);
            return 1;
        }

        int fd = open(LOGFILE, O_WRONLY | O_CREAT | O_APPEND, 0644);

        if (fd < 0) {
            perror("open");
            return 1;
        }

        write(fd, argv[2], strlen(argv[2]));
        write(fd, "\n", 1);

        close(fd);

        return 0;
    }

    // VIEW 
    else if (strcmp(argv[1], "--view") == 0) {

        int fd = open(LOGFILE, O_RDONLY);

        if (fd < 0) {
            perror("open");
            return 1;
        }

        char buffer[BUFFER_SIZE];
        char line[BUFFER_SIZE];
        int bytesRead;

        int lineNo = 1;
        int index = 0;

        while ((bytesRead = read(fd, buffer, BUFFER_SIZE)) > 0) {

            for (int i = 0; i < bytesRead; i++) {

                if (buffer[i] == '\n') {

                    line[index] = '\0';

                    char prefix[32];
                    sprintf(prefix, "%d: ", lineNo++);

                    write(STDOUT_FILENO, prefix, strlen(prefix));
                    write(STDOUT_FILENO, line, strlen(line));
                    write(STDOUT_FILENO, "\n", 1);

                    index = 0;
                }
                else {
                    line[index++] = buffer[i];
                }
            }
        }

        close(fd);

        return 0;
    }

    // INVALID OPTION 
    else {

        write(STDOUT_FILENO,
              "Invalid option.\nUse --add or --view\n",
              36);
    }

    return 0;
}
