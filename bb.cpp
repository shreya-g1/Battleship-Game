//bb.cpp
//Shreya Gowda, CISP 400
//12/18/19

#include <iostream>
#include <string>
#include <cstdlib>
#include <cctype>
#include <iomanip>
#include <fstream>
using namespace std;

// https://www.alt-codes.net/square-symbols

//defined types to substitute for numbers for readability later on
typedef enum {CARRIER, BATTLESHIP, CRUISER, SUBMARINE, DESTROYER, NUM_SHIPS} ship_type;
typedef enum {GRID1, GRID2} grid_type;
typedef enum {COMPUTER, HUMAN} player_type;


class InvalidInput{
  //class which checks for invalid input and throws exception if necessary
  public:
  class OutOfRange{};
  class BadLength{};

  void checkRange(int min, int max, int data){
    if(data<min||data>max){
      throw OutOfRange();
    }
  }

  void checkLength(int len, int data){
    if(data!=len){
      throw BadLength();
    }
  }
};

// Specification A3 - Date Class in Program Greeting
class Date{
  //class which can set and display date 
  private:
    int day, month, year;
    time_t today;

  public:
    Date(){
      day=0;
      month=0;
      year=0;
      today=time(NULL);
    }

    void setNow(){
      tm* date_info = localtime(&today); 
      day=date_info->tm_mday;
      month=(date_info->tm_mon)+1;
      year=(date_info->tm_year)+1900;
    }

    friend ostream & operator << (ostream &out, const Date &info) { 
  
      out << setw(2) << setfill('0') << info.month; 

      out << "/" << setw(2) << setfill('0') << info.day; 

      out << "/" << setw(4) << setfill('0') << info.year <<endl;
    
      return out; 
    } 
};

// Specification C1 - OOP
class Player{
  //class which can hold, manipulate, and display player's grid
  private:
  static const int ROWS=10;  //number rows in grid
  static const int COLS=10;  //number columns in grid
  static const int MIN_ROWS_COLS=0;  //min number of rows and columns in a grid
  static const int UI_LEN=2; //user input length
  int SHIP_SIZES[int(NUM_SHIPS)]={5,4,3,3,2};  //size of each ship in grid
  InvalidInput check;  //object to check for invalid input
  string** grid;       //player's grid

  public:
  //constructor
  Player(){
    //initializing 2D grid
    grid= new string*[ROWS];
    for(int i=0; i<COLS; i++){
      grid[i]=new string[COLS];
    }
  }

  //exceptions
  class BadShot{};

  void setGrid0(){
    //one type of grid. grid is set in this function
    
    //all array spaces are set to hollow square character to signify "empty"
    for(int i=0; i<ROWS; i++){
      for(int j=0; j<COLS; j++){
        grid[i][j]="□";
      }
    }

    //sets position of carrier in grid. signified by "A" character in array.
    for(int i=0; i<SHIP_SIZES[int(CARRIER)]; i++){
      grid[1][i+1]="A";
    }

    //sets position of battleship. signified by "B" char.
    for(int i=0; i<SHIP_SIZES[int(BATTLESHIP)]; i++){
      grid[i+3][1]="B";
    }

    //sets position of cruiser. signified by "C" char.
    for(int i=0; i<SHIP_SIZES[int(CRUISER)]; i++){
      grid[i+6][6]="C";
    }

    //sets position of submarine. signified by "S" char.
    for(int i=0; i<SHIP_SIZES[int(SUBMARINE)]; i++){
      grid[4][i+4]="S";
    }

    //sets position of destroyer. signified by "D" char.
    for(int i=0; i<SHIP_SIZES[int(DESTROYER)]; i++){
      grid[2][i+7]="D";
    }
  }

  void setGrid1(){
    //second type of grid. grid is set in this function

    //all array spaces are set to hollow square character to signify "empty"
    for(int i=0; i<ROWS; i++){
      for(int j=0; j<COLS; j++){
        grid[i][j]="□";
      }
    }

    //sets position of carrier in grid. signified by "A" character in array.
    for(int i=0; i<SHIP_SIZES[int(CARRIER)]; i++){
      grid[i+1][1]="A";
    }

    //sets position of battleship. signified by "B" char.
    for(int i=0; i<SHIP_SIZES[int(BATTLESHIP)]; i++){
      grid[8][i+3]="B";
    }

    //sets position of cruiser. signified by "C" char.
    for(int i=0; i<SHIP_SIZES[int(CRUISER)]; i++){
      grid[6][i+6]="C";
    }

    //sets position of submarine. signified by "S" char.
    for(int i=0; i<SHIP_SIZES[int(SUBMARINE)]; i++){
      grid[i+4][4]="S";
    }

    //sets position of destroyer. signified by "D" char.
    for(int i=0; i<SHIP_SIZES[int(DESTROYER)]; i++){
      grid[2][i+7]="D";
    }
  
  }

