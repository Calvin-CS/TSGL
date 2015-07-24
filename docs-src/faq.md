## FAQ

Q: A test keeps crashing with the error "the font file could not be opened and read
"! What do I do?

A: Are you in the bin folder of the tsgl folder? Some tests won't work unless you are in the bin folder. Specifically:
* testBlurImage
* testCalcPi
* testCosineIntegral
* testGetPixels
* testGreyscale
* testImage
* testImageCart
* testInverter
* testMandelbrot
* testPhilosophers
* testProgressBar
* testSpectrogram
* testText
* testTextCart
* testTextTwo
* testUnits

If the test that keeps crashing is in the list above, go into the bin folder and execute it from there.

Q: Windows Threads?

A: You can only have up to 64 threads at a time in Windows.
