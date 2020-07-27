# Master Makefile for Tests

# *****************************************************

# SUBDIRS_TO_BUILD := $(wildcard test*/.)							# Used to build the tests
SUBDIRS_TO_BUILD := test_specs \
			test2Dvs3D \
			test3DRotation \
			testAlphaRectangle \
			testArrows \
			testBackground \
			testCircle \
			testConcavePolygon \
			testCone \
			testConvexPolygon \
			testCube \
			testCuboid \
			testCylinder \
			testDice \
			testDiorama \
			testEllipse \
			testEllipsoid \
			testImage \
			testLines \
			testPrism \
			testProcedural \
			testPyramid \
			testRectangle \
			testRegularPolygon \
			testSphere \
			testSquare \
			testStar \
			testText \
			testTransparency \
			testTriangle \
			testTriangleStrip \
# 			testAura \
# 			testBlurImage \
# 			testCalcPi \
# 			testColorWheel \
# 			testConstructors \
# 			testCosineIntegral \
# 			testFunction \
# 			testGetPixels \
# 			testGradientWheel \
# 			testGraydient \
# 			testGreyscale \
# 			testHighData \
# 			testImageCart \
# 			testInverter \
# 			testLineChain \
# 			testLineFan \
# 			testMouse \
# 			testPixels \
# 			testProgressBar \
# 			testProjectiles \
# 			testScreenshot \
# 			testSpectrogram \
# 			testSpectrum \
# 			testTextCart \
# 			testTextTwo \
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