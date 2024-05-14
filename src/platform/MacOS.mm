#ifdef __APPLE__

#include "../cx.h"
#include "../cx/WindowBase.h"

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

- (BOOL)isFlipped
{
    return YES;
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
    float scale = ref->GetDPIScale();
    NSRect bounds = [self bounds];
    ref->OnSize(bounds.size.width * scale, bounds.size.height * scale);
}

- (void)drawRect:(NSRect)dirtyRect
{
    ref->OnPaint();
}

- (void)mouseDown:(NSEvent*)event
{
    float scale = ref->GetDPIScale();
    NSPoint curPoint = [self convertPoint:[event locationInWindow] fromView:nil];
    ref->OnMouseDown({ (int)(curPoint.x * scale), (int)(curPoint.y * scale), LEFT });
}

- (void)rightMouseDown:(NSEvent*)event
{
    float scale = ref->GetDPIScale();
    NSPoint curPoint = [self convertPoint:[event locationInWindow] fromView:nil];
    ref->OnMouseDown({ (int)(curPoint.x * scale), (int)(curPoint.y * scale), RIGHT });
}

- (void)mouseUp:(NSEvent*)event
{
    float scale = ref->GetDPIScale();
    NSPoint curPoint = [self convertPoint:[event locationInWindow] fromView:nil];
    ref->OnMouseUp({ (int)(curPoint.x * scale), (int)(curPoint.y * scale), LEFT });
}

- (void)rightMouseUp:(NSEvent*)event
{
    float scale = ref->GetDPIScale();
    NSPoint curPoint = [self convertPoint:[event locationInWindow] fromView:nil];
    ref->OnMouseUp({ (int)(curPoint.x * scale), (int)(curPoint.y * scale), RIGHT });
}

- (void)mouseMoved:(NSEvent*)event
{
    float scale = ref->GetDPIScale();
    NSPoint curPoint = [self convertPoint:[event locationInWindow] fromView:nil];
    ref->OnMouseMove({ (int)(curPoint.x * scale), (int)(curPoint.y * scale), NONE });
}

- (void)mouseDragged:(NSEvent*)event
{
    float scale = ref->GetDPIScale();
    NSPoint curPoint = [self convertPoint:[event locationInWindow] fromView:nil];
    ref->OnMouseDragged({ (int)(curPoint.x * scale), (int)(curPoint.y * scale), LEFT });
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
    float scale = GetDPIScale();
    NSRect frame = [WND_NSWND frame];
    frame.origin.x = x * scale;
    frame.origin.y = y * scale;
    [WND_NSWND setFrame: frame display: YES animate: YES];
}

void cxWindowBase::SetSize(int width, int height)
{
    float scale = GetDPIScale();
    NSRect frame = [WND_NSWND frame];
    frame.size = NSMakeSize(width * scale, height * scale);
    [WND_NSWND setFrame: frame display: YES animate: YES];
}

void cxWindowBase::GetTitle(std::wstring& out)
{
    NSData* pSData = [[WND_NSWND title] dataUsingEncoding: NSUTF32LittleEndianStringEncoding];
    out = std::wstring((wchar_t*) [pSData bytes], [pSData length] / sizeof(wchar_t));
}

void cxWindowBase::GetPosition(int& x, int& y)
{
    float scale = GetDPIScale();
    NSRect rect = [WND_NSWND frame];
    x = rect.origin.x * scale;
    y = rect.origin.y * scale;
}

void cxWindowBase::GetSize(int& width, int& height)
{
    float scale = GetDPIScale();
    NSRect rect = [WND_NSWND frame];
    width = rect.size.width * scale;
    height = rect.size.height * scale;
}

void cxWindowBase::GetClientSize(int& width, int& height)
{
    float scale = GetDPIScale();
    NSRect rect = [ [WND_NSWND contentView] frame ];
    width = rect.size.width * scale;
    height = rect.size.height * scale;
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
    [WND_GLVIEW setNeedsDisplay: YES];
}

void cxWindowBase::CaptureMouse()
{
    //[WND_GLVIEW disableCursorRects];
}

void cxWindowBase::ReleaseMouse()
{

}

float cxWindowBase::GetDPIScale()
{
    return [WND_NSWND backingScaleFactor];
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

void cxLog(std::wstring str, ...)
{
    va_list args;
    va_start(args, str);

    wchar_t* buffer = new wchar_t[1024];
    memset(buffer, '\0', 1024);
    vswprintf(buffer, 1024, str.c_str(), args);
    int len = wcslen(buffer);

    va_end(args);

    NSString* s = [[NSString alloc] initWithBytes:buffer length:len * sizeof(wchar_t) encoding:NSUTF32LittleEndianStringEncoding];
    NSLog(@"%@", s);

    delete[] buffer;
}

#endif