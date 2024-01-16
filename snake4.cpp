#include<bits/stdc++.h>
#include "inc/SDL.h"
#undef main
using namespace std;
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
const int SNAKE_SIZE = 20;

struct SnakeSegment {
    int x, y;
};

enum class Direction { UP, RIGHT, DOWN, LEFT };

class SnakeGame {
public:
    SnakeGame();
    ~SnakeGame();

    void Run();

private:
    SDL_Window* window;
    SDL_Renderer* renderer;
    std::vector<SnakeSegment> snake;
    Direction snakeDirection;
    int appleX, appleY;
    int score;

    void Init();
    void HandleInput();
    void UpdateGame();
    void Draw();
    void GameOver();
};

SnakeGame::SnakeGame() : window(nullptr), renderer(nullptr), snakeDirection(Direction::RIGHT), score(0) {
    Init();
}

SnakeGame::~SnakeGame() {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void SnakeGame::Init() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
        exit(EXIT_FAILURE);
    }

    window = SDL_CreateWindow("Snake Game", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (window == nullptr) {
        std::cerr << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        exit(EXIT_FAILURE);
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == nullptr) {
        std::cerr << "Renderer could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        exit(EXIT_FAILURE);
    }

    // Initialize snake
    snake.push_back({SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2});

    // Initialize apple
    appleX = rand() % (SCREEN_WIDTH / SNAKE_SIZE) * SNAKE_SIZE;
    appleY = rand() % (SCREEN_HEIGHT / SNAKE_SIZE) * SNAKE_SIZE;
}

void SnakeGame::HandleInput() {
    SDL_Event e;
    while (SDL_PollEvent(&e) != 0) {
        if (e.type == SDL_QUIT) {
            exit(EXIT_SUCCESS);
        } else if (e.type == SDL_KEYDOWN) {
            switch (e.key.keysym.sym) {
                case SDLK_UP:
                    if (snakeDirection != Direction::DOWN) {
                        snakeDirection = Direction::UP;
                    }
                    break;
                case SDLK_DOWN:
                    if (snakeDirection != Direction::UP) {
                        snakeDirection = Direction::DOWN;
                    }
                    break;
                case SDLK_LEFT:
                    if (snakeDirection != Direction::RIGHT) {
                        snakeDirection = Direction::LEFT;
                    }
                    break;
                case SDLK_RIGHT:
                    if (snakeDirection != Direction::LEFT) {
                        snakeDirection = Direction::RIGHT;
                    }
                    break;
            }
        }
    }
}

void SnakeGame::UpdateGame() {
    // Move the snake
    SnakeSegment newHead = snake.front();
    switch (snakeDirection) {
        case Direction::UP:
            newHead.y -= SNAKE_SIZE;
            break;
        case Direction::DOWN:
            newHead.y += SNAKE_SIZE;
            break;
        case Direction::LEFT:
            newHead.x -= SNAKE_SIZE;
            break;
        case Direction::RIGHT:
            newHead.x += SNAKE_SIZE;
            break;
    }

    // Check for collision with walls or itself
    if (newHead.x < 0 || newHead.x >= SCREEN_WIDTH || newHead.y < 0 || newHead.y >= SCREEN_HEIGHT) {
        GameOver();
    }

    for (size_t i = 1; i < snake.size(); ++i) {
        if (newHead.x == snake[i].x && newHead.y == snake[i].y) {
            GameOver();
        }
    }

    // Check for collision with apple
    if (newHead.x == appleX && newHead.y == appleY) {
        // Increase score
        score++;

        // Generate a new random position for the apple
        appleX = rand() % (SCREEN_WIDTH / SNAKE_SIZE) * SNAKE_SIZE;
        appleY = rand() % (SCREEN_HEIGHT / SNAKE_SIZE) * SNAKE_SIZE;

        // Add a new segment to the front of the snake
        snake.insert(snake.begin(), newHead);
    } else {
        // Move the snake by updating its body
        snake.pop_back();
        snake.insert(snake.begin(), newHead);
    }
}

void SnakeGame::Draw() {
    // Clear the screen
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    // Draw the snake
    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255); // Green color for snake
    for (const auto& segment : snake) {
        SDL_Rect snakeRect = {segment.x, segment.y, SNAKE_SIZE, SNAKE_SIZE};
        SDL_RenderFillRect(renderer, &snakeRect);
    }

    // Draw the apple
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); // Red color for apple
    SDL_Rect appleRect = {appleX, appleY, SNAKE_SIZE, SNAKE_SIZE};
    SDL_RenderFillRect(renderer, &appleRect);

    // Update the screen
    SDL_RenderPresent(renderer);
}

void SnakeGame::GameOver() {
    std::cout << "Game Over! Score: " << score << std::endl;
    exit(EXIT_SUCCESS);
}

void SnakeGame::Run() {
    while (true) {
        HandleInput();
        UpdateGame();
        Draw();
        SDL_Delay(100);
    }
}

int main() {
    SnakeGame snakeGame;
    snakeGame.Run();

    return 0;
}
