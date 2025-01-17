#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include <iostream>
#include <string>
#include <cmath>

// Screen dimension constants
const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

// Ball properties
const int BALL_RADIUS = 20;
int ballX = SCREEN_WIDTH / 2;
int ballY = SCREEN_HEIGHT / 2;
int ballVelX = 3; // Initial speed
int ballVelY = 3; // Initial speed
const int INITIAL_SPEED = 3;

// Platform properties
const int PLATFORM_WIDTH = 100;
const int PLATFORM_HEIGHT = 20;
int platformX = (SCREEN_WIDTH - PLATFORM_WIDTH) / 2;
int platformY = SCREEN_HEIGHT - PLATFORM_HEIGHT - 10;
const int PLATFORM_SPEED = 10;

// Score properties
int score = 0;
TTF_Font* font = nullptr;
SDL_Color textColor = { 0, 0, 0, 255 };

// Background textures
SDL_Texture* menuBackground = nullptr;
SDL_Texture* gameBackground = nullptr;

// Game state
enum GameState {
    MENU,
    PLAYING,
    PAUSED
};

GameState currentState = MENU;

// Initialize SDL
bool init() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cout << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
        return false;
    }
    if (TTF_Init() == -1) {
        std::cout << "SDL_ttf could not initialize! SDL_ttf Error: " << TTF_GetError() << std::endl;
        return false;
    }
    if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
        std::cout << "SDL_image could not initialize! SDL_image Error: " << IMG_GetError() << std::endl;
        return false;
    }
    return true;
}

// Create window
SDL_Window* createWindow() {
    SDL_Window* window = SDL_CreateWindow("Bouncing Ball with Platform", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (window == nullptr) {
        std::cout << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
    }
    return window;
}

// Create renderer
SDL_Renderer* createRenderer(SDL_Window* window) {
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == nullptr) {
        std::cout << "Renderer could not be created! SDL_Error: " << SDL_GetError() << std::endl;
    }
    return renderer;
}

// Load font
TTF_Font* loadFont(const std::string& path, int fontSize) {
    TTF_Font* font = TTF_OpenFont(path.c_str(), fontSize);
    if (font == nullptr) {
        std::cout << "Failed to load font! SDL_ttf Error: " << TTF_GetError() << std::endl;
    }
    return font;
}

// Load texture
SDL_Texture* loadTexture(const std::string& path, SDL_Renderer* renderer) {
    SDL_Texture* texture = IMG_LoadTexture(renderer, path.c_str());
    if (texture == nullptr) {
        std::cout << "Failed to load texture! SDL_image Error: " << IMG_GetError() << std::endl;
    }
    return texture;
}

// Close SDL
void close(SDL_Window* window, SDL_Renderer* renderer, TTF_Font* font, SDL_Texture* menuBackground, SDL_Texture* gameBackground) {
    SDL_DestroyTexture(menuBackground);
    SDL_DestroyTexture(gameBackground);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_CloseFont(font);
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
}

// Draw text
void drawText(SDL_Renderer* renderer, TTF_Font* font, const std::string& text, int x, int y) {
    SDL_Color textColor = { 0, 0, 0, 255 };
    SDL_Surface* surface = TTF_RenderText_Solid(font, text.c_str(), textColor);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    SDL_Rect textRect = { x, y, surface->w, surface->h };
    SDL_RenderCopy(renderer, texture, nullptr, &textRect);
    SDL_DestroyTexture(texture);
}

