CC := g++-13
PYTHON := python3.10
CFLAGS := -std=c++20 -Wall -Wextra -O3 -mtune=native -march=native -fopenmp -I/usr/local/include -L/usr/local/lib -lyaml-cpp
DEBUGF := -std=c++20 -Wall -Wextra -gdwarf-3 -fopenmp -g -I/usr/local/include -L/usr/local/lib -lyaml-cpp
TESTFLAGS := -std=c++20 -Wall -Wextra -lgtest -lgtest_main  -I/usr/local/include  -L/usr/local/lib -lyaml-cpp
OBJS := Vec3.o Cell.o Simulation.o CellList.o UserSimulation.o UserCell.o SimulationSettings.o MoleculeSpace.o UserMoleculeSpace.o
DOBJS := D_Vec3.o D_Cell.o D_Simulation.o D_CellList.o D_UserSimulation.o D_UserCell.o D_SimulationSettings.o D_MoleculeSpace.o D_UserMoleculeSpace.o
DIR := result image video

nowdate:=$(shell date +%Y%m%d_%H%M)

# FULL_ARCHIVE := 0# 0がfalse 1がtrue

CORE := src/core
UTIL := src/utils
MAIN := src
USER := src
TEST := src/test
BACKUP := src/backup
DEBUG := src/debug

DEBUGOBJS := $(UTIL)/Vec3.cpp Cell.o Simulation.o CellList.o UserSimulation.o

SimMain: $(MAIN)/SimMain.cpp $(OBJS)
	$(CC) -o $@ $(CFLAGS) $(OBJS) $(MAIN)/SimMain.cpp  -lyaml-cpp

Debug: $(MAIN)/SimMain.cpp $(DOBJS)
	$(CC) -o $@ $(DEBUGF) $(DOBJS) $(MAIN)/SimMain.cpp -lyaml-cpp

SpeedTest: $(MAIN)/SpeedTest.cpp $(OBJS)
	$(CC) -o $@ $(CFLAGS) $(OBJS) $(MAIN)/SpeedTest.cpp

MoleculeDebug: $(DEBUG)/MoleculeSpaceDebug.cpp D_MoleculeSpace.o D_SimulationSettings.o
	$(CC) -o $@ $(DEBUGF) D_MoleculeSpace.o D_SimulationSettings.o $(DEBUG)/MoleculeSpaceDebug.cpp

# プログラムの定数倍最適化を考える際に使う
# 新しくプロファイリングしたいときは、result.traceを削除してから実行する
Profiling: Debug
	xcrun xctrace record --template 'Time Profiler' --output ./result.trace --launch -- ./Debug; open ./result.trace

version:
	@$(CC) --version
	@$(PYTHON) --version

SimulationSettings.o: $(USER)/SimulationSettings.cpp $(USER)/SimulationSettings.hpp
	$(CC) -o $@ -c $(CFLAGS) $(USER)/SimulationSettings.cpp

D_SimulationSettings.o: $(USER)/SimulationSettings.cpp $(USER)/SimulationSettings.hpp
	$(CC) -c -o $@ $(DEBUGF) $(USER)/SimulationSettings.cpp

Vec3.o: $(UTIL)/Vec3.cpp $(UTIL)/Vec3.hpp
	$(CC) -o $@ -c $(CFLAGS) $(UTIL)/Vec3.cpp

D_Vec3.o: $(UTIL)/Vec3.cpp $(UTIL)/Vec3.hpp
	$(CC) -c -o $@ $(DEBUGF) $(UTIL)/Vec3.cpp

Vec3Test: $(UTIL)/Vec3.cpp $(UTIL)/Vec3.hpp $(TEST)/Vec3Test.cpp Vec3.o
	$(CC) -o $@ $(TESTFLAGS) $(TEST)/Vec3Test.cpp Vec3.o
	
test: Vec3Test
	./Vec3Test

Cell.o: $(UTIL)/Vec3.cpp $(UTIL)/Vec3.hpp $(CORE)/Cell.cpp $(CORE)/Cell.hpp SimulationSettings.o Vec3.o
	$(CC) -o $@ -c $(CFLAGS) $(CORE)/Cell.cpp 

D_Cell.o: $(UTIL)/Vec3.cpp $(UTIL)/Vec3.hpp $(CORE)/Cell.cpp $(CORE)/Cell.hpp D_SimulationSettings.o
	$(CC) -c -o $@ $(DEBUGF) $(CORE)/Cell.cpp 

UserCell.o: $(UTIL)/Vec3.cpp $(UTIL)/Vec3.hpp $(CORE)/Cell.cpp $(CORE)/Cell.hpp $(USER)/UserCell.cpp $(USER)/UserCell.hpp SimulationSettings.o
	$(CC) -o $@ -c $(CFLAGS) $(USER)/UserCell.cpp 

D_UserCell.o: $(UTIL)/Vec3.cpp $(UTIL)/Vec3.hpp $(CORE)/Cell.cpp $(CORE)/Cell.hpp $(USER)/UserCell.cpp $(USER)/UserCell.hpp D_SimulationSettings.o
	$(CC) -c -o $@ $(DEBUGF) $(USER)/UserCell.cpp

