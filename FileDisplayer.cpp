//
//  FileDisplayer.cpp
//  browser
//
//  Created by Gina Young on 9/23/18.
//  Copyright © 2018 Gina Young. All rights reserved.
//

// Next step: Part 2 with stack
// Next step: Mouse input in console to click links


#include "FileDisplayer.hpp"
#include <fstream> // defines ofstream and ifstream
#include <iostream>
#include <string>
#include <cctype>
#include <vector>
using std::string;
using std::vector;
using std::ifstream;
using std::cerr;
using std::cout;
using std::endl;

FileDisplayer::FileDisplayer(string file)
{
    m_URL = file;
    m_links = 0;
}

FileDisplayer::~FileDisplayer()
{
    
}

void FileDisplayer::display()
{
    ifstream infile(m_URL.c_str());
    //ifstream infile("/Users/ginayg/Desktop/browser/build/products/debug/p2.html"); // replace later
    if (!infile)
    {
        cerr << "Error. Webpage file " << m_URL << " could not be opened." << endl;
        return;
    }

    char c;
    string oneWord;
    vector<string> containsWord;
    string tag;
    int charRemaining = MAX_COLS;
    
    // process html file until eof reached //
    while (infile.get(c))
    {
       oneWord = "";
       tag = "";
        containsWord.clear();
       
       // blank space or newline //
       if (c == ' ' || (c == '\n' && charRemaining < MAX_COLS ))
       {
           if (0 < charRemaining && charRemaining < MAX_COLS)
           {
               cout << " ";
               charRemaining--;
           }
           else if (charRemaining == MAX_COLS)
           {
               continue;
           }
           else
           {
               cout << endl;
               charRemaining = MAX_COLS;
           }
       }
       else if (c == '\n')
       {
           continue;
       }
       // tag //
       else if (c == '<')
       {
           bool lookForEndTag = false;
           bool firstTimeLoop = false;
           bool containsCenterTag = false;
           tag.push_back(c);

           while (infile.get(c))
           {
               tag.push_back(c);
               if (c == '>')
               {
                   if (tag.find("< CENTER >") != string::npos)
                   {
                       containsCenterTag = true;
                   }
                   for (int i = 1; i <tag.size(); i++)
                   {
                       if (isblank(tag[i]))
                       {
                           continue;
                       }
                       else if ((tag[i] == 'A' || containsCenterTag) && !firstTimeLoop)
                       {
                           firstTimeLoop = true;
                           lookForEndTag = true;
                           break;
                       }
                       else
                       {
                           firstTimeLoop = true;
                           lookForEndTag = false;
                           break;
                       }
                   }
                   if (!lookForEndTag)
                       break;
               }
           }
           vector<string> formatCodes;
           vector<string> printTag = processTag(tag, formatCodes,charRemaining);
           printItems(printTag, charRemaining, formatCodes);
       }
        // word //
       else if (isgraph(c))
       {
           oneWord.push_back(c);
           while (infile.get(c))
           {
               if (isgraph(c) && c != '<')
               {
                   oneWord.push_back(c);
               }
               else
               {
                   infile.putback(c);
                   break;
               }
           }
           containsWord.push_back(oneWord);
           vector<string> formatCode;
           formatCode.push_back("00");
           printItems(containsWord,charRemaining, formatCode);
       }
    }
    infile.close();
    m_links = 0;
    m_urls.empty();
    cout << endl;
}

