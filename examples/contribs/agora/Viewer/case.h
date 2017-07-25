#include <iostream>

#include <qpoint.h>

class Case {
public:
  Case(){};

  void initFrom(int binary);
  operator int() const;

  int nbTop() const { return nbTop_; }
  int nbBottom() const { return nbBottom_; }
  bool topIsBlack() const { return topIsBlack_; }
  int topAltitude() const { return altitude_ + nbBottom_ + nbTop_; }

  friend std::ostream &operator<<(std::ostream &out, const Case &c);
  friend std::istream &operator>>(std::istream &in, Case &c);

  void draw(const QPoint &pos) const;
  void drawTop(const QPoint &pos) const;
  void drawTopPiece(const QPoint &pos) const;
  void drawTopPieceTop(const QPoint &pos) const;
  void drawPieces(const QPoint &pos) const;

  void removePiece();
  void addPiece(bool under, bool black);

private:
  static void drawParallelepiped(float size, float height);
  static void drawSquare(float size);
  void drawBasis(const QPoint &pos) const;
  void drawPiece(const QPoint &pos, int altitude, bool black) const;
  void checkForRevolution();

  int altitude_;
  bool topIsBlack_;
  int nbTop_, nbBottom_;
};
