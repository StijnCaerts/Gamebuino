/*
  https://github.com/ivanseidel/LinkedList
  Vergeet de library niet eerst te installeren in de IDE!
*/

#include <SPI.h>
#include <Gamebuino.h>
#include <LinkedList.h>

extern const byte font5x7[];

// Gebruik het Coordinate object om posities toe te voegen aan de lijst.
// nieuw object aanmaken: Coordinate *c = new Coordinate(x, y)
// krijg x: c->x

class Coordinate {
  public:
    int x;
    int y;

    Coordinate(int x, int y) {
      this->x = x;
      this->y = y;
    };
};

// lijst met posities van de slang
// nieuwe coordinaat toevoegen vooraan in de lijst:
// Coordinate *c = new Coordinate(x,y)
// positions.add(0,c);
LinkedList<Coordinate*> positions;


Gamebuino gb;

void setup() {
  gb.begin();
  gb.display.setFont(font5x7);
  gb.titleScreen(F("Snake"));
  gb.pickRandomSeed();
  gb.battery.show = false;
}

void loop() {
  if (gb.update()) {
    if (gb.buttons.pressed(BTN_C)) {
      gb.titleScreen(F("Snake"));
      gb.battery.show = false;
    }
  }
}
