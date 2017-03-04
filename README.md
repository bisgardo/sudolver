Sodolver
========

Sudoku solver written in C that uses simple standard methods with optimistic
brute-force fallback. It has currently been able to solve every single one of
the approximately nine boards that has been thrown at it so far (including what
is allegedly the [hardest sudoku in the world](http://www.telegraph.co.uk/news/science/science-news/9359579/Worlds-hardest-sudoku-can-you-crack-it.html)).

The command

    ./sudolver [[from-recursion-depth:5] to-recursion-depth:9]

parses a board from standard input one cell value at a time in row-order.
Numbers define the value of the corresponding cell while space characters
correspond to blank/undetermined cells. Newlines are ignored. When the parser
has received characters for each of the 81 cells, it ignores any remaining
input and starts the solver.

The solver uses simple standard methods to try and eliminate candidate values
from the cells that are not yet solved. If the solver gets stuck, it will pick
a cell and make an educated guess about its value and see if this allows the
solver to crack the puzzle. If not, the solver may either make another guess
recursively or backtrack in order to make a different guess.

The maximum number of such guesses that will be performed recursively ranges
from `from-recusion-depth` to `to-recursion-depth`: If the search depth
is insuffient to find the solution, it is incremented by 1 until the whole
range of depths has been attempted.

### Build

The project has currently only been built on Visual Studio 2015, but is planned
to be made ANSI C compliant and compileable on both Windows and Linux as a
CMake project.

