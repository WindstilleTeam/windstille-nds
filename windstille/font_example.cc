

struct Glyph
{
  char chr;

  unsigned char  x;
  unsigned char  y;

  unsigned char  width;
  unsigned char  height;

  const unsigned char* data;
};

struct Font
{
  unsigned char height;
  Glyph glyphs[256];
};

const unsigned char pixeldata[] = { 1, 2, 3, 4, 5};

Font font = { 0, { 
    { 'a', 0, 0, 5, 5, 0 },
    { 'b', 0, 0, 5, 5, pixeldata+1 },
    // ...
  }};

int main()
{
  
  return 0;
}

/* EOF */
