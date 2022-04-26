#include <bits/stdc++.h>
#include <time.h>
#include <windows.h>

//public private and all fn definitions outside classes

using namespace std;

struct Move {
    int posn;
    int util;
};

class TicTacToe {

    private:
        char winner = ' ';

        friend class IntelligentPlayer;
        friend class HumanPlayer;

    public:
        TicTacToe();

        char gameBoard[3][3];
        
        char getWinner();

        void print_board(int printChoice);

        bool putMove(int posn, char letter);

        bool winnerCheck(int posn, char letter);

        int num_empty();

        string possible_moves();
};

TicTacToe :: TicTacToe() {
    for(int i = 0; i < 3; i++)
        for(int j = 0; j < 3; j++)
            gameBoard[i][j] = ' ';
}

char TicTacToe :: getWinner() {
    return winner;
}

void TicTacToe :: print_board(int printChoice) {
    for(int i = 0; i < 3; i++) {
        // if(i)
        //     cout << "----------\n";
        for(int j = 0; j < 3; j++){
            
            char charToPrint;

            if(printChoice)
                charToPrint = gameBoard[i][j] == ' ' ? 3*i + j + '0' : gameBoard[i][j];     //show positions
            else
                charToPrint = gameBoard[i][j];      //don't show positions

            if(j+1)
                cout << " | ";
            
            cout << charToPrint;

            if(j==2)
                cout << " |";
        }
        cout << '\n';
    }
}

bool TicTacToe :: putMove(int posn, char letter) {               //makemove
    if(gameBoard[posn/3][posn%3] == ' ') {
        gameBoard[posn/3][posn%3] = letter;

        if(winnerCheck(posn, letter))
            winner = letter;
        
        return true;
    }
    return false;
}

bool TicTacToe :: winnerCheck(int posn, char letter) {
    int row = posn / 3;
    int col = posn % 3;
    bool ch = true;
    
    //check if row complete
    for(int j = 0; j < 3; j++)
        if(gameBoard[row][j] != letter) {
            ch = false;
            break;
        }
    
    if(ch)
        return ch;
    
    ch = true;

    //check if column complete
    for(int i = 0; i < 3; i++)
        if(gameBoard[i][col] != letter) {
            ch = false;
            break;
        }

    if(ch)
        return ch;

    if(posn % 2 == 0) {

        ch = true;

        //check if left diagonal complete
        for(int i = 0; i < 3; i++)
            if(gameBoard[i][i] != letter) {
                ch = false;
                break;
            }
        
        if(ch)
            return ch;
        
        ch = true;

        //check if right diagonal complete
        for(int i = 0; i < 3; i++)
            if(gameBoard[i][2-i] != letter) {
                ch = false;
                break;
            }
    }
    
    return ch;
}

int TicTacToe :: num_empty() {
    int ctr = 0;
    for(int i = 0; i < 3; i++)
        for(int j = 0; j < 3; j++)
            if(gameBoard[i][j] == ' ')
                ctr++;

    return ctr;
}

string TicTacToe :: possible_moves() {
    string ctr = "";
    for(int i = 0; i < 3; i++)
        for(int j = 0; j < 3; j++)
            if(gameBoard[i][j] == ' ')
                ctr += ((3*i) + j) + '0';
                //ctr.append(to_string((3*i) + j));

    return ctr;
}

class Player {
    private:
        char letter;

    public:
        Player(char letterOfPlayer);

        char getLetter();

        virtual Move move(TicTacToe g) {return{};}
};

Player :: Player(char letterOfPlayer) {
    letter = letterOfPlayer;
}

char Player :: getLetter() {
    return letter;
}

class IntelligentPlayer : public Player {

    private:
        int utility(TicTacToe g);

        Move bestMove(TicTacToe t, char player);

    public:
        IntelligentPlayer(char letterOfPlayer);

        Move move(TicTacToe g);
};

IntelligentPlayer :: IntelligentPlayer(char letterOfPlayer): Player(letterOfPlayer) {}

Move IntelligentPlayer :: move(TicTacToe g) {                 //getmove
    if(g.possible_moves().length() == 9) {
        
        srand(time(0));
        return {rand() % 10};
    }

    else
        return bestMove(g, getLetter());
}

int IntelligentPlayer :: utility(TicTacToe g) {
    return g.num_empty() + 1;
}

