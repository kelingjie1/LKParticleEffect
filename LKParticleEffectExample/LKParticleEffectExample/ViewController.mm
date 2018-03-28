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
#import <ARKit/ARKit.h>
#import <CoreMotion/CoreMotion.h>

using namespace LKKit;

class LKParticleEffectVarExt
{
public:
    double panX;
    double panY;
};

@interface ViewController ()<ARSessionDelegate>

@property (nonatomic) EAGLContext *context;
@property (nonatomic) GLKView *glview;
@property (nonatomic) LKParticleEffectSystem *system;
@property (nonatomic) LKParticleEffectVarExt varExt;
@property (nonatomic) CGPoint oldPoint;
@property (nonatomic) ARSession *session;
@property (nonatomic) CMMotionManager *motionManager;
@property (nonatomic) LKParticleEffectSystemManager *systemManager;


@end

void loggerListener(LKParticleEffectLogLevel level,const char* str)
{
    NSLog(@"%s",str);
}

@implementation ViewController

- (void)startRecord
{

    
}

- (void)captureOutput:(AVCaptureOutput *)output didOutputSampleBuffer:(CMSampleBufferRef)sampleBuffer fromConnection:(AVCaptureConnection *)connection
{
    
}

- (void)viewDidLoad
{
    [super viewDidLoad];
    
    self.preferredFramesPerSecond = 60;
    
    LKParticleEffectLogger::instance()->listener = loggerListener;
    
    self.context = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES3];
    self.glview = (GLKView*)self.view;
    self.glview.context = self.context;
    [EAGLContext setCurrentContext:self.context];
    auto config = LKParticleEffectConfig();
    config.viewWidth = self.view.bounds.size.width;
    config.viewHeight = self.view.bounds.size.height;
    config.vars.push_back(new RVar("panX",&_varExt.panX));
    config.vars.push_back(new RVar("panY",&_varExt.panY));
    self.system = new LKParticleEffectSystem(config);
    self.systemManager = [[LKParticleEffectSystemManager alloc] initWithSystem:self.system];
    self.systemManager.context = self.context;
    NSString *path = [[[NSBundle mainBundle] resourcePath] stringByAppendingPathComponent:@"effects/test"];
    self.system->load([path cStringUsingEncoding:NSUTF8StringEncoding]);
    
    [self.view addGestureRecognizer:[[UITapGestureRecognizer alloc] initWithTarget:self action:@selector(tapGR:)]];
    [self.view addGestureRecognizer:[[UIPanGestureRecognizer alloc] initWithTarget:self action:@selector(panGR:)]];
}

- (void)viewWillAppear:(BOOL)animated
{
    [super viewWillAppear:animated];
    self.session = [[ARSession alloc] init];
    self.session.delegate = self;
    self.session.delegateQueue = dispatch_queue_create("ARKit", nil);
    [self.session runWithConfiguration:[[ARWorldTrackingConfiguration alloc] init]];
    self.motionManager = [[CMMotionManager alloc] init];
    [self.motionManager startDeviceMotionUpdatesUsingReferenceFrame:CMAttitudeReferenceFrameXArbitraryZVertical toQueue:[[NSOperationQueue alloc] init] withHandler:^(CMDeviceMotion * _Nullable motion, NSError * _Nullable error) {
        
        [self.systemManager updateWithDeviceMotion:motion];
    }];
}

- (void)session:(ARSession *)session didUpdateFrame:(ARFrame *)frame
{
    [self.systemManager updateWithARFrame:frame];
}

- (void)tapGR:(UITapGestureRecognizer*)gr
{
    if (gr.state == UIGestureRecognizerStateEnded)
    {
        [self.systemManager updateSystemData:^{
            self.system->triggerEvent("tap");
        }];
        
    }
}

- (void)panGR:(UIPanGestureRecognizer*)gr
{
    if (gr.state == UIGestureRecognizerStateBegan)
    {
        [self.systemManager updateSystemData:^{
            self.system->triggerEvent("panBegin");
        }];
        
    }
    else if (gr.state == UIGestureRecognizerStateChanged)
    {
        CGPoint point = [gr locationInView:gr.view];
        [self.systemManager updateSystemData:^{
            self.system->inputProperty.touch2DX = point.x/self.view.bounds.size.width;
            self.system->inputProperty.touch2DY = point.y/self.view.bounds.size.height;
            self.system->triggerEvent("panChange");
        }];
        
    }
    else if (gr.state == UIGestureRecognizerStateEnded||gr.state == UIGestureRecognizerStateCancelled)
    {
        [self.systemManager updateSystemData:^{
            self.system->triggerEvent("panEnd");
        }];
        
    }
}

- (void)dealloc
{
    [self.session pause];
    delete self.system;
}

- (void)glkView:(GLKView *)view drawInRect:(CGRect)rect
{
    auto camera = dynamic_cast<LKParticleEffect3DCamera*>(self.system->camera.get());
    [self.systemManager updateSystem:self.timeSinceLastDraw];
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    [self.systemManager render];;
}


- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}


@end
