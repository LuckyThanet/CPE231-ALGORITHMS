# Mastermind Code-Breaking Game Project

## Introduction
Mastermind is a code-breaking game involving two players: the code creator and the codebreaker. The objective is for the codebreaker to guess the code as quickly as possible. The game can vary in difficulty, with options such as decoding numbers, colored pegs, or other symbols arranged in sequences of 4 positions with 6 colors or 5 positions with 8 colors.

## Gameplay Overview
1. The code creator sets up a secret code.
2. The codebreaker attempts to guess the code by selecting a sequence of colors or numbers.
3. The code creator provides feedback:
   - **Xi**: Number of correct colors/numbers in the correct positions.
   - **Yi**: Number of correct colors/numbers in incorrect positions.

The development team identified that this game can be adapted for programming by generating a set of `n` numbers (1, 2, ..., `n`) in `r` positions (1, 2, ..., `r`). The total number of possible combinations is `Cn,r`. The program randomly selects a code and uses initial guesses, applying algorithms to generate the next best guesses.

## Project Objectives
This project aims to develop the Mastermind game using three different programming approaches:
1. **C Language using Genetic Algorithm** on [Replit](https://replit.com/).
2. **Wolfram Language** using Genetic Algorithm in **Wolfram Mathematica**.
3. **C Language using Brute Force Algorithm** on [Replit](https://replit.com/).

The performance of the two algorithms (Genetic and Brute Force) will be compared by calculating the average number of guesses required to crack the code over 3 game trials. The data will be presented in a table for analysis.

## Implementation Details

### 1. C Language with Genetic Algorithm
- Implemented on Replit.
- Uses Genetic Algorithm to evolve guesses to find the best possible match close to the secret code.
- Each iteration calculates:
  - **Xi**: Correct numbers in the correct positions.
  - **Yi**: Correct numbers in incorrect positions.

### 2. Wolfram Language with Genetic Algorithm
- Implemented in Wolfram Mathematica.
- Utilizes built-in functionalities for Genetic Algorithm.
- Focuses on generating a new guess that progressively aligns with the feedback of previous attempts.

### 3. C Language with Brute Force Algorithm
- Implemented on Replit.
- Explores all possible combinations methodically.
- Evaluates the effectiveness of brute force by comparing the average number of attempts over 3 games.

## Results and Comparison
- The project will present a table summarizing the average number of attempts required for each algorithm to crack the code across 3 trials.
- This comparison will help identify which method is more efficient for code-breaking in the context of the Mastermind game.

## Conclusion
This project will provide insights into the performance of Genetic Algorithm versus Brute Force Algorithm in the context of the Mastermind code-breaking game. The results will inform the best algorithm for efficient guessing strategies.

## Authors
This project is developed by a team aiming to explore algorithmic solutions in game-based problem-solving and to enhance programming skills through different approaches.
