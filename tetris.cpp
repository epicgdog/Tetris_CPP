#include <iostream>
#include <Windows.h>
#include <string>
#include <vector>
#include <chrono>
#include <thread>

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

// check if the piece fits in the given scenario
// nTetromino: which type of tetromino?
// nRotation: what is current rotation of tetromino
// nPos: what is the location of the tetromino; the top left
bool DoesPieceFit(int nTetromino, int nRotation, int nPosX, int nPosY){
    
    // loop over each piece of the tetromino
    for (int px = 0; px < 4;  px++)
        for (int py = 0; py < 4; py++){
            // get index into the piece (which number is it on? cuase its like a 0-15 matrix)
            int pi = Rotate(px, py, nRotation);
            // get the location of the actual square of tetromino; based on its current position
            int fi = (nPosY + py) * nFieldWidth + (nPosX + px);

            // basically c++ is prone to weird memory things; prevents weird memory thingies from affecting our results
            if (nPosX + px >= 0 && nPosX + px < nFieldWidth){
                if (nPosY + py >= 0 && nPosY + py < nFieldHeight){
                    // the actual collision detection
                    // idk why but u need to make an L string with single quotes
                    // basically, if we are in a box of the tetromino and the playing field that its on is already occupied, then there is a collision (by returning false)
                    if (tetris_shapes[nTetromino][pi] == L'X' && pField[fi] != 0){
                        return false;
                    }

                }
            }
        }

    return true;
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
    // game logic includes variables that tkeep track of what is happening
    int nCurrentPiece = 0;
    int nCurrentRotation = 0;
    int nCurrentX = nFieldWidth/2; 
    int nCurrentY = 0;
    bool bKey[4];
    bool bRotateHold = false; // flag so no spasms when rotating so when holding doesn't spin a lot

    // speed of dropping
    int nSpeed = 20;
    int nSpeedCounter = 0;
    bool bForceDown = false;

    while (!bGameOver){

        // want to focus on TIMING FIRST ===================
        // think of it as the task.wait() in roblox lua
        this_thread::sleep_for(50ms);
        // want to lessen the time per forced drop right?
        nSpeedCounter++;
        // bForceDown illustrates the need to force the current tetromino down or not
        bForceDown = (nSpeedCounter == nSpeed);


        // then USER INPUT (not even event driven lol) ========================
        // to get the key input, use getasynckeystate that tells if it is pressed or not
        for (int k = 0; k < 4; k++){
            // have 0 clue how this works                        R    L   D   Z
            bKey[k] = (0x8000 & GetAsyncKeyState((unsigned char)("\x27\x25\x28Z"[k]))) != 0;
        }

        // then check collisions and tetris in the GAME LOGIC SECTION ===============

        // when keys are pressed, move the tetromino accordingly
        // using ternary bc just better :)
        nCurrentX += (bKey[0] && DoesPieceFit(nCurrentPiece, nCurrentRotation, nCurrentX + 1, nCurrentY)) ? 1 : 0;
        nCurrentX -= (bKey[1] && DoesPieceFit(nCurrentPiece, nCurrentRotation, nCurrentX - 1, nCurrentY)) ? 1 : 0;
        nCurrentY += (bKey[2] && DoesPieceFit(nCurrentPiece, nCurrentRotation, nCurrentX, nCurrentY+1)) ? 1 : 0;
        
        if (bKey[3]){
            nCurrentRotation += (!bRotateHold && DoesPieceFit(nCurrentPiece, nCurrentRotation + 1, nCurrentX, nCurrentY))? 1 : 0;
            bRotateHold = true;
        } else{
            bRotateHold = false;
        }

        // when it needs to be forced down
        if (bForceDown){
            // check if can fit
            if (DoesPieceFit(nCurrentPiece, nCurrentRotation, nCurrentX, nCurrentY+1)){
                nCurrentY++;
            } else {
                // lock the current piece in the field; not movable anymore
                 for (int px = 0; px < 4; px++)
                    for (int py = 0; py < 4; py++){
                        if (tetris_shapes[nCurrentPiece][Rotate(px, py, nCurrentRotation)] != L'.'){
                            // lock in by adding it to the playing field array
                            pField[(nCurrentY + py)*nScreenWidth + (nCurrentX + px)] = nCurrentPiece + 1;
                        }
                }
                // check any horizontal lines
                // choose the next piece
                nCurrentX = nFieldWidth / 2;
				nCurrentY = 0;
				nCurrentRotation = 0;
				nCurrentPiece = rand() % 7;
                // if can't fit, then game over!
                bGameOver = !DoesPieceFit(nCurrentPiece, nCurrentRotation, nCurrentX, nCurrentY);
            }

            nSpeedCounter = 0; 
        }

        //finallly want to RENDER OUTPUT

        // Draw playing field which includes teh whole board
        for (int x = 0; x < nFieldWidth; x++)
            for (int y = 0; y < nFieldHeight; y++)
                // will draw whatever index character is in; so if 0 will draw the " " in " ABDCEFG=#"
                screen[(y + 2)*nScreenWidth + (x + 2)] = L" ABCDEFG=#"[pField[y*nFieldWidth + x]];

        // Draw the current piece 
        for (int px = 0; px < 4; px++)
            for (int py = 0; py < 4; py++){
                // basically get teh current rotation of teh piece and determining if that space has an X or not
                if (tetris_shapes[nCurrentPiece][Rotate(px, py, nCurrentRotation)] == L'X'){
                    // if it does, we add it to this screen thingy that will render it
                    // using y*width + x
                    // adding 65 because it is the ASCII code
                    screen[(nCurrentY + py + 2)*nScreenWidth + (nCurrentX + px + 2)] = nCurrentPiece + 65;
                }
            }

        // ouputs the characters in the pointer?
        WriteConsoleOutputCharacter(hConsole, screen, nScreenWidth * nScreenHeight, { 0,0 }, &dwBytesWritten);
    }

    return 0;
}   