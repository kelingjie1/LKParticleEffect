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

void loggerListener(LKParticleEffectLogLevel level,const char* str)
{
    NSLog(@"%s",str);
}

@implementation ViewController

- (void)startRecord
{
//    NSError *error;
//    self.videoQueue = dispatch_queue_create("videoQueue", nil);
//    self.session = [[AVCaptureSession alloc] init];
//    self.videoInput = [[AVCaptureDeviceInput alloc] initWithDevice:[AVCaptureDevice defaultDeviceWithMediaType:AVMediaTypeVideo] error:&error];
//    [self.session addInput:self.videoInput];
//    self.videoDataOutput = [[AVCaptureVideoDataOutput alloc] init];
//    [self.videoDataOutput setSampleBufferDelegate:self queue:self.videoQueue];
//    [[self.videoDataOutput connectionWithMediaType:AVMediaTypeVideo] setEnabled:NO];
//    AVCaptureConnection *videoConnection = [self.videoDataOutput connectionWithMediaType:AVMediaTypeVideo];
//    [self.session addOutput:self.videoDataOutput];
//    [self.session startRunning];
}

- (void)captureOutput:(AVCaptureOutput *)output didOutputSampleBuffer:(CMSampleBufferRef)sampleBuffer fromConnection:(AVCaptureConnection *)connection
{
    
}

- (void)viewDidLoad
{
    [super viewDidLoad];
    
    LKParticleEffectLogger::instance()->listener = loggerListener;
    
    self.context = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES3];
    self.glview = (GLKView*)self.view;
    self.glview.context = self.context;
    [EAGLContext setCurrentContext:self.context];
    auto config = LKParticleEffectConfig();
    config.viewWidth = self.view.bounds.size.width;
    config.viewHeight = self.view.bounds.size.height;
    self.system = new LKParticleEffectSystem(config);
    NSString *path = [[[NSBundle mainBundle] resourcePath] stringByAppendingPathComponent:@"effects/test"];
    self.system->load([path cStringUsingEncoding:NSUTF8StringEncoding]);
    
    [self.view addGestureRecognizer:[[UITapGestureRecognizer alloc] initWithTarget:self action:@selector(tapGR:)]];
    [self.view addGestureRecognizer:[[UIPanGestureRecognizer alloc] initWithTarget:self action:@selector(panGR:)]];
}

- (void)tapGR:(UITapGestureRecognizer*)gr
{
    if (gr.state == UIGestureRecognizerStateEnded)
    {
        self.system->triggerEvent("tap");
    }
}

- (void)panGR:(UIPanGestureRecognizer*)gr
{
    
}

- (void)dealloc
{
    delete self.system;
}

- (void)glkView:(GLKView *)view drawInRect:(CGRect)rect
{
    glBindVertexArray(1);
    self.system->update(self.timeSinceLastDraw);
    glClearColor(1, 1, 1, 1);
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    self.system->render();
    glBindVertexArray(0);
}


- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}


@end
