#include <Application.h>
#include <bobcat_ui/bobcat_ui.h>
#include <fstream>
#include <string>
#include <vector>
#include <RandomSupport.h>

using namespace std;
using namespace bobcat;

static const string STATS_FILE = "stats.csv";

Application::Application(){
    srand((unsigned int)time(0));
    maxGuesses = 6;
    guessCount = 0;
    gameOver = false;

 
    ofstream statsFile(STATS_FILE, ios::trunc);
    statsFile.close();

 
    mainWindow = new Window(100, 100, 600, 400, "Word Game");

    mainMenu = new Menu();
    quitMenuItem  = new MenuItem("File/Exit");
    gameMenuItem  = new MenuItem("Game/Play");
    statsMenuItem = new MenuItem("Game/Statistics");

    mainMenu->addItem(quitMenuItem);
    mainMenu->addItem(gameMenuItem);
    mainMenu->addItem(statsMenuItem);

    mainTitle = new TextBox(0, 40, 600, 30, "Letter Guess Game");
    mainTitle->align(FL_ALIGN_CENTER);
    mainTitle->labelfont(FL_BOLD);
    mainTitle->labelsize(24);

    mainInstructions = new TextBox(50, 100, 500, 200,"");
    mainInstructions->align(FL_ALIGN_INSIDE | FL_ALIGN_TOP_LEFT);

 
    mainWindow->end();

  
    gameWindow = new Window(150, 150, 600, 400, "Play Game");

    gameTitle = new TextBox(0, 20, 600, 30, "Play: Letter Guess Game");
    gameTitle->align(FL_ALIGN_CENTER);
    gameTitle->labelfont(FL_BOLD);
    gameTitle->labelsize(20);

    gamePatternBox = new TextBox(100, 80, 400, 30, "");
    gamePatternBox->align(FL_ALIGN_CENTER);
    gamePatternBox->labelsize(20);

    gameInfoBox = new TextBox(100, 120, 400, 40, "");
    gameInfoBox->align(FL_ALIGN_INSIDE | FL_ALIGN_TOP_LEFT);

    initRow("QWERTYUIOP", 200);
    initRow("ASDFGHJKL",  240);
    initRow("ZXCVBNM",    280);

    gameNewRoundButton = new Button(250, 330, 100, 25, "New Round");

    gameWindow->end();

    
    statsWindow = new Window(200, 200, 600, 400, "Statistics");

    statsTitle = new TextBox(0, 20, 600, 30, "Game Statistics");
    statsTitle->align(FL_ALIGN_CENTER);
    statsTitle->labelfont(FL_BOLD);
    statsTitle->labelsize(20);

    int y = 70;
    for (int i = 0; i < 10; i++){
        TextBox *row = new TextBox(50, y, 500, 25, "");
        row->align(FL_ALIGN_INSIDE | FL_ALIGN_LEFT);
        statsRows.push_back(row);
        y += 25;
    }

    statsWindow->end();

    mainWindow->show();
    mainInstructions->label(
    "Instructions:\n"
    "- Use the Game menu to open Play or Statistics.\n"
    "- Guess word based on description\n"
    "- You have six guess for each round.\n"
    "- After each round, your result is saved.\n"
    "- Your score is saved in Statistics. Enjoy!"
);


    ON_CLICK(quitMenuItem, Application::handleMenuItemClick);
    ON_CLICK(gameMenuItem, Application::handleMenuItemClick);
    ON_CLICK(statsMenuItem, Application::handleMenuItemClick);
    ON_CLICK(gameNewRoundButton, Application::handleNewRoundClick);
}


