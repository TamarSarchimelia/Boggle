/*
 * File: Boggle.cpp
 * ----------------
 * Name: [TODO: enter name here]
 * Section: [TODO: enter section leader here]
 * This file is the main starter file for Assignment #4, Boggle.
 * [TODO: extend the documentation]
 */

#include <iostream>
#include "gboggle.h"
#include "grid.h"
#include "gwindow.h"
#include "lexicon.h"
#include "random.h"
#include "simpio.h"
#include "strlib.h"
#include "random.h"
#include "vector.h"
#include "lexicon.h"
#include "set.h"
using namespace std;

/* Constants */

const int BOGGLE_WINDOW_WIDTH = 650;
const int BOGGLE_WINDOW_HEIGHT = 350;

const string STANDARD_CUBES[16]  = {
    "AAEEGN", "ABBJOO", "ACHOPS", "AFFKPS",
    "AOOTTW", "CIMOTU", "DEILRX", "DELRVY",
    "DISTTY", "EEGHNW", "EEINSU", "EHRTVW",
    "EIOSST", "ELRTTY", "HIMNQU", "HLNNRZ"
};
 
const string BIG_BOGGLE_CUBES[25]  = {
    "AAAFRS", "AAEEEE", "AAFIRS", "ADENNN", "AEEEEM",
    "AEEGMU", "AEGMNN", "AFIRSY", "BJKQXZ", "CCNSTW",
    "CEIILT", "CEILPT", "CEIPST", "DDLNOR", "DDHNOT",
    "DHHLOR", "DHLNOR", "EIIITT", "EMOTTT", "ENSSSU",
    "FIPRSY", "GORRVW", "HIPRRY", "NOOTUW", "OOOTTU"
};

/* Function prototypes */

static Lexicon lexicon("EnglishWords.dat");

void welcome();
void giveInstructions();
int greedChoose();
string boardConfiguration(int greed);
string computerGreed(int greed);
void drawConfiguration(string configuration, int greed);
void gamerPlay(string configuration,int greed);
bool wordCheck(string word, string configuration, int greed);
bool charcheck(string word,string configuration);
bool findword(string word, string configuration,int  greed,string place);
bool placecheck(string place, string last);
void paintWord(string place);
bool find(string word, string configuration,int greed, int row, int colomn,string place);
void findAllword(Set<string> &me,int greed,string configuration,Set<string> &words, string word,string place);

/* Main program */

int main() {
    GWindow gw(BOGGLE_WINDOW_WIDTH, BOGGLE_WINDOW_HEIGHT);
    initGBoggle(gw);
    welcome();
	string newGame = "yes";
	while(newGame == "yes"){  
		int greed = greedChoose();
		string configuration = boardConfiguration(greed);
		drawConfiguration(configuration,greed);
		gamerPlay(configuration,greed);
		newGame = toLowerCase(getLine("do you want play again? "));
	}
	gw.close();

    return 0;
}

/*
 * Function: welcome
 * Usage: welcome();
 * -----------------
 * Print out a cheery welcome message.
 */

void welcome() {
    cout << "Welcome!  You're about to play an intense game ";
    cout << "of mind-numbing Boggle.  The good news is that ";
    cout << "you might improve your vocabulary a bit.  The ";
    cout << "bad news is that you're probably going to lose ";
    cout << "miserably to this little dictionary-toting hunk ";
    cout << "of silicon.  If only YOU had a gig of RAM..." << endl << endl;
	string instructionBool = getLine("Do you need instructions?");
	while(toLowerCase(instructionBool)!="yes"&&toLowerCase(instructionBool)!="no"){
		cout<<"wrote only yes or no"<<endl;
		instructionBool = getLine("Do you need instructions?");
	}
	if(toLowerCase(instructionBool)=="yes"){
		giveInstructions();
	}
	
}

/*
 * Function: giveInstructions
 * Usage: giveInstructions();
 * --------------------------
 * Print out the instructions for the user.
 */

void giveInstructions() {
    cout << endl;
    cout << "The boggle board is a grid onto which I ";
    cout << "I will randomly distribute cubes. These ";
    cout << "6-sided cubes have letters rather than ";
    cout << "numbers on the faces, creating a grid of ";
    cout << "letters on which you try to form words. ";
    cout << "You go first, entering all the words you can ";
    cout << "find that are formed by tracing adjoining ";
    cout << "letters. Two letters adjoin if they are next ";
    cout << "to each other horizontally, vertically, or ";
    cout << "diagonally. A letter can only be used once ";
    cout << "in each word. Words must be at least four ";
    cout << "letters long and can be counted only once. ";
    cout << "You score points based on word length: a ";
    cout << "4-letter word is worth 1 point, 5-letters ";
    cout << "earn 2 points, and so on. After your puny ";
    cout << "brain is exhausted, I, the supercomputer, ";
    cout << "will find all the remaining words and double ";
    cout << "or triple your paltry score." << endl << endl;
    cout << "Hit return when you're ready...";
    getLine();
}

