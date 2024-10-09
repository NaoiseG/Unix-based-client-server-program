# Client-Server Quiz Program in C

This project implements a client-server program where the server hosts a quiz and the client connects to answer questions. The server and client communicate using TCP/IP sockets. The server can handle multiple clients, each interacting with the quiz, and providing feedback on the score at the end of the session.

## Features

- Server hosts a quiz with random questions for each client.
- Clients connect to the server, answer questions, and receive a final score.
- Communication between the server and client is done using TCP/IP sockets.
- Error handling for socket operations and network communication.

## Prerequisites

- A C compiler (such as `gcc`).
- A Unix-based system or Windows with WSL (Windows Subsystem for Linux) or Cygwin for Unix compatibility.

## Compilation

To compile both the server and the client, use the following commands:

For the server:
gcc -o myserver myserver.c -I.

For the client:
gcc -o myclient myclient.c -I.

## Running the programme

Run the server by providing a port number:
./myserver <port_number>

In a separate terminal, run the client and provide the server's IP address and the port number:
./myclient <server_ip_address> <port_number>

For example, if running both client and server on the same machine, use 127.0.0.1 as the IP address.

## How it works
Server: Hosts a quiz and waits for clients to connect. Once connected, the server sends quiz questions to the client, receives responses, and provides feedback with the final score.

Client: Connects to the server, answers quiz questions, and receives the final score after the quiz is complete.

## Files
myserver.c: Contains the server-side code that handles client connections and manages the quiz.

myclient.c: Contains the client-side code that communicates with the server and answers quiz questions.

quiz.h: Header file containing the quiz questions and answers used by the server.


