-------------------------------
QUICK NOTES
-------------------------------
+ To close the window after each successive test, press the ESC key.
+ Every test except testConcavePolygon and testUnits can take in at least one command-line argument.
+ Some tests have I/O capabilities, in particular: testAura, testBallroom, testConway, testLangton, testMandelbrot, testMouse, testPong, and testProjectiles.
+ If you do not specify command-line arguments, default ones will be given. 
+ If you specify large values for the command-line arguments of some of the tests, default values will be given so that the tests can run appropriately.
+ Lastly, some tests will have two windows. One will be a progress bar/tiled square, the other will be a window for the test animation. Close each window with ESC key as normal.
    The progress bar shows how much work each thread needs to do, the tiled square shows how much work each thread did in the animation. In the test that displays the tiled square, 
   	a Legend will appear showing the color for each thread or details explaining the animation.
-----------------------------------
DESCRIPTIONS
-----------------------------------
1). testAlphaRectangle - Partially transparent rectangles are drawn to the screen. Takes command-line arguments for the width and height of the screen. 
2). testAura - Displays a psychedelic visualization of colors. Takes command-line arguments for the width and height of the screen. I/O capabilities:
					Move the mouse around in the animation to change the center of the animation. 
3). testBallroom - Multiple colored balls are drawn and follow the mouse wherever it goes. I/O capabilities: The balls follow the mouse wherever it goes on the screen. Click the left 
				       mouse button to make them repel from the mouse. Click the left mouse button again to make them follow the mouse again. Takes command-line arguments for the 
				       width and height of the screen.
4). testBlurImage - Showcases an example usage of recursive splitting by blurring a given image. Takes in command-line arguments for the number of threads to use as well as for the
						image file. 
5). testCalcPi - Displays the integral for a Unit Circle and for the Cosine function using rectangles and trapezoids. Takes in command-line arguments for the number of segments and for the
					number of threads to use.
5). testColorPoints - A pattern is drawn to the screen. Takes command-line arguments for the width and height of the screen as well as for the number of threads to use. 
6). testColorWheel - Shows a color wheel that acts as a sort of sonar device. Takes command-line arguments for the width and height of the screen. 
7). testConcavePolygon - Draws two Concave polygons on the screen. See http://www.mathopenref.com/polygonconcave.html for information on what a Concave polygon is.
8). testConway - Simulates Conway's game of life. See https://en.wikipedia.org/wiki/Conway's_Game_of_Life for more details. Takes command-line arguments for the 
             		width and height of the screen. I/O capablilities: Press the spacebar to pause the animation loop and take a screenshot. Press it again to resume the loop and take another
             		screenshot. Hold the left mouse button down when the loop is paused/unpaused and *SLOWLY* drag it across the screen in order to add more cells to the game. Release it 
             		so that they can show up (and make sure the animation is unpaused).
9). testCosineIntegral - Visualizes the integration of the Cosine function. Takes command-line arguments for the width and height of the screen as well as the number of threads to 
						   use. 
10). testDumbSort - Visual representation of a Shaker sort (or Bubble sort as some call it). Takes command-line arguments for the width and height of the screen. 
11). testFireworks - Displays a pretty firework show. Takes command-line arguments for the width and height of the screen as well as for the number of threads, number of fireworks, and the speed
						of the animation.
12). testForestFire - Simulates a forest fire. Takes command-line arguments for the width and height of the screen.
13). testFunction - Visualizes the output of certain functions on the screen. In particular, the Cosine and Power functions as well as a special one. Takes command-line arguments for 
				      the width and height of the screen.
14). testGetPixels - Takes a few pixels from a photo and overwrites them with a new pixel that has a different color. Takes a command-line argument for the number of threads to use.
15). testGradientWheel - Shows a color wheel with a gradient color palate on a black background. Takes command-line arguments for the width and height of the screen. 
16). testGraydient - Shows a black-to-white gradient. Takes command-line arguments for the width and height of the screen as well as for the number of threads to use. 
17). testGreyscale - Takes pixels from an colored image and converts them to greyscale colors. Takes command-line arguments for the width and height of the screen as well as for the 
				        number of threads to use. 
18). testHighData - Simple stress test for the Canvas class where a large number of pixels are drawn to the screen at a high framerate. Takes command-line arguments for the width and height
					of the screen as well as for the number of threads to use. 
19). testImage - Draws a set of images on a screen. Takes command-line arguments for the width and height of the screen.
20). testImageCart - Draws a set of images on a screen that has a Cartesian coordinate system. Takes command-line arguments for the width and height of the screen. 
21). testInverter - Inverts the colors of an colored image to their greyscale colors. Takes a command-line argument for the number of the threads to use.
22). testLangton - Displays several visualizations of Langton's Ant (For a detailed look at Langton's Ant, go here: https://en.wikipedia.org/?title=Langton%27s_ant ). Takes command-line
                                 arguments for the width and height of the screen. There are four visualizations in total. Some may have I/O capabilities. Hit ESC to iterate through each one. 
                                 The visualizations are described below:
			                	+ Simulates four Langton's Ants that are alpha transparent on a black background. I/O capabilities: Click the left mouse button or press the ENTER key to make the screen flash more. 
				                   Press the spacebar to clear the screen. I/O capabilities: Press the ENTER key to pause the animation.
                                   Press the ENTER key again to resume the animation.
                                + Displays one Langton's Ant. 
                                + Displays four different colored Langton's Ants.
                                + Display four dynamically colored Langton's Ants.  
