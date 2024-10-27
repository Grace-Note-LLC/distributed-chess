# Dispatch 

`Dispatch` is the server that houses the game engine and the task delegator. 
The chess game itself will be stored in here, where it will create game states to be delegated to the MCUs to evaluate a specific depth tre.

## Communication Specification

Under the hood, the dispatcher should send a bit stream of data that will compose of:
- Game State (what this is composed of is still vague)
- Current Depth (what depth the tree is in)
- Explore Depth (the depth at which the node should go in)
- Hyperparameters (well depend on the algorithm)
    - how aggressive pruning is
    - values for piece position
    - piece value

The plan is to use `SPI`, but `I2C` if this is not possible. 

