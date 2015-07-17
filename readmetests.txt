-------------------------------
QUICK NOTES
-------------------------------
+ To close the window after each successive test, press the ESC key.
+ Every test can take in at least one command-line argument.
+ Some tests have I/O capabilities, in particular: testBallroom, testGradientMandelbrot, testJulia,  testMandelbrot, testMouse, testPong, testProjectiles, and
    testLangton.

+ Lastly, some tests will have two windows. One will be a progress bar/tiled square, the other will be a window for the test animation. Close each window with ESC key as normal.
    The progress bar shows how much work each thread needs to do, the tiled square shows how much work each thread did in the animation.

-----------------------------------
DESCRIPTIONS
-----------------------------------
testAlphaRectangle - Partially transparent rectangles are drawn to the screen. Takes command-line arguments for the width and height of the screen. 
testBallroom - Multiple colored balls are drawn and follow the mouse wherever it goes. I/O capabilities: The balls follow the mouse wherever it goes on the screen. Click the left 
				       mouse button to make them repel from the mouse. Click the left mouse button again to make them follow the mouse again. Takes command-line arguments for the 
				       width and height of the screen.
testBuddhabrot - Draws a Buddhabrot on the screen, see https://en.wikipedia.org/wiki/Buddhabrot for details on what a Buddhabrot is. Takes command-line arguments for the 
				             width and height of the screen as well as the number of threads to use and the depth.
testCalcPi - 
testColorPoints -  A pattern is drawn to the screen. Takes command-line arguments for the width and height of the screen as well as for the number of threads to use. 
testColorWheel - Shows a color wheel that acts as a sort of sonar device. Takes command-line arguments for the width and height of the screen. 
testConcavePolygon - Draws two Concave polygons on the screen. See http://www.mathopenref.com/polygonconcave.html for information on what a Concave polygon is.
						    Takes command-line arguments for the width and height of the screen.
testConway - Simulates Conway's game of life. See https://en.wikipedia.org/wiki/Conway's_Game_of_Life for more details. Takes command-line arguments for the width and 
				    height of the screen.	
testCosineIntegral - Visualizes the integration of the Cosine function. Takes command-line arguments for the width and height of the screen as well as the number of threads to 
						   use. 
testDumbSort++ - Visual representation of a Shaker sort (or Bubble sort as some call it). Takes command-line arguments for the width and height of the screen. 
testForestFire++ - Simulates a forest fire. Takes command-line arguments for the width and height of the screen.
testFunction++ - Visualizes the output of certain functions on the screen. In particular, the Cosine and Power functions as well as a special one. Takes command-line arguments for 
				      the width and height of the screen.
testGetPixels - Takes a few pixels from a photo and overwrites them with a new pixel that has a different color. 
testGradientMandelbrot++ - Draws a Mandelbrot set but with a gradient fill and color palate. See http://linas.org/art-gallery/escape/smooth.html for what a Mandelbrot set is. 
								 I/O capabilities:  Scroll up on the mouse wheel to zoom in, scroll down to zoom out. Clicking on the left mouse button does the same thing for zooming
								in. Clicking on the right mouse button does the same thing for zooming out. Takes command-line arguments for the width and height of the screen as 
								well as for the number of threads and for the depth.
testGradientWheel++ - Shows a color wheel with a gradient color palate on a black background. Takes command-line arguments for the width and height of the screen. 
testGraydient++ - Shows a black-to-white gradient. Takes command-line arguments for the width and height of the screen as well as for the number of threads to use. 
testGreyscale++ - Takes pixels from an colored image and converts them to greyscale colors. Takes command-line arguments for the width and height of the screen as well as for the 
				        number of threads to use. 
testHighData - Simple stress test for the Canvas class where a large number of pixels are drawn to the screen at a high framerate.
testImage - Draws a set of images on a screen. 
testImageCart - Draws a set of images on a screen that has a Cartesian coordinate system. 
testInverter - Inverts the colors of an colored image to their greyscale colors. 
testJulia++ - Displays a Julia set. See https://en.wikipedia.org/wiki/Julia_set for more details on what a Julia set is. Takes command-line arguments for the width and the height of the
			     screen as well as for the number of threads to use and the depth of the set. I/O capabilities: Scroll up with the mouse wheel to zoom in, scroll down to zoom out.

