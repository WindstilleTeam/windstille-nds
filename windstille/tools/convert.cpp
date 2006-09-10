/* To compile this programm:

% g++ -o png2stratagus  png2stratagus.cpp -lpng
*/

/* This programm can be used to fix the palette of a indexed png file
   to be suitable for Stratagus. It works like this:

   1) You create a RGBA image in Gimp

   2) You convert it to indexed with 227 colors
   
   [Generate Optimal Palette # Colors 227] (MAX_COLORS - 1)
   
   3) You run png2stratagus on the image

   4) The final images will be written to out.png in the current
   directory
*/

#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <iostream>
#include <iomanip>
#include <stdio.h>
#include <stdlib.h>
#include <png.h>
#include <assert.h>


struct Glyph
{
  unsigned char chr;

  unsigned char  x;
  unsigned char  y;

  unsigned char  width;
  unsigned char  height;

  unsigned char* data;
};

struct Font
{
  unsigned char height;
  Glyph glyphs[256];
};

class Color
{
public:
  unsigned int red;
  unsigned int green;
  unsigned int blue;

  Color ()
    : red (0), green (255), blue (0)
  {
  }

  Color (int r, int g, int b) 
    : red (r), green (g), blue (b)
  {    
  }
};

class Image
{
private:
  int m_width;
  int m_height;
  int m_transcol;
  std::vector<unsigned char> m_image;
  std::vector<Color> m_palette;
  
public:
  /** Load an image from a given png source */
  Image (const std::string& filename) 
  {
    FILE* fp;
    png_structp png_ptr;
    png_infop info_ptr;
    png_uint_32 pwidth, pheight;
    int bit_depth, color_type, interlace_type, compression_type, filter_type;
    //int row_bytes; // row_bytes is useless when png_set_packing is used

    if ((fp = fopen(filename.c_str (), "rb")) == NULL)
      {
	perror (filename.c_str ());
	exit (EXIT_FAILURE);
      }

    png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING,
				     NULL, NULL, NULL);

    // Use one byte per pixel, always
    png_set_packing(png_ptr);

    info_ptr = png_create_info_struct(png_ptr);

    png_init_io(png_ptr, fp);
    png_read_info(png_ptr, info_ptr);
    png_get_IHDR(png_ptr, info_ptr, &pwidth, &pheight,
		 &bit_depth, &color_type, &interlace_type,
		 &compression_type, &filter_type);

    //row_bytes = png_get_rowbytes(png_ptr, info_ptr);

    //std::cout << "Bit Depth: " << bit_depth << std::endl;
    //std::cout << "ROw Bytes: " << row_bytes << std::endl;
    
    // Create the 'data' array
    png_bytep row_pointers[pheight];
    for (unsigned int i = 0; i < pheight; i++)
      row_pointers[i] = new png_byte[pwidth]; // row_bytes];

    png_read_image(png_ptr, row_pointers);

    if (color_type != PNG_COLOR_TYPE_PALETTE)
      {
        std::cout << "Unsupported color type" << std::endl;
        exit (EXIT_FAILURE);
      }
    
    int num_colors;
    int num_trans = 0;
    png_colorp lpalette;
    png_bytep trans;
    png_color_16p trans_values;

    // Read some more data
    png_get_PLTE(png_ptr, info_ptr, &lpalette, &num_colors);
    png_get_tRNS(png_ptr, info_ptr, &trans, &num_trans, &trans_values);

    // not sure what trans_values stand for

    //for (int i = 0; i < num_trans; ++i)
    //std::cout <<  "transcolors: " << trans_values[i] << std::endl;

    if (num_trans > 1)
      {
	std::cout << "Multiple transcolors not supported" << std::endl;
	exit (EXIT_FAILURE);
      }
    else if (num_trans == 1)
      m_transcol = trans[0];
    else
      m_transcol = -1;

    if (0)
      for (int i = 0; i < num_trans; i++)
        std::cout << "transcolor: " << int(trans[i]) << std::endl;
    
    m_width  = pwidth;
    m_height = pheight;

    m_image.resize(m_width * m_height);

    // Convert the png into our internal data structure
    for (int y = 0; y < m_height; y++)
      {
	for (int i = 0; i < int(pwidth) /*row_bytes*/; i++)
	  {
            //std::cout << std::setw(3) << int(row_pointers[y][i]) << " ";
	    m_image[i + (m_width * y)] = row_pointers[y][i];
	  }
        //std::cout << std::endl;
      }

    assert(num_colors <= 256);
    if (0) std::cout << "Num Colors: " << num_colors << std::endl;

    m_palette.resize(256);
    for (int i = 0; i < num_colors; ++i)
      {
	m_palette[i].red   = lpalette[i].red;
	m_palette[i].green = lpalette[i].green;
	m_palette[i].blue  = lpalette[i].blue;

        if (0)
          std::cout << "Color: " 
                    << m_palette[i].red << ", "
                    << m_palette[i].green << ", "
                    << m_palette[i].blue << std::endl;
      }