vector<string> FileDisplayer::processTag(const string& tag, vector<string>& formatCodes, int remainingChar)
{
    vector<string> toBePrinted;
    string code; 
    vector<string> defaultFormat;
    defaultFormat.push_back("00");
    for (int i = 0; i < tag.size(); i++)
    {
        if (isalpha(tag[i]))  // assuming that code inside is only alphabetical
        {
            code.push_back(tag[i]);
            for (int j = i+1; j < tag.size() && tag[j] != ' '; j++)
            {
                code.push_back(tag[j]);
            }
            break;
        }
    }
    
    if (code == "P")
    {
        toBePrinted.push_back("\n\n");
        formatCodes = defaultFormat;
    }

    else if (code == "BR")
    {
        toBePrinted.push_back("\n");
        formatCodes = defaultFormat;
    }
    else if (code == "CENTER")
    {
        size_t textBeg = tag.find('>') + 1;
        while (isspace(tag[textBeg]))
        {
            textBeg++;
        }
        size_t textEnd = tag.find('<',textBeg) - 1;
        while (isspace(tag[textEnd]))
        {
            textEnd--;
        }
        size_t textLen = textEnd - textBeg +1;
        string text = tag.substr(textBeg,textLen);
        // just do case where centered text fits on current line for now //
        // do second case tomorrow //
        if (textLen > remainingChar)   // If it doesn't fit on current line, do calculation as if for new line
        {
            remainingChar = MAX_COLS;
            toBePrinted.push_back("\n");
        }

        int spacesUntilText= (remainingChar - static_cast<int>(textLen)) / 2;
        for (int i = 0; i < spacesUntilText; i++)
            toBePrinted.push_back(" ");
        
        string wordOfText;                     // duplicate code
        for (int i = 0; i < text.size(); i++)
        {
            if (isspace(text[i]))
            {
                toBePrinted.push_back(wordOfText);
                toBePrinted.push_back(" ");
                wordOfText = "";
            }
            else
            {
                wordOfText.push_back(text[i]);
            }
        }                                    
        toBePrinted.push_back(wordOfText);     // duplicate code ends
        toBePrinted.push_back("\n");
        size_t numCodesNeeded = toBePrinted.size();       // similar code
        for (int i = 0; i < numCodesNeeded; i++)
            formatCodes.push_back("00");        // similar code ends
    }
    else if (code == "A")
    {
        updateURLS(tag);
        
        char nextLinkNum = m_links + 1 + '0';
        toBePrinted.push_back("[0]");
        toBePrinted.push_back(" ");
        toBePrinted[0][1] = nextLinkNum;
        size_t hpTextPosBeg = tag.find('>') + 1;   // Correct use of size_t?
        while (isblank(tag[hpTextPosBeg]))
        {
            hpTextPosBeg++;
        }
        size_t hpTextPosEnd = tag.find('<', hpTextPosBeg) - 1;
        while (isblank(tag[hpTextPosEnd]))
        {
            hpTextPosEnd--;
        }
        size_t substrLen = hpTextPosEnd - hpTextPosBeg + 1;
        string hpText = tag.substr(hpTextPosBeg, substrLen);
        string wordOfHpText;
        
        for (int i = 0; i < hpText.size(); i++)
        {
            if (isspace(hpText[i]))
            {
                toBePrinted.push_back(wordOfHpText);
                toBePrinted.push_back(" ");
                wordOfHpText = "";
            }
            else
            {
                wordOfHpText.push_back(hpText[i]);
            }
        }
        toBePrinted.push_back(wordOfHpText);
        toBePrinted.push_back(" ");
        toBePrinted.push_back("[ ]");
        
        formatCodes.clear();
        size_t numCodesNeeded = toBePrinted.size();
        string formatCodeForA = "bu";
        for (int i = 0; i < numCodesNeeded; i++)
            formatCodes.push_back(formatCodeForA);
        
        m_links++;
    }
    return toBePrinted;
}

void FileDisplayer::printItems(const vector<string>& item, int& remainingChar, const vector<string>& formatCodes)
{
    // formatCode OTF (color)(underline y/n)
    // implement colorizing later - can't do with xcode
    
    for (int i = 0; i < item.size(); i++)
    {
        // disable this for xcode - only two options for now //
        string formatCode = formatCodes[i];
        int j = 0;
        if (formatCode[j] == 'b')
            cout << BLUE;
        j++;
        if (formatCode[j] == 'u')
            cout << UNDERLINE;
        // disable this for xcode - only two options for now //
        
        size_t numChar = item[i].size();
        if (item[i] == "\n")
        {
            cout << endl;
            remainingChar = MAX_COLS;
        }
        else if (item[i] == "\n\n")
        {
            cout << endl << endl;
            remainingChar = MAX_COLS;
        }
        else if (numChar <= remainingChar)
        {
            cout << item[i];
            remainingChar -= numChar;
        }
        else
        {
            cout << endl << item[i]; // deal with case when word longer than max width later
            remainingChar = MAX_COLS - static_cast<int>(numChar);
        }
        cout << RESET; // Disable for XCode
    }
}

void FileDisplayer::updateURLS(const string& Atag)
{
    size_t URLpos = Atag.find("HREF") + 6;  // deal with badly formed html later
    while (isspace(Atag[URLpos]) || Atag[URLpos] == '"')
        URLpos++;
    size_t URLend = Atag.find(">",URLpos) - 3;
    while (isspace(Atag[URLend]) || Atag[URLend] == '"')
        URLend--;
    size_t URLlength = URLend - URLpos + 1;
    string URL = Atag.substr(URLpos,URLlength);
    m_urls.push_back(URL);
}

string FileDisplayer::getURL(int linkNum)
{
    if (linkNum > m_urls.size())
    {
        return "";
    }
    return m_urls[linkNum-1];
}
