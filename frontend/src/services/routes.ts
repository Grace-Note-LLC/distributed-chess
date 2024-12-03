import { Piece } from '../components/Constant';

const API_BASE_URL = 'http://localhost:8080/api';

export const boardRoutes = {
    // Sending board state on piece update
    sendBoardState: async (pieces: Piece[]): Promise<{ status: string }> => {
        console.log("sending board state");
        try {
            const response = await fetch(`${API_BASE_URL}/board-state`, {
                method: 'POST',
                headers: {
                    'Content-Type': 'application/json',
                },
                body: JSON.stringify(pieces)
            });

            if (!response.ok) {
                throw new Error('Network response was not ok');
            }

            return await response.json();
        } catch (e) {
            console.error('Error sending board state:', e);
            throw e;
        }
    },

    // Abandon Game
    abandonGame: async(): Promise<{ status: string }> => {
        console.log("Abandoning game");
        try {
            const response = await fetch(`${API_BASE_URL}/abandon`, {
                method: 'POST',
                headers: {
                    'Content-Type': 'application/json',
                },
                body: JSON.stringify({
                    timestamp: new Date().toISOString()
                })
            });
    
            if (!response.ok) {
                throw new Error(`HTTP error! status: ${response.status}`);
            }
    
            const data = await response.json();
            return data;
            
        } catch(e) {
            console.error('Error abandoning game:', e);
            throw e;
        }
    }
};