void Application::startNewRound() {
    static string testWords[] = {"CAT", "DOG", "SUN", "MAP"};
    static string descriptions[] = {
        "A small feline that purrs",
        "I love playing fetch!",
        "I am the light in your morning, I blaze high in the sky spreading light",
        "Oh no! You are lost. What are you going to use to get home"
    };
    static int numWords = 4;
    static int currentIndex = 0;
    static bool shuffled = false;

   
    if (!shuffled){
        for (int i = 0; i < numWords; i++){
            int j = rand() % numWords;
            swap(testWords[i], testWords[j]);
            swap(descriptions[i], descriptions[j]);
        }
        shuffled = true;
    }

    
    if (words.empty()){
        secretWord = testWords[currentIndex];

        if (gameInfoBox != nullptr)
            gameInfoBox->label(descriptions[currentIndex].c_str());

        currentIndex++;
        if (currentIndex >= numWords) currentIndex = 0;
    } else {
        int upper = static_cast<int>(words.size()) - 1;
        RNG rng(0, upper);
        secretWord = words[rng.get()];

        if (gameInfoBox != nullptr)
            gameInfoBox->label("Guess the word!");
    }

    
    for (size_t i = 0; i < secretWord.size(); i++){
        secretWord[i] = static_cast<char>(toupper(static_cast<unsigned char>(secretWord[i])));
    }

   
    guesses.clear();
    guessCount = 0;
    gameOver = false;

    if (gamePatternBox != nullptr)
        gamePatternBox->label(generatePattern().c_str());

    gameWindow->redraw();
}




void Application::handleKeyboardClick(Widget *sender){
    if (gameOver) return;

    char letter = sender->label()[0];
    if (letterAlreadyGuessed(letter)){
        showMessage("You already guessed that...");
        return;
    }

    guesses.push_back(letter);
    guessCount++;

    if (letterInWord(letter)){
        if (checkWin()){
            showMessage("Congratulations! You won!");
            appendStatsRow(secretWord, guessCount, true);
            gameOver = true;
        }
    } else {
        if (guessCount >= maxGuesses){
            showMessage("You lost! Word was: " + secretWord);
            appendStatsRow(secretWord, guessCount, false);
            gameOver = true;
        }
    }

    if (gamePatternBox != nullptr)
        gamePatternBox->label(generatePattern().c_str());
}


void Application::handleMenuItemClick(Widget *sender){
    if (sender == quitMenuItem){
        exit(0);
    } else if (sender == gameMenuItem){
        gameWindow->set_modal();
        gameWindow->show();
        startNewRound();
    } else if (sender == statsMenuItem){
        statsWindow->set_modal();
        statsWindow->show();
        loadStatsFromCSV();
    }
}


string Application::generatePattern(){
    string pattern = "";
    for (size_t i = 0; i < secretWord.size(); i++){
        char c = secretWord[i];
        if (letterAlreadyGuessed(c)){
            pattern += c;
        } else {
            pattern += "_";
        }
        if (i < secretWord.size() - 1) pattern += " ";
    }
    return pattern;
}


bool Application::letterAlreadyGuessed(char c){
    for (char g : guesses) if (g == c) return true;
    return false;
}

bool Application::letterInWord(char c){
    for (char w : secretWord) if (w == c) return true;
    return false;
}

bool Application::checkWin(){
    for (char w : secretWord) if (!letterAlreadyGuessed(w)) return false;
    return true;
}


void Application::handleNewRoundClick(Widget *sender){
    startNewRound();
}


void Application::appendStatsRow(const string &word, int guessesUsed, bool won){
    ofstream file(STATS_FILE, ios::app);
    if (!file.is_open()) return;

    file << word << "," << guessesUsed << "," << (won ? "WIN" : "LOSE") << "\n";
    file.close();
}

void Application::loadStatsFromCSV(){
    ifstream file(STATS_FILE);
    if (!file.is_open()){
        for (TextBox* row : statsRows) row->label("");
        return;
    }

    string line;
    int row = 0;
    while (getline(file, line) && row < (int)statsRows.size()){
        statsRows[row]->label(line.c_str());
        row++;
    }
    file.close();
}


void Application::initRow(const string &row, int y){
    int startX = 200;
    int width = 400;
    int rowWidth = static_cast<int>(row.length() * 30 - 5);
    int start = startX + (width - rowWidth) / 2;

    for (size_t i = 0; i < row.size(); i++){
        string letter(1, row[i]);
        gameKeyboard.push_back(new Button(start + i * 30, y, 25, 25, letter));
        ON_CLICK(gameKeyboard.back(), Application::handleKeyboardClick);
    }
}








