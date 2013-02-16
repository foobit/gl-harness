/*
	Copyright 2013 Scott Ramsay
 
	Licensed under the Apache License, Version 2.0 (the "License");
	you may not use this file except in compliance with the License.
	You may obtain a copy of the License at
 
		http://www.apache.org/licenses/LICENSE-2.0
 
	Unless required by applicable law or agreed to in writing, software
	distributed under the License is distributed on an "AS IS" BASIS,
	WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
	See the License for the specific language governing permissions and
	limitations under the License.
*/

#include "pch.h"
#include "application.h"
#include "str.h"

namespace glh {
namespace {

const wchar_t*		_class_name = L"glh:winclass";
application*		_self;
bool				_glew_init;

} // unnamed namespace

application::application(const std::string& title, int width, int height, bool resizable) :
	m_hwnd(nullptr),
	m_hdc(nullptr),
	m_hrc(nullptr),
	m_title(title),
	m_resizable(resizable),
	m_width(width),
	m_height(height),
	m_ready(false)
{
	_self = this;

	memset(&m_wc, 0, sizeof(WNDCLASSEXW));

	m_wc.cbSize = sizeof(WNDCLASSEXW);
	m_wc.style = CS_OWNDC|CS_HREDRAW|CS_VREDRAW;
	m_wc.hbrBackground = nullptr;
	m_wc.hCursor = ::LoadCursor(0, IDC_ARROW);
	m_wc.hInstance = static_cast<HINSTANCE>(::GetModuleHandle(0));
	m_wc.lpfnWndProc = msg_proc;
	m_wc.lpszClassName = _class_name;
}


application::~application()
{
}


application::callback_function& application::start()
{
	return m_fn_start;
}


application::callback_function& application::update()
{
	return m_fn_update;
}


application::callback_function& application::render()
{
	return m_fn_render;
}


bool application::create_window()
{
	WNDCLASSEXW wc;

	// register window class if it doesn't exist
	if (!::GetClassInfoExW(m_wc.hInstance, _class_name, &wc))
	{
		if (!::RegisterClassExW(&m_wc))
		{
			return false;
		}
	}

	int style = m_resizable ? WS_OVERLAPPEDWINDOW : WS_OVERLAPPEDWINDOW & ~(WS_THICKFRAME|WS_MAXIMIZEBOX);
	int ex_style = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;

	RECT rc;
	::SetRect(&rc, 0, 0, m_width, m_height);
	::AdjustWindowRectEx(&rc, style, FALSE, ex_style);

	m_hwnd = ::CreateWindowExW(ex_style, _class_name, str::to_wide(m_title).c_str(), style, CW_USEDEFAULT, 0, rc.right - rc.left, rc.bottom - rc.top, nullptr, nullptr, nullptr, nullptr);
	if (m_hwnd == nullptr)
	{
		return false;
	}

	// set window as GL
	PIXELFORMATDESCRIPTOR pfd;
	memset(&pfd, 0, sizeof(PIXELFORMATDESCRIPTOR));
	pfd.nSize		= sizeof(PIXELFORMATDESCRIPTOR);
	pfd.nVersion	= 1;
	pfd.dwFlags		= PFD_DOUBLEBUFFER | PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW;
	pfd.iPixelType	= PFD_TYPE_RGBA;
	pfd.cColorBits	= 32;
	pfd.cDepthBits	= 32;
	pfd.iLayerType	= PFD_MAIN_PLANE;

	m_hdc = ::GetDC(m_hwnd);
	if (!m_hdc)
	{
		return false;
	}

	int pixelFormat = ::ChoosePixelFormat(m_hdc, &pfd);

	if (!::SetPixelFormat(m_hdc, pixelFormat, &pfd))
	{
		return false;
	}

	m_hrc = ::wglCreateContext(m_hdc);
    make_current();

	// init glew
	if (!_glew_init)
	{
		_glew_init = true;

		GLenum err = glewInit();
		if (err != GLEW_OK)
		{
			return false;
		}

		if (!GLEW_VERSION_2_1)
		{
			return false;
		}
	}

	if (m_fn_start != nullptr)
	{
		m_fn_start();
	}

	m_ready = true;
	return true;
}


void application::make_current()
{
	::wglMakeCurrent(m_hdc, m_hrc);
}


void application::on_update()
{
	if (m_fn_update != nullptr)
	{
		m_fn_update();
	}
}


void application::on_render()
{
	if (m_fn_render != nullptr)
	{
		m_fn_render();
	}
}


application::callback_size_function& application::size_change()
{
	return m_fn_size;
}


void application::on_size_changed()
{
	if (m_fn_size != nullptr)
	{
		m_fn_size(m_width, m_height);
	}
}


void application::handle_min_max_info(MINMAXINFO& mmi)
{
	mmi.ptMinTrackSize.x = 96;
	mmi.ptMinTrackSize.y = 64;
}


void application::handle_size(int w, int h, int)
{
	m_width = w;
	m_height = h;
	on_size_changed();
	perform_idle();
}


int application::run()
{
	if (!create_window())
	{
		return 1;
	}

	::ShowWindow(m_hwnd, SW_SHOWNORMAL);
	::UpdateWindow(m_hwnd);

	MSG msg;

	bool done = false;
	while (!done)
	{
		while (::PeekMessageW(&msg, 0, 0, 0, PM_NOREMOVE))
		{
			if (!::GetMessageW(&msg, 0, 0, 0))
			{
				done = true;
				break;
			}
			::TranslateMessage(&msg);
			::DispatchMessageW(&msg);
		}
		
		perform_idle();
	}

	return 0;
}


void application::begin_render()
{
	glClearColor(0, 0, 0.14f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}


void application::end_render()
{
	glFlush();
	::SwapBuffers(m_hdc);
}


void application::perform_idle()
{
	on_update();
	
	if (m_ready)
	{
		begin_render();
		on_render();
		end_render();
	}
}


LRESULT WINAPI application::msg_proc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	if (_self)
	{
		switch (msg)
		{
			case WM_DESTROY:
				::PostQuitMessage(0);
				break;

			case WM_SIZE:
				_self->handle_size(static_cast<short>(LOWORD(lparam)), static_cast<short>(HIWORD(lparam)), static_cast<int>(wparam));
				break;

			case WM_GETMINMAXINFO:
				_self->handle_min_max_info(*reinterpret_cast<MINMAXINFO*>(lparam));
				break;

			default:
				return ::DefWindowProcW(hwnd, msg, wparam, lparam);
		}

		return 0;
	}

	return ::DefWindowProcW(hwnd, msg, wparam, lparam);
}

} // namespace glh
