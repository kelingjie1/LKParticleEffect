//
//  ViewController.m
//  LKParticleEffectExample
//
//  Created by 柯灵杰 on 2018/2/13.
//  Copyright © 2018年 柯灵杰. All rights reserved.
//

#import "ViewController.h"
#import <GLKit/GLKit.h>
#import <OpenGLES/ES3/gl.h>
#import <OpenGLES/ES3/glext.h>
#import <LKParticleEffect/LKParticleEffect.h>

using namespace LKKit;

@interface ViewController ()

@property (nonatomic) EAGLContext *context;
@property (nonatomic) GLKView *glview;
@property (nonatomic) LKParticleEffectSystem *system;

@end

@implementation ViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    
    
    
    self.context = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES3];
    self.glview = (GLKView*)self.view;
    self.glview.context = self.context;
    [EAGLContext setCurrentContext:self.context];
    self.system = new LKParticleEffectSystem(LKParticleEffectConfig());
    NSString *path = [[[NSBundle mainBundle] resourcePath] stringByAppendingPathComponent:@"effects/test"];
    self.system->loadResources([path cStringUsingEncoding:NSUTF8StringEncoding]);
}

- (void)dealloc
{
    delete self.system;
}

- (void)glkView:(GLKView *)view drawInRect:(CGRect)rect
{
    glClearColor(0, 0, 1, 1);
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    self.system->render();
}


- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}


@end
