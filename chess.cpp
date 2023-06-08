#include <iostream>

using namespace std;

const int BOARDSIZE = 8;
string history = "";

/*
w - white
b - black
K - king (король)
Q - queen (королева)
R - rook (тура)
H - knight (кінь)
B - bishop (слон)
P - pawn (пішак)
e - empty
*/

class Tile {
private:
    char color;
    char piece;
    bool special;
public:
    Tile(char c, char p) {
        color = c;
        piece = p;
        special = 0;
    }

    Tile() : Tile('e', 'e') {};

    void SetColor(char newcolor) {
        if (newcolor == 'b' or newcolor == 'w')
            color = newcolor;
    }

    void SetPiece(char newpiece) {
        piece = newpiece;
    }

    void SetSpecial(bool newspecial) {
        special = newspecial;
    }

    void ClearTile() {
        color = 'e';
        piece = 'e';
    }

    char GetColor() {
        return color;
    }

    char GetPiece() {
        return piece;
    }

    bool GetSpecial() {
        return special;
    }

};

void PrintBoard(Tile board[][BOARDSIZE]);

void SetBoard(Tile board[][BOARDSIZE]);

bool IsMoveAllowed(Tile board[][BOARDSIZE], int startX, int startY, int stopX, int stopY, char clr);

void MovePiece(Tile board[][BOARDSIZE], int startX, int startY, int stopX, int stopY);

bool MovePlayer(Tile board[][BOARDSIZE], char clr);

bool Check(Tile board[][BOARDSIZE], int startX, int startY, int stopX, int stopY, char clr);

void Return(Tile board[][BOARDSIZE], int startX, int startY, int stopX, int stopY, char memFig, char memClr, bool memSpcl);



int main() {
    Tile board[BOARDSIZE][BOARDSIZE];
    bool game = true;

    SetBoard(board);
    while (game) {
        cout << "\nWhite player turn:\n\n";
        game = MovePlayer(board, 'w');
        if (!game) {
            break;
        }
        cout << "\nBlack player turn:\n\n";
        game = MovePlayer(board, 'b');
    }
    cout << "Move history:\n";
    for (int i = 0; i < history.size(); i += 5) {
        cout << history[i];
        cout << history[i + 1];
        cout << history[i + 2];
        cout << history[i + 3];
        cout << history[i + 4] << "\n";
    }
}

void PrintBoard(Tile board[][BOARDSIZE]) {
    cout << "    A   B   C   D   E   F   G   H\n";
    for (int y = 0; y < BOARDSIZE; y++) {
        cout << BOARDSIZE - y << " ";
        for (int x = 0; x < BOARDSIZE; x++) {
            if (board[y][x].GetColor() != 'e') {
                cout << "|" << board[y][x].GetColor() << board[y][x].GetPiece() << ' ';
            }
            else {
                cout << "|   ";
            }
        }
        cout << "| " << BOARDSIZE - y << "\n   ";
        for (int x = 0; x < BOARDSIZE; x++) {
            cout << "----";
        }
        cout << "\n";
    }
    cout << "    A   B   C   D   E   F   G   H\n";
}

void SetBoard(Tile board[][BOARDSIZE]) {
    char sequence[BOARDSIZE] = { 'R', 'H', 'B', 'Q', 'K', 'B', 'H', 'R' };
    for (int x = 0; x < BOARDSIZE; x++) {
        board[0][(BOARDSIZE - 1) - x].SetColor('b');
        board[0][(BOARDSIZE - 1) - x].SetPiece(sequence[x]);
        board[1][x].SetColor('b');
        board[1][x].SetPiece('P');
        board[6][x].SetColor('w');
        board[6][x].SetPiece('P');
        board[7][x].SetColor('w');
        board[7][x].SetPiece(sequence[x]);
    }
    for (int x = 0; x < BOARDSIZE; x++) {
        board[1][x].SetSpecial(true);
        board[6][x].SetSpecial(true);
        if (board[0][x].GetColor() == 'K' or board[0][x].GetColor() == 'R') {
            board[0][x].SetSpecial(true);
        }
        if (board[7][x].GetColor() == 'K' or board[7][x].GetColor() == 'R') {
            board[7][x].SetSpecial(true);
        }
    }
}

