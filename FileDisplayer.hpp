//
//  FileDisplayer.hpp
//  browser
//
//  Created by Gina Young on 9/23/18.
//  Copyright © 2018 Gina Young. All rights reserved.
//

#ifndef FileDisplayer_hpp
#define FileDisplayer_hpp
#include <string>
#include <queue>
#include <vector>
using std::string;
using std::queue;
using std::vector;

#define RESET   "\033[0m"
#define BLACK   "\033[30m"      // Black
#define BLUE    "\033[34m"      // Blue
#define UNDERLINE "\033[4m"     // Underline
#define CLEAR_CONSOLE "\33c\e[3J"   // Clear console to go to another webpage

const int MAX_COLS = 40;

class FileDisplayer
{
public:
    FileDisplayer(string file);
    ~FileDisplayer();
    void display();
    string getURL(int linkNum);
private:
    string m_URL;
    vector<string> processTag(const string& tag, vector<string>& formatCodes, int remainingChar);
    void printItems(const vector<string>& item, int& remainingChar, const vector<string>& formatCodes);
    int m_links;   // might be redundant 
    vector<string> m_urls;
    void updateURLS(const string& Atag);
};

#endif /* FileDisplayer_hpp */
