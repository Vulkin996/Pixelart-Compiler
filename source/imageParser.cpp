#include "../header/image.h"
#include "../header/util.hpp"
#include <iostream>
#include <fstream>
#include <stack>

using namespace std;

int parseImage(char* image_path){
  Image* image;
	image = image_init(0, 0);
  string asd(image_path);
	char *cstr = &asd[0u];
	image_read(image, cstr);

	ofstream tmpfile;
	tmpfile.open("tmpReprFile");


	stack<int> arguments;
	//flag for the first instruction in file
	bool first = true;
	//for every pixel:
  for (int i=0; i<image->width*image->height; i++){
    unsigned r = (256+(int)image->pixels[i*4])  %256;
    unsigned g = (256+(int)image->pixels[i*4+1])%256;
    unsigned b = (256+(int)image->pixels[i*4+2])%256;
    unsigned a = (256+(int)image->pixels[i*4+3])%256;
    /*cout << " pixels[0] = " << r;
    cout << " pixels[1] = " << g;
    cout << " pixels[2] = " << b;
    cout << " pixels[3] = " << a<<endl;*/

		//if a pixel is not at maximum alpha, or its white ignore it.
		if(a < 255 || (r == 255 && g == 255 & b == 255))
			continue;

		//combine RGB values into a single value
		unsigned color = r * 1000000 + g * 1000 + b;
		int top;

		//if there are arguments treat pixels as numbers.
		if(!arguments.empty()){
			switch(color){
				case 0: // r 000 g 000 b 000 aka black
					tmpfile << "0";
					top = arguments.top();
					arguments.pop();
					if(--top){
						arguments.push(top);
					}
					else
						tmpfile << " ";
					break;
				case 244067054: // r 244 g 067 b 054 aka some orange
					tmpfile << "1";
					top = arguments.top();
					arguments.pop();
					if(--top){
						arguments.push(top);
					}
					else
						tmpfile << " ";
					break;
				case 233030099: // r 233 g 030 b 099 aka some red
					tmpfile << "2";
					top = arguments.top();
					arguments.pop();
					if(--top){
						arguments.push(top);
					}
					else
						tmpfile << " ";
					break;
				case 156039176: // r 156 g 039 b 176 aka some pink
					tmpfile << "3";
					top = arguments.top();
					arguments.pop();
					if(--top){
						arguments.push(top);
					}
					else
						tmpfile << " ";
					break;
				case 103058183: // r 103 g 056 b 183 aka some purple
					tmpfile << "4";
					top = arguments.top();
					arguments.pop();
					if(--top){
						arguments.push(top);
					}
					else
						tmpfile << " ";
					break;
				case 63081181: //r 063 081 181 aka some blue
				 	tmpfile << "5";
					top = arguments.top();
					arguments.pop();
					if(--top){
						arguments.push(top);
					}
					else
						tmpfile << " ";
					break;
				case 33150243:
					tmpfile << "6";
					top = arguments.top();
					arguments.pop();
					if(--top){
						arguments.push(top);
					}
					else
						tmpfile << " ";
					break;
				case 3169244:
					tmpfile << "7";
					top = arguments.top();
					arguments.pop();
					if(--top){
						arguments.push(top);
					}
					else
						tmpfile << " ";
					break;
				case 188212:
					tmpfile << "8";
					top = arguments.top();
					arguments.pop();
					if(--top){
						arguments.push(top);
					}
					else
						tmpfile << " ";
					break;
				case 150136:
					tmpfile << "9";
					top = arguments.top();
					arguments.pop();
					if(--top){
						arguments.push(top);
					}
					else
						tmpfile << " ";
					break;
				case 139195074:
					tmpfile << "a";
					top = arguments.top();
					arguments.pop();
					if(--top){
						arguments.push(top);
					}
					else
						tmpfile << " ";
					break;
				case 255235059:
					tmpfile << "b";
					top = arguments.top();
					arguments.pop();
					if(--top){
						arguments.push(top);
					}
					else
						tmpfile << " ";
					break;
				case 121085072:
					tmpfile << "c";
					top = arguments.top();
					arguments.pop();
					if(--top){
						arguments.push(top);
					}
					else
						tmpfile << " ";
					break;
				case 158158158:
					tmpfile << "d";
					top = arguments.top();
					arguments.pop();
					if(--top){
						arguments.push(top);
					}
					else
						tmpfile << " ";
					break;
				case 96125139:
					tmpfile << "e";
					top = arguments.top();
					arguments.pop();
					if(--top){
						arguments.push(top);
					}
					else
						tmpfile << " ";
					break;
				case 26035126:
					tmpfile << "f";
					top = arguments.top();
					arguments.pop();
					if(--top){
						arguments.push(top);
					}
					else
						tmpfile << " ";
					break;
			}
		}
		//if there arent arguments treat pixels as instructions.
		else {
			switch(color){
				case 0: // r 000 g 000 b 000 aka black
					if(!first)
				  	tmpfile << "\n";
					else
						first = 0;
					tmpfile << "print ";
					arguments.push(5);
					arguments.push(1);
					break;
				case 244067054: // r 244 g 067 b 054 aka some orange
					if(!first)
						tmpfile << "\n";
					else
						first = 0;
					tmpfile << "add ";
					arguments.push(5);
					arguments.push(5);
					arguments.push(5);
					break;
				case 233030099: // r 233 g 030 b 099 aka some red
					if(!first)
						tmpfile << "\n";
					else
						first = 0;
					tmpfile << "mul ";
					arguments.push(5);
					arguments.push(5);
					arguments.push(5);
					break;
				case 156039176: // r 156 g 039 b 176 aka some pink
					if(!first)
						tmpfile << "\n";
					else
						first = 0;
					tmpfile << "jump ";
					arguments.push(5);
					arguments.push(5);
					break;
				case 103058183: // r 103 g 056 b 183 aka some purple
					if(!first)
						tmpfile << "\n";
					else
						first = 0;
					tmpfile << "label ";
					arguments.push(5);
					break;
				case 63081181: //r 063 081 181 aka some blue
					if(!first)
						tmpfile << "\n";
					else
						first = 0;
				 	tmpfile << "++";
					arguments.push(5);
					break;
			}
		}

  }

	image_done(image);
	tmpfile.close();
  return 0;
}
