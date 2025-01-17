#include <GL/glut.h>
#include <iostream>
#include <cstdlib>
#include <vector>
#include <ctime>
#include <sstream>

constexpr float PI = 3.14159265f;
constexpr float MARIO_HEAD_RADIUS = 0.05f;
constexpr float MARIO_BODY_WIDTH = 0.1f;
constexpr float MARIO_BODY_HEIGHT = 0.15f;

constexpr float JUMP_VELOCITY = 0.04f;
constexpr float GRAVITY = 0.009f;
                                                                                    // coins
constexpr float COIN_RADIUS = 0.025f;
constexpr float COIN_SPEED = 0.02f;
constexpr float COIN_APPEAR_INTERVAL = 4.0f;  // Time interval between coin appearances
std::vector<std::pair<float, float>> coins;  // Vector to store coin positions (x, y)
float coinAppearTime = COIN_APPEAR_INTERVAL;  // Time until the next coin appears

                                                                                    //  OBSTACLE
constexpr float OBSTACLE_WIDTH = 0.15f;  // Width of the turtle body
constexpr float OBSTACLE_HEIGHT = 0.1f;  // Height of the turtle body
constexpr float OBSTACLE_HEAD_RADIUS = 0.035f;  // Radius of the turtle head
constexpr float OBSTACLE_SPEED = 0.02f;
constexpr float OBSTACLE_APPEAR_INTERVAL = 3.0f;  // Time interval between appearances
float obstacleX = 1.0f;
float obstacleY = 0.9f;
std::vector<std::pair<float, float>> obstacles;  // Vector to store obstacle positions (x, y)
float obstacleAppearTime = OBSTACLE_APPEAR_INTERVAL;  // Time until the next obstacle appears

constexpr float BACKGROUND_SPEED = 0.005f;
constexpr float SCORE_FONT_SIZE = 0.05f;
constexpr float HEALTH_FONT_SIZE = 0.05f;
constexpr int MAX_SCORE = 100;
constexpr int MAX_HEALTH = 100;
                                                                                    // mario
float marioX = -1.8f;
float marioY = -0.6f;
float marioVelocityY = 0.0f;
float marioJumpTimeLeft = 0.0f;
float coinX = 0.5f;
float coinY = 0.4f;
float score = 0;
int health = MAX_HEALTH;
                                                                                   // apple
constexpr float APPLE_RADIUS = 0.035f;
constexpr float APPLE_LEAF_RADIUS = 0.01f;
constexpr float APPLE_SPEED = 0.02f;
constexpr float APPLE_APPEAR_INTERVAL = 5.0f;
std::vector<std::pair<float, float>> apples;  // Vector to store apple positions (x, y)
float appleAppearTime = APPLE_APPEAR_INTERVAL;  // Time until the next apple appears

                                                                                      //clouds
float cloud1X = 1.0f;
float cloud1Y = 0.8f;
float cloud2X = 0.5f;
float cloud2Y = 0.9f;
float cloud3X = -0.2f;
float cloud3Y = 0.75f;


enum GameState {
    START,
    PLAYING,
    GAME_OVER
};

GameState gameState = START; // Initial game 

void drawStartScreen() {
    glColor3f(0.0, 0.0, 0.0);
    glRasterPos2f(-1.0, 0.2);
    const char* startText = "Press 'k' to start or 'e' to exit";
    for (int i = 0; startText[i] != '\0'; ++i) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, startText[i]);
    }
}

void drawGameOverScreen() {
    glColor3f(1.0, 0.0, 0.0);
    glRasterPos2f(-0.8, 0.0);
    const char* gameOverText = "Game Over! Press 'k' to restart or 'e' to exit";
    for (int i = 0; gameOverText[i] != '\0'; ++i) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, gameOverText[i]);
    }
}



