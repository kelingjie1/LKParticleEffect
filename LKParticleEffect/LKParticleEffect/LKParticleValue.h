//
//  LKParticleValue.h
//  ParticleSystem
//
//  Created by lingtonke on 2017/3/3.
//  Copyright © 2017年 lingtonke. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface LKParticleValue : NSObject

@property (nonatomic) NSObject *object;


-(instancetype)initWithObject:(NSObject*)object;
-(double)value;
-(void)setVar:(double*)var name:(NSString*)name;
-(instancetype)clone;
@end