// [TODO: Fill in the rest of the code]
// this code chooses big or small greed
int greedChoose(){
	cout<<"You can choose standard Boggle(4X4 grid)"<<endl;
	cout<<"or Big Boggle(5X5 grid)"<<endl;
	string choose = getLine("Would you like Big Boggle? ");
	if(toLowerCase(choose)!="no"&&toLowerCase(choose)!="yes"){
		cout<<"write yes or no"<<endl;
		choose = getLine("Would you like Big Boggle? ");
		cout<<endl;
	}
	if(toLowerCase(choose)=="no"){
		drawBoard(4,4);
		return 4;
	}else{
		drawBoard(5,5);
		return 5;
	}
}
//this code asks a question which greed do you want. random or in your mind.
string boardConfiguration(int greed){
	cout<<"I'll give you a chance to set up the board to your specification,"<<endl;
	cout<<"which makes it easier to confirm your boggle program is working."<<endl;
	string boolConfiguration = getLine("Do you want to force the board configuration?");
	if(toLowerCase(boolConfiguration)!="yes"&&toLowerCase(boolConfiguration)!="no"){
		cout<<"please enter yes or no"<<endl;
		boolConfiguration = getLine("Do you want to force the board configuration?");
	}
	if(toLowerCase(boolConfiguration)=="yes"){
		cout<<"Enter a "<<greed*greed<<"-character string to indetify which letters you want on the cubes."<<endl;
		cout<<"The firs "<<greed<<" letter are the cubes on the top row from left to right,"<<endl;
		cout<<"the next "<< greed<<" letters are the second row, and so on."<<endl;
		string letters = getLine("enter the string: ");
		while(letters.size()!=greed*greed){
			cout<<"String must include "<<greed*greed;
			letters = getLine(" characters! Try again: ");
		}
		return letters;
	}else{
		string letters = computerGreed(greed);
		return letters;
	}
}
// this code randoms configuration of greed
string computerGreed(int greed){
	string letters = "";
		Vector<string> cubes;
		for(int i = 0 ; i<greed*greed; i++){
			if(greed==4){
				cubes.add(STANDARD_CUBES[i]);
			}else{
				cubes.add(BIG_BOGGLE_CUBES[i]);
			}
			
		}
		for(int i=0; i<greed*greed; i++){
			int row = randomInteger(i,greed*greed-1);
			int colomn = randomInteger(0,5);
			string st = cubes[row];
			char ch = st[colomn];
			letters = letters  + ch;
			if(row!=i){
				string stringAtI = cubes.get(colomn);
				string stringAtRandom = cubes.get(row);
				cubes[i] = stringAtRandom;
				cubes[row] = stringAtI;
			}
		}
	return letters;
}
// this code draws greed configuration

