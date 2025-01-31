#include "raylib.h"
#include <iostream>
#include <string>
#include <vector>
#include <fstream>

// Constants
const int SCREEN_WIDTH = 1800;
const int SCREEN_HEIGHT =900;
const int MAX_INPUT_CHARS = 20;
const Color BUTTON_COLOR = { 50, 120, 190, 255 };
const Color BUTTON_HOVER_COLOR = { 70, 140, 210, 255 };
const Color CORRECT_COLOR = { 0, 200, 0, 255 };
const Color WRONG_COLOR = { 200, 0, 0, 255 };

// Game states
enum GameState {
    MENU,
    NAME_INPUT,
    QUIZ,
    SCOREBOARD,
    GAME_OVER
};

// Question structure
struct Question {
    std::string text;
    std::vector<std::string> options;
    char correctAnswer;
};

// Global variables
GameState currentState = MENU;
std::vector<Question> questions;
int currentQuestion = 0;
int score = 0;
int lives = 5;
char name[MAX_INPUT_CHARS + 1] = "\0";
int nameInputIndex = 0;
bool mouseOnText = false;
float messageTimer = 0;
std::string feedbackMessage = "";
Color feedbackColor = WHITE;

// Function declarations
void LoadQuestions();
void DrawMenu();
void DrawNameInput();
void DrawQuiz();
void DrawScoreboard();
void DrawGameOver();
void SaveScore();
Rectangle CreateButton(float x, float y, float width, float height, const char* text, Color color);
void DrawQuestionCard(const Question& question, int questionNumber);
void ShowFeedback(const char* message, Color color);

void LoadQuestions() {
    questions.clear();

    questions.push_back({ "Which of the following is used to print output in C++?",
     {"cin", "cout", "clog", "cerr"}, 'b' });

    questions.push_back({ "What is the default value of an uninitialized int in C++?",
        {"0", "1", "Garbage value", "Undefined"}, 'c' });

    questions.push_back({ "Which of the following is a valid C++ data type?",
        {"integer", "real", "boolean", "float"}, 'd' });

    questions.push_back({ "Which of the following is used to read input from the user in C++?",
        {"cin", "cout", "scanf", "getchar"}, 'a' });

    questions.push_back({ "Which C++ loop is guaranteed to execute at least once?",
        {"while", "for", "do-while", "foreach"}, 'c' });

    questions.push_back({ "Which function is used to find the length of a string in C++?",
        {"length()", "size()", "strlen()", "strlength()"}, 'c' });

    questions.push_back({ "What is the correct way to declare a constant in C++?",
        {"const int x;", "int x = const;", "int const x;", "constant x;"}, 'a' });

    questions.push_back({ "Which of the following is the correct syntax for a C++ function?",
        {"function func();", "void func();", "int func();", "func();"}, 'b' });

    questions.push_back({ "Which header file is required for input/output operations in C++?",
        {"iostream", "stdlib.h", "string.h", "stdio.h"}, 'a' });

    questions.push_back({ "Which of the following is used to exit a program in C++?",
        {"exit(0);", "break;", "return 0;", "terminate();"}, 'a' });

    questions.push_back({ "Which of the following is NOT a valid variable name in C++?",
        {"int x;", "float y;", "double 1var;", "char z;"}, 'c' });

    questions.push_back({ "What is the output of the following C++ code: cout << 5 + 3 * 2;",
        {"16", "11", "13", "8"}, 'b' });

    questions.push_back({ "Which of the following is the correct syntax to declare a class in C++?",
        {"class MyClass;", "class MyClass {}", "class MyClass[];", "MyClass class;"}, 'a' });

    questions.push_back({ "Which C++ feature allows objects to have different implementations?",
        {"Polymorphism", "Encapsulation", "Abstraction", "Inheritance"}, 'a' });

    questions.push_back({ "Which of the following is used to declare a pointer in C++?",
        {"int ptr;", "int* ptr;", "int& ptr;", "ptr* int;"}, 'b' });

    questions.push_back({ "What will the following C++ code snippet output? cout << 10 / 4;",
        {"2.5", "2", "3", "1"}, 'b' });

    questions.push_back({ "What is the correct syntax for an if-else statement in C++?",
        {"if x = 1 { } else { }", "if(x == 1) { } else { }", "if x == 1 then { } else { }", "if x == 1: { } else: { }"}, 'b' });

    questions.push_back({ "What does the 'continue' statement do in a loop in C++?",
        {"Ends the loop", "Skips the current iteration", "Exits the function", "Does nothing"}, 'b' });

    questions.push_back({ "Which of the following is the correct way to declare a two-dimensional array in C++?",
        {"int arr[5][5];", "int[5,5] arr;", "int arr(5,5);", "arr[5][5] int;"}, 'a' });

    questions.push_back({ "What will the following C++ code output? cout << 'A' + 1;",
        {"B", "A", "Error", "C"}, 'a' });

    questions.push_back({ "Which of the following is the correct syntax to declare a reference in C++?",
        {"int &x;", "int x&;", "int & x;", "int ref x;"}, 'a' });

    questions.push_back({ "Which of the following is the correct syntax for a for loop in C++?",
        {"for(i = 0; i < 10; i++) { }", "for(i = 0; i < 10) { }", "for(i < 10; i++) { }", "for(i = 0: i < 10; i++) { }"}, 'a' });

    questions.push_back({ "What is the purpose of the 'sizeof' operator in C++?",
        {"To get the size of a variable", "To print the size of a variable", "To compare sizes", "To allocate memory"}, 'a' });

    questions.push_back({ "Which of the following is used to read a file in C++?",
        {"ifstream", "ofstream", "fstream", "fileReader"}, 'a' });

    questions.push_back({ "Which of the following is a valid way to define a string in C++?",
        {"string s = 'Hello';", "string s = \"Hello\";", "char s = \"Hello\";", "string s = Hello;"}, 'b' });
}
// main function
int main() {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Mega Quiz");
    SetTargetFPS(60);

    LoadQuestions();

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(RAYWHITE);

        switch (currentState) {
        case MENU:
            DrawMenu();
            break;
        case NAME_INPUT:
            DrawNameInput();
            break;
        case QUIZ:
            DrawQuiz();
            break;
        case SCOREBOARD:
            DrawScoreboard();
            break;
        case GAME_OVER:
            DrawGameOver();
            break;
        }

        if (messageTimer > 0) {
            DrawText(feedbackMessage.c_str(),
                SCREEN_WIDTH / 2 - MeasureText(feedbackMessage.c_str(), 30) / 2,
                SCREEN_HEIGHT - 100, 30, feedbackColor);
            messageTimer -= GetFrameTime();
        }

        EndDrawing();
    }

    CloseWindow();
    return 0;
}

