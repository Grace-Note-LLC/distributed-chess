export const col: string[] = ["1", "2", "3", "4", "5", "6", "7", "8"]
export const row: string[] = ["a", "b", "c", "d", "e", "f", "g", "h"]


export const GRID_SIZE = 100;
export function samePosition(p1: Position, p2){
    return p1.x === p2.x && p1.y === p2.y;
}
export interface Piece{
    image: string
    position: Position;
    type: PieceType;
    team: TeamType;
    enPassant?: boolean;
    possibleMoves?: Position[];
}

export interface Position {
    x: number;
    y:number;
}

export enum PieceType{
    PAWN,
    BISHOP,
    KNIGHT,
    ROOK,
    QUEEN,
    KING


}

export enum TeamType{
    OPPONENT,
    OUR
}

export const intitialBoardState: Piece[] = [
    {image: `images/pieces-basic-svg/rook-b.svg`, position:{ x:0, y:7}, type: PieceType.ROOK, team: TeamType.OPPONENT},
    {image: `images/pieces-basic-svg/knight-b.svg`, position:{ x:1, y:7}, type: PieceType.KNIGHT, team: TeamType.OPPONENT},
    {image: `images/pieces-basic-svg/bishop-b.svg`, position:{x:2, y:7}, type: PieceType.BISHOP, team: TeamType.OPPONENT},
    {image: `images/pieces-basic-svg/queen-b.svg`, position:{x:3, y:7}, type: PieceType.QUEEN, team: TeamType.OPPONENT},
    {image: `images/pieces-basic-svg/king-b.svg`, position:{x:4, y:7}, type: PieceType.KING, team: TeamType.OPPONENT},
    {image: `images/pieces-basic-svg/bishop-b.svg`, position:{x:5, y:7}, type: PieceType.BISHOP, team: TeamType.OPPONENT},
    {image: `images/pieces-basic-svg/knight-b.svg`, position:{x:6, y:7}, type: PieceType.KNIGHT, team: TeamType.OPPONENT},
    {image: `images/pieces-basic-svg/rook-b.svg`, position:{x:7, y:7}, type: PieceType.ROOK, team: TeamType.OPPONENT},
    {image: `images/pieces-basic-svg/pawn-b.svg`, position:{x:0, y:6}, type: PieceType.PAWN, team: TeamType.OPPONENT},
    {image: `images/pieces-basic-svg/pawn-b.svg`, position:{x:1, y:6}, type: PieceType.PAWN, team: TeamType.OPPONENT},
    {image: `images/pieces-basic-svg/pawn-b.svg`, position:{x:2, y:6}, type: PieceType.PAWN, team: TeamType.OPPONENT},
    {image: `images/pieces-basic-svg/pawn-b.svg`, position:{x:3, y:6}, type: PieceType.PAWN, team: TeamType.OPPONENT},
    {image: `images/pieces-basic-svg/pawn-b.svg`, position:{x:4, y:6}, type: PieceType.PAWN, team: TeamType.OPPONENT},
    {image: `images/pieces-basic-svg/pawn-b.svg`, position:{x:5, y:6}, type: PieceType.PAWN, team: TeamType.OPPONENT},
    {image: `images/pieces-basic-svg/pawn-b.svg`, position:{x:6, y:6}, type: PieceType.PAWN, team: TeamType.OPPONENT},
    {image: `images/pieces-basic-svg/pawn-b.svg`, position:{x:7, y:6}, type: PieceType.PAWN, team: TeamType.OPPONENT},
    
    {image: `images/pieces-basic-svg/rook-w.svg`, position:{x:0, y:0}, type: PieceType.ROOK, team: TeamType.OUR},
    {image: `images/pieces-basic-svg/knight-w.svg`, position:{x:1, y:0}, type: PieceType.KNIGHT, team: TeamType.OUR},
    {image: `images/pieces-basic-svg/bishop-w.svg`, position:{x:2, y:0}, type: PieceType.BISHOP, team: TeamType.OUR},
    {image: `images/pieces-basic-svg/queen-w.svg`, position:{x:3, y:0}, type: PieceType.QUEEN, team: TeamType.OUR},
    {image: `images/pieces-basic-svg/king-w.svg`, position:{x:4, y:0}, type: PieceType.KING, team: TeamType.OUR},
    {image: `images/pieces-basic-svg/bishop-w.svg`, position:{x:5, y:0}, type: PieceType.BISHOP, team: TeamType.OUR},
    {image: `images/pieces-basic-svg/knight-w.svg`, position:{x:6, y:0}, type: PieceType.KNIGHT, team: TeamType.OUR},
    {image: `images/pieces-basic-svg/rook-w.svg`, position:{x:7, y:0}, type: PieceType.ROOK, team: TeamType.OUR},
    {image: `images/pieces-basic-svg/pawn-w.svg`, position:{x:0, y:1}, type: PieceType.PAWN, team: TeamType.OUR},
    {image: `images/pieces-basic-svg/pawn-w.svg`, position:{x:1, y:1}, type: PieceType.PAWN, team: TeamType.OUR},
    {image: `images/pieces-basic-svg/pawn-w.svg`, position:{x:2, y:1}, type: PieceType.PAWN, team: TeamType.OUR},
    {image: `images/pieces-basic-svg/pawn-w.svg`, position:{x:3, y:1}, type: PieceType.PAWN, team: TeamType.OUR},
    {image: `images/pieces-basic-svg/pawn-w.svg`, position:{x:4, y:1}, type: PieceType.PAWN, team: TeamType.OUR},
    {image: `images/pieces-basic-svg/pawn-w.svg`, position:{x:5, y:1}, type: PieceType.PAWN, team: TeamType.OUR},
    {image: `images/pieces-basic-svg/pawn-w.svg`, position:{x:6, y:1}, type: PieceType.PAWN, team: TeamType.OUR},
    {image: `images/pieces-basic-svg/pawn-w.svg`, position:{x:7, y:1}, type: PieceType.PAWN, team: TeamType.OUR},




];