// Main menu
void mainMenu(SDL_Window* window, SDL_Renderer* renderer, TTF_Font* font, bool& quit, bool& startGame) {
    bool menuRunning = true;
    SDL_Event e;

    while (menuRunning) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = true;
                menuRunning = false;
            } else if (e.type == SDL_MOUSEBUTTONDOWN) {
                int mouseX, mouseY;
                SDL_GetMouseState(&mouseX, &mouseY);

                if (mouseX >= 350 && mouseX <= 450 && mouseY >= 250 && mouseY <= 300) {
                    startGame = true;
                    menuRunning = false;
                } else if (mouseX >= 350 && mouseX <= 450 && mouseY >= 350 && mouseY <= 400) {
                    quit = true;
                    menuRunning = false;
                }
            }
        }

        // Clear screen
        SDL_RenderClear(renderer);

        // Render menu background
        SDL_RenderCopy(renderer, menuBackground, nullptr, nullptr);

        // Draw menu text
        drawText(renderer, font, "Play", 350, 250);
        drawText(renderer, font, "Exit", 350, 350);

        // Update screen
        SDL_RenderPresent(renderer);
    }
}

// Pause menu
void pauseMenu(SDL_Window* window, SDL_Renderer* renderer, TTF_Font* font, bool& quit, bool& resumeGame) {
    bool menuRunning = true;
    SDL_Event e;

    while (menuRunning) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = true;
                menuRunning = false;
            } else if (e.type == SDL_MOUSEBUTTONDOWN) {
                int mouseX, mouseY;
                SDL_GetMouseState(&mouseX, &mouseY);

                if (mouseX >= 350 && mouseX <= 450 && mouseY >= 250 && mouseY <= 300) {
                    resumeGame = true;
                    menuRunning = false;
                } else if (mouseX >= 350 && mouseX <= 450 && mouseY >= 350 && mouseY <= 400) {
                    quit = true;
                    menuRunning = false;
                }
            }
        }

        // Clear screen
        SDL_RenderClear(renderer);

        // Render game background
        SDL_RenderCopy(renderer, gameBackground, nullptr, nullptr);

        // Draw pause menu text
        drawText(renderer, font, "Resume", 350, 250);
        drawText(renderer, font, "Quit", 350, 350);

        // Update screen
        SDL_RenderPresent(renderer);
    }
}

