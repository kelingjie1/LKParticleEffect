//
//  ViewController.h
//  LKParticleEffectExample
//
//  Created by 柯灵杰 on 2018/2/13.
//  Copyright © 2018年 柯灵杰. All rights reserved.
//

#import <UIKit/UIKit.h>
#import <GLKit/GLKit.h>
#import <AVFoundation/AVFoundation.h>

@interface ViewController : GLKViewController<AVCaptureVideoDataOutputSampleBufferDelegate>

@property (nonatomic) dispatch_queue_t videoQueue;

@end

