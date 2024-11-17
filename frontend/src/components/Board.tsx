import React, { useRef, useState } from 'react';
import Square from '../components/Square';
import "./Board.css";


const col: string[] = ["1", "2", "3", "4", "5", "6", "7", "8"]
const row: string[] = ["a", "b", "c", "d", "e", "f", "g", "h"]


interface Piece{
    image: string
    x: number
    y: number
}

const pieces: Piece[] = [];

for(let p =0; p<2; p++){
    const type = (p===0) ? "b" : "w"
    const y = (p===0) ? 7: 0;

    pieces.push({image: `images/pieces-basic-svg/rook-${type}.svg`, x:0, y})
    pieces.push({image: `images/pieces-basic-svg/rook-${type}.svg`, x:7, y})
    pieces.push({image: `images/pieces-basic-svg/knight-${type}.svg`, x:1, y})
    pieces.push({image: `images/pieces-basic-svg/knight-${type}.svg`, x:6, y})
    pieces.push({image: `images/pieces-basic-svg/bishop-${type}.svg`, x:2, y})
    pieces.push({image: `images/pieces-basic-svg/bishop-${type}.svg`, x:5, y})
    pieces.push({image: `images/pieces-basic-svg/queen-${type}.svg`, x:3, y})
    pieces.push({image: `images/pieces-basic-svg/king-${type}.svg`, x:4, y})
}

for(let i =0;  i<8; i++){
    pieces.push({image: "images/pieces-basic-svg/pawn-b.svg", x:i, y:6});
}

for(let i =0;  i<8; i++){
    pieces.push({image: "images/pieces-basic-svg/pawn-w.svg", x:i, y:1});
}



let activePiece: HTMLElement | null = null;


export default function Board(){
    const boardRef = useRef<HTMLDivElement>(null);

    let activePiece: HTMLElement | null = null;

function grabPiece(e: React.MouseEvent){
    const element = e.target as HTMLElement;
    if (element.classList.contains("chess-piece")){

        const x = e.clientX-50;
        const y = e.clientY-50;
        element.style.position = "absolute";
        element.style.left = `${x}px`;
        element.style.top = `${y}px`;

        activePiece = element;
    }

}

function movePiece(e: React.MouseEvent){
    const [pieces, setPieces] = useState<Piece[]>()
    const chessboard = boardRef.current;
    if(activePiece&& chessboard){
        const minX = chessboard.offsetLeft-25;
        const minY = chessboard.offsetTop-25;
        const maxX = chessboard.offsetLeft + chessboard.clientWidth -75;
        const maxY = chessboard.offsetTop + chessboard.clientHeight - 75;
        const x = e.clientX-50;
        const y = e.clientY-50;
        activePiece.style.position = "absolute";
        // activePiece.style.left = `${x}px`;
        // activePiece.style.top = `${y}px`;

        console.log(chessboard);

        
        if (x < minX){
            activePiece.style.left = `${minX}px`;
        } else if (x> maxX){
            activePiece.style.left = `${maxX}px`;
        } else {
            activePiece.style.left = `${x}px`
        }

        if (y < minY){
            activePiece.style.top = `${minY}px`;
        } else if (y> maxY){
            activePiece.style.top = `${maxY}px`;
        } else {
            activePiece.style.top = `${y}px`
        }
    }
    
}

function dropPiece(e: React.MouseEvent){
    console.log(e)
    if(activePiece){
        pieces[0].x = 5;
        activePiece = null;
    }
}

    let board: JSX.Element[] = [];
    for(let j=col.length-1; j>=0; j--){
        for(let i = 0; i<row.length; i++){
            const number = j+i+2;
            let image = "";

            pieces.forEach((p) => {
                if(p.x === i && p.y === j){
                    image = p.image;
                }
            });

            board.push(<Square key = {`${j}, ${i}`} image ={image} number = {number} />);

           
        
    }
}
    return(
        <div
           onMouseMove= {(e) => movePiece(e)}
           onMouseDown= {e => grabPiece(e)} 
           onMouseUp = {(e) => dropPiece(e)}
           id = "board"
           ref = {boardRef}
        >
            {board}
        </div>
    );

}

    