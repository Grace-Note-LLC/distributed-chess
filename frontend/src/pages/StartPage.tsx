import React from 'react';
import { useNavigate } from 'react-router';
import './StartPage.css';

const StartPage: React.FC = () => {
  const navigate = useNavigate();

  const handleStartGame = () => {
    navigate('/game');
  };

  return (
    <div className="start-page">
      <h1>Distributed Chess</h1>
      <p>Play against a distributed chess engine powered by recycled IoT devices</p>
      <button onClick={handleStartGame}>Start Game</button>
    </div>
  );
};

export default StartPage;
