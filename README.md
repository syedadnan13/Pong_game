# Pong_game using C++ and RayLib library
This is a Pong game written in C++ using the Raylib library. Pong is one of the earliest arcade video games, originally released in 1972, and is a table tennis sports game featuring simple two-dimensional graphics. This implementation builds upon the classic Pong concept, adding features like difficulty levels and dynamic gameplay adjustments.
The game allows the player to compete against a CPU opponent

# Features
1. Classic Pong Gameplay:  
    * Two paddles (Player and CPU) and a ball.  
    * The objective is to score points by hitting the ball past the opponent's paddle.
2. Difficulty Levels:
    * Players can choose between Easy, Medium, and Difficult modes at the start of the game.
    * The CPU's reaction speed adjusts based on the selected difficulty.
3. Game Over Conditions:
    * If the CPU wins, the game ends, and the player is prompted to restart and vice versa
    * The high score is displayed and saved to a file.
4. High Score System:
    * The game tracks and saves the highest score achieved by the player.
5. Dynamic Gameplay:
    * The ball bounces off walls and paddles, with its speed increasing as levels progress.
    * The CPU paddle dynamically adjusts its movement based on the ball's position and difficulty level.
# How to Play
1. Controls:
    * Up Arrow: Move the player's paddle up.
    * Down Arrow: Move the player's paddle down.
    * R key: Restart the game after a loss.
2. Objective:
    * Score points by hitting the ball past the CPU paddle.
    * Avoid letting the ball pass your paddle.
3. Winning and Losing:
    * Win: Reduce the CPU's lives to 0 to win
    * Lose: If the player's lives reach 0, the game ends, and the high score is displayed.
# Customization
You can modify the game by tweaking the following parameters in the code:
1. Ball Speed:
    * Adjust the ball_speed_increment variable to change how much the ball's speed increases per level.
2. Paddle Size:
    * Modify the player_paddle_height variable to change the initial size of the player's paddle.
    * Adjust the shrinking factor in the ShowWinner() function to control how much the paddle shrinks per level.
3. Lives:
    * Change the player_lives and cpu_lives variables to set the initial number of lives.
4. Difficulty:
    * Adjust the CPU's reaction speed in the CpuPaddle::Update() function for each difficulty level.
# Credits
This game was developed using the Raylib library, a simple and easy-to-use library for game development in C++. The game is inspired by the classic Pong arcade game.
