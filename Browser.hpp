//
//  Browser.hpp
//  browser
//
//  Created by Gina Young on 9/26/18.
//  Copyright © 2018 Gina Young. All rights reserved.
//

#ifndef Browser_hpp
#define Browser_hpp
#include "PageHistory.hpp"
#include <string>
using namespace std;

class Browser
{
public:
    Browser(string firstPg);
    ~Browser();
    bool goBack();
    bool goToURL(string url);
    bool goToLink(int linkNum);
private:
    PageHistory m_history;
};

#endif /* Browser_hpp */
