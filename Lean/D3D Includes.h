#pragma once
#ifndef D3D_INCLUDES_H
#define D3D_INCLUDES_H

/////////////
// LINKING //
/////////////
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3d11.lib")
/*#pragma comment(lib, "d3dx11.lib")
#pragma comment(lib, "d3dx10.lib")*/
/*#pragma comment(lib, "..\\Lib\\x86\\d3d11.lib")
#pragma comment(lib, "..\\Lib\\x86\\d3dx11.lib")
#pragma comment(lib, "..\\Lib\\x86\\d3dx10.lib")*/
#pragma comment(lib, "D3DCompiler.lib")

#include "Windows//WICTextureLoader.h"
#include "Windows//DDSTextureLoader.h"

#include <dxgi.h>
#include <d3dcommon.h>
#include <d3d11.h>
#include <D3Dcompiler.h>
//#include "..\\Include\\d3dx10math.h"

#endif