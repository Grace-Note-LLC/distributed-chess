export interface Position {
  x: number;
  y: number;
}

export interface ChessPiece {
  type: 'p' | 'n' | 'b' | 'r' | 'q' | 'k';
  color: 'w' | 'b';
  position: Position;
  image: string; // URL to piece image from /assets/pieces/{color}{type}.png
}

export interface MovePayload {
  from: string;
  to: string;
  piece: ChessPiece;
} 