Move IntelligentPlayer :: bestMove(TicTacToe t, char player) {
    Move best;

    char maximizer = getLetter();

    char opp;
    if(player == 'X') opp = 'O';
    else opp = 'X';

    // to check if the last move was the winning move
    if (t.getWinner() == opp) {
        if(opp == maximizer){

            Move mv;
            mv.posn = -1;
            mv.util = utility(t);
            return mv;
        }
        else {

            Move mv;
            mv.posn = -1;
            mv.util = -utility(t);       //return Move{-1, -utility(t)};
            return mv;
        }
    }

    else if(t.num_empty() == 0) {
        Move mv;
        mv.posn = -1;
        mv.util = 0;
        return mv;      //return Move{-1, 0};
    }
    
    if (player == maximizer) {
        best.posn = -1;
        best.util = INT_MIN;   //we want to find max score
    }

    else {
        best.posn = -1;
        best.util = INT_MAX;   //we want to find min score
    }

    for(int i = 0; i < 3; i++)
        for(int j = 0; j < 3; j++) {

            if (t.gameBoard[i][j] == ' ') {
                t.putMove(3*i + j, player);

                Move simulated = bestMove(t, opp);

                t.gameBoard[i][j] = ' ';
                t.winner = ' ';
                simulated.posn = (3*i) + j;

                if(player == maximizer) {
                    if(simulated.util > best.util)
                        best = simulated;
                }
                else {
                    if(simulated.util < best.util)
                        best = simulated;
                }
            }
        }
    return best;
}

class HumanPlayer : public Player {

    public:
        HumanPlayer(char letterOfPlayer);

        Move move(TicTacToe g);
};

HumanPlayer :: HumanPlayer(char letterOfPlayer): Player(letterOfPlayer) {}

Move HumanPlayer :: move(TicTacToe g) {
    bool allowed;
    int posn;
    do {
        cout << "\nPlayer " << getLetter() << "'s turn. Choose where to make a move(0-8): ";
        cin >> posn;
        allowed = g.possible_moves().find(to_string(posn)) != string::npos; //g.possible_moves()

        if(!allowed)
            cout << "\nPlease enter a valid character. Please try again.\n";
    }while(!allowed);

    Move mv;
    mv.posn = posn;
    return mv;
}

void play_game(TicTacToe t, Player* player1, Player* player2) {
    
    Player* playerX;
    Player* playerO;

    char ch;
    int printChoice = 1;

    cout << "\nDo you want to see numbered positions on the board everytime it's printed?(Y/N)\n";
    cin >> ch;
    
    if(ch == 'n' | ch == 'N') {
        printChoice = 0;
        cout << "\n For your reference:\n";
        t.print_board(1);
    }

    if(player1 -> getLetter() == 'X'){
        playerX = player1;
        playerO = player2;
    }
    else{
        playerX = player2;
        playerO = player1;
    }

    char letter = 'X';
    Move mv;
    mv.posn = -1;
    mv.util = 0;

    while(t.num_empty() > 0) {
        if (letter == 'X')
            mv = playerX->move(t);
        else
            mv = playerO->move(t);
        
        if(t.putMove(mv.posn, letter)) {
            cout << "\nPlayer " << letter << " makes a move on position " << mv.posn << ".\n";

            t.print_board(printChoice);

            if(t.getWinner() != ' ') {
                cout << "\nPlayer " << t.getWinner() << " wins the game!\n";
                return;
            }

            if(letter == 'X') letter = 'O';
            else letter = 'X';
        }

        Sleep(1000);
    }

    cout << "\nThe game ends on a tie :) \n";
    return;
}

int main() {

    bool b = true;
    char ch;
    while(b) {

        //TicTacToe t1;

        cout << "\nMenu:\n";
        cout << "1. Play Against an AI\n";
        cout << "2. Play against another human\n";
        cout << "3. See 2 AIs battle it out!\n";
        
        cin >> ch;

        if(ch == '1') {
            char letter;
            cout << "Choose a Symbol(X or O only. X starts first): ";
            cin >> ch;
        
            Player *player1 = new HumanPlayer(toupper(ch));
            Player *player2 = new IntelligentPlayer(toupper(ch) == 'X' ? 'O' : 'X');
            play_game(TicTacToe(), player1, player2);
        }

        else if (ch == '2') {
            Player *player1 = new HumanPlayer('X');
            Player *player2 = new HumanPlayer('O');
            play_game(TicTacToe(), player1, player2);
        }

        else if(ch == '3') {
            Player *player1 = new IntelligentPlayer('X');
            Player *player2 = new IntelligentPlayer('O');
            play_game(TicTacToe(), player1, player2);
        }
        cout << "\nDo you want to play again(Y/N)?\n";
        cin >> ch;

        if(ch == 'n' | ch == 'N')
            b = false;
    }

    // Player *playerX = new intelligentPlayer('X');
    // Player *playerO = new humanPlayer('O');

    // TicTacToe().play_game(playerX, playerO);

    return 0;
}