    // Following line causes a crash in combination with png_set_packing
    png_destroy_read_struct(&png_ptr, &info_ptr, (png_infopp)NULL);

    fclose (fp);
  }
  
  ~Image () 
  {
  }

  void write_palette16bit(const std::string& filename)
  {
#define RGB5(r,g,b)     ((r)|((g)<<5)|((b)<<10))

    std::ofstream out(filename.c_str());
    for(std::vector<Color>::iterator i = m_palette.begin(); i != m_palette.end(); ++i)
      {
        unsigned short c = RGB5(i->red/8, i->green/8, i->blue/8);
        out.write(reinterpret_cast<char*>(&c), sizeof(unsigned short));
      }
  }

  void write_palette(const std::string& filename)
  {
    std::ofstream out(filename.c_str());
    out.write(reinterpret_cast<char*>(&*m_palette.begin()), m_palette.size()); 
  }

  void write_image(const std::string& filename)
  {
    if (1)
      {
        // Simple dump of the image data
        std::ofstream out(filename.c_str());
        out.write(reinterpret_cast<char*>(&*m_image.begin()), m_image.size());
      }
    else
      {
        std::cout << "Writing tileset" << std::endl;
        std::ofstream out(filename.c_str());
        unsigned char* ptr = &*m_image.begin();
        int num_tiles = (get_width() * get_height()) / 64;
        int pitch = get_width();
        int tile_width = get_width()/8;
        for(int i = 0; i < num_tiles; ++i)
          {
            char* tile_start = reinterpret_cast<char*>(ptr + 64*tile_width*(i/tile_width) + 8*(i%tile_width));
            for(int y = 0; y < 8; ++y)
              {
                out.write(tile_start + y*pitch, 8);
              }
          }
      }
  }

  void set_color (int i, Color c) {
    m_palette[i] = c;
  }

  int get_transcolor () {
    return m_transcol;
  }
  
  int num_colors () {
    return m_palette.size ();
  }

  int get_width () const {
    return m_width;
  }
  
  int get_height () const {
    return m_height;
  }

  unsigned char at(int x, int y) const {
    assert(x >= 0 && x < m_width);
    assert(y >= 0 && y < m_height);
    return m_image[(y * m_width) + x];
  }
};

bool vline_empty(Image& image, int x)
{
  //std::cout << "Vline: X: " << x << "  -  ";
  for(int y = 0; y < image.get_height(); ++y)
    {
      //std::cout << std::setw(4) << (int)image.at(x, y) << " ";
      if (image.at(x, y) != image.get_transcolor())
        {
          //std::cout << "!EMPTY" << std::endl;
          return false;
        }
    }
  //std::cout << std::endl;
  return true;
}

bool hline_emppty(Image& image, int start_x, int end_x, int y)
{
  for(int x = start_x; x < end_x; ++x)
    if (image.at(x, y) != image.get_transcolor())
      return false;
  return true;
}

struct Char
{
  char chr;

  // Pixeldata goes from [x1, x2) and [y1, y2)
  int x;
  int y;
  int w;
  int h;
};

