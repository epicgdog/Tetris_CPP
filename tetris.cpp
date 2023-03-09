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

    // Z piece right
    tetris_shapes[2].append(
        L"..X."
        L".XX."
        L".X.."
        L"...."
    );

    // Z piece left
    tetris_shapes[3].append(
        L".X.."
        L".XX."
        L"..X."
        L"...."
    );

    // Z piece right
    tetris_shapes[4].append(
        L"...."
        L".XX."
        L".X.."
        L".X.."
    );

    // Z piece left
    tetris_shapes[5].append(
        L"...."
        L".XX."
        L"..X."
        L"..X."
    );

    // Y piece
    tetris_shapes[5].append(
        L"..X."
        L".XX."
        L"..X."
        L"...."
    );

    std::cout << "Hello World!";
    return 0;
}