bool IsMoveAllowed(Tile board[][BOARDSIZE], int startX, int startY, int stopX, int stopY, char clr) {

    if (startX >= BOARDSIZE or startY >= BOARDSIZE or stopX >= BOARDSIZE or stopY >= BOARDSIZE
        or startX < 0 or startY < 0 or stopX < 0 or stopY < 0) {
        cout << "\nWrong command or coordinate\n\n";
        return false;
    }

    if (board[startY][startX].GetColor() != clr) {
        cout << "\nStarting tile is empty or has another player's figure\n\n";
        return false;
    }

    if (board[stopY][stopX].GetColor() == clr) {
        cout << "\nDestination tile has your figure on it\n\n";
        return false;
    }

    switch (board[startY][startX].GetPiece()) {
    case 'K':
        if (startX - stopX <= 1 and startX - stopX >= -1 and
            startY - stopY <= 1 and startY - stopY >= -1) {
            board[startY][startX].SetSpecial(false);
            return Check(board, startX, startY, stopX, stopY, clr);
        }

        else {
            cout << "\nThis figure cannot move in that way\n\n";
            return false;
        }
        break;
    case 'Q':
        if (startX == stopX) {//vertical
            for (int i = min(startY, stopY) + 1; i < max(startY, stopY); i++) {
                if (board[i][startX].GetPiece() != 'e') {
                    cout << "\nThere is a figure in the path\n\n";
                    return false;
                }
            }
            return Check(board, startX, startY, stopX, stopY, clr);
        }

        else if (startY == stopY) {//horizontal
            for (int i = min(startX, stopX) + 1; i < max(startX, stopX); i++) {
                if (board[startY][i].GetPiece() != 'e') {
                    cout << "\nThere is a figure in the path\n\n";
                    return false;
                }
            }
            return Check(board, startX, startY, stopX, stopY, clr);
        }

        else if (startX < stopX and startY < stopY and startX - stopX == startY - stopY
            or startX > stopX and startY > stopY and startX - stopX == startY - stopY) { //diagonal 1
            for (int x = min(startX, stopX) + 1, y = min(startY, stopY) + 1;
                x < max(startX, stopX) and y < max(startY, stopY); x++, y++) {
                if (board[y][x].GetPiece() != 'e') {
                    cout << "\nThere is a figure in the path\n\n";
                    return false;
                }
            }
            return Check(board, startX, startY, stopX, stopY, clr);
        }

        else if (startX < stopX and startY > stopY and startX - stopX == stopY - startY) { //diagonal 2
            for (int x = startX + 1, y = startY - 1; x < stopX and y > stopY; x++, y--) {
                if (board[y][x].GetPiece() != 'e') {
                    cout << "\nThere is a figure in the path\n\n";
                    return false;
                }
            }
            return Check(board, startX, startY, stopX, stopY, clr);
        }

        else if (startX > stopX and startY < stopY and startX - stopX == stopY - startY) { //diagonal 3
            for (int x = startX - 1, y = startY + 1; x > stopX and y < stopY; x--, y++) {
                if (board[y][x].GetPiece() != 'e') {
                    cout << "\nThere is a figure in the path\n\n";
                    return false;
                }
            }
            return Check(board, startX, startY, stopX, stopY, clr);
        }

        else {
            cout << "\nThis figure cannot move in that way\n\n";
            return false;
        }
        break;
    case 'R':
        if (startX == stopX) {//vertical
            for (int i = min(startY, stopY) + 1; i < max(startY, stopY); i++) {
                if (board[i][startX].GetPiece() != 'e') {
                    cout << "\nThere is a figure in the path\n\n";
                    return false;
                }
            }
            board[startY][startX].SetSpecial(false);
            return Check(board, startX, startY, stopX, stopY, clr);
        }

        else if (startY == stopY) {//horizontal
            for (int i = min(startX, stopX) + 1; i < max(startX, stopX); i++) {
                if (board[startY][i].GetPiece() != 'e') {
                    cout << "\nThere is a figure in the path\n\n";
                    return false;
                }
            }
            board[startY][startX].SetSpecial(false);
            return Check(board, startX, startY, stopX, stopY, clr);
        }
        else {
            cout << "\nThis figure cannot move in that way\n\n";
            return false;
        }
        break;
    case 'B':
        if (startX < stopX and startY < stopY and startX - stopX == startY - stopY
            or startX > stopX and startY > stopY and startX - stopX == startY - stopY) { //diagonal 1
            for (int x = min(startX, stopX) + 1, y = min(startY, stopY) + 1;
                x < max(startX, stopX) and y < max(startY, stopY); x++, y++) {
                if (board[y][x].GetPiece() != 'e') {
                    cout << "\nThere is a figure in the path\n\n";
                    return false;
                }
            }
            return Check(board, startX, startY, stopX, stopY, clr);
        }

        else if (startX < stopX and startY > stopY and startX - stopX == stopY - startY) { //diagonal 2
            for (int x = startX + 1, y = startY - 1; x < stopX and y > stopY; x++, y--) {
                if (board[y][x].GetPiece() != 'e') {
                    cout << "\nThere is a figure in the path\n\n";
                    return false;
                }
            }
            return Check(board, startX, startY, stopX, stopY, clr);
        }

        else if (startX > stopX and startY < stopY and startX - stopX == stopY - startY) { //diagonal 3
            for (int x = startX - 1, y = startY + 1; x > stopX and y < stopY; x--, y++) {
                if (board[y][x].GetPiece() != 'e') {
                    cout << "\nThere is a figure in the path\n\n";
                    return false;
                }
            }
            return Check(board, startX, startY, stopX, stopY, clr);
        }

        else {
            cout << "\nThis figure cannot move in that way\n\n";
            return false;
        }
        break;
    case 'H':
        if ((startX - stopX) * (startX - stopX) == 4 and (startY - stopY) * (startY - stopY) == 1 or
            (startX - stopX) * (startX - stopX) == 1 and (startY - stopY) * (startY - stopY) == 4) {
            return Check(board, startX, startY, stopX, stopY, clr);
        }
        else {
            cout << "\nThis figure cannot move in that way\n\n";
            return false;
        }
        break;
    default:
        switch (clr) {
        case 'w':
            if (startX == stopX and startY == stopY + 1 and board[stopY][stopX].GetPiece() == 'e') {
                board[startY][startX].SetSpecial(false);
                return Check(board, startX, startY, stopX, stopY, clr);
            }
            else if (startX == stopX and startY == stopY + 2 and board[stopY][stopX].GetPiece() == 'e'
                and board[startY][startX].GetSpecial()) {
                board[startY][startX].SetSpecial(false);
                return Check(board, startX, startY, stopX, stopY, clr);
            }
            else if ((startX - stopX) * (startX - stopX) == 1 and startY == stopY + 1 and board[stopY][stopX].GetColor() == 'b') {
                board[startY][startX].SetSpecial(false);
                return Check(board, startX, startY, stopX, stopY, clr);
            }
            else {
                cout << "\nThis figure cannot move in that way\n\n";
                return false;
            }
            break;

        case 'b':
            if (startX == stopX and startY == stopY - 1 and board[stopY][stopX].GetPiece() == 'e') {
                board[startY][startX].SetSpecial(false);
                return Check(board, startX, startY, stopX, stopY, clr);
            }
            else if (startX == stopX and startY == stopY - 2 and board[stopY][stopX].GetPiece() == 'e'
                and board[startY][startX].GetSpecial()) {
                board[startY][startX].SetSpecial(false);
                return Check(board, startX, startY, stopX, stopY, clr);
            }
            else if ((startX - stopX) * (startX - stopX) == 1 and startY == stopY - 1 and board[stopY][stopX].GetColor() == 'w') {
                board[startY][startX].SetSpecial(false);
                return Check(board, startX, startY, stopX, stopY, clr);
            }
            else {
                cout << "\nThis figure cannot move in that way\n\n";
                return false;
            }
            break;
        }
        break;
    }
}

