#include "htmlhelper.h"

HtmlHelper::HtmlHelper()
{
}

bool HtmlHelper::parse(const QByteArray &strHtml, int startPos)
{
    bool bParseOK = true;
    m_tagMap.clear();
    bool bInTag = false;
    bool bCloseTag = false;
    int lastOpenPos = 0;
    for(int charPos=startPos; charPos<strHtml.size() && bParseOK; charPos++) {
        char currentChar = strHtml[charPos];
        switch(currentChar) {
            case '<':
                bInTag = true;
                bCloseTag = charPos < strHtml.size()-2 && strHtml[charPos+1] == '/';
                lastOpenPos = charPos;
                break;
            case '>':
                if(bInTag) {
                    bInTag = false;
                    TagInfo tagInfo;
                    tagInfo.startPos = lastOpenPos;
                    tagInfo.endPos = charPos;
                    if(!bCloseTag) {
                        if(charPos > 0 && strHtml[charPos-1] == '/') {
                            tagInfo.tagType = open_close_tag;
                        }
                        else {
                            tagInfo.tagType = open_tag;
                        }
                    }
                    else {
                        tagInfo.tagType = close_tag;
                    }
                    m_tagMap[lastOpenPos] = tagInfo;
                }
                else {
                    bParseOK = false;
                }
        }
    }
    return bParseOK;
}

