FT = -I/usr/local/include/freetype2 -lfreetype
GL = -lGLEW -lglfw -lGL
Comp = g++ -c -std=c++11
Ofiles = exec.o Canvas.o Color.o Error.o Timer.o TextureHandler.o Image.o Rectangle.o Polyline.o
Exec = g++ -o exec
Thread = -lpthread
Warnings = -Wno-narrowing

#don't forget to compile all of your object files!
exec: $(Ofiles)
	$(Exec) $(Ofiles) $(GL) $(Thread) $(FT)

exec.o: exec.cpp
	$(Comp) exec.cpp $(FT)

Canvas.o: Canvas.cpp Canvas.h Array.h Shape.h Texture.h Keynums.h Error.h Util.h Timer.h Array.h Image.h Rectangle.h Polyline.h
	$(Comp) Canvas.cpp $(GL) $(Thread) $(FT) $(Warnings)

Color.o: Color.cpp Color.h Util.h
	$(Comp) Color.cpp

Timer.o: Timer.cpp Timer.h
	$(Comp) Timer.cpp

Error.o: Error.cpp Error.h
	$(Comp) Error.cpp

Image.o: Image.cpp Image.h Shape.h TsglAssert.h TextureHandler.h
	$(Comp) Image.cpp $(FT)

TextureHandler.o: TextureHandler.cpp TextureHandler.h Error.h TsglAssert.h Util.h
	$(Comp) TextureHandler.cpp $(FT) $(GL) $(Warnings)

Rectangle.o: Rectangle.cpp Rectangle.h Shape.h
	$(Comp) Rectangle.cpp

Polyline.o: Polyline.cpp Polyline.h Shape.h
	$(Comp) Polyline.cpp

clean:
	rm -f *.o *.gch *~ exec
