# junkfish

## What it does

Junkfish is a distributed system of microcontrollers harvested from recycled smart devices working together to power a chess engine. Anyone can connect to the front end web application and play a game against a variety of hardware products sourced from Temu and Amazon, such as smart lightbulbs, a smart switch, or smart camera.

## Inspiration

We were inspired by how chess bots were able to understand the game from a mathematical standpoint and perform complex tasks and strategies.

- We wanted to include distributed computing, so we implemented a way to incorporate MCUs from recycled IoT devices. We believe recycling is an important thing that society needs to do. There is a lot of E-waste that has valuable parts that get underutilized, so we wanted to find an innovative solution to it.

- We drew inspiration from the magic of vacations—where even the simplest moments turn into unforgettable adventures. There's a lot of fun when you finally overcome the chessbot and make a move that leads to a decisive victory. We wanted to encapsulate these moments in junkfish. With its vacation-themed design and clever distributed architecture, our chess bot isn’t just a technical achievement; it’s a reminder that creativity and collaboration can take you further than you ever imagined, both on the board and in life.

- We wanted to deepen our understanding of distributed systems, algorithms, and optimization while gaining experience in designing scalable and resilient architectures.

- We found it enjoyable to combine a passion for chess with cutting-edge technology. Playing against the chess bot was pretty awesome! Plus, creating a bot that performs well despite limited resources demonstrates ingenuity and could spark broader applications.

## How we built it

Junkfish is made up of 5 parts:

### Frontend
The front end of the chess website was developed using TypeScript and React, ensuring a robust and dynamic user experience. React provided the flexibility to create reusable components, such as the chessboard, individual pieces, and game controls, which streamlined development and maintenance. Using TypeScript added the benefit of static typing, catching errors early and improving code reliability, especially for complex features like move validation and dynamic piece rendering. The chessboard was implemented as an interactive grid where users could drag and drop pieces, with visual cues highlighting legal moves and selected pieces. State management, powered by React hooks, ensured smooth updates for real-time gameplay, including move animations, timers, and status indicators. The front end communicated with the backend through REST APIs or WebSockets, enabling multiplayer functionality and saving game states. 

### Golang-based Backend
We implemented the backend in Golang because we wanted to try out a new programming language that was high performance, good for scaling, and had very robust error handling. Along the way, we learned about networking, Cross Origin Resource Sharing, REST APIs, and data processing.

### Dispatcher
The dispatcher plays a central role in coordinating tasks across the distributed system. It dynamically assigns computations to devices based on their current load, ensuring optimal utilization of resources. It is able to interface with the MCUs based on the SPI interface on the RaspberryPi By monitoring device availability and performance, the dispatcher maintains balance within the network, enabling smooth and efficient execution of chess engine computations even in heterogeneous environments.

It is able to take an entire chess board state in compress it down to only twelve 64-bit integers, allowing us to transfer data at a much faster rate.

### Minimax Algorithm + Alpha-Beta Pruning
At the core of Junkfish's chess engine lies the Minimax algorithm, which evaluates possible moves to identify the optimal one by simulating future gameplay scenarios. We designed a powerful board evaluation function so the algorithm can determine optimal board states for the bot's turn, predicting what the opponent will do in future moves. Our algorithm is entirely stateless, so we can predict all possible valid board states anytime, anywhere.

To enhance its efficiency in a distributed environment, we integrated alpha-beta pruning. This optimization significantly reduces the number of nodes the algorithm evaluates by eliminating branches that cannot influence the final decision, ensuring faster computations and better performance across the distributed system. 

### Firmware 
The firmware is written in C, and adapted to different hardware devices such that each works together synchronously. When a player makes a move, the AI move generation pipeline is kicked off, and the data is passed to the backend. From there, the engine checks whether a checkmate or stalemate has been reached, and then generates a list of all possible moves. The minimax algorithm is run on those moves, and each unique board state is send as needed to an available smart device over SPI for scoring. The smart device then evaluates the current score of the board based on a variety of factors including mobility, material balance, whether the move was a capture, etc.

It should be noted that each of these devices needed to be reverse engineered so that the custom board evaluation and SPI communication firmware could be flashed onto the microcontroller. To accomplish this, we physically opened each device to expose the debug pins on the MCU, powered it, and then sent commands to flash the device over UART.

