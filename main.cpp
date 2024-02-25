#include <fstream>
#include <iostream>
#include <vector>
#include "color.h"
using namespace std;

// FUNCTION PROTOTYPES
// checking input validity
bool isValidRGB(int, int, int);
bool isInRange(int);
// adjustments
void bulkChangeBrightness(vector <color> &, vector <double>);
// categorizing colors
void categorize(vector <color> &, vector <color> &, vector <color> &, vector <color> &);
// redistribute colors to groups
void redistribute(vector <vector <color>> &);
int minSize(vector <vector <color>>);
int maxSize(vector <vector <color>>);
// displaying
void vectorPush(vector <color>, vector <color> &);
void result(vector <color>);

int main() {
  int r;
  int g;
  int b;
  vector <color> colorList;
  // standard brightness
  const vector <double> standardLight = {0.7, 0.5, 0.3};

  // PROMPT INPUT FOR RGB VALUES
  cout << "Greetings!"
       << endl;
  cout << "This program will adjust the provided color palette to optimize the "
          "percentage of people who will be able to easily detect the color "
          "differences by changing the brightness and hue of the colors."
       << endl << endl;
  cout << "To start, please provide at least 2 colors you want to work with "
          "and their respective RGB values, 9 colors max."
       << endl << endl;

  // the code below was adapted from the ColorBrightness sandbox, with some modifications
  do
  {
    cout << "Enter RGB values (0-255) for color " << colorList.size() + 1
         << endl
         << "Or enter values outside of the range to end."
         << endl;
    cout << "Red: ";
    cin >> r;
    cout << "Green: ";
    cin >> g;
    cout << "Blue: ";
    cin >> b;

    if(isValidRGB(r, g, b))
    {
      color input{r, g, b};
      colorList.push_back(input);
    }

    if(colorList.size() > 9)
    {
      break;
    }
  } while(isValidRGB(r, g, b) || colorList.size() < 2);

  // MAKING ADJUSTMENTS
  const int colorNum = colorList.size();
  vector <color> purpleToYellow;
  vector <color> yellowToCyan;
  vector <color> cyanToPurple;

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

      vector <vector <color>> allGroups;
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

// modify the brightness of a vector of colors using another vector of brightness levels
void bulkChangeBrightness(vector <color> & colorVector, vector <double> brightnessVector)
{
  for(int i = 0; i < colorVector.size(); i++)
  {
    colorVector[i].changeBrightness(brightnessVector[i]);
  }
}

// categorizing colors by different groups with similar hues
void categorize(vector <color> & list, vector <color> & group1, vector <color> & group2, vector <color> & group3)
{
  const int colorNum = list.size();
  
  for(int i = 0; i < colorNum; i++)
  {
    int hue = list[0].getHue();
    
    if( (hue >= 311 && hue <= 360) || (hue >= 0 && hue <= 49) )
    {
      group1.push_back(list[0]);
    }
    else if(hue >= 50 && hue <= 180)
    {
      group2.push_back(list[0]);
    }
    else
    {
      group3.push_back(list[0]);
    }

    // remove the first element of a vector, source:
    // https://stackoverflow.com/questions/40656871/remove-from-the-beginning-of-stdvector
    list.erase(list.begin()); // clear the list for later update
  }
}

// redistribute the groups so that no group has more than 3 colors
void redistribute(vector <vector <color>> & all)
{
  while(all[0].size() > 3 || all[1].size() > 3 || all[2].size() > 3)
  {
    color movedColor;
    int minIndex = minSize(all);
    int maxIndex = maxSize(all);

    // move the last color from the biggest group to the smallest
    movedColor = all[maxIndex][all[maxIndex].size() - 1];

    // adjust the hue of the color we want to move to its destination group
    movedColor.standardizeHue(minIndex);

    all[minIndex].push_back(movedColor);
    all[maxIndex].pop_back();
  }
}

// find the vector with minimum size
int minSize(vector <vector <color>> list)
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
int maxSize(vector <vector <color>> list)
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

// push the components of a vector into another
void vectorPush(vector <color> inputVector, vector <color> & destinationVector)
{
  for(int i = 0; i < inputVector.size(); i++)
  {
    destinationVector.push_back(inputVector[i]);
  }
}

// output function
void result(vector <color> list)
{
  ofstream writer;
  string outputFile = "Optimized palette.txt";
  writer.open(outputFile);

  if(writer.is_open())
  {
    writer << "Based on your input, I recommend using this palette:" << endl;
    for(int i = 0; i < list.size(); i++)
    {
      writer << list[i].getRGB() << endl;
    }

    writer.close();
  }

  cout << "\nOptimization completed. Please check the file \"Optimized palette.txt\" for your results." 
       << endl;
}