#pragma once

#include "Window.h"

#define CHWND_EXCEPT(hr) Window::HrException(__LINE__, __FILE__,(hr))
#define CHWND_LAST_EXCEPT() Window::HrException(__LINE__, __FILE__, GetLastError())
#define CWHND_NOGFX_EXCEPT() Window::NoGfxException(__LINE__,__FILE__)