void drawConfiguration(string configuration, int greed){
	int at = 0;
	for(int i = 0; i<greed; i++){
		for(int j= 0; j<greed;j++){
			labelCube(i,j,configuration[at]);
			at++;
		}
	}
}
// here is the body of game play. in the top of the code is players code and then computers code
void gamerPlay(string configuration,int greed){
	Set<string> me;
	cout<<"Ok, take all the time you want and find all the words you can!"<<endl;
	cout<<"Signal that you're finished by entering an empty line."<<endl<<endl;
	string word = getLine("Enter a word: ");
	while(word.size()!=0){
		while(word.size()<=3){
			cout<<"you need to write more than 3 chars"<<endl;
			word = getLine("Enter a word: ");
		}
		if(lexicon.contains(word)){
		
		if(wordCheck(word, configuration,greed)){
			if(me.contains(word)){
				cout<<"new word please"<<endl;
			}else{
				me.add(word);
				recordWordForPlayer(word,HUMAN);
				drawConfiguration(configuration, greed);
			}
		}
		}else{
			cout<<"That's not a word"<<endl;
		}
		word = getLine("Enter a word: ");
	}
	cout<<" computer time "<<endl;
	Set<string> words;
	for(int i=0; i <configuration.size(); i++){
		int row  = i/greed +1 ;
		int colomn = i%greed+1;
		string word = "";
		char ch = configuration.at(i);
		word = word+ch;
		findAllword(me,greed,configuration,words,word,integerToString(row*10+colomn));
	}
	return;

}
// here is small word check to check chars in the configurations. and than to find can find this word in the greed or not
bool wordCheck(string word, string configuration, int greed){
	string place = "";
	if(!charcheck(toLowerCase(word),toLowerCase(configuration))) {
		cout<<"You can't make that word!"<<endl;
		return false;
	}
	if(findword(toLowerCase(word),toLowerCase(configuration), greed,place)) {
		return true;
	}else{
		cout<<"You can't make that word!"<<endl;
		return false;
	}
}
// this is small code to see if in the greed is all chars of word
bool charcheck(string word, string configuration){
	word = toLowerCase(word);
	configuration = toLowerCase(configuration);
	if(word.size()==0){
		return true;
	}
	if(configuration.find(word.at(0))!=string::npos){
		charcheck(word.substr(1),configuration);
		return true;
	}else{
		return false;
	}
}
//this code find if players word can be in the greed or not
bool findword(string word,string configuration,int greed,string place){
	bool find = true;
	string copy = configuration;
	if(word.size()==0){
		paintWord(place);
		return true;
	}
	while(copy.find(word.at(0))!=string::npos){
		int sumOfPlace = copy.find(word.at(0));
		copy[sumOfPlace]='@';
		configuration[sumOfPlace]='@';
		int row  = sumOfPlace/greed +1 ;
		int colomn = sumOfPlace%greed+1;
		if(colomn==0) {
			colomn = greed;
		}
		if(place.size()==0){
			place = place + integerToString(row*10+colomn);
			return findword(word.substr(1),configuration,greed,place);
			place = "";
		}else{
			int lastplace = stringToInteger(place.substr(place.size()-2));
			int lastColomn = lastplace%10;
				int lastRow = lastplace/10;
			if((lastColomn-1==colomn||lastColomn==colomn||lastColomn+1==colomn)&&(lastRow-1==row||lastRow==row||lastRow+1==row)){
				if(!placecheck(place,integerToString(row*10+colomn))){
					row = row*10+colomn;
					find = find && findword(word.substr(1),configuration,greed,place+integerToString(row));
					if(find==true) return true;
					return find;
				}
			}
		}
		configuration[sumOfPlace]=word.at(0);
	}
	return find;
	return false;
}
// this code check if the place(current char) was or wasn't used
bool placecheck(string place, string last){
	if(place.size()==0) return true; 
	bool t = place.at(0)==last.at(0);
	bool tq = place.at(1)==place.at(1);
	if(place.at(0)==last.at(0)&&place.at(1)==last.at(1))return false; 
	if(place.size()==2)return true; 	
	return placecheck(place.substr(2),last);
}
// this code paints chars that was used for player word
void paintWord(string place){
	if(place.size()==0) return;
	int colomn = place.at(1) - 49;
	int row = place.at(0) - 49;
	highlightCube(row,colomn,true);
	paintWord(place.substr(2));

}

// here is recursion to find all words in the greed. this code also sees if player alredy use the word.
void findAllword(Set<string> &me,int greed,string configuration,Set<string> &words, string word,string place){
	string copy = "";
	if(word.size()==greed*greed)return;
	if(place.size()==2){
		copy = place;
	}else{
		copy = place.substr(place.size()-2);
	}
	int row = copy.at(0)-49;
	int colomn = copy.at(1)-49;
	for(int i=0;i<3;i++){
		for(int j=0;j<3;j++){
			if(row+i>0&&colomn+j>0&&row+i<=greed&&colomn+j<=greed){
				char ch =  configuration.at((row+i-1)*greed+colomn+j-1);
				int placeforsecond = (row+i)*10+colomn+j;
				string st = integerToString(placeforsecond);
				if(placecheck(place,st)){
					if(lexicon.containsPrefix(word+ch)){
						if(word.size()>2){
							if(lexicon.contains(word+ch)){
								if(!me.contains(word+ch)){
									if(!words.contains(word+ch)){
									words.add(word+ch);
									recordWordForPlayer(word+ch,COMPUTER);
									}
								}
							}
						}
						findAllword(me,greed,configuration,words,word+ch,place + integerToString((row+i)*10+colomn+j));
						
					}
				}
			}
		}
	}
}
