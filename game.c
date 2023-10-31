#include <raylib.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Snake

//----------------------------------------------------------------------------------
// Module functions declaration
//----------------------------------------------------------------------------------
struct SnakePiece;
void DrawBlock(struct SnakePiece piece, int blockSize);
void DrawSnake(struct SnakePiece piece, int blockSize);
void UpdatePosition(struct SnakePiece *piece, int blockSize); 
Vector2 GetApple(int blockSize);
bool getCollision(struct SnakePiece *piece, int headX, int headY);
struct SnakePiece* GetSnakePiece();

//----------------------------------------------------------------------------------
// Structs
//----------------------------------------------------------------------------------
struct SnakePiece {
    struct SnakePiece* next;
    struct SnakePiece* prev;
    int x;
    int y;
    Vector2 direction;
    Vector2 oldLocation;
    bool head;
    int length;
};

//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
int main(void)
{
    srand(time(NULL));
    // Initialization
    //--------------------------------------------------------------------------------------
    bool endScreen = false;
    const int screenWidth = 500;
    const int screenHeight = 500;
    const int blockSize = 10;

    InitWindow(screenWidth, screenHeight, "Snake!"); 

    struct SnakePiece head;
    head.next = NULL;
    head.next = NULL;
    head.x = screenWidth/2;
    head.y = screenHeight/2;
    Vector2 defaultDirection = { (float)1, (float)0 };
    head.direction = defaultDirection;
    bool thisIsHead = true;
    head.head = thisIsHead; 
    head.length = 0;

    struct SnakePiece *tail = &head;

    Vector2 apple = GetApple(blockSize);

    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        // Move head direction based on user input
        if (IsKeyDown(KEY_SPACE) && endScreen) {
            exit(0);
        } if (IsKeyDown(KEY_D)) {
            Vector2 newDirection = { (float)1, (float)0 };
            head.direction = newDirection;
        } if (IsKeyDown(KEY_A)) {
            Vector2 newDirection = { (float)-1, (float)0 };
            head.direction = newDirection;
        } if (IsKeyDown(KEY_W)) {
            Vector2 newDirection = { (float)0, (float)-1 };
            head.direction = newDirection;
        } if (IsKeyDown(KEY_S)) {
            Vector2 newDirection = { (float)0, (float)1 };
            head.direction = newDirection;
        } if (endScreen) {
            Vector2 newDirection = { (float)0, (float)0 };
            head.direction = newDirection;
        }
        // Move head position based on new direction
        // Move the rest of the snake to new positions
        UpdatePosition(&head, blockSize); 

        // Check for collision with apple
        if (head.x == apple.x && head.y == apple.y) {
            head.length = head.length + 1;
            apple = GetApple(blockSize);
            struct SnakePiece *newPiece = GetSnakePiece();
            tail->next = newPiece;
            newPiece->prev = &*tail;
            newPiece->x = tail->oldLocation.x;
            newPiece->y = tail->oldLocation.y;
            tail = newPiece;
        }

        // Wait to draw to the screen
        if (!endScreen) {
        WaitTime(0.3);
        } else {
        WaitTime(0.0);
        }

        //----------------------------------------------------------------------------------
        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();
            bool collision = getCollision(&head, head.x, head.y);
            if (collision) {
                endScreen = true; 

                ClearBackground(RAYWHITE);

                const char* text = "GAME OVER";
                int textSize = MeasureText(text, 50);
                Vector2 textPosition = {(screenWidth - textSize)/2, screenHeight/2};
                const char* text2 = "Press <SPACE> to exit"; 
                textSize = MeasureText(text2, 30);
                Vector2 textPosition2 = {(screenWidth - textSize)/2, screenHeight/2 + 50};

                DrawText("GAME OVER", textPosition.x, textPosition.y, 50, RED);  
                DrawText("Press <SPACE> to exit", textPosition2.x, textPosition2.y, 30, DARKGRAY);  
                DrawText("Score: ", 10, 10, 20, DARKGRAY);
                char lengthStr[3];
                sprintf(lengthStr, "%d", head.length);
                DrawText(lengthStr, 90, 10, 20, DARKGRAY);
            } else {          
                ClearBackground(RAYWHITE);
                DrawSnake(head, blockSize);
                DrawRectangle(apple.x, apple.y, blockSize, blockSize, GREEN);
                DrawText("Score: ", 10, 10, 20, DARKGRAY);
                char lengthStr[3];
                sprintf(lengthStr, "%d", head.length);
                DrawText(lengthStr, 90, 10, 20, DARKGRAY);
            }

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}

void DrawBlock(struct SnakePiece piece, int blockSize) {
    if (piece.head) { 
        DrawRectangle(piece.x, piece.y, blockSize, blockSize, MAROON); 
    } else { 
        DrawRectangle(piece.x, piece.y, blockSize, blockSize, DARKGRAY); 
    } 
}   

void DrawSnake(struct SnakePiece piece, int blockSize) {
    DrawBlock(piece, blockSize);  
    if (piece.next != NULL) {  
        DrawSnake(*piece.next, blockSize); 
    } 
}

void UpdatePosition(struct SnakePiece *piece, int blockSize) {
    if (piece->head) {
        Vector2 oldLocation = { (float)piece->x, (float)piece->y };
        piece->oldLocation = oldLocation; 
        piece->x += piece->direction.x * blockSize;
        piece->y += piece->direction.y * blockSize;
    } else {
        Vector2 oldLocation = { (float)piece->x, (float)piece->y };
        piece->oldLocation = oldLocation; 
        struct SnakePiece prev = *piece->prev;
        piece->x = prev.oldLocation.x;
        piece->y = prev.oldLocation.y;
    } 
    if (piece->next != NULL) {
        UpdatePosition(piece->next, blockSize);
    }
}

Vector2 GetApple(int blockSize) {
    int min = 0;
    int max = 49; 
    int x = (rand() % (max - min + 1) + min) * 10;
    int y = (rand() % (max - min + 1) + min) * 10;
    Vector2 apple = { (float)x, (float)y };
    return apple;
}

struct SnakePiece* GetSnakePiece() {
    struct SnakePiece *piece = (struct SnakePiece*)malloc(sizeof(struct SnakePiece));
    piece->next = NULL;
    piece->prev = NULL;
    piece->x = 0;
    piece->y = 0;
    Vector2 defaultVec = { 0, 0 };
    piece->direction = defaultVec;
    piece->oldLocation = defaultVec;
    return piece;
} 

bool getCollision(struct SnakePiece *piece, int headX, int headY) {
    bool collision = false;
    if (piece->head != true && piece->x == headX && piece->y == headY) {
        collision = true;
    } else if (piece->next != NULL) {
        collision = getCollision(piece->next, headX, headY); 
    }
    return collision;
}