  void shootSquare(string user_input){
    //if an array space is shot at by user, reassigns the character at that array space to signify that it has been shot

    int ui1=0; //user input column
    int ui2=0; //user input row

    //checks that user input is only two characters (for column and row)
    check.checkLength(UI_LEN, user_input.length());

    //changes user input into numbers
    ui1=int(user_input[0])-65;
    ui2=stoi(user_input.substr(1));
    //checks that user input is within valid range of values
    check.checkRange(MIN_ROWS_COLS, ROWS, ui2);
    check.checkRange(MIN_ROWS_COLS, COLS, ui1);


    if(grid[ui2][ui1]=="▣"||grid[ui2][ui1]=="☒"){
      // Specification A1 - Adv Input Validation
      throw BadShot();
    }
    else if(grid[ui2][ui1]=="A"||grid[ui2][ui1]=="B"||grid[ui2][ui1]=="C"||grid[ui2][ui1]=="S"||grid[ui2][ui1]=="D"){
      //if user hits array space with ship
      grid[ui2][ui1]="▣";
    }
    else{
      //if user misses ship
      grid[ui2][ui1]="☒";
    }
  }

  void shootSquare(int i, int j){
  //if an array space is shot at by computer, reassigns the character at that array space to signify that it has been shot
    if(grid[i][j]=="▣"||grid[i][j]=="☒"){
      // Specification C2 - Prohibit AI wasted shots
      throw BadShot();
    }
    else if(grid[i][j]=="A"||grid[i][j]=="B"||grid[i][j]=="C"||grid[i][j]=="S"||grid[i][j]=="D"){
      //if computer hits array space with ship
      grid[i][j]="▣";
    }
    else{
      //if comp misses ship
      grid[i][j]="☒";
    }
  }

  int checkShips(){
    //checks to see how many ships are left

    bool contains_ship[NUM_SHIPS]; //for checking if the grid contains certain type of ship
    int num_ships=0;  //to count number of ships at end
   
   //no ships found on grid yet, so set all to false
    for(int i=0; i<int(NUM_SHIPS); i++){
      contains_ship[i]=false;
    }

    //checks to see if any ship type is still afloat by checking for signifying character in array
    for(int i=0; i<ROWS; i++){
      for(int j=0; j<COLS; j++){
        if(grid[i][j]=="A"){
          contains_ship[CARRIER]=true;
        }
        if(grid[i][j]=="B"){
          contains_ship[BATTLESHIP]=true;
        }
        if(grid[i][j]=="C"){
          contains_ship[CRUISER]=true;
        }
        if(grid[i][j]=="S"){
          contains_ship[SUBMARINE]=true;;
        }
        if(grid[i][j]=="D"){
          contains_ship[DESTROYER]=true;
        }
      }
    }

    //adds up ships afloat
    if(contains_ship[CARRIER]==true){
      num_ships++;
    }
    if(contains_ship[BATTLESHIP]==true){
      num_ships++;
    }
    if(contains_ship[CRUISER]==true){
      num_ships++;
    }
    if(contains_ship[SUBMARINE]==true){
      num_ships++;
    }
    if(contains_ship[DESTROYER]==true){
      num_ships++;
    }

    return num_ships;
  }

  void printFullGrid(){
    //prints player's grid fully (shows ship location)

    //column label
    cout<<"  A B C D E F G H I J"<<endl;
    for(int i=0; i<ROWS; i++){
      //row label
      cout<<i<<" ";
      for(int j=0; j<COLS; j++){
        //if a ship is in array location, prints colored square
        if(grid[i][j]=="A"||grid[i][j]=="B"||grid[i][j]=="C"||grid[i][j]=="S"||grid[i][j]=="D"){
          cout<<"■"<<" ";
        }
        //if no ship, prints hollow square
        else{cout<<grid[i][j]<<" ";}
      }
      cout<<endl;
    }

  }

  void printHiddenGrid(){
    //prints player's grid w/o showing ship location

    //column label
    cout<<"  A B C D E F G H I J"<<endl;
    for(int i=0; i<ROWS; i++){
      //row label
      cout<<i<<" ";
      for(int j=0; j<COLS; j++){
        //if a ship is in array location, prints hollow square
        if(grid[i][j]=="A"||grid[i][j]=="B"||grid[i][j]=="C"||grid[i][j]=="S"||grid[i][j]=="D"){
          cout<<"□"<<" ";
        }
        //if no ship, still prints hollow square
        else{cout<<grid[i][j]<<" ";}
      }
      cout<<endl;
    }
  }

