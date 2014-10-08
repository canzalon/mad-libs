/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
Project: madlibs
Author: Christopher M. Anzalone		

Description: The madlib program accepts two files from the user, a story
file and dictionary file, which are used to produce a madlib story. At the 
start of the program, the user is prompted for the story filename, 
dictionary filename, and the name of the output file. The story file 
contains a story with certain words omitted and replaced with a word that 
represents a part of speech, inside brackets. The dictionary file contains 
a list of pairs of words. In each pair there is a key and a value. The key 
is any part of speech that correlates to the bracketed parts of speech 
scattered amongst the story file. This key acts as a reference for the 
value that it is associated with. The value is what is used to replace the 
bracketed part of speech shown in the story.  After both the dictionary 
and story files are input into the program, the story is read and 
simultaneously fixed with values from the dictionary file. The dictionary 
file is read through completely only once. In other words, when a key is 
being searched for, any keys found before the correct key are skipped over 
permanently. Therefore, bracketed words will remain intact if the order of 
the appearance of the bracketed words are not in the same order as they 
appear in the dictionary file. Upon completion, an output file of the 
resulting story is created and saved to the current directory. 
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

/*Preprocessor Directives*/
#include <iostream>
#include <fstream>
#include <string>
#include <ctype.h>
#include <vector>

using namespace std;

/*ADT for dictionary entries*/
class entry
{
	public:
	string key;
	string value;
};

//Function Prototypes start--------------------------------------

	//FUNCTION NAME: file_prompts							(#FP)
	//DESCRIPTION: Prompts the user for the filenames/paths of 
	//the story file and dictionary file, and the name of the 
	//output file.
	//PRECONDITIONS: A story text file and dictionary text file
	//must be present on the user's hard drive.
	//POSTCONDITIONS: A story data file and dictionary data file
	//have been uploaded into the program, and the name (and
	//perhaps location) of the output file has been established.
void file_prompts(ifstream&, ifstream&, ofstream&);

	//FUNCTION NAME: read_story								(#RS)
	//DESCRIPTION: Reads the data from the story file while
	//processing said data for bracketed words and their
	//dictionary match.
	//PRECONDITIONS: A story file and dictionary file have been
	//loaded into the program. 
	//POSTCONDITIONS: The story file has been read and 
	//processed.
void read_story(vector<string>&, vector<entry>&, vector<entry> &, ifstream&);

	//FUNCTION NAME: read_dictionary						(#RD)
	//DESCRIPTION: Reads the data from the dictionary file and
	//stores this data into a container object, and a copy is
	//made for validation checking.
	//PRECONDITIONS: A dictionary file must be loaded into the
	//program.
	//POSTCONDITIONS: The contents of the dictionary file have
	//been copied into two container objects.
void read_dictionary(vector<entry>&, vector<entry> &, ifstream&);

	//FUNCTION NAME: valid_key								(#VK)
	//DESCRIPTION: Checks the current bracketed-word being
	//examined to see if it is a possible valid key.
	//PRECONDITIONS: A dictionary and story file are loaded. A 
	//word within brackets is found in the bracketed word
	//process/function.
	//POSTCONDITIONS: A boolean value of either 1 or 0 has
	//been returned (0=false, 1=true).
bool valid_key(vector<entry>, string);

	//FUNCTION NAME: scan_dictionary						(#SD)
	//DESCRIPTION: Purpose is to scan the dictionary file for
	//a match to the word (key) in question.
	//PRECONDITIONS: A dictionary and story file have been
	//loaded into the program.
	//POSTCONDITIONS: The function has returned a value for the
	//matching key, if one exists.
string scan_dictionary(vector<entry>&, string);

	//FUNCTION NAME: bracketed_word							(#BW)
	//DESCRIPTION: Checks to see if the current word being read
	//in is surrounded by brackets, if it is it runs the
	//validation check. If the key is valid, it runs the 
	//dictionary scan. A word not surrounded by brackets is 
	//returned in its original form.
	//PRECONDITIONS: A dictionary and story file have been
	//loaded into the program.
	//POSTCONDITIONS: The current word being read, if it is
	//bracketed, has been identified as such, validated, and
	//replaced with its matching value from the dictionary. 
string bracketed_word(vector<entry> &, vector<entry> &, string &);

	//FUNCTION NAME: out_story								(#OS)
	//DESCRIPTION: Prints the data from the vector that holds the 
	//modified story to a new text file.
	//PRECONDITIONS: The user has loaded a story and dictionary
	//file into the program.
	//POSTCONDITIONS: The modified contents of the story have
	//been outputted to a text file in the current directory.
