//
//  APGeometry.h
//  pandajump2
//
//  Created by li shiyong on 12-11-13.
//
//

#ifndef __pandajump2__APGeometry__
#define __pandajump2__APGeometry__

#include "cocos2d.h"

USING_NS_CC;


//公共私有变量属性
#define AP_SYNTHESIZE_PUBLIC(varType, varName, funName)\
public: varType varName;\
public: virtual const varType& get##funName(void) const { return varName; }\
public: virtual void set##funName(varType var){ varName = var; }


/** 获取对象的RTTI 名称
 @return const char*
 @
 */
#define apcn(aClass) typeid(aClass).name()

/** 获取字符串比较
 @return int  0 相同 ，非0不同
 @
 */
#define apstrcmp(achar,bchar) strcmp(achar, bchar)

CCPoint APCCPointMake(float rateX, float rateY);

/** 调整屏幕点，使之适应不同屏幕分辨率
 @return CCPoint 
 @
 */
#define apccp(__X__,__Y__) APCCPointMake((float)(__X__), (float)(__Y__))

CCPoint APCCPointMake(float rateX, float rateY);

/** 
 @return CCPoint
 */
#define apccp_(__X__,__Y__) APCCPointMake_((float)(__X__), (float)(__Y__))

CCPoint APCCPointMake_(float rateX , float rateY);


/** 按比例从屏幕可视size中，取得size
 @return CCSize
 @
 */
CCSize APCCSizeMake(float rateW, float rateH);




/** 节点比例的点
 @return CCPoint
 @
 */
#define aprateccp(__node__,__X__,__Y__) APCCPointRateMake((CCNode*)(__node__), (float)(__X__), (float)(__Y__))

CCPoint APCCPointRateMake(CCNode* node, float rateX, float rateY);


/** 按照比例加到父节点
 @return CCPoint 子节点的位置
 @
 */
void APAddChildForRate(CCNode* parent, CCNode* child, float rateX, float rateY, int zOrder = 0);


/** 修正Scale来适应VisibleSize
 @return 
 @
 */
#define apftv(__pNode__) fitToVisibleSize(__pNode__)
void fitToVisibleSize(CCNode *pNode);

/** 屏幕可视化比率
 @return
 @
 */
CCPoint APScaleRate();

/** 设对象在paths组成的线段中匀速移动，共用时duration。
 @return 在 time 时刻对象的位置
 @ time:     某时刻 
 @ duration: 移动完成总共用时
 */
CCPoint APGetPointAtTime(float time, CCArray* pPaths, float duration);


/** 设对象在paths组成的线段中匀速移动，共用时duration。
 @return 在 time 时刻对象的位置
 @ time:     某时刻
 @ duration: 移动完成总共用时
 */
CCPoint APGetPointAtTime(float time, CCArray* pPaths, float duration);

/** 取两点间的夹角
 @return 弧度，在(0 ~ 2PI)之间
 @ 
 */
float APccpAngle(CCPoint &p1, CCPoint &p2);
float APccpAngleDegree(CCPoint &p1, CCPoint &p2);

/** 两点间移动需要的时间
 @return 移动需要的时间
 @
 */
float APMoveDurationWithSpeed(CCPoint &p1, CCPoint &p2, float speed);
float APMoveDuration(CCPoint &p1, CCPoint &p2, float uDistance, float uDuration);

/** 两点间距离与单位长度的比率
 @return 
 @
 */
float APDistanceRate(CCPoint &p1, CCPoint &p2, float unitDistance);

/** 两点间距离
 @return
 @
 */
float APDistance(CCPoint &p1, CCPoint &p2);

#endif /* defined(__pandajump2__APGeometry__) */
