# App name
NAME = rpg_side_screen
CC = g++ -std=c++20 -Wno-narrowing

STATIC_LIBS_WIN = -static-libstdc++ -static-libgcc

INCLUDES = -Iinclude -Iinclude/sdl -Iinclude/headers -Llib

# platform-specific libs
ifeq ($(OS),Windows_NT)
    LIBS = -lstdc++fs -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_gpu -lSDL2_mixer -lole32 -mwindows
    DEBUG_LIBS = -lstdc++fs -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_gpu -lSDL2_mixer -lpsapi -lpdh -lole32 -mwindows
    OUTPUT_EXT = .exe
    ICON = res/icon/icon.res
    STATIC_LIBS = $(STATIC_LIBS_WIN)
else
    LIBS = -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_gpu -lSDL2_mixer
    DEBUG_LIBS = $(LIBS)
    OUTPUT_EXT =
    ICON =
    STATIC_LIBS =
endif

DEBUG_FOLDER = .build
RELEASE_FOLDER = .release
BIN = bin/*.o

DEBUG_FLAGS = -D_ASSERTS -D_DEBUG -D_IMGUI
GAME_FLAGS = -D_IMGUI -DWIN_WIDTH=1600 -DWIN_HEIGHT=900 -DGAME_SCALE=1

bin_dir:
	mkdir -p bin

SRC_DIRS := src/imgui src/core src/entity/assets src/entity/tabs src/entity src/entity/data src/entity/actions src/entity/editors/modules src/entity/editors src/entity/visualizers src/renderer src/res src/game src/tools
OBJ_FILES := $(foreach dir, $(SRC_DIRS), $(patsubst $(dir)/%.cpp, bin/%.o, $(wildcard $(dir)/*.cpp)))

bin/%.o: src/**/%.cpp
	$(CC) $(GAME_FLAGS) $(if $(filter true,$(DEBUG)),-g $(DEBUG_FLAGS)) $(INCLUDES) -c $< -o $@

debug: $(OBJ_FILES)
	${CC} -g -O0 $(STATIC_LIBS) $(INCLUDES) -o $(DEBUG_FOLDER)/fortress$(OUTPUT_EXT) $(OBJ_FILES) $(ICON) $(DEBUG_LIBS) -mconsole

build: $(OBJ_FILES)
	${CC} -s -O3 -finline-functions -flto $(STATIC_LIBS) $(INCLUDES) -o $(RELEASE_FOLDER)/${NAME}$(OUTPUT_EXT) $(OBJ_FILES) $(ICON) $(LIBS)

compile: bin_dir $(OBJ_FILES)
	${CC} $(STATIC_LIBS) $(INCLUDES) -o ${NAME}_debug$(OUTPUT_EXT) $(OBJ_FILES) $(ICON) $(LIBS) -mconsole
