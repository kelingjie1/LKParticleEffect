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
@property (nonatomic) ARFrame *arframe;
@property (nonatomic) CMMotionManager *motionManager;
@property (nonatomic) GLKMatrix4 matrix;
@property (nonatomic) CMDeviceMotion *motion;

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
        
        CMRotationMatrix a = motion.attitude.rotationMatrix;
        self.matrix=GLKMatrix4Make(a.m11, a.m21, a.m31, 0.0f,
                                   a.m12, a.m22, a.m32, 0.0f,
                                   a.m13, a.m23, a.m33, 0.0f,
                                   0.0f , 0.0f , 0.0f , 1.0f);
        self.matrix = GLKMatrix4Rotate(self.matrix, M_PI_2, 1, 0, 0);
        self.motion = motion;
    }];
}

- (void)session:(ARSession *)session didUpdateFrame:(ARFrame *)frame
{
    self.arframe = frame;
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
    if (gr.state == UIGestureRecognizerStateBegan)
    {
        self.system->triggerEvent("panBegin");
    }
    else if (gr.state == UIGestureRecognizerStateChanged)
    {
        CGPoint point = [gr locationInView:gr.view];
        self.system->inputProperty.touch2DX = point.x/self.view.bounds.size.width;
        self.system->inputProperty.touch2DY = point.y/self.view.bounds.size.height;
        self.system->triggerEvent("panChange");
    }
    else if (gr.state == UIGestureRecognizerStateEnded||gr.state == UIGestureRecognizerStateCancelled)
    {
        self.system->triggerEvent("panEnd");
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
    if (camera&&self.motion)
    {
        GLKMatrix4 r = self.matrix;
        Matrix4f m;
        m<<
        r.m00,r.m10,r.m20,r.m30,
        r.m01,r.m11,r.m21,r.m31,
        r.m02,r.m12,r.m22,r.m32,
        r.m03,r.m13,r.m23,r.m33;
        camera->motionMatrix = m;
        if (self.arframe)
        {
            float mutiple = 500;
            float x = -self.arframe.camera.transform.columns[3][0]*mutiple;
            float y = self.arframe.camera.transform.columns[3][1]*mutiple;
            float z = self.arframe.camera.transform.columns[3][2]*mutiple;
            camera->positionOffsetX = x;
            camera->positionOffsetY = y;
            camera->positionOffsetZ = z;
        }
    }
    
    self.system->update(self.timeSinceLastDraw);
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    self.system->render();
}


- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}


@end
