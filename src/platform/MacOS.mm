#ifdef __APPLE__

#include "../cx.h"
#include "../window/WindowBase.h"

#include <Cocoa/Cocoa.h>
#include <string>

#define WND_NSWND ((MacWnd*)m_Window)->m_NSWindow
#define WND_GLVIEW ((MacWnd*)m_Window)->m_GLView


@interface Window : NSWindow <NSWindowDelegate> //<NSDraggingSource, NSDraggingDestination, NSPasteboardItemDataProvider>
{
@public 
    cxWindowBase* ref;
}
@end

@implementation Window

- (void)windowWillClose:(NSNotification*)NSNotification
{
    ref->OnClosing();
}

@end



@interface GLView : NSOpenGLView // <NSDraggingSource, NSDraggingDestination, NSPasteboardItemDataProvider>
{
@public 
    cxWindowBase* ref;
}
@end

@implementation GLView

- (void)updateTrackingAreas
{
    NSRect screenRect = [[NSScreen mainScreen] frame];
    NSTrackingAreaOptions options = (NSTrackingActiveAlways | NSTrackingMouseEnteredAndExited | NSTrackingMouseMoved);
    NSTrackingArea *area = [[NSTrackingArea alloc] initWithRect:[self bounds] options:options owner:self userInfo:nil];
    [self addTrackingArea: area];

    [NSEvent addGlobalMonitorForEventsMatchingMask:NSMouseMovedMask handler:^(NSEvent* event) {
        NSPoint curPoint = [event locationInWindow];
        //NSLog(@"mouseMove x: %f, %f " , curPoint.x*2, curPoint.y*2);
    }];
}

-(void)awakeFromNib
{
    NSOpenGLPixelFormatAttribute pixelFormatAttributes[] =
    {
        NSOpenGLPFADoubleBuffer,
        NSOpenGLPFADepthSize, 24,
        NSOpenGLPFAOpenGLProfile, NSOpenGLProfileVersion3_2Core,
        NSOpenGLPFAMultisample,
        NSOpenGLPFASampleBuffers, 1,
        NSOpenGLPFASamples, 4,
        NSOpenGLPFAAccelerated,
        NSOpenGLPFANoRecovery,
        0
    };

    NSOpenGLPixelFormat *pixelFormat = [[NSOpenGLPixelFormat alloc] initWithAttributes: pixelFormatAttributes];
    NSOpenGLContext* glc = [[NSOpenGLContext  alloc]initWithFormat:pixelFormat shareContext:nil];
    [self setOpenGLContext: glc];
}

-(void)prepareOpenGL
{
    [super prepareOpenGL];
}

- (void)reshape
{
    [super reshape];
    [self.openGLContext makeCurrentContext];

    // Get new viewport size
    NSRect bounds = [self bounds];
    ref->OnSize(bounds.size.width * 2, bounds.size.height * 2);
}

- (void)drawRect:(NSRect)dirtyRect
{
    ref->OnPaint();
}

- (void)mouseDown:(NSEvent*)event
{
    NSPoint curPoint = [self convertPoint:[event locationInWindow] fromView:nil];
    ref->OnMouseDown(curPoint.x * 2, curPoint.y * 2);
}

- (void)mouseUp:(NSEvent*)event
{
    NSPoint curPoint = [self convertPoint:[event locationInWindow] fromView:nil];
    ref->OnMouseUp(curPoint.x * 2, curPoint.y * 2);
}

- (void)mouseMoved:(NSEvent*)event
{
    NSPoint curPoint = [self convertPoint:[event locationInWindow] fromView:nil];
    ref->OnMouseMove(curPoint.x * 2, curPoint.y * 2);
    //NSLog(@"mouseMove x: %f, %f", curPoint.x*2, curPoint.y*2);
}

- (void)mouseDragged:(NSEvent*)event
{
    NSPoint curPoint = [self convertPoint:[event locationInWindow] fromView:nil];
    ref->OnMouseDragged(curPoint.x * 2, curPoint.y * 2);
    //NSLog(@"mouseDragged x: %f, %f", curPoint.x*2, curPoint.y*2);
}

- (BOOL)isFlipped
{
    return YES;
}

@end



struct MacWnd
{
    Window* m_NSWindow;
    GLView* m_GLView;
};


cxWindowBase::cxWindowBase()
{
    m_Window = new MacWnd;

    NSRect graphicsRect = NSMakeRect(0, 0, 500, 500);
    
    WND_NSWND = [[Window alloc] initWithContentRect:graphicsRect styleMask:NSTitledWindowMask|NSClosableWindowMask|NSMiniaturizableWindowMask|NSWindowStyleMaskResizable backing:NSBackingStoreBuffered defer:NO ];
    [WND_NSWND setDelegate: WND_NSWND];
    WND_NSWND->ref = this;

    // Window view
    WND_GLVIEW = [[[GLView alloc] initWithFrame:graphicsRect] autorelease];
    [WND_NSWND setContentView: WND_GLVIEW];
    WND_GLVIEW->ref = this;
}

cxWindowBase::~cxWindowBase()
{
    [WND_NSWND close];
    //delete m_Window;
}

void cxWindowBase::SetTitle(std::wstring title)
{
    NSString* s = [[NSString alloc] initWithBytes:title.data() length:title.size() * sizeof(wchar_t) encoding:NSUTF32LittleEndianStringEncoding];
    [WND_NSWND setTitle: s];
}

void cxWindowBase::SetPosition(int x, int y)
{
    NSRect frame = [WND_NSWND frame];
    frame.origin.x = x;
    frame.origin.y = y;
    [WND_NSWND setFrame: frame display: YES animate: YES];
}

void cxWindowBase::SetSize(int width, int height)
{
    NSRect frame = [WND_NSWND frame];
    frame.size = NSMakeSize(width, height);
    [WND_NSWND setFrame: frame display: YES animate: YES];
}

void cxWindowBase::Show(bool show)
{
    if (show)
        [WND_NSWND makeKeyAndOrderFront: nil];
    else
        [WND_NSWND orderOut: nil];
}

void cxWindowBase::Invalidate()
{
    //[self setNeedsDisplay: YES];
}

void cxWindowBase::CaptureMouse()
{
    //[WND_GLVIEW disableCursorRects];
}

void cxWindowBase::ReleaseMouse()
{

}

void cxRunApp()
{
    NSAutoreleasePool* pool = [[NSAutoreleasePool alloc] init];
    NSApplication* application = [NSApplication sharedApplication];

    [application setActivationPolicy:NSApplicationActivationPolicyRegular];

    [application run];
    [pool release];
}

void cxQuitApp(int exitCode)
{
    [NSApp terminate: nil];
}

void cxMessageBox(std::wstring text)
{
    NSString* message = [[NSString alloc] initWithBytes:text.data() length:text.size() * sizeof(wchar_t) encoding:NSUTF32LittleEndianStringEncoding];
    NSAlert *alert = [[[NSAlert alloc] init] autorelease];
    [alert setMessageText: message];
    [alert runModal];
}

#endif