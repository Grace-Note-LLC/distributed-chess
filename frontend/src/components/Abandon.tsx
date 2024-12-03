import React from 'react';
import { boardRoutes } from '../services/routes';
import { useNavigate } from "react-router"

const Abandon: React.FC = () => {
    const navigate = useNavigate();

    const handleAbandon = async () => {
        try {
            navigate('/abandon');
            await boardRoutes.abandonGame();
        } catch (error) {
            console.error('Failed to abandon game:', error);
        }
    };

    return (
        <div style={{ position: 'fixed', bottom: '20px', width: '100%', textAlign: 'center' }}>
            <button 
                onClick={handleAbandon}
                style={{
                    padding: '40px 100px',
                    backgroundColor: '#ff4444',
                    color: 'white',
                    border: 'none',
                    borderRadius: '5px',
                    cursor: 'pointer',
                    fontSize: '32px'
                }}
            >
                End Game
            </button>
        </div>
    );
};

export default Abandon;
