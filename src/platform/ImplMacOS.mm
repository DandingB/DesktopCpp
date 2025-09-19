#ifdef __APPLE__

#include "../cx.h"
#include "WindowBase.h"

#include <Cocoa/Cocoa.h>
#include <string>

#define GET_WND(wnd) ((Window*)wnd)
#define GET_VIEW(view) ((CustomView*)view)

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



@interface CustomView : NSView // <NSDraggingSource, NSDraggingDestination, NSPasteboardItemDataProvider>
{
@public 
    cxWindowBase* ref;
}
@end


@implementation CustomView

- (void) viewWillMoveToWindow:(NSWindow *)newWindow
{
    NSTrackingArea* trackingArea = [[NSTrackingArea alloc] initWithRect:[self bounds] options:(NSTrackingMouseMoved | NSTrackingMouseEnteredAndExited | NSTrackingActiveAlways) owner:self userInfo:nil];
    [self addTrackingArea:trackingArea];
}

- (void) setFrameSize:(NSSize)newSize
{
    [super setFrameSize:newSize];
    ref->OnSize(newSize.width, newSize.height);
}

- (void)drawRect:(NSRect)rect
{
    ref->OnPaint();
}

- (void)mouseDown:(NSEvent*)event
{
    float scale = 1.0;
    NSPoint curPoint = [self convertPoint:[event locationInWindow] fromView:nil];
    ref->OnMouseDown({ (float)(curPoint.x * scale), (float)(curPoint.y * scale), LEFT });
}

- (void)rightMouseDown:(NSEvent*)event
{
    float scale = 1.0;
    NSPoint curPoint = [self convertPoint:[event locationInWindow] fromView:nil];
    ref->OnMouseDown({ (float)(curPoint.x * scale), (float)(curPoint.y * scale), RIGHT });
}

- (void)mouseUp:(NSEvent*)event
{
    float scale = 1.0;
    NSPoint curPoint = [self convertPoint:[event locationInWindow] fromView:nil];
    ref->OnMouseUp({ (float)(curPoint.x * scale), (float)(curPoint.y * scale), LEFT });
}

- (void)rightMouseUp:(NSEvent*)event
{
    float scale = 1.0;
    NSPoint curPoint = [self convertPoint:[event locationInWindow] fromView:nil];
    ref->OnMouseUp({ (float)(curPoint.x * scale), (float)(curPoint.y * scale), RIGHT });
}

- (void)mouseMoved:(NSEvent*)event
{
    float scale = 1.0;
    NSPoint curPoint = [self convertPoint:[event locationInWindow] fromView:nil];
    ref->OnMouseMove({ (float)(curPoint.x * scale), (float)(curPoint.y * scale), NONE });
}

- (void)mouseDragged:(NSEvent*)event
{
    float scale = 1.0;
    NSPoint curPoint = [self convertPoint:[event locationInWindow] fromView:nil];
    ref->OnMouseMove({ (float)(curPoint.x * scale), (float)(curPoint.y * scale), LEFT });
}

- (BOOL)isFlipped
{
    return YES;
}

@end


cxWindowBase::cxWindowBase()
{
    NSRect graphicsRect = NSMakeRect(0, 0, 500, 500);
    
    Window* window = [[Window alloc] initWithContentRect:graphicsRect styleMask:NSTitledWindowMask|NSClosableWindowMask|NSMiniaturizableWindowMask|NSWindowStyleMaskResizable backing:NSBackingStoreBuffered defer:NO ];
    [window setDelegate: window];
    window->ref = this;

    // Window view
    CustomView* view = [[[CustomView alloc] initWithFrame:graphicsRect] autorelease];
    view->ref = this;
    [window setContentView: view];
    [window makeKeyAndOrderFront: nil];

    m_NSWindow = window;
    m_NSView = view;
}

cxWindowBase::~cxWindowBase()
{
    // [WND_NSWND close];
    //delete m_Window;
}

void cxWindowBase::SetTitle(std::wstring title)
{
    NSString* s = [[NSString alloc] initWithBytes:title.data() length:title.size() * sizeof(wchar_t) encoding:NSUTF32LittleEndianStringEncoding];
    [GET_WND(m_NSWindow) setTitle: s];
}