23). testLineChain - Displays lines that move in a vivid pattern. Takes command-line arguments for the width and height of the screen as well as for the number of threads to use.
24). testLineFan - Displays a vibrant fan of colored lines. Takes command-line arguments for the width and height of the screen as well as for the number of threads to use.
25). testMandelbrot - Displays several visualizations of a Mandelbrot set. ( see http://linas.org/art-gallery/escape/smooth.html for what a Mandelbrot set is. ) Takes command-line
						arguments for the width and height of the screen as well as for the number of threads to use and the depth. There are five visualizations total.
						The visualizations are described below:
						+ Draws a Mandelbrot set with a colored outline on a black background. I/O capabilities: Scroll up on the mouse wheel to zoom in, scroll down to zoom out. 
					      Clicking on the left mouse button does the same thing for zooming in. The right mouse button does the same thing for zooming out.
						+ Draws a Mandelbrot set but with a gradient fill and color palate. See http://linas.org/art-gallery/escape/smooth.html for what a Mandelbrot set is. 
								I/O capabilities:  Scroll up on the mouse wheel to zoom in, scroll down to zoom out. Clicking on the left mouse button does the same thing for zooming
								in. Clicking on the right mouse button does the same thing for zooming out. Takes command-line arguments for the width and height of the screen as 
								well as for the number of threads and for the depth.
						+ Draws a Buddhabrot on the screen, see https://en.wikipedia.org/wiki/Buddhabrot for details on what a Buddhabrot is.
			 			+ Displays a Julia set. See https://en.wikipedia.org/wiki/Julia_set for more details on what a Julia set is. I/O capabilities: Scroll up with the mouse wheel to zoom in, scroll down to zoom out.
			 			+ Displays a Nova fractal. See http://en.wikipedia.org/wiki/Nova_fractal for more details.
26). testMouse - A painting tool with the mouse on a white screen. Affectionately nicknamed "Art Creator". I/O capabilities:  Clicking and holding down on the left mouse button while dragging the mouse around on the 
					white background draws a pattern. Pressing the spacebar clears the screen. Takes command-line arguments for the width and height of the screen as well as for the number of threads to use.
27). testNewtonPendulum - Displays a simulation of Newton's Pendulum (or Newton's Cradle as some call it). See https://en.wikipedia.org/wiki/Newton's_cradle for more details.
							Takes command-line arguments for the width and height of the screen as well as for the number of balls.
							*NOTE* The number of balls must be an odd number.
28). testPhilosophers - A visual representation of the Dining Philosophers problem in parallel programming. Takes command-line arguments for the number of philosophers to display as well as the speed
							of the animation.
29). testPong - Play the hit old-school game, Pong. I/O capabilities: The w and s keys are for moving the left paddle up and down (respectively) and the up and down arrow keys move the
					right paddle up and down (respectively). Takes command-line arguments for the speed of the ball and for the speed of the paddles.
30). testProgressBar - Displays the progress bar that is shown in testMandelbrot. Takes command-line arguments for the width and height of the screen.
31). testProjectiles - Play a target shooting game. There are 10 targets and you have to hit the center of the target in order to get a point. Your score is printed after all 10 targets have 
					 	been displayed. I/O capabilities: Click the mouse in the center of the target to get a point. Takes command-line arguments for the width and height of the screen.
32). testScreenshot - Displays multiple colorful triangles then takes a screenshot of each drawing frame. The screenshots are saved to wherever the test is running 
						(if you run it in the bin folder, they will be saved there. If you run it anywhere else, they'll be saved in whatever folder the test is located in).
33). testSeaUrchin - Displays colorful sea urchins. Each one is drawn on a different thread. If you close the window, the message "YOU KILLED MY SEA URCHINS! : ' (" is outputted to the
				   		console. Takes command-line arguments for the number of threads to use. Can be used as a visual representation of what it means to put a process on a thread.
34). testSmartSort - Visualizes the bottom-up mergesort algorithm. Takes command-line arguments for the number of elements to sort as well as for the number of threads to use.
35). testSpectrogram - Shows a spectrogram for the colors of a given photo. Takes a command-line argument for the picture file to use. 
36). testSpectrum - Draws the full spectrum of colors across the screen and changes over time. Takes a command-line argument for the number of threads to use in drawing.
37). testText - Displays text on the screen. Takes command-line arguments for the width and height of the screen as well as for the font file to use.
38). testTextCart - Displays text on a screen that has a Cartesian coordinate system. Takes command-line arguments for the width and height of the screen as well as for the font file to use.
39). testTextTwo - Tests the library's ability to set a default font if one is not specified. Takes command-line arguments for the width and height of the screen.
40). testUnits - Unit tests for a set number of key classes used in the library. 
41). testVoronoi - Displays two visualizations of a Voronoi diagram. Takes command-line arguments for the width and height of the screen as well as for the number of threads to use.
						The visualizations are described below:
						+ Displays a simple Voronoi diagram. See https://en.wikipedia.org/wiki/Voronoi_diagram for more details on what a Voronoi diagram is.
						+ Displays a Voronoi diagram that has been shaded.