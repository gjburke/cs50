#include "helpers.h"
#include <math.h>
#include <stdio.h>

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    //Iterate over every pixel
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            //Take the average of the rgb values
            BYTE avg = round((image[i][j].rgbtBlue + image[i][j].rgbtGreen + image[i][j].rgbtRed) / 3.0);
            //Assign the average to each rgb color
            image[i][j].rgbtBlue = avg;
            image[i][j].rgbtGreen = avg;
            image[i][j].rgbtRed = avg;
        }
    }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    //Iterate over the rows of pixels
    for (int i = 0; i < height; i++) {
        //Iterate over half the pixel columns
        for (int j = 0; j < width / 2; j++) {
            //Flip the pixels with the other side
            RGBTRIPLE temp = image[i][j];
            image[i][j] = image[i][width - 1 - j];
            image[i][width - 1 - j] = temp;
        }
    }
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    //Make temporary image that serves to put the new pixels and not change the original
    RGBTRIPLE temp[height][width];

    //Iterate over every pixel
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            //Instantiating total values and pixel count
            int totalBlue = 0, totalGreen = 0, totalRed = 0;
            int pixelCount = 0;
            //Add numbers in square of center pixel
            for (int k = i - 1; k <= i + 1; k++) {
                for (int l = j - 1; l <= j + 1; l++) {
                    //Check if the index of the square is inside bounds of image
                    if ((k >= 0 && k < height) && (l >= 0 && l < width)) {
                        totalBlue += image[k][l].rgbtBlue;
                        totalGreen += image[k][l].rgbtGreen;
                        totalRed += image[k][l].rgbtRed;
                        //Keep track of pixel count
                        pixelCount++;
                    }
                }
            }
            //Set new average values of colors to temp image
            temp[i][j].rgbtBlue = round((double)totalBlue / pixelCount);
            temp[i][j].rgbtGreen = round((double)totalGreen / pixelCount);
            temp[i][j].rgbtRed = round((double)totalRed / pixelCount);
        }
    }

    //Copying over the temp image to the original image
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            image[i][j] = temp[i][j];
        }
    }
    return;
}

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    //Make temporary image that serves to put the new pixels and not change the original
    RGBTRIPLE temp[height][width];

    //Define Gx and Gy kernels
    int Gx[3][3] = {{-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1}};
    int Gy[3][3] = {{-1, -2, -1}, {0, 0, 0}, {1, 2, 1}};

    //Iterate over every pixel
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            //Instantiating total values and pixel count
            int GxBlue = 0, GyBlue = 0, GxGreen = 0, GyGreen = 0, GxRed = 0, GyRed = 0;
            int heightCount = 0, widthCount = 0;

            //Add numbers in square of center pixel
            for (int k = i - 1; k <= i + 1; k++) {
                heightCount = k - i + 1;
                //printf("hc: %i k: %i\n", heightCount, k);
                for (int l = j - 1; l <= j + 1; l++) {
                    widthCount = l - j + 1;
                    //printf("wc: %i l: %i\n", widthCount, l);
                    //Check if the index of the square is inside bounds of image
                    if ((k >= 0 && k < height) && (l >= 0 && l < width)) {
                        GxBlue += Gx[heightCount][widthCount] * image[k][l].rgbtBlue;
                        GyBlue += Gy[heightCount][widthCount] * image[k][l].rgbtBlue;
                        GxGreen += Gx[heightCount][widthCount] * image[k][l].rgbtGreen;
                        GyGreen += Gy[heightCount][widthCount] * image[k][l].rgbtGreen;
                        GxRed += Gx[heightCount][widthCount] * image[k][l].rgbtRed;
                        GyRed += Gy[heightCount][widthCount] * image[k][l].rgbtRed;
                    }
                }
            }
            //Make final values
            double finalBlue = sqrt(pow(GxBlue, 2) + pow(GyBlue, 2));
            double finalGreen = sqrt(pow(GxGreen, 2) + pow(GyGreen, 2));
            double finalRed = sqrt(pow(GxRed, 2) + pow(GyRed, 2));

            //Assign final values
            if (finalBlue > 255) {
                temp[i][j].rgbtBlue = 255;
            }
            else {
                temp[i][j].rgbtBlue = (int)round(finalBlue);
            }
            if (finalGreen > 255) {
                temp[i][j].rgbtGreen = 255;
            }
            else {
                temp[i][j].rgbtGreen = (int)round(finalGreen);
            }
            if (finalRed > 255) {
                temp[i][j].rgbtRed = 255;
            }
            else {
                temp[i][j].rgbtRed = (int)round(finalRed);
            }
        }
    }

    //Copying over the temp image to the original image
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            image[i][j] = temp[i][j];
        }
    }
    return;
}