void detect_characters(Image& image, char* str, char* fontname)
{
  Font font;

  font.height = image.get_height();

  for(int i = 0; i < 256; ++i)
    {
      font.glyphs[i].chr    = i;
      font.glyphs[i].x      = 0;
      font.glyphs[i].y      = 0;
      font.glyphs[i].width  = 0;
      font.glyphs[i].height = 0;
      font.glyphs[i].data   = 0;
    }


  int width  = image.get_width();
  int height = image.get_height();

  bool last_was_empty = true;

  int char_start = 0;
  int char_end   = 0;
  
  int i = 0;

  Char last_char;
  last_char.chr = 0;
  last_char.x = 0;
  last_char.y = 0;
  last_char.w = 0;
  last_char.h = 0;

  for(int x = 0; x < width; ++x)
    {
      if (!vline_empty(image, x))
        {
          if (last_was_empty)
            {
              // beginning of char
              last_was_empty = false;
              char_start = x;
            }
          else
            {
              // in a character
            }
        }
      else
        {
          if (!last_was_empty)
            {
              char_end = x;
              // end of char
              int y_start = image.get_height();
              int y_end   = 0;
              for(int y = 0; y < height; ++y)
                {
                  if (!hline_emppty(image, char_start, char_end, y))
                    {
                      y_start = std::min(y, y_start);
                      y_end   = std::max(y+1, y_end);
                    }
                }
              
              if (str[i] == '\0')
                assert(!"Premature end of character list");

              
              Char new_char;
              
              new_char.chr = str[i];
              new_char.x = char_start;
              new_char.y = y_start;
              new_char.w = char_end - char_start;
              new_char.h = y_end - y_start;

              if (last_char.chr != 0)
                { // merge char with previous one
                  int last_x2 = last_char.x + last_char.w;
                  int last_y2 = last_char.y + last_char.h;
                  
                  int x2 = new_char.x + new_char.w;
                  int y2 = new_char.y + new_char.h;

                  new_char.x = std::min(new_char.x, last_char.x);
                  new_char.y = std::min(new_char.y, last_char.y);

                  new_char.w = std::max(x2, last_x2) - new_char.x;
                  new_char.h = std::max(y2, last_y2) - new_char.y;
                }

              if (str[i+1] == str[i])
                { // Character consisting of multiple rects
                  last_char = new_char;
                }
              else
                {
                  // Found a new char
                  Glyph& glyph = font.glyphs[int(new_char.chr)];
                  glyph.x      = 0; // new_char.x; any way to detect this?
                  glyph.y      = new_char.y;
                  glyph.width  = new_char.w;
                  glyph.height = new_char.h;
                  glyph.data   = new unsigned char[glyph.width * glyph.height];
                  
                  for(int y = 0; y < glyph.height; ++y)
                    for(int x = 0; x < glyph.width; ++x)
                      glyph.data[y * glyph.width + x] = image.at(x + new_char.x,
                                                                 y + new_char.y);
                  last_char.chr = 0;
                  last_char.x = 0;
                  last_char.y = 0;
                  last_char.w = 0;
                  last_char.h = 0;
                }
              i += 1;
            }
          last_was_empty = true;
        }
    }

  if (str[i] != '\0')
    {
      std::cout << "Not all characters found, missing: " << str+i << std::endl;
      exit(EXIT_FAILURE);
    }

  std::stringstream out;

  // Write .h file
  out << "// This file is automatically generated, don't edit by hand!" << std::endl;
  for(int i = 0; i < 256; ++i)
    {
      Glyph& glyph = font.glyphs[i];

      if (glyph.data != 0)
        {
          out << "\n// Character: '"<< glyph.chr << "'" << std::endl;
          out << "// Size: " << std::dec << int(glyph.width) << "x" << int(glyph.height) << "\n";
          out << "const unsigned char " << fontname << "_glyph_0x" << std::hex << int(glyph.chr)
              << "[] = {\n";
          for(int y = 0; y < glyph.height; ++y)
            {
              out << "    ";
              for(int x = 0; x < glyph.width; ++x)
                {
                  out << "0x" << std::setw(2) << std::setfill('0') << std::hex << int(glyph.data[y*glyph.width + x]);
                  out << ", ";
                }
              out << std::endl;
            }
          out << "};\n";
        }
    }

  out << std::endl;
  out << "const Font " << fontname << "_font = {\n";
  out << "  " << std::dec << int(font.height) << ", // font height" << std::endl;
  out << "  {\n";
  for(int i = 0; i < 256; ++i)
    {
      Glyph& glyph = font.glyphs[i];
      out << "    { ";
      if (isprint(glyph.chr))
        out << "'"  << (glyph.chr == '\'' || glyph.chr == '\\' ? "\\" : "") << glyph.chr << "', ";
      else
        out << std::dec << int(glyph.chr) << ", ";

        out << std::dec << std::setfill(' ') << std::setw(3) << int(glyph.x) << ", " 
          << std::dec << std::setfill(' ') << std::setw(3) << int(glyph.y) << ", " 
          << std::dec << std::setfill(' ') << std::setw(2) << int(glyph.width) << ", "
          << std::dec << std::setfill(' ') << std::setw(2) << int(glyph.height) << ", ";
      if (glyph.data)
        out << fontname << "_glyph_0x" << std::hex << int(glyph.chr) << " }," << std::endl;
      else
        out << 0 << " }," << std::endl;
    }
  out << "  }\n";
  out << "};" << std::endl;
  out << "\n/* EOF */" << std::endl;

  std::cout << out.str();
}

int main (int argc, char* argv[])
{
  if (argc < 2)
    {
      std::cout << "Usage: " << argv[0] << " OP INFILE OUTFILE" << std::endl;
      std::cout << std::endl;
      std::cout << " OP: findchars" << std::endl;
      exit (EXIT_FAILURE);
    }
  else
    {
      if (strcmp(argv[1], "findchars") == 0)
        {
          if (argc < 5)
            {
              std::cout << "Usage: " << argv[0] << " findchars FILENAME CHARS FONTNAME" << std::endl;
            }
          else
            {
              Image image(argv[2]);
              detect_characters(image, argv[3], argv[4]);
              //image.write_image("/tmp/foo");
            }
        }
      else
        {
          std::cout << "Unknown OP: " << argv[1] << std::endl;
        }
    }
    
  if (0)
    {
      Image image (argv[1]);
      std::cout << image.get_width() << "x" << image.get_height() << std::endl;
      std::cout << "writing " << std::string(argv[2]) + "_pal.bin" << std::endl;
      std::cout << "writing " << std::string(argv[2]) + "_img.bin" << std::endl;
      image.write_palette16bit(std::string(argv[2]) + "_pal.bin");
      image.write_image(std::string(argv[2]) + "_img.bin");
    }
}

// EOF //
