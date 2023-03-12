#include <iostream>
#include <Windows.h>
#include <string>
#include <vector>

using namespace std;


// storing the possible tetris shapes in a wstring
// wstring; a string interpreted in unicode rather than ASCII
// 7 means number of spaces, not characters
wstring tetris_shapes[7];


// need the "playing field" thingy
int nFieldWidth = 12;
int nFieldHeight = 18;


// width of window and height
int nScreenWidth = 80;
int nScreenHeight = 30;

// idk what this does; apparently dynamically stores in memory so that each thing has own special number
// so  all info is stored in one array rather than multiple
unsigned char *pField = nullptr;

// rotation function
// to get location of a character in a string can do
// i = y*w + x
// where y = row of char, w = width of total string, and x = column of the string
// rotate 90: i = 12 + y - (x*4) (is like the matrices)
// rotate 180: i = 15 - (y*4) - x
// rotate 270: i = 3 + y + (x*4)

int Rotate(int px, int py, int r){
    // switch block like in python; if it matches a case, then it will run expression within that case
    switch(r%4){
        case 0: return py * 4 + px; // 0 degrees
        case 1: return 12 + py - (px * 4); // 90 degrees
        case 2: return 15 - (py*4) - px;
        case 3: return 3 - py + (px*4);
    }
    // if not rotatable
    return 0;
}
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
    tetris_shapes[6].append(
        L"..X."
        L".XX."
        L"..X."
        L"...."
    );

    // initialize the "playing field"
    pField = new unsigned char[nFieldWidth*nFieldHeight];
    for (int x = 0; x < nFieldWidth; x++)
        for (int y = 0; y < nFieldHeight; y++)
            // literally forgot the === sign bruh
            pField[y*nFieldWidth + x] = (x == 0 || x == nFieldWidth - 1 || y == nFieldHeight - 1) ? 9 : 0;

    // testing it on screen
    wchar_t *screen = new wchar_t[nScreenWidth*nScreenHeight]; // creates an array of w_chars to represent the screen
    for (int i = 0; i < nScreenWidth*nScreenHeight; i++) screen[i] = L' ';
    // idk what this does; will have to google
    HANDLE hConsole = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
    SetConsoleActiveScreenBuffer(hConsole);
    DWORD dwBytesWritten = 0;

    // game loop; pretty basic game loop stuff
    bool bGameOver = false; 

    while (!bGameOver){
        // Draw playing field which includes teh whole board
        for (int x = 0; x < nFieldWidth; x++)
            for (int y = 0; y < nFieldHeight; y++)
                // will draw whatever index character is in; so if 0 will draw the " " in " ABDCEFG=#"
                screen[(y + 2)*nScreenWidth + (x + 2)] = L" ABCDEFG=#"[pField[y*nFieldWidth + x]];


        // ouputs the characters in the pointer?
        WriteConsoleOutputCharacter(hConsole, screen, nScreenWidth * nScreenHeight, { 0,0 }, &dwBytesWritten);
    }

    return 0;
}   