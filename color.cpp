#include "color.h"
#include <iomanip> // for static cast
#include <algorithm> // to find max/min of a list
#include <cmath> // to find mod between a double and an int

using std::to_string;
using std::string;
using std::max;
using std::min;
using std::fmod;

/* Member variables
  int red;
  int green;
  int blue;

  int hue;
  double saturation;
  double lightness;
*/

// CONSTRUCTORS

// default constructor
color::color()
{
  red = 0;
  green = 0;
  blue = 0;
  hue = 0;
  saturation = 0;
  lightness = 0;
}

// overloaded constructor
color::color(int r, int g, int b) : red{r}, green{g}, blue{b}
{
  RGBtoHSL();
}

// MEMBER FUNCTIONS

// RGB to HSL conversion, formula source:
// https://www.rapidtables.com/convert/color/rgb-to-hsl.html
void color::RGBtoHSL()
{
  // convert int to double, using the iomanip library
  // https://www.geeksforgeeks.org/static_cast-in-cpp
  double r = static_cast <double> (red);
  double g = static_cast <double> (green);
  double b = static_cast <double> (blue);

  r = r / 255.0;
  g = g / 255.0;
  b = b / 255.0;

  // find max value of a list of values, reference:
  // https://cplusplus.com/reference/algorithm/max
  double cmax = max({r, g, b});
  // find min value of a list of values, reference:
  // https://cplusplus.com/reference/algorithm/min
  double cmin = min({r, g, b});
  double delta = cmax - cmin;

  // lightness calculation
  double l = (cmax + cmin) / 2;

  // hue calculation
  double h;

  if(delta == 0)
  {
    h = 0;
  }
  else
  {
    if(cmax == r)
    {
      h = (g - b) / delta;
      // find modulus between a double and an int, reference:
      // https://en.cppreference.com/w/cpp/numeric/math/fmod
      h = fmod(h, 6);
      h = 60 * h;
    }
    else if(cmax == g)
    {
      h = 60 * (((b - r) / delta) + 2);
    }
    else
    {
      h = 60 * (((r - g) / delta) + 4);
    }
  }

  if(h < 0)
  {
    h = h + 360;
  }
  h = int(h + 0.5);

  // saturation calculation
  double s;

  if(delta == 0)
  {
    s = 0;
  }
  else
  {
    s = delta / (1 - fabs((2 * l) - 1));
  }

  // RGB values get converted to HSL values, even though the variable names remain unchanged
  hue = h;
  saturation = s;
  lightness = l;
}

// HSL to RGB conversion, formula source:
// https://www.rapidtables.com/convert/color/hsl-to-rgb.html
void color::HSLtoRGB()
{
  double r, g, b;
  double h = hue;
  double s = saturation;
  double l = lightness;

  double c = fabs(2 * l - 1);
  c = 1 - c;
  c = c * s;

  double x = h / 60;
  x = fmod(x, 2);
  x = fabs(x - 1);
  x = 1 - x;
  x = x * c;

  double m = l - (c / 2);

  // calculate RGB
  if(h >= 0 && h < 60)
  {
    r = c;
    g = x;
    b = 0;
  }
  else if(h >= 60 && h < 120)
  {
    r = x;
    g = c;
    b = 0;
  }
  else if(h >= 120 && h < 180)
  {
    r = 0;
    g = c;
    b = x;
  }
  else if(h >= 180 && h < 240)
  {
    r = 0;
    g = x;
    b = c;
  }
  else if(h >= 240 && h < 300)
  {
    r = x;
    g = 0;
    b = c;
  }
  else
  {
    r = c;
    g = 0;
    b = x;
  }

  r = (r + m) * 255;
  r = (int)(r + 0.5);
  g = (g + m) * 255;
  g = (int)(g + 0.5);
  b = (b + m) * 255;
  b = (int)(b + 0.5);

  red = r;
  green = g;
  blue = b;
}

// adjust brightness of a RGB color
void color::changeBrightness(double brightness)
{
  lightness = brightness;
  HSLtoRGB();
}

int color::getHue()
{
  return hue;
}

// change the hue of the color based on the group we want to move it to
void color::standardizeHue(int destination)
{
  if(destination == 0)
  {
    hue = 0; // standard hue: red
  }
  else if(destination == 1)
  {
    hue = 120; // standard hue: green
  }
  else
  {
    hue = 240; // standard hue: blue
  }

  HSLtoRGB();
}

string color::getRGB()
{
  string colorRGB = "(" + to_string(red) + ", " + to_string(green) + ", " + to_string(blue) + ")";
  
  return colorRGB;
}

string color::getHSL()
{
  string colorHSL = "(" + to_string(hue) + ", " + to_string(lightness) + ", " + to_string(saturation) + ")";

  return colorHSL;
}