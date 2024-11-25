#include <iostream>
#include <string>
#include <thread>
#include <vector>
#include <sstream>
#include <asio.hpp>
#include "Board.h"
#include "Bot.h"

using asio::ip::tcp;

void handleClient(tcp::socket socket, ChessBot& bot) {
    try {
        asio::streambuf buffer;
        asio::read_until(socket, buffer, "\n");
        std::istream input(&buffer);
        std::string request;
        std::getline(input, request);

        // Parse request (assuming JSON input)
        std::istringstream requestStream(request);
        std::string playerStr;
        Board board;
        tileState player;

        // Parse player and board state (placeholder logic, replace with actual parsing)
        if (requestStream >> playerStr) {
            player = (playerStr == "WHITE") ? WHITE : BLACK;
        }
        board.fillStandard(); // Replace with parsed board state

        // Find the best move
        auto result = bot.findBestMove(&board, player);

        // Send the result back as JSON
        std::ostringstream responseStream;
        responseStream << "{ \"bestMove\": \"" << result.first.getPieceType() << "\", "
                       << "\"score\": " << result.second << " }";
        std::string response = responseStream.str() + "\n";

        asio::write(socket, asio::buffer(response));
    } catch (std::exception& e) {
        std::cerr << "Error handling client: " << e.what() << std::endl;
    }
}

void startServer(unsigned short port, ChessBot& bot) {
    try {
        asio::io_context ioContext;
        tcp::acceptor acceptor(ioContext, tcp::endpoint(tcp::v4(), port));

        std::cout << "Server started on port " << port << std::endl;

        while (true) {
            tcp::socket socket(ioContext);
            acceptor.accept(socket);

            // Handle client in a new thread
            std::thread(handleClient, std::move(socket), std::ref(bot)).detach();
        }
    } catch (std::exception& e) {
        std::cerr << "Server error: " << e.what() << std::endl;
    }
}

int main() {
    MoveGenerator moveGen;
    ChessBot bot(moveGen);

    unsigned short port = 8080;
    startServer(port, bot);
    return 0;
}
