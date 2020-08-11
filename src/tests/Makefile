# Master Makefile for Tests

# *****************************************************

# SUBDIRS_TO_BUILD := $(wildcard test*/.)							# Used to build the tests
SUBDIRS_TO_BUILD := test2Dvs3D \
			test3DRotation \
			testAlphaRectangle \
			testArrows \
 			testAura \
			testBackground \
 			testBlurImage \
			testCircle \
 			testColorWheel \
			testConcavePolygon \
			testCone \
			testConvexPolygon \
			testCube \
			testCuboid \
			testCylinder \
			testDiorama \
			testEllipse \
			testEllipsoid \
			testGetColors \
 			testGetPixels \
 			testGradientWheel \
 			testGraydient \
 			testGreyscale \
 			testHighData \
			testImage \
 			testInverter \
 			testLineChain \
 			testLineFan \
			testLines \
 			testMouse \
 			testPixels \
			testPrism \
			testProcedural \
 			testProgressBar \
 			testProjectiles \
			testPyramid \
			testRectangle \
			testRegularPolygon \
 			testScreenshot \
 			testSpectrogram \
 			testSpectrum \
			testSphere \
			testSquare \
			testStar \
			testText \
 			testTextTwo \
			testTransparency \
			testTriangle \
			testTriangleStrip \
#			test_specs \
# 			testCalcPi \
# 			testCosineIntegral \
#			testDice \
# 			testFunction \
# 			testImageCart \
# 			testTextCart \
# 			testUnits \

SUBDIRS_TO_CLEAN := $(subst test,..., $(SUBDIRS_TO_BUILD))	# Used to clean the tests


all: $(SUBDIRS_TO_BUILD)

$(SUBDIRS_TO_BUILD):
	@echo ""
	@tput setaf 3;
	@echo "+++++++++++++++++ Generating Binaries for$(subst /., , $@) +++++++++++++++++"
	@tput sgr0;
	@echo ""
	$(MAKE) -C $@

.PHONY: all $(SUBDIRS_TO_BUILD) clean $(SUBDIRS_TO_CLEAN)

clean: $(SUBDIRS_TO_CLEAN)

$(SUBDIRS_TO_CLEAN):
	cd $(subst ...,test,$@) && $(MAKE) clean