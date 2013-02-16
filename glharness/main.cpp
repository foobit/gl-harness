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

using namespace std::placeholders;

class my_app
{
public:
	my_app(glh::application& app)
	{
		app.start() = std::bind(&my_app::start, this);
		app.update() = std::bind(&my_app::update, this);
		app.render() = std::bind(&my_app::render, this);
		app.size_change() = std::bind(&my_app::size_changed, this, _1, _2);
	}

private:
	void start()
	{
		
	}

	void update()
	{
	}

	void render()
	{
		glBegin(GL_TRIANGLES);
			glColor3f(1, 0, 0);
			glVertex3f( 0.0f, 1.0f, 0.0f);
			glColor3f(0, 1, 0);
			glVertex3f(-1.0f,-1.0f, 0.0f);
			glColor3f(0, 0, 1);
			glVertex3f( 1.0f,-1.0f, 0.0f);
		glEnd();
	}

	void size_changed(int width, int height)
	{
		glViewport(0, 0, width, height);
	}
};

int CALLBACK WinMain(HINSTANCE,HINSTANCE,LPSTR,int)
{
	glh::application app("test harness", 640, 480);
	my_app ma(app);
	return app.run();
}
