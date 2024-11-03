import React from 'react';
import Square from './Square';


const row = ["a", "b,", "c", "d", "e", "f", "g"]
const col = [1, 2, 3, 4, 5, 6, 7, 8]

export default function Board(){
    let board = [];

    for(let i = 0; i<row.length; i++){
        for(let j = 0; j<col.length; j++){
            console.log(row[i], col[j])
            board.push(<span>{row[i]} {col[j]}</span>)
        }
    }
    return <div id = "Board">{
        <Square />
    }</div>

}