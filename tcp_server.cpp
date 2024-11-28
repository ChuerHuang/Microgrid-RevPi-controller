#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <cstring>

int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    char buffer[1024] = {0};
    const char *hello = "Hello from RevPi server";

    // Create socket file descriptor
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == 0) {
        std::cerr << "Socket creation error" << std::endl;
        return -1;
    }

    // Bind the socket to an IP/port
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;  // Bind to any available network interface
    address.sin_port = htons(8080);        // Port number to listen on

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        std::cerr << "Bind failed" << std::endl;
        close(server_fd);
        return -1;
    }

    // Listen for incoming connections
    if (listen(server_fd, 3) < 0) {
        std::cerr << "Listen failed" << std::endl;
        close(server_fd);
        return -1;
    }
    std::cout << "Server is listening on port 8080" << std::endl;

    // Accept incoming connections and communicate
    while (true) {
        new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen);
        if (new_socket < 0) {
            std::cerr << "Accept failed" << std::endl;
            continue;
        }
        std::cout << "Connection accepted" << std::endl;

        ////Reading part

        // Read data sent by client
        // int valread = read(new_socket, buffer, 1024);
        // std::cout << "Received from client: " << buffer << std::endl;

        // Read data(floating type) sent by client
        float received_numbers[2];
        int valread = read(new_socket, received_numbers, sizeof(received_numbers));
        if (valread == sizeof(received_numbers)) {
            float num1 = received_numbers[0];
            float num2 = received_numbers[1];
            std::cout << "Received floats: " << num1 << " and " << num2 << std::endl;

            //// Control part
            // Calculate the difference of consumption and generation
            float differ = num1 - num2;  // Use signed integers for differ, since it can be negative
            float contr;  // Control value (signed for handling both cases)
            // Determine the control value and status
            if (differ > 0) {
                contr = 1;          // Overgeneration
            } else if (differ < 0) {
                contr = 2;         // Overconsumption
                
            } else {
                contr = 3;          // Balance   
            }
            ////

            // sending data(float type back to matlab)
            // Prepare the results to send back (send control and difference)
            float response[2] = {contr, differ};  // Control and Difference (as floats)

            send(new_socket, response, sizeof(response), 0);  // Send numerical results

            std::cout << "Sent results - Control: " << contr << ", Difference: " << differ << std::endl;

            

        } else {
            std::cerr << "Error: Received unexpected number of bytes (" << valread << ")" << std::endl;
        }

 
        //// Sending part

        // Send a response back to the client
        // send(new_socket, hello, strlen(hello), 0);
        // std::cout << "Hello message sent to client" << std::endl;

        // Close the connection to the client
        close(new_socket);
        //std::fill(std::begin(buffer), std::end(buffer), 0); // Clear buffer for next message
        std::fill(std::begin(received_numbers), std::end(received_numbers), 0); // Clear buffer for next message
        



        
    }

    // Close the server socket (won't reach here in this example, but good practice)
    close(server_fd);
    return 0;
}