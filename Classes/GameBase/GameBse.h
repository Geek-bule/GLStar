//
//  GameBse.h
//  泡你妹改
//
//  Created by justin yang on 13-9-30.
//
//

#ifndef ________GameBse__
#define ________GameBse__

#include <iostream>
#include "PublicDefine.h"

#define TYPE_Image       1
#define TYPE_Sprite      2
#define TYPE_Font        3
#define TYPE_Togg       4

class GameBase
{
public:
    static Sprite* createSprite(const char* filename ,
                                int type = TYPE_Image,
                                const std::string& label = "",
                                float size = 1.0,
                                Vec2 pos = Vec2(0, 0));
    static MenuItem* createButton(const char *normalImage,
                                  const char *selectedImage,
                                  const char *disabledImage,
                                  const ccMenuCallback& callback,
                                  const std::string&label ="",
                                  float size = 1.0,
                                  int type = TYPE_Image);
    static std::string getCString(const char *path);
    
    static std::string getPicName(const char* picName);
    
    static std::string stringControll(std::string str,int length);
private:
    
};

#endif /* defined(________GameBse__) */
