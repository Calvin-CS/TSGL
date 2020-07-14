# Master Makefile for Examples

# *****************************************************

SUBDIRS := $(wildcard test*/.)

all: $(SUBDIRS)

$(SUBDIRS):
	@echo ""
	@tput setaf 3;
	@echo "+++++++++++++++++ Generating Binaries for$(subst /., , $@)+++++++++++++++++"
	@tput sgr0;
	@echo ""
	$(MAKE) -C $@

.PHONY: all $(SUBDIRS)

clean:
	(cd test3DRotation && $(MAKE) clean)
	(cd testArrows && $(MAKE) clean)
	(cd testCircle && $(MAKE) clean)
	(cd testConcavePolygon && $(MAKE) clean)
	(cd testCone && $(MAKE) clean)
	(cd testConvexPolygon && $(MAKE) clean)
	(cd testCube && $(MAKE) clean)
	(cd testCuboid && $(MAKE) clean)
	(cd testCylinder && $(MAKE) clean)
	(cd testEllipse && $(MAKE) clean)
	(cd testEllipsoid && ${MAKE} clean)
	(cd testImage && $(MAKE) clean)
	(cd testLines && $(MAKE) clean)
	(cd testPrism && $(MAKE) clean)
	(cd testPyramid && $(MAKE) clean)
	(cd testRectangle && $(MAKE) clean)
	(cd testRegularPolygon && $(MAKE) clean)
	(cd testSphere && $(MAKE) clean)
	(cd testSquare && $(MAKE) clean)
	(cd testStar && $(MAKE) clean)
	(cd testText && $(MAKE) clean)
	(cd testTransparency && ${MAKE} clean)
	(cd testTriangle && $(MAKE) clean)
	(cd testTriangleStrip && ${MAKE} clean)