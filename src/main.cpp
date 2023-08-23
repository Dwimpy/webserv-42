#include "../include/webserv.hpp"

const int SERVER_PORT = 8080;

int read_input(std::string input) {
    int				std_in;
    std::string 	line;
    char 			buf[100];

    std_in = open("/dev/stdin", O_RDONLY);
    if (std_in < 0) {
        return (1);
    }
    read(std_in, &buf, sizeof(buf));
    input += buf;
    close(std_in);
    input = input.substr(0, input.find('\n'));
    if (input == "exit")
        return 1;
    return (EXIT_SUCCESS);
}

int main() {
//    int fd = open("example.txt", O_RDONLY);
//    if (fd == -1) {
//        perror("open");
//        return 1;
//    }
//    if (fcntl(fd, F_SETFL, O_NONBLOCK, FD_CLOEXEC) == -1)
//    {
//        perror("fcntl");
//        return 1;
//    }

    // Set O_NONBLOCK flag
//    int flags = fcntl(fd, F_GETFL, 0);
//    if (flags == -1) {
//        perror("fcntl");
//        return 1;
//    }
//
//    flags |= O_NONBLOCK;
//    if (fcntl(fd, F_SETFL, flags) == -1) {
//        perror("fcntl");
//        return 1;
//    }
//
//    // Set FD_CLOEXEC flag
//    int current_flags = fcntl(fd, F_GETFD);
//    if (current_flags == -1) {
//        perror("fcntl");
//        return 1;
//    }
//
//    current_flags |= FD_CLOEXEC;
//    if (fcntl(fd, F_SETFD, current_flags) == -1) {
//        perror("fcntl");
//        return 1;
//    }

    // Now the file descriptor has both O_NONBLOCK and FD_CLOEXEC flags set.

//    close(fd);


    int sockfd, newsockfd;
    struct sockaddr_in serv_addr, cli_addr;
    socklen_t clilen;
    char buffer[1024];

    // Create a socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("ERROR opening socket");
        return 1;
    }

    // Prepare the server address structure
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(SERVER_PORT);

    // Bind the socket to the server address
    if (bind(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("ERROR on binding");
        return 1;
    }

    // Start listening for incoming connections
    listen(sockfd, 5);
    std::string input;
    while (true) {
        clilen = sizeof(cli_addr);

        // Accept a new connection
        newsockfd = accept(sockfd, (struct sockaddr*)&cli_addr, &clilen);
        if (newsockfd < 0) {
            perror("ERROR on accept");
            continue;
        }

        // Read the request from the client
        memset(buffer, 0, sizeof(buffer));
        int bytes_received = recv(newsockfd, buffer, sizeof(buffer) - 1, 0);
        if (bytes_received < 0) {
            perror("ERROR reading from socket");
            close(newsockfd);
            continue;
        }

        // Send a simple HTTP response
        const char* response = "HTTP/1.0 200 OK\r\n"
                               "Content-Type: text/html\r\n"
                               "\r\n"
                               "<html><body><h1>WRONG! Start again!</h1></body></html>";
        send(newsockfd, response, strlen(response), 0);

        // Close the client socket
        close(newsockfd);

        if (read_input(input))
            break;
    }

    // Close the server socket
    close(sockfd);

    return 0;
}
