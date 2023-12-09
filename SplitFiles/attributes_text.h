#ifndef _ATTRTXT_H
#define _ATTRTXT_H

struct FontStruct {
  int               width;  
  int               height;
  const char*       t;
  const char*       o;
  const char*       a;
  const char*       s;
  const char*       h;
};

typedef struct FontStruct AttrFont;

void attr_write_base(AttrFont* font, char letter, int x, int y, char colour, int ink);

void attr_write(AttrFont* font, char letter, int x, int y, char colour);

char random_colour(int ink);

void write_block_base(int x1, int y1, int x2, int y2, char bgcolour);

void write_block(int x1, int y1, int x2, int y2);

int attractor();

int write_fat(char letter, int x, int y, char colour);

int write_thin(char letter, int x, int y, char colour);

#endif