void drawMario() {
    // Head
    glBegin(GL_POLYGON);
    glColor3f(1.0, 0.8, 0.6); 
    for (int i = 0; i < 360; i++) {
        float angle = i * (PI / 180);
        float marioHeadX = marioX + MARIO_HEAD_RADIUS * cos(angle);
        float marioHeadY = marioY + MARIO_HEAD_RADIUS * sin(angle);
        glVertex2f(marioHeadX, marioHeadY);
    }
    glEnd();

    // Mustache
    glBegin(GL_LINES);
    glColor3f(0.0, 0.0, 0.0);  // Black
    glVertex2f(marioX + 0.02f, marioY + 0.01f);
    glVertex2f(marioX + 0.04f, marioY + 0.02f);

    glVertex2f(marioX + 0.04f, marioY + 0.02f);
    glVertex2f(marioX + 0.02f, marioY + 0.03f);
    glEnd();

    // Body
    glBegin(GL_POLYGON);
    glColor3f(1.0, 0.0, 0.0);  // Red 
    glVertex2f(marioX - MARIO_BODY_WIDTH / 2, marioY - MARIO_HEAD_RADIUS * 1.5);
    glVertex2f(marioX + MARIO_BODY_WIDTH / 2, marioY - MARIO_HEAD_RADIUS * 1.5);
    glVertex2f(marioX + MARIO_BODY_WIDTH / 2, marioY - MARIO_BODY_HEIGHT);
    glVertex2f(marioX - MARIO_BODY_WIDTH / 2, marioY - MARIO_BODY_HEIGHT);
    glEnd();

    // Legs
    glBegin(GL_POLYGON);
    glColor3f(0.0, 1.0, 0.0);
    glVertex2f(marioX - MARIO_BODY_WIDTH / 4, marioY - MARIO_BODY_HEIGHT);
    glVertex2f(marioX + MARIO_BODY_WIDTH / 4, marioY - MARIO_BODY_HEIGHT);
    glVertex2f(marioX + MARIO_BODY_WIDTH / 4, marioY - MARIO_BODY_HEIGHT - MARIO_BODY_HEIGHT / 2);
    glVertex2f(marioX - MARIO_BODY_WIDTH / 4, marioY - MARIO_BODY_HEIGHT - MARIO_BODY_HEIGHT / 2);
    glEnd();
    glBegin(GL_POLYGON);
    glColor3f(0.0, 1.0, 0.0);
    glVertex2f(marioX - MARIO_BODY_WIDTH / 2, marioY - MARIO_BODY_HEIGHT - MARIO_BODY_HEIGHT / 2);
    glVertex2f(marioX + MARIO_BODY_WIDTH / 2, marioY - MARIO_BODY_HEIGHT - MARIO_BODY_HEIGHT / 2);
    glVertex2f(marioX + MARIO_BODY_WIDTH / 2, marioY - MARIO_BODY_HEIGHT - MARIO_BODY_HEIGHT);
    glVertex2f(marioX - MARIO_BODY_WIDTH / 2, marioY - MARIO_BODY_HEIGHT - MARIO_BODY_HEIGHT);
    glEnd();
}

void drawObstacle(float x, float y) {
    glBegin(GL_POLYGON);
    glColor3f(0.0, 0.5, 0.0);  // green
    glVertex2f(x - OBSTACLE_WIDTH / 2, y);
    glVertex2f(x + OBSTACLE_WIDTH / 2, y);
    glVertex2f(x + OBSTACLE_WIDTH / 2, y - OBSTACLE_HEIGHT);
    glVertex2f(x - OBSTACLE_WIDTH / 2, y - OBSTACLE_HEIGHT);
    glEnd();

    // Turtle head 
    glBegin(GL_POLYGON);
    glColor3f(0.545, 0.27, 0.075);  // Brown
    for (int i = 0; i < 180; i++) {  // Adjust to create a semi-circle for the head
        float angle = i * (PI / 180);
        float headX = x - OBSTACLE_WIDTH / 2 + OBSTACLE_HEAD_RADIUS * cos(angle);
        float headY = y - OBSTACLE_HEIGHT + OBSTACLE_HEAD_RADIUS * sin(angle);
        glVertex2f(headX, headY);
    }
    glEnd();
}


