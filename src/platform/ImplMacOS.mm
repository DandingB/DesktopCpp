#ifdef __APPLE__

#include <Cocoa/Cocoa.h>
#include <CoreText/CoreText.h>
#include <mach-o/dyld.h>

#include <map>
#include <string>

#include "WindowBase.h"
#include "Font.h"
#include "Platform.h"

NSString* WStringToNSString(std::wstring wstr)
{
    return [[NSString alloc] initWithBytes:wstr.data() length:wstr.size() * sizeof(wchar_t) encoding:NSUTF32LittleEndianStringEncoding];
}

struct cxSolidBrush
{
    float r,g,b,a;
};

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

- (void)mouseExited:(NSEvent*)event
{
    ref->OnMouseLeave();
}


- (BOOL)isFlipped
{
    return YES;
}

@end

struct cxWindowBase::Impl
{
    Window* m_Window;
    std::map<int, cxSolidBrush> m_pBrushes;
    std::map<int, cxFont> m_pFonts;
};

struct cxFont::Impl
{
    std::wstring fontName;
    float size;
};

cxWindowBase::cxWindowBase() : p(std::make_unique<Impl>())
{
    NSRect graphicsRect = NSMakeRect(0, 0, 500, 500);
    
    p->m_Window = [[Window alloc] initWithContentRect:graphicsRect styleMask:NSTitledWindowMask|NSClosableWindowMask|NSMiniaturizableWindowMask|NSWindowStyleMaskResizable backing:NSBackingStoreBuffered defer:NO ];
    [p->m_Window setDelegate: p->m_Window];
    p->m_Window->ref = this;

    // Window view
    CustomView* view = [[[CustomView alloc] initWithFrame:graphicsRect] autorelease];
    view->ref = this;
    [p->m_Window setContentView: view];
    [p->m_Window makeKeyAndOrderFront: nil];
}

cxWindowBase::~cxWindowBase()
{
    // [WND_NSWND close];
    //delete m_Window;
}

void cxWindowBase::SetTitle(std::wstring title)
{
    NSString* s = [[NSString alloc] initWithBytes:title.data() length:title.size() * sizeof(wchar_t) encoding:NSUTF32LittleEndianStringEncoding];
    [p->m_Window setTitle: s];
}

void cxWindowBase::SetPosition(int x, int y)
{
    float scale = 1.0;
    NSRect frame = [p->m_Window frame];
    frame.origin.x = x / scale;
    frame.origin.y = y / scale;
    [p->m_Window setFrame: frame display: YES animate: YES];
}

void cxWindowBase::SetSize(int width, int height)
{
    float scale = 1.0;
    NSRect frame = [p->m_Window frame];
    frame.size = NSMakeSize(width / scale, height / scale);
    [p->m_Window setFrame: frame display: YES animate: YES];
}

void cxWindowBase::GetTitle(std::wstring& out)
{
    NSData* pSData = [ [ p->m_Window title ] dataUsingEncoding: NSUTF32LittleEndianStringEncoding ];
    out = std::wstring((wchar_t*) [pSData bytes], [pSData length] / sizeof(wchar_t));
}

void cxWindowBase::GetPosition(int& x, int& y)
{
    float scale = 1.0;
    NSRect rect = [p->m_Window frame];
    x = rect.origin.x * scale;
    y = rect.origin.y * scale;
}

void cxWindowBase::GetSize(int& width, int& height)
{
    float scale = 1.0;
    NSRect rect = [p->m_Window frame];
    width = rect.size.width * scale;
    height = rect.size.height * scale;
}

void cxWindowBase::GetClientSize(float& width, float& height)
{
    float scale = 1.0;
    NSRect rect = [ [p->m_Window contentView] frame ];
    width = (float)rect.size.width * scale;
    height = (float)rect.size.height * scale;
}


void cxWindowBase::Show(bool show)
{
    if (show)
        [p->m_Window makeKeyAndOrderFront: nil];
    else
        [p->m_Window orderOut: nil];
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
    [[p->m_Window contentView] setNeedsDisplay: YES];
}

void cxWindowBase::SetDrawConstraints(cxRect rect)
{
    CGContextRef context = [[NSGraphicsContext currentContext] CGContext];
    CGContextTranslateCTM(context, rect.left, rect.top);
}

void cxWindowBase::RemoveDrawConstraints()
{
    CGContextRef context = [[NSGraphicsContext currentContext] CGContext];
    CGContextRestoreGState(context); 
}

void cxWindowBase::MakeSolidBrush(int key, float r, float g, float b, float a)
{
    p->m_pBrushes.insert({ key, {r,g,b,a} });
}

void cxWindowBase::FillRectangle(cxRect rect, int brushKey)
{
    cxSolidBrush brush = p->m_pBrushes[brushKey];
    
    CGContextRef context = [[NSGraphicsContext currentContext] CGContext];
    CGContextSetRGBFillColor(context, brush.r, brush.g, brush.b, brush.a);
    CGContextFillRect(context, CGRectMake(rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top));
}

