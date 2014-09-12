import os

env = Environment(
	CC="g++",
	CCFLAGS="-std=c++11 -g -pedantic -Wall -Wno-switch -O3",
	CPPPATH="#/",
	LIBS=[]
)

source = Glob("src/*.cpp")

# get the base name without the extension
program = env.Program(target='bin/checkers', source=source)
Install("../bin", source = program)