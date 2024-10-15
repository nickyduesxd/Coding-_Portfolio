//Nicholas Zayfman 257050
//Help Recieved: None.
//Add libraries.
#include <iostream>
#include <unistd.h>
#include <string>
using namespace std;
//Function prototypes
void userInput(int*);
void shuffle(int*);
void printCard(int*, int);
void printHand(int*, int*);
void gameSetup(int*, int*, int*);
void dealerTurn(int, int*, int*, int*);
void playerTurn(int, int*, int*, int*);
int score(int*);
bool endGame(int*, int*);
int* newDeck();
void playGame(int*, int*, int*);
int winner(int*, int*);
bool ifTie(int*, int*, int*);
int playRound();
bool welcome();
bool withdrawFromTable();
void bankBalance(double, double);
//Main Function.
int main(){
  double moneyputin = 0;
  double overallbalance = 0;
  if(welcome() == true){
    bool d = true;
    while(d || withdrawFromTable() == true){
      cout << "You are putting 5$ in" << endl;
      moneyputin = moneyputin + 5;
      int x = playRound();
      if(x == 1){
        overallbalance = overallbalance;
      }
      else if(x == 2){
        overallbalance = overallbalance + 7.5;
      }
      else{
        overallbalance = overallbalance + 10;
      }
      bankBalance(overallbalance, moneyputin);
      d = false;
    }
    cout << "Thank you for playing." << endl;
    cout << "Come again soon." << endl;
    return 0;
  }
  else{
    cout << "Thank you for stopping by." << endl;
    return 0;
  }
}
//Gives the player the choice to withdraw once the first two cards are dealt.
bool withdrawfromgame(){
  string choice;
  cout << "Do you wish to withdraw from game? [yes, no]" << endl;
  cin >> choice;
  if(choice == "yes"){
    return true;
  }
  else{
    return false;
  }
}
//Prints to console if the user is up money, down money or breaking even.
void bankBalance(double overallbalance, double moneyputin){
  double x = overallbalance - moneyputin;
  if(x > 0){
    cout << "You are currently up by $" << x << endl;
  }
  else if(x < 0){
    cout << "You are currently down by $" << -1*x << endl;
  }
  else{
    cout << "You are currently breaking even." << endl;
  }
}
//Welcome message to the user. Asks if they want to play.
bool welcome(){
  string choice;
  cout << "Welcome to the table." << endl;
  cout << "Do you want to be dealt in?" << endl;
  cout << "[yes | no]" << endl;
  cin >> choice;
  if(choice == "yes"){
    return true;
  }
  else if(choice == "no"){
    return false;
  }
  return false;
}
//Gives the user the choice to end the program and leave the table.
bool withdrawFromTable(){
  string choice;
  cout << "Do you wish to withdraw from the table?" << endl;
  cout << "[yes | no]" << endl;
  cin >> choice;
  if(choice == "no"){
    return true;
  }
  else if(choice == "yes"){
    return false;
  }
  return false;
}
//Plays one game.
int playRound(){
  //Creating the Deck of Cards
  int *Deck;
  Deck = newDeck();
  //Asks user for how they want the Deck to be shuffled.
  userInput(Deck);
  //Creates the Player's Hand.
  int *Player;
  Player = new int[26];
  //Creates the Dealer's Hand.
  int *Dealer;
  Dealer = new int[26];
    playGame(Deck, Player, Dealer);
    if(Player[0]==0){
      return 2;
    }
    while(ifTie(Deck, Player, Dealer)){
      for(int i =0; i<26; i++){
        Player[i] = 0;
        Dealer[i] = 0;
      }
      Deck = newDeck();
      shuffle(Deck);
      playGame(Deck, Player, Dealer);
      if(Player[0]==0){
        return 2;
      }
    }
    if (winner(Player, Dealer)==0){
      return 1;
    }
    else{
      return 0;
    }
}
//Checks to see if the game has ended with logic.
bool endGame(int* Player, int* Dealer){
  // Ends game if either the player or Dealer goes over 21 or there is a tie.
  if((score(Player)>21)||(score(Dealer)>21)||
  (((score(Player)==21))&&(score(Dealer) == 21))){
    return true;
  }
  //Ends game if two consecutive stand moves.
  else{
    for(int i = 0; i < 26; i++){
      for(int j = 0; j < 27; j++){
        if((Player[i]==-1)&&(Dealer[j]==-1)){
          return true;
        }
      }
    }
    return false;
  }
}
//Checks if there is a tie.
bool ifTie(int* Deck, int* Player, int* Dealer){
  if(endGame(Player, Dealer)==true){
    if((score(Player)==score(Dealer))){
      return true;
    }
    else{
      return false;
    }
  }
  else{
    return false;
  }
}
//Identifies the winner givin both hands with logic. Prints game result to the
//console.
int winner(int* Player, int* Dealer){
  if(endGame(Player, Dealer)==true){
    if((score(Player)<=21)&&((score(Dealer)<=21)&&((score(Player)!=score(Dealer))))){
      if(score(Player)>score(Dealer)){
        cout << "Player wins" << endl;
        return 1;
      }
      else{
        cout << "Dealer wins" << endl;
        return 0;
      }
    }
    else if(score(Player)>21){
      cout << "Player busts, dealer wins" << endl;
      return 0;
    }
    else if(score(Dealer)>21){
      cout << "Dealer busts, player wins" << endl;
      return 1;
    }
    else{
      return -1;
    }
  }
  return -1;
}
//Actual playing of the game.
void playGame(int* Deck, int* Player, int* Dealer){
  gameSetup(Deck, Player, Dealer);
  //Asks if they want to leave once they see their first two cards.
  if(withdrawfromgame() == true){
    Player[0] = 0;
  }
  else{
    for(int i = 1; endGame(Player, Dealer)==false ; i++){
      if(endGame(Player, Dealer)== false){
        playerTurn(i, Player, Dealer, Deck);
      }
      if(endGame(Player, Dealer)==false){
        dealerTurn(i, Player, Dealer, Deck);
      }
    }
  }
}
//Creates a new deck of cards.
int* newDeck(){
    //Assigns cardvalue to each part of the Array.
    int* Deck = new int[52];
    int i =0;
    while(i<52){
        for(int j = 1; j < 5; j++){
            for(int k = 2; k < 15; k++){
                Deck[i] = k + j*100;
                i++;
            }
         }
    }
    return Deck;
}
//Uses the card values to give a score for either the player or dealer's hand.
int score(int* Hand){
  int score = 0;
  for(int i = 0; i < 26; i++){
    int x = Hand[i]%100;
    if((x>1)&&(x<11)){
      score = score + x;
    }
    else if((x>10)&&(x<14)){
      score = score + 10;
    }
    else if(x == 14){
      score = score + 11;
    }
  }
  for(int j = 0; j < 26; j++){
    int y = Hand[j]%100;
    if((score>21)&&(y==14)){
      score = score - 10;
    }
  }
  return score;
}
//Asks user for input on how the deck should be shuffled.
void userInput(int* Deck){
  char c;
  cout << "Shuffle: [n | s | u <seed>]: ";
  cin >> c;
  if(c== 's'){
    srand(time(0));
    shuffle(Deck);
  }
  else if(c== 'u'){
    int k;
    cin >> k;
    srand(k);
    shuffle(Deck);
  }
}
//Shuffles the deck based on user input.
void shuffle(int* Deck){
  for(int i = 0; i<52; i++){
    int j = rand() % 52;
    //Swap.
    int f = Deck[i];
    Deck[i] = Deck[j];
    Deck[j] = f;
  }
}
//Prints card to console given card value.
void printCard(int* Deck, int count){
     int x = Deck[count] %100;
     if((x < 10)&&(x>1)){
       cout << " " << x;
     }
     else if(x == 10){
       cout << x;
     }
     else if(Deck[count]==-1){
           cout << "   ";
     }
     else{
       if(x == 11){
       cout << " J";
       }
     else if(x == 12){
       cout << " Q";
       }
     else if(x == 13){
       cout << " K";
       }
     else if(x == 14){
       cout << " A";
       }
     else if(x==0){
       cout << "   ";
       }
     }
     int y = Deck[count] / 100;
     if(y==1){
       cout << "\u2663";
     }
     else if(y ==2){
       cout << "\u2666";
     }
     else if(y==3){
       cout << "\u2665";
     }
     else if(y==4){
       cout << "\u2660";
     }
}
//Prints both the hand of the Player and Dealer in the proper format.
void printHand(int* Player, int* Dealer){
   bool p = false;
   bool d = false;
   system("clear");
   cout << " Player Dealer" << endl;
   if(endGame(Player, Dealer)==false){
        for(int i = 0; (((Player[i]!=0)&&(Player[i]!=-1))||((Dealer[i]!=0)&&(Dealer[i]!=-1))); i++){
                cout << "| ";
                printCard(Player, i);
                cout << "  | ";
                if(i==0){
                    cout << " **";
                }
                else{
                    printCard(Dealer, i);
                }
                cout << "  |" << endl;
         }
   }
   else{
    for(int i = 0; (((Player[i]!=-1)&&(Player[i]!=0))||((Dealer[i]!=-1)&&(Dealer[i]!=0))); i++){
     cout << "| ";
     printCard(Player, i);
     cout << "  | ";
     printCard(Dealer, i);
     cout << "  |" << endl;
   }
   }

}
//First two cards are dealt out. Starts the game.
void gameSetup(int* Deck, int* Player, int* Dealer){
    Player[0] = Deck[0];
    Dealer[0] = Deck[1];
    Player[1] = Deck[2];
    Dealer[1] = Deck[3];
    for(int i = 0; i < 4; i++){
        Deck[i] = 0;
    }
    printHand(Player, Dealer);
}
//Player input in the game- hit or stand. Code below processess and makes
//changes accordingly.
void playerTurn(int round, int* Player, int*Dealer, int* Deck){
    int l = 0;
    char c;
    cout << "Round " << round << " Player's turn" << endl;
    cout << "hit or stand? [h/s] ";
    cin >> c;
    bool d = true;
    if(c == 'h'){
      for(int i = 0; i< 26; i++){
        for(int j = 0; (j < 52)&&d; j++){
          if(((Player[i]==0)||(Player[i]==-1))&&(Deck[j]!=0)&&d){
                Player[i] = Deck[j];
                Deck[j] = 0;
                d = false;
                break;
          }
        }
      }
    }
    else if(c == 's'){
      for(int i = 0; (i < 26)&&d; i++){
        if((Player[i]==0)){
          Player[i] = -1;
          d = false;
        }
      }
    }
    printHand(Player, Dealer);
}
//Dealer algorithm for choice- hit or stand based on logic. Also displays the
//decision to the console for the User to see.
void dealerTurn(int round, int* Player, int* Dealer, int* Deck){
    char c;
    cout << "Round " << round << " Dealer's turn" << endl;
    cout << "hit or stand? [h/s] ";
    if(score(Dealer)<17){
      c = 'h';
      cout << c << endl;
    }
    else{
      c = 's';
      cout << 's'<< endl;
    }
    sleep(2);
    bool d = true;
    if(c == 'h'){
      for(int i = 0; i< 26; i++){
        for(int j = 0; j < 52; j++){
          if((Dealer[i]==0)&&(Deck[j]!=0)&&d){
            Dealer[i] = Deck[j];
            Deck[j] = 0;
            d = false;
          }
        }
      }
    }
    else if(c == 's'){
      for(int i = 0; i < 26; i++){
        if((Dealer[i]==0)&&d){
          Dealer[i] = -1;
          d = false;
        }
      }
    }
    printHand(Player, Dealer);
}