void cxWindowBase::DrawRectangle(cxRect rect, int brushKey, float strokeWidth)
{
    cxSolidBrush brush = p->m_pBrushes[brushKey];

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

void cxWindowBase::DrawTextInRect(cxFont* font, int brushKey, std::wstring str, cxRect rect, cxTextOptions options)
{
    float top = rect.top;
    cxSolidBrush brush = p->m_pBrushes[brushKey];

    NSMutableParagraphStyle* style = [[NSMutableParagraphStyle alloc] init];
    switch (options.m_TextAlignment)
    {
        case cxTextOptions::TEXT_ALIGNMENT_LEFT: [style setAlignment:NSTextAlignmentLeft]; break; 
        case cxTextOptions::TEXT_ALIGNMENT_CENTER: [style setAlignment:NSTextAlignmentCenter]; break;
        case cxTextOptions::TEXT_ALIGNMENT_RIGHT: [style setAlignment:NSTextAlignmentRight]; break;
    }

    NSDictionary* attributes = @{
        NSFontAttributeName: [NSFont fontWithName:WStringToNSString(font->p->fontName) size:font->p->size],
        NSForegroundColorAttributeName: [NSColor colorWithDeviceRed:brush.r green:brush.g blue:brush.b alpha:brush.a],
        NSParagraphStyleAttributeName: style
    };

    NSAttributedString* currentText = [[NSAttributedString alloc] initWithString:WStringToNSString(str) attributes: attributes];

    CGRect paragraphRect =
        [currentText boundingRectWithSize:CGSizeMake(rect.right - rect.left, rect.bottom - rect.top)
        options:(NSStringDrawingUsesLineFragmentOrigin|NSStringDrawingUsesFontLeading)
        context:nil];


    switch (options.m_ParagraphAlignment)
    {
        case cxTextOptions::PARAGRAPH_ALIGNMENT_TOP: break;
        case cxTextOptions::PARAGRAPH_ALIGNMENT_CENTER: 
            top += ((rect.bottom - rect.top) - paragraphRect.size.height)/2; 
            break;
        case cxTextOptions::PARAGRAPH_ALIGNMENT_BOTTOM: break;
    }
    
    [currentText drawInRect:CGRectMake(rect.left, top, rect.right - rect.left, rect.bottom - rect.top)];
}


float cxWindowBase::GetDPIScale()
{
    return [p->m_Window backingScaleFactor];
}

void cxWindowBase::OnSize(int width, int height)
{
}



cxFont::cxFont(std::wstring fontName, float size) : p(std::make_unique<Impl>())
{
    p->fontName = fontName;
    p->size = size;
}

cxFont::~cxFont()
{

}

void cxFont::GetFontTextMetrics(std::wstring str, float maxWidth, float maxHeight, cxTextOptions options, float& width, float& height)
{
    NSMutableParagraphStyle *style = [[NSMutableParagraphStyle alloc] init];

    switch (options.m_TextAlignment)
    {
        case cxTextOptions::TEXT_ALIGNMENT_LEFT: [style setAlignment:NSTextAlignmentLeft]; break;
        case cxTextOptions::TEXT_ALIGNMENT_CENTER: [style setAlignment:NSTextAlignmentCenter]; break;
        case cxTextOptions::TEXT_ALIGNMENT_RIGHT: [style setAlignment:NSTextAlignmentRight]; break;
    }
    
    NSDictionary *attributes = @{
        NSFontAttributeName: [NSFont fontWithName:WStringToNSString(p->fontName) size: p->size],
        NSForegroundColorAttributeName: [NSColor blackColor],
        NSParagraphStyleAttributeName: style
    };

    NSAttributedString* currentText = [[NSAttributedString alloc] initWithString:WStringToNSString(str) attributes: attributes];

    CGRect paragraphRect =
        [currentText boundingRectWithSize:CGSizeMake(maxWidth, maxHeight)
        options:(NSStringDrawingUsesLineFragmentOrigin|NSStringDrawingUsesFontLeading)
        context:nil];

    width = paragraphRect.size.width;
    height = paragraphRect.size.height;

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

void cxShowCursor(bool show)
{
    if (show)
        [NSCursor unhide];
    else
        [NSCursor hide];
}

void cxSetCursor(cxCursorType type)
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

void cxRegisterFontFile(std::wstring file)
{
    // Resolve relative path to absolute
    char pathbuf[512];
    uint32_t bufsize = sizeof(pathbuf);
    if (_NSGetExecutablePath(pathbuf, &bufsize) != 0)
    {
        cxLog(L"_NSGetExecutablePath failed!");
        exit(1);
    }

    NSString *execPath = [[NSFileManager defaultManager] stringWithFileSystemRepresentation:pathbuf length:strlen(pathbuf)];
    NSString *execDir = [execPath stringByDeletingLastPathComponent];
    NSString *fontPath = [execDir stringByAppendingPathComponent: [[NSString alloc] initWithBytes: file.data() length: file.size() * sizeof(wchar_t) encoding:NSUTF32LittleEndianStringEncoding]];

    NSURL* fontURL = [NSURL fileURLWithPath:fontPath];

    CFErrorRef error;
    if (!CTFontManagerRegisterFontsForURL((__bridge CFURLRef)fontURL, kCTFontManagerScopeProcess, &error)) 
    {
        CFStringRef errorDescription = CFErrorCopyDescription(error);
        NSLog(@"Failed to load font: %@", (__bridge NSString *)errorDescription);
        CFRelease(errorDescription);
        exit(1);
    }
}

#endif