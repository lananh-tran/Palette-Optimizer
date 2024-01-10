#include <iostream>
#include <vector>
#include <algorithm> // to find max/min of a list
#include <cmath> // to find mod between a double and an int
#include <iomanip> // for static cast
using namespace std;

// FUNCTION PROTOTYPES
// checking input
bool isValidRGB(int, int, int);
bool isInRange(int);
bool foundDuplicate(vector <int>, int);
// conversions
vector <double> RGBtoHSL(int, int, int);
vector <int> HSLtoRGB(double, double, double);
// displaying
void result(vector <int>, vector <double>, vector <double>, vector <double>);

// MAIN FUNCTION
int main()
{
  int red;
  int green;
  int blue;
  vector <int> redList;
  vector <int> greenList;
  vector <int> blueList;

  // PROMPT INPUT FOR RGB VALUES
  // total 3 colors
  cout << "Greetings!" << endl;
  cout << "This program will adjust the provided color palette to optimize the percentage of people who will be able to easily detect the color differences by changing the brightness level of each color."
    << endl << endl;
  cout << "To start, please provide 3 colors." << endl << endl;
  // the code below was adapted from the ColorBrightness sandbox, with some modifications of my own
  while(redList.size() < 3)
  {
    cout << "Enter RGB values (0-255) for color " << redList.size() + 1 << endl;
    cout << "Red: ";
    cin >> red;
    cout << "Green: ";
    cin >> green;
    cout << "Blue: ";
    cin >> blue;

    if(isValidRGB(red, green, blue))
    {
      redList.push_back(red);
      greenList.push_back(green);
      blueList.push_back(blue);
    }
    else
    {
      cout << endl << "Your entered input(s) doesn't fall within the interval of 0-255"
        << endl << "Try again." << endl << endl;
    }
  }

  // PROMPT INPUT FOR BRIGHTNESS LEVEL
  vector <int> brightness;
  int n = 3;
  // number of colors we're dealing with, might need to adjust it later if decide to deal with infinite number of colors
  int level;
  
  cout << "\nThere are " << n << " levels of brightness for you to choose from, with level 1 being the brightest.\n";
  cout << "\nEnter the desired brightness for color 1, 2 and 3 by typing the number 1, 2 or 3.\n";

  while(brightness.size() < n)
  {
    cout << "Brightness for color " << brightness.size() + 1 << ": ";
    cin >> level;

    if(level >= 1 && level <= 3)
    {
      brightness.push_back(level);
      
      // determine if the same value is entered twice or more
      if(foundDuplicate(brightness, level) == 1) // duplicate(s) found
      {
        brightness.pop_back();
        cout << endl << "You can't choose the same brightness level for 2 or more colors."
          << endl << "Try again." << endl << endl;
      }
    }
    else
    {
      cout << endl << "Your entered input(s) doesn't fall within the interval 1-3"
        << endl << "Try again." << endl << endl;
    }
  }

  // RGB TO HSL CONVERSION
  vector <double> HSL;
  vector <double> HList;
  vector <double> SList;
  vector <double> LList;

  for(int i = 0; i < n; i++)
  {
    HSL = RGBtoHSL(redList[i], greenList[i], blueList[i]);

    HList.push_back(HSL[0]);
    SList.push_back(HSL[1]);
    LList.push_back(HSL[2]);
  }

  // SET NEW LIGHTNESS LEVEL
  vector <double> newLight = {0.7, 0.5, 0.3};
  
  // HSL TO RGB CONVERSION WITH MODIFIED LIGHTNESS
  vector <int> RGB;
  vector <double> newRList;
  vector <double> newGList;
  vector <double> newBList;

  for(int i = 0; i < n; i++)
  {
    int lightNum = brightness[i] - 1;
    RGB = HSLtoRGB(HList[i], SList[i], newLight[lightNum]);
    
    newRList.push_back(RGB[0]);
    newGList.push_back(RGB[1]);
    newBList.push_back(RGB[2]);
  }  

  // OUTPUT
  result(brightness, newRList, newGList, newBList);
}

// FUNCTION DEFINITIONS
// isVallidRGB and isInRange was adapted from the ColorBrightness sandbox
bool isValidRGB(int r, int g, int b)
{
  if (isInRange(r) && isInRange(g) && isInRange(b))
  {
      return 1;
  }
  else
  {
      return 0;
  }
}

bool isInRange(int num)
{
  return num >= 0 && num <= 255;
}

// detect if the user has entered the same value twice or more for the brightness levels
bool foundDuplicate(vector <int> list, int item)
{
  if(list.size() > 1)
  {
    for(int i = 1; i < list.size(); i++)
    {
      if(list[i-1] == item)
      {
        return 1;
      }
    }
  }

  return 0;
}

// RGB to HSL conversion, formula source:
// https://www.rapidtables.com/convert/color/rgb-to-hsl.html
vector <double> RGBtoHSL(int m, int n, int p)
{ 
  vector <double> HSL;
  // convert int to double, using the iomanip library
  // https://www.geeksforgeeks.org/static_cast-in-cpp
  double r = static_cast <double> (m);
  double g = static_cast <double> (n);
  double b = static_cast <double> (p);
  
  r = r / 255.0;
  g = g / 255.0;
  b = b / 255.0;

  // find max value of a list of values, reference: https://cplusplus.com/reference/algorithm/max
  double cmax = std::max({r, g, b});
  // find min value of a list of values, reference: https://cplusplus.com/reference/algorithm/min
  double cmin = std::min({r, g, b});
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
      // find modulus between a double and an int, reference: https://en.cppreference.com/w/cpp/numeric/math/fmod
      h = std::fmod(h, 6);
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
    s = delta / (1 - abs((2 * l) - 1));
  }

  HSL.push_back(h);
  HSL.push_back(s);
  HSL.push_back(l);

  return HSL;
}

// HSL to RGB conversion, formula source:
// https://www.rapidtables.com/convert/color/hsl-to-rgb.html
vector <int> HSLtoRGB(double h, double s, double l)
{
  vector <int> RGB;
  double r;
  double g;
  double b;
  
  double c = abs(2 * l - 1);
  c = 1 - c;
  c = c * s;
  
  double x = h / 60;
  x = fmod(x, 2);
  x = abs(x - 1);
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

  RGB.push_back(r);
  RGB.push_back(g);
  RGB.push_back(b);
  
  return RGB;
}

// output function
void result(vector <int> l, vector <double> r, vector <double> g, vector <double> b)
{
  cout << "\nYour adjusted colors are:" << endl;

  for (int i = 0; i < l.size(); i++)
  {
    cout << "(" << r[i] << ", " << g[i] << ", " << b[i] << ")" << endl;
  }
}