void cxWindowBase::SetPosition(int x, int y)
{
    float scale = 1.0;
    NSRect frame = [GET_WND(m_NSWindow) frame];
    frame.origin.x = x / scale;
    frame.origin.y = y / scale;
    [GET_WND(m_NSWindow) setFrame: frame display: YES animate: YES];
}

void cxWindowBase::SetSize(int width, int height)
{
    float scale = 1.0;
    NSRect frame = [GET_WND(m_NSWindow) frame];
    frame.size = NSMakeSize(width / scale, height / scale);
    [GET_WND(m_NSWindow) setFrame: frame display: YES animate: YES];
}

void cxWindowBase::GetTitle(std::wstring& out)
{
    NSData* pSData = [[GET_WND(m_NSWindow) title] dataUsingEncoding: NSUTF32LittleEndianStringEncoding];
    out = std::wstring((wchar_t*) [pSData bytes], [pSData length] / sizeof(wchar_t));
}

void cxWindowBase::GetPosition(int& x, int& y)
{
    float scale = 1.0;
    NSRect rect = [GET_WND(m_NSWindow) frame];
    x = rect.origin.x * scale;
    y = rect.origin.y * scale;
}

void cxWindowBase::GetSize(int& width, int& height)
{
    float scale = 1.0;
    NSRect rect = [GET_WND(m_NSWindow) frame];
    width = rect.size.width * scale;
    height = rect.size.height * scale;
}

void cxWindowBase::GetClientSize(int& width, int& height)
{
    float scale = 1.0;
    NSRect rect = [ [GET_WND(m_NSWindow) contentView] frame ];
    width = rect.size.width * scale;
    height = rect.size.height * scale;
}

void cxWindowBase::Show(bool show)
{
    if (show)
        [GET_WND(m_NSWindow) makeKeyAndOrderFront: nil];
    else
        [GET_WND(m_NSWindow) orderOut: nil];
}

void cxWindowBase::ShowCursor(bool show)
{
    if (show)
        [NSCursor unhide];
    else
        [NSCursor hide];
}


void cxWindowBase::SetCursor(cxCursorType type)
{
    switch(type)
    {
        case cxArrow:
            [[NSCursor arrowCursor] set];
            return;
        case cxIBeam:
            [[NSCursor IBeamCursor] set];
            return;
        case cxPointingHand:
            [[NSCursor pointingHandCursor] set];
            return;
        case cxHand:
            [[NSCursor openHandCursor] set];
            return;
        case cxGrab:
            [[NSCursor closedHandCursor] set];
            return;
        case cxCrosshair:
            [[NSCursor crosshairCursor] set];
            return;
        case cxSizeWE:
            [[NSCursor resizeLeftRightCursor] set];
            return;
        case cxSizeNS:
            [[NSCursor resizeUpDownCursor] set];
            return;
        case cxNo:
            [[NSCursor operationNotAllowedCursor] set];
            return;
        default:
            [[NSCursor arrowCursor] set];
            return;
    }
}


void cxWindowBase::CaptureMouse()
{
    //[WND_GLVIEW disableCursorRects];
}

void cxWindowBase::ReleaseMouse()
{

}

void cxWindowBase::Invalidate()
{
    [GET_VIEW(m_NSView) setNeedsDisplay: YES];
}

void cxWindowBase::SetDrawConstraints(cxRect rect)
{
    CGContextRef context = [[NSGraphicsContext currentContext] CGContext];
    CGContextTranslateCTM(context, rect.left, rect.top);
}

void cxWindowBase::RemoveDrawConstraints()
{
    CGContextRef context = [[NSGraphicsContext currentContext] CGContext];
    CGContextTranslateCTM(context, 0, 0);
}

void cxWindowBase::MakeSolidBrush(int key, float r, float g, float b, float a)
{
    m_pBrushes.insert({ key, {r,g,b,a} });
}

void cxWindowBase::MakeFont(int key, std::wstring fontName, float size)
{
    m_pFonts.insert({ key, {fontName, size} });
}

