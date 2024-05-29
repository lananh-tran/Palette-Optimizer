# Color Blindness Palette Optimizer

## Description

### Developer

Lan-Anh Tran

### Program Purpose

This is a C++ program that can optimize a color palette obtained from the user’s RGB inputs, thereby maximizing the percentage of people who will be able to easily differentiate the colors in the palette.

### Disclaimer

I wrote this program for a beginner CS college class during the Fall of 2023 in Replit. Therefore, I cannot guarantee that everything will work like how I tested multiple times on Replit. I will try to fix it as I learn more about programming in the future.

## Updates
### v1.0

This program will prompt the user for a palette consisting of 3 different colors in RGB format from the terminal screen, then let the user choose from 3 different brightness levels for each color and adjust them accordingly to optimize the number of people who will be able to easily detect the differences.

My idea came from [this thread](https://stackoverflow.com/questions/50937550/formula-to-increase-brightness-of-rgb) on Stack Overflow.

### v2.0

For the second update, I keep the brightness adjustment feature for 3 or fewer input colors and upgrade my program with the ability to process up to 9 colors by grouping similar colors into 3 groups, redistributing the colors so that no group has more than 3 colors and adjusting their brightness so that as many people can differentiate them as possible.

The way the colors-grouping mechanism works was inspired by the picture below. Through some observations of the color wheel, I have concluded that for most colorblind people, colors can be sorted into 3 main groups - Purple-to-Yellow, Yellow-to-Cyan, and Cyan-to-Purple - where all colors that are in the same group look similar to each other in the eyes of a colorblind person.

After grouping the colors, their brightness levels would be adjusted so that they no longer look similar to each other.

![visibleColorSpectrum](visibleColorSpectrum.jpg)

### v3.0

For the last update, I have fixed a major bug that I didn't notice in the previous update - my program didn't categorize and sort the colors correctly, which resulted in it throwing away half of the colors the user entered and creating duplicates of the other half. The categorize() function (line 147) now works as I intended.

I also adjusted the input prompt loop so that the program will keep asking for input unless it has at least 2 colors since its purpose would be quite meaningless with only 1 or no colors at all.

Last but not least, I've created a new class called "color" that serves as a separate library used in processing colors in RGB and HSL color spaces for my program. This class can also be used in my future projects as well.

## Getting Started

First, compile the program using the command:

```g++ main.cpp color.cpp -std=c++17 -o cvp```

Then run the program:

```./cvp```

When the program runs, do the following:

1. Enter the Red, Green, and Blue values for each color. Make sure to enter values that fall within the interval of 0-255, at least 2 colors, and no more than 9 colors. If you enter values out of this range, the program will quit the input-receiving phase and start to make adjustments to the previous inputs.

2. The program should give you the output as a separate file named "Optimized palette.txt" with a list of RGB values of the adjusted colors, with 3 different levels of brightness and different hues so that most colorblind people can differentiate the colors.

## Demonstration

### Console Input

![image](https://github.com/lananh-tran64/paletteOptimizer/assets/118518981/02e62dd8-6a4e-4329-816d-297a9a4d29c5)

### Output File

![image](https://github.com/lananh-tran64/paletteOptimizer/assets/118518981/f350391d-65d7-43a5-8cc3-036b33fec3cd)

For more details on how my program works, see [Documentation](Documentation.md).