## Challenges we ran into

### Frontend
On the frontend one of the biggest hurdles was debugging issues with the game's logic, such as accurately implementing the rules for special moves like castling and en-passant. These bugs often required caareful review of the code and unit tests. 

Time management was another challenge, as balancing development tasks with other commitments meant juggling priorities and making sacrifices. Additionally, there were moments of frustration when we got stuck, particularly while designing an efficient algorithm for move validation and game state tracking. To overcome these obstacles, we broke tasks into smaller, manageable pieces, set clear daily goals, and leveraged online resources like forums and documentation. Debugging became easier as we adopted a more systematic approach, using test cases to isolate issues. 

When stuck, we found that taking breaks and revisiting the problem with a new perspective often led to new ideas.

### Backend
The backend went fairly seamlessly. The only problem we had was the Cross Origin Resource Sharing that blocked one of our servers from being able to communicate with the other server. 

### Algorithm
A lot of our time went into reading chess theory and the Minimax theory. We knew that Minimax would work if designed properly, so there was a good relationship between board evaluation and elo. 

There were a lot of bugs, such as:
- sometimes the bot would play the worst possible move
- the king could be captured
- the MoveGenerator would generate impossible moves
- the bot would take a long time to find an optimal move
- if there was no move possible, the bot was unable to recognize it was at a checkmate or stalemate
- lack of collision (capture) detection
- statefulness broke Board responsibility
- capturing was not properly removing the pieces
- bot did not consider forced checkmates
- alphabeta did not properly prune branches that did were clearly unoptimal
- some pieces assumed they could jump over other pieces
- score was not being implemented properly
- pawns did not promote properly and update the queen state
- heuristic failed when certain pieces were going in a different direction
- board eval was too simplistic and would not capture important information like tempo
- compiler optimizations could break certain parts of the algorithm
- multithreaded operations could cause deadlock/massive slowdowns
- dependency issues (in particular gtest.h)
- lack of documentation for kernel functions
- printing the board lacked font support for certain Unicode characters
- memory leak issues
- and many more!

We fixed these by implementing over 100 unit tests with > 95% code coverage.

### Team Development
- This quarter was a rough quarter. Our team had very busy schedules, so we worked almost entirely asynchronously. We had weekly stand ups and meetings about what we did for the team.

- It was hard to get everyone to work together on the same things. Many parts of the project required sequential steps to be completed before we could work on certain parts. Because of this, some people were waiting on other teammates before they could work on their task. 

## Accomplishments that we're proud of

| Name    | Contributions |
|---------|--------------------------|
| Kyle    | Built an entire chess engine + bot + algorithm from scratch, developed highly scalable back end, developed APIs between all systems/servers, managed to pull together the team last minute |
| Cody    |  Developed the firmware for the smart devices and supported development of the chess engine. |
| Anusha  | Frontend of a chess website from scratch, used new languages– TypeScript, React, CSS for the first time, designed scalable and maintainable code, integrated state management with React hooks to handle real-time updates including move animations and game status tracking, connected the frontend and backend for multiplayer functionality |
| Jeremy  | Supported device build-up, HW/SW integration and debugging |
| Matthew | Front-end developer |
| Gowtham | Tested and made sure the chess engine produced the correct moves for each piece; For instance can the queen move to legal and valid spaces on the board. Elo calculations for the chess engine compared to various bots with known elos. |            
## What we learned

| Name    | What We Learned |
|---------|--------------------------|
|Kyle| Golang still remains the second most beautiful language. Golang >> Python >>>> C++ > JS/TS. Also I love Makefiles! |
|Cody| Reverse engineering, hardware hacking, bare-metal firmware development with toolchains |
|Anusha| React, TypeScript, and CSS, breaking down a user interface into reusable components, state management using React hooks, static typing through TypeScript, how to connect backend to frontend |
|Jeremy| Embedded software development on Linux  |
|Matthew| | 
|Gowtham| How we can leverage REST API development to reuse existing work! Also using the gtest library and just c in general |

## What's next for junkfish

- Beat Magnus Carlsen