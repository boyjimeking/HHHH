//
//  NDCompare.h
//  Example
//
//  Created by jhzheng on 10-12-14.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//

#include <UIKit/UIKit.h>


@protocol NDCompare
// ����
-(BOOL) compareEqualOtherObj:(id)otherObj;
// С��
-(BOOL) compareSmallThanOtherObj:(id)otherObj;
@end