void MovePiece(Tile board[][BOARDSIZE], int startX, int startY, int stopX, int stopY) {
    board[stopY][stopX].SetPiece(board[startY][startX].GetPiece());
    board[stopY][stopX].SetColor(board[startY][startX].GetColor());
    board[startY][startX].ClearTile();
}

bool MovePlayer(Tile board[][BOARDSIZE], char clr) {
    int startX, startY, stopX, stopY;
    string command;

    PrintBoard(board);
    cout << "\nEnter piece and destination coordinates without a space (i.e. 'e2e4')\n" <<
        "or 'surrender' if you want to surrender or are in a checkmate:\n\n";
    do {
        cin >> command;
    } while (command.size() < 4);

    if (command == "surrender") {//здатися
        if (clr == 'w') {
            cout << "\nWhite player has surrendered. Black player won!\n";
        }
        else {
            cout << "\nBlack player has surrendered. White player won!\n";
        }
        return false;
    }
    else if (command == "skip") { //пропустити хід (тільки для тестування)
        return true;
    }
    else {
        startX = int(command[0]) - 97;
        startY = (BOARDSIZE - 1) - (int(command[1]) - 49);
        stopX = int(command[2]) - 97;
        stopY = (BOARDSIZE - 1) - (int(command[3]) - 49);
        if (IsMoveAllowed(board, startX, startY, stopX, stopY, clr)) {
            MovePiece(board, startX, startY, stopX, stopY);
            history += command[0];
            history += command[1];
            history += command[2];
            history += command[3];
            history += ' ';
            return true;
        }
        else {
            return MovePlayer(board, clr);
        }
    }
}

