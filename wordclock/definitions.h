// convert xy coordinates to index
uint16_t XY( uint8_t x, uint8_t y)
{
  // initally used to ignore a dead pixel,
  // probably not necessary anymore
  uint16_t offset = 0;
  
  if(y % 2 == 0)
    return y * (MATRIX_WIDTH) + x - offset;
  else
    return y * (MATRIX_WIDTH) + (MATRIX_WIDTH-1-x) - offset;
}

const int EIN[4] = {
    3,
    XY(2, 4),
    XY(3, 4),
    XY(4, 4)
};

const int EINS[5] = {
    4,
    XY(2, 4),
    XY(3, 4),
    XY(4, 4),
    XY(5, 4)
};

const int ZWEI[5] = {
    4,
    XY(7, 4),
    XY(8, 4),
    XY(9, 4),
    XY(10, 4)
};

const int DREI[5] = {
    4,
    XY(0, 5),
    XY(1, 5),
    XY(2, 5),
    XY(3, 5)
};

const int VIER[5] = {
    4,
    XY(5, 5),
    XY(6, 5),
    XY(7, 5),
    XY(8, 5)
};

const int FUENF[5] = {
    4,
    XY(1, 6),
    XY(2, 6),
    XY(3, 6),
    XY(4, 6)
};

const int SECHS[6] = {
    5,
    XY(6, 6),
    XY(7, 6),
    XY(8, 6),
    XY(9, 6),
    XY(10, 6)
};

const int SIEBEN[7] = {
    6,
    XY(0, 7),
    XY(1, 7),
    XY(2, 7),
    XY(3, 7),
    XY(4, 7),
    XY(5, 7)
};

const int ACHT[5] = {
    4,
    XY(7, 7),
    XY(8, 7),
    XY(9, 7),
    XY(10, 7)
};

const int NEUN[5] = {
    4,
    XY(2, 8),
    XY(3, 8),
    XY(4, 8),
    XY(5, 8)
};

const int ZEHN[5] = {
    4,
    XY(6, 8),
    XY(7, 8),
    XY(8, 8),
    XY(9, 8)
};

const int ELF[4] = {
    3,
    XY(0, 9),
    XY(1, 9),
    XY(2, 9)
};

const int ZWOELF[6] = {
    5,
    XY(3, 9),
    XY(4, 9),
    XY(5, 9),
    XY(6, 9),
    XY(7, 9)
};

const int UHR[4] = {
    3,
    XY(8, 9),
    XY(9, 9),
    XY(10, 9)
};

const int NACH[5] = {
    4,
    XY(0, 3),
    XY(1, 3),
    XY(2, 3),
    XY(3, 3)
};

const int VOR[4] = {
    3,
    XY(8, 2),
    XY(9, 2),
    XY(10, 2)
};

const int VIERTEL[8] = {
    7,
    XY(0, 2),
    XY(1, 2),
    XY(2, 2),
    XY(3, 2),
    XY(4, 2),
    XY(5, 2),
    XY(6, 2)
};

const int HALB[5] = {
    4,
    XY(5, 3),
    XY(6, 3),
    XY(7, 3),
    XY(8, 3)
};

const int MIN_FUENF[6] = {
    4,
    XY(7, 0),
    XY(8, 0),
    XY(9, 0),
    XY(10, 0)
};

const int MIN_ZEHN[5] = {
    4,
    XY(0, 1),
    XY(1, 1),
    XY(2, 1),
    XY(3, 1)
};

const int MIN_ZWANZIG[8] = {
    7,
    XY(4, 1),
    XY(5, 1),
    XY(6, 1),
    XY(7, 1),
    XY(8, 1),
    XY(9, 1),
    XY(10, 1)
};

