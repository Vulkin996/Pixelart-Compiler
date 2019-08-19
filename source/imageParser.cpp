#include "../header/image.h"
#include <iostream>

using namespace std;

int main(){
  Image* image;
	image = image_init(0, 0);
  string asd("test.bmp");
	char *cstr = &asd[0u];
	image_read(image, cstr);
  for (int i=0; i<image->width*image->height; i++){
    unsigned r = (256+(int)image->pixels[i*4])%256;
    unsigned g = (256+(int)image->pixels[i*4+1])%256;
    unsigned b = (256+(int)image->pixels[i*4+2])%256;
    unsigned a = (256+(int)image->pixels[i*4+3])%256;
    cout << "pixels[0] = " << r;
    cout << " pixels[1] = " << g;
    cout << " pixels[2] = " << b;
    cout << " pixels[3] = " << a<<endl;
  }
	image_done(image);
  return 0;
}