int main(int argc, char* args[]) {
    if (!init()) {
        std::cout << "Failed to initialize!" << std::endl;
    } else {
        SDL_Window* window = createWindow();
        SDL_Renderer* renderer = createRenderer(window);

        font = loadFont("../Assets/Poppins-Regular.ttf", 28); // Replace with the path to your font file
        menuBackground = loadTexture("../Assets/bg-icebergs-1.png", renderer); // Replace with the path to your menu background image
        gameBackground = loadTexture("../Assets/bg-icebergs-1.png", renderer); // Replace with the path to your game background image


        bool quit = false;
        bool startGame = false;
        bool resumeGame = false;

        // Show main menu
        mainMenu(window, renderer, font, quit, startGame);

        if (startGame) {
            currentState = PLAYING;
            bool gameOver = false;
            SDL_Event e;

            while (!quit && !gameOver) {
                while (SDL_PollEvent(&e) != 0) {
                    if (e.type == SDL_QUIT) {
                        quit = true;
                    } else if (e.type == SDL_KEYDOWN) {
                        if (e.key.keysym.sym == SDLK_ESCAPE) {
                            currentState = MENU;
                            mainMenu(window, renderer, font, quit, startGame);
                            if (startGame) {
                                currentState = PLAYING;
                            }
                        } else if (e.key.keysym.sym == SDLK_p) {
                            currentState = PAUSED;
                        }
                    }
                }

                if (currentState == PLAYING) {
                    // Get keyboard state
                    const Uint8* currentKeyStates = SDL_GetKeyboardState(nullptr);
                    if (currentKeyStates[SDL_SCANCODE_LEFT]) {
                        platformX -= PLATFORM_SPEED;
                        if (platformX < 0) {
                            platformX = 0;
                        }
                    }
                    if (currentKeyStates[SDL_SCANCODE_RIGHT]) {
                        platformX += PLATFORM_SPEED;
                        if (platformX + PLATFORM_WIDTH > SCREEN_WIDTH) {
                            platformX = SCREEN_WIDTH - PLATFORM_WIDTH;
                        }
                    }

                    // Update ball position
                    ballX += ballVelX;
                    ballY += ballVelY;

                    // Check for collision with walls
                    if (ballX - BALL_RADIUS < 0 || ballX + BALL_RADIUS > SCREEN_WIDTH) {
                        ballVelX = -ballVelX;
                    }
                    if (ballY - BALL_RADIUS < 0) {
                        ballVelY = -ballVelY;
                    }

                    // Check for collision with platform
                    if (ballY + BALL_RADIUS >= platformY &&
                        ballY + BALL_RADIUS <= platformY + PLATFORM_HEIGHT &&
                        ballX + BALL_RADIUS >= platformX &&
                        ballX - BALL_RADIUS <= platformX + PLATFORM_WIDTH) {
                        ballVelY = -ballVelY;
                        score += 10;


                        // Increase ball speed by 15% on every strik
                        ballVelX = static_cast<int>(std::round(ballVelX * 1.15));
                        ballVelY = static_cast<int>(std::round(ballVelY * 1.15));
                        // Increase ball speed by 15% every 100 points
                        // if (score % 100 == 0) {
                        //     ballVelX = static_cast<int>(std::round(ballVelX * 1.15));
                        //     ballVelY = static_cast<int>(std::round(ballVelY * 1.15));

                        //     // Ensure the ball speed does not become zero
                        //     if (ballVelX == 0) ballVelX = 1;
                        //     if (ballVelY == 0) ballVelY = 1;
                        // }
                    }

                    // Check for game over
                    if (ballY + BALL_RADIUS > SCREEN_HEIGHT) {
                        gameOver = true;
                    }

                    // Clear screen
                    SDL_RenderClear(renderer);

                    // Render game background
                    SDL_RenderCopy(renderer, gameBackground, nullptr, nullptr);

                    // Draw ball
                    SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF);
                    for (int w = 0; w < BALL_RADIUS * 2; w++) {
                        for (int h = 0; h < BALL_RADIUS * 2; h++) {
                            int dx = BALL_RADIUS - w;
                            int dy = BALL_RADIUS - h;
                            if ((dx * dx + dy * dy) <= (BALL_RADIUS * BALL_RADIUS)) {
                                SDL_RenderDrawPoint(renderer, ballX - BALL_RADIUS + w, ballY - BALL_RADIUS + h);
                            }
                        }
                    }

                    // Draw platform
                    SDL_Rect platformRect = { platformX, platformY, PLATFORM_WIDTH, PLATFORM_HEIGHT };
                    SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0xFF, 0xFF);
                    SDL_RenderFillRect(renderer, &platformRect);

                    // Draw score
                    std::string scoreText = "Score: " + std::to_string(score);
                    SDL_Surface* surface = TTF_RenderText_Solid(font, scoreText.c_str(), textColor);
                    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
                    SDL_FreeSurface(surface);

                    SDL_Rect textRect = { 10, 10, surface->w, surface->h };
                    SDL_RenderCopy(renderer, texture, nullptr, &textRect);
                    SDL_DestroyTexture(texture);

                    // Draw "Press ESC to Exit"
                    std::string escText = "Press ESC to Exit";
                    surface = TTF_RenderText_Solid(font, escText.c_str(), textColor);
                    texture = SDL_CreateTextureFromSurface(renderer, surface);
                    SDL_FreeSurface(surface);

                    SDL_Rect escRect = { 570, 10, surface->w, surface->h };
                    SDL_RenderCopy(renderer, texture, nullptr, &escRect);
                    SDL_DestroyTexture(texture);

                    // Update screen
                    SDL_RenderPresent(renderer);

                    // Cap the frame rate
                    SDL_Delay(16); // approximately 60 frames per second
                } else if (currentState == PAUSED) {
                    pauseMenu(window, renderer, font, quit, resumeGame);
                    if (resumeGame) {
                        currentState = PLAYING;
                        resumeGame = false;
                    }
                }
            }

            if (gameOver) {
                std::cout << "Game Over! Final Score: " << score << std::endl;
            }
        }

        close(window, renderer, font, menuBackground, gameBackground);
    }

    return 0;
}
