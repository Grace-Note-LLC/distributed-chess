import React, { useState, useRef } from "react";
import "./Board.css";
import { initialPieces } from "./pieces";

const Board: React.FC = () => {
    const [pieces, setPieces] = useState(initialPieces);
    const boardRef = useRef<HTMLDivElement>(null);
    const [activePiece, setActivePiece] = useState<HTMLElement | null>(null);
    const [offset, setOffset] = useState<{ x: number; y: number } | null>(null);

    function grabPiece(e: React.MouseEvent) {
        const element = e.target as HTMLElement;
        if (element.classList.contains("chess-piece")) {
            setActivePiece(element);

            // calculate offset between mouse click and piece's top-left corner
            const rect = element.getBoundingClientRect();
            setOffset({
                x: e.clientX - rect.left,
                y: e.clientY - rect.top,
            });

            // Set piece style when grabbed
            element.style.position = "absolute";
            element.style.zIndex = "1000";
            element.style.pointerEvents = "none";
        }
    }

    // move the piece along with the mouse
    function movePiece(e: React.MouseEvent) {
        if (activePiece && offset) {
            const boardRect = boardRef.current?.getBoundingClientRect();
            if (boardRect) {
                // calculate new position within the board based on mouse position
                const x = e.clientX - boardRect.left - offset.x;
                const y = e.clientY - boardRect.top - offset.y;

                // update the piece's position to follow the mouse
                activePiece.style.left = `${x}px`;
                activePiece.style.top = `${y}px`;
            }
        }
    }

    // drop the piece and snap it to the closest tile
    function dropPiece(e: React.MouseEvent) {
        if (activePiece) {
            const boardRect = boardRef.current?.getBoundingClientRect();
            if (boardRect) {
                const tileSize = boardRect.width / 8;

                // calculate the position relative to the board
                const offsetX = e.clientX - boardRect.left;
                const offsetY = e.clientY - boardRect.top;

                // determine which tile to snap to
                const snapX = Math.max(0, Math.min(7, Math.floor(offsetX / tileSize)));
                const snapY = Math.max(0, Math.min(7, Math.floor(offsetY / tileSize)));

                // update the piece position in the state to reflect the new tile
                setPieces((prevPieces) =>
                    prevPieces.map((p) =>
                        p.image === activePiece.style.backgroundImage // Assuming unique images
                            ? { ...p, x: snapX, y: snapY }
                            : p
                    )
                );

                // snap the piece to the calculated tile position
                activePiece.style.left = `${snapX * tileSize}px`;
                activePiece.style.top = `${snapY * tileSize}px`;

                // reset piece styles after dropping
                activePiece.style.zIndex = "1";
                activePiece.style.pointerEvents = "auto";
            }
            // prep board state to be sent to the backend
            const boardState = pieces.map((piece) => ({
                image: piece.image,
                x: piece.x,
                y: piece.y,
            }));

            // Make the API call to the Go backend on port 8080
            fetch("http://localhost:8080/api/board-state", {
                method: "POST",
                headers: {
                    "Content-Type": "application/json",
                },
                body: JSON.stringify(boardState),
            })
                .then((response) => response.json())
                .then((data) => {
                    console.log("Board state sent successfully:", data);
                })
                .catch((error) => {
                    console.error("Error sending board state:", error);
                });

            // reset state
            setActivePiece(null);
            setOffset(null);
        }
    }

    // render the chessboard and pieces in their correct positions
    const renderPieces = () => {
        const tileSize = boardRef.current ? boardRef.current.clientWidth / 8 : 100;

        return pieces.map((piece, index) => (
            <div
                key={`${piece.image}-${index}`} // Combine image path with index to create a unique key
                className="chess-piece"
                style={{
                    backgroundImage: `url(${piece.image})`,
                    width: `${tileSize}px`,
                    height: `${tileSize}px`,
                    backgroundSize: "cover",
                    position: "absolute",
                    left: `${piece.x * tileSize}px`,
                    top: `${piece.y * tileSize}px`,
                }}
                onMouseDown={(e) => grabPiece(e)}
            ></div>
        ));
    };


    return (
        <div
            onMouseMove={(e) => movePiece(e)}
            onMouseUp={(e) => dropPiece(e)}
            ref={boardRef}
            id="chessboard"
            style={{
                width: "800px",
                height: "800px",
                display: "grid",
                gridTemplateColumns: "repeat(8, 1fr)",
                gridTemplateRows: "repeat(8, 1fr)",
                position: "relative",
                top: "0",
                left: "0",
            }}
        >
            {Array.from({ length: 8 }).map((_, row) =>
                Array.from({ length: 8 }).map((_, col) => (
                    <div
                        key={`${row}-${col}`}
                        className={`tile ${(row + col) % 2 === 0 ? "black-tile" : "white-tile"}`}
                        style={{
                            width: "100%",
                            height: "100%",
                        }}
                    />
                ))
            )}
            {renderPieces()}
        </div>
    );
};

export default Board;
