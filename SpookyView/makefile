CXX     := g++
WINDRES := windres
OBJ_DIR := obj
BIN_DIR := bin

EXE := $(BIN_DIR)/SpookyView.exe
SRC := $(wildcard *.cpp)
OBJ := $(SRC:%.cpp=$(OBJ_DIR)/%.o)
RES := $(OBJ_DIR)/SpookyViewRes.o $(OBJ_DIR)/Manifest.o

CXXFLAGS := -std=c++14 -DWIN32 -D_WINDOWS -DDEBUG -DUNICODE -D_UNICODE -g -nostdlib \
            -I. -I..\packages\nlohmann.json.3.10.5\build\native\include
LDFLAGS  := -lcomctl32 -lcomdlg32 -lshlwapi -lversion -lpsapi -lwinhttp -mwindows -municode

.PHONY: build clean

build: $(EXE)

$(EXE): $(OBJ) $(RES) | $(BIN_DIR)
	$(CXX) $(OBJ) $(RES) -o $@ $(LDFLAGS)

$(OBJ_DIR)/%.o: %.cpp | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJ_DIR)/SpookyViewRes.o: SpookyView.rc | $(OBJ_DIR)
	$(WINDRES) $< $@

$(OBJ_DIR)/Manifest.o: Manifest.rc | $(OBJ_DIR)
	$(WINDRES) $< $@

$(OBJ_DIR):
	@if not exist "$(OBJ_DIR)" mkdir "$(OBJ_DIR)"

$(BIN_DIR):
	@if not exist "$(BIN_DIR)" mkdir "$(BIN_DIR)"

clean:
	@if exist "$(OBJ_DIR)" rmdir /s /q "$(OBJ_DIR)"
	@if exist "$(BIN_DIR)" rmdir /s /q "$(BIN_DIR)"