# Mandelbrot Renderer

The Mandelbrot set, popularized by Benoˆıt B. Mandelbrot in the 1980s, is a renowned mathematical fractal admired for its intricate, self-similar patterns. As you zoom in, increasingly complex structures emerge, creating visually stunning and captivating imagery. This program allows users to visualize the Mandelbrot set in color and adjust rendering parameters easily. 

## How to build the program

To build the program, the user must have a C compiler installed on their system. The program was developed using the GCC compiler. Navigate to the folder containing this file, then run the following command in the terminal to compile the program: 

```cmd
gcc -o mandelbrot_renderer.exe ./src/* ./include/*
```

## How to use the program

Parameters that influence how an image of the set looks like include the _viewport_ in the complex plane that we want to visualize, a color scheme (_inner_color_ and _outer_colors_) and a number called _iteration depth_ that determines the accuracy of the calculation. The program reads these parameters from a configuration file. Information about the dimensions of the output picture are not included here as they don't influence the appearance of the set but rather the resolution of the image. An example configuration file is show below. 

```ini
# Viewport of the complex plane
lower_left_real = -2
lower_left_imag = -2
upper_right_real = 2
upper_right_imag = 2

# Maximum number of iterations
iteration_depth = 100

# Color for points inside of the mandelbrot set
inner_color = 0x000000

# Color gradient for points outside of the mandelbrot set
# At least one color must be specified. 
outer_colors = 0xFFFFFF , 0xFFFF00 , 0x00FFFF
```

When running the program on the command line, the user can specify a path to a configuration file, the width of the output image in pixels and an output path. The program then generates an image of the Mandelbrot set based on all these parameters and saves it to the specified output path. A command must be of following syntax: 

```cmd
./mandelbrot_renderer.exe <path to configuration file> <image width> <output path>
```

The resulting image will be saved in BMP format. It can be viewed with any image viewer that supports this format. 

There is also an help option. If the user runs the program with the -h flag, the program will print a help message and exit: 

```cmd
./mandelbrot_renderer.exe -h
```

## Example Interaction

A correct command that references a configuration file as shown above and specifies an image width of 1920 pixels and an output path of ./output.bmp would look like this: 

```cmd
./mandelbrot_renderer.exe ./config.ini 1920 ./example_output.bmp
```

This would generate the following image: 

![example_output](.\example_output.bmp)
