/*
#include <Arks/bmh.h>
#include <GLFW/glfw3.h>
*/
#include "bmh.h"
#include "glfw3.h"

#include <vector>
#include <ctime>

int w = 960;
int h = 960;
bool changed = true;
bool first = true;

int depthtrans = 0;

int renderMode = 1;

vec2 trans(0);
double sscale = 1;

void dynamicResizeWindow(GLFWwindow * window, int iw, int ih) {
	w = iw;
	h = ih;
	first = true;
	changed = true;
	glViewport(0, 0, w, h);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (action != GLFW_PRESS) return;
		
	switch (key)
		{
        case GLFW_KEY_ESCAPE:
		    glfwSetWindowShouldClose(window, GL_TRUE);
		    break;

		case GLFW_KEY_M:
            renderMode = 1 - renderMode;
            changed = 1;
            cout << "renderMode:" << renderMode << endl;
			break;
        
        case GLFW_KEY_KP_ADD:
            sscale += .1*sscale;
            changed = 1;
            break;
        case GLFW_KEY_KP_SUBTRACT:
            sscale -= .1*sscale;
            changed = 1;
            break;
        
        case GLFW_KEY_EQUAL:
            sscale += 2*abs(sscale);
            changed = 1;
            break;
        case GLFW_KEY_MINUS:
            sscale -= 2*(abs(sscale)/4);
            changed = 1;
            break;

        case GLFW_KEY_UP:
            trans.y -= .1/sscale;
            changed = 1;
            break;
        case GLFW_KEY_DOWN:
            trans.y += .1/sscale;
            changed = 1;
            break;
        case GLFW_KEY_LEFT:
            trans.x += .1/sscale;
            changed = 1;
            break;
        case GLFW_KEY_RIGHT:
            trans.x -= .1/sscale;
            changed = 1;
            break;

        case GLFW_KEY_W:
            trans.y -= 1.2/sscale;
            changed = 1;
            break;
        case GLFW_KEY_S:
            trans.y += 1.2/sscale;
            changed = 1;
            break;
        case GLFW_KEY_A:
            trans.x += 1.2/sscale;
            changed = 1;
            break;
        case GLFW_KEY_D:
            trans.x -= 1.2/sscale;
            changed = 1;
            break;

        case GLFW_KEY_LEFT_BRACKET:
            depthtrans -= 1;
            changed = 1;
            break;
        case GLFW_KEY_RIGHT_BRACKET:
            depthtrans += 1;
            changed = 1;
            break;
        
        case GLFW_KEY_SEMICOLON:
            depthtrans -= 10;
            changed = 1;
            break;
        case GLFW_KEY_APOSTROPHE:
            depthtrans += 10;
            changed = 1;
            break;
        
        case GLFW_KEY_COMMA:
            depthtrans -= 100;
            changed = 1;
            break;
        case GLFW_KEY_PERIOD:
            depthtrans += 100;
            changed = 1;
            break;

        case GLFW_KEY_H:
            trans = vec2(0.488393403505, -0.304677829451);
            changed = 1;
            break;

		default:
			break;
		}
}

int Mandel(const vec2 & pos, const int & depth = 16){
    double x,y, nx,ny;

    x = pos.x;
    y = pos.y;

    for (int i = 0; i < depth; i++)
    {
        nx = x*x - y*y + pos.x;
        ny = 2*x*y + pos.y;

        if(nx*nx + ny*ny > 8){
            return i;
        }

        x = nx;
        y = ny;
    }
    
    return depth;
}

int main(){

    cout << "Mandelbrot Render c++ program\ncode by Ark_Noya\n";

    int depth = 24;
    double ratio2;

    clock_t st,et;

    BMHead bmh(w,h);
    ofstream ofs(".\\outImage.bmp", ios::binary);

    vector<vec> colors(w*h);

    glfwInit();
    GLFWwindow * window = glfwCreateWindow(w, h, "Mandelbrot render", NULL, NULL);
	glfwMakeContextCurrent(window);

    while (!glfwWindowShouldClose(window)) {
        glfwSetKeyCallback(window, key_callback);
		glfwSetFramebufferSizeCallback(window, dynamicResizeWindow);

		if (first) {
			glClearColor(.05, .1, .1, 1.);
			glClear(GL_COLOR_BUFFER_BIT);
			glfwSwapBuffers(window);

			first = false;
		}
        
        if (changed) {

            bmh = BMHead(w,h);
            colors.resize(w*h);

            ratio2 = 1.*h/w;

            depth = 24 + sqrtf(abs(sscale)) + depthtrans;

            cout.precision(12);
            cout << "w:" << w << endl << "h:" << h << endl << "ratio2:" << ratio2 << endl << "scale:" << sscale << endl << "depth:" << depth << "(";
            if(depthtrans > 0) cout << "+";
            else cout << "-";
            cout << abs(depthtrans) << ")" << endl << "transform:" << trans << endl;

            st = clock();
#pragma omp parallel for
            for (int i = 0; i < w*h; i++)
            {
                int x = i%w;
                int y = i/w;
                vec2 pos(x,y);

                vec color;
                int ri = Mandel((vec2(pos.x/w*2-1, pos.y/w*2-1 + (1-ratio2)))/sscale*1.5 - trans - vec2(.7,0), depth);
                switch (renderMode)
                {
                case 0:
                    if(ri < depth){
                        color = 1;
                    }else{
                        color = 0;
                    }
                    break;
                    
                case 1:
                    color = min(1.*ri/depth, .999);
                    break;
                
                default:
                    break;
                }

                colors[i] = color;
            }
            et = clock();

            for (int i = 0; i < w*h; i++)
            {
                int x = i%w;
                int y = i/w;
                glBegin(GL_POINTS);

				glVertex2f(1.*x / w * 2 - 1, 1.*y / h * 2 - 1);
				glColor3f(colors[i].x, colors[i].y, colors[i].z);

				glEnd();
            }
            

            cout << "Rendering OK! time used : " << (double)(et-st)/CLOCKS_PER_SEC << "seconds\n";

            glfwSwapBuffers(window);
            changed = false;
        }
        glfwPollEvents();
    }

    glfwTerminate();

    
    st = clock();
    for(int i=0; i<bmh.size(); i++) ofs << bmh[i];
    for (int i = 0; i < colors.size(); i++) BMofs(ofs, colors[i]*255);
    et = clock();
    cout << "Writing OK! time used : " << (double)(et-st)/CLOCKS_PER_SEC << "seconds\n";

    ofs.close();
    //system(".\\outImage.bmp");

    return 233;
}