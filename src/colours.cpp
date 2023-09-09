#include "..\include\colours.h"
#include  <vector>

const Color darkGrey = { 26, 31, 40, 255 };
const Color green = { 47, 230, 23, 255 };
const Color red = { 232, 18, 18, 255 };
const Color orange = { 226, 116, 17, 255 };
const Color yellow = { 237, 234, 4, 255 };
const Color purple = { 166, 0, 247, 255 };
const Color cyan = { 21, 204, 209, 255 };
const Color blue = { 13, 64, 216, 255 };
const Color darkGrayGrid = { 86, 86, 96, 255 };
const Color purpleBackground = { 67, 1, 135, 255 }; 
const Color lightPurpleBackground = {104, 64, 144, 255};

std::vector<Color> GetCellColours() {
    //EMPTY - DARKGRAY, O - Yellow, I - SKYBLUE, S - GREEN, Z - RED, L - ORANGE, J - PURPLE, T - PINK
    return { darkGrey, green, red, orange, yellow, purple, cyan, blue };
}
