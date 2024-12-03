#include <iostream>
#include <string>
#include <thread>
#include <vector>
#include <sstream>
#include <asio.hpp>
#include <nlohmann/json.hpp>

#include "Board.h"
#include "Bot.h"

using asio::ip::tcp;
using json = nlohmann::json;

void recreateBoard(Board* board, json boardState) {
    board->setPieceBin(Board::WHITE_PAWNS,   boardState.at("WHITE_PAWNS"));
    board->setPieceBin(Board::BLACK_PAWNS,   boardState.at("BLACK_PAWNS"));
    board->setPieceBin(Board::WHITE_ROOKS,   boardState.at("WHITE_ROOKS"));
    board->setPieceBin(Board::BLACK_ROOKS,   boardState.at("BLACK_ROOKS"));
    board->setPieceBin(Board::WHITE_KNIGHTS, boardState.at("WHITE_KNIGHTS"));
    board->setPieceBin(Board::BLACK_KNIGHTS, boardState.at("BLACK_KNIGHTS"));
    board->setPieceBin(Board::WHITE_BISHOPS, boardState.at("WHITE_BISHOPS"));
    board->setPieceBin(Board::BLACK_BISHOPS, boardState.at("BLACK_BISHOPS"));
    board->setPieceBin(Board::WHITE_QUEEN,   boardState.at("WHITE_QUEEN"));
    board->setPieceBin(Board::BLACK_QUEEN,   boardState.at("BLACK_QUEEN"));
    board->setPieceBin(Board::WHITE_KING,    boardState.at("WHITE_KING"));
    board->setPieceBin(Board::BLACK_KING,    boardState.at("BLACK_KING"));
}


void handleClient(tcp::socket socket, ChessBot& bot) {
    try {
        asio::streambuf buffer;
        asio::read_until(socket, buffer, "\n");
        std::istream input(&buffer);
        std::string request;
        std::getline(input, request);

        // Parse the JSON input
        json requestJson = json::parse(request);
        Board* board = new Board();
        tileState player;

        auto boardState = requestJson.at("boardState");
        recreateBoard(board, boardState);
        board->prettyPrint();
        std::string playerStr = requestJson.at("player");
        player = (playerStr == "WHITE") ? WHITE : BLACK;

        pair<Move, int> result = bot.findBestMove(board, player);
        Board boardCopy = *board;
        MoveGenerator moveGen;
        boardCopy.applyMove(result.first);
        bool isCheckmate = moveGen.isCheckmate(board, (player == WHITE) ? BLACK : WHITE);

        json responseJson = {
            {"bestMove", result.first.getPieceType()},
            {"oldPosition", result.first.getOldPosition()},
            {"newPosition", result.first.getNewPosition()},
            {"isCheckmate", isCheckmate},
            {"score", result.second}
        };

        std::string response = responseJson.dump() + "\n";
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

    bot.detectSPIDevices();

    unsigned short port = 8081;
    startServer(port, bot);
    return 0;
}