void drawApple(float x, float y) {
    // Apple body (circle)
    glBegin(GL_POLYGON);
    glColor3f(1.0, 0.0, 0.0);  // Red
    for (int i = 0; i < 360; i++) {
        float angle = i * (PI / 180);
        float appleX = x + APPLE_RADIUS * cos(angle);
        float appleY = y + APPLE_RADIUS * sin(angle);
        glVertex2f(appleX, appleY);
    }
    glEnd();

    // Apple leaf 
    glBegin(GL_POLYGON);
    glColor3f(0.0, 1.0, 0.0);  // Green
    for (int i = 0; i < 360; i++) {
        float angle = i * (PI / 180);
        float leafX = x + APPLE_RADIUS * cos(angle);
        float leafY = y + APPLE_RADIUS + APPLE_LEAF_RADIUS * sin(angle);
        glVertex2f(leafX, leafY);
    }
    glEnd();
}

void drawHealthBar() {
    // Health bar background
    glBegin(GL_POLYGON);
    glColor3f(0.8, 0.8, 0.8);
    glVertex2f(-2.0, -0.2);  // خفض قيمة المحور y هنا
    glVertex2f(-2.0, -0.25);  // خفض قيمة المحور y هنا
    glVertex2f(0.2, -0.25);  // خفض قيمة المحور y هنا
    glVertex2f(0.2, -0.2);  // خفض قيمة المحور y هنا
    glEnd();

    // Health bar
    glBegin(GL_POLYGON);
    glColor3f(0.0, 1.0, 0.0);
    glVertex2f(-2.0, -0.2);  // خفض قيمة المحور y هنا
    glVertex2f(-2.0, -0.25);  // خفض قيمة المحور y هنا
    float healthBarWidth = (static_cast<float>(health) / MAX_HEALTH) * 2.2;  // Adjust the width based on health
    glVertex2f(-2.0 + healthBarWidth, -0.25);  // خفض قيمة المحور y هنا
    glVertex2f(-2.0 + healthBarWidth, -0.2);  // خفض قيمة المحور y هنا
    glEnd();


void drawCoin(float x, float y) {
    glBegin(GL_POLYGON);
    glColor3f(1.0, 1.0, 0.0);  // Yellow
    for (int i = 0; i < 360; i++) {
        float angle = i * (PI / 180);
        float coinX = x + COIN_RADIUS * cos(angle);
        float coinY = y + COIN_RADIUS * sin(angle);
        glVertex2f(coinX, coinY);
    }
    glEnd();
}


void drawScore() {
    std::stringstream scoreText;
    scoreText << "Total Score : " << score;
    glColor3f(0.0, 0.0, 0.0);

    // Adjust the x-coordinate to position the score at the top right
    float xCoordinate = 0.2 - SCORE_FONT_SIZE * 6;  // خفض قيمة المحور x هنا (أخذت في اعتباري توسيع نطاق النافذة إلى 2.2)

    // Adjust the y-coordinate to position the score at the top
    float yCoordinate = -0.4;  // خفض قيمة المحور y هنا

    glRasterPos2f(xCoordinate, yCoordinate);

    for (const char& character : scoreText.str()) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, character);  // تكبير الخط هنا
    }
}


void updateCoins() {
    // Update coin positions
    for (auto& coin : coins) {
        coin.first -= COIN_SPEED;  // Move coins towards Mario
        coin.second = -0.6f;  // Set the y-coordinate to match Mario's level
    }

    coinAppearTime -= 0.01;  
    if (coinAppearTime <= 0) {
        coins.emplace_back(std::make_pair(2.0f, -0.6f));
        coinAppearTime = COIN_APPEAR_INTERVAL + (rand() % 5);  
    }
    // Remove coins that are out of the screen
    coins.erase(std::remove_if(coins.begin(), coins.end(),
        [](const std::pair<float, float>& coin) {
            return coin.first < -2.0f;
        }),
        coins.end());
}

