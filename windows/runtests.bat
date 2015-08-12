set run=start /d ".\Debug\" /wait ""

%run% testAlphaRectangle.exe 640 480
%run% testAura.exe 1200 900 8             
%run% testBallroom.exe 640 640
%run% testBlurImage.exe 36 ../assets/pics/HDR_landscape.jpg                            
%run% testCalcPi.exe 100 4                       
%run% testColorPoints.exe 400 400 4              
%run% testColorWheel.exe 640 640 64               
%run% testConway.exe 640 640                     
%run% testCosineIntegral.exe 640 480 32          
%run% testDumbSort.exe 1200 600
%run% testFireworks.exe 800 800 4 50 10                  
%run% testForestFire.exe 800 600                 
%run% testFunction.exe 640 480
%run% testGetPixels.exe 4                      
%run% testGradientWheel.exe 640 640 256          
%run% testGraydient.exe 400 400 8                
%run% testGreyscale.exe 640 480 8                
%run% testHighData.exe 1200 900 4
%run% testImage.exe 1200 600
%run% testImageCart.exe 1200 600
%run% testInverter.exe 8              
%run% testLangton.exe 640 640                                 
%run% testLineChain.exe 400 400 8                
%run% testLineFan.exe 800 800 16                 
%run% testMandelbrot.exe 1200 900 8 255          
%run% testMouse.exe 900 900 5
%run% testNewtonPendulum.exe 900 400 11
%run% testPhilosophers.exe 5 10                    
%run% testPong.exe 7 4
%run% testProjectiles.exe 400 400                
%run% testSeaUrchin.exe 16
%run% testScreenshot.exe 900 650          
%run% testSmartSort.exe 1024
%run% testSpectrogram.exe ../assets/pics/test.png                     
%run% testSpectrum.exe 8                         
%run% testText.exe 1200 900 ../assets/freefont/FreeSerif.ttf
%run% testTextCart.exe 1200 900 ../assets/freefont/FreeSerifItalic.ttf
%run% testTextTwo.exe 1200 900
%run% testVoronoi.exe 640 480 8                  