void out_story(vector<string>&, ofstream&); 

//Function Prototypes end----------------------------------------

int main()
{
	ifstream inStory;
	ofstream outStory;
	ifstream inDictionary;
	vector<entry> dictionary;
	vector<string> story;
	vector<entry> dictionary_copy;

	file_prompts(inStory, inDictionary, outStory);			//(#FP)

	read_dictionary(dictionary, dictionary_copy, inDictionary);	//(#RD)
	
	read_story(story, dictionary, dictionary_copy, inStory);	//(#RS)

	out_story(story, outStory);								//(#OS)

	return 0;
}

//Function Definitions start-------------------------------------

////////////////////////////////////////////////////////////
//FUNCTION NAME: file_prompts						(#FP)
//DESCRIPTION: Prompts the user for the filenames/paths of 
//the story file and dictionary file, and the name of the 
//output file.
//PRECONDITIONS: A story text file and dictionary text file
//must be present on the user's hard drive.
//POSTCONDITIONS: A story data file and dictionary data file
//have been uploaded into the program, and the name (and
//perhaps location) of the output file has been established.
////////////////////////////////////////////////////////////
void file_prompts(ifstream & inStory, ifstream & inDictionary, ofstream & outStory)
{
	string filename;

	//input story file
	cout<<"Enter the story filename: ";
	cin>>filename;
	inStory.open(filename.c_str());

	while (!inStory)
	{
		if (inStory.fail())	
		{
			inStory.clear();
			cout<<"\nFailed to open, please try again.\n"<<endl;
			cout<<"Enter the story filename: ";
			cin>>filename;
			inStory.open(filename.c_str());
		}
	}
	
	//input dictionary file
	cout<<"Enter the dictionary filename: ";
	cin>>filename;
	inDictionary.open(filename.c_str());
	while (!inDictionary)
	{
		if (inDictionary.fail())
		{	
			inDictionary.clear();
			cout<<"Failed to open, please try again.\n"<<endl;
			cout<<"Enter the dictionary filename: ";
			cin>>filename;
			inDictionary.open(filename.c_str());
		}
	}
	
	//input for output file
	cout<<"Enter the output filename: ";
	cin>>filename;
	outStory.open(filename.c_str());
}

////////////////////////////////////////////////////////////
//FUNCTION NAME: read_story							(#RS)
//DESCRIPTION: Reads the data from the story file while
//processing said data for bracketed words and their
//dictionary match.
//PRECONDITIONS: A story file and dictionary file have been
//loaded into the program. 
//POSTCONDITIONS: The story file has been read and 
//processed.
////////////////////////////////////////////////////////////
void read_story(vector<string> & story, vector<entry> & dictionary, vector<entry> & dictionary_copy, ifstream & inStory)
{
	string 
		filename,
		tempStory;
	string word;
	string newWord;
	string spaces=" ";
	string sentence = "";

	//reads story data and processes
	while (!inStory.eof())
	{
		inStory>>word;
		
		//single spaces after words, double spaces after '?' or '.'
		if (word[word.length()-1] == '?' || word[word.length()-1] == '.')
		{
			spaces = "  ";
		}
		else 
		{
			spaces = " ";
		}	
		
		//checkpoint for bracketed words
		newWord=bracketed_word(dictionary, dictionary_copy, word);	
		word=newWord;

		//structures strings to not exceed 70 char/line limit
		if ((sentence.length() + word.length() + spaces.length()) <= 70)
		{
			sentence = sentence + word + spaces;
		}
		else
		{	
			story.push_back(sentence);
			story.push_back("\n");
			sentence=word + " ";	

			//additional punctuation check required after process
			if (sentence[sentence.length()-1] == '.' || sentence[sentence.length()-1] == '?')
			{
				sentence = sentence + " " + " ";
			}
		}		
	}

	story.push_back(sentence);
	
	inStory.close();
}

////////////////////////////////////////////////////////////
//FUNCTION NAME: read_dictionary					(#RD)
//DESCRIPTION: Reads the data from the dictionary file and
//stores this data into a container object, and a copy is
//made for validation checking.
//PRECONDITIONS: A dictionary file must be loaded into the
//program.
//POSTCONDITIONS: The contents of the dictionary file have
//been copied into two container objects.
////////////////////////////////////////////////////////////
void read_dictionary(vector<entry> & dictionary, vector<entry> & dictionary_copy, ifstream & inDictionary)
{
	entry tempEntry;
	string 
		key, 
		value; 

	//reads data into container objects
	while (!inDictionary.eof())
	{
		inDictionary>>key>>value;
		tempEntry.key=key;
		tempEntry.value=value;
		dictionary.push_back(tempEntry);
	}

	//copy of dictionary for validity check
	dictionary_copy = dictionary;

	inDictionary.close();
}

