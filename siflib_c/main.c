#include <siflib.h>

int
main(int argc, char* argv[])
{
	// Image paths
    
	const char* in_path = "/home/samu/dev/SIF/siflib_c/image.sif";
    const char* out_path = "/home/samu/dev/SIF/siflib_c/out.sif";

	// Create the image

	sifImage* img;
	sifLoadImage(in_path, &img);

    // Output image

    sifWriteImage(out_path, &img);

    // Free memory
    
    sifUnloadImage(&img);

    // Create image from gimp-generated buffer

    sifImage* g_img;
    sifCreateImage(&g_img, 6, 6);


	return 0;

}
