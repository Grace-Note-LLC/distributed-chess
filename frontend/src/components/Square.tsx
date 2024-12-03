import React from "react"
import './Square.css';

interface Props{
    image?: string;
    number: number;
    highlight: boolean; 
}

export default function Square({number, image, highlight}: Props){
    const className: string = ["tile",
        number% 2 ===0 && "black-tile",
        number % 2 !== 0 && "white-tile",
        highlight && "tile-highlight"].filter(Boolean).join(' ');

    return (
        <div className = {className} >
            {image && <div style = {{backgroundImage: `url(${image})`}} className = "chess-piece"></div>}
        </div>
    );
    
}



