//
// Created by palom on 23/10/2022.
//
#include "Image_aos.hpp"
int main()
{
    //const int width = 640;
    //const int height = 400;

    //Image image(width, height);

    //for (int y = 0; y < height; y++)
    //{
    //  for (int x = 0; x < width; x++){
    //       image.SetColor(Color((float)x / (float)width, 1.0f - ((float)x / (float)width), (float)y / (float)height), x, y);
    //   }
    //}
    Image image(0, 0);
    image.Histo("plane.bmp");

}