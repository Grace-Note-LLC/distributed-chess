import React, { useState } from 'react';
import { DndProvider, useDrag, useDrop } from 'react-dnd';
import { HTML5Backend } from 'react-dnd-html5-backend';
import { Chess } from 'chess.js';
import { sendMove } from '../services/api';
import Square from './Square'; 

const ChessBoard = () => {
  const [game] = useState(new Chess());
  const [position, setPosition] = useState(game.board());

  const handleMove = async (from: string, to: string) => {
    try {
      const move = game.move({ from, to });
      if (move) {
        setPosition(game.board());
        await sendMove({
          from,
          to,
          piece: {
            type: move.piece,
            color: move.color,
            position: { x: move.to[0], y: move.to[1] }
          }
        });
      }
    } catch (error) {
      console.error('Invalid move:', error);
    }
  };

  return (
    <DndProvider backend={HTML5Backend}>
      <div className="board">
        {position.map((row, i) => 
          row.map((piece, j) => (
            <Square 
              key={`${i}-${j}`} 
              piece={piece ? {
                ...piece,
                position: { x: j, y: i },
                image: `/pieces/${piece.color}${piece.type}.png`
              } : null} 
              position={{ x: j, y: i }}
              onMove={handleMove}
            />
          ))
        )}
      </div>
    </DndProvider>
  );
};

export default ChessBoard; 