# Makefile

CC = gcc
CFLAGS = `pkg-config --cflags gtk+-3.0 sdl` -Wall -Wextra -std=c99 -O3
LDLIBS = `pkg-config --libs gtk+-3.0 sdl` -lSDL_image -lm

all: main

main: main.o UI/ui.o NeuralNetwork/NeuralNetworks.o Correction/CorrectAI.o SDL/sdl.o Segmentation/segmentation_G.o NeuralNetwork/Matrix.o NeuralNetwork/Functions.o NeuralNetwork/AnnexesNeuralNetworks.o
main.o: UI/ui.h NeuralNetwork/NeuralNetworks.h SDL/sdl.h
ui.o: UI/ui.h NeuralNetwork/NeuralNetworks.h SDL/sdl.h Segmentation/segmentation_G.h Correction/CorrectAI.h
NeuralNetwork.o: NeuralNetwork/Matrix.h AnnexesNeuralNetworks.h NeuralNetwork/Functions.h
Matrix.o: NeuralNetwork/Matrix.h
AnnexesNeuralNetworks.o: NeuralNetwork/AnnexesNeuralNetworks.h NeuralNetwork/Functions.h NeuralNetwork/Matrix.h Segmentation/segmentation_G.h
Functions.o: NeuralNetwork/Functions.h NeuralNetwork/Matrix.h
segmentation_G.o: SDL/sdl.o NeuralNetwork/Matrix.h
CorrectAI.o: Segmentation/segmentation_G.h Correction/CorrectAI.h
sdl.o: SDL/sdl.h

clean:
	${RM} main *.o ./data/*.txt ./Correction/*.o ./data/*.bmp ./SDL/*.o ./NeuralNetwork/*.o ./UI/*.o ./Segmentation/*.o

# END
