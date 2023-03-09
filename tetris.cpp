#include <iostream>

using namespace std;


// storing the possible tetris shapes in a wstring
// wstring; a string interpreted in unicode rather than ASCII
// 7 means number of spaces, not characters
wstring tetris_shapes[7];

int main(){
    // creating the shapes
    // long piece
    tetris_shapes[0].append(
        L"..X."
        L"..X."
        L"..X."
        L"..X."
    );

    // square piece
    tetris_shapes[1].append(
        L"...."
        L".XX."
        L".XX."
        L"...."
    );

    // square piece
    tetris_shapes[2].append(
        L"...."
        L".XX."
        L".XX."
        L"...."
    );

    std::cout << "Hello World!";
    return 0;
}