//
//  browser.cpp
//  browser
//
//  Created by Gina Young on 9/23/18.
//  Copyright © 2018 Gina Young. All rights reserved.
//

#include "FileDisplayer.hpp"
#include "PageHistory.hpp"
#include "Browser.hpp"
#include <iostream>
#include <cstdlib>
#include <sstream>
using namespace std;

int main(int argc, char * argv[])
{
    string firstPg(argv[1]);
    Browser mySession(firstPg);
    while (true)
    {
        string userInput;
        cout << "Enter b, URL, link number, or q to quit: ";
        getline(cin,userInput);
        bool isLinkNumber = false;
        bool successfulAction = false;
        
        for (int i = 0; i < userInput.size(); i++)
        {
            if (isdigit(userInput[i]))
                isLinkNumber = true;
            else
            {
                isLinkNumber = false;
                break;
            }
        }
        if (userInput == "q")
        {
            exit(0);
        }
        else if (userInput == "b")
        {
            successfulAction = mySession.goBack();
            if (!successfulAction)
            {
                cout << "Back not available. Try again. " << endl;
            }
        }
        else if (isLinkNumber)
        {
            stringstream ss(userInput);
            int linkN;
            ss >> linkN;
            
            successfulAction = mySession.goToLink(linkN);
            if (!successfulAction)
            {
                cout << "Link not available. Try again. " << endl;
            }
        }
        else
        {
            successfulAction = mySession.goToURL(userInput);
            if (!successfulAction)
            {
                cout << "URL not found. Try again. " << endl;
            }
        }
    }
}
