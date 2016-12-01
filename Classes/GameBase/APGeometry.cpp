//
//  APGeometry.cpp
//  pandajump2
//
//  Created by li shiyong on 12-11-13.
//
//

#include "APGeometry.h"

//当前屏幕为基准的
CCPoint APCCPointMake(float rateX, float rateY)
{
    CCSize vs = CCDirector::sharedDirector()->getVisibleSize();
    CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();
    
    return ccpAdd(origin, ccp(vs.width * rateX, vs.height * rateY));
}
//以640x960为基准的
CCPoint APCCPointMake_(float rateX, float rateY)
{
    CCSize vs_ = CCDirector::sharedDirector()->getVisibleSize();
    CCSize vs = CCSizeMake(640, 960);
    CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();
    CCPoint padd = CCPointMake((vs_.width-vs.width)/2, (vs_.height-vs.height)/2);
    origin = ccpAdd(origin, padd);
    return ccpAdd(origin, ccp(vs.width * rateX, vs.height * rateY));
}

CCSize APCCSizeMake(float rateW, float rateH)
{
    CCSize vs = CCDirector::sharedDirector()->getVisibleSize();
    return CCSizeMake(vs.width * rateW, vs.height * rateH);
}

CCPoint APCCPointRateMake(CCNode* node, float rateX, float rateY){
    CCSize size = node->getContentSize();
    CCPoint position = ccp(size.width * rateX, size.height * rateY);
    return position;
}

void APAddChildForRate(CCNode* parent, CCNode* child, float rateX, float rateY, int zOrder)
{
    CCSize parentSize = parent->getContentSize();
    CCPoint childPosition = ccp(parentSize.width * rateX, parentSize.height * rateY);
    child->setPosition(childPosition);
    /*
    if (-1 == zOrder) {
        parent->addChild(child);
    }else{
        parent->addChild(child, zOrder);
    }
     */
    
    parent->addChild(child, zOrder);
}

void fitToVisibleSize(CCNode *pNode)
{
    /*
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    CCSize vs = CCDirector::sharedDirector()->getVisibleSize();
    float scaleX = vs.width/winSize.width;
    float scaleY = vs.height/winSize.height;
    pNode->setScaleX(scaleX);
    pNode->setScaleY(scaleY);
     */
}

CCPoint APScaleRate(){
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    CCSize vs = CCDirector::sharedDirector()->getVisibleSize();
    float scaleX = vs.width/winSize.width;
    float scaleY = vs.height/winSize.height;
    
    return ccp(scaleX, scaleY);
}

CCPoint APGetPointAtTime(float time, CCArray* pPaths, float duration)
{
    CCPoint rPoint = CCPointZero;
    
    //处理各种边缘情况
    if (pPaths==NULL || pPaths->count() == 0) {
        return rPoint;
    }
    if (pPaths->count() < 2 || time >= duration) {
        CCPoint *lastPoint = (CCPoint*)pPaths->lastObject();
        rPoint = ccp(lastPoint->x,lastPoint->y);
        return rPoint;
    }
    if (time == 0.0) {
        CCPoint *firstPoint = (CCPoint*)pPaths->objectAtIndex(0);
        rPoint = ccp(firstPoint->x,firstPoint->y);
        return rPoint;
    }
    
    
    //求总距离、各段距离、速度   
    float* distances = new float[pPaths->count()-1];
    float totalDistance = 0.0f;
    
    for (int i = 0; i < pPaths->count(); i++) {
        
        CCPoint *p0=(CCPoint*)pPaths->objectAtIndex(i);
        if (p0 != (CCPoint*)pPaths->lastObject()) {
            CCPoint *p1=(CCPoint*)pPaths->objectAtIndex(i+1);
            CCPoint sub = ccpSub(*p1, *p0);
            float distance = sqrt(pow(sub.x,2) + pow(sub.y,2));
            distances[i] = distance;
            totalDistance += distance;
        }
    }
    //速度
    float speed = totalDistance / duration;
    
    //第time的移动的距离
    float distanceAtTime = speed * time;
    
    
    //求在time时候，在哪个段上、在该段上移动的距离
    int curLine = 0;             //在哪个段上
    float distance = 0.0f;       //段距离
    float moveDistance = 0.0f;   //在该段上移动的距离
    
    float tempDistance = 0.0;
    for (int i = 0; i < pPaths->count() - 1; i++) {
        tempDistance += distances[i];
        if (tempDistance >= distanceAtTime) {
            curLine = i;
            moveDistance = distanceAtTime - (tempDistance - distances[i]);
            distance = distances[i];
            break;
        }
    }
    
    //按三角形等比原理，求出当前移动到的点。（先平行移动到原点，在移回来）
    CCPoint *p0=(CCPoint*)pPaths->objectAtIndex(curLine);
    CCPoint *p1=(CCPoint*)pPaths->objectAtIndex(curLine+1);
    CCPoint sub = ccpSub(*p1, *p0);
    
    CCPoint rPointOrigin = ccpMult(sub, moveDistance/distance);
    rPoint = ccpAdd(rPointOrigin, *p0);
    
    
    delete distances;
    return rPoint;
    
}

float APccpAngle(CCPoint &p1, CCPoint &p2)
{
    CCPoint sub = ccpSub(p1, p2);
    float radian = 0.0; //
    
    if (sub.y == 0.0 && p1.x > p2.x){          // 0度
        
        radian = 0.0f;
        
    }else if (sub.x == 0.0 && p1.y > p2.y){    // 90度
        
        radian = M_PI / 2.0;
        
    }else if (sub.y == 0.0 && p1.x < p2.x){    // 180度
        
        radian = M_PI;
        
    }else if (sub.x == 0.0 && p1.y < p2.y){    // 270度
        
        radian = M_PI * 1.5;
        
    }else{
        
        radian = atanf(sub.y/sub.x);
        
        if (sub.x < 0 && sub.y > 0) {                 // 第二象限
            
            radian = radian + M_PI;
            
        }else if (sub.x < 0 && sub.y < 0) {           // 第三象限
            
            radian =  radian + M_PI;
            
        }else if (sub.x > 0 && sub.y < 0) {           // 第四象限
            
            radian = radian + M_PI * 2;
            
        }
    }
    
    return radian;
}

float APccpAngleDegree(CCPoint &p1, CCPoint &p2)
{
    float angle = APccpAngle(p1,p2);
    return CC_RADIANS_TO_DEGREES(angle);
}

float APMoveDurationWithSpeed(CCPoint &p1, CCPoint &p2, float speed)
{
    if (p1.equals(p2))
        return 0.0;
    if (speed <= 0) 
        return 1000000.0;
    
    CCPoint sub = ccpSub(p1, p2);
    float distance = sqrt(pow(sub.x,2) + pow(sub.y,2));
    
    return distance/speed;
    
}

float APMoveDuration(CCPoint &p1, CCPoint &p2, float uDistance, float uDuration){
    if (uDuration <= 0 )
        return 0.0;
    
    return APMoveDurationWithSpeed(p1,p2, uDistance/uDuration);
}

float APDistanceRate(CCPoint &p1, CCPoint &p2, float unitDistance){
    
    CCPoint sub = ccpSub(p1, p2);
    float distance = sqrt(pow(sub.x,2) + pow(sub.y,2));
    return distance/unitDistance;
}

float APDistance(CCPoint &p1, CCPoint &p2)
{
    CCPoint sub = ccpSub(p1, p2);
    float distance = sqrt(pow(sub.x,2) + pow(sub.y,2));
    return distance;
}