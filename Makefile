CC := g++
PYTHON := python
CFLAGS := -std=c++11 -Wall -Wextra -O2 
OBJS := Vec3.o Cell.o Simulation.o CellList.o UserSimulation.o
DIR := result image video

nowdate:=$(shell date +%Y%m%d_%H%M)

FULL_ARCHIVE := 0# 0がfalse 1がtrue

CORE := src/core
UTIL := src/utils
MAIN := src
USER := src
TEST := src/test
BACKUP := src/backup

SimMain: $(MAIN)/SimMain.cpp $(OBJS) result
	$(CC) -o SimMain $(CFLAGS) $(OBJS) $(MAIN)/SimMain.cpp

Vec3.o: $(UTIL)/Vec3.cpp
	$(CC) -c $(CFLAGS) $(UTIL)/Vec3.cpp

Cell.o: $(CORE)/Cell.cpp
	$(CC) -c $(CFLAGS) $(CORE)/Cell.cpp

Simulation.o: $(CORE)/Simulation.cpp $(USER)/SimulationSettings.hpp
	$(CC) -c $(CFLAGS) $(CORE)/Simulation.cpp

UserSimulation.o: $(CORE)/Simulation.cpp $(USER)/UserSimulation.cpp
	$(CC) -c $(CFLAGS) $(USER)/UserSimulation.cpp

SegmentTree.o: $(CORE)/SegmentTree.cpp
	$(CC) -c $(CFLAGS) $(CORE)/SegmentTree.cpp

CellList.o: $(CORE)/CellList.cpp
	$(CC) -c $(CFLAGS) $(CORE)/CellList.cpp

VariableRatioCellList.o: $(CORE)/VariableRatioCellList.cpp
	$(CC) -c $(CFLAGS) $(CORE)/VariableRatioCellList.cpp

seg-test: SegmentTree.o $(TEST)/SegTest.cpp
	$(CC) -o SegTest $(CFLAGS) SegmentTree.o $(TEST)/SegTest.cpp

all: clean $(DIR) SimMain run convert open

result:
	mkdir result

image:
	mkdir image

video:
	mkdir video

run: SimMain result
	rm -f result/*
	./SimMain

clean: 
	rm -f *.o

data-cleanup:
	rm -f result/*
	rm -f image/*
	rm -f video/out.mp4

CONVERT := src/convert_tools

reset:
	cp $(BACKUP)/SimulationSettings.hpp $(BACKUP)/UserSimulation.cpp $(BACKUP)/UserSimulation.hpp $(USER)/
	cp $(BACKUP)/create_image.py $(CONVERT)/

png: result image
	$(PYTHON) $(CONVERT)/create_image.py

img2video: image video
	$(PYTHON) $(CONVERT)/img2video.py

convert: $(DIR)
	rm -f image/*
	rm -f video/out.mp4
	$(PYTHON) $(CONVERT)/create_image.py
	$(PYTHON) $(CONVERT)/img2video.py

open:
	open video/out.mp4

ifeq (FULL_ARCHIVE,1)
	ARCHIVE_COMMAND := cp image/* archive_$(nowdate)/image\
	cp video/* archive_$(nowdate)/video
endif

data-archive:
	mkdir archive_$(nowdate)
	mkdir archive_$(nowdate)/result
	mkdir archive_$(nowdate)/image
	mkdir archive_$(nowdate)/video

	cp result/* archive_$(nowdate)/result/

	$(ARCHIVE_COMMAND)

	zip -r archive_$(nowdate).zip archive_$(nowdate)

	rm -rf archive_$(nowdate)

PACKAGE_LIST := src/SimMain.cpp src/SimulationSettings.hpp src/UserSimulation.hpp src/UserSimulation.cpp src/UserRule.hpp

packaging:
	mkdir package/$(name)

	cp $(PACKAGE_LIST) package/$(name)

archive-cleanup:
	rm -r archive_*

archive-restore:
	unzip archive_$(date).zip

	@touch result/result.txt
	@touch image/image.txt

	rm image/*
	rm result/*

	@touch archive_$(date)/result/result.txt
	@touch archive_$(date)/image/image.txt
	@touch archive_$(date)/video/video.txt

	mv archive_$(date)/image/* image/
	mv archive_$(date)/result/* result/
	mv archive_$(date)/video/* video/

	@rm result/result.txt
	@rm image/image.txt
	@rm video/video.txt

	rm -r archive_$(date)

help:
	@echo "Usage: make [target]"
	@echo "make : build the program"
	@echo "make run : run the program"
	@echo "make convert : convert from simulation's result to mp4"
	@echo "make open : open out.mp4"
	@echo "make all : build, run, convert, and open mp4"
	@echo "make packaging name=NAME : packaging user program"
	@echo "make data-archive : result data to zip archive"
	@echo "make archive-restore date=YYYYMMDD_HHMM : restore archive files"
	@echo "make clean : remove all object files(*.o)"
	@echo "make data-cleanup : remove all data files(*.txt, *.png, out.mp4)"
	@echo "make reset : reset SimulationSettings and UserSimulation to default"
	@echo "make archive-cleanup : remove all archive files(archive_*)"
