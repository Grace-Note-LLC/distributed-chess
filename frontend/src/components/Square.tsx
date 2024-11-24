import React from 'react';
import { ChessPiece, Position } from '../types/types';
import './Square.css';

interface SquareProps {
  piece: {
    position: { x: number; y: number };
    image: string;
    type: string;
    color: string;
  } | null;
  position: { x: number; y: number };
  onMove: (from: string, to: string) => Promise<void>;
}

export default function Square({ piece, position, onMove }: SquareProps) {
  const isBlack = (position.x + position.y) % 2 === 0;
  
  return (
    <div 
      className={`
        tile 
        ${isBlack ? 'black-tile' : 'white-tile'}
      `}
    >
      {piece && (
        <div
          className="chess-piece"
          style={{ backgroundImage: `url(${piece.image})` }}
        />
      )}
    </div>
  );
}



