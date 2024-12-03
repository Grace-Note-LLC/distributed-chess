import React, { useEffect, useRef, useState } from 'react';
import Square from '../components/Square';
import "./Board.css";
import Referee from './Referee.tsx/Referee';
import {row, col, GRID_SIZE, Piece, PieceType, TeamType, intitialBoardState, Position, samePosition} from '../components/Constant'


export default function Board(){
    const [activePiece, setActivePiece] = useState<HTMLElement | null>(null);
    const [promotionPawn, setPromotionPawn] = useState<Piece>();
    const [grabPosition, setGrabPosition] = useState<Position>({x:-1, y:-1});
    const [pieces, setPieces] = useState<Piece[]>(intitialBoardState);
    const boardRef = useRef<HTMLDivElement>(null);
    const modelRef= useRef<HTMLDivElement>(null);
    const referee = new Referee();
    
    function updateValidMoves(){
        setPieces((currentPieces) =>{
            return currentPieces.map(p=>{
                p.possibleMoves = referee.getValidMove(p, currentPieces);
                return p;
            });

        });
    }

    function grabPiece(e: React.MouseEvent){
        updateValidMoves();
        const element = e.target as HTMLElement;
        const chessboard = boardRef.current;
        if (element.classList.contains("chess-piece") && chessboard){
            const grabX = Math.floor((e.clientX - chessboard.offsetLeft)/GRID_SIZE);
            const grabY = Math.abs(Math.ceil((e.clientY - chessboard.offsetTop - 800)/GRID_SIZE));
            setGrabPosition({x: grabX, y: grabY});
            const x = e.clientX-GRID_SIZE/2;
            const y = e.clientY-GRID_SIZE/2;
            element.style.position = "absolute";
            element.style.left = `${x}px`;
            element.style.top = `${y}px`;

            setActivePiece(element);
        }

    }

    function movePiece(e: React.MouseEvent){
        const chessboard = boardRef.current;
        if(activePiece&& chessboard){
            const minX = chessboard.offsetLeft-25;
            const minY = chessboard.offsetTop -25;
            const maxX = chessboard.offsetLeft + chessboard.clientWidth -75;
            const maxY = chessboard.offsetTop + chessboard.clientHeight -75;
            const x = e.clientX-50;
            const y = e.clientY-50;
            activePiece.style.position = "absolute";
            

            if (x < minX) {
                activePiece.style.left = `${minX}px`;
            } 
            else if (x > maxX) {
                activePiece.style.left = `${maxX}px`;
            } 
            else{
                activePiece.style.left =  `${x}px`;
            }

            if (y < minY) {
                activePiece.style.top = `${minY}px`;
            } else if (y> maxY){
                activePiece.style.top = `${maxY}px`;
            } else{
                activePiece.style.top =  `${y}px`;
            }
        }
    
       }

    function dropPiece(e: React.MouseEvent){
        const chessboard = boardRef.current;
        if(activePiece && chessboard){
            const x = Math.floor((e.clientX - chessboard.offsetLeft)/GRID_SIZE);
            const y = Math.abs(Math.ceil((e.clientY - chessboard.offsetTop - 800)/GRID_SIZE));

            const currentPiece = pieces.find((p) => samePosition(p.position, grabPosition));
            
            if(currentPiece){
                const validMove = referee.isValidMove(grabPosition, {x, y}, currentPiece?.type, currentPiece?.team, pieces);
                
                const isEnPassantMove = referee.isEnPassantMove(grabPosition, {x, y}, currentPiece.type, currentPiece.team, pieces);

                const pawnDirection = currentPiece.team === TeamType.OUR ? 1: -1;
                
                if(isEnPassantMove){
                    const updatePieces = pieces.reduce((results, piece) => {
                        if(samePosition(piece.position, grabPosition)){
                            piece.enPassant =false;
                            piece.position.x = x;
                            piece.position.y = y;
                            results.push(piece);
                        } else if (!samePosition(piece.position, {x, y:y-pawnDirection})){
                            if(piece.type === PieceType.PAWN){
                                piece.enPassant = false;
                            }
                            results.push(piece);
                        }
                        return results;
                    }, [] as Piece[])

                    setPieces(updatePieces);
                } else if(validMove){
                    const updatedPieces = pieces.reduce((results, piece) => {
                        if (samePosition(piece.position, grabPosition)){
                            //special move
                            piece.enPassant = Math.abs(grabPosition.y - y) === 2 && piece.type === PieceType.PAWN;
                            
                            piece.position.x =x;
                            piece.position.y = y;

                            let promotionRow = (piece.team == TeamType.OUR ? 7:0);
                            if(y === promotionRow && piece.type === PieceType.PAWN){
                                modelRef.current?.classList.remove("hidden");
                                setPromotionPawn(piece);
                            }
                            results.push(piece);
                        } else if (!(samePosition(piece.position, {x, y}))){
                            if(piece.type === PieceType.PAWN){
                                piece.enPassant = false;
                            }
                            results.push(piece);
                        }

                        return results;
                    }, [] as Piece[]);

                    setPieces(updatedPieces);
                } else{
                    activePiece.style.position = 'relative';
                    activePiece.style.removeProperty('top');
                    activePiece.style.removeProperty('left');
                }
                }

            setActivePiece(null);
        }
    }

    function promotePawn(pieceType: PieceType){
        if(promotionPawn === undefined){
            return;
        }

        const updatedPieces = pieces.reduce((results, piece) =>{
            if(samePosition(piece.position, promotionPawn.position)){
                piece.type = pieceType;
                piece.type = pieceType;
                const teamType = (piece.team === TeamType.OUR) ? "w" : "b";
                let image = "";
                switch(pieceType){
                    case(PieceType.ROOK): {
                        image= "rook";
                        break;
                    }
                    case(PieceType.BISHOP): {
                        image= "bishop";
                        break;
                    }
                    case(PieceType.KNIGHT): {
                        image= "knight";
                        break;
                    }
                    case(PieceType.QUEEN): {
                        image= "queen";
                        break;
                    }
                }

                piece.image = `images/pieces-basic-svg/${image}-${teamType}.svg`
            }

            results.push(piece);
            return results;
        }, [] as Piece[])

        setPieces(updatedPieces);
        modelRef.current?.classList.add("hidden");
    }

    function promotionTeamType(){
        return (promotionPawn?.team === TeamType.OUR) ? "w" : "b";
    }

    let board: JSX.Element[] = [];

    for(let j=col.length-1; j>=0; j--){
        for(let i = 0; i<row.length; i++){
            const number = j+i+2;
            const piece = pieces.find((p) => samePosition(p.position, {x: i, y: j}));
            let image = piece ? piece.image : undefined;

            let currentPiece = activePiece!= null ? pieces.find(p => samePosition(p.position, grabPosition)) : undefined;
            let highlight = currentPiece?.possibleMoves ? currentPiece.possibleMoves.some(p => samePosition(p, {x:i, y:j})): false;

            board.push(<Square key = {`${j}, ${i}`} image ={image} number = {number} highlight = {highlight} />); 
    }
}
    return(
        <>
        <div id = "pawn-promotion-model" className = "hidden" ref={modelRef}> 
            <div className = "model-body">
                <img onClick = {() => promotePawn(PieceType.ROOK)} src = {`/images/pieces-basic-svg/rook-${promotionTeamType()}.svg`}/>
                <img onClick = {() => promotePawn(PieceType.BISHOP)} src = {`/images/pieces-basic-svg/bishop-${promotionTeamType()}.svg`}/>
                <img onClick = {() => promotePawn(PieceType.KNIGHT)} src = {`/images/pieces-basic-svg/knight-${promotionTeamType()}.svg`}/>
                <img onClick = {() => promotePawn(PieceType.QUEEN)} src = {`/images/pieces-basic-svg/queen-${promotionTeamType()}.svg`}/>
            </div>
        </div>
            <div
                onMouseMove= {(e) => movePiece(e)}
                onMouseDown= {e => grabPiece(e)} 
                onMouseUp = {(e) => dropPiece(e)}
                id = "board"
                ref = {boardRef}
            >
                {board}
            </div>
        </>
    );

}

    