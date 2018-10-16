//
//  Browser.cpp
//  browser
//
//  Created by Gina Young on 9/26/18.
//  Copyright © 2018 Gina Young. All rights reserved.
//

#include "Browser.hpp"
#include "FileDisplayer.hpp"
#include <string>
using namespace std;

Browser::Browser(string firstPg)
{
    m_history.addToHistory(firstPg);
    m_history.displayCurrentPage();
}

Browser::~Browser()
{
    
}

bool Browser::goBack()
{
    bool backAvailable = false;
    m_history.goBack(backAvailable);
    if (backAvailable)
    {
        m_history.displayCurrentPage();
    }
    return backAvailable;
}

bool Browser::goToURL(string url)
{
    m_history.addToHistory(url);
    m_history.displayCurrentPage();
    return true; // temp for now
}

bool Browser::goToLink(int linkNum)
{
    bool goodLink = m_history.addLinkToHistory(linkNum);
    if (goodLink)
    {
        m_history.displayCurrentPage();
        return true;
    }
    else
        return false;
}
