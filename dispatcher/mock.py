"""
This file acts as a mock client to the Dispatcher. It is used to test the Dispatcher without the need for a real client.
"""

import socket
import json

def send_request(board_state, player):
    request = json.dumps({
        "boardState": board_state,  # Serialize your board state
        "player": player
    })

    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as sock:
        sock.connect(('127.0.0.1', 8080))
        sock.sendall(request.encode('utf-8') + b'\n')

        response = sock.recv(1024).decode('utf-8')
        return json.loads(response)

# Example usage
if __name__ == "__main__":
    board_state = "standard"  # Replace with serialized board
    player = "WHITE"
    response = send_request(board_state, player)
    print("Best Move:", response["bestMove"], "Score:", response["score"])