  //Specification B2 - Log file to Disk
  void logFullGrid(ofstream* log){
    //logs player's grid fully (shows ship location)

    //column label
    *log<<"  A B C D E F G H I J"<<endl;
    for(int i=0; i<ROWS; i++){
      //row label
      *log<<i<<" ";
      for(int j=0; j<COLS; j++){
        //if a ship is in array location, logs colored square
        if(grid[i][j]=="A"||grid[i][j]=="B"||grid[i][j]=="C"||grid[i][j]=="S"||grid[i][j]=="D"){
          *log<<"■"<<" ";
        }
        //if no ship, logs hollow square
        else{*log<<grid[i][j]<<" ";}
      }
      *log<<endl;
    }

  }

  void logHiddenGrid(ofstream* log){
    //logs player's grid w/o showing ship location

    //column label
    *log<<"  A B C D E F G H I J"<<endl;
    for(int i=0; i<ROWS; i++){
      //row label
      *log<<i<<" ";
      for(int j=0; j<COLS; j++){
        //if a ship is in array location, logs hollow square
        if(grid[i][j]=="A"||grid[i][j]=="B"||grid[i][j]=="C"||grid[i][j]=="S"||grid[i][j]=="D"){
          *log<<"□"<<" ";
        }
        //if no ship, still logs hollow square
        else{*log<<grid[i][j]<<" ";}
      }
      *log<<endl;
    }
  }

  //destructor
  ~Player(){
    //deletes 2d grid
    for(int i=0; i<COLS; i++){
      delete [] grid[i];
    }
    delete grid;
  }

};

//Function Prototypes
string opt_validation();
void instructions();
void humanTurn(Player* player1, Player* player2, ofstream* log);
void computerTurn(Player* player1, Player* player2, ofstream* log);

int main() {
  srand(time(0));
  grid_type gridnum=grid_type(0);  
  player_type turn=player_type(rand()%2);
  string opt="";
  bool multifire=false;
  Player* human=new Player;
  Player* computer=new Player;
  string winner="";
  Date now;
  ofstream logfile("log.txt");
  
  //Program Greeting
  cout<<"Welcome to Battleship Game!"<<endl;
  now.setNow();
  cout<<now;
  instructions();

  logfile<<"Battleship Game Record"<<endl;
  
  // Specification B3 - Random Start
  gridnum=grid_type(rand()%2);
  if(gridnum){
    human->setGrid1();
    computer->setGrid0();
  }
  else{
    human->setGrid0();
    computer->setGrid1();
  }

  //prompts user for game mode
  cout<<"Multifire? (Y/N)"<<endl;
  opt=opt_validation();
  if(opt=="Y"){
    multifire=true;
    logfile<<endl<<"Multifire Mode"<<endl;
  }

  cout<<"Play Game? (Y/N) "<<endl;

  // Specification A2 - MultiFire Game
  while(opt!="N"&&multifire==true&&winner==""){
    opt=opt_validation();

    if(human->checkShips()==0){
      winner="COMPUTER";
    }
    if(computer->checkShips()==0){
      winner="HUMAN";
    }

    // Specification C3 - Secret Option
    if(opt=="X"){
      cout<<endl;
      computer->printFullGrid();
      cout<<endl;
      human->printHiddenGrid();
      cout<<endl;
      cout<<endl<<"Continue? (Y/N)"<<endl;
    }

    if(opt=="Y"){
      if(turn==HUMAN){
        for(int i=0; i<human->checkShips(); i++){
          humanTurn(human, computer, &logfile);
        }
        human->printFullGrid();
        cout<<endl;
        computer->printHiddenGrid();
        turn=COMPUTER;
      }
      else{
        for(int i=0; i<computer->checkShips(); i++){
          computerTurn(human, computer, &logfile);
        }
        turn=HUMAN;
      }

      cout<<endl<<"Continue? (Y/N)"<<endl;
    }  

  }

  while(opt!="N"&&multifire==false&&winner==""){
    opt=opt_validation();

    if(human->checkShips()==0){
      winner="COMPUTER";
    }
    if(computer->checkShips()==0){
      winner="HUMAN";
    }
    
    if(opt=="X"){
      cout<<endl;
      computer->printFullGrid();
      cout<<endl;
      human->printHiddenGrid();
      cout<<endl;
      cout<<endl<<"Continue? (Y/N)"<<endl;
    }

    if(opt=="Y"){
      if(turn==HUMAN){
        humanTurn(human, computer, &logfile);
        human->printFullGrid();
        cout<<endl;
        computer->printHiddenGrid();
        turn=COMPUTER;
      }
      else{
        computerTurn(human, computer, &logfile);
        turn=HUMAN;
      }
      cout<<endl<<"Continue? (Y/N)"<<endl;
    }  

  }

  cout<<endl;
  if(winner==""){cout<<"No one won.";}
  else{cout<<winner<<" won.";}
  cout<<" Thanks for playing battleship."<<endl;

  delete human;
  delete computer;
}

