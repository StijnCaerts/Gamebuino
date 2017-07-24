#include <SPI.h>
#include <Gamebuino.h>

// GLOBAL VARIABLES
Gamebuino gb;
extern const byte font5x7[];

int defaultPlayerWidth = 3;
int defaultPlayerHeight = 16;

class Player {
  public: 
    int score;
    // x and y coordinates are of the top-left corner
    int x;
    int y;
    int width;
    int height;
    
    Player(int x) {
      this->score = 0;
      this->x = x;
      this->width = defaultPlayerWidth;
      this->height = defaultPlayerHeight;
      this->y = (LCDHEIGHT - this->height)/2;
    }
    
    void up() {
      if (this->y > 0) {
        this->y -= 2;
      }
    }
    
    void down() {
      if (this->y + this->height < LCDHEIGHT) {
        this->y += 2;
      }
    }

    void draw() {
      gb.display.fillRect(this->x, this->y, this->width, this->height);
    }
};

Player player1(0);
Player player2(LCDWIDTH - defaultPlayerWidth);

class Ball {
  public:
    int x;
    int y;
    int dim = 6;
    int vx = 3;
    int vy = 3;
    
    Ball() {
      this->x = (LCDWIDTH - this->dim) / 2;
      this->y = (LCDHEIGHT - this->dim) / 2;
    }

    void draw() {
      gb.display.fillRect(this->x, this->y, this->dim, this->dim);
    }
};

Ball ball;

void setup() {
  gb.begin();
  gb.display.setFont(font5x7);
  gb.titleScreen(F("Pong"));
  gb.pickRandomSeed();
  gb.battery.show = false;
}

void loop() {
  if (gb.update()) {
    // if C is pressed, pause game
    if (gb.buttons.pressed(BTN_C)) {
      pauseGame();
    } else {
      // process game
      movePlayer1();
      movePlayer2();
      moveBall();

      // draw
      drawScore();
      player1.draw();
      player2.draw();
      ball.draw();
    }
  }
}

void pauseGame() {
  gb.titleScreen(F("Pong"));
  gb.battery.show = false;
}

void movePlayer1() {
  if (gb.buttons.repeat(BTN_UP, 1)) {
    player1.up();
  }
  if (gb.buttons.repeat(BTN_DOWN, 1)) {
    player1.down();
  }
}

void movePlayer2() {
  // follow midpoint of the ball
  if (((ball.y + ball.dim)/2) < ((player2.y + player2.height)/2)) {
    player2.up();
  } else if (((ball.y + ball.dim)/2) > ((player2.y + player2.height)/2)) {
    player2.down();
  }
}

void moveBall() {
  ball.x += ball.vx;
  ball.y += ball.vy;

  // ball hits top or bottom border
  if (ball.y < 0) {
    ball.y = 0;
    ball.vy *= -1;
    gb.sound.playTick();
  } else if (ball.y + ball.dim > LCDHEIGHT) {
    ball.y = LCDHEIGHT - ball.dim;
    ball.vy *= -1;
    gb.sound.playTick();
  }

  // ball hits player
  if (gb.collideRectRect(ball.x, ball.y, ball.dim, ball.dim, player1.x, player1.y, player1.width, player1.height)) {
    ball.x = player1.x + player1.width;
    ball.vx *= -1;
    gb.sound.playTick();
  } else if (gb.collideRectRect(ball.x, ball.y, ball.dim, ball.dim, player2.x, player2.y, player2.width, player2.height)) {
    ball.x = player2.x - ball.dim;
    ball.vx *= -1;
    gb.sound.playTick();
  }

  // ball hits left or right border
  if (ball.x < 0) {
    player2.score++;
    gb.sound.playOK();
    resetBall();
  } else if ((ball.x + ball.dim) > LCDWIDTH) {
    player1.score++;
    gb.sound.playOK();
    resetBall();
  }

  // reset score if one of the players reaches 10 points
  if ((player1.score == 10) || (player2.score == 10)) {
    player1.score = 0;
    player2.score = 0;
  }
}

void resetBall() {
  ball.x = (LCDWIDTH - ball.dim) / 2;
  ball.y = (LCDWIDTH - ball.dim) / 2;
  ball.vx *= -1;
}

void drawScore() {
  gb.display.fontSize = 2;
  gb.display.cursorX = 15;
  gb.display.cursorY = 16;
  gb.display.print(player1.score);

  gb.display.cursorX = 57;
  gb.display.cursorY = 16;
  gb.display.print(player2.score);
}

