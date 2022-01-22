# SIFLIB
This is a lightway single-header library (200 lines) for loading, storing and using images as minimalist as possible in your C program.<br>
Use suckless's Farbfeld if you want something more usefull.

Structure of a .sif image file (little endian):
~~~
+═══════+═════════════════════════════════════+
| Bytes │ Funtion                             |
+═══════+═════════════════════════════════════+
|   3   | "SIF" Magic Bytes)                  |
+───────+─────────────────────────────────────+
|   4   │ Width (32-bit unsigned interger)    |
+───────+─────────────────────────────────────+
|   4   │ Height (32-bit unsigned interger)   |
+───────+─────────────────────────────────────+
|   +   │ RGBA Pixel array (4 x 8-bit values) |
+═══════+═════════════════════════════════════+
~~~

# Funtions

To load from file:
~~~
int32_t sifLoadImage(const char* path, sifImage** image);
~~~
To write to file
~~~
int32_t sifWriteImage(const char* path, sifImage** image);
~~~
To unload a image
~~~
int32_t sifUnloadImage(sifImage** image);
~~~
To create an empty image
~~~
int32_t sifCreateImage(sifImage** image, uint32_t x, uint32_t y);
~~~