Simulation.o: $(CORE)/Simulation.cpp $(USER)/SimulationSettings.hpp $(CORE)/Simulation.hpp $(CORE)/Cell.hpp $(CORE)/Cell.cpp $(CORE)/CellList.hpp $(CORE)/CellList.cpp SimulationSettings.o MoleculeSpace.o UserMoleculeSpace.o
	$(CC) -c $(CFLAGS) $(CORE)/Simulation.cpp 

D_Simulation.o: $(CORE)/Simulation.cpp $(USER)/SimulationSettings.hpp $(CORE)/Simulation.hpp $(CORE)/Cell.hpp $(CORE)/Cell.cpp $(CORE)/CellList.hpp $(CORE)/CellList.cpp D_SimulationSettings.o D_MoleculeSpace.o UserMoleculeSpace.o
	$(CC) -c -o $@ $(DEBUGF) $(CORE)/Simulation.cpp 

UserSimulation.o: $(CORE)/Simulation.cpp $(CORE)/Simulation.hpp $(USER)/UserSimulation.cpp $(USER)/UserSimulation.hpp SimulationSettings.o MoleculeSpace.o
	$(CC) -c $(CFLAGS) $(USER)/UserSimulation.cpp 

D_UserSimulation.o: $(CORE)/Simulation.cpp $(CORE)/Simulation.hpp $(USER)/UserSimulation.cpp $(USER)/UserSimulation.hpp D_SimulationSettings.o D_MoleculeSpace.o
	$(CC) -c -o $@ $(DEBUGF) $(USER)/UserSimulation.cpp 

MoleculeSpace.o: $(CORE)/MoleculeSpace.cpp $(CORE)/MoleculeSpace.hpp SimulationSettings.o $(UTIL)/Util.hpp
	$(CC) -c $(CFLAGS) $(CORE)/MoleculeSpace.cpp

D_MoleculeSpace.o: $(CORE)/MoleculeSpace.cpp $(CORE)/MoleculeSpace.hpp D_SimulationSettings.o $(UTIL)/Util.hpp
	$(CC) -c -o $@ $(DEBUGF) $(CORE)/MoleculeSpace.cpp
	
UserMoleculeSpace.o: $(CORE)/MoleculeSpace.cpp $(CORE)/MoleculeSpace.hpp $(USER)/UserMoleculeSpace.cpp $(USER)/UserMoleculeSpace.hpp SimulationSettings.o $(UTIL)/Util.hpp
	$(CC) -c $(CFLAGS) $(USER)/UserMoleculeSpace.cpp

D_UserMoleculeSpace.o: $(CORE)/MoleculeSpace.cpp $(CORE)/MoleculeSpace.hpp $(USER)/UserMoleculeSpace.cpp $(USER)/UserMoleculeSpace.hpp D_SimulationSettings.o $(UTIL)/Util.hpp
	$(CC) -c -o $@ $(DEBUGF) $(USER)/UserMoleculeSpace.cpp

SegmentTree.o: $(CORE)/SegmentTree.cpp
	$(CC) -c $(CFLAGS) $(CORE)/SegmentTree.cpp

CellList.o: $(CORE)/CellList.cpp $(CORE)/CellList.hpp $(CORE)/Cell.hpp $(CORE)/Cell.cpp SimulationSettings.o
	$(CC) -c $(CFLAGS) $(CORE)/CellList.cpp 

D_CellList.o: $(CORE)/CellList.cpp $(CORE)/CellList.hpp $(CORE)/Cell.hpp $(CORE)/Cell.cpp D_SimulationSettings.o
	$(CC) -c -o $@ $(DEBUGF) $(CORE)/CellList.cpp 

VariableRatioCellList.o: $(CORE)/VariableRatioCellList.cpp
	$(CC) -c $(CFLAGS) $(CORE)/VariableRatioCellList.cpp

SegTest: SegmentTree.o $(TEST)/SegTest.cpp
	$(CC) -o $@ $(CFLAGS) SegmentTree.o $(TEST)/SegTest.cpp

all: clean data-cleanup $(DIR) SimMain run convert open

result:
	mkdir result

image:
	mkdir image

video:
	mkdir video

run: SimMain result
	rm -f result/*
	rm -fr molecule_result/*
	./SimMain

clean: 
	rm -f *.o

data-cleanup:
	rm -f result/*
	rm -f image/*
	rm -f video/out.mp4

CONVERT := src/convert_tools

# TODO : これに合わせてディレクトリ構成を変える
reset:
	cp $(BACKUP)/config.yaml $(BACKUP)/SimulationSettings.hpp $(BACKUP)/SimulationSettings.cpp $(BACKUP)/UserSimulation.cpp $(BACKUP)/UserSimulation.hpp $(USER)/

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

data-archive:
	mkdir archive_$(nowdate)
	mkdir archive_$(nowdate)/result
	mkdir archive_$(nowdate)/image
	mkdir archive_$(nowdate)/video

ifeq ($(FULL_ARCHIVE),1)
	cp image/* archive_$(nowdate)/image/
	cp video/* archive_$(nowdate)/video
endif
	cp result/* archive_$(nowdate)/result/

	# $(ARCHIVE_COMMAND)

	zip -r archive_$(nowdate).zip archive_$(nowdate)

	rm -rf archive_$(nowdate)

PACKAGE_LIST := src/SimMain.cpp src/SimulationSettings.hpp src/UserSimulation.hpp src/UserSimulation.cpp src/UserRule.hpp

packaging:
	mkdir -p package/$(name)

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
