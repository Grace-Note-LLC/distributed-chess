"""
This file acts as a mock client to the Dispatcher. It is used to test the Dispatcher without the need for a real client.
"""

import socket
import json

board_state = {
    "WHITE_PAWNS"   : 0x000000000000FF00,  
    "BLACK_PAWNS"   : 0x00FF000000000000,  
    "WHITE_ROOKS"   : 0x0000000000000081,  
    "BLACK_ROOKS"   : 0x8100000000000000,  
    "WHITE_KNIGHTS" : 0x0000000000000042,  
    "BLACK_KNIGHTS" : 0x4200000000000000,  
    "WHITE_BISHOPS" : 0x0000000000000024,  
    "BLACK_BISHOPS" : 0x2400000000000000,  
    "WHITE_QUEEN"   : 0x0000000000000008,  
    "BLACK_QUEEN"   : 0x0800000000000000,  
    "WHITE_KING"    : 0x0000000000000010,
    "BLACK_KING"    : 0x1000000000000000,
}

def send_request(board_state, player):
    request = json.dumps({
        "boardState": board_state,  # Serialize your board state
        "player": player
    })

    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as sock:
        sock.connect(('127.0.0.1', 8081))
        sock.sendall(request.encode('utf-8') + b'\n')

        response = sock.recv(1024).decode('utf-8')
        return json.loads(response)

# Example usage
if __name__ == "__main__":
    board_state = board_state  # Replace with serialized board
    player = "WHITE"
    response = send_request(board_state, player)
    print("Best Move:", response["bestMove"], "Score:", response["score"])
