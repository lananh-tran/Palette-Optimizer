#ifndef COLOR_H
#define COLOR_H

#include <string>
using std::string;

class color
{
  private:
    // RGB
    int red;
    int green;
    int blue;
    // HSL
    int hue;
    double saturation;
    double lightness;
    // conversions
    void RGBtoHSL();
    void HSLtoRGB();

  public:
    // default constructor
    color();
    // overloaded constructor
    color(int, int, int);
    // adjustments
    void changeBrightness(double);
    void standardizeHue(int);
    // output
    string getRGB();
    string getHSL();
    int getHue();
};

#endif