bool Check(Tile board[][BOARDSIZE], int startX, int startY, int stopX, int stopY, char clr) {

    char memFig = board[stopY][stopX].GetPiece();
    char memClr = board[stopY][stopX].GetColor();
    bool memSpcl = board[stopY][stopX].GetSpecial();
    int kingX = -1, kingY = -1;

    MovePiece(board, startX, startY, stopX, stopY);

    for (int y = 0; y < BOARDSIZE; y++) {
        for (int x = 0; x < BOARDSIZE; x++) {
            if (board[y][x].GetColor() == clr and board[y][x].GetPiece() == 'K') {
                kingX = x;
                kingY = y;
                break;
            }
        }
        if (kingX != -1 and kingY != -1) {
            break;
        }
    }
    //pawn
    switch (clr) {
    case 'w':
        if (board[kingY - 1][kingX - 1].GetPiece() == 'P' and board[kingY - 1][kingX - 1].GetColor() != clr or
            board[kingY - 1][kingX + 1].GetPiece() == 'P' and board[kingY - 1][kingX + 1].GetColor() != clr) {
            cout << "\nThis move leaves you in check!\n\n";
            Return(board, startX, startY, stopX, stopY, memFig, memClr, memSpcl);
            return false;
        }
        break;
    case 'b':
        if (board[kingY + 1][kingX - 1].GetPiece() == 'P' and board[kingY + 1][kingX - 1].GetColor() != clr or
            board[kingY + 1][kingX + 1].GetPiece() == 'P' and board[kingY + 1][kingX + 1].GetColor() != clr) {
            cout << "\nThis move leaves you in check!\n\n";
            Return(board, startX, startY, stopX, stopY, memFig, memClr, memSpcl);
            return false;
        }
        break;
    }
    //horse
    int j1 = 1, j2 = 2;

    if (board[kingY + j1][kingX + j2].GetPiece() == 'H' and board[kingY + j1][kingX + j2].GetColor() != clr or
        board[kingY + j2][kingX + j1].GetPiece() == 'H' and board[kingY + j2][kingX + j1].GetColor() != clr) {
        cout << "\nThis move leaves you in check!\n\n";
        Return(board, startX, startY, stopX, stopY, memFig, memClr, memSpcl);
        return false;
    }
    j1 *= -1;
    if (board[kingY + j1][kingX + j2].GetPiece() == 'H' and board[kingY + j1][kingX + j2].GetColor() != clr or
        board[kingY + j2][kingX + j1].GetPiece() == 'H' and board[kingY + j2][kingX + j1].GetColor() != clr) {
        cout << "\nThis move leaves you in check!\n\n";
        Return(board, startX, startY, stopX, stopY, memFig, memClr, memSpcl);
        return false;
    }
    j2 *= -1;
    if (board[kingY + j1][kingX + j2].GetPiece() == 'H' and board[kingY + j1][kingX + j2].GetColor() != clr or
        board[kingY + j2][kingX + j1].GetPiece() == 'H' and board[kingY + j2][kingX + j1].GetColor() != clr) {
        cout << "\nThis move leaves you in check!\n\n";
        Return(board, startX, startY, stopX, stopY, memFig, memClr, memSpcl);
        return false;
    }
    j1 *= -1;
    if (board[kingY + j1][kingX + j2].GetPiece() == 'H' and board[kingY + j1][kingX + j2].GetColor() != clr or
        board[kingY + j2][kingX + j1].GetPiece() == 'H' and board[kingY + j2][kingX + j1].GetColor() != clr) {
        cout << "\nThis move leaves you in check!\n\n";
        Return(board, startX, startY, stopX, stopY, memFig, memClr, memSpcl);
        return false;
    }
    //horizontal 1
    for (int x = kingX + 1; x < BOARDSIZE; x++) {
        if (board[kingY][x].GetPiece() != 'e') {
            if (board[kingY][x].GetColor() != clr) {
                if (board[kingY][x].GetPiece() == 'R' or board[kingY][x].GetPiece() == 'Q') {
                    cout << "\nThis move leaves you in check!\n\n";
                    Return(board, startX, startY, stopX, stopY, memFig, memClr, memSpcl);
                    return false;
                }
                else {
                    break;
                }
            }
            else {
                break;
            }
        }
    }
    //horizontal 2
    for (int x = kingX - 1; x >= 0; x--) {
        if (board[kingY][x].GetPiece() != 'e') {
            if (board[kingY][x].GetColor() != clr) {
                if (board[kingY][x].GetPiece() == 'R' or board[kingY][x].GetPiece() == 'Q') {
                    cout << "\nThis move leaves you in check!\n\n";
                    Return(board, startX, startY, stopX, stopY, memFig, memClr, memSpcl);
                    return false;
                }
                else {
                    break;
                }
            }
            else {
                break;
            }
        }
    }
    //vertical 1
    for (int y = kingY + 1; y < BOARDSIZE; y++) {
        if (board[y][kingX].GetPiece() != 'e') {
            if (board[y][kingX].GetColor() != clr) {
                if (board[y][kingX].GetPiece() == 'R' or board[y][kingX].GetPiece() == 'Q') {
                    cout << "\nThis move leaves you in check!\n\n";
                    Return(board, startX, startY, stopX, stopY, memFig, memClr, memSpcl);
                    return false;
                }
                else {
                    break;
                }
            }
            else {
                break;
            }
        }
    }
    //vertical 2
    for (int y = kingY - 1; y >= 0; y--) {
        if (board[y][kingX].GetPiece() != 'e') {
            if (board[y][kingX].GetColor() != clr) {
                if (board[y][kingX].GetPiece() == 'R' or board[y][kingX].GetPiece() == 'Q') {
                    cout << "\nThis move leaves you in check!\n\n";
                    Return(board, startX, startY, stopX, stopY, memFig, memClr, memSpcl);
                    return false;
                }
                else {
                    break;
                }
            }
            else {
                break;
            }
        }
    }
    //horizontal 1
    for (int y = kingY + 1, x = kingX + 1; x < BOARDSIZE and y < BOARDSIZE; x++, y++) {
        if (board[y][x].GetPiece() != 'e') {
            if (board[y][x].GetColor() != clr) {
                if (board[y][x].GetPiece() == 'B' or board[y][x].GetPiece() == 'Q') {
                    cout << "\nThis move leaves you in check!\n\n";
                    Return(board, startX, startY, stopX, stopY, memFig, memClr, memSpcl);
                    return false;
                }
                else {
                    break;
                }
            }
            else {
                break;
            }
        }
    }
    //horizontal 2
    for (int y = kingY - 1, x = kingX - 1; x >= 0 and y >= 0; x--, y--) {
        if (board[y][x].GetPiece() != 'e') {
            if (board[y][x].GetColor() != clr) {
                if (board[y][x].GetPiece() == 'B' or board[y][x].GetPiece() == 'Q') {
                    cout << "\nThis move leaves you in check!\n\n";
                    Return(board, startX, startY, stopX, stopY, memFig, memClr, memSpcl);
                    return false;
                }
                else {
                    break;
                }
            }
            else {
                break;
            }
        }
    }
    //horizontal 3
    for (int y = kingY - 1, x = kingX + 1; x < BOARDSIZE and y >= 0; x++, y--) {
        if (board[y][x].GetPiece() != 'e') {
            if (board[y][x].GetColor() != clr) {
                if (board[y][x].GetPiece() == 'B' or board[y][x].GetPiece() == 'Q') {
                    cout << "\nThis move leaves you in check!\n\n";
                    Return(board, startX, startY, stopX, stopY, memFig, memClr, memSpcl);
                    return false;
                }
                else {
                    break;
                }
            }
            else {
                break;
            }
        }
    }
    //horizontal 4
    for (int y = kingY + 1, x = kingX - 1; x >= 0 and y < BOARDSIZE; x--, y++) {
        if (board[y][x].GetPiece() != 'e') {
            if (board[y][x].GetColor() != clr) {
                if (board[y][x].GetPiece() == 'B' or board[y][x].GetPiece() == 'Q') {
                    cout << "\nThis move leaves you in check!\n\n";
                    Return(board, startX, startY, stopX, stopY, memFig, memClr, memSpcl);
                    return false;
                }
                else {
                    break;
                }
            }
            else {
                break;
            }
        }
    }
    Return(board, startX, startY, stopX, stopY, memFig, memClr, memSpcl);
    return true;
}

void Return(Tile board[][BOARDSIZE], int startX, int startY, int stopX, int stopY, char memFig, char memClr, bool memSpcl) {
    MovePiece(board, stopX, stopY, startX, startY);
    board[stopY][stopX].SetPiece(memFig);
    board[stopY][stopX].SetColor(memClr);
    board[stopY][stopX].SetSpecial(memSpcl);
}