////////////////////////////////////////////////////////////
//FUNCTION NAME: valid_key							(#VK)
//DESCRIPTION: Checks the current bracketed-word being
//examined to see if it is a possible valid key.
//PRECONDITIONS: A dictionary and story file are loaded. A 
//word within brackets is found in the bracketed word
//process/function.
//POSTCONDITIONS: A boolean value of either 1 or 0 has
//been returned (0=false, 1=true).
////////////////////////////////////////////////////////////
bool valid_key(vector<entry> dictionary_copy, string word) 
{
	//scans original dictionary for key validity
	for (unsigned int x=0; x < dictionary_copy.size(); x++)
	{
		if (dictionary_copy[x].key == word)
			return 1;
	}
	return 0;
}

////////////////////////////////////////////////////////////
//FUNCTION NAME: scan_dictionary					(#SD)
//DESCRIPTION: Purpose is to scan the dictionary file for
//a match to the word (key) in question.
//PRECONDITIONS: A dictionary and story file have been
//loaded into the program.
//POSTCONDITIONS: The function has returned a value for the
//matching key, if one exists.
////////////////////////////////////////////////////////////
string scan_dictionary(vector<entry> & dictionary, string tempWord)
{
	string wordNew;

	//scans dictionary for next instance of current key
	while (!dictionary.empty())
	{
		if (dictionary[0].key==tempWord)
		{
			wordNew=dictionary[0].value;
			for (unsigned int x=0; x < dictionary.size()-1; x++) 
			{													
				dictionary[x].key = dictionary[x+1].key;	
				dictionary[x].value = dictionary[x+1].value;
			}
			dictionary.pop_back();
			return wordNew;
		}
		else
		{
			for (unsigned int x=0; x < dictionary.size()-1; x++)
			{
				dictionary[x].key = dictionary[x+1].key;
				dictionary[x].value = dictionary[x+1].value;
			}
			dictionary.pop_back();	
		}
	}
	tempWord= "[" + tempWord + "]";	
	return tempWord;		
}

////////////////////////////////////////////////////////////
//FUNCTION NAME: bracketed_word						(#BW)
//DESCRIPTION: Checks to see if the current word being read
//in is surrounded by brackets, if it is it runs the
//validation check. If the key is valid, it runs the 
//dictionary scan. A word not surrounded by brackets is 
//returned in its original form.
//PRECONDITIONS: A dictionary and story file have been
//loaded into the program.
//POSTCONDITIONS: The current word being read, if it is
//bracketed, has been identified as such, validated, and
//replaced with its matching value from the dictionary.
////////////////////////////////////////////////////////////
string bracketed_word(vector<entry> & dictionary, vector<entry> & dictionary_copy, string & word)
{
	string 
		tempWord,
		valueNew=word,
		punct;

	//checks for brackets, validity, and new value
	if (word[0]=='[' && word[word.length()-1]==']')	
	{
		tempWord=word.substr(1,word.length()-2);

		if ((valid_key(dictionary_copy, tempWord)) == 1)
		{
			valueNew=scan_dictionary(dictionary, tempWord);	
			return valueNew;
		}
		else 
		{
			return word;
		}
	}
	else if (word[0]=='[' && word[word.length()-2]==']' && ispunct(word[word.length()-1]))	
	{
		tempWord=word.substr(1,word.length()-3);
		punct=word.substr(word.length()-1,1);

		if ((valid_key(dictionary_copy, tempWord)) == 1)
		{
			valueNew=scan_dictionary(dictionary, tempWord) + punct;	
			return valueNew;
		}
		else
		{
			return word;
		}
	}
	else 
	{
		return word;
	}
}

////////////////////////////////////////////////////////////
//FUNCTION NAME: out_story							(#OS)
//DESCRIPTION: Prints the data from the vector that holds the 
//modified story to a new text file.
//PRECONDITIONS: The user has loaded a story and dictionary
//file into the program.
//POSTCONDITIONS: The modified contents of the story have
//been outputted to a text file in the current directory.
////////////////////////////////////////////////////////////
void out_story(vector<string> & story, ofstream & outStory)
{
	//saves data into output file
	for (unsigned int x = 0; x < story.size(); x++)
	{
		outStory<<story[x];
	}

	outStory.close();
}

//Function Definitions end---------------------------------------