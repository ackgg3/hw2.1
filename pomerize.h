#ifndef POMERIZE
#define POMERIZE

#include <vector>
#include <string>
#include <ios>

using namespace std;

//copied from http://www.beautiful-love-quotes.com/roses-are-red-poems.html

class pomerize {
	private:
		int stanza;
		vector<string> line1;
		vector<string> line2;
		vector<string> line3;
		
	public: 
		pomerize() {
			line1.push_back("Roses are red, violets are blue,");
			
			line2.push_back("Use one of these poems");
			line2.push_back("The center of my world");
			line2.push_back("It's you and me together");
			line2.push_back("The way that you kiss me");
			line2.push_back("I've never experienced");
			line2.push_back("You will always be");
			line2.push_back("Call it what you want to, but");
			line2.push_back("I just want you to know that"); 
			line2.push_back("I'm seeing a future filled with");
			line2.push_back("What I feel in my heart");
			line2.push_back("The depth of my love?"); 
			line2.push_back("You don't always smell good, but");
			line2.push_back("We have nothing in common, so");
			line2.push_back("Clinging vines are almost as bad as");
			line2.push_back("You have a great sense of humor that has");
			line2.push_back("I'm telling you I love you, so");
			line2.push_back("We go together like");
			line2.push_back("I want to have sex now,"); 
			line2.push_back("I already said I was sorry, so"); 
			line2.push_back("Sure there are others,"); 
			line2.push_back("You are stupid,");

			line3.push_back("as a love message from you");
			line3.push_back("begins and ends with you.");
			line3.push_back("a lifetime of love, just for two.");
			line3.push_back("....Whew!");
			line3.push_back("a love so true.");
			line3.push_back("my special Boo.");
			line3.push_back("I need to be with you.");
			line3.push_back("I think the world of you.");
			line3.push_back("me and you.");
			line3.push_back("is wonderful and new.");
			line3.push_back("If you only knew....... ");
			line3.push_back("I still love you.");
			line3.push_back("baby we're through.");
			line3.push_back("super glue.");
			line3.push_back("me hooked on you.");
			line3.push_back("what are you gonna do?");
			line3.push_back("my clothes match with my shoes. ");
			line3.push_back("don't you think I've paid my dues?");
			line3.push_back("why continue to stew? ");
			line3.push_back("not many more than two. ");
			line3.push_back("he/she is using you.");
			
			stanza = 0;
		}
		
		string getLine() {
			stanza++;
			if (stanza == 4) {
				stanza = 1;
			}
			int randLine = 0;
			string toReturn = "Unknown stanza state!";
			switch (stanza) {
				case 1:
					randLine = (rand() % line1.size());
					toReturn = line1[randLine];
					break;
				case 2:
					randLine = (rand() % line2.size());
					toReturn =  line2[randLine];					
					break;
				case 3:
					randLine = (rand() % line3.size());
					toReturn =  line3[randLine];					
					break;
			}
			return toReturn;
		}
};

#endif