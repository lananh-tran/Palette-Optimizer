#include <algorithm> // to find max/min of a list
#include <cmath>     // to find mod between a double and an int
#include <fstream>
#include <iomanip> // for static cast
#include <iostream>
#include <vector>
using namespace std;

// DATA STRUCTURES
struct RGB // store RGB values of a single color
{
  int r;
  int g;
  int b;
};

struct HSL // store HSL values for calculations
{
  int h;
  double s;
  double l;
};

// FUNCTION PROTOTYPES
// checking input validity
bool isValidRGB(int, int, int);
bool isInRange(int);
// adjustments
HSL RGBtoHSL(RGB);
RGB HSLtoRGB(HSL);
void changeBrightness(RGB &, double);
void bulkChangeBrightness(vector <RGB> &, vector <double>);
void changeHue(RGB &, int);
// categorizing colors
void categorize(vector <RGB> &, vector <RGB> &, vector <RGB> &, vector <RGB> &);
void vectorPush(vector <RGB>, vector <RGB> &);
// redistribute colors to groups
void redistribute(vector <vector <RGB>> &);
int minSize(vector <vector <RGB>>);
int maxSize(vector <vector <RGB>>);
// displaying
void result(vector <RGB>);

// MAIN FUNCTION
int main() {
  RGB color;
  int red;
  int green;
  int blue;
  vector <RGB> colorList;
  const vector <double> standardLight = {0.7, 0.5, 0.3}; // standard brightness

  // PROMPT INPUT FOR RGB VALUES
  cout << "Greetings!"
       << endl;
  cout << "This program will adjust the provided color palette to optimize the "
          "percentage of people who will be able to easily detect the color "
          "differences by changing the brightness and hue of the colors."
       << endl << endl;
  cout << "To start, please provide the colors you want to work with and their "
          "respective RGB values, 9 colors max."
       << endl << endl;

  // the code below was adapted from the ColorBrightness sandbox, with some modifications
  do
  {
    cout << "Enter RGB values (0-255) for color " << colorList.size() + 1
         << endl
         << "Or enter values outside of the range to end."
         << endl;
    cout << "Red: ";
    cin >> red;
    cout << "Green: ";
    cin >> green;
    cout << "Blue: ";
    cin >> blue;

    if(isValidRGB(red, green, blue))
    {
      color.r = red;
      color.g = green;
      color.b = blue;

      colorList.push_back(color);
    }

    if(colorList.size() > 9)
    {
      break;
    }
  } while(isValidRGB(red, green, blue));

  // MAKING ADJUSTMENTS
  const int colorNum = colorList.size();
  vector <RGB> purpleToYellow;
  vector <RGB> yellowToCyan;
  vector <RGB> cyanToPurple;
  
  if(colorNum <= 9)
  {
    if(colorNum <= 3)
    {
      // convert the entire list of color
      bulkChangeBrightness(colorList, standardLight);
    }
    else
    { 
      // divide colors into 3 groups with & adjust the hues if necessary
      categorize(colorList, purpleToYellow, yellowToCyan, cyanToPurple);
      // result(yellowToCyan);

      vector <vector <RGB>> allGroups;
      allGroups.push_back(purpleToYellow);
      allGroups.push_back(yellowToCyan);
      allGroups.push_back(cyanToPurple);
      
      redistribute(allGroups);

      // modify the brightness
      for(int i = 0; i < allGroups.size(); i++)
      {
        bulkChangeBrightness(allGroups[i], standardLight);
      }

      // combine the 3 groups into a single list of colors
      for(int i = 0; i < allGroups.size(); i++)
      {
        vectorPush(allGroups[i], colorList);
      }
    }

    // OUTPUT TO FILE
    result(colorList);
  }
  else
  {
    cout << "\nError! This program currently cannot process more than 9 colors."
         << endl
         << "Please restart the program."
         << endl;
  }
}