void drawCloud(float x, float y) {
    // Cloud 
    glBegin(GL_POLYGON);
    glColor3f(1.0, 1.0, 1.0);
    for (int i = 0; i < 360; i++) {
        float angle = i * (PI / 180);
        float cloudX = x + 0.1 * cos(angle);
        float cloudY = y + 0.05 * sin(angle);
        glVertex2f(cloudX, cloudY);
    }
    glEnd();
}



void background() {
    // Draw sky background
    glBegin(GL_POLYGON);
    glColor3f(0.529f, 0.808f, 0.922f);
    glVertex2f(-2.0, -1.0);
    glVertex2f(2.0, -1.0);
    glVertex2f(2.0, 1.0);
    glVertex2f(-2.0, 1.0);
    glEnd();

    // Draw clouds
    drawCloud(cloud1X, cloud1Y);
    drawCloud(cloud2X, cloud2Y);
    drawCloud(cloud3X, cloud3Y);

    // Update cloud positions
    cloud1X -= BACKGROUND_SPEED;
    cloud2X -= BACKGROUND_SPEED;
    cloud3X -= BACKGROUND_SPEED;


   
   
    // Check if clouds are out of the screen, then reset their positions
    if (cloud1X < -1.0f) {
        cloud1X = 2.0f;
    }
    if (cloud2X < -1.0f) {
        cloud2X = 2.0f;
    }
    if (cloud3X < -1.0f) {
        cloud3X = 2.0f;
    }
   
}


void updateObstacles() {
    // Update obstacle positions
    for (auto& obstacle : obstacles) {
        obstacle.first -= OBSTACLE_SPEED;  // Move obstacles towards Mario

        // Check for collision with Mario
        if (marioX - MARIO_BODY_WIDTH / 2 < obstacle.first + OBSTACLE_WIDTH / 2 &&
            marioX + MARIO_BODY_WIDTH / 2 > obstacle.first - OBSTACLE_WIDTH / 2 &&
            marioY - MARIO_BODY_HEIGHT < obstacle.second &&
            marioY > obstacle.second - OBSTACLE_HEIGHT) {                 
            health -= 10;                                                        // Collision occurred, decrease health                             
            obstacle.first = -2.0f;                                               // Move the obstacle out of the screen
        }
    }

    // Check if it's time to create a new obstacle
    obstacleAppearTime -= 0.01;  // Decrease by the frame time (16ms)
    if (obstacleAppearTime <= 0) {
        // Create a new obstacle at the right side
        obstacles.emplace_back(std::make_pair(2.0f, -0.6f));
        // Reset the obstacle appearance timer
        obstacleAppearTime = OBSTACLE_APPEAR_INTERVAL + (rand() % 3);  // Randomize the interval
    }
    // Remove obstacles that are out of the screen
    obstacles.erase(std::remove_if(obstacles.begin(), obstacles.end(),
        [](const std::pair<float, float>& obstacle) {
            return obstacle.first < -2.0f;
        }),
        obstacles.end());
}

void updateApples() {
    for (auto& apple : apples) {
        apple.first -= APPLE_SPEED;  
        apple.second = -0.6f;  
    }
    appleAppearTime -= 0.01;  
    if (appleAppearTime <= 0) {
        apples.emplace_back(std::make_pair(2.0f, -0.6f));
        appleAppearTime = APPLE_APPEAR_INTERVAL + (rand() % 5); 
    }
    apples.erase(std::remove_if(apples.begin(), apples.end(),
        [](const std::pair<float, float>& apple) {
            return apple.first < -2.0f;
        }),
        apples.end());
}