void DrawMenu() {
    const char* title = "MEGA QUIZ PLATFORM";
    DrawText(title, SCREEN_WIDTH / 2 - MeasureText(title, 50) / 2, 100, 50, BLACK);

    Rectangle startButton = CreateButton(SCREEN_WIDTH / 2 - 150, 300, 300, 60, "Start Quiz", BUTTON_COLOR);
    Rectangle scoreButton = CreateButton(SCREEN_WIDTH / 2 - 150, 400, 300, 60, "Scoreboard", BUTTON_COLOR);
    Rectangle exitButton = CreateButton(SCREEN_WIDTH / 2 - 150, 500, 300, 60, "Exit", BUTTON_COLOR);

    if (CheckCollisionPointRec(GetMousePosition(), startButton) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        currentState = NAME_INPUT;
        nameInputIndex = 0;
        name[0] = '\0';
    }
    else if (CheckCollisionPointRec(GetMousePosition(), scoreButton) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        currentState = SCOREBOARD;
    }
    else if (CheckCollisionPointRec(GetMousePosition(), exitButton) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        CloseWindow();
        exit(0);  // Ensure the program actually exits
    }
}

void DrawNameInput() {
    DrawText("Enter Your Name", SCREEN_WIDTH / 2 - MeasureText("Enter Your Name", 40) / 2, 200, 40, BLACK);

    Rectangle textBox = { SCREEN_WIDTH / 2 - 200, 300, 400, 50 };
    DrawRectangleRec(textBox, LIGHTGRAY);
    DrawRectangleLines(textBox.x, textBox.y, textBox.width, textBox.height, BLACK);
    DrawText(name, textBox.x + 10, textBox.y + 10, 40, BLACK);

    Rectangle startButton = CreateButton(SCREEN_WIDTH / 2 - 100, 400, 200, 50, "Start", BUTTON_COLOR);

    if (CheckCollisionPointRec(GetMousePosition(), textBox)) {
        SetMouseCursor(MOUSE_CURSOR_IBEAM);
        mouseOnText = true;
    }
    else {
        SetMouseCursor(MOUSE_CURSOR_DEFAULT);
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) mouseOnText = false;
    }

    if (mouseOnText) {
        int key = GetCharPressed();
        while (key > 0) {
            if ((key >= 32) && (key <= 125) && (nameInputIndex < MAX_INPUT_CHARS)) {
                name[nameInputIndex] = (char)key;
                name[++nameInputIndex] = '\0';
            }
            key = GetCharPressed();
        }

        if (IsKeyPressed(KEY_BACKSPACE)) {
            nameInputIndex--;
            if (nameInputIndex < 0) nameInputIndex = 0;
            name[nameInputIndex] = '\0';
        }
    }

    if (CheckCollisionPointRec(GetMousePosition(), startButton) &&
        IsMouseButtonPressed(MOUSE_LEFT_BUTTON) &&
        nameInputIndex > 0) {
        currentState = QUIZ;
        currentQuestion = 0;
        score = 0;
        lives = 5;
    }
}

