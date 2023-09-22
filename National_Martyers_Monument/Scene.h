#pragma once

namespace Control {
	// Window Handling Functions
	void display();
	void reshape(int width, int height);
	void keyboard(unsigned char key, int x, int y);
	void keyboardup(unsigned char key, int x, int y);
	void special(int key, int x, int y);
	void specialup(int key, int x, int y);
	void mouse(int button, int state, int x, int y);
	void motion(int x, int y);
	void idle();
}