#pragma once
#include "../headers/headers.h"


class Camera
{
private:
    float cameraXthposition;
    float cameraYthposition;
    float screenWidth;
    float screenHeight; //data type discussed that ai give me int i personally think that it must be float
    float lerp(float p1, float p2, float weight)
    {

        return p1 + weight * (p2 - p1); //the formula for linear interpolation is (1-weight)*p1 + weight*p2 which can be rearranged to p1 + weight*(p2-p1)
        //one more thing is that weight is the ratio in between current distance and total distance
    }
    //clamp algortham for games
    //min<=limit<=max
    //that can be done only
    //clamp(limit , max , min) = max (min ,  min (v , max))
    //here is  clamping works

    // v=150 max =100 , min = 0
    // min(150,100)=100
    // max(0,100)=100
    // Final = 100 (clamped down)
    // but if
    // v=−20
    // min(−20,100)=−20
    // max(0,−20)=0
    // v=150 max =100 , min = 0
    // min(150,100)=100
    // max(0,100)=100
    // Final = 100 (clamped down)
    // but if
    // v=−20
    // min(−20,100)=−20
    // max(0,−20)=0
    // Final = 0 (clamped up)

    // assuming a and b be two floating points

    // first i pass by reference then return regardless of knowing that we can't return the ref of local var lol :P
    // Camera update logic (smooth follow)
    float mini(float a, float b)
    {
        return (a < b) ? a : b;
    }
    float maxi(float a, float b)
    {
        return (a > b) ? a : b;
    }
    float clamp(float limit, float minVal, float maxVal)
    {

        // return max(minVal, min(limit, maxVal)); you can also use that as its that compact form

        if (limit < minVal)
            return minVal;
        if (limit > maxVal)
            return maxVal;

        return limit;
    }

public:
    Camera(float width, float height) : cameraXthposition(0), cameraYthposition(0), screenWidth(width), screenHeight(height) {}
    
    void followCamera(float playerX, float playerY, float levelWidth, float levelHeight)
    {
        float seprationFromcentreX = playerX - (screenWidth / 2);
        float seprationFromcentreY = playerY + 263 - screenHeight;
        float addingSmoothMovement = 0.198;
        cameraXthposition = lerp(cameraXthposition, seprationFromcentreX, addingSmoothMovement);
        cameraYthposition = lerp(cameraYthposition, seprationFromcentreY, addingSmoothMovement);
        cameraXthposition = clamp(cameraXthposition, 0, levelWidth - screenWidth);
        cameraYthposition = clamp(cameraYthposition, 0, levelHeight - screenHeight);
    }
    
    float toScreenX(float windowCurrentX) const { return windowCurrentX - cameraXthposition; }
    float toScreenY(float windowCurrentY) const { return windowCurrentY - cameraYthposition; }
    float getCamX() const { return cameraXthposition; }
    float getCamY() const { return cameraYthposition; }
    int getScreenW() const { return screenWidth; }
    int getScreenH() const { return screenHeight; }
    
    void moveBy(float dx, float dy, float levelPixelWidth, float levelPixelHeight) {
        cameraXthposition = clamp(cameraXthposition + dx, 0, levelPixelWidth  - screenWidth);
        cameraYthposition = clamp(cameraYthposition + dy, 0, levelPixelHeight - screenHeight);
    }
};