void DrawQuestionCard(const Question& question, int questionNumber) {
    const float cardWidth = SCREEN_WIDTH - 200;
    const float cardHeight = SCREEN_HEIGHT - 200;
    const float cardX = 100;
    const float cardY = 100;

    DrawRectangle(cardX, cardY, cardWidth, cardHeight, LIGHTGRAY);

    std::string questionText = "Q" + std::to_string(questionNumber + 1) + ". " + question.text;
    DrawText(questionText.c_str(), cardX + 20, cardY + 20, 30, BLACK);

    for (int i = 0; i < 4; i++) {
        float buttonY = cardY + 150 + (i * 80);
        std::string optionText = std::string(1, char('A' + i)) + ") " + question.options[i];
        Rectangle optionButton = CreateButton(cardX + 20, buttonY, cardWidth - 40, 60,
            optionText.c_str(), BUTTON_COLOR);

        if (CheckCollisionPointRec(GetMousePosition(), optionButton) &&
            IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            // checking the correct or wrong
            if (char('a' + i) == question.correctAnswer) {
                score++;
                ShowFeedback("Correct!", CORRECT_COLOR);   
            }
            else {
                lives--;
                ShowFeedback("Wrong!", WRONG_COLOR);
            }

            if (lives <= 0) {
                currentState = GAME_OVER;
                SaveScore();
            }
            else if (currentQuestion < questions.size() - 1) {
                currentQuestion++;
            }
            else {
                currentState = GAME_OVER;
                SaveScore();
            }
        }
    }
}

void DrawQuiz() {
    if (currentQuestion < questions.size() && lives > 0) {
        DrawQuestionCard(questions[currentQuestion], currentQuestion);

        DrawText(TextFormat("Score: %d/%d", score, (int)questions.size()), 20, 20, 30, BLACK);
        DrawText(TextFormat("Lives: %d", lives), SCREEN_WIDTH - 150, 20, 30, RED);
    }
}

void SaveScore() {
    const char* fileName = "scores.txt";
    std::ofstream file(fileName, std::ios::app);
    if (file.is_open()) {
        file << "Name: " << name << " - Score: " << score << "/" << questions.size() << "\n";
        file.close();
        std::cout << "Score saved successfully to " << fileName << std::endl;
    }
    else {
        std::cerr << "Error: Could not open file " << fileName << " for writing" << std::endl;
    }
}

void DrawScoreboard() {
    DrawText("SCOREBOARD", SCREEN_WIDTH / 2 - MeasureText("SCOREBOARD", 40) / 2, 50, 40, BLACK);

    const char* fileName = "scores.txt";
    std::ifstream file(fileName);
    std::string line;
    int yPos = 150;

    if (file.is_open()) {
        while (getline(file, line) && yPos < SCREEN_HEIGHT - 100) {
            DrawText(line.c_str(), 50, yPos, 20, BLACK);
            yPos += 30;
        }
        file.close();
    }
    else {
        DrawText("No scores available", SCREEN_WIDTH / 2 - MeasureText("No scores available", 30) / 2,
            SCREEN_HEIGHT / 2, 30, GRAY);
    }

    Rectangle backButton = CreateButton(SCREEN_WIDTH / 2 - 100, SCREEN_HEIGHT - 80, 200, 50,
        "Back to Menu", BUTTON_COLOR);

    if (CheckCollisionPointRec(GetMousePosition(), backButton) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        currentState = MENU;
    }
}

void DrawGameOver() {
    DrawText("Game Over!", SCREEN_WIDTH / 2 - MeasureText("Game Over!", 50) / 2, 200, 50, BLACK);
    DrawText(TextFormat("Final Score: %d/%d", score, (int)questions.size()),
        SCREEN_WIDTH / 2 - MeasureText(TextFormat("Final Score: %d/%d", score, (int)questions.size()), 40) / 2,
        300, 40, BLACK);

    Rectangle menuButton = CreateButton(SCREEN_WIDTH / 2 - 150, 400, 300, 60, "Back to Menu", BUTTON_COLOR);
    Rectangle playAgainButton = CreateButton(SCREEN_WIDTH / 2 - 150, 500, 300, 60, "Play Again", BUTTON_COLOR);

    if (CheckCollisionPointRec(GetMousePosition(), menuButton) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        currentState = MENU;
    }
    else if (CheckCollisionPointRec(GetMousePosition(), playAgainButton) &&
        IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        currentState = NAME_INPUT;
        nameInputIndex = 0;
        name[0] = '\0';
    }
}

Rectangle CreateButton(float x, float y, float width, float height, const char* text, Color color) {
    Rectangle button = { x, y, width, height };
    Color currentColor = color;

    if (CheckCollisionPointRec(GetMousePosition(), button)) {
        currentColor = BUTTON_HOVER_COLOR;
    }

    DrawRectangleRec(button, currentColor);
    DrawRectangleLines(x, y, width, height, BLACK);

    int fontSize = 20;
    int textWidth = MeasureText(text, fontSize);
    DrawText(text, x + (width - textWidth) / 2, y + (height - fontSize) / 2, fontSize, WHITE);

    return button;
}

void ShowFeedback(const char* message, Color color) {
    feedbackMessage = message;
    feedbackColor = color;
    messageTimer = 2.0f;
}