void cxWindowBase::GetFontTextMetrics(int fontKey, std::wstring str, float maxWidth, float maxHeight, float& width, float& height)
{
    cxFont font = m_pFonts[fontKey];

    NSMutableParagraphStyle *style = [[NSMutableParagraphStyle alloc] init];
    [style setAlignment:NSTextAlignmentCenter];

    NSDictionary *attributes = @{
        NSFontAttributeName: [NSFont fontWithName:@"Helvetica" size:font.size],
        NSForegroundColorAttributeName: [NSColor blackColor],
        NSParagraphStyleAttributeName: style
    };

    NSString* nsStr = [[NSString alloc] initWithBytes:str.data() length:str.size() * sizeof(wchar_t) encoding:NSUTF32LittleEndianStringEncoding];
    NSAttributedString* currentText = [[NSAttributedString alloc] initWithString:nsStr attributes: attributes];

    CGRect paragraphRect =
        [currentText boundingRectWithSize:CGSizeMake(maxWidth, maxHeight)
        options:(NSStringDrawingUsesLineFragmentOrigin|NSStringDrawingUsesFontLeading)
        context:nil];

    width = paragraphRect.size.width;
    height = paragraphRect.size.height;
}

void cxWindowBase::FillRectangle(cxRect rect, int brushKey)
{
    cxSolidBrush brush = m_pBrushes[brushKey];
    
    CGContextRef context = [[NSGraphicsContext currentContext] CGContext];
    CGContextSetRGBFillColor(context, brush.r, brush.g, brush.b, brush.a);
    CGContextFillRect(context, CGRectMake(rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top));
}

void cxWindowBase::DrawRectangle(cxRect rect, int brushKey, float strokeWidth)
{
    cxSolidBrush brush = m_pBrushes[brushKey];

    CGContextRef context = [[NSGraphicsContext currentContext] CGContext];
    CGContextSetRGBStrokeColor(context, brush.r, brush.g, brush.b, brush.a);
    CGContextSetLineWidth(context, strokeWidth);
    CGContextStrokeRect(context, CGRectMake(rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top));
}

void cxWindowBase::FillRoundedRectangle(cxRect rect, float r1, float r2, int brush)
{

}

void cxWindowBase::DrawRoundedRectangle(cxRect rect, float r1, float r2, int brush, float strokeWidth)
{

}

void cxWindowBase::DrawText(int fontKey, std::wstring str, cxRect rect, int brush)
{
    cxFont font = m_pFonts[fontKey];

    NSMutableParagraphStyle *style = [[NSMutableParagraphStyle alloc] init];
    [style setAlignment:NSTextAlignmentCenter];

    NSDictionary *attributes = @{
        NSFontAttributeName: [NSFont fontWithName:@"Helvetica" size:font.size],
        NSForegroundColorAttributeName: [NSColor blackColor],
        NSParagraphStyleAttributeName: style
    };

    NSString * nsStr = [[NSString alloc] initWithBytes:str.data() length:str.size() * sizeof(wchar_t) encoding:NSUTF32LittleEndianStringEncoding];
    NSAttributedString* currentText=[[NSAttributedString alloc] initWithString:nsStr attributes: attributes];

    NSSize attrSize = [currentText size];
    [currentText drawInRect:CGRectMake(rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top)];

}


float cxWindowBase::GetDPIScale()
{
    return [GET_WND(m_NSWindow) backingScaleFactor];
}

void cxInitApp()
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

void cxGetMousePosition(float& x, float& y)
{
    // float scale = 1;

    // NSRect screenRect = [[NSScreen mainScreen] frame];
    // NSInteger height = screenRect.size.height;

    // cxLog(L"%d", height);

    // NSPoint position = [NSEvent mouseLocation];
    // x = position.x * scale;
    // y = position.y * scale;


    NSPoint location = [NSEvent mouseLocation];

    for (id screen in [NSScreen screens]) {
        if (NSMouseInRect(location, [screen frame], NO)) {
            NSSize size = {1, 1};
            NSRect mouseRect = {location, size};
            NSRect retinaMouseRect = [screen convertRectToBacking:mouseRect];

            x = retinaMouseRect.origin.x;
            y = retinaMouseRect.origin.y;

            //NSLog(@"Mouse Rect = %@", NSStringFromRect(mouseRect));
            //NSLog(@"Retina Mouse Rect = %@", NSStringFromRect(retinaMouseRect));
        }
    }
}

#endif