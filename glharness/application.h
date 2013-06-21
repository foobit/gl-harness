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

#pragma once

#include <thread>

namespace glh {

class application
{
	application(const application&);
	const application& operator= (const application&);
public:
	typedef std::function<void ()> callback_function;
	typedef std::function<void (int width, int height)> callback_size_function;

	application(const std::string& title, int width, int height, bool resizable = true);
	virtual ~application();

	virtual int run();

	callback_function& start();
	callback_function& update();
	callback_function& render();
	callback_size_function& size_change();

protected:
	virtual bool create_window();
	virtual void make_current();

	virtual void on_size_changed();
	virtual void on_update();
	virtual void on_render();

	virtual void begin_render();
	virtual void end_render();

private:
	static LRESULT WINAPI msg_proc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
	void handle_size(int w, int h, int t);
	void handle_min_max_info(MINMAXINFO& mmi);
	void perform_idle();
	void thread_worker();
	void perform_render();

	HWND					m_hwnd;
	HDC						m_hdc;
	HGLRC					m_hrc_main;
	HGLRC					m_hrc_work;
	WNDCLASSEXW				m_wc;
	std::string				m_title;
	bool					m_resizable;
	bool					m_ready;
	int						m_width;
	int						m_height;
	std::thread::id			m_main_id;
	std::thread				m_thread;

	callback_function		m_fn_start;
	callback_function		m_fn_update;
	callback_function		m_fn_render;
	callback_size_function	m_fn_size;
};

} // glh
