#pragma once


#define KEY_WAS_DOWN_MASK 0x80
#define KEY_IS_DOWN_MASK  0x01

#define MOUSE_LEFT_BUTTON   0x01
#define MOUSE_MIDDLE_BUTTON 0x02
#define MOUSE_RIGHT_BUTTON  0x04
#define MOUSE_WHEEL         0x08


// Returns the string for the given D3DFORMAT.
//       bWithPrefix determines whether the string should include the "D3DFMT_"

LPCWSTR DXUTD3DFormatToString(D3DFORMAT format, bool bWithPrefix);



// Returns a view matrix for rendering to a face of a cubemap.

D3DXMATRIX DXUTGetCubeMapViewMatrix(DWORD dwFace);

// Debug printing support
// See dxerr.h for more debug printing support

void DXUTOutputDebugStringW(LPCWSTR strMsg, ...);
void DXUTOutputDebugStringA(LPCSTR strMsg, ...);
HRESULT WINAPI DXUTTrace(const CHAR* strFile, DWORD dwLine, HRESULT hr, const WCHAR* strMsg, bool bPopMsgBox);
void DXUTTraceDecl(D3DVERTEXELEMENT9 decl[MAX_FVF_DECL_SIZE]);
WCHAR* DXUTTraceD3DDECLUSAGEtoString(BYTE u);
WCHAR* DXUTTraceD3DDECLMETHODtoString(BYTE m);
WCHAR* DXUTTraceD3DDECLTYPEtoString(BYTE t);

#ifdef UNICODE
#define DXUTOutputDebugString DXUTOutputDebugStringW
#else
#define DXUTOutputDebugString DXUTOutputDebugStringA
#endif

// These macros are very similar to dxerr's but it special cases the HRESULT defined
// by DXUT to pop better message boxes. 
#if defined(DEBUG) || defined(_DEBUG)
#define DXUT_ERR(str,hr)           DXUTTrace(__FILE__, (DWORD)__LINE__, hr, str, false)
#define DXUT_ERR_MSGBOX(str,hr)    DXUTTrace(__FILE__, (DWORD)__LINE__, hr, str, true)
#define DXUTTRACE                  DXUTOutputDebugString
#else
#define DXUT_ERR(str,hr)           (hr)
#define DXUT_ERR_MSGBOX(str,hr)    (hr)
#define DXUTTRACE                  (__noop)
#endif



// Direct3D9 dynamic linking support -- calls top-level D3D9 APIs with graceful
// failure if APIs are not present.


IDirect3D9 * WINAPI DXUT_Dynamic_Direct3DCreate9(UINT SDKVersion);
int WINAPI DXUT_Dynamic_D3DPERF_BeginEvent(D3DCOLOR col, LPCWSTR wszName);
int WINAPI DXUT_Dynamic_D3DPERF_EndEvent(void);
void WINAPI DXUT_Dynamic_D3DPERF_SetMarker(D3DCOLOR col, LPCWSTR wszName);
void WINAPI DXUT_Dynamic_D3DPERF_SetRegion(D3DCOLOR col, LPCWSTR wszName);
BOOL WINAPI DXUT_Dynamic_D3DPERF_QueryRepeatFrame(void);
void WINAPI DXUT_Dynamic_D3DPERF_SetOptions(DWORD dwOptions);
DWORD WINAPI DXUT_Dynamic_D3DPERF_GetStatus(void);



// Profiling/instrumentation support

// The following macros provide a convenient way for your code to call the D3DPERF 
// functions only when PROFILE is defined.  If PROFILE is not defined (as for the final 
// release version of a program), these macros evaluate to nothing, so no detailed event
// information is embedded in your shipping program.  It is recommended that you create
// and use three build configurations for your projects:
//     Debug (nonoptimized code, asserts active, PROFILE defined to assist debugging)
//     Profile (optimized code, asserts disabled, PROFILE defined to assist optimization)
//     Release (optimized code, asserts disabled, PROFILE not defined)

#ifdef PROFILE
// PROFILE is defined, so these macros call the D3DPERF functions
#define DXUT_BeginPerfEvent(color, pstrMessage)   DXUT_Dynamic_D3DPERF_BeginEvent(color, pstrMessage)
#define DXUT_EndPerfEvent()                         DXUT_Dynamic_D3DPERF_EndEvent()
#define DXUT_SetPerfMarker(color, pstrMessage)    DXUT_Dynamic_D3DPERF_SetMarker(color, pstrMessage)
#else
// PROFILE is not defined, so these macros do nothing
#define DXUT_BeginPerfEvent(color, pstrMessage)   (__noop)
#define DXUT_EndPerfEvent()                         (__noop)
#define DXUT_SetPerfMarker(color, pstrMessage)    (__noop)
#endif


// Multimon handling to support OSes with or without multimon API support.  
// Purposely avoiding the use of multimon.h so DXUT.lib doesn't require 
// COMPILE_MULTIMON_STUBS and cause complication with MFC or other users of multimon.h

#ifndef MONITOR_DEFAULTTOPRIMARY
    #define MONITORINFOF_PRIMARY        0x00000001
    #define MONITOR_DEFAULTTONULL       0x00000000
    #define MONITOR_DEFAULTTOPRIMARY    0x00000001
    #define MONITOR_DEFAULTTONEAREST    0x00000002
    typedef struct tagMONITORINFO
    {
        DWORD   cbSize;
        RECT    rcMonitor;
        RECT    rcWork;
        DWORD   dwFlags;
    } MONITORINFO, *LPMONITORINFO;
    typedef struct tagMONITORINFOEXW : public tagMONITORINFO
    {
        WCHAR       szDevice[CCHDEVICENAME];
    } MONITORINFOEXW, *LPMONITORINFOEXW;
    typedef MONITORINFOEXW MONITORINFOEX;
    typedef LPMONITORINFOEXW LPMONITORINFOEX;
#endif

HMONITOR DXUTMonitorFromWindow(HWND hWnd, DWORD dwFlags);
BOOL     DXUTGetMonitorInfo(HMONITOR hMonitor, LPMONITORINFO lpMonitorInfo);
void     DXUTGetDesktopResolution(UINT AdapterOrdinal, UINT* pWidth, UINT* pHeight);






// Creates a REF or NULLREF device and returns that device.  The caller should call
// Release() when done with the device.

IDirect3DDevice9* DXUTCreateRefDevice(HWND hWnd, bool bNullRef = true);
