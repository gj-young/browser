//
//  PageHistory.hpp
//  browser
//
//  Created by Gina Young on 9/26/18.
//  Copyright © 2018 Gina Young. All rights reserved.
//

#ifndef PageHistory_hpp
#define PageHistory_hpp

#include <stack>
#include <string>
class FileDisplayer;
using std::stack;
using std::string;

class PageHistory
{
public:
    PageHistory();
    ~PageHistory();
    void addToHistory(string URL);
    FileDisplayer* goBack(bool& success);
    void displayCurrentPage();
    bool addLinkToHistory(int linkNumOfCurrentPg);
    void deleteOldestPage();
private:
    stack<FileDisplayer*> m_history;
    int m_count;
};


#endif /* PageHistory_hpp */
