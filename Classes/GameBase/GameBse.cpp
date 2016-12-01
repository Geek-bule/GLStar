//
//  GameBse.cpp
//  泡你妹改
//
//  Created by justin yang on 13-9-30.
//
//

#include "GameBse.h"
#include "ZYTools.h"

Sprite* GameBase::createSprite(const char *filename,
                               int type/*=TYPE_IMAGE*/,
                               const std::string& label/*=""*/,
                               float size/*=1*/,
                               Vec2 pos/*=Vec2(0,0)*/
){
    std::string show = stringControll(label, 640);
    Sprite* sprte = NULL;
    if (type == TYPE_Image) {
        sprte = Sprite::create(filename);
    }
    if (type == TYPE_Sprite) {
        sprte = Sprite::createWithSpriteFrameName(filename);
    }
    return sprte;
}

MenuItem* GameBase::createButton(const char *normalImage,
                                 const char *selectedImage,
                                 const char *disabledImage,
                                 const ccMenuCallback& callback,
                                 const std::string& label/* =""*/,
                                 float size/* =1*/,
                                 int type/* =TYPE_Image */)
{
    MenuItemImage* image;
    Sprite *normal;
    Sprite *select;
    Sprite *disable;
    MenuItemSprite* sprite;
    MenuItemFont* font;
    MenuItemToggle* togg;
    MenuItemImage* image1;
    MenuItemImage* image2;
    Label *pString = Label::createWithBMFont(BMfontName, label);
    pString->setScale(size);
    
    switch (type) {
        case TYPE_Image:
            image = MenuItemImage::create(normalImage,selectedImage, disabledImage, callback);
            image->addChild(pString);
            pString->setPosition(image->getContentSize().width/2,image->getContentSize().height/2);
            return image;
        case TYPE_Sprite:
            normal = Sprite::createWithSpriteFrameName(normalImage);
            select = Sprite::createWithSpriteFrameName(selectedImage);
            disable = Sprite::createWithSpriteFrameName(disabledImage);
            sprite = MenuItemSprite::create(normal,select, disable, callback);
            sprite->addChild(pString);
            pString->setPosition(sprite->getContentSize().width/2,sprite->getContentSize().height/2);
            return sprite;
        case TYPE_Font:
            font = MenuItemFont::create(normalImage, callback);
            
            return font;
        case TYPE_Togg:
            image1 = MenuItemImage::create(normalImage, selectedImage);
            image2 = MenuItemImage::create(selectedImage, normalImage);
            if (userDefault->getBoolForKey(MUSIC_ONOFF)) {
                togg = MenuItemToggle::createWithCallback(callback, image1,image2,NULL);
            }else{
                togg = MenuItemToggle::createWithCallback(callback, image2,image1,NULL);
            }
            
            return togg;
        default:
            log("不存在这种按钮设置");
            return NULL;
    }
}

std::string GameBase::getCString(const char *path)
{
    std::string res = "language/english.xml";
    LanguageType currentLanguageType = Application::getInstance()->getCurrentLanguage();
    switch (currentLanguageType)
    {
        case LanguageType::ENGLISH:
            res = "language/english.xml";
            break;
        case LanguageType::CHINESE:
            res = "language/chinese.xml";
            break;
        default:
            res = "language/english.xml";
            break;
    }
    __Dictionary *strings = __Dictionary::createWithContentsOfFile(res.c_str());
    std::string chinese = ((__String*)strings->objectForKey(path))->getCString();
    return chinese;
}

std::string GameBase::getPicName(const char* picName)
{
    std::string res = "_en.png";
    LanguageType currentLanguageType = Application::getInstance()->getCurrentLanguage();
    switch (currentLanguageType)
    {
        case LanguageType::ENGLISH:
            res = "_en.png";
            break;
        case LanguageType::CHINESE:
            res = "_zh.png";
            break;
        default:
            res = "_en.png";
            break;
    }
    __String* localPicName = __String::createWithFormat("%s%s",picName,res.c_str());
    return localPicName->_string;
}

std::string GameBase::stringControll(std::string str,int length)
{
    unsigned int beginPos = 0;  //字符串的初始位置
    std::string resultStr;      //返回的字符串
    
    log("string size is %lu",str.size());
    std::vector<std::string> str_vec;  //创建一个字符串类型的顺序容器
    do
    {
        str_vec.push_back(str.substr(beginPos,length)); //substr函数的作用类似剪刀，将str中从beginPos到length之间的字符串剪下来，单独放入容器中
        if (beginPos+length >str.size())
        {
            break;  //当要裁剪的长度超出str的长度，则退出循环
        }
        else
        {
            beginPos += length;
        }
            
    } while (true);
        
    for (unsigned int i = 0;i<str_vec.size();++i) {
        resultStr.append(str_vec.at(i)).append("\n");
        //从容器逐一取出之前裁剪好的一段段字符串，分别在字符串后面加上换行符。append()类似胶水，将\n粘到字符串后面
    }
    resultStr.pop_back();
    //这一句是将最后一个多余的\n给删掉
    return resultStr;
}




