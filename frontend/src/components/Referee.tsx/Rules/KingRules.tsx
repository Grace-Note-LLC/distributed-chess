import { Piece, Position, samePosition, TeamType } from "../../Constant";
import { tileIsEmptyOrOccupiedByOpponent, tileIsOccupied, tileIsOccupiedByOpponent } from "./GeneralRules";

export const kingMove =(initialPosition: Position, desiredPosition: Position, team: TeamType, boardState: Piece[]):boolean => {
    for(let i =1; i<2; i++) {

        //diagonal
        let multiplierX = (desiredPosition.x < initialPosition.x) ? -1 : (desiredPosition.x > initialPosition.x) ? 1:0;
        let multiplierY = (desiredPosition.y < initialPosition.y) ? -1 : (desiredPosition.y > initialPosition.y) ? 1 : 0;
        
        let passedPosition: Position = {x: initialPosition.x +  (i * multiplierX), y: initialPosition.y + (i * multiplierY)};

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
    return false;
}

export const getPossibleKingMoves = (king: Piece, boardState: Piece[]): Position[] => {
    const possibleMoves: Position[] = [];

  // Top movement
  for (let i = 1; i < 2; i++) {
    const destination: Position = {x: king.position.x, y: king.position.y + i};

    // If the move is outside of the board don't add it
    if(destination.x < 0 || destination.x > 7 
      || destination.y < 0 || destination.y > 7) {
        break;
    }

    if (!tileIsOccupied(destination, boardState)) {
      possibleMoves.push(destination);
    } else if (tileIsOccupiedByOpponent(destination, boardState, king.team)) {
      possibleMoves.push(destination);
      break;
    } else {
      break;
    }
  }

  // Bottom movement
  for (let i = 1; i < 2; i++) {
    const destination: Position = {x: king.position.x, y: king.position.y - i};

    // If the move is outside of the board don't add it
    if(destination.x < 0 || destination.x > 7 
      || destination.y < 0 || destination.y > 7) {
        break;
    }

    if (!tileIsOccupied(destination, boardState)) {
      possibleMoves.push(destination);
    } else if (tileIsOccupiedByOpponent(destination, boardState, king.team)) {
      possibleMoves.push(destination);
      break;
    } else {
      break;
    }
  }

  // Left movement
  for (let i = 1; i < 2; i++) {
    const destination: Position = {x: king.position.x -i, y: king.position.y};

    // If the move is outside of the board don't add it
    if(destination.x < 0 || destination.x > 7 
      || destination.y < 0 || destination.y > 7) {
        break;
    }

    if (!tileIsOccupied(destination, boardState)) {
      possibleMoves.push(destination);
    } else if (tileIsOccupiedByOpponent(destination, boardState, king.team)) {
      possibleMoves.push(destination);
      break;
    } else {
      break;
    }
  }

  // Right movement
  for (let i = 1; i < 2; i++) {
    const destination: Position = {x: king.position.x + i, y: king.position.y};

    // If the move is outside of the board don't add it
    if(destination.x < 0 || destination.x > 7 
      || destination.y < 0 || destination.y > 7) {
        break;
    }

    if (!tileIsOccupied(destination, boardState)) {
      possibleMoves.push(destination);
    } else if (tileIsOccupiedByOpponent(destination, boardState, king.team)) {
      possibleMoves.push(destination);
      break;
    } else {
      break;
    }
  }

  // Upper right movement
  for (let i = 1; i < 2; i++) {
    const destination: Position = {x: king.position.x+ i, y: king.position.y + i};

    // If the move is outside of the board don't add it
    if(destination.x < 0 || destination.x > 7 
      || destination.y < 0 || destination.y > 7) {
        break;
    }

    if (!tileIsOccupied(destination, boardState)) {
      possibleMoves.push(destination);
    } else if (tileIsOccupiedByOpponent(destination, boardState, king.team)) {
      possibleMoves.push(destination);
      break;
    } else {
      break;
    }
  }

  // Bottom right movement
  for (let i = 1; i < 2; i++) {
    const destination: Position = {x: king.position.x+ i, y: king.position.y - i};

    // If the move is outside of the board don't add it
    if(destination.x < 0 || destination.x > 7 
      || destination.y < 0 || destination.y > 7) {
        break;
    }

    if (!tileIsOccupied(destination, boardState)) {
      possibleMoves.push(destination);
    } else if (tileIsOccupiedByOpponent(destination, boardState, king.team)) {
      possibleMoves.push(destination);
      break;
    } else {
      break;
    }
  }

  // Bottom left movement
  for (let i = 1; i < 2; i++) {
    const destination: Position = {x: king.position.x -i, y: king.position.y - i};

    // If the move is outside of the board don't add it
    if(destination.x < 0 || destination.x > 7 
      || destination.y < 0 || destination.y > 7) {
        break;
    }

    if (!tileIsOccupied(destination, boardState)) {
      possibleMoves.push(destination);
    } else if (tileIsOccupiedByOpponent(destination, boardState, king.team)) {
      possibleMoves.push(destination);
      break;
    } else {
      break;
    }
  }

  // Top left movement
  for (let i = 1; i < 2; i++) {
    const destination: Position = {x: king.position.x- i, y: king.position.y + i};

    // If the move is outside of the board don't add it
    if(destination.x < 0 || destination.x > 7 
      || destination.y < 0 || destination.y > 7) {
        break;
    }
    
    if (!tileIsOccupied(destination, boardState)) {
      possibleMoves.push(destination);
    } else if (tileIsOccupiedByOpponent(destination, boardState, king.team)) {
      possibleMoves.push(destination);
      break;
    } else {
      break;
    }
  }

  return possibleMoves;
}