// FUNCTION DEFINITIONS
// isVallidRGB and isInRange was adapted from the ColorBrightness sandbox
bool isValidRGB(int r, int g, int b)
{
  if(isInRange(r) && isInRange(g) && isInRange(b))
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

// RGB to HSL conversion, formula source:
// https://www.rapidtables.com/convert/color/rgb-to-hsl.html
// access and change the values directly
HSL RGBtoHSL(RGB colorRGB)
{
  HSL colorHSL;
  // convert int to double, using the iomanip library
  // https://www.geeksforgeeks.org/static_cast-in-cpp
  double r = static_cast <double> (colorRGB.r);
  double g = static_cast <double> (colorRGB.g);
  double b = static_cast <double> (colorRGB.b);

  r = r / 255.0;
  g = g / 255.0;
  b = b / 255.0;

  // find max value of a list of values, reference:
  // https://cplusplus.com/reference/algorithm/max
  double cmax = std::max({r, g, b});
  // find min value of a list of values, reference:
  // https://cplusplus.com/reference/algorithm/min
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
      // find modulus between a double and an int, reference:
      // https://en.cppreference.com/w/cpp/numeric/math/fmod
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

  // RGB values get converted to HSL values, even though the variable names
  // remain unchanged
  colorHSL.h = h;
  colorHSL.s = s;
  colorHSL.l = l;

  return colorHSL;
}

// HSL to RGB conversion, formula source:
// https://www.rapidtables.com/convert/color/hsl-to-rgb.html
RGB HSLtoRGB(HSL colorHSL)
{
  RGB colorRGBnew;
  double r, g, b;
  double h = colorHSL.h;
  double s = colorHSL.s;
  double l = colorHSL.l;

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

  colorRGBnew.r = r;
  colorRGBnew.g = g;
  colorRGBnew.b = b;

  return colorRGBnew;
}

// RGB to HSL to RGB conversion with adjusted brightness
void changeBrightness(RGB & colorRGB, double brightness)
{
  HSL colorHSL = RGBtoHSL(colorRGB);
  colorHSL.l = brightness;
  colorRGB = HSLtoRGB(colorHSL);
}

// modify the brightness of a vector of colors using another vector of brightness levels
void bulkChangeBrightness(vector <RGB> & colorVector, vector <double> brightnessVector)
{
  for(int i = 0; i < colorVector.size(); i++)
  {
    changeBrightness(colorVector[i], brightnessVector[i]);
  }
}

// categorizing colors by different groups with similar hues
void categorize(vector <RGB> & list, vector <RGB> & group1, vector <RGB> & group2, vector <RGB> & group3)
{
  HSL colorHSL;

  for(int i = 0; i < list.size(); i++)
  {
    colorHSL = RGBtoHSL(list[i]);
    
    if( (colorHSL.h >= 311 && colorHSL.h <= 360) || (colorHSL.h >= 0 && colorHSL.h <= 49) )
    {
      group1.push_back(list[i]);
    }
    else if(colorHSL.h >= 50 && colorHSL.h <= 180)
    {
      group2.push_back(list[i]);
    }
    else
    {
      group3.push_back(list[i]);
    }

    // remove the first element of a vector, source:
    // https://stackoverflow.com/questions/40656871/remove-from-the-beginning-of-stdvector
    list.erase(list.begin()); // clear the list for later update
  }
}

// redistribute the groups so that no group has more than 3 colors
void redistribute(vector <vector <RGB>> & allGroups)
{
  while(allGroups[0].size() > 3 || allGroups[1].size() > 3 || allGroups[2].size() > 3)
  {
    RGB movedColor;
    int minIndex = minSize(allGroups);
    int maxIndex = maxSize(allGroups);

    // move the last color from the biggest group to the smallest
    movedColor = allGroups[maxIndex][allGroups[maxIndex].size() - 1];
    
    // adjust the hue of the color we want to move to its destination group
    changeHue(movedColor, minIndex);
    
    allGroups[minIndex].push_back(movedColor);
    allGroups[maxIndex].pop_back();
  }
}

// find the vector with minimum size
int minSize(vector <vector <RGB>> list)
{
  int minIndex = 0;

  for(int i = 1; i < list.size(); i++)
  {
    if(list[i].size() < list[i-1].size())
    {
      minIndex = i;
    }
  }

  return minIndex;
}

// find the vector with maximum size
int maxSize(vector <vector <RGB>> list)
{
  int maxIndex = 0;

  for(int i = 1; i < list.size(); i++)
  {
    if(list[i].size() > list[i-1].size())
    {
      maxIndex = i;
    }
  }

  return maxIndex;
}

// change the hue of the color based on the group we want to move it to
void changeHue(RGB & color, int group)
{
  HSL colorHSL = RGBtoHSL(color);

  if(group == 0)
  {
    colorHSL.h = 0; // standard hue: red
  }
  else if(group == 1)
  {
    colorHSL.h = 120; // standard hue: green
  }
  else
  {
    colorHSL.h = 240; // standard hue: blue
  }

  color = HSLtoRGB(colorHSL);
}

// push the components of a vector into another
void vectorPush(vector <RGB> inputVector, vector <RGB> & destinationVector)
{
  for(int i = 0; i < inputVector.size(); i++)
  {
    destinationVector.push_back(inputVector[i]);
  }
}

// output function
void result(vector <RGB> list)
{
  ofstream writer;
  string outputFile = "Optimized palette.txt";
  writer.open(outputFile);

  if(writer.is_open())
  {
    writer << "Based on your input, I recommend using this palette:" << endl;
    for(int i = 0; i < list.size(); i++)
    {
      writer << "(" << list[i].r << ", " << list[i].g << ", " << list[i].b << ")" << endl;
    }

    writer.close();
  }

  cout << "\nOptimization completed. Please check the file \"Optimized palette.txt\" for your results." 
       << endl;
}