#ifndef APPLICATION_H
#define APPLICATION_H

#include <bobcat_ui/all.h> 
#include <bobcat_ui/bobcat_ui.h> 
#include <bobcat_ui/menu.h> 
#include <vector> 
#include <string>

class Application: public bobcat::Application_{
bobcat::Window *mainWindow; 
bobcat::Window *gameWindow; 
bobcat::Window *statsWindow;

bobcat::Menu *mainMenu; 
bobcat::MenuItem *quitMenuItem; 
bobcat::MenuItem *gameMenuItem; 
bobcat::MenuItem *statsMenuItem;

bobcat::TextBox *mainTitle; 
bobcat::TextBox *mainInstructions;

bobcat::TextBox *gameTitle; 
bobcat::TextBox *gamePatternBox; 
bobcat::TextBox *gameInfoBox; 
std::vector<bobcat::Button *>gameKeyboard; 
bobcat::Button *gameNewRoundButton;

bobcat::TextBox *statsTitle;
std::vector<bobcat::TextBox*> statsRows;

std::vector<std::string> words;
std::string secretWord;
std::vector<char> guesses;
int maxGuesses; 
int guessCount; 
bool gameOver;




void handleMenuItemClick(bobcat::Widget *sender); 
void handleNewRoundClick(bobcat::Widget *sender); 
void handleKeyboardClick(bobcat::Widget *sender);

void readWordsFromFile(); 
void startNewRound(); 
std::string generatePattern(); 
bool letterAlreadyGuessed(char c); 
bool letterInWord(char c); 
bool checkWin(); 
void appendStatsRow(const std::string &word, int guessesUsed, bool won); 
void loadStatsFromCSV(); 
void initRow(const std::string &row, int y);

public:
    Application();

    friend struct ::AppTest;
};

#endif