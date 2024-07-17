SRC = src/maze.cpp
GENERATOR = src/generator/mazegenerator.cpp src/generator/bfs.cpp src/generator/kruskal.cpp
SOLVER = src/solver/mazesolver.cpp src/solver/dfs.cpp src/solver/dijkstra.cpp
TARGET = maze.out

compile:
	mpic++ -o $(TARGET) $(SRC) $(GENERATOR) $(SOLVER)

clean:
	rm $(TARGET)