void instructions(){
  //gives user instructions on how to play game
  cout<<"Rules:"<<endl;
  cout<<"You will be playing against the computer. Each of you has a grid with 5 ships on it:"<<endl;
  cout<<" CARRIER (size-- 5 blocks) \n BATTLESHIP (size-- 4 blocks) \n CRUISER (size-- 3 blocks) \n SUBMARINE (size-- 3 blocks) \n DESTROYER (size-- 2 blocks)"<<endl;
  cout<<"The objective of the game is to sink all 5 ships by correctly guessing the position of the ship tiles. If you guess a ship tile correctly, it will be displayed as '▣' on the opponent's grid. If you guess incorrectly, the tile will display '☒'. Whoever sinks all the ships on the opponent's grid first, wins."<<endl;
  cout<<endl<<"You will be prompted as to whether you would like to play the multifire or normal version of the game. Normal means each player gets one shot each turn. Multifire means the number of shots you get each round is determined by the number of ships you still have afloat."<<endl<<endl;
}

string opt_validation(){
  //validates user input in menu and other prompts
  const int OPT_LEN=1;
  string user_choice="";
  bool bad_input=false;

  do{
    bad_input=false;
    cout<<"enter: ";
    getline(cin, user_choice);
    user_choice[0]=toupper(user_choice[0]);
    cout<<endl;
    if(user_choice.length()!=OPT_LEN){
      cout<<"Invalid Input. Input 1 character, no space."<<endl;
      bad_input=true;
    }
    if(user_choice!="Y"&&user_choice!="N"&&user_choice!="X"){
      cout<<"Invalid Input. Type valid letter (Y or N)."<<endl;
      bad_input=true;
    }
  }while(bad_input);

  return user_choice;
}

void humanTurn(Player* player1, Player* player2, ofstream* log){
  //runs when it is human's turn to shoot

  bool bad_input;
  string opt="";
  time_t log_time = time(0);

  cout<<endl<<"Your Turn"<<endl<<endl;
  *log<<endl<<"Human Turn"<<endl;
  *log<<ctime(&log_time)<<endl<<endl;
  player1->printFullGrid();
  player1->logFullGrid(log);
  cout<<endl;
  *log<<endl;
  player2->printHiddenGrid();
  player2->logHiddenGrid(log);

  cout<<endl<<"Map square, (ex: A0)? "<<endl;
  // Specification B1 - Validate Input
  do{
    bad_input=false;
    cout<<"enter:";
    getline(cin, opt);
    try{
      player2->shootSquare(opt);
    }
    catch(InvalidInput::BadLength){
      cout<<endl<<"Invalid Input. Input 2 characters, no space."<<endl;
      bad_input=true;
    }
    catch(std::invalid_argument){
      cout<<endl<<"Invalid Row. Type number."<<endl;
      bad_input=true;
    }
    catch(InvalidInput::OutOfRange){
      cout<<endl<<"Row/Column Out of Range (A-J, 0-9)."<<endl;
      bad_input=true;
    }
    catch(Player::BadShot){
      cout<<endl<<"Already Fired on That Square! Pick another. "<<endl;
      bad_input=true;
    }
  }while(bad_input);

}

void computerTurn(Player* player1, Player* player2, ofstream* log){
  //runs when it is computer's turn to shoot
  bool bad_input=false;
  int rand_col=0;
  int rand_row=0;
  time_t log_time = time(0);

  cout<<endl<<"Computer's Turn"<<endl<<endl;
  *log<<endl<<"Computer Turn"<<endl;
  *log<<ctime(&log_time)<<endl<<endl;

  do{
    bad_input=false;
    rand_col=rand()%10;
    rand_row=rand()%10;
    try{
      player1->shootSquare(rand_row, rand_col);
    }
    catch(Player::BadShot){
      bad_input=true;
    }
  }while(bad_input);

  cout<<"Computer hit "<<char(rand_col+65)<<rand_row<<endl<<endl;
  *log<<"Computer hit "<<char(rand_col+65)<<rand_row<<endl<<endl;

  player1->printFullGrid();
  player1->logFullGrid(log);
  cout<<endl;
  *log<<endl;
  player2->printHiddenGrid();
  player2->logHiddenGrid(log);
}
