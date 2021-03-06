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

void my_update()
{
}


void my_render()
{
}


void my_size_changed(int /* width */, int /* height */)
{
}


int CALLBACK WinMain(HINSTANCE,HINSTANCE,LPSTR,int)
{
	auto app = std::make_shared<glh::application>("test harness", 640, 480);
	app->update() = std::bind(&my_update);
	app->render() = std::bind(&my_render);
	app->size_change() = std::bind(&my_size_changed, _1, _2);

	return app->run();
}