testLangton -  Displays several visualizations of Langton's Ant (For a detailed look at Langton's Ant, go here: https://en.wikipedia.org/?title=Langton%27s_ant ). Takes command-line
                                 arguments for the width and height of the screen. There are five visualizations in total. Some may have I/O capabilities. Hit ESC to iterate through each one. 
                                 The visualizations are described below:
				+  Simulates four Langton's Ants that are alpha transparent.   I/O capabilities: Click the left mouse button or press the ENTER key to make the screen flash more. 
				     Press the spacebar to clear the screen.
                                +   Displays one Langton's Ant. 
                                +   Displays four different colored Langton's Ants.
                                +   Display four dynamically colored Langton's Ants.  
                                +   Displays another simulation of Langton's Ant with four colored ants on a black background. I/O capabilities: Press the ENTER key to pause the animation.
                                      Press the ENTER key again to resume the animation. Press the spacebar to clear the screen. The screenshots are saved to wherever the test is running (if you 
	                              run it in the bin folder, they will be saved there. If you run it anywhere else, they'll be saved in whatever folder the test is located in).
testLineChain++ - Displays randomly colored lines. Takes command-line arguments for the width and height of the screen.
testLineFan - Displays a vibrant fan of colored lines.
testMandelbrot++ - Draws another Mandelbrot set with a colored outline on a black background. I/O capabilities: Scroll up on the mouse wheel to zoom in, scroll down to zoom out. 
					     Clicking on the left mouse button does the same thing for zooming in. The right mouse button does the same thing for zooming out.
					     Takes command-line arguments for the width and height of the screen and for the number of threads to use and for the depth.
testMaster* - Does nothing at the moment.
testMouse++ - A painting tool with the mouse on a white screen. I/O capabilities:  Clicking and holding down on the left mouse button while dragging the mouse around on the 
				white background draws a line. Releasing the left mouse button draws a Convex polygon based on the outline of the shape drawn with the mouse's path.
				Pressing the spacebar clears the screen. Takes command-line arguments for the width and height of the screen.
testMultiCanvas++ - Tests the ability of the library to have multiple screens open at once.
testNewtonPendulum - Displays a simulation of Newton's Pendulum (or Newton's Cradle as some call it). See https://en.wikipedia.org/wiki/Newton's_cradle for more details.
testNova++ - Displays a Nova fractal. See http://en.wikipedia.org/wiki/Nova_fractal for more details. Takes command-line arguments for the width and height of the screen as well as
			      for the number of threads to use.
testPong - Play the hit old-school game, Pong. I/O capabilities: The w and s keys are for moving the left paddle up and down (respectively) and the up and down arrow keys move the
			right paddle up and down (respectively). 
testProgressBar++ - Displays the progress bar that is shown in testMandelbrot. Takes command-line arguments for the width and height of the screen.
testProjectiles++ - Play a target shooting game. There are 10 targets and you have to hit the center of the target in order to get a point. Your score is printed after all 10 targets have 
					  been displayed. I/O capabilities: Click the mouse in the center of the target to get a point. Takes command-line arguments for the width and height of the screen.
testSeaUrchin - Displays colorful sea urchins with the title, "Sea Urchins in the Spotlight!" If you close the window, the message "YOU KILLED MY SEA URCHINS! : ' (" is outputted to the
				   console.
testScreenshot - Displays multiple colorful triangles then takes a screenshot of each drawing frame. The screenshots are saved in a similar fashion as they are in the 
				      testLangton's fifth visualization (wherever the test is running is where the screenshots will be saved). Takes command-line arguments for the width and height of the
                                      screen.
testShadedVoronoi++ - Displays a Voronoi diagram that has been shaded. See https://en.wikipedia.org/wiki/Voronoi_diagram for more details on what a Voronoi diagram is. Takes 
						command-line arguments for the width and height of the screen. 
testSmartSort++ - Simulates the mergesort algorithm. Takes command-line arguments for the size of the screen as well as for the number of threads to use. 
testSpectrum++ - Draws the full spectrum of colors across the screen and changes over time. Takes a command-line argument for the number of threads to use in drawing. 
testText - Displays text on the screen. 
testTextCart - Displays text on a screen that has a Cartesian coordinate system.
testTextTwo - Displays text on the screen (in a different font).
testUnits - Unit tests for a set number of key classes used in the library. 
testVoronoi++ - Displays another Voronoi pattern. Takes command-line arguments for the width and height of the screen.  