void detectCoinCollision() {
    // Iterate over coins to check for collisions
    for (auto& coin : coins) {
        if (marioX - MARIO_BODY_WIDTH / 2 < coin.first + COIN_RADIUS &&
            marioX + MARIO_BODY_WIDTH / 2 > coin.first - COIN_RADIUS &&
            marioY - MARIO_BODY_HEIGHT < coin.second + COIN_RADIUS &&
            marioY > coin.second - COIN_RADIUS) {
            // Collision occurred with coin, increase score and move the coin out of the screen
            score += 10;
            coin.first = -2.0f;
        }
    }

    // Iterate over apples to check for collisions
    for (auto& apple : apples) {
        if (marioX - MARIO_BODY_WIDTH / 2 < apple.first + APPLE_RADIUS &&
            marioX + MARIO_BODY_WIDTH / 2 > apple.first - APPLE_RADIUS &&
            marioY - MARIO_BODY_HEIGHT < apple.second + APPLE_RADIUS &&
            marioY > apple.second - APPLE_RADIUS) {
            // Collision occurred with apple, increase score and move the apple out of the screen
            score += 20;
            apple.first = -2.0f;
        }
    }
}


void update(int value) {
    if (gameState == PLAYING) {
        // Implement Mario's movement and jump logic here
        marioY += marioVelocityY; // Update Mario's vertical position

        if (marioJumpTimeLeft > 0) {
            marioVelocityY = JUMP_VELOCITY; // Apply jump velocity
            marioJumpTimeLeft -= 0.2;
        }
        else if (marioY > -0.6f) {
            marioVelocityY = -JUMP_VELOCITY; // Apply gravity when above ground
        }
        else {
            marioY = -0.6f;
            marioVelocityY = 0.0;
            marioJumpTimeLeft = 0.0; 
        }

        updateObstacles();  
        updateCoins(); 
        updateApples(); 
        detectCoinCollision();  

        
        if (health <= 0) {
            gameState = GAME_OVER;
        }
    }

    glutPostRedisplay();
    glutTimerFunc(16, update, 0);
}

void handleKeyPress(unsigned char key, int x, int y) {
    if (gameState == START) {
        if (key == 'k') {
            marioX = -1.8f;
            marioY = -0.6f;
            marioVelocityY = 0.0f;
            marioJumpTimeLeft = 0.0f;
            coinX = 0.5f;
            coinY = 0.4f;
            obstacleX = 1.0f;
            obstacleY = 0.9f;
            score = 0;
            health = MAX_HEALTH;
            coins.clear();
            obstacles.clear();
            apples.clear();
            coinAppearTime = COIN_APPEAR_INTERVAL;
            obstacleAppearTime = OBSTACLE_APPEAR_INTERVAL;
            appleAppearTime = APPLE_APPEAR_INTERVAL;
            gameState = PLAYING; 
        }
        else if (key == 'e') {
            exit(0);
        }
    }
    else if (gameState == PLAYING) {
        switch (key) {
        case 'a':
            marioX -= 0.05f;
            break;
        case 'd':
            marioX += 0.05f;
            break;
        case 'x':
            if (marioY == -0.6f && marioJumpTimeLeft == 0.0f) {
                marioJumpTimeLeft = 5.0; // Set the jump time (adjust as needed)
            }
            break;
        }
    }

    glutPostRedisplay();
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    background();

    if (gameState == START) {
        drawStartScreen();
    }
    else if (gameState == PLAYING) {
        drawMario();

        for (const auto& obstacle : obstacles) {
            drawObstacle(obstacle.first, obstacle.second);
        }
        for (const auto& coin : coins) {
            drawCoin(coin.first, coin.second);
        }
        for (const auto& apple : apples) {
            drawApple(apple.first, apple.second);
        }

        drawHealthBar();
        drawScore();
    }
    else if (gameState == GAME_OVER) {
        drawGameOverScreen();
    }

    glutSwapBuffers();
}





int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(1200, 600);  
    glutCreateWindow("Super Mario Game");
    glClearColor(1.0, 1.0, 1.0, 1.0);
    glutInitWindowSize(1200, 600);  
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-2.0, 2.0, -1.0, 1.0);

    glutDisplayFunc(display);
    glutKeyboardFunc(handleKeyPress);
    glutTimerFunc(0, update, 0);
    glutMainLoop();

    return 0;
}