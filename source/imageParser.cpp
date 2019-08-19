#include "../header/image.h"
#include <iostream>
#include <fstream>
#include <stack>

using namespace std;

int main(){
  Image* image;
	image = image_init(0, 0);
  string asd("test2.bmp");
	char *cstr = &asd[0u];
	image_read(image, cstr);

	ofstream tmpfile;
	tmpfile.open("tmpReprFile");

	//for every pixel:
	stack<int> arguments;

  for (int i=0; i<image->width*image->height; i++){
    unsigned r = (256+(int)image->pixels[i*4])  %256;
    unsigned g = (256+(int)image->pixels[i*4+1])%256;
    unsigned b = (256+(int)image->pixels[i*4+2])%256;
    unsigned a = (256+(int)image->pixels[i*4+3])%256;
    cout << " pixels[0] = " << r;
    cout << " pixels[1] = " << g;
    cout << " pixels[2] = " << b;
    cout << " pixels[3] = " << a<<endl;

		unsigned color = r * 1000000 + g * 1000 + b;
		int top;

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
				case 233030090: // r 233 g 030 b 090 aka some red
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
			}
		}
		else {
			switch(color){
				case 0: // r 000 g 000 b 000 aka black
				  tmpfile << "\n";
					tmpfile << "print ";
					arguments.push(5);
					arguments.push(1);
					break;
				case 244067054: // r 244 g 067 b 054 aka some orange
					tmpfile << "\n";
					tmpfile << "add ";
					arguments.push(5);
					arguments.push(5);
					arguments.push(5);
					break;
				case 233030090: // r 233 g 030 b 090 aka some red
					tmpfile << "\n";
					tmpfile << "mul ";
					arguments.push(5);
					arguments.push(5);
					arguments.push(5);
					break;
				case 156039176: // r 156 g 039 b 176 aka some pink
					tmpfile << "\n";
					tmpfile << "jump ";
					arguments.push(5);
					arguments.push(5);
					break;
				case 103058183: // r 103 g 056 b 183 aka some purple
				  tmpfile << "\n";
					tmpfile << "label ";
					arguments.push(5);
					break;
				case 63081181: //r 063 081 181 aka some blue
					tmpfile << "\n";
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
