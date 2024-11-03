import React from 'react';
import Square from './Square';

interface Position {
  row: number;
  col: number;
}

interface BoardProps {

}

interface SquareProps {
  position: Position;
}

const Board: React.FC<BoardProps> = () => {

  const rows: null[] = Array(8).fill(null);
  const cols: null[] = Array(8).fill(null);

  return (
    <div className="board">
      {rows.map((_, rowIndex) => (
        <div key={rowIndex} className="board-row">
          {cols.map((_, colIndex) => (
            <Square 
              key={`${rowIndex}-${colIndex}`}
              position={{ row: rowIndex, col: colIndex }}
            />
          ))}
        </div>
      ))}
    </div>
  );
};

export default Board;