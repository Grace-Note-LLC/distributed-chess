import { Piece, Position, samePosition, TeamType } from "../../Constant";
import { tileIsEmptyOrOccupiedByOpponent, tileIsOccupied, tileIsOccupiedByOpponent } from "./GeneralRules";

export const bishopMove = (initialPosition: Position, desiredPosition: Position, team: TeamType, boardState: Piece[]):boolean => {
    for (let i = 1; i<8; i++){
        if(desiredPosition.x > initialPosition.x && desiredPosition.y > initialPosition.y){
            let passedPosition: Position = {x: initialPosition.x + i, y: initialPosition.y + i};                    
            if(samePosition(passedPosition, desiredPosition)){
                if(tileIsEmptyOrOccupiedByOpponent(passedPosition, boardState, team)){
                    return true;
                }
                } else {
                    if(tileIsOccupied(passedPosition, boardState)){
                        break;
                    }
                       
                }
            }

        if(desiredPosition.x > initialPosition.x && desiredPosition.y < initialPosition.y){
            let passedPosition: Position = {x: initialPosition.x + i, y: initialPosition.y -i};
            if(samePosition(passedPosition, desiredPosition)){
                if(tileIsEmptyOrOccupiedByOpponent(passedPosition, boardState, team)){
                    return true;
                }
            } else {
                if(tileIsOccupied(passedPosition, boardState)){
                    break;
                }
            }                
        }

        if(desiredPosition.x < initialPosition.x && desiredPosition.y < initialPosition.y){
            let passedPosition: Position = {x: initialPosition.x - i, y: initialPosition.y -i};
            if(samePosition(passedPosition, desiredPosition)){
                if(tileIsEmptyOrOccupiedByOpponent(passedPosition, boardState, team)){
                    return true;
                }
            } else {
                if(tileIsOccupied(passedPosition, boardState)){
                    break;
                }
            }
        }
          

            
        if(desiredPosition.x < initialPosition.x && desiredPosition.y > initialPosition.y){
            let passedPosition: Position = {x: initialPosition.x - i, y: initialPosition.y + i};
            if(samePosition(passedPosition, desiredPosition)){
                if(tileIsEmptyOrOccupiedByOpponent(passedPosition, boardState, team)){
                    return true;
                }
            } else {
                if(tileIsOccupied(passedPosition, boardState)){
                    break;
                }
            }
        }
    }

return false;
}


export const getPossibleBishopMoves = (bishop: Piece, boardstate: Piece[]): Position[] => {
    const possibleMoves: Position[] = [];

    // Upper right movement
    for(let i = 1; i < 8; i++) {
        const destination: Position = ({x: bishop.position.x +i, y:bishop.position.y+i});

      if(!tileIsOccupied(destination, boardstate)) {
        possibleMoves.push(destination);
      } else if(tileIsOccupiedByOpponent(destination, boardstate, bishop.team)) {
        possibleMoves.push(destination);
        break;
      } else {
        break;
      }
    }

    // Bottom right movement
    for(let i = 1; i < 8; i++) {
      const destination: Position = ({x: bishop.position.x +i, y:bishop.position.y-i});

      if(!tileIsOccupied(destination, boardstate)) {
        possibleMoves.push(destination);
      } else if(tileIsOccupiedByOpponent(destination, boardstate, bishop.team)) {
        possibleMoves.push(destination);
        break;
      } else {
        break;
      }
    }

    // Bottom left movement
    for(let i = 1; i < 8; i++) {
        const destination: Position = ({x: bishop.position.x -i, y:bishop.position.y-i});

      if(!tileIsOccupied(destination, boardstate)) {
        possibleMoves.push(destination);
      } else if(tileIsOccupiedByOpponent(destination, boardstate, bishop.team)) {
        possibleMoves.push(destination);
        break;
      } else {
        break;
      }
    }

    // Top left movement
    for(let i = 1; i < 8; i++) {
        const destination: Position = ({x: bishop.position.x -i, y:bishop.position.y+i});

      if(!tileIsOccupied(destination, boardstate)) {
        possibleMoves.push(destination);
      } else if(tileIsOccupiedByOpponent(destination, boardstate, bishop.team)) {
        possibleMoves.push(destination);
        break;
      } else {
        break;
      }
    }


    return possibleMoves;
  }


