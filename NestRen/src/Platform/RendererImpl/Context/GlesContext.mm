//
// Created by Admin on 11.02.2022.
//

#include "GlesContext.hpp"
#include "Miren/PlatformData.hpp"

#import <UIKit/UIKit.h>
#import <OpenGLES/ES3/gl.h>

namespace Miren {
    GLuint colorRenderBuffer;
    GLuint depthRenderBuffer;
    GLuint frameBuffer;

    void GlesContext::create() {
        EAGLContext* _context = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES3];
        context = _context;
        PlatformData::get()->renderingContext = context;
        [EAGLContext setCurrentContext:_context];

        glGenRenderbuffers(1, &colorRenderBuffer);
        glBindRenderbuffer(GL_RENDERBUFFER, colorRenderBuffer);
        
        CAEAGLLayer *eaglLayer = (__bridge CAEAGLLayer*) PlatformData::get()->layer;
        [_context renderbufferStorage:GL_RENDERBUFFER fromDrawable: eaglLayer];
        eaglLayer.drawableProperties = @{kEAGLDrawablePropertyRetainedBacking : @NO,
                                         kEAGLDrawablePropertyColorFormat     : kEAGLColorFormatRGBA8 };
        eaglLayer.opaque = NO;

        glGenRenderbuffers(1, &depthRenderBuffer);
        glBindRenderbuffer(GL_RENDERBUFFER, depthRenderBuffer);

        int width = UIScreen.mainScreen.bounds.size.width * UIScreen.mainScreen.scale;
        int height = UIScreen.mainScreen.bounds.size.height * UIScreen.mainScreen.scale;
        printf("Screen size: %d x %d\n", width, height);

        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, width, height);

        glGenFramebuffers(1, &frameBuffer);
        glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, colorRenderBuffer);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthRenderBuffer);
        glBindRenderbuffer(GL_RENDERBUFFER, colorRenderBuffer);

        glViewport(0, 0, width, height);
        glClearColor(1.0f, 0.0f, 0.0f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT);
    }

    void GlesContext::flip() {
        EAGLContext* _context = (__bridge EAGLContext*) context;
        [_context presentRenderbuffer: GL_RENDERBUFFER];
    }

    uint32_t GlesContext::getDefaultFrameBufferId() {
        return frameBuffer;
    }
}
