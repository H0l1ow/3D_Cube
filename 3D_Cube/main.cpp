﻿#include <cmath>
#include <vector>
#include <chrono>
#include <thread>

float A, B, C;

const int width = 80;   
const int height = 25;
float cubeWidth = 5;
std::vector<float> zBuffer(width * height);
std::vector<char> buffer(width * height);
const int backgroundASCIICode = ' ';
const int distanceFromCam = 120;
float horizontalOffset;
float K1 = 40;

float incrementSpeed = 1.0;

float x, y, z;
float ooz;
int xp, yp;
int idx;

float calculateX(int i, int j, int k) {
    return j * sin(A) * sin(B) * cos(C) - k * cos(A) * sin(B) * cos(C) +
        j * cos(A) * sin(C) + k * sin(A) * sin(C) + i * cos(B) * cos(C);
}

float calculateY(int i, int j, int k) {
    return j * cos(A) * cos(C) + k * sin(A) * cos(C) -
        j * sin(A) * sin(B) * sin(C) + k * cos(A) * sin(B) * sin(C) -
        i * cos(B) * sin(C);
}

float calculateZ(int i, int j, int k) {
    return k * cos(A) * cos(B) - j * sin(A) * cos(B) + i * sin(B);
}

void calculateForSurface(float cubeX, float cubeY, float cubeZ, char ch) {
    x = calculateX(cubeX, cubeY, cubeZ);
    y = calculateY(cubeX, cubeY, cubeZ);
    z = calculateZ(cubeX, cubeY, cubeZ) + distanceFromCam;

    ooz = 1 / z;

    xp = static_cast<int>(width / 2 + K1 * ooz * x * 2);
    yp = static_cast<int>(height / 2 + K1 * ooz * y);

    idx = xp + yp * width;
    if (idx >= 0 && idx < width * height) {
        if (ooz > zBuffer[idx]) {
            zBuffer[idx] = ooz;
            buffer[idx] = ch;
        }
    }
}

int main() {

    while (true) {
        std::fill(buffer.begin(), buffer.end(), backgroundASCIICode);
        std::fill(zBuffer.begin(), zBuffer.end(), 0.0f);
        cubeWidth = 20;

        for (float cubeX = -cubeWidth; cubeX < cubeWidth; cubeX += incrementSpeed) {
            for (float cubeY = -cubeWidth; cubeY < cubeWidth;
                cubeY += incrementSpeed) {
                calculateForSurface(cubeX, cubeY, -cubeWidth, '@');
                calculateForSurface(cubeWidth, cubeY, cubeX, '$');
                calculateForSurface(-cubeWidth, cubeY, -cubeX, '~');
                calculateForSurface(-cubeX, cubeY, cubeWidth, '#');
                calculateForSurface(cubeX, -cubeWidth, -cubeY, ';');
                calculateForSurface(cubeX, cubeWidth, cubeY, '+');
            }
        }

        system("cls");
        for (int k = 0; k < width * height; k++) {
            putchar(k % width ? buffer[k] : '\n');
        }

        A += 0.05;
        B += 0.05;
        C += 0.01;
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
    return 0;
}
