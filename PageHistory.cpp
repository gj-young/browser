//
//  PageHistory.cpp
//  browser
//
//  Created by Gina Young on 9/26/18.
//  Copyright © 2018 Gina Young. All rights reserved.
//

#include "PageHistory.hpp"
#include "FileDisplayer.hpp"
#include <stack>
#include <iostream>
#include <set>
using std::stack;
using std::set;
using std::cout;

PageHistory::PageHistory()
: m_count(0)
{
}

PageHistory::~PageHistory()
{
    // Think this is the correct algo to deallocate stack of pointers -
    // Test later
    while (m_history.size() > 0)
    {
        FileDisplayer* nextPtr = m_history.top();
        delete nextPtr;
        m_history.pop();
    }
}

void PageHistory::addToHistory(string URL)
{
    if (m_count == 10)
    {
        deleteOldestPage();
    }
    FileDisplayer* newFD = new FileDisplayer(URL);
    m_history.push(newFD);
    m_count++;
}

void PageHistory::deleteOldestPage()
{
    // I think this works. Test it later // 
    FileDisplayer* temp[9];
    for (int i = 8; i >= 0 && !m_history.empty(); i--)
    {
        FileDisplayer* next = m_history.top();
        temp[i] = next;
        m_history.pop();
    }
    FileDisplayer* killMe = m_history.top();
    delete killMe;
    m_history.pop();
    
    for (int i = 0; i < 9; i++)
    {
        m_history.push(temp[i]);
    }
    m_count = 9;
}

FileDisplayer* PageHistory::goBack(bool& success)
{
    if (m_history.size() == 1)
    {
        success = false;
        return nullptr;
    }
    success = true;
    FileDisplayer* ptrToCurr = m_history.top();
    delete ptrToCurr;
    m_history.pop();
    FileDisplayer* ptrToBack = m_history.top();
    
    return ptrToBack;
}

bool PageHistory::addLinkToHistory(int linkNumOfCurrentPg)
{
    FileDisplayer* ptrToCurr = m_history.top();
    string URL = ptrToCurr->getURL(linkNumOfCurrentPg);
    if (URL != "")
    {
        addToHistory(URL);
        return true;
    }
    return false;
}

void PageHistory::displayCurrentPage()
{
    cout << CLEAR_CONSOLE;   // disable for xcode
    FileDisplayer* currPtr = m_history.top();
    if (currPtr != nullptr